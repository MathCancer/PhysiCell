#!/usr/bin/env python3
"""
plot_representative_scenarios_figure.py
----------------------------------------
Build a journal-style multi-panel SVG/PNG figure comparing representative
PhysiCelldFBA scenarios.

The figure is designed to compare phenotypes such as:
  1. no motility / no growth
  2. motility / no growth
  3. motility + growth recovery

For each scenario, the top row shows the cell trajectory over the glucose
field. Lower rows show population-mean migration speed, growth rate, and
metabolic flux dynamics.

Typical usage from the project root:

    python scripts/plot_representative_scenarios_figure.py \
        --runs-dir runs \
        --results-dir results/representative_scenarios

Manual scenario selection:

    python scripts/plot_representative_scenarios_figure.py \
        --scenario runs/hill1_km050_glc010_jobXXXX/output::No motility / no growth \
        --scenario runs/hill1_km002_glc050_jobXXXX/output::Motility / no growth \
        --scenario runs/hill5_km002_glc065_jobXXXX/output::Motility + growth recovery \
        --results-dir results/representative_scenarios

Outputs:
    fig_representative_scenarios.svg
    fig_representative_scenarios.png
    representative_scenarios_metrics.csv
"""

from __future__ import annotations

import argparse
import os
import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

import matplotlib as mpl
mpl.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.colors import Normalize
from matplotlib.cm import ScalarMappable
import numpy as np
import pandas as pd
from scipy.interpolate import griddata
from pctk import multicellds

MICRO_IDX_GLUCOSE = 5

# -----------------------------------------------------------------------------
# Styling
# -----------------------------------------------------------------------------


def apply_style() -> None:
    mpl.rcParams.update({
        "font.family": "sans-serif",
        "font.sans-serif": ["Arial", "Helvetica", "DejaVu Sans"],
        "font.size": 7,
        "axes.labelsize": 7,
        "axes.titlesize": 8,
        "xtick.labelsize": 6,
        "ytick.labelsize": 6,
        "legend.fontsize": 6,
        "axes.linewidth": 0.6,
        "xtick.major.width": 0.6,
        "ytick.major.width": 0.6,
        "xtick.major.size": 3,
        "ytick.major.size": 3,
        "lines.linewidth": 1.25,
        "savefig.dpi": 300,
        "savefig.bbox": "tight",
        "savefig.pad_inches": 0.03,
        "svg.fonttype": "none",  # keep text editable in Inkscape / Illustrator
    })


# -----------------------------------------------------------------------------
# PhysiCell helpers
# -----------------------------------------------------------------------------


def get_cell_columns(output_folder: str | Path) -> list[str]:
    xml_fname = Path(output_folder) / "initial.xml"
    tree = ET.parse(xml_fname)
    root = tree.getroot()

    def _find_simplified(node):
        if node is None:
            return None
        for child in node:
            if child.tag == "simplified_data" and child.attrib.get("source") == "PhysiCell":
                return child
        for child in node:
            r = _find_simplified(child)
            if r is not None:
                return r
        return None

    node = _find_simplified(root.find("cellular_information"))
    if node is None:
        raise RuntimeError(f"Could not find PhysiCell simplified_data labels in {xml_fname}")
    labels = node.find("labels")
    if labels is None:
        raise RuntimeError(f"Could not find labels in {xml_fname}")

    cols: list[str] = []
    for child in labels:
        name = child.text.strip()
        size = int(child.attrib["size"])
        if size == 1:
            cols.append(name)
        elif size == 3:
            cols.extend([f"x_{name}", f"y_{name}", f"z_{name}"])
        elif size == 2:
            cols.extend([f"x_{name}", f"y_{name}"])
        else:
            cols.extend([f"{name}_{i}" for i in range(size)])
    return cols


def ensure_rows(a: np.ndarray, ncols: int) -> np.ndarray:
    a = np.asarray(a)
    if a.ndim != 2:
        raise ValueError(f"Expected 2-D matrix, got shape {a.shape}")
    if a.shape[1] == ncols:
        return a
    if a.shape[0] == ncols:
        return a.T
    raise ValueError(f"Cannot orient matrix: shape={a.shape}, expected ncols={ncols}")


