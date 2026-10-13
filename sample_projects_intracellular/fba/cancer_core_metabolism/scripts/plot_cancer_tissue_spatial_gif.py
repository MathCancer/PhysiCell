#!/usr/bin/env python3
"""
GIF of cancer-tissue frames at 1-hour outputs.

Modes
-----
1. Single spatial field (default ``--only growth_rate``) — same look as
   ``growth_rate_*.svg``.
2. ``--uptake-stack`` — one column, one row per substrate, each row a full
   spatial uptake map like ``glycine_uptake_48.0h.svg``.
3. ``--substrate-profiles`` — stacked line plots of microenvironment
   concentration vs distance from vessel (y-mean at midplane z).

Examples
--------
    python scripts/plot_cancer_tissue_spatial_gif.py

    python scripts/plot_cancer_tissue_spatial_gif.py --uptake-stack

    python scripts/plot_cancer_tissue_spatial_gif.py \\
        --only '' --uptake-stack --duration-ms 250

    python scripts/figure_generation.py --time-point 72 --uptake-stack
"""

from __future__ import annotations

import argparse
import io
import sys
from pathlib import Path
from typing import Optional

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from PIL import Image

import figure_paths  # noqa: E402

figure_paths.ensure_paths()

from pctk import multicellds  # noqa: E402
from plot_cells_spatial import plot_cells_spatial  # noqa: E402
from plot_cancer_tissue_spatial import (  # noqa: E402
    _DEFAULT_STYLE,
    _DROP_COLS,
    _figure_specs,
    _parse_only,
    read_cells,
)

# Default rows in the spatial uptake stack (stems from _figure_specs)
_DEFAULT_STACK_STEMS = (
    "oxygen_uptake",
    "glucose_uptake",
    "glutamine_uptake",
    "glycine_uptake",
    "lactate_secretion",
)

# Microenvironment names (match PhysiCell output column labels)
_DEFAULT_SUBSTRATE_PROFILES = (
    "O2",
    "D-Glucose",
    "L-Glutamine",
    "Glycine",
    "L-Lactate",
)

_SUBSTRATE_COLORS = {
    "O2": "navy",
    "D-Glucose": "#f1c232",
    "L-Glutamine": "indigo",
    "Glycine": "hotpink",
    "L-Lactate": "firebrick",
}

_SUBSTRATE_YLABELS = {
    "O2": r"$O_2$ (mM)",
    "D-Glucose": "Glucose (mM)",
    "L-Glutamine": "Gln-L (mM)",
    "Glycine": "Glycine (mM)",
    "L-Lactate": "Lactate (mM)",
}


def _fig_to_rgba(fig, *, tight: bool = True, pad_inches: float = 0.04) -> np.ndarray:
    buf = io.BytesIO()
    if tight:
        fig.savefig(
            buf, format="png", dpi=110, bbox_inches="tight", pad_inches=pad_inches
        )
    else:
        # Fixed canvas so stacked rows share the same pixel width / axes alignment
        fig.savefig(buf, format="png", dpi=110, bbox_inches=None, pad_inches=0)
    plt.close(fig)
    buf.seek(0)
    return np.asarray(Image.open(buf).convert("RGBA"))


def _pad_to_common_width(panels: list[np.ndarray], *, align: str = "left") -> list[np.ndarray]:
    """Pad panels to a shared width so vertical stacks stay column-aligned."""
    w = max(p.shape[1] for p in panels)
    out = []
    for p in panels:
        if p.shape[1] == w:
            out.append(p)
            continue
        canvas = np.full((p.shape[0], w, 4), 255, dtype=np.uint8)
        canvas[..., 3] = 255
        if align == "right":
            canvas[:, w - p.shape[1] :] = p
        else:
            canvas[:, : p.shape[1]] = p
        out.append(canvas)
    return out


def _trim_right_whitespace(arr: np.ndarray, *, pad: int = 36) -> np.ndarray:
    """Drop trailing nearly-white columns, keeping ``pad`` px after content."""
    rgb = arr[..., :3]
    content = np.any(rgb < 250, axis=2)
    cols = np.where(content.any(axis=0))[0]
    if cols.size == 0:
        return arr
    right = min(arr.shape[1], int(cols[-1]) + 1 + max(pad, 0))
    return arr[:, :right]


