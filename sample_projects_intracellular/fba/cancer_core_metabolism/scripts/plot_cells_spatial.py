"""Styled 2D cell field plots.

Companion to the classic ``plot_cells_svg`` in the cancer-tissue notebooks.
Same layout idea (top-down circles, vessel at xmin, optional zone guides),
but tuned for a cleaner publication look:

  - soft filled cells, faint blob nuclei, light rims
  - schematic vessel (lumen / glow / ticks)
  - x-axis ruler below the tissue (optional legacy scale bar)
  - publication typography via ``composite_figure_style`` when available

Does **not** replace ``plot_cells_svg`` — use this to iterate on figure style.
"""

from __future__ import annotations

import sys
from pathlib import Path as _Path
from typing import Callable, Optional, Sequence, Tuple, Union

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import Normalize, PowerNorm, to_rgba
from matplotlib.patches import Circle, Ellipse, FancyBboxPatch, PathPatch, Rectangle
from matplotlib.path import Path as MplPath


# ---------------------------------------------------------------------------
# Style
# ---------------------------------------------------------------------------

_FS = 8.0
_BG = "#ffffff"
_VESSEL_WALL = "#b85c5f"
_VESSEL_LUMEN = "#f3d6d7"
_DEAD = "#383731"       # near-black (necrotic / dead)
_PREDEAD = "#b3b3b3"    # classic plot_cells_svg grey (necrosis rate > 0, still alive)
_EDGE = (0.15, 0.15, 0.15, 0.35)
_DEAD_EDGE = (1.0, 1.0, 1.0, 0.85)  # white rim separates necrotic cells in dense cores
_ANALYSIS_DIR = _Path(__file__).resolve().parent

# cell_shape options: "circle" | "ellipse" | "blob" | "squircle"
# vessel_style options: "lumen" | "bar" | "glow" | "ticks"
CellShape = str
VesselStyle = str


def _apply_cell_press_rc():
    try:
        if str(_ANALYSIS_DIR) not in sys.path:
            sys.path.insert(0, str(_ANALYSIS_DIR))
        from composite_figure_style import apply_cell_press_style

        apply_cell_press_style()
    except Exception:
        plt.rcParams.update(
            {
                "font.family": "sans-serif",
                "font.sans-serif": ["Arial", "Helvetica", "DejaVu Sans"],
                "pdf.fonttype": 42,
                "ps.fonttype": 42,
                "svg.fonttype": "none",
            }
        )


def _radius_from_volume(volume, scale: float = 1.0) -> np.ndarray:
    return scale * np.cbrt((3.0 * np.asarray(volume, dtype=float)) / (4.0 * np.pi))


def _cell_rng(seed_key: Union[int, float], base_seed: int = 0) -> np.random.Generator:
    """Stable per-cell RNG so shapes don't jump between frames/reloads."""
    # SplitMix64–ish mix of cell id + base seed
    x = (int(seed_key) * 0x9E3779B97F4A7C15) ^ (int(base_seed) * 0xBF58476D1CE4E5B9)
    x &= (1 << 64) - 1
    return np.random.default_rng(x)


def _blob_vertices(
    x: float,
    y: float,
    r: float,
    rng: np.random.Generator,
    *,
    n: int = 48,
    roughness: float = 0.18,
    harmonics: int = 5,
) -> np.ndarray:
    """Closed organic outline via low-order Fourier radius perturbation."""
    theta = np.linspace(0.0, 2.0 * np.pi, n, endpoint=False)
    rad = np.ones(n)
    for k in range(1, harmonics + 1):
        amp = roughness / k
        phase = rng.uniform(0.0, 2.0 * np.pi)
        rad += amp * np.cos(k * theta + phase)
    rad = np.clip(rad, 0.55, 1.45)
    # Renormalize mean radius so area ~ π r² roughly
    rad *= r / rad.mean()
    verts = np.column_stack([x + rad * np.cos(theta), y + rad * np.sin(theta)])
    return np.vstack([verts, verts[0]])


