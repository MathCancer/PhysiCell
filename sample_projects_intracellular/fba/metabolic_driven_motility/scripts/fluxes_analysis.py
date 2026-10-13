#!/usr/bin/env python3
"""Plot exchange fluxes from PhysiCell dFBA simulations.

This script reads cell matrices via `pctk.multicellds.MultiCellDS` and uses the
`output*.xml` <labels> section as the authoritative source for the column indices
of custom_data fields.

Adapted for the PhysiCell dFBA case where you write:

    oxygen_flux   <- R_EX_o2_e
    glucose_flux  <- R_EX_glc__D_e
    acetate_flux  <- R_EX_ac_e
    co2_flux      <- R_EX_co2_e

You can point it at a single `output/` folder or at a `runs/` directory to batch
process multiple simulations.
"""

from __future__ import annotations

import argparse
import glob
import os
import xml.etree.ElementTree as ET
from typing import Dict, Iterable, List, Optional, Tuple

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pctk import multicellds


DEFAULT_RUNS_DIR = "runs"  # repo-root default; we also try ../runs as a fallback
DEFAULT_RESULTS_DIR = "results"

# Flux names MUST match exactly what you write into custom_data in C++
DEFAULT_FLUXES = ["oxygen_flux", "glucose_flux", "acetate_flux", "co2_flux"]

# A simple, consistent palette
DEFAULT_COLORS = {
    "oxygen_flux": "#4daf4a",   # green
    "glucose_flux": "#377eb8",  # blue
    "acetate_flux": "#ff7f00",  # orange
    "co2_flux": "#555555",      # dark gray
}

DEFAULT_BAND_MODE = "quantile"  # better than std when distributions are skewed/outlier-heavy
DEFAULT_QLOW = 0.25
DEFAULT_QHIGH = 0.75
DEFAULT_CENTER = "median"  # median is typically more stable than mean for flux distributions

def _get_xml_label_index(output_dir: str, column_name: str) -> Optional[int]:
    """Return the matrix column index for `column_name` from `output*.xml` labels."""
    try:
        output_xml_files = sorted(glob.glob(os.path.join(output_dir, "output*.xml")))
        if not output_xml_files:
            return None
        xml_tree = ET.parse(output_xml_files[0])
        labels = xml_tree.find(".//labels")
        if labels is None:
            return None
        for label in labels:
            if label.text == column_name:
                xml_index = int(label.attrib.get("index", -1))
                return xml_index if xml_index >= 0 else None
    except Exception as exc:
        print(f"Warning: Could not read XML labels in '{output_dir}': {exc}")
    return None

def get_column_index(reader: multicellds.MultiCellDS, output_dir: str, column_name: str) -> Optional[int]:
    """Resolve a column index for `column_name` via XML labels and sanity-check it."""
    xml_idx = _get_xml_label_index(output_dir, column_name)
    if xml_idx is None:
        return None

    # Verify against first matrix frame
    for _t, a in reader.cells_as_matrix_iterator():
        return xml_idx if xml_idx < a.shape[1] else None
    return None

def _iter_simulations(runs_dir: str) -> List[Tuple[str, str]]:
    """Return list of (sim_name, output_dir) under a runs directory."""
    sims: List[Tuple[str, str]] = []
    if not os.path.isdir(runs_dir):
        return sims
    for entry in sorted(os.scandir(runs_dir), key=lambda e: e.name):
        if not entry.is_dir():
            continue
        out_dir = os.path.join(entry.path, "output")
        if os.path.isdir(out_dir):
            sims.append((entry.name, out_dir))
    return sims