def _pad_to_common_size(frames: list[np.ndarray]) -> list[np.ndarray]:
    h = max(f.shape[0] for f in frames)
    w = max(f.shape[1] for f in frames)
    out = []
    for f in frames:
        canvas = np.full((h, w, 4), 255, dtype=np.uint8)
        canvas[..., 3] = 255
        canvas[: f.shape[0], : f.shape[1]] = f
        out.append(canvas)
    return out


def _stack_vertical(panels: list[np.ndarray], gap: int = 6) -> np.ndarray:
    """Stack RGBA panels into one column; all panels must share the same width."""
    w = panels[0].shape[1]
    if any(p.shape[1] != w for p in panels):
        raise ValueError(
            "Stack panels have mismatched widths: "
            + ", ".join(str(p.shape[1]) for p in panels)
        )
    if gap > 0 and len(panels) > 1:
        spacer = np.full((gap, w, 4), 255, dtype=np.uint8)
        spacer[..., 3] = 255
        parts: list[np.ndarray] = []
        for i, p in enumerate(panels):
            parts.append(p)
            if i < len(panels) - 1:
                parts.append(spacer)
        return np.concatenate(parts, axis=0)
    return np.concatenate(panels, axis=0)


def _save_gif(path: Path, frames: list[np.ndarray], duration_ms: int, loop: int) -> None:
    try:
        import imageio.v2 as imageio
    except ImportError:
        import imageio  # type: ignore
    frames = _pad_to_common_size(frames)
    imageio.mimsave(
        path,
        frames,
        duration=duration_ms / 1000.0,
        loop=loop,
    )
    print(f"  wrote {path} ({len(frames)} frames)")


def _living_value_extent(
    df_cells,
    times,
    spec,
    *,
    q_high: float = 0.98,
) -> tuple[float, float]:
    """Robust vmin/vmax over living cells (avoids fringe outliers washing the map)."""
    value_fn = spec["color_config"]["value_fn"]
    vals: list[float] = []
    for t in times:
        df_t = df_cells[df_cells["time"] == t].copy()
        if df_t.empty:
            continue
        dead = (
            df_t["dead"].to_numpy(dtype=float) >= 1.0
            if "dead" in df_t.columns
            else np.zeros(len(df_t), dtype=bool)
        )
        if "y_death_rates" in df_t.columns:
            ydeath = df_t["y_death_rates"].to_numpy(dtype=float)
        elif "death_rates_1" in df_t.columns:
            ydeath = df_t["death_rates_1"].to_numpy(dtype=float)
        else:
            ydeath = np.zeros(len(df_t), dtype=float)
        pre = (~dead) & (ydeath > 0.0)
        for i, row in enumerate(df_t.itertuples(index=True)):
            if dead[i] or pre[i]:
                continue
            try:
                vals.append(float(value_fn(row)))
            except Exception:
                continue
    if not vals:
        return 0.0, 1.0
    arr = np.asarray(vals, dtype=float)
    arr = arr[np.isfinite(arr)]
    if arr.size == 0:
        return 0.0, 1.0
    lo = float(np.min(arr))
    hi = float(np.quantile(arr, q_high))
    if hi <= lo:
        hi = float(np.max(arr))
    if hi <= lo:
        hi = lo + 1e-12
    if lo >= 0:
        lo = 0.0
    return lo, hi


def _row_style(base: dict, *, top: bool, bottom: bool) -> dict:
    """Match static spatial SVG width; keep left/right layout identical across rows."""
    style = dict(base)
    # Wide canvas: colorbar + ylabel use ~0.68–0.90; 0.90–1.0 is spare margin
    style["figsize"] = (20.0, 5.6)
    style["zone_labels"] = top
    # Keep vessel label on every row so left margin stays fixed (aligned columns)
    style["vessel_label"] = True
    style["show_x_ruler"] = bottom
    style["xlabel"] = "Distance from vessel (µm)" if bottom else ""
    style["annotation_fontsize"] = base.get("annotation_fontsize", 25)
    style["fontsize"] = base.get("fontsize", 20)
    return style