def get_domain_bounds(reader: multicellds.MultiCellDS) -> tuple[tuple[float, float], tuple[float, float]]:
    root = reader._tree.getroot()
    bb_text = root.find(".//microenvironment/domain/mesh/bounding_box").text
    xmin, ymin, _zmin, xmax, ymax, _zmax = map(float, bb_text.split())
    return (xmin, xmax), (ymin, ymax)


def load_aggregate_timeseries(reader: multicellds.MultiCellDS) -> pd.DataFrame:
    cols = get_cell_columns(reader._output_folder)
    name2idx = {n: i for i, n in enumerate(cols)}

    required = ["x_position", "y_position", "migration_speed", "growth_rate",
                "atp_flux", "motility_atp_flux", "glucose_flux"]
    missing = [c for c in required if c not in name2idx]
    if missing:
        raise ValueError(f"Missing required cell columns: {missing}\nAvailable: {cols}")

    rows = []
    for t_min, a in reader.cells_as_matrix_iterator():
        a = ensure_rows(a, len(cols))
        row = {
            "time_h": t_min / 60.0,
            "n_cells": a.shape[0],
            "x_position": float(np.mean(a[:, name2idx["x_position"]])),
            "y_position": float(np.mean(a[:, name2idx["y_position"]])),
            "migration_speed": float(np.mean(a[:, name2idx["migration_speed"]])),
            "growth_rate": float(np.mean(a[:, name2idx["growth_rate"]])),
            "atp_flux": float(np.mean(a[:, name2idx["atp_flux"]])),
            "motility_atp_flux": float(np.mean(a[:, name2idx["motility_atp_flux"]])),
            "glucose_flux": float(np.mean(a[:, name2idx["glucose_flux"]])),
        }
        rows.append(row)
    if not rows:
        raise RuntimeError("No cell time series found.")
    return pd.DataFrame(rows)


def load_cell_snapshots(
    reader: multicellds.MultiCellDS,
    n_frames: int,
    color_col: str,
    frame_spacing: str = "linear",
) -> list[dict]:
    cols = get_cell_columns(reader._output_folder)
    name2idx = {n: i for i, n in enumerate(cols)}
    needed = ["x_position", "y_position", "total_volume", color_col]
    missing = [c for c in needed if c not in name2idx]
    if missing:
        raise ValueError(f"Missing required cell columns: {missing}")

    all_items = list(reader.cells_as_matrix_iterator())
    n_total = len(all_items)
    if n_total == 0:
        raise RuntimeError("No cell snapshots found.")
    if n_total <= 1:
        indices = np.array([0], dtype=int)
    elif frame_spacing == "log":
        raw = np.geomspace(1, n_total, min(n_frames, n_total))
        indices = np.unique(np.clip(np.round(raw).astype(int) - 1, 0, n_total - 1))
    else:
        indices = np.unique(np.linspace(0, n_total - 1, min(n_frames, n_total)).astype(int))

    snapshots = []
    for idx in indices:
        t_min, a = all_items[idx]
        a = ensure_rows(a, len(cols))
        snapshots.append({
            "time_h": t_min / 60.0,
            "x": a[:, name2idx["x_position"]],
            "y": a[:, name2idx["y_position"]],
            "total_volume": a[:, name2idx["total_volume"]],
            color_col: a[:, name2idx[color_col]],
        })
    return snapshots


def glucose_grid(
    reader: multicellds.MultiCellDS,
    frame: str,
    domain_x: tuple[float, float],
    domain_y: tuple[float, float],
    grid_nx: int = 700,
    grid_ny: int = 160,
) -> tuple[np.ndarray, np.ndarray, float, float]:
    frames = list(reader.microenvironment_as_matrix_iterator())
    if not frames:
        raise RuntimeError("No microenvironment data found.")
    if frame == "first":
        _t, m = frames[0]
    else:
        _t, m = frames[-1]

    vox_x = m[0, :]
    vox_y = m[1, :]
    glc = m[MICRO_IDX_GLUCOSE, :]

    xi = np.linspace(domain_x[0], domain_x[1], grid_nx)
    yi = np.linspace(domain_y[0], domain_y[1], grid_ny)
    Xi, Yi = np.meshgrid(xi, yi)
    Gi = griddata((vox_x, vox_y), glc, (Xi, Yi), method="linear")

    return Gi, glc, float(np.nanmin(glc)), float(np.nanmax(glc))


