"""Combined net-export flux panel (0–50 h).

"""

from __future__ import annotations

import os
import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.lines import Line2D

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(_SCRIPT_DIR)
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

import config
from lib import composite_figure_style as cfs
from lib.io import get_xml_column_index, open_reader


def _cell_dry_mass_g(total_volume_um3):
    vol = np.asarray(total_volume_um3, dtype=float)
    return (
        vol * config.UM3_TO_ML
        * config.NET_EXPORT_CELL_DENSITY_G_PER_ML
        * config.NET_EXPORT_SOLID_FRACTION
    )


def _per_cell_net_export_fmol_per_s(flux_mmol_gdw_h, total_volume_um3):
    flux = np.asarray(flux_mmol_gdw_h, dtype=float)
    return flux * _cell_dry_mass_g(total_volume_um3) / 3600.0 * config.MMOL_TO_FMOL


def _flux_aggregate(flux_values, volumes):
    n = flux_values.size
    per_cell = _per_cell_net_export_fmol_per_s(flux_values, volumes)
    return float(np.sum(per_cell)), 0.0


def _resolve_flux_column_indices(output_dir, reader, flux_names):
    """Map flux column names to matrix indices (once per run)."""
    indices = {}
    sample_cols = None
    for _t, a in reader.cells_as_matrix_iterator():
        if a is not None and a.shape[0] > 0:
            sample_cols = a.shape[1]
            break
    for fn in flux_names:
        xml_idx = get_xml_column_index(output_dir, fn)
        if xml_idx is not None and (sample_cols is None or xml_idx < sample_cols):
            indices[fn] = xml_idx
    return indices


def _collect_flux_data(output_dir: str) -> tuple[pd.DataFrame, pd.DataFrame]:
    reader = open_reader(output_dir)
    cols = reader.cell_columns
    name2idx = {n: i for i, n in enumerate(cols)}
    cell_type_idx = name2idx.get('cell_type')
    dead_idx = name2idx.get('dead')
    total_volume_idx = name2idx.get('total_volume')
    if cell_type_idx is None:
        raise RuntimeError("'cell_type' column not found in cell output")

    flux_idx_cb = _resolve_flux_column_indices(output_dir, reader, config.KEY_FLUXES_CB)
    flux_idx_mb = _resolve_flux_column_indices(output_dir, reader, config.KEY_FLUXES_MB)

    data_cb, data_mb = [], []
    for t, a in reader.cells_as_matrix_iterator():
        if a.shape[0] == 0:
            continue
        if dead_idx is not None and dead_idx < a.shape[1]:
            a = a[a[:, dead_idx] < 0.5, :]
        if a.shape[0] == 0:
            continue
        time_h = t / 60.0

        cb_mask = a[:, cell_type_idx] == config.CELL_TYPE_CB
        if np.any(cb_mask):
            cb_cells = a[cb_mask]
            vol = (
                cb_cells[:, total_volume_idx].astype(float)
                if total_volume_idx is not None else np.ones(cb_mask.sum())
            )
            row = {'time': time_h}
            for fn in config.KEY_FLUXES_CB:
                idx = flux_idx_cb.get(fn)
                if idx is not None and idx < a.shape[1]:
                    vals = cb_cells[:, idx]
                    center, spread = _flux_aggregate(vals, vol)
                    row[f'{fn}_mean'] = center
                    row[f'{fn}_std'] = spread
                else:
                    row[f'{fn}_mean'] = 0.0
                    row[f'{fn}_std'] = 0.0
            data_cb.append(row)

        mb_mask = a[:, cell_type_idx] == config.CELL_TYPE_MB
        if np.any(mb_mask):
            mb_cells = a[mb_mask]
            vol = (
                mb_cells[:, total_volume_idx].astype(float)
                if total_volume_idx is not None else np.ones(mb_mask.sum())
            )
            row = {'time': time_h}
            for fn in config.KEY_FLUXES_MB:
                idx = flux_idx_mb.get(fn)
                if idx is not None and idx < a.shape[1]:
                    vals = mb_cells[:, idx]
                    center, spread = _flux_aggregate(vals, vol)
                    row[f'{fn}_mean'] = center
                    row[f'{fn}_std'] = spread
                else:
                    row[f'{fn}_mean'] = 0.0
                    row[f'{fn}_std'] = 0.0
            data_mb.append(row)

    df_cb = pd.DataFrame(data_cb)
    df_mb = pd.DataFrame(data_mb)
    if not df_cb.empty:
        df_cb = df_cb[df_cb['time'] > 0].drop_duplicates('time').sort_values('time')
    if not df_mb.empty:
        df_mb = df_mb[df_mb['time'] > 0].drop_duplicates('time').sort_values('time')
    return df_cb, df_mb


def _symmetric_ylim(df, flux_names, t_min=None, t_max=None, pad=1.08):
    if df is None or df.empty:
        return 1.0
    sub = df
    if t_min is not None:
        sub = sub[sub['time'] >= t_min]
    if t_max is not None:
        sub = sub[sub['time'] <= t_max]
    if sub.empty:
        return 1.0
    mags = []
    for fn in flux_names:
        mc = f'{fn}_mean'
        if mc not in sub.columns:
            continue
        v = sub[mc].to_numpy(dtype=float)
        if v.size:
            mags.append(float(np.nanmax(np.abs(v))))
    if not mags:
        return 1.0
    ymax = float(np.nanmax(mags))
    return ymax * pad if ymax > 0 else 1.0


