#!/usr/bin/env python3
"""Dynamics analysis for PhysiCelldFBA – metabolic-driven motility simulation.

Figures produced
----------------
fig01_population_dynamics.svg  – 3-panel: migration speed / growth rate / ATP fluxes
fig02_phase_portrait.svg       – growth rate vs migration speed coloured by local glucose
animation_cells_glucose.gif    – cells (circles) over glucose gradient field

Usage
-----
    python dynamics_analysis.py [--output-dir DIR] [--results-dir DIR]
                                 [--sample-every N] [--fps N]
                                 [--anim-sample-every N] [--no-animation]
"""

from __future__ import annotations

import argparse
import io
import os
import glob
import xml.etree.ElementTree as ET
from pathlib import Path

import matplotlib as mpl
mpl.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.colors import Normalize
from matplotlib.cm import ScalarMappable
import matplotlib.cm as cm
import numpy as np
import pandas as pd
from scipy.interpolate import griddata
from pctk import multicellds

# ---------------------------------------------------------------------------
# Default paths
# ---------------------------------------------------------------------------
_SCRIPT_DIR = Path(__file__).resolve().parent
_REPO_ROOT   = _SCRIPT_DIR.parent
DEFAULT_OUTPUT_DIR  = str(_REPO_ROOT / "output")
DEFAULT_RESULTS_DIR = str(_REPO_ROOT / "results" / "dynamics_analysis")

# Microenvironment substrate column indices (0-2 = x/y/z; 3 = voxel vol; 4+ = substrates)
MICRO_IDX_OXYGEN  = 4
MICRO_IDX_GLUCOSE = 5
MICRO_IDX_ACETATE = 6


# ===========================================================================
# Helpers
# ===========================================================================

def get_cell_columns(output_folder: str) -> list[str]:
    """Return flat list of cell-matrix column labels from initial.xml."""
    xml_fname = os.path.join(output_folder, "initial.xml")
    tree = ET.parse(xml_fname)
    root = tree.getroot()

    def _find_simplified(node):
        for child in node:
            if child.tag == "simplified_data" and child.attrib.get("source") == "PhysiCell":
                return child
        for child in node:
            r = _find_simplified(child)
            if r is not None:
                return r

    node   = _find_simplified(root.find("cellular_information"))
    labels = node.find("labels")

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


def _ensure_rows(a: np.ndarray, ncols: int) -> np.ndarray:
    a = np.asarray(a)
    if a.ndim != 2:
        raise ValueError(f"Expected 2-D array, got shape {a.shape}")
    if a.shape[1] == ncols:
        return a
    if a.shape[0] == ncols:
        return a.T
    raise ValueError(f"Cannot orient matrix: shape={a.shape}, expected ncols={ncols}")


def get_domain_bounds(reader: multicellds.MultiCellDS) -> tuple[tuple, tuple]:
    root    = reader._tree.getroot()
    bb_text = root.find(".//microenvironment/domain/mesh/bounding_box").text
    xmin, ymin, _zmin, xmax, ymax, _zmax = map(float, bb_text.split())
    return (xmin, xmax), (ymin, ymax)


def _glucose_at_cells(micro_a: np.ndarray, cell_x: np.ndarray, cell_y: np.ndarray) -> np.ndarray:
    """Return glucose concentration at each cell position via nearest-voxel lookup."""
    vox_x = micro_a[0, :]
    vox_y = micro_a[1, :]
    glc   = micro_a[MICRO_IDX_GLUCOSE, :]
    # Squared distance (n_cells, n_voxels) – feasible for small cell counts
    dist2   = (cell_x[:, None] - vox_x[None, :]) ** 2 + (cell_y[:, None] - vox_y[None, :]) ** 2
    nearest = np.argmin(dist2, axis=1)
    return glc[nearest]


# ===========================================================================
# Cell columns used (all verified present in simulation output)
# ===========================================================================
_CELL_COLS = [
    "ID", "x_position", "y_position",
    "total_volume",
    "migration_speed",           # µm/min
    "growth_rate",               # dFBA-derived (h⁻¹)
    "atp_flux",                  # mmol·gDW⁻¹·h⁻¹
    "motility_atp_flux",
    "glucose_flux",
]