# Layout fractions for stack rows: tissue | colorbar | ylabel | spare right margin.
# Spare margin matters because stack frames are saved with bbox_inches=None.
_STACK_RIGHT = 0.68
_STACK_CAX = (0.695, 0.18, 0.012, 0.68)  # left, bottom, width, height


def _lock_stack_layout(
    fig, *, top: bool, bottom: bool, show_colorbar: bool = True
) -> None:
    """Identical left/right so tissue (+ optional colorbar) columns line up."""
    top_m = 0.90 if top else 0.96
    bottom_m = 0.18 if bottom else 0.06
    right = _STACK_RIGHT if show_colorbar else 0.98
    fig.subplots_adjust(left=0.08, right=right, top=top_m, bottom=bottom_m)
    if show_colorbar and len(fig.axes) >= 2:
        cax = fig.axes[-1]
        # Match vertical span to row (top row has zone labels → slightly lower top)
        cax_top = 0.18
        cax_h = (top_m - 0.04) - cax_top
        cax.set_position((_STACK_CAX[0], cax_top, _STACK_CAX[2], max(cax_h, 0.5)))
        ylabel = cax.get_ylabel()
        if ylabel:
            cax.set_ylabel(ylabel, labelpad=8)


def _place_time_above_colorbar(fig, t: float, *, fontsize: float) -> None:
    """Put ``t = N h`` on top of the colorbar, y-aligned with zone labels."""
    if len(fig.axes) < 2:
        fig.text(
            0.98,
            0.97,
            f"t = {t:.0f} h",
            ha="right",
            va="top",
            fontsize=fontsize,
            transform=fig.transFigure,
            color="black",
            clip_on=False,
        )
        return

    ax, cax = fig.axes[0], fig.axes[-1]
    y_lo, y_hi = sorted(ax.get_ylim())
    # Same baseline as zone labels in plot_cells_spatial
    y_text = y_hi + 0.04 * (y_hi - y_lo)
    _, y_disp = ax.transData.transform((0.0, y_text))
    _, y_fig = fig.transFigure.inverted().transform((0.0, y_disp))

    # Center on the colorbar strip + tick labels (exclude the vertical ylabel,
    # which sits further right and would pull the timestamp off the bar).
    from matplotlib.transforms import Bbox

    renderer = fig.canvas.get_renderer()
    boxes = [cax.get_window_extent(renderer=renderer)]
    for tick in cax.get_yticklabels():
        if tick.get_visible():
            boxes.append(tick.get_window_extent(renderer=renderer))
    bbox = Bbox.union(boxes)
    bb_fig = bbox.transformed(fig.transFigure.inverted())
    x_fig = 0.5 * (bb_fig.x0 + bb_fig.x1)

    fig.text(
        x_fig,
        y_fig,
        f"t = {t:.0f} h",
        ha="center",
        va="bottom",
        fontsize=fontsize,
        transform=fig.transFigure,
        color="black",
        clip_on=False,
    )


def _render_stem(
    df_cells,
    times: list[float],
    spec: dict,
    style: dict,
    *,
    fixed_vlim: tuple[float, float] | None = None,
) -> list[np.ndarray]:
    frames = []
    for i, t in enumerate(times):
        print(f"  [{i + 1}/{len(times)}] t = {t:.0f} h")
        kwargs = dict(style)
        if fixed_vlim is not None:
            kwargs["vmin"], kwargs["vmax"] = fixed_vlim
        else:
            if spec.get("vmax") is not None:
                kwargs["vmax"] = spec["vmax"]
            if "vmin" in spec and spec["vmin"] is not None:
                kwargs["vmin"] = spec["vmin"]
        if spec.get("power_gamma") is not None:
            kwargs["power_gamma"] = spec["power_gamma"]

        fig = plot_cells_spatial(
            df_cells,
            time_point=t,
            color_config=spec["color_config"],
            **kwargs,
        )
        # Needed so data→figure transforms for the time label are valid
        fig.canvas.draw()
        _place_time_above_colorbar(
            fig,
            t,
            fontsize=float(style.get("annotation_fontsize", 15)),
        )
        frames.append(_fig_to_rgba(fig))
    return frames


