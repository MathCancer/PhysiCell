"""

Biomass combined panel (0–50 h).

"""

from __future__ import annotations

import os
import sys

import matplotlib.pyplot as plt
import pandas as pd

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(_SCRIPT_DIR)
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

import config
from lib import composite_figure_style as cfs
from lib.io import cell_type_name, open_reader


def _style_ax(ax):
    ax.set_facecolor('white')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_linewidth(cfs.LW_SPINE)
    ax.spines['bottom'].set_linewidth(cfs.LW_SPINE)
    ax.tick_params(
        axis='both', which='major', labelsize=cfs.FS_TICK,
        length=2, width=cfs.LW_SPINE, pad=1.5, colors='#222222',
    )
    ax.tick_params(axis='both', which='minor', length=0)
    ax.grid(False)


def _collect_biomass_by_species(output_dir: str) -> tuple[pd.DataFrame, pd.DataFrame]:
    reader = open_reader(output_dir)
    cols = reader.cell_columns
    if 'cell_type' not in cols or 'total_volume' not in cols:
        raise RuntimeError('Missing cell_type or total_volume in cell output')
    type_idx = cols.index('cell_type')
    vol_idx = cols.index('total_volume')

    rows = []
    for t, cells in reader.cells_as_matrix_iterator():
        if cells.shape[0] == 0:
            continue
        time_h = t / 60.0
        for raw_type in set(cells[:, type_idx]):
            mask = cells[:, type_idx] == raw_type
            name = cell_type_name(raw_type)
            if name not in config.SPECIES_LINE_COLORS:
                continue
            rows.append({
                'time': time_h,
                'cell_type': name,
                'biomass': float(cells[mask, vol_idx].sum()),
            })

    df = pd.DataFrame(rows)
    if df.empty:
        return pd.DataFrame(), pd.DataFrame()
    df_cb = df[df['cell_type'] == 'C. beijerinckii'].sort_values('time').reset_index(drop=True)
    df_mb = df[df['cell_type'] == 'M. barkeri'].sort_values('time').reset_index(drop=True)
    return df_cb, df_mb


def plot_biomass_combined(output_dir: str, save_dir: str, max_hours: int) -> list[str]:
    df_cb, df_mb = _collect_biomass_by_species(output_dir)
    if df_cb.empty and df_mb.empty:
        raise RuntimeError('No biomass data found')

    df_cb = df_cb[df_cb['time'] <= max_hours]
    df_mb = df_mb[df_mb['time'] <= max_hours]
    colors = config.SPECIES_LINE_COLORS
    stem = f'biomass_combined_0_{int(max_hours)}h'

    cfs.apply_cell_press_style()
    fig, ax = plt.subplots(figsize=(cfs.FIG_FLUX_ROW_W_IN, cfs.FIG_FLUX_ROW_BELOW_H_IN))
    if not df_cb.empty:
        ax.plot(
            df_cb['time'], df_cb['biomass'],
            color=colors['C. beijerinckii'], linewidth=cfs.LW_DATA,
            linestyle='-', label='C. beijerinckii',
        )
    if not df_mb.empty:
        ax.plot(
            df_mb['time'], df_mb['biomass'],
            color=colors['M. barkeri'], linewidth=cfs.LW_DATA,
            linestyle='-', label='M. barkeri',
        )
    ax.set_ylabel('Biomass volume (μm³)', fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')
    ax.set_xlabel('Time (h)', fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')
    ax.ticklabel_format(style='plain', axis='y', useOffset=False)
    ax.set_ylim(bottom=0)
    ax.set_xlim(0, max_hours)
    _style_ax(ax)
    cfs.layout_flux_row_panel_legend_below(fig)
    cfs.add_combined_panel_legend(ax, ncol=2, italic=True)

    out_svg = os.path.join(save_dir, f'{stem}.svg')
    out_png = os.path.join(save_dir, f'{stem}.png')
    fig.savefig(out_svg, format='svg', dpi=300, bbox_inches='tight', facecolor='white')
    fig.savefig(out_png, format='png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    return [out_svg, out_png]