# ===========================================================================
# Data loading
# ===========================================================================

def build_per_cell_dataframe(
    reader: multicellds.MultiCellDS,
    sample_every: int = 1,
    verbose: bool = True,
) -> pd.DataFrame:
    """Long-format per-cell DataFrame.

    Iterates cell and microenvironment frames together so that local glucose
    concentration at each cell's position can be recorded.
    """
    cols     = get_cell_columns(reader._output_folder)
    name2idx = {n: i for i, n in enumerate(cols)}

    missing = [c for c in _CELL_COLS if c not in name2idx]
    if missing:
        raise ValueError(f"Required columns not found: {missing}\nAvailable: {cols}")

    dfs = []
    cell_iter  = reader.cells_as_matrix_iterator()
    micro_iter = reader.microenvironment_as_matrix_iterator()

    for k, ((t_c, cells_a), (_t_m, micro_a)) in enumerate(zip(cell_iter, micro_iter)):
        if k % sample_every != 0:
            continue
        cells_a = _ensure_rows(cells_a, len(cols))
        time_h  = t_c / 60.0

        cx = cells_a[:, name2idx["x_position"]]
        cy = cells_a[:, name2idx["y_position"]]

        df_t = pd.DataFrame({c: cells_a[:, name2idx[c]] for c in _CELL_COLS})
        df_t["ID"]           = df_t["ID"].astype(int)
        df_t["time"]         = time_h
        df_t["glucose_local"] = _glucose_at_cells(micro_a, cx, cy)
        dfs.append(df_t)

        if verbose and len(dfs) == 1:
            print(f"  First snapshot (t={time_h:.2f} h): {cells_a.shape[0]} cells")
            for c in ["migration_speed", "growth_rate", "atp_flux", "glucose_local"]:
                v = df_t[c]
                print(f"    {c:25s}: [{v.min():.4f}, {v.max():.4f}]  mean={v.mean():.4f}")

    if not dfs:
        raise RuntimeError("No cell data found in output folder.")
    return pd.concat(dfs, ignore_index=True)


def load_cell_aggregate_timeseries(reader: multicellds.MultiCellDS) -> pd.DataFrame:
    """Population-mean cell variables over time."""
    cols     = get_cell_columns(reader._output_folder)
    name2idx = {n: i for i, n in enumerate(cols)}

    data = []
    for t, a in reader.cells_as_matrix_iterator():
        a   = _ensure_rows(a, len(cols))
        row = {"time": t / 60.0, "n_cells": a.shape[0]}
        for c in ["migration_speed", "growth_rate", "atp_flux",
                  "motility_atp_flux", "glucose_flux"]:
            if c in name2idx:
                row[c] = a[:, name2idx[c]].mean()
        data.append(row)
    return pd.DataFrame(data).set_index("time")


# ===========================================================================
# Style
# ===========================================================================

def _apply_style() -> None:
    mpl.rcParams.update({
        "font.family":       "sans-serif",
        "font.sans-serif":   ["Arial", "Helvetica", "DejaVu Sans"],
        "font.size":         8,
        "axes.labelsize":    8,
        "axes.titlesize":    9,
        "xtick.labelsize":   7,
        "ytick.labelsize":   7,
        "legend.fontsize":   7,
        "lines.linewidth":   1.5,
        "axes.linewidth":    0.6,
        "axes.spines.top":   False,
        "axes.spines.right": False,
        "figure.dpi":        150,
        "savefig.dpi":       200,
        "savefig.bbox":      "tight",
    })


def _savefig(path: str) -> None:
    os.makedirs(os.path.dirname(path) if os.path.dirname(path) else ".", exist_ok=True)
    plt.savefig(path, bbox_inches="tight")
    print(f"  Saved: {path}")


# ===========================================================================
# Figure 1 – Population mean: migration speed, growth rate, ATP fluxes
# ===========================================================================