def _render_uptake_stack(
    df_cells,
    times: list[float],
    specs: list[dict],
    base_style: dict,
    *,
    show_colorbar: bool = True,
) -> list[np.ndarray]:
    """One GIF frame = vertical stack of spatial maps."""
    # Fixed color scales across time (per substrate)
    vlims: dict[str, tuple[float, float]] = {}
    print("- Computing shared color scales for uptake stack …")
    for spec in specs:
        if spec.get("vmax") is not None and "vmin" in spec:
            vlims[spec["stem"]] = (float(spec["vmin"]), float(spec["vmax"]))
        else:
            lo, hi = _living_value_extent(df_cells, times, spec, q_high=0.98)
            vlims[spec["stem"]] = (lo, hi)
            print(f"  {spec['stem']}: vmin={lo:.4g} vmax={hi:.4g} (98th pct)")

    frames: list[np.ndarray] = []
    n_spec = len(specs)
    for i, t in enumerate(times):
        print(f"  [{i + 1}/{len(times)}] uptake stack t = {t:.0f} h")
        panels: list[np.ndarray] = []
        for j, spec in enumerate(specs):
            style = _row_style(
                base_style,
                top=(j == 0),
                bottom=(j == n_spec - 1),
            )
            kwargs = dict(style)
            vmin, vmax = vlims[spec["stem"]]
            kwargs["vmin"] = vmin
            kwargs["vmax"] = vmax
            kwargs["show_colorbar"] = show_colorbar
            fig = plot_cells_spatial(
                df_cells,
                time_point=t,
                color_config=spec["color_config"],
                **kwargs,
            )
            _lock_stack_layout(
                fig,
                top=(j == 0),
                bottom=(j == n_spec - 1),
                show_colorbar=show_colorbar,
            )
            # Time stamp only on multi-frame GIFs (static single-frame stacks omit it)
            if j == 0 and len(times) > 1:
                fig.text(
                    0.98,
                    0.99,
                    f"t = {t:.0f} h",
                    ha="right",
                    va="top",
                    fontsize=13,
                    transform=fig.transFigure,
                    color="black",
                )
            # Fixed canvas (non-tight) keeps vessel columns aligned across rows;
            # figsize/margins leave room for long colorbar ylabels (e.g. O₂).
            panels.append(_fig_to_rgba(fig, tight=False))
        frames.append(_trim_right_whitespace(_stack_vertical(panels, gap=4), pad=40))
    return frames


def _nearest_z(z_values: np.ndarray, target: float = 0.0) -> float:
    uniq = np.unique(z_values)
    return float(uniq[np.argmin(np.abs(uniq - target))])


def _load_substrate_profiles(
    reader,
    substrates: list[str],
    *,
    time_interval: float = 60.0,
    x_max: float = 320.0,
) -> tuple[list[float], dict[str, dict[float, pd.Series]], dict[str, tuple[float, float]]]:
    """
    Return (times_h, profiles[name][t] -> Series indexed by x, ylims[name]).

    Profile = mean over y at the midplane z closest to 0.
    """
    available = {name for name, _, _ in reader.microenvironment_columns}
    missing = [s for s in substrates if s not in available]
    if missing:
        raise SystemExit(
            f"Substrates not in microenvironment: {missing}. "
            f"Available: {sorted(available)}"
        )

    col_index = {
        name: 4 + int(str_idx) for name, _, str_idx in reader.microenvironment_columns
    }
    profiles: dict[str, dict[float, pd.Series]] = {s: {} for s in substrates}
    times: list[float] = []

    for t_min, menv in reader.microenvironment_as_matrix_iterator():
        t_h = float(t_min) / float(time_interval)
        times.append(t_h)
        x = menv[0, :]
        y = menv[1, :]
        z = menv[2, :]
        z0 = _nearest_z(z, 0.0)
        mask = np.isclose(z, z0)
        df = pd.DataFrame({"x": x[mask], "y": y[mask]})
        for name in substrates:
            df[name] = menv[col_index[name], mask]
        for name in substrates:
            grid = df.pivot_table(index="y", columns="x", values=name, aggfunc="mean")
            s = grid.mean(axis=0).sort_index()
            s = s[s.index <= x_max + 1e-9]
            profiles[name][t_h] = s

    ylims: dict[str, tuple[float, float]] = {}
    for name in substrates:
        vals = np.concatenate([s.to_numpy() for s in profiles[name].values()])
        lo = 0.0
        hi = float(np.nanmax(vals)) if vals.size else 1.0
        if not np.isfinite(hi) or hi <= lo:
            hi = lo + 1.0
        # small headroom so the curve is not flush with the top
        ylims[name] = (lo, hi * 1.05)
        print(f"  {name}: ymax={hi:.4g}")

    return times, profiles, ylims


