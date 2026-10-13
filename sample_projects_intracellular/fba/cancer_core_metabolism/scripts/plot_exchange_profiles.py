#!/usr/bin/env python3
"""
Distance-binned net export rates and substrate concentration grids.

Matches the main ``next_export_rates_grid_*.svg`` figure in
``cancer_tissue_figures_v2.ipynb``, split into two regenerable modes:

* ``flux`` — cell net export / uptake vs distance (fmol/min)
* ``concentration`` — microenvironment concentration vs distance (mM)

Examples
--------
    python scripts/plot_cancer_tissue_spatial.py --time-point 72 --net-export-rates
    python scripts/plot_cancer_tissue_spatial.py --time-point 72 --concentration-grid
    python scripts/figure_generation.py --time-point 72 --net-export-rates --concentration-grid
"""

from __future__ import annotations

import math
from typing import Mapping, Optional

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Exchange reaction → microenvironment / display name (notebook order)
DEFAULT_EXCHANGES: dict[str, str] = {
    "R_EX_o2_e": "O2",
    "R_EX_glc_e": "D-Glucose",
    "R_EX_gln_L_e": "L-Glutamine",
    "R_EX_gly_e": "Glycine",
    "R_EX_thr_L_e": "L-Threonine",
    "R_EX_ile_L_e": "L-Isoleucine",
    "R_EX_leu_L_e": "L-Leucine",
    "R_EX_lys_L_e": "L-Lysine",
    "R_EX_orn_e": "Ornithine",
    "R_EX_pi_e": "Phosphate",
    "R_EX_ser_L_e": "L-Serine",
    "R_EX_phe_L_e": "L-Phenylalanine",
    "R_EX_nh4_e": "Ammonium",
    "R_EX_co2_e": "CO2",
    "R_EX_lac_L_e": "L-Lactate",
}

_SOLID_FRACTION = 0.75
_CELL_DENSITY = 1.04
_X_MIN_CELL = 15.0
_X_MAX = 320.0
_ZONE_X = (120.0, 200.0)


def _smooth_series(s: pd.Series, window: int) -> pd.Series:
    if window is None or int(window) <= 1:
        return s
    w = int(window)
    if w % 2 == 0:
        w += 1
    return s.rolling(window=w, center=True, min_periods=1).mean()


def _nearest_z(z: np.ndarray, z_slice: Optional[float] = None) -> float:
    unique_z = np.unique(z)
    if z_slice is None:
        return float(unique_z[np.argmin(np.abs(unique_z))])
    if (z == z_slice).any():
        return float(z_slice)
    return float(unique_z[np.argmin(np.abs(unique_z - z_slice))])


def read_microenvironment_step(
    reader,
    time_step=None,
    z_slice=None,
    *,
    convert_to_hours: bool = True,
    time_interval: float = 60.0,
) -> pd.DataFrame:
    """Load one microenvironment midplane as a DataFrame (x, y, substrates)."""
    t_current, menv = None, None
    for t, a in reader.microenvironment_as_matrix_iterator():
        if convert_to_hours:
            t = float(t) / float(time_interval)
        t_current, menv = t, a
        if time_step is None or float(t) == float(time_step):
            break
    if menv is None:
        raise ValueError("No microenvironment data found in output folder")

    x = menv[0, :]
    y = menv[1, :]
    z = menv[2, :]
    z0 = _nearest_z(z, z_slice)
    mask = z == z0
    df_micro = pd.DataFrame({"x": x[mask], "y": y[mask]})
    for name, _, str_idx in reader.microenvironment_columns:
        df_micro[name] = menv[4 + int(str_idx), mask]
    df_micro.attrs["z_slice"] = z0
    df_micro.attrs["time"] = t_current
    return df_micro