def collect_flux_timeseries(
    output_dir: str,
    flux_names: Iterable[str],
    cell_type_id: Optional[int] = None,
    qlow: float = DEFAULT_QLOW,
    qhigh: float = DEFAULT_QHIGH,
) -> pd.DataFrame:
    """Collect mean/std/min/max for each flux over time.

    If `cell_type_id` is provided, filter to cells with that `cell_type`.
    """
    reader = multicellds.MultiCellDS(output_folder=output_dir)
    name2idx = {n: i for i, n in enumerate(reader.cell_columns)}

    cell_type_idx = name2idx.get("cell_type")
    if cell_type_id is not None and cell_type_idx is None:
        raise RuntimeError("Requested --cell-type-id, but 'cell_type' column not found in cell_columns")

    flux_indices: Dict[str, int] = {}
    missing: List[str] = []
    for flux in flux_names:
        idx = get_column_index(reader, output_dir, flux)
        if idx is None:
            missing.append(flux)
        else:
            flux_indices[flux] = idx

    if missing:
        print(f"  Note: missing flux columns: {', '.join(missing)}")
    if not flux_indices:
        raise RuntimeError(
            "No requested flux columns were found in output XML labels. "
            "This usually means you need to recompile and rerun so the fields are written."
        )

    if not (0.0 <= qlow < qhigh <= 1.0):
        raise ValueError(f"Invalid quantiles: qlow={qlow}, qhigh={qhigh} (need 0 <= qlow < qhigh <= 1)")

    rows: List[Dict[str, float]] = []
    for t, a in reader.cells_as_matrix_iterator():
        if a.size == 0 or a.shape[0] == 0:
            continue

        if cell_type_id is not None:
            mask = (a[:, cell_type_idx] == cell_type_id)
            if not np.any(mask):
                continue
            cells = a[mask, :]
        else:
            cells = a

        row: Dict[str, float] = {"time": t / 60.0, "n_cells": int(cells.shape[0])}
        for flux, idx in flux_indices.items():
            if idx >= cells.shape[1]:
                continue
            vals = cells[:, idx]
            row[f"{flux}_mean"] = float(np.mean(vals))
            row[f"{flux}_std"] = float(np.std(vals))
            row[f"{flux}_sem"] = float(np.std(vals) / np.sqrt(vals.size)) if vals.size > 0 else float("nan")
            row[f"{flux}_median"] = float(np.median(vals))
            row[f"{flux}_q{int(qlow*100):02d}"] = float(np.quantile(vals, qlow))
            row[f"{flux}_q{int(qhigh*100):02d}"] = float(np.quantile(vals, qhigh))
            row[f"{flux}_min"] = float(np.min(vals))
            row[f"{flux}_max"] = float(np.max(vals))
        rows.append(row)

    if not rows:
        raise RuntimeError("No cell data found (empty output or filter removed all cells).")

    df = pd.DataFrame(rows).sort_values("time").reset_index(drop=True)
    return df