def plot_substrate_profiles(
    profiles: dict[str, dict[float, pd.Series]],
    substrates: list[str],
    time_point: float,
    ylims: dict[str, tuple[float, float]],
    *,
    x_min: float = 0.0,
    x_max: float = 320.0,
    zone_x: tuple[float, float] = (120.0, 200.0),
    title: Optional[str] = None,
):
    """Stacked concentration-vs-x line plots at one time (returns a Figure)."""
    n = len(substrates)
    fig, axes = plt.subplots(
        n,
        1,
        figsize=(10.5, 1.55 * n + 0.6),
        sharex=True,
        dpi=110,
        constrained_layout=True,
    )
    if n == 1:
        axes = [axes]

    # Snap to nearest available frame if needed
    available_t = sorted({t for name in substrates for t in profiles[name]})
    t = float(time_point)
    if available_t and t not in profiles[substrates[0]]:
        t = float(min(available_t, key=lambda u: abs(u - t)))

    for ax, name in zip(axes, substrates):
        s = profiles[name].get(t)
        color = _SUBSTRATE_COLORS.get(name, "black")
        if s is not None and len(s):
            ax.plot(s.index, s.values, color=color, linewidth=2.2)
            ax.fill_between(s.index, 0.0, s.values, color=color, alpha=0.12)
        for zx in zone_x:
            ax.axvline(zx, color="0.45", linestyle="--", linewidth=1.0, alpha=0.85)
        ax.set_xlim(x_min, x_max)
        ax.set_ylim(*ylims[name])
        ax.set_ylabel(_SUBSTRATE_YLABELS.get(name, name), fontsize=11)
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.tick_params(labelsize=10)
        if ax is axes[0]:
            ax.set_title(
                title
                or f"Substrate vs distance from vessel    t = {t:.0f} h",
                fontsize=13,
                pad=8,
            )
            y_top = ylims[name][1]
            ax.text(
                0.5 * zone_x[0],
                y_top * 0.92,
                "Prolif.",
                ha="center",
                va="top",
                fontsize=9,
                color="0.35",
            )
            ax.text(
                0.5 * (zone_x[0] + zone_x[1]),
                y_top * 0.92,
                "Hypoxic",
                ha="center",
                va="top",
                fontsize=9,
                color="0.35",
            )
            ax.text(
                0.5 * (zone_x[1] + x_max),
                y_top * 0.92,
                "Necrotic",
                ha="center",
                va="top",
                fontsize=9,
                color="0.35",
            )
    axes[-1].set_xlabel("Distance from vessel (µm)", fontsize=12)
    return fig


def _render_substrate_profiles(
    times: list[float],
    profiles: dict[str, dict[float, pd.Series]],
    substrates: list[str],
    ylims: dict[str, tuple[float, float]],
    *,
    x_min: float = 0.0,
    x_max: float = 320.0,
    zone_x: tuple[float, float] = (120.0, 200.0),
) -> list[np.ndarray]:
    """Stacked concentration-vs-x line plots, one GIF frame per time."""
    frames: list[np.ndarray] = []
    for i, t in enumerate(times):
        print(f"  [{i + 1}/{len(times)}] substrate profiles t = {t:.0f} h")
        fig = plot_substrate_profiles(
            profiles,
            substrates,
            t,
            ylims,
            x_min=x_min,
            x_max=x_max,
            zone_x=zone_x,
        )
        frames.append(_fig_to_rgba(fig, tight=True))
    return frames


