"""
Figures for the FVA degeneracy diagnostic.

Reads the two CSVs written by fva_degeneracy.py and renders three figures:

  1. per-exchange degeneracy, ranked          (magnitude across categories)
  2. degeneracy against the spatial axis      (magnitude over space)
  3. the widest exchanges over time           (change over time)

Colour follows the job it does: a single hue where it encodes magnitude, and a
fixed-order categorical palette validated for colour-vision deficiency where it
encodes identity (worst adjacent pair dE 11.4 protan, 18.4 normal vision), with
both a legend and direct labels so identity is never carried by colour alone.
Entries at the solver noise floor and spatial bins containing no cells are drawn
as absent, so neither can be misread as a measurement.

Usage:
    python3 fva_figures.py --experiment <dir> [--spatial-axis x] [--title "..."]
"""

from __future__ import annotations

import argparse
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.colors import LinearSegmentedColormap
from matplotlib.patches import Patch

INK, MUTED, GRID = "#222222", "#6b6b6b", "#dddddd"
BAR = "#0072B2"
# fixed order, never cycled; validated with the dataviz palette checker
PALETTE = ["#0072B2", "#D55E00", "#56B4E9", "#009E73", "#E69F00", "#CC79A7"]
FLOOR = 1e-9          # below this a range is solver noise, not degeneracy


def short(rid: str) -> str:
    return rid.replace("R_EX_", "").replace("_e", "")


def fig_ranking(summary: pd.DataFrame, title: str, path: str) -> None:
    s = summary[summary.median_range.notna()].copy()
    s["plot_med"] = s.median_range.clip(lower=FLOOR)
    s["plot_p95"] = s.p95_range.clip(lower=FLOOR)
    s = s.sort_values("plot_med")

    fig, ax = plt.subplots(figsize=(5.4, 0.26 * len(s) + 1.3), dpi=200)
    y = np.arange(len(s))
    at_floor = s.median_range.to_numpy() <= FLOOR
    ax.barh(y, s.plot_med, height=0.55, linewidth=0, zorder=3,
            color=np.where(at_floor, "#d9d9d9", BAR))
    ax.scatter(s.plot_p95, y, s=16, facecolors="none", edgecolors=BAR,
               linewidths=1.0, zorder=4)

    ax.set_yticks(y)
    ax.set_yticklabels([short(r) for r in s.reaction], fontsize=7)
    ax.set_xscale("log")
    ax.set_xlabel("FVA interval width at fixed growth rate  "
                  "(mmol gDW$^{-1}$ h$^{-1}$)", fontsize=8)
    ax.axvline(FLOOR, color=MUTED, lw=0.7, ls=":", zorder=2)
    ax.xaxis.grid(True, color=GRID, lw=0.5, zorder=0)
    ax.set_axisbelow(True)
    for sp in ("top", "right", "left"):
        ax.spines[sp].set_visible(False)
    ax.tick_params(colors=INK, labelsize=7, length=2)
    ax.legend(handles=[Patch(facecolor=BAR, label="median width"),
                       Patch(facecolor="#d9d9d9", label="at noise floor"),
                       plt.Line2D([], [], marker="o", ls="none", mfc="none",
                                  mec=BAR, ms=4, label="95th percentile")],
              frameon=False, fontsize=6.5, loc="lower right")
    ax.set_title(title, fontsize=9, color=INK, loc="left")
    fig.tight_layout()
    fig.savefig(path, bbox_inches="tight")
    plt.close(fig)
    print(f"  wrote {path}")


def fig_spatial(df: pd.DataFrame, order, axis: str, axis_label: str,
                title: str, path: str, nbins: int = 12) -> None:
    ok = df[df.replay_status == "optimal"].copy()
    if ok.empty:
        print("  (no optimal rows; skipping the spatial figure)")
        return
    edges = np.linspace(df[axis].min(), df[axis].max(), nbins + 1)
    ok["bin"] = pd.cut(ok[axis], bins=edges, include_lowest=True)
    M = np.vstack([ok.groupby("bin", observed=False)[f"{r}_range"].median()
                   .reindex(ok["bin"].cat.categories).to_numpy() for r in order])

    seq = LinearSegmentedColormap.from_list("seq", ["#f2f7fb", "#0072B2", "#00334f"])
    seq.set_bad("#eeeeee")          # empty bin: no cells there, not zero width
    M = np.ma.masked_invalid(M)

    fig, ax = plt.subplots(figsize=(6.2, 0.24 * len(order) + 1.5), dpi=200)
    im = ax.imshow(np.log10(np.clip(M, FLOOR, None)), aspect="auto", cmap=seq,
                   origin="upper", extent=[edges[0], edges[-1], len(order) - 0.5, -0.5])
    ax.set_yticks(range(len(order)))
    ax.set_yticklabels([short(r) for r in order], fontsize=7)
    ax.set_xlabel(axis_label, fontsize=8)
    cb = fig.colorbar(im, ax=ax, pad=0.02, fraction=0.035)
    cb.set_label("log$_{10}$ median FVA width", fontsize=7)
    cb.ax.tick_params(labelsize=6)
    ax.tick_params(colors=INK, labelsize=7, length=2)
    ax.set_title(title, fontsize=9, color=INK, loc="left")
    ax.text(1.005, -0.10, "grey = no cells in that bin", transform=ax.transAxes,
            fontsize=6, color=MUTED, ha="right")
    fig.tight_layout()
    fig.savefig(path, bbox_inches="tight")
    plt.close(fig)
    print(f"  wrote {path}")