def plot_fluxes(
    df: pd.DataFrame,
    flux_names: Iterable[str],
    out_prefix: str,
    title: Optional[str] = None,
    colors: Optional[Dict[str, str]] = None,
    band: str = DEFAULT_BAND_MODE,
    qlow: float = DEFAULT_QLOW,
    qhigh: float = DEFAULT_QHIGH,
    center: str = DEFAULT_CENTER,
    show_secondary_center: bool = True,
) -> None:
    flux_names = [f for f in flux_names if f"{f}_mean" in df.columns]
    if not flux_names:
        raise RuntimeError("Nothing to plot: no flux mean columns in dataframe.")

    if colors is None:
        colors = DEFAULT_COLORS

    band = band.lower()
    if band not in {"quantile", "std", "sem", "none"}:
        raise ValueError("--band must be one of: quantile, std, sem, none")

    center = center.lower()
    if center not in {"mean", "median"}:
        raise ValueError("--center must be one of: mean, median")

    qlow_col = f"q{int(qlow*100):02d}"
    qhigh_col = f"q{int(qhigh*100):02d}"

    n = len(flux_names)
    fig, axes = plt.subplots(n, 1, figsize=(9.0, 2.2 * n), sharex=True, dpi=300)
    axes = np.atleast_1d(axes)

    for i, flux in enumerate(flux_names):
        ax = axes[i]
        mean_col = f"{flux}_mean"
        std_col = f"{flux}_std"
        min_col = f"{flux}_min"
        max_col = f"{flux}_max"
        med_col = f"{flux}_median"

        c = colors.get(flux, None)
        if c is None:
            c = plt.cm.tab10(i % 10)

        primary_col = med_col if center == "median" else mean_col
        primary_label = "median" if center == "median" else "mean"
        ax.plot(df["time"], df[primary_col], color=c, linewidth=2.0, label=primary_label)

        if show_secondary_center:
            secondary_col = mean_col if center == "median" else med_col
            secondary_label = "mean" if center == "median" else "median"
            if secondary_col in df.columns:
                ax.plot(
                    df["time"],
                    df[secondary_col],
                    color=c,
                    linewidth=1.2,
                    linestyle=":",
                    alpha=0.9,
                    label=secondary_label,
                )

        if band == "quantile":
            lo = f"{flux}_{qlow_col}"
            hi = f"{flux}_{qhigh_col}"
            if lo in df.columns and hi in df.columns:
                ax.fill_between(
                    df["time"],
                    df[lo],
                    df[hi],
                    color=c,
                    alpha=0.22,
                    label=f"{int(qlow*100)}–{int(qhigh*100)}%",
                )

        elif band == "std":
            if std_col in df.columns:
                ax.fill_between(
                    df["time"],
                    df[mean_col] - df[std_col],
                    df[mean_col] + df[std_col],
                    color=c,
                    alpha=0.25,
                    label="± std",
                )

        elif band == "sem":
            sem_col = f"{flux}_sem"
            if sem_col in df.columns:
                ax.fill_between(
                    df["time"],
                    df[mean_col] - df[sem_col],
                    df[mean_col] + df[sem_col],
                    color=c,
                    alpha=0.25,
                    label="± sem",
                )

        ax.axhline(y=0.0, color="k", linestyle="--", linewidth=0.6, alpha=0.5)
        ax.set_ylabel(f"{flux}\n(mmol/gDW/h)")
        ax.grid(True, alpha=0.25)
        ax.legend(loc="best", fontsize=9)

        if min_col in df.columns and max_col in df.columns:
            y_min = float(np.nanmin(df[min_col]))
            y_max = float(np.nanmax(df[max_col]))
            y_rng = y_max - y_min
            if np.isfinite(y_rng) and y_rng > 0:
                ax.set_ylim(y_min - 0.1 * y_rng, y_max + 0.1 * y_rng)

    axes[-1].set_xlabel("Time (hours)")
    if title:
        fig.suptitle(title)
        fig.tight_layout(rect=[0, 0, 1, 0.98])
    else:
        fig.tight_layout()

    os.makedirs(os.path.dirname(out_prefix), exist_ok=True)
    fig.savefig(out_prefix + ".png", dpi=300, bbox_inches="tight", facecolor="white")
    fig.savefig(out_prefix + ".svg", format="svg", dpi=300, bbox_inches="tight", facecolor="white")
    plt.close(fig)