def main(argv: Optional[list[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description="GIF of cancer-tissue frames (hourly)."
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=None,
        help="PhysiCell output folder (default: ./output if present).",
    )
    parser.add_argument("--fig-dir", type=Path, default=None)
    parser.add_argument(
        "--only",
        default="growth_rate",
        help="Comma-separated spatial stems for individual GIFs "
        "(default: growth_rate). Use '' with --uptake-stack to skip.",
    )
    parser.add_argument(
        "--uptake-stack",
        action="store_true",
        help=(
            "Write a 1-column GIF stacking spatial uptake maps "
            "(like glycine_uptake_*.svg)."
        ),
    )
    parser.add_argument(
        "--stack-stems",
        default=None,
        help=(
            "Comma-separated stems for --uptake-stack "
            "(default: oxygen,glucose,glutamine,glycine,lactate)."
        ),
    )
    parser.add_argument(
        "--no-colorbar",
        action="store_true",
        help="Hide colorbars on uptake-stack panels (shared scales still apply).",
    )
    parser.add_argument(
        "--substrate-profiles",
        action="store_true",
        help=(
            "Write a stacked GIF of microenvironment concentration vs "
            "distance from vessel (y-mean profiles)."
        ),
    )
    parser.add_argument(
        "--profile-substrates",
        default=None,
        help=(
            "Comma-separated microenvironment names for --substrate-profiles "
            "(default: O2,D-Glucose,L-Glutamine,Glycine,L-Lactate)."
        ),
    )
    parser.add_argument(
        "--profiles",
        action="store_true",
        help=argparse.SUPPRESS,  # backward alias → uptake-stack
    )
    parser.add_argument("--time-interval", type=float, default=60.0)
    parser.add_argument("--t-min", type=float, default=None)
    parser.add_argument("--t-max", type=float, default=None)
    parser.add_argument("--stride", type=int, default=1)
    parser.add_argument("--duration-ms", type=int, default=250)
    parser.add_argument("--loop", type=int, default=0)
    parser.add_argument(
        "--skip-zero-flux-start",
        action="store_true",
        default=True,
        help="Skip leading frames where all exchange fluxes are zero (default: on).",
    )
    parser.add_argument(
        "--include-zero-flux-start",
        action="store_true",
        help="Keep t=0 even if fluxes are still zero.",
    )
    args = parser.parse_args(argv)

    if args.profiles:
        args.uptake_stack = True

    # Headless GIF rendering only — do not set this at import time (breaks Jupyter).
    matplotlib.use("Agg", force=True)

    output_dir = (args.output_dir or figure_paths.default_output_dir()).resolve()
    if not output_dir.is_dir():
        raise SystemExit(f"Output directory not found: {output_dir}")
    fig_dir = (args.fig_dir or (output_dir / "fig")).resolve()
    fig_dir.mkdir(parents=True, exist_ok=True)

    only = _parse_only(args.only)
    need_cells = bool(only) or args.uptake_stack

    reader = multicellds.MultiCellDS(output_folder=str(output_dir))
    df_cells = None
    times: list[float]

    if need_cells:
        print(f"- Reading cells from {output_dir}")
        df_cells = read_cells(
            reader, drop_cols=_DROP_COLS, max_time=-1, time_interval=args.time_interval
        )
        available = sorted(float(t) for t in df_cells["time"].unique())
        t0 = available[0] if args.t_min is None else float(args.t_min)
        t1 = available[-1] if args.t_max is None else float(args.t_max)
        times = [t for t in available if t0 - 1e-9 <= t <= t1 + 1e-9]
        if args.stride > 1:
            times = times[:: args.stride]

        # t=0 is often pre-FBA (all fluxes zero) — drop leading empty frames
        if args.skip_zero_flux_start and not args.include_zero_flux_start:
            flux_cols = [c for c in df_cells.columns if c.startswith("R_EX_")]
            trimmed = []
            skipping = True
            for t in times:
                if not skipping:
                    trimmed.append(t)
                    continue
                df_t = df_cells[df_cells["time"] == t]
                if flux_cols and float(df_t[flux_cols].abs().to_numpy().max()) > 0:
                    skipping = False
                    trimmed.append(t)
                else:
                    print(f"- Skipping t={t:.0f} h (no exchange fluxes yet)")
            times = trimmed

        if not times:
            raise SystemExit(f"No time points in [{t0}, {t1}] h")
        print(f"- Frames: {len(times)} (t = {times[0]:.0f} … {times[-1]:.0f} h)")
    else:
        times = []  # filled from microenvironment below

    style = dict(_DEFAULT_STYLE)
    all_specs = {s["stem"]: s for s in _figure_specs()}

    if only:
        unknown = only - set(all_specs)
        if unknown:
            raise SystemExit(f"Unknown --only stems: {sorted(unknown)}")
        for stem in only:
            spec = all_specs[stem]
            missing = [c for c in spec["required"] if c not in df_cells.columns]
            if missing:
                print(f"- Skipping {stem}: missing {missing}")
                continue
            print(f"- Rendering spatial {stem} …")
            frames = _render_stem(df_cells, times, spec, style)
            _save_gif(
                fig_dir / f"{stem}.gif",
                frames,
                args.duration_ms,
                args.loop,
            )

    if args.uptake_stack:
        if args.stack_stems:
            stems = [s.strip() for s in args.stack_stems.split(",") if s.strip()]
        else:
            stems = list(_DEFAULT_STACK_STEMS)
        specs = []
        for stem in stems:
            if stem not in all_specs:
                raise SystemExit(f"Unknown stack stem: {stem}")
            spec = all_specs[stem]
            missing = [c for c in spec["required"] if c not in df_cells.columns]
            if missing:
                print(f"- Skipping stack row {stem}: missing {missing}")
                continue
            specs.append(spec)
        if not specs:
            raise SystemExit("No valid stems for --uptake-stack")
        print(f"- Rendering uptake stack ({len(specs)} rows) …")
        frames = _render_uptake_stack(
            df_cells,
            times,
            specs,
            style,
            show_colorbar=not args.no_colorbar,
        )
        gif_name = (
            "uptake_stack_no_cbar.gif" if args.no_colorbar else "uptake_stack.gif"
        )
        _save_gif(
            fig_dir / gif_name,
            frames,
            args.duration_ms,
            args.loop,
        )

    if args.substrate_profiles:
        if args.profile_substrates:
            substrates = [
                s.strip() for s in args.profile_substrates.split(",") if s.strip()
            ]
        else:
            substrates = list(_DEFAULT_SUBSTRATE_PROFILES)
        print(f"- Loading microenvironment profiles ({len(substrates)} substrates) …")
        all_t, profiles, ylims = _load_substrate_profiles(
            reader,
            substrates,
            time_interval=args.time_interval,
            x_max=float(style["X_MAX"]),
        )
        t0 = all_t[0] if args.t_min is None else float(args.t_min)
        t1 = all_t[-1] if args.t_max is None else float(args.t_max)
        prof_times = [t for t in all_t if t0 - 1e-9 <= t <= t1 + 1e-9]
        if args.stride > 1:
            prof_times = prof_times[:: args.stride]
        # Align with cell-GIF convention: drop t=0 unless asked
        if (
            args.skip_zero_flux_start
            and not args.include_zero_flux_start
            and prof_times
            and abs(prof_times[0]) < 1e-9
        ):
            print("- Skipping t=0 h for substrate profiles")
            prof_times = prof_times[1:]
        if not prof_times:
            raise SystemExit("No microenvironment time points in requested range")
        print(
            f"- Rendering substrate profiles "
            f"({len(prof_times)} frames, t = {prof_times[0]:.0f} … {prof_times[-1]:.0f} h) …"
        )
        frames = _render_substrate_profiles(
            prof_times,
            profiles,
            substrates,
            ylims,
            x_min=float(style["X_MIN"]),
            x_max=float(style["X_MAX"]),
            zone_x=tuple(style["zone_x"]),
        )
        _save_gif(
            fig_dir / "substrate_profiles.gif",
            frames,
            args.duration_ms,
            args.loop,
        )

    if not only and not args.uptake_stack and not args.substrate_profiles:
        raise SystemExit(
            "Nothing to do: set --only and/or --uptake-stack and/or --substrate-profiles"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