def fig_time(df: pd.DataFrame, order, title: str, path: str, topn: int = 6) -> None:
    ok = df[df.replay_status == "optimal"]
    if ok.empty:
        print("  (no optimal rows; skipping the time figure)")
        return
    top = list(order)[:topn]
    fig, ax = plt.subplots(figsize=(5.8, 3.2), dpi=200)
    placed = []
    for colour, rid in zip(PALETTE, top):
        ts = ok.groupby("time")[f"{rid}_range"].median()
        ax.plot(ts.index / 60.0, ts.to_numpy(), color=colour, lw=2.0,
                label=short(rid), solid_capstyle="round")
        yv = ts.to_numpy()
        if len(yv) and np.isfinite(yv[-1]) and yv[-1] > 0:
            yend = yv[-1]
            while any(abs(np.log10(yend) - np.log10(p)) < 0.045 for p in placed):
                yend *= 1.11
            placed.append(yend)
            ax.annotate(f" {short(rid)}", (ts.index[-1] / 60.0, yend), fontsize=6.5,
                        color=colour, va="center", annotation_clip=False)

    ax.set_xlabel("time (h)", fontsize=8)
    ax.set_ylabel("median FVA width  (mmol gDW$^{-1}$ h$^{-1}$)", fontsize=8)
    ax.set_yscale("log")
    ax.yaxis.grid(True, color=GRID, lw=0.5)
    ax.set_axisbelow(True)
    for sp in ("top", "right"):
        ax.spines[sp].set_visible(False)
    ax.tick_params(colors=INK, labelsize=7, length=2)
    ax.legend(frameon=False, fontsize=7, ncol=3, loc="upper center",
              bbox_to_anchor=(0.5, -0.22))
    ax.set_title(title, fontsize=9, color=INK, loc="left")
    fig.subplots_adjust(right=0.84, bottom=0.30)
    fig.savefig(path, bbox_inches="tight")
    plt.close(fig)
    print(f"  wrote {path}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--experiment", required=True,
                    help="experiment directory holding fva_cells.csv and fva_summary.csv")
    ap.add_argument("--spatial-axis", default="x", choices=["x", "y", "z", "r"],
                    help="r = radial distance from the colony centre, for the "
                         "radially symmetric cases")
    ap.add_argument("--spatial-label", default=None)
    ap.add_argument("--label", default="", help="short name used in figure titles")
    args = ap.parse_args()

    e = args.experiment
    df = pd.read_csv(os.path.join(e, "fva_cells.csv"), low_memory=False)
    if args.spatial_axis == "r":
        df["r"] = np.hypot(df["x"] - df["x"].mean(), df["y"] - df["y"].mean())
    summary = pd.read_csv(os.path.join(e, "fva_summary.csv"))
    figdir = os.path.join(e, "figures")
    os.makedirs(figdir, exist_ok=True)

    order = [r for r in summary.sort_values("median_range", ascending=False).reaction
             if f"{r}_range" in df.columns]
    name = args.label or os.path.basename(e.rstrip("/"))
    axis_label = args.spatial_label or f"{args.spatial_axis} ($\\mu$m)"

    print(f"{name}: {len(df):,} cell-timepoints, {len(order)} exchanges")
    fig_ranking(summary, f"Degeneracy of each exchange flux — {name}",
                os.path.join(figdir, "01_ranking.png"))
    fig_spatial(df, order, args.spatial_axis, axis_label,
                f"Where the flux distribution is underdetermined — {name}",
                os.path.join(figdir, "02_spatial.png"))
    fig_time(df, order, f"Degeneracy over time — {name}",
             os.path.join(figdir, "03_time.png"))


if __name__ == "__main__":
    main()