def plot_population_dynamics(df: pd.DataFrame, out_path: str) -> None:
    """Three-panel overview of migration speed, growth rate, and ATP fluxes."""
    _apply_style()
    fig, axes = plt.subplots(3, 1, figsize=(7, 6.5), sharex=True, constrained_layout=True)

    axes[0].plot(df.index, df["migration_speed"], color="#e6550d")
    axes[0].set_ylabel("Migration speed\n(µm/min)")

    axes[1].plot(df.index, df["growth_rate"], color="#31a354")
    axes[1].set_ylabel("Growth rate\n(h⁻¹)")

    axes[2].plot(df.index, df["atp_flux"],          color="#3182bd", label="Total ATP flux")
    axes[2].plot(df.index, df["motility_atp_flux"],  color="#9ecae1", label="Motility ATP flux")
    axes[2].plot(df.index, df["glucose_flux"].abs(), color="#756bb1", label="|Glucose flux|",
                 linestyle="--")
    axes[2].set_ylabel("Flux\n(mmol·gDW⁻¹·h⁻¹)")
    axes[2].set_xlabel("Time (h)")
    axes[2].legend(frameon=False)

    axes[0].set_title("Population-mean metabolic & motility dynamics")
    _savefig(out_path)
    plt.close(fig)


# ===========================================================================
# Figure 2 – Phase portrait: growth rate vs migration speed, coloured by
#             local glucose concentration at each cell's position
# ===========================================================================

def plot_phase_portrait(
    df: pd.DataFrame,
    out_path: str,
) -> None:
    """Scatter of growth rate vs migration speed; colour = local glucose (mM).

    Three behavioural phases:
      (1) Quiescent     – low growth, low speed    (low glucose)
      (2) Motility      – low growth, high speed   (intermediate glucose)
      (3) Proliferation – high growth, low speed   (high glucose)
    """
    _apply_style()

    glc  = df["glucose_local"].values
    g_lo = float(np.percentile(glc, 1))
    g_hi = float(np.percentile(glc, 99))

    fig, ax = plt.subplots(figsize=(5.5, 4.5), constrained_layout=True)
    sc = ax.scatter(
        df["growth_rate"], df["migration_speed"],
        c=glc, cmap="YlOrRd", vmin=g_lo, vmax=g_hi,
        s=20, alpha=0.7, linewidths=0.4, edgecolors="#333333",
    )
    cbar = fig.colorbar(sc, ax=ax)
    cbar.set_label("Local glucose (mM)")

    # Phase annotations positioned in data coordinates near actual clusters.
    # x-limits span [~0, growth_max]; y-limits span [~0, speed_max].
    # Offsets expressed as fractions so they adapt to actual data range.
    gr_max = float(df["growth_rate"].quantile(0.99))
    sp_max = float(df["migration_speed"].quantile(0.99))
    ann_kw = dict(fontsize=7, style="italic",
                  bbox=dict(boxstyle="round,pad=0.25", fc="white", ec="0.6",
                            alpha=0.85, linewidth=0.5))
    # (1) Quiescent: bottom-left cluster
    ax.text(gr_max * 0.05, sp_max * 0.08,
            "(1) Quiescent\nlow growth · low speed",
            color="#444444", ha="left", va="bottom", **ann_kw)
    # (2) Motility: top-left cluster
    ax.text(gr_max * 0.05, sp_max * 0.92,
            "(2) Motility\nlow growth · high speed",
            color="#7a5500", ha="left", va="top", **ann_kw)
    # (3) Proliferation: bottom-right cluster
    ax.text(gr_max * 0.95, sp_max * 0.08,
            "(3) Proliferation\nhigh growth · low speed",
            color="#7a0000", ha="right", va="bottom", **ann_kw)

    ax.set_xlabel("Growth rate (h⁻¹)")
    ax.set_ylabel("Migration speed (µm/min)")
    ax.set_title("Phase portrait: growth vs migration speed\n"
                 "(colour = local glucose concentration)")
    _savefig(out_path)
    plt.close(fig)


# ===========================================================================
# Animation: cells (circles) over glucose gradient field
# ===========================================================================