def _squircle_vertices(
    x: float,
    y: float,
    r: float,
    rng: np.random.Generator,
    *,
    n: int = 64,
    p: float = 3.5,
) -> np.ndarray:
    """Superellipse (squircle) with mild random aspect + rotation."""
    aspect = float(rng.uniform(0.82, 1.18))
    angle = float(rng.uniform(0.0, 2.0 * np.pi))
    a, b = r * np.sqrt(aspect), r / np.sqrt(aspect)
    theta = np.linspace(0.0, 2.0 * np.pi, n, endpoint=False)
    # |cos|^2/p * sign, |sin|^2/p * sign
    c, s = np.cos(theta), np.sin(theta)
    px = a * np.sign(c) * np.abs(c) ** (2.0 / p)
    py = b * np.sign(s) * np.abs(s) ** (2.0 / p)
    ca, sa = np.cos(angle), np.sin(angle)
    xr = x + ca * px - sa * py
    yr = y + sa * px + ca * py
    verts = np.column_stack([xr, yr])
    return np.vstack([verts, verts[0]])


def _make_cell_patch(
    x: float,
    y: float,
    r: float,
    facecolor,
    *,
    shape: CellShape = "blob",
    seed_key: Union[int, float] = 0,
    shape_seed: int = 0,
    roughness: float = 0.18,
    linewidth: float = 0.35,
    edgecolor=None,
    zorder: int = 3,
):
    """Build a matplotlib patch for one cell."""
    shape = shape.lower()
    rng = _cell_rng(seed_key, shape_seed)
    if linewidth <= 0:
        edge = "none"
    elif edgecolor is not None:
        edge = edgecolor
    else:
        edge = _EDGE

    if shape == "circle":
        return Circle(
            (x, y), r, facecolor=facecolor, edgecolor=edge, linewidth=linewidth, zorder=zorder
        )

    if shape == "ellipse":
        aspect = float(rng.uniform(0.72, 1.28))
        angle = float(np.degrees(rng.uniform(0.0, 2.0 * np.pi)))
        w, h = 2.0 * r * np.sqrt(aspect), 2.0 * r / np.sqrt(aspect)
        return Ellipse(
            (x, y),
            width=w,
            height=h,
            angle=angle,
            facecolor=facecolor,
            edgecolor=edge,
            linewidth=linewidth,
            zorder=zorder,
        )

    if shape == "squircle":
        verts = _squircle_vertices(x, y, r, rng)
    elif shape == "blob":
        verts = _blob_vertices(x, y, r, rng, roughness=roughness)
    else:
        raise ValueError(
            f"Unknown cell_shape={shape!r}. Use 'circle', 'ellipse', 'blob', or 'squircle'."
        )

    path = MplPath(verts, closed=True)
    return PathPatch(
        path, facecolor=facecolor, edgecolor=edge, linewidth=linewidth, zorder=zorder
    )