def main():
    parser = argparse.ArgumentParser(
        description="Plot oxygen/glucose/acetate/co2 exchange fluxes from PhysiCell dFBA outputs."
    )
    src = parser.add_mutually_exclusive_group(required=False)
    src.add_argument(
        "--output-dir",
        type=str,
        default=None,
        help="Path to a single PhysiCell output directory (contains output*.xml).",
    )
    src.add_argument(
        "--runs-dir",
        type=str,
        default=DEFAULT_RUNS_DIR,
        help=f"Path to runs directory containing simulation subfolders (default: {DEFAULT_RUNS_DIR}).",
    )
    parser.add_argument(
        "--results-dir",
        type=str,
        default=DEFAULT_RESULTS_DIR,
        help=f"Where to save plots (default: {DEFAULT_RESULTS_DIR}).",
    )
    parser.add_argument(
        "--cell-type-id",
        type=int,
        default=None,
        help="Optional: only include cells with this cell_type ID.",
    )
    parser.add_argument(
        "--flux",
        action="append",
        dest="fluxes",
        default=None,
        help="Flux custom_data field to plot. Repeatable. Default plots oxygen_flux, glucose_flux, acetate_flux, co2_flux.",
    )
    parser.add_argument(
        "--band",
        choices=["quantile", "std", "sem", "none"],
        default=DEFAULT_BAND_MODE,
        help="Shaded uncertainty band type. 'quantile' is robust for skew/outliers (default).",
    )
    parser.add_argument(
        "--quantiles",
        nargs=2,
        type=float,
        default=[DEFAULT_QLOW, DEFAULT_QHIGH],
        metavar=("QLOW", "QHIGH"),
        help=f"Quantile band bounds when --band=quantile (default: {DEFAULT_QLOW} {DEFAULT_QHIGH}).",
    )
    parser.add_argument(
        "--center",
        choices=["mean", "median"],
        default=DEFAULT_CENTER,
        help="Center line to plot. 'median' is often better when flux distributions are skewed/outlier-heavy.",
    )
    parser.add_argument(
        "--no-secondary-center",
        action="store_true",
        help="If set, do not draw the secondary center line (mean/median) as a dotted line.",
    )

    args = parser.parse_args()
    fluxes = args.fluxes if args.fluxes else list(DEFAULT_FLUXES)
    qlow, qhigh = float(args.quantiles[0]), float(args.quantiles[1])

    fluxes_dir = os.path.join(args.results_dir, "fluxes")
    os.makedirs(fluxes_dir, exist_ok=True)

    if args.output_dir:
        sims = [(os.path.basename(os.path.normpath(args.output_dir)), args.output_dir)]
    else:
        runs_dir = args.runs_dir
        if not os.path.isdir(runs_dir):
            script_dir = os.path.dirname(os.path.abspath(__file__))
            alt = os.path.normpath(os.path.join(script_dir, "..", runs_dir))
            if os.path.isdir(alt):
                runs_dir = alt
        sims = _iter_simulations(runs_dir)

    if not sims:
        if args.output_dir:
            raise SystemExit(f"No output found at: {args.output_dir}")
        raise SystemExit(f"No simulations found under runs dir: {args.runs_dir}")

    print("=" * 80)
    print("PHYSICELL dFBA FLUX PLOTTER")
    print("=" * 80)
    print(f"Simulations to process: {len(sims)}")
    print(f"Fluxes: {', '.join(fluxes)}")
    print(f"Band: {args.band}" + (f" ({int(qlow*100)}–{int(qhigh*100)}%)" if args.band == "quantile" else ""))
    if args.cell_type_id is not None:
        print(f"Filtering by cell_type == {args.cell_type_id}")
    print(f"Saving to: {os.path.abspath(fluxes_dir)}")
    print("=" * 80)

    for sim_name, out_dir in sims:
        print(f"\nProcessing: {sim_name}")
        print(f"  output_dir: {out_dir}")
        try:
            df = collect_flux_timeseries(out_dir, fluxes, cell_type_id=args.cell_type_id, qlow=qlow, qhigh=qhigh)
            out_prefix = os.path.join(fluxes_dir, sim_name, f"{sim_name}_fluxes")
            plot_fluxes(
                df,
                fluxes,
                out_prefix,
                title=f"Exchange fluxes: {sim_name}",
                band=args.band,
                qlow=qlow,
                qhigh=qhigh,
                center=args.center,
                show_secondary_center=(not args.no_secondary_center),
            )
            df.to_csv(os.path.join(fluxes_dir, sim_name, f"{sim_name}_fluxes.csv"), index=False)
            print(f"  Saved plots + CSV under: {os.path.join(fluxes_dir, sim_name)}")
        except Exception as exc:
            print(f"  ERROR: {exc}")

    print("\nDone.")

if __name__ == "__main__":
    main()