def _draw_panel(ax, df, flux_names, colors):
    if df is None or df.empty:
        return
    for idx, fn in enumerate(flux_names):
        if idx >= len(colors):
            break
        mc = f'{fn}_mean'
        if mc not in df.columns:
            continue
        ax.plot(df['time'], df[mc], color=colors[idx], linewidth=cfs.LW_DATA, zorder=3)


def _add_species_label(ax, name):
    ax.text(
        -0.10, 0.5, name, transform=ax.transAxes,
        fontsize=cfs.FS_ROW_LABEL, color='#222222',
        va='center', ha='center', rotation=90,
    )


def _style_ax(ax, ylabel):
    ax.set_facecolor('white')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    for side in ('left', 'bottom'):
        ax.spines[side].set_linewidth(cfs.LW_SPINE)
    ax.tick_params(
        axis='both', which='major', labelsize=cfs.FS_TICK,
        length=2, width=cfs.LW_SPINE, pad=1.5, colors='#222222',
    )
    ax.axhline(0, color='#aaaaaa', linewidth=0.5, linestyle='--', zorder=0)
    ax.set_ylabel(ylabel, fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')


def plot_fluxes_combined(output_dir: str, save_dir: str, max_hours: int) -> list[str]:
    df_cb, df_mb = _collect_flux_data(output_dir)
    if df_cb.empty and df_mb.empty:
        raise RuntimeError('No flux data found')

    stem = 'fluxes_combined_net_export_fmol_s'
    ylabel = 'Net export rate\n(fmol/s)'
    cfs.apply_cell_press_style()

    fig, axes = plt.subplots(2, 1, figsize=(cfs.FIG_W_IN, cfs.FIG_H_IN), sharex=True)
    ax1, ax2 = axes

    _draw_panel(ax1, df_cb, config.KEY_FLUXES_CB, config.FLUX_COLORS_CB)
    _style_ax(ax1, ylabel)
    _add_species_label(ax1, 'C. beijerinckii')
    leg1 = ax1.legend(
        handles=[
            Line2D([0], [0], color=config.FLUX_COLORS_CB[i], linewidth=cfs.LW_DATA,
                   label=config.FLUX_DISPLAY_NAMES.get(fn, fn))
            for i, fn in enumerate(config.KEY_FLUXES_CB)
            if not df_cb.empty and f'{fn}_mean' in df_cb.columns
        ],
        loc='center left', bbox_to_anchor=(1.01, 0.5),
        fontsize=cfs.FS_LEGEND, frameon=False,
    )
    for txt in leg1.get_texts():
        txt.set_color('#222222')

    _draw_panel(ax2, df_mb, config.KEY_FLUXES_MB, config.FLUX_COLORS_MB)
    _style_ax(ax2, ylabel)
    _add_species_label(ax2, 'M. barkeri')
    ax2.set_xlabel('Time (h)', fontsize=cfs.COMBINED_PANEL_FS_LABEL, labelpad=2, color='#222222')
    leg2 = ax2.legend(
        handles=[
            Line2D([0], [0], color=config.FLUX_COLORS_MB[i], linewidth=cfs.LW_DATA,
                   label=config.FLUX_DISPLAY_NAMES.get(fn, fn))
            for i, fn in enumerate(config.KEY_FLUXES_MB)
            if not df_mb.empty and f'{fn}_mean' in df_mb.columns
        ],
        loc='center left', bbox_to_anchor=(1.01, 0.5),
        fontsize=cfs.FS_LEGEND, frameon=False,
    )
    for txt in leg2.get_texts():
        txt.set_color('#222222')

    def apply_ylims(t_max=None):
        y1 = _symmetric_ylim(df_cb, config.KEY_FLUXES_CB, t_min=0, t_max=t_max)
        y2 = _symmetric_ylim(df_mb, config.KEY_FLUXES_MB, t_min=0, t_max=t_max)
        ax1.set_ylim(-y1, y1)
        ax2.set_ylim(-y2, y2)
        for ax, yv in ((ax1, y1), (ax2, y2)):
            ax.set_yticks([-yv, 0, yv])
            ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda v, _: f'{v:.2g}'))

    fig.subplots_adjust(left=0.24, right=cfs.FLUX_ROW_RIGHT, top=0.96, bottom=0.14, hspace=0.12)
    apply_ylims(t_max=float(max_hours))
    for ax in axes:
        ax.set_xlim(0, max_hours)

    out_svg = os.path.join(save_dir, f'{stem}_0_{int(max_hours)}h.svg')
    out_png = os.path.join(save_dir, f'{stem}_0_{int(max_hours)}h.png')
    fig.savefig(out_svg, format='svg', dpi=300, bbox_inches='tight', facecolor='white')
    fig.savefig(out_png, format='png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    return [out_svg, out_png]
