"""
Reduced substrate time series (glucose, CH₄, H₂, acetate).

"""

from __future__ import annotations

import os
import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(_SCRIPT_DIR)
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

import config
from lib import composite_figure_style as cfs
from lib.io import open_reader


def _interior_mask(microenv, n_layers=config.INTERIOR_BOUNDARY_LAYERS):
    x, y = microenv[0, :], microenv[1, :]
    x_unique = np.sort(np.unique(x))
    y_unique = np.sort(np.unique(y))
    n_layers = int(n_layers)
    if len(x_unique) <= 2 * n_layers or len(y_unique) <= 2 * n_layers:
        return np.ones(x.shape[0], dtype=bool)
    x_keep = set(x_unique[n_layers:-n_layers])
    y_keep = set(y_unique[n_layers:-n_layers])
    return np.array([(xv in x_keep) and (yv in y_keep) for xv, yv in zip(x, y)])


def _totals_row(t, name, conc_values, vol_um3, mask):
    vol_sel = np.asarray(vol_um3, dtype=float)[mask]
    conc_sel = np.asarray(conc_values, dtype=float)[mask]
    domain_vol_um3 = float(np.sum(vol_sel))
    domain_vol_L = domain_vol_um3 / config.UM3_PER_L
    total_mmol = float(np.sum(conc_sel * vol_sel) / config.UM3_PER_L)
    return {
        'time': t,
        'substrate': name,
        'total_per_domain_volume_mM': (
            total_mmol / domain_vol_L if domain_vol_L > 0 else 0.0
        ),
    }


def collect_substrate_totals(output_dir: str) -> pd.DataFrame:
    reader = open_reader(output_dir)
    substrate_names = [name for name, _u, _i in reader.microenvironment_columns]
    rows = []
    for t, microenv in reader.microenvironment_as_matrix_iterator():
        if microenv is None:
            continue
        vol_um3 = microenv[3, :]
        mask = np.ones(vol_um3.shape[0], dtype=bool)
        for i, name in enumerate(substrate_names):
            conc = microenv[i + 4, :]
            rows.append(_totals_row(t, name, conc, vol_um3, mask))
    return pd.DataFrame(rows)


def _substrates_for_plot(df_tot: pd.DataFrame) -> list[str]:
    targets = {
        'glucose': ['glucose', 'Glucose'],
        'methane': ['methane', 'CH4', 'ch4'],
        'H2': ['H2', 'h2', 'hydrogen'],
        'acetate': ['acetate', 'Acetate'],
    }
    available = set(df_tot['substrate'].unique())
    out = []
    for _key, names in targets.items():
        for n in names:
            if n in available:
                out.append(n)
                break
    return out


def plot_substrate_time_series_reduced(
    output_dir: str, save_dir: str, max_hours: int,
) -> list[str]:
    df_tot = collect_substrate_totals(output_dir)
    if df_tot.empty:
        raise RuntimeError('No microenvironment time series data found')

    df_zoom = df_tot[df_tot['time'] / 60.0 <= max_hours]
    substrates = _substrates_for_plot(df_zoom)
    if not substrates:
        raise RuntimeError('No target substrates (glucose, methane, H2, acetate) in output')

    cfs.apply_cell_press_style()
    fig, ax = plt.subplots(figsize=(cfs.FIG_FLUX_ROW_W_IN, cfs.FIG_FLUX_ROW_BELOW_H_IN))

    for substrate in substrates:
        sub = df_zoom[df_zoom['substrate'] == substrate]
        if sub.empty:
            continue
        color = config.SUBSTRATE_TS_COLORS.get(substrate, '#333333')
        label = config.SUBSTRATE_DISPLAY_NAMES.get(substrate, substrate)
        ax.plot(
            sub['time'] / 60.0,
            sub['total_per_domain_volume_mM'],
            color=color, linewidth=cfs.LW_DATA, label=label,
        )

    ax.set_ylabel('Concentration (mM)', fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')
    ax.set_xlabel('Time (h)', fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')
    ax.set_facecolor('white')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_linewidth(cfs.LW_SPINE)
    ax.spines['bottom'].set_linewidth(cfs.LW_SPINE)
    ax.tick_params(
        axis='both', which='major', labelsize=cfs.FS_TICK,
        length=2, width=cfs.LW_SPINE, pad=1.5, colors='#222222',
    )
    ax.set_ylim(bottom=0)
    ax.set_xlim(0, max_hours)
    ax.grid(False)
    cfs.layout_flux_row_panel_legend_below(fig)
    cfs.add_combined_panel_legend(ax, ncol=len(substrates))

    stem = 'substrate_time_series_reduced'
    out_svg = os.path.join(save_dir, f'{stem}.svg')
    out_png = os.path.join(save_dir, f'{stem}.png')
    fig.savefig(out_svg, format='svg', dpi=300, bbox_inches='tight', facecolor='white')
    fig.savefig(out_png, format='png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    return [out_svg, out_png]