def plot_exchange_profile_grid(
    *,
    mode: str,
    time_point: float,
    df_cells: Optional[pd.DataFrame] = None,
    df_micro: Optional[pd.DataFrame] = None,
    exchanges: Optional[Mapping[str, str]] = None,
    n_bins: int = 20,
    x_max: float = _X_MAX,
    x_min_cell: float = _X_MIN_CELL,
    zone_x: tuple[float, float] = _ZONE_X,
    smooth_window: int = 3,
    solid_fraction: float = _SOLID_FRACTION,
    cell_density: float = _CELL_DENSITY,
    n_cols: int = 4,
    figsize: Optional[tuple[float, float]] = None,
    dpi: int = 90,
) -> plt.Figure:
    """
    Multi-panel distance profiles.

    Parameters
    ----------
    mode :
        ``\"flux\"`` — net export rate (fmol/min) ± std from cells.
        ``\"concentration\"`` — mean microenvironment concentration (mM).
    """
    mode = mode.lower().strip()
    if mode not in {"flux", "concentration"}:
        raise ValueError("mode must be 'flux' or 'concentration'")

    exchanges = dict(exchanges or DEFAULT_EXCHANGES)
    bins = np.linspace(0.0, x_max, n_bins + 1)
    n_rows = math.ceil(len(exchanges) / n_cols)
    if figsize is None:
        figsize = (16.0, 1.6 * n_rows)

    fig, axes = plt.subplots(
        n_rows, n_cols, figsize=figsize, sharex=True, dpi=dpi
    )
    axes_f = np.atleast_1d(axes).ravel()

    df_filtered = None
    if mode == "flux":
        if df_cells is None:
            raise ValueError("df_cells is required for mode='flux'")
        df_filtered = df_cells[
            (df_cells["x_position"] >= x_min_cell) & (df_cells["time"] == time_point)
        ]

    for i, (ex_id, substrate) in enumerate(exchanges.items()):
        ax = axes_f[i]
        ax.axvline(zone_x[0], linestyle="--", color="darkgrey", linewidth=1.5)
        ax.axvline(zone_x[1], linestyle="--", color="darkgrey", linewidth=1.5)
        ax.set_title(substrate, fontsize=16)
        ax.set_xlim(0, x_max)
        ax.tick_params(axis="both", labelsize=12)

        if mode == "flux":
            if ex_id not in df_filtered.columns:
                ax.set_title(f"{substrate} (missing flux column)", fontsize=16)
                continue
            flux = ex_id[5:-2].replace("_", "-")
            df = df_filtered[["total_volume", "x_position", ex_id]].copy()
            df.loc[:, ex_id] *= (
                df.loc[:, "total_volume"] * cell_density * solid_fraction * (1 / 60.0)
            )
            df = df.rename({ex_id: flux}, axis=1)
            df["x_bin"] = pd.cut(df["x_position"], bins=bins)
            grouped = (
                df.groupby("x_bin", observed=True)[flux]
                .agg(["mean", "std", "count"])
                .reset_index()
            )
            grouped["x_center"] = grouped["x_bin"].apply(lambda b: b.mid)
            grouped = grouped.sort_values("x_center")
            x = grouped["x_center"]
            mean = _smooth_series(grouped["mean"], smooth_window)
            std = _smooth_series(grouped["std"].fillna(0), smooth_window)
            color = "firebrick" if np.nanmax(mean.to_numpy()) > 0 else "royalblue"
            ax.plot(x, mean, linewidth=2, color=color)
            ax.fill_between(x, mean - std, mean + std, alpha=0.2, color=color)
            ax.axhline(0, linestyle=":", linewidth=1, color="black")
        else:
            if df_micro is None or substrate not in df_micro.columns or df_micro.empty:
                ax.set_title(f"{substrate} (missing concentration)", fontsize=16)
                continue
            grid = df_micro.pivot_table(index="y", columns="x", values=substrate)
            profile = grid.mean(axis=0).sort_index()
            profile_binned = profile.groupby(
                pd.cut(profile.index, bins=bins), observed=True
            ).mean()
            profile_x = profile_binned.index.map(lambda b: b.mid)
            profile_y = _smooth_series(
                pd.Series(profile_binned.to_numpy(), index=list(profile_x)),
                smooth_window,
            )
            ax.plot(
                profile_y.index,
                profile_y.values,
                color="black",
                linewidth=2.0,
            )
            ax.fill_between(
                profile_y.index,
                0.0,
                profile_y.values,
                color="0.5",
                alpha=0.12,
            )
            ax.set_ylim(bottom=0.0)

    # Drop unused axes
    for j in range(len(exchanges), len(axes_f)):
        fig.delaxes(axes_f[j])

    fig.tight_layout(rect=[0.04, 0.05, 1, 1])
    fig.text(
        0.5,
        0.025,
        "Distance to blood vessel (µm)",
        ha="center",
        fontsize=16,
    )
    if mode == "flux":
        ylabel = r"Cell net export rate ($fmol/min$)"
    else:
        ylabel = "Concentration (mM)"
    fig.text(0.025, 0.5, ylabel, va="center", rotation="vertical", fontsize=16)
    return fig