def _render_frame(
    cells_a:      np.ndarray,
    name2idx:     dict[str, int],
    micro_a:      np.ndarray,
    time_h:       float,
    glc_vmin:     float,
    glc_vmax:     float,
    domain_x:     tuple[float, float],
    domain_y:     tuple[float, float],
    grid_nx:      int = 500,
    grid_ny:      int = 80,
    cell_radius:  float = 4.0,
) -> np.ndarray:
    """Render one animation frame and return an RGB uint8 numpy array."""
    vox_x = micro_a[0, :]
    vox_y = micro_a[1, :]
    glc   = micro_a[MICRO_IDX_GLUCOSE, :]

    xi     = np.linspace(domain_x[0], domain_x[1], grid_nx)
    yi     = np.linspace(domain_y[0], domain_y[1], grid_ny)
    Xi, Yi = np.meshgrid(xi, yi)
    Gi     = griddata((vox_x, vox_y), glc, (Xi, Yi), method="linear")

    # Figure proportions follow domain aspect ratio, but with a minimum height
    dx    = domain_x[1] - domain_x[0]
    dy    = domain_y[1] - domain_y[0]
    fig_w = 12.0
    fig_h = max(2.2, fig_w * dy / dx + 1.0)

    fig, ax = plt.subplots(figsize=(fig_w, fig_h), dpi=100,
                           constrained_layout=True)
    mpl.rcParams.update({"font.size": 8, "axes.linewidth": 0.5})

    im = ax.imshow(
        Gi,
        origin="lower",
        extent=[domain_x[0], domain_x[1], domain_y[0], domain_y[1]],
        vmin=glc_vmin, vmax=glc_vmax,
        cmap="YlOrBr_r",
        interpolation="bilinear",
        aspect="auto",
    )
    cb = fig.colorbar(im, ax=ax, fraction=0.015, pad=0.02)
    cb.set_label("Glucose (mM)", fontsize=7)
    cb.ax.tick_params(labelsize=6)

    # Draw cells
    if cells_a.shape[0] > 0:
        for row in cells_a:
            cx  = row[name2idx["x_position"]]
            cy  = row[name2idx["y_position"]]
            # Use migration_speed to colour the cell circle
            spd = row[name2idx["migration_speed"]]
            circ = Circle((cx, cy), radius=cell_radius,
                           facecolor="steelblue", edgecolor="white",
                           linewidth=0.4, alpha=0.9, zorder=3)
            ax.add_patch(circ)

    ax.set_xlim(domain_x)
    ax.set_ylim(domain_y[0] - 1, domain_y[1] + 1)
    ax.set_xlabel("x (µm)", fontsize=8)
    ax.set_ylabel("y (µm)", fontsize=8)
    ax.set_title(f"t = {time_h:.2f} h", fontsize=9)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    # Render to RGB array via BytesIO buffer
    buf = io.BytesIO()
    fig.savefig(buf, format="png", dpi=100)
    buf.seek(0)
    plt.close(fig)

    from PIL import Image
    img = Image.open(buf).convert("RGB")
    return np.array(img)