def infer_glucose_boundary_x(reader: multicellds.MultiCellDS, domain_x: tuple[float, float]) -> float:
    frames = list(reader.microenvironment_as_matrix_iterator())
    _t, m = frames[-1]
    vox_x = m[0, :]
    glc = m[MICRO_IDX_GLUCOSE, :]
    left_mask = vox_x <= np.percentile(vox_x, 5)
    right_mask = vox_x >= np.percentile(vox_x, 95)
    left_mean = float(np.nanmean(glc[left_mask]))
    right_mean = float(np.nanmean(glc[right_mask]))
    return domain_x[1] if right_mean >= left_mean else domain_x[0]


# -----------------------------------------------------------------------------
# Scenario metadata and selection
# -----------------------------------------------------------------------------


_PARAM_RE = re.compile(r"hill(?P<hill>\d+)_km(?P<km>\d+)_glc(?P<glc>\d+)")


@dataclass
class Scenario:
    output_dir: Path
    title: str
    short_title: str
    df: pd.DataFrame
    reader: multicellds.MultiCellDS
    domain_x: tuple[float, float]
    domain_y: tuple[float, float]
    boundary_x: float
    params: dict
    summary: dict


def parse_params(path: Path) -> dict:
    m = _PARAM_RE.search(str(path))
    if not m:
        return {"hill": np.nan, "km": np.nan, "glc": np.nan, "param_label": path.parent.name}
    hill = int(m.group("hill"))
    km_raw = m.group("km")
    glc_raw = m.group("glc")
    return {
        "hill": hill,
        "km": int(km_raw) / 100.0,
        "glc": int(glc_raw) / 100.0,
        "km_raw": km_raw,
        "glc_raw": glc_raw,
        "param_label": f"Hill {hill}, Km {int(km_raw) / 100.0:.2f}, Glc {int(glc_raw) / 100.0:.2f}",
    }


def summarize_run(output_dir: Path, growth_threshold: float, movement_threshold: float) -> tuple[pd.DataFrame, multicellds.MultiCellDS, tuple, tuple, float, dict]:
    reader = multicellds.MultiCellDS(str(output_dir))
    domain_x, domain_y = get_domain_bounds(reader)
    boundary_x = infer_glucose_boundary_x(reader, domain_x)
    df = load_aggregate_timeseries(reader)

    x0 = float(df["x_position"].iloc[0])
    direction = 1.0 if boundary_x >= x0 else -1.0
    df["displacement_toward_glucose"] = (df["x_position"] - x0) * direction
    df["displacement_toward_glucose"] = df["displacement_toward_glucose"].clip(lower=0.0)

    max_migration = float(df["migration_speed"].max())
    max_disp = float(df["displacement_toward_glucose"].max())
    final_growth = float(df["growth_rate"].iloc[-1])
    max_growth = float(df["growth_rate"].max())
    growth_detected = bool(max_growth > growth_threshold)
    movement_detected = bool((max_migration > movement_threshold) or (max_disp > 5.0))

    if movement_detected and growth_detected:
        behavior = "motility + growth recovery"
    elif movement_detected and not growth_detected:
        behavior = "motility / no growth"
    else:
        behavior = "no motility / no growth"

    summary = {
        "max_migration_speed": max_migration,
        "max_displacement_toward_glucose": max_disp,
        "final_growth_rate": final_growth,
        "max_growth_rate": max_growth,
        "growth_detected": int(growth_detected),
        "movement_detected": int(movement_detected),
        "behavior": behavior,
        "boundary_x": boundary_x,
    }
    return df, reader, domain_x, domain_y, boundary_x, summary