def _draw_vessel(
    ax,
    x_lo: float,
    y_lo: float,
    y_hi: float,
    *,
    style: VesselStyle = "lumen",
    label: bool = True,
    fontsize: float = _FS,
):
    """Schematic vessel at the proximal boundary.

    Styles
    ------
    lumen : rounded wall + pale lumen (default; reads as a vessel)
    bar   : flat muted rectangle (legacy)
    glow  : soft nutrient halo, no solid bar
    ticks : small circular pores along xmin (Dirichlet sources)
    """
    style = style.lower()
    height = y_hi - y_lo
    cy = 0.5 * (y_lo + y_hi)

    if style == "bar":
        w, gap = 8.0, 2.0
        ax.add_patch(
            Rectangle(
                (x_lo - w - gap, y_lo),
                w,
                height,
                facecolor=to_rgba(_VESSEL_WALL, 0.55),
                edgecolor=to_rgba(_VESSEL_WALL, 0.8),
                linewidth=0.6,
                clip_on=False,
                zorder=2,
            )
        )
        label_x = x_lo - w - gap - 1.5

    elif style == "glow":
        # Soft source halo bleeding into the tissue from xmin
        for i, (w, a) in enumerate(((18.0, 0.07), (10.0, 0.12), (4.0, 0.22))):
            ax.add_patch(
                Rectangle(
                    (x_lo - 1.0, y_lo),
                    w,
                    height,
                    facecolor=to_rgba(_VESSEL_WALL, a),
                    edgecolor="none",
                    clip_on=False,
                    zorder=1,
                )
            )
        ax.plot(
            [x_lo, x_lo],
            [y_lo, y_hi],
            color=to_rgba(_VESSEL_WALL, 0.85),
            lw=1.4,
            solid_capstyle="round",
            clip_on=False,
            zorder=2,
        )
        label_x = x_lo - 4.0

    elif style == "ticks":
        # Discrete source pores along the vessel face
        n = max(3, int(round(height / 12.0)))
        ys = np.linspace(y_lo + 0.12 * height, y_hi - 0.12 * height, n)
        r = min(3.2, 0.35 * (ys[1] - ys[0]) if n > 1 else 3.2)
        for y in ys:
            ax.add_patch(
                Circle(
                    (x_lo, y),
                    r,
                    facecolor=to_rgba(_VESSEL_LUMEN, 0.95),
                    edgecolor=to_rgba(_VESSEL_WALL, 0.9),
                    linewidth=0.9,
                    clip_on=False,
                    zorder=2,
                )
            )
            ax.add_patch(
                Circle(
                    (x_lo, y),
                    0.35 * r,
                    facecolor=to_rgba(_VESSEL_WALL, 0.55),
                    edgecolor="none",
                    clip_on=False,
                    zorder=3,
                )
            )
        label_x = x_lo - 6.0

    else:  # "lumen" — default
        # Longitudinal vessel: outer wall + pale lumen, caps rounded
        outer_w, lumen_w, gap = 11.0, 5.5, 1.5
        x_outer = x_lo - outer_w - gap
        ax.add_patch(
            FancyBboxPatch(
                (x_outer, y_lo),
                outer_w,
                height,
                boxstyle=f"round,pad=0,rounding_size={min(3.5, 0.08 * height)}",
                facecolor=to_rgba(_VESSEL_WALL, 0.88),
                edgecolor=to_rgba(_VESSEL_WALL, 1.0),
                linewidth=0.5,
                clip_on=False,
                zorder=2,
            )
        )
        x_lumen = x_outer + 0.5 * (outer_w - lumen_w)
        ax.add_patch(
            FancyBboxPatch(
                (x_lumen, y_lo + 0.04 * height),
                lumen_w,
                0.92 * height,
                boxstyle=f"round,pad=0,rounding_size={min(2.5, 0.06 * height)}",
                facecolor=to_rgba(_VESSEL_LUMEN, 0.95),
                edgecolor="none",
                clip_on=False,
                zorder=3,
            )
        )
        label_x = x_outer - 1.5

    if label:
        ax.text(
            label_x,
            cy,
            "Vessel",
            ha="right",
            va="center",
            rotation=90,
            fontsize=fontsize,
            color="black",
            clip_on=False,
            zorder=5,
        )


# ---------------------------------------------------------------------------
# Main 2D plotter
# ---------------------------------------------------------------------------