def build_animation(
    reader: multicellds.MultiCellDS,
    out_gif: str,
    domain_x: tuple[float, float],
    domain_y: tuple[float, float],
    sample_every: int = 2,
    fps: int = 6,
    cell_radius: float = 4.0,
    verbose: bool = True,
) -> None:
    """Build and save an animated GIF of cells over the glucose gradient."""
    try:
        import imageio
        from PIL import Image  # noqa: F401 – used in _render_frame
    except ImportError:
        print("  WARNING: imageio and/or Pillow not installed – skipping animation.\n"
              "  Install with: pip install imageio Pillow")
        return

    cols     = get_cell_columns(reader._output_folder)
    name2idx = {n: i for i, n in enumerate(cols)}

    # First pass: compute glucose colour scale from all frames
    if verbose:
        print("  Computing glucose colour scale ...")
    glc_vals = []
    for _t, m in reader.microenvironment_as_matrix_iterator():
        glc_vals.append(m[MICRO_IDX_GLUCOSE, :])
    glc_all  = np.concatenate(glc_vals)
    glc_vmin = float(np.percentile(glc_all, 1))
    glc_vmax = float(np.percentile(glc_all, 99))
    if verbose:
        print(f"  Glucose range: [{glc_vmin:.3f}, {glc_vmax:.3f}] mM")

    # Second pass: render frames
    frames = []
    cell_iter  = reader.cells_as_matrix_iterator()
    micro_iter = reader.microenvironment_as_matrix_iterator()

    for k, ((t_c, cells_a), (_t_m, micro_a)) in enumerate(zip(cell_iter, micro_iter)):
        if k % sample_every != 0:
            continue
        cells_a = _ensure_rows(cells_a, len(cols))
        time_h  = t_c / 60.0
        if verbose:
            print(f"  Frame {k:04d}  t={time_h:.2f} h  ({cells_a.shape[0]} cells)", end="\r")

        rgb = _render_frame(
            cells_a, name2idx, micro_a, time_h,
            glc_vmin, glc_vmax, domain_x, domain_y,
            cell_radius=cell_radius,
        )
        frames.append(rgb)

    if verbose:
        print(f"\n  Total frames rendered: {len(frames)}")

    os.makedirs(os.path.dirname(out_gif) if os.path.dirname(out_gif) else ".", exist_ok=True)
    imageio.mimsave(out_gif, frames, fps=fps, loop=0)
    if verbose:
        print(f"  Saved: {out_gif}")


# ===========================================================================
# Main
# ===========================================================================

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Dynamics analysis for metabolic-driven motility PhysiCelldFBA simulation.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument("--output-dir",         default=DEFAULT_OUTPUT_DIR)
    parser.add_argument("--results-dir",        default=DEFAULT_RESULTS_DIR)
    parser.add_argument("--sample-every",       type=int,   default=1,
                        help="Every N-th snapshot for per-cell analysis")
    parser.add_argument("--anim-sample-every",  type=int,   default=2,
                        help="Every N-th snapshot for the animation")
    parser.add_argument("--fps",                type=int,   default=6,
                        help="Frames per second in the output GIF")
    parser.add_argument("--cell-radius",        type=float, default=4.0,
                        help="Display radius of cells in animation (µm, inflated for visibility)")
    parser.add_argument("--no-animation",       action="store_true",
                        help="Skip the animated GIF")
    args = parser.parse_args()

    os.makedirs(args.results_dir, exist_ok=True)
    print(f"Output dir  : {args.output_dir}")
    print(f"Results dir : {args.results_dir}")

    # ---- reader + domain ---------------------------------------------------
    print("\n[1] Loading PhysiCell output ...")
    reader   = multicellds.MultiCellDS(output_folder=args.output_dir)
    domain_x, domain_y = get_domain_bounds(reader)
    print(f"  Domain x: {domain_x}  y: {domain_y}")

    # ---- aggregate timeseries ----------------------------------------------
    print("[2] Building aggregate time series ...")
    df_agg = load_cell_aggregate_timeseries(reader)

    print("[3] Plotting population dynamics ...")
    plot_population_dynamics(
        df_agg,
        out_path=os.path.join(args.results_dir, "fig01_population_dynamics.svg"),
    )

    # ---- per-cell long dataframe (with local glucose) ----------------------
    print("[4] Building per-cell dataframe (with local glucose) ...")
    df = build_per_cell_dataframe(reader, sample_every=args.sample_every, verbose=True)

    # ---- phase portrait (growth vs speed, coloured by local glucose) -------
    print("[5] Plotting phase portrait ...")
    plot_phase_portrait(
        df,
        out_path=os.path.join(args.results_dir, "fig02_phase_portrait.svg"),
    )

    # ---- animation ---------------------------------------------------------
    if not args.no_animation:
        print("[6] Building animation GIF ...")
        build_animation(
            reader,
            out_gif=os.path.join(args.results_dir, "animation_cells_glucose.gif"),
            domain_x=domain_x,
            domain_y=domain_y,
            sample_every=args.anim_sample_every,
            fps=args.fps,
            cell_radius=args.cell_radius,
            verbose=True,
        )
    else:
        print("[6] Animation skipped (--no-animation).")

    print(f"\nDone. All outputs in: {args.results_dir}")


if __name__ == "__main__":
    main()