def make_scenario(output_dir: Path, title: str | None, growth_threshold: float, movement_threshold: float) -> Scenario:
    df, reader, domain_x, domain_y, boundary_x, summary = summarize_run(output_dir, growth_threshold, movement_threshold)
    params = parse_params(output_dir)
    if title is None or title.strip() == "":
        title = f"{summary['behavior']}\n{params['param_label']}"
    short_title = str(title).split("\n")[0]
    return Scenario(output_dir, title, short_title, df, reader, domain_x, domain_y, boundary_x, params, summary)


def find_output_dirs(runs_dir: Path) -> list[Path]:
    candidates = []
    for p in sorted(runs_dir.glob("hill*_job*/output")):
        if (p / "initial.xml").exists():
            candidates.append(p)
    return candidates


def auto_select_scenarios(
    runs_dir: Path,
    growth_threshold: float,
    movement_threshold: float,
    max_scenarios: int = 3,
) -> list[Scenario]:
    rows = []
    print(f"Scanning runs in: {runs_dir}")
    for out in find_output_dirs(runs_dir):
        try:
            df, reader, domain_x, domain_y, boundary_x, summary = summarize_run(out, growth_threshold, movement_threshold)
            params = parse_params(out)
            rows.append({
                "output_dir": out,
                "df": df,
                "reader": reader,
                "domain_x": domain_x,
                "domain_y": domain_y,
                "boundary_x": boundary_x,
                "params": params,
                **summary,
            })
        except Exception as e:
            print(f"  WARNING: skipping {out}: {e}")

    if not rows:
        raise RuntimeError(f"No valid outputs found in {runs_dir}")

    meta = pd.DataFrame([{k: v for k, v in r.items() if k not in {"df", "reader", "domain_x", "domain_y", "params"}} for r in rows])
    print("Available behavior classes:")
    print(meta["behavior"].value_counts().to_string())

    selected = []

    def _row_to_scenario(r, title_prefix):
        params = r["params"]
        title = f"{title_prefix}\n{params['param_label']}"
        return Scenario(
            output_dir=r["output_dir"],
            title=title,
            short_title=title_prefix,
            df=r["df"],
            reader=r["reader"],
            domain_x=r["domain_x"],
            domain_y=r["domain_y"],
            boundary_x=r["boundary_x"],
            params=params,
            summary={
                "max_migration_speed": r["max_migration_speed"],
                "max_displacement_toward_glucose": r["max_displacement_toward_glucose"],
                "final_growth_rate": r["final_growth_rate"],
                "max_growth_rate": r["max_growth_rate"],
                "growth_detected": r["growth_detected"],
                "movement_detected": r["movement_detected"],
                "behavior": r["behavior"],
                "boundary_x": r["boundary_x"],
            },
        )

    # 1. No motility / no growth: choose the most static example.
    no_rows = [r for r in rows if r["behavior"] == "no motility / no growth"]
    if no_rows:
        r = sorted(no_rows, key=lambda x: (x["max_migration_speed"], x["max_displacement_toward_glucose"]))[0]
        selected.append(_row_to_scenario(r, "No motility / no growth"))

    # 2. Motility / no growth: choose the largest displacement without growth.
    mot_rows = [r for r in rows if r["behavior"] == "motility / no growth"]
    if mot_rows:
        r = sorted(mot_rows, key=lambda x: (x["max_displacement_toward_glucose"], x["max_migration_speed"]), reverse=True)[0]
        selected.append(_row_to_scenario(r, "Motility / no growth"))

    # 3. Motility + growth recovery: choose strongest final growth.
    grow_rows = [r for r in rows if r["behavior"] == "motility + growth recovery"]
    if grow_rows:
        r = sorted(grow_rows, key=lambda x: (x["final_growth_rate"], x["max_growth_rate"]), reverse=True)[0]
        selected.append(_row_to_scenario(r, "Motility + growth recovery"))

    # Fallback if fewer than requested.
    if len(selected) < min(max_scenarios, 3):
        already = {str(s.output_dir) for s in selected}
        remaining = [r for r in rows if str(r["output_dir"]) not in already]
        remaining = sorted(remaining, key=lambda x: (x["growth_detected"], x["max_displacement_toward_glucose"]), reverse=True)
        for r in remaining:
            if len(selected) >= max_scenarios:
                break
            selected.append(_row_to_scenario(r, r["behavior"].capitalize()))

    return selected[:max_scenarios]