def plot_cells_spatial(
    df: pd.DataFrame,
    time_point: float,
    color_config: dict,
    *,
    X_MIN: float = 0.0,
    X_MAX: float = 360.0,
    Y_MIN: float = -60.0,
    Y_MAX: float = 60.0,
    xlim: Optional[Tuple[float, float]] = None,
    ylim: Optional[Tuple[float, float]] = None,
    skip_dead: bool = True,
    scale_bar_length: float = 50.0,
    show_scale_bar: bool = False,
    show_x_ruler: bool = True,
    x_major: float = 50.0,
    x_minor: float = 10.0,
    xlabel: str = "Distance from vessel (µm)",
    colorbar_range: Optional[Tuple[float, float]] = None,
    linewidth: float = 0.35,
    hide_axis: bool = True,
    vmax: Optional[float] = None,
    vmin: Optional[float] = None,
    power_gamma: Optional[float] = None,
    normalize_colors: bool = True,
    radius_scale: float = 1.0,
    cell_alpha: float = 0.95,
    fontsize: float = _FS,
    annotation_fontsize: Optional[float] = None,
    figsize: Optional[Tuple[float, float]] = None,
    show_vessel: bool = True,
    vessel_style: VesselStyle = "lumen",
    vessel_label: bool = True,
    show_zones: bool = True,
    zone_x: Sequence[float] = (120.0, 200.0),
    zone_labels: bool = True,
    zone_names: Sequence[str] = ("Proliferative", "Hypoxic", "Necrotic"),
    zone_bands: bool = False,
    zone_band_colors: Sequence[str] = ("#e8f5e9", "#eeeeee", "#e8e4ea"),
    show_nuclei: bool = True,
    nucleus_alpha: float = 0.22,
    nucleus_scale: float = 1.0,
    show_colorbar: Optional[bool] = None,
    cell_shape: CellShape = "blob",
    shape_seed: int = 0,
    roughness: float = 0.18,
    perspective: float = 0.28,
    depth_fade: float = 0.15,
) -> plt.Figure:
    """Minimal 2D cell field plots for the cancer-tissue column.

    API deliberately mirrors ``plot_cells_svg`` so existing notebook call
    sites can swap with small edits.

    Parameters
    ----------
    cell_shape :
        ``"circle"`` | ``"ellipse"`` | ``"blob"`` (organic) | ``"squircle"``.
    roughness :
        Only for ``blob`` — amplitude of boundary wobble (~0.1–0.25 looks good).
    shape_seed :
        Global seed; per-cell outline is stable given cell ID + this seed.
    perspective :
        Apparent size vs *z* when projecting onto XY. Camera from +z: higher
        *z* → closer → larger. ``0`` disables. ~0.25–0.35 suits ±7.5 µm layers.
    depth_fade :
        Extra transparency for far (low-*z*) cells; ``0`` disables.
    power_gamma :
        If set (e.g. ``0.5``), use a power-law color stretch with the same
        ``vmin``/``vmax`` (keeps an absolute ceiling while expanding contrast
        in the lower part of the range — useful for growth rate when cells
        only reach ~½ of ``max_growth_rate``).
    """
    _apply_cell_press_rc()
    ann_fs = float(annotation_fontsize) if annotation_fontsize is not None else float(fontsize) + 3.0

    df_t = df[df["time"] == time_point].copy()
    df_t = df_t.loc[:, ~df_t.columns.duplicated()]
    if df_t.empty:
        available = sorted(df["time"].unique())
        raise ValueError(
            f"No cells at time_point={time_point} h. "
            f"Available times (h): {available[0]:.0f}–{available[-1]:.0f} "
            f"({len(available)} frames)."
        )

    if skip_dead and "dead" in df_t.columns:
        df_t = df_t[df_t["dead"] == False]  # noqa: E712

    x_lo, x_hi = xlim if xlim is not None else (X_MIN, X_MAX)
    y_lo, y_hi = ylim if ylim is not None else (Y_MIN, Y_MAX)
    # Note: if both Y_MIN/Y_MAX and ylim are passed, ylim wins (same for x).

    df_t = df_t[
        (df_t["x_position"] >= x_lo)
        & (df_t["x_position"] <= x_hi)
        & (df_t["y_position"] >= y_lo)
        & (df_t["y_position"] <= y_hi)
    ]
    if df_t.empty:
        raise ValueError(f"No cells inside crop at time_point={time_point} h.")

    radius_cell = _radius_from_volume(df_t["total_volume"].to_numpy(float), radius_scale)
    radius_nuc = None
    if show_nuclei:
        if "nuclear_radius" in df_t.columns:
            radius_nuc = radius_scale * nucleus_scale * df_t["nuclear_radius"].to_numpy(float)
        elif "nuclear_volume" in df_t.columns:
            radius_nuc = _radius_from_volume(
                df_t["nuclear_volume"].to_numpy(float), radius_scale * nucleus_scale
            )
        else:
            # Fallback: ~45% of cell radius if nuclear size missing
            radius_nuc = 0.45 * radius_cell

    cmap = color_config["cmap"]
    value_fn: Callable = color_config["value_fn"]

    # Column arrays (avoid itertuples attribute quirks on wide PhysiCell frames)
    dead_arr = (
        df_t["dead"].to_numpy(dtype=float)
        if "dead" in df_t.columns
        else np.zeros(len(df_t), dtype=float)
    )
    if "y_death_rates" in df_t.columns:
        y_death_arr = df_t["y_death_rates"].to_numpy(dtype=float)
    elif "death_rates_1" in df_t.columns:
        y_death_arr = df_t["death_rates_1"].to_numpy(dtype=float)
    else:
        y_death_arr = np.zeros(len(df_t), dtype=float)

    is_dead = dead_arr >= 1.0
    # Match classic plot_cells_svg: any living cell with necrosis rate > 0
    is_predead = (~is_dead) & (y_death_arr > 0.0)

    raw_values = np.full(len(df_t), np.nan, dtype=float)
    for i, row in enumerate(df_t.itertuples(index=True)):
        if is_dead[i] or is_predead[i]:
            continue
        raw_values[i] = float(value_fn(row))
    finite = np.isfinite(raw_values)

    if normalize_colors:
        if not finite.any():
            raise ValueError(f"No plottable values at time_point={time_point} h.")
        v_min = float(vmin) if vmin is not None else float(np.nanmin(raw_values))
        v_max = float(vmax) if vmax is not None else float(np.nanmax(raw_values))
        if v_max == v_min:
            v_max = v_min + 1e-12
        if power_gamma is not None and float(power_gamma) > 0:
            norm = PowerNorm(gamma=float(power_gamma), vmin=v_min, vmax=v_max)
        else:
            norm = Normalize(vmin=v_min, vmax=v_max)
    else:
        norm = None

    colors = []
    for i, row in enumerate(df_t.itertuples(index=True)):
        if is_dead[i]:
            colors.append(to_rgba(_DEAD, cell_alpha))
        elif is_predead[i]:
            colors.append(to_rgba(_PREDEAD, cell_alpha))
        elif normalize_colors:
            colors.append(to_rgba(cmap(norm(raw_values[i])), cell_alpha))
        else:
            colors.append(to_rgba(cmap(value_fn(row)), cell_alpha))

    # Z positions for perspective (camera above +z looking onto XY)
    if "z_position" in df_t.columns:
        z_arr = df_t["z_position"].to_numpy(dtype=float)
    else:
        z_arr = np.zeros(len(df_t), dtype=float)
    z_mid = 0.5 * (float(z_arr.min()) + float(z_arr.max())) if len(z_arr) else 0.0
    z_half = max(0.5 * (float(z_arr.max()) - float(z_arr.min())), 1e-6)

    def _depth_scale(z: float) -> float:
        if perspective <= 0:
            return 1.0
        # z_norm in [-1, 1]: +1 = nearest (high z), -1 = farthest
        z_norm = (z - z_mid) / z_half
        return max(0.35, 1.0 + perspective * z_norm)

    def _depth_alpha(base_rgba, z: float):
        if depth_fade <= 0:
            return base_rgba
        z_norm = (z - z_mid) / z_half  # +1 near, -1 far
        # Far cells a bit more transparent
        fade = 1.0 - 0.5 * depth_fade * (1.0 - z_norm)
        r, g, b, a = to_rgba(base_rgba)
        return (r, g, b, a * fade)

    if figsize is None:
        span_x = x_hi - x_lo
        span_y = y_hi - y_lo
        # Match classic plot_cells_svg scale (~12" wide); pad height so the
        # thin tissue strip + labels/scale bar are readable in notebooks.
        fig_w = 12.0
        data_h = fig_w * (span_y / max(span_x, 1e-6))
        chrome = 1.4 if (zone_labels and show_zones) else 1.0
        fig_h = max(data_h + chrome, 3.2)
        figsize = (fig_w, fig_h)

    fig, ax = plt.subplots(figsize=figsize, dpi=150, facecolor=_BG)
    ax.set_facecolor(_BG)

    # Painter's algorithm: far (low z) → near (high z)
    draw_order = np.argsort(z_arr)

    for rank, i in enumerate(draw_order):
        i = int(i)
        row = df_t.iloc[i]
        x, y, z = float(row["x_position"]), float(row["y_position"]), float(z_arr[i])
        scale = _depth_scale(z)
        r = float(radius_cell[i]) * scale
        face = _depth_alpha(colors[i], z)
        # Prefer stable cell ID; fall back to row position
        seed_key = row.name if row.name is not None else i
        if "ID" in df_t.columns:
            seed_key = row["ID"]
        if is_dead[i]:
            cell_edge = _DEAD_EDGE
            cell_lw = max(linewidth, 0.5)
        else:
            cell_edge = None
            cell_lw = linewidth
        ax.add_patch(
            _make_cell_patch(
                x,
                y,
                r,
                face,
                shape=cell_shape,
                seed_key=seed_key,
                shape_seed=shape_seed,
                roughness=roughness,
                linewidth=cell_lw,
                edgecolor=cell_edge,
                zorder=3 + rank,
            )
        )
        if radius_nuc is not None:
            # Blob nuclear outline, slightly softer than the cell rim
            if is_dead[i]:
                nuc_edge = (1.0, 1.0, 1.0, 0.45)
            else:
                nuc_edge = (0.12, 0.12, 0.14, 0.28)
            ax.add_patch(
                _make_cell_patch(
                    x,
                    y,
                    float(radius_nuc[i]) * scale,
                    "none",
                    shape=cell_shape if cell_shape != "circle" else "blob",
                    seed_key=seed_key,
                    shape_seed=shape_seed + 17,
                    roughness=min(roughness + 0.04, 0.28),
                    linewidth=max(cell_lw * 0.7, 0.25),
                    edgecolor=_depth_alpha(nuc_edge, z),
                    zorder=4 + rank,
                )
            )

    ax.set_xlim(x_lo, x_hi)
    ax.set_ylim(y_lo, y_hi)
    ax.set_aspect("equal")

    # Thin black border of the simulation / view domain
    ax.add_patch(
        Rectangle(
            (x_lo, y_lo),
            x_hi - x_lo,
            y_hi - y_lo,
            fill=False,
            edgecolor="black",
            linewidth=0.8,
            zorder=9_999,
            clip_on=False,
        )
    )

    # Axes / ruler
    ax.set_yticks([])
    for side in ("left", "right", "top"):
        ax.spines[side].set_visible(False)

    if show_x_ruler:
        ax.spines["bottom"].set_visible(True)
        ax.spines["bottom"].set_linewidth(0.8)
        ax.spines["bottom"].set_color("black")
        ax.xaxis.set_ticks_position("bottom")
        ax.tick_params(
            axis="x",
            which="major",
            length=5.5,
            width=0.8,
            color="black",
            labelcolor="black",
            labelsize=fontsize,
            pad=2,
        )
        ax.tick_params(
            axis="x",
            which="minor",
            length=3.0,
            width=0.55,
            color="black",
        )
        # Ruler ticks: majors labeled, minors unmarked
        major = np.arange(
            np.ceil(x_lo / x_major) * x_major,
            x_hi + 0.5 * x_major,
            x_major,
        )
        minor = np.arange(
            np.ceil(x_lo / x_minor) * x_minor,
            x_hi + 0.5 * x_minor,
            x_minor,
        )
        ax.set_xticks(major)
        ax.set_xticks(minor, minor=True)
        ax.set_xlabel(xlabel, fontsize=ann_fs, labelpad=6, color="black")
        ax.set_frame_on(True)
    else:
        ax.spines["bottom"].set_visible(False)
        ax.set_xticks([])
        if hide_axis:
            ax.axis("off")

    # Vessel at xmin
    if show_vessel:
        _draw_vessel(
            ax,
            x_lo,
            y_lo,
            y_hi,
            style=vessel_style,
            label=vessel_label,
            fontsize=ann_fs,
        )

    # Phenotype bands + guides. zone_x splits [x_lo, x_hi] into regions
    # (default names: Proliferative / Hypoxic / Necrotic).
    if show_zones:
        edges = [float(x_lo), *[float(z) for z in zone_x], float(x_hi)]
        edges = sorted(edges)
        # Soft background washes behind cells
        if zone_bands:
            colors = list(zone_band_colors)
            while len(colors) < len(edges) - 1:
                colors.append(colors[-1] if colors else "#f5f5f5")
            for i in range(len(edges) - 1):
                ax.axvspan(
                    edges[i],
                    edges[i + 1],
                    facecolor=colors[i],
                    edgecolor="none",
                    alpha=0.55,
                    zorder=0,
                )
        zone_z = 10_000  # guides above any cell zorder (3 + rank)
        for zx in zone_x:
            if x_lo < zx < x_hi:
                ax.axvline(
                    x=zx,
                    color=(0.25, 0.25, 0.25, 0.85),
                    linestyle=(0, (3.5, 2.0)),
                    linewidth=1.35,
                    zorder=zone_z,
                )

    if zone_labels and show_zones:
        edges = [float(x_lo), *[float(z) for z in zone_x], float(x_hi)]
        edges = sorted(edges)
        names = list(zone_names)
        while len(names) < len(edges) - 1:
            names.append(f"Zone {len(names) + 1}")
        y_text = y_hi + 0.04 * (y_hi - y_lo)
        for i in range(len(edges) - 1):
            xc = 0.5 * (edges[i] + edges[i + 1])
            if x_lo <= xc <= x_hi:
                ax.text(
                    xc,
                    y_text,
                    names[i],
                    ha="center",
                    va="bottom",
                    fontsize=ann_fs,
                    color="black",
                    clip_on=False,
                )

    # Colorbar
    draw_cbar = (
        color_config.get("show_colorbar", True)
        if show_colorbar is None
        else show_colorbar
    )
    if draw_cbar:
        if normalize_colors:
            sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
        else:
            sm = plt.cm.ScalarMappable(cmap=cmap)
            sm.set_array(raw_values[finite])
            if colorbar_range is not None:
                sm.set_clim(*colorbar_range)
        sm.set_array([])
        # Sit close to the axes (right margin) and span most of the panel height.
        cax = fig.add_axes([0.905, 0.20, 0.016, 0.66])
        cb = fig.colorbar(sm, cax=cax)
        cb.outline.set_linewidth(0.4)
        cbar_tick_fs = max(float(fontsize) + 1.0, 10.0)
        cb.ax.tick_params(labelsize=cbar_tick_fs, length=3, width=0.5, pad=2)
        label = color_config.get("label", "")
        if label:
            cax.set_ylabel(label, fontsize=ann_fs, labelpad=8)

    # Optional legacy scale bar (off by default when x-ruler is used)
    if show_scale_bar:
        bar_x0 = x_lo + 0.02 * (x_hi - x_lo)
        bar_x1 = bar_x0 + scale_bar_length
        bar_y = y_lo + 0.08 * (y_hi - y_lo)
        ax.plot(
            [bar_x0, bar_x1],
            [bar_y, bar_y],
            color="k",
            lw=1.6,
            solid_capstyle="butt",
            clip_on=False,
            zorder=10,
        )
        ax.text(
            0.5 * (bar_x0 + bar_x1),
            bar_y - 0.06 * (y_hi - y_lo),
            f"{int(scale_bar_length)} µm",
            ha="center",
            va="top",
            fontsize=fontsize,
            color="k",
            clip_on=False,
            zorder=10,
        )

    # Room for vessel + colorbar + optional zone labels / x ruler
    top = 0.90 if (zone_labels and show_zones) else 0.96
    left = 0.09 if (show_vessel and vessel_label) else (0.06 if show_vessel else 0.02)
    bottom = 0.18 if show_x_ruler else (0.10 if show_scale_bar else 0.06)
    # Tighter right margin so the colorbar sits closer to the tissue panel
    fig.subplots_adjust(left=left, right=0.895, top=top, bottom=bottom)
    return fig


if __name__ == "__main__":
    print(
        "Import from a notebook, e.g.\n"
        "  from plot_cells_spatial import plot_cells_spatial\n"
        "  fig = plot_cells_spatial(df_cells, time_point, color_config, ...)"
    )