# -----------------------------------------------------------------------------
# Plotting
# -----------------------------------------------------------------------------


def truncated_cmap(name: str, low: float = 0.15, high: float = 0.95, n: int = 256):
    base = mpl.colormaps[name]
    colors = base(np.linspace(low, high, n))
    return mpl.colors.LinearSegmentedColormap.from_list(f"{name}_trunc", colors)


def metric_for_color(color_by: str) -> tuple[str, str, str]:
    if color_by == "growth_rate":
        return "growth_rate", "Growth rate (h$^{-1}$)", "Greens"
    if color_by == "velocity":
        return "migration_speed", "Migration speed (µm/min)", "summer"
    raise ValueError("color_by must be 'velocity' or 'growth_rate'")


def draw_composite_panel(
    ax,
    scenario: Scenario,
    snapshots: list[dict],
    Gi: np.ndarray,
    domain_x: tuple[float, float],
    domain_y: tuple[float, float],
    glc_norm: Normalize,
    glc_cmap,
    cell_norm: Normalize,
    cell_cmap,
    color_col: str,
    cell_radius: float | None,
    x_scale: str,
    show_time_labels: bool = True,
):
    ax.imshow(
        Gi,
        origin="lower",
        extent=[domain_x[0], domain_x[1], domain_y[0], domain_y[1]],
        norm=glc_norm,
        cmap=glc_cmap,
        interpolation="bilinear",
        aspect="auto",
        zorder=1,
        alpha=0.92,
    )

    n_snap = len(snapshots)
    label_min_sep = (domain_x[1] - domain_x[0]) / max(n_snap, 1) * 0.55
    last_label_x = -np.inf

    for k, snap in enumerate(snapshots):
        xs = snap["x"]
        ys = snap["y"]
        vols = snap["total_volume"]
        metric = snap[color_col]
        time_h = snap["time_h"]

        for i in range(len(xs)):
            r = cell_radius if cell_radius is not None else (3.0 * vols[i] / (4.0 * np.pi)) ** (1.0 / 3.0)
            # white halo for contrast against blue background
            halo = Circle((xs[i], ys[i]), radius=r * 1.18, facecolor="white", edgecolor="none",
                          alpha=0.92, zorder=2 + k)
            ax.add_patch(halo)
            circ = Circle((xs[i], ys[i]), radius=r,
                          facecolor=cell_cmap(cell_norm(metric[i])),
                          edgecolor="#145A32", linewidth=0.45,
                          alpha=0.96, zorder=3 + k)
            ax.add_patch(circ)

        if show_time_labels:
            cx_mean = float(np.mean(xs))
            if abs(cx_mean - last_label_x) >= label_min_sep or k in {0, n_snap - 1}:
                trans = ax.get_xaxis_transform()
                ax.plot([cx_mean, cx_mean], [1.0, 1.025], transform=trans,
                        color="0.55", lw=0.5, clip_on=False, zorder=20)
                ax.text(cx_mean, 1.035, f"{time_h:.1f} h", transform=trans,
                        ha="center", va="bottom", fontsize=5.5, color="0.2",
                        clip_on=False, zorder=21)
                last_label_x = cx_mean

    ax.set_xlim(domain_x[0], domain_x[1])
    ax.set_ylim(domain_y[0] - 2, domain_y[1] + 2)
    ax.set_aspect("equal", adjustable="box")
    if x_scale == "symlog":
        ax.set_xscale("symlog", linthresh=20, linscale=0.5)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_representative_figure(
    scenarios: list[Scenario],
    results_dir: Path,
    out_name: str,
    composite_color_by: str = "growth_rate",
    n_frames: int = 7,
    frame_spacing: str = "linear",
    cell_radius: float | None = 4.0,
    glc_frame: str = "last",
    glc_cmap_name: str = "Blues",
    figsize_per_col: float = 2.55,
    x_scale: str = "linear",
):
    apply_style()
    n = len(scenarios)
    if n < 1:
        raise ValueError("Need at least one scenario")

    color_col, cell_label, default_cell_cmap = metric_for_color(composite_color_by)
    cell_cmap_name = default_cell_cmap
    cell_cmap = truncated_cmap(cell_cmap_name, low=0.20, high=0.92)
    glc_cmap = mpl.colormaps[glc_cmap_name]

    # Preload composite data so we can use shared color scales.
    comp_data = []
    all_glc_values = []
    all_cell_values = []
    global_domain_x = None
    global_domain_y = None

    for s in scenarios:
        domain_x, domain_y = s.domain_x, s.domain_y
        if global_domain_x is None:
            global_domain_x, global_domain_y = domain_x, domain_y
        snapshots = load_cell_snapshots(s.reader, n_frames=n_frames, color_col=color_col, frame_spacing=frame_spacing)
        Gi, glc_vals, glc_min, glc_max = glucose_grid(s.reader, glc_frame, domain_x, domain_y)
        all_glc_values.append(glc_vals)
        all_cell_values.append(np.concatenate([snap[color_col] for snap in snapshots]))
        comp_data.append({"snapshots": snapshots, "Gi": Gi, "domain_x": domain_x, "domain_y": domain_y})

    glc_all = np.concatenate(all_glc_values)
    glc_norm = Normalize(vmin=float(np.nanmin(glc_all)), vmax=float(np.nanmax(glc_all)))

    cell_all = np.concatenate(all_cell_values)
    if composite_color_by == "velocity":
        cell_norm = Normalize(vmin=0.0, vmax=max(1.0, float(np.nanmax(cell_all))))
    else:
        vmax = float(np.nanmax(cell_all))
        if vmax <= 0:
            vmax = 1e-9
        cell_norm = Normalize(vmin=0.0, vmax=vmax)

    # Figure layout: 4 data rows x n scenarios + 2 colorbar columns.
    fig_w = max(7.2, figsize_per_col * n + 0.85)
    fig_h = 6.2 if n == 3 else 5.8
    fig = plt.figure(figsize=(fig_w, fig_h), constrained_layout=False)
    gs = fig.add_gridspec(
        nrows=4,
        ncols=n + 2,
        width_ratios=[1] * n + [0.045, 0.045],
        height_ratios=[1.25, 0.78, 0.78, 0.9],
        left=0.065,
        right=0.955,
        bottom=0.10,
        top=0.91,
        wspace=0.34,
        hspace=0.33,
    )

    axes = [[fig.add_subplot(gs[r, c]) for c in range(n)] for r in range(4)]
    cax_glc = fig.add_subplot(gs[0, n])
    cax_cell = fig.add_subplot(gs[0, n + 1])
    # Leave colorbar columns invisible in lower rows.
    for r in range(1, 4):
        ax_blank1 = fig.add_subplot(gs[r, n])
        ax_blank2 = fig.add_subplot(gs[r, n + 1])
        ax_blank1.axis("off")
        ax_blank2.axis("off")

    panel_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

    # Top composite row
    for c, s in enumerate(scenarios):
        ax = axes[0][c]
        d = comp_data[c]
        draw_composite_panel(
            ax, s, d["snapshots"], d["Gi"], d["domain_x"], d["domain_y"],
            glc_norm, glc_cmap, cell_norm, cell_cmap, color_col,
            cell_radius=cell_radius, x_scale=x_scale, show_time_labels=True,
        )
        ax.set_title(s.title, pad=13, fontsize=8)
        if c == 0:
            ax.set_ylabel("y position (µm)")
        else:
            ax.set_yticklabels([])
        ax.set_xlabel("Position along gradient axis (µm)")
        ax.text(-0.12, 1.16, panel_letters[c], transform=ax.transAxes,
                fontsize=10, fontweight="bold", va="top", ha="left")

    cb_glc = fig.colorbar(ScalarMappable(norm=glc_norm, cmap=glc_cmap), cax=cax_glc)
    cb_glc.set_label("Glucose (mM)", labelpad=3)
    cb_glc.ax.tick_params(labelsize=5, length=2)
    cb_glc.outline.set_linewidth(0.5)

    cb_cell = fig.colorbar(ScalarMappable(norm=cell_norm, cmap=cell_cmap), cax=cax_cell)
    cb_cell.set_label(cell_label, labelpad=3)
    cb_cell.ax.tick_params(labelsize=5, length=2)
    cb_cell.outline.set_linewidth(0.5)

    # Determine shared y-limits for line rows.
    max_speed = max(float(s.df["migration_speed"].max()) for s in scenarios)
    max_growth = max(float(s.df["growth_rate"].max()) for s in scenarios)
    max_flux = max(float(max(s.df["atp_flux"].max(), s.df["motility_atp_flux"].max(), s.df["glucose_flux"].abs().max())) for s in scenarios)
    max_t = max(float(s.df["time_h"].max()) for s in scenarios)

    speed_ylim = (0, max(0.05, max_speed * 1.08))
    growth_ylim = (0, max(0.05, max_growth * 1.12))
    flux_ylim = (0, max(1.0, max_flux * 1.12))

    line_colors = {
        "speed": "#0072B2",
        "growth": "#009E73",
        "atp": "#3366AA",
        "mot_atp": "#88CCEE",
        "glucose": "#CC6677",
    }

    for c, s in enumerate(scenarios):
        df = s.df
        t = df["time_h"]

        # Migration speed
        ax = axes[1][c]
        ax.plot(t, df["migration_speed"], color=line_colors["speed"])
        ax.set_ylim(speed_ylim)
        ax.set_xlim(0, max_t)
        if c == 0:
            ax.set_ylabel("Migration speed\n(µm/min)")
        else:
            ax.set_yticklabels([])
        ax.set_xticklabels([])
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.text(-0.12, 1.08, panel_letters[n + c], transform=ax.transAxes,
                fontsize=10, fontweight="bold", va="top", ha="left")

        # Growth rate
        ax = axes[2][c]
        ax.plot(t, df["growth_rate"], color=line_colors["growth"])
        ax.set_ylim(growth_ylim)
        ax.set_xlim(0, max_t)
        if c == 0:
            ax.set_ylabel("Growth rate\n(h$^{-1}$)")
        else:
            ax.set_yticklabels([])
        ax.set_xticklabels([])
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.text(-0.12, 1.08, panel_letters[2 * n + c], transform=ax.transAxes,
                fontsize=10, fontweight="bold", va="top", ha="left")

        # Fluxes
        ax = axes[3][c]
        ax.plot(t, df["atp_flux"], color=line_colors["atp"], label="Total ATP flux")
        ax.plot(t, df["motility_atp_flux"], color=line_colors["mot_atp"], label="Motility ATP flux")
        ax.plot(t, df["glucose_flux"].abs(), color=line_colors["glucose"], linestyle="--", label="|Glucose flux|")
        ax.set_ylim(flux_ylim)
        ax.set_xlim(0, max_t)
        ax.set_xlabel("Time (h)")
        if c == 0:
            ax.set_ylabel("Flux\n(mmol·gDW$^{-1}$·h$^{-1}$)")
        else:
            ax.set_yticklabels([])
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.text(-0.12, 1.08, panel_letters[3 * n + c], transform=ax.transAxes,
                fontsize=10, fontweight="bold", va="top", ha="left")

    # A single flux legend below the line panels.
    handles, labels = axes[3][-1].get_legend_handles_labels()
    fig.legend(handles, labels, loc="lower center", ncol=3, frameon=False,
               bbox_to_anchor=(0.52, 0.025), fontsize=6)

    results_dir.mkdir(parents=True, exist_ok=True)
    out_svg = results_dir / f"{out_name}.svg"
    out_png = results_dir / f"{out_name}.png"
    fig.savefig(out_svg)
    fig.savefig(out_png, dpi=300)
    plt.close(fig)
    print(f"Saved: {out_svg}")
    print(f"Saved: {out_png}")


def write_metrics_csv(scenarios: list[Scenario], results_dir: Path) -> None:
    rows = []
    for s in scenarios:
        row = {
            "output_dir": str(s.output_dir),
            "title": s.title.replace("\n", " | "),
            **s.params,
            **s.summary,
        }
        rows.append(row)
    df = pd.DataFrame(rows)
    path = results_dir / "representative_scenarios_metrics.csv"
    df.to_csv(path, index=False)
    print(f"Saved: {path}")


# -----------------------------------------------------------------------------
# CLI
# -----------------------------------------------------------------------------


def parse_scenario_arg(s: str) -> tuple[Path, str | None]:
    if "::" in s:
        path, title = s.split("::", 1)
        return Path(path), title.replace("\\n", "\n")
    return Path(s), None


def main(argv=None) -> None:
    p = argparse.ArgumentParser(
        description="Create a journal-style representative scenario comparison figure.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    p.add_argument("--runs-dir", default="runs", help="Runs directory used for automatic scenario selection")
    p.add_argument("--results-dir", default="results/representative_scenarios", help="Output directory")
    p.add_argument("--scenario", action="append", default=[],
                   help="Manual scenario as output_dir::Title. Repeat 2 or 3 times. If omitted, scenarios are auto-selected from --runs-dir.")
    p.add_argument("--out-name", default="fig_representative_scenarios", help="Output basename without extension")
    p.add_argument("--growth-threshold", type=float, default=1e-3, help="Growth-rate threshold for automatic scenario classification")
    p.add_argument("--movement-threshold", type=float, default=1e-3, help="Migration-speed threshold for automatic scenario classification")
    p.add_argument("--max-scenarios", type=int, default=3, choices=[2, 3], help="Number of scenarios to plot when auto-selecting")
    p.add_argument("--composite-color-by", default="growth_rate", choices=["growth_rate", "velocity"],
                   help="Cell color in the top trajectory panels")
    p.add_argument("--n-frames", type=int, default=7, help="Number of cell snapshots shown in the composite panels")
    p.add_argument("--frame-spacing", default="linear", choices=["linear", "log"], help="Snapshot sampling for composite panels")
    p.add_argument("--cell-radius", type=float, default=4.0, help="Display cell radius in µm. Use a larger value for visibility.")
    p.add_argument("--glc-frame", default="last", choices=["first", "last"], help="Microenvironment frame used for glucose background")
    p.add_argument("--glc-cmap", default="Blues", help="Matplotlib colormap for glucose background")
    p.add_argument("--x-scale", default="linear", choices=["linear", "symlog"], help="X-axis scaling for composite panels")
    args = p.parse_args(argv)

    results_dir = Path(args.results_dir)
    results_dir.mkdir(parents=True, exist_ok=True)

    if args.scenario:
        scenarios = []
        for scen_arg in args.scenario:
            path, title = parse_scenario_arg(scen_arg)
            if not path.exists():
                raise FileNotFoundError(f"Scenario output directory does not exist: {path}")
            scenarios.append(make_scenario(path, title, args.growth_threshold, args.movement_threshold))
        if len(scenarios) not in (2, 3):
            raise ValueError("Please provide either 2 or 3 --scenario arguments.")
    else:
        scenarios = auto_select_scenarios(Path(args.runs_dir), args.growth_threshold, args.movement_threshold, args.max_scenarios)

    print("Selected scenarios:")
    for s in scenarios:
        print(f"  - {s.title.replace(chr(10), ' | ')}")
        print(f"    {s.output_dir}")

    write_metrics_csv(scenarios, results_dir)
    plot_representative_figure(
        scenarios=scenarios,
        results_dir=results_dir,
        out_name=args.out_name,
        composite_color_by=args.composite_color_by,
        n_frames=args.n_frames,
        frame_spacing=args.frame_spacing,
        cell_radius=args.cell_radius,
        glc_frame=args.glc_frame,
        glc_cmap_name=args.glc_cmap,
        x_scale=args.x_scale,
    )


if __name__ == "__main__":
    main()