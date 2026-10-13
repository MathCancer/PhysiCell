"""Initial/final (+ optional mid) growth + H₂ + acetate spatial composite
(per-frame scale).

plot_spatial_composite() saves two versions: the original 2-column
initial/final layout, and (by default, mid_hours=25.0) an additional
3-column initial/mid/final sequence. Pass mid_hours=None to skip the
3-column version.
"""

from __future__ import annotations

import os
import sys

import matplotlib.colors as mcolors
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.cm import ScalarMappable
from matplotlib.colors import PowerNorm
from matplotlib.ticker import ScalarFormatter
from scipy.interpolate import griddata

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(_SCRIPT_DIR)
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

from lib import composite_figure_style as cfs
from lib.io import get_domain_extent, get_xml_cell_indices, open_reader

SUBSTRATE_HEX = {'acetate': '#cc0000'}


def _to_cmap(hex_color: str, name: str):
    return mcolors.LinearSegmentedColormap.from_list(name, ['#f8f9fa', hex_color], N=256)


SUBSTRATE_CMAPS = {
    'acetate': _to_cmap(SUBSTRATE_HEX['acetate'], 'acetate_map'),
    'H2': _to_cmap('#457b9d', 'h2_map'),
    'h2': _to_cmap('#457b9d', 'h2_map'),
}


def _reshape_to_grid(x, y, v):
    xu = np.unique(x)
    yu = np.unique(y)
    nx, ny = xu.size, yu.size
    if nx * ny != v.size:
        raise ValueError(f'Cannot reshape grid: expected {nx * ny}, got {v.size}')
    ix = np.searchsorted(xu, x)
    iy = np.searchsorted(yu, y)
    Z = np.full((ny, nx), np.nan)
    Z[iy, ix] = v
    dx = np.diff(xu).mean() if nx > 1 else 1.0
    dy = np.diff(yu).mean() if ny > 1 else 1.0
    Xg, Yg = np.meshgrid(xu, yu, indexing='xy')
    return Xg, Yg, Z, dx, dy


def _soft_zero(Z, abs_eps=1e-10, rel_eps=1e-6, clamp_nonneg=False):
    Z = Z.copy()
    finite = np.isfinite(Z)
    if not finite.any():
        return Z
    max_abs = np.nanmax(np.abs(Z[finite]))
    eps = max(abs_eps, rel_eps * max_abs) if max_abs > 0 else abs_eps
    Z[np.abs(Z) < eps] = 0.0
    if clamp_nonneg:
        Z[Z < 0] = 0.0
    return Z


def _resolve_substrate(substrate_names, candidates):
    lower_map = {n.lower(): n for n in substrate_names}
    for c in candidates:
        if c in substrate_names:
            return substrate_names.index(c), c
        if c.lower() in lower_map:
            actual = lower_map[c.lower()]
            return substrate_names.index(actual), actual
    return None, None


def _field_grid(A, sub_idx):
    x, y = A[0, :], A[1, :]
    vals = A[sub_idx + 4, :]
    try:
        Xg, Yg, Z, dx, dy = _reshape_to_grid(x, y, vals)
    except ValueError:
        x_unique = np.unique(x)
        y_unique = np.unique(y)
        Xg, Yg = np.meshgrid(x_unique, y_unique)
        points = np.column_stack((x, y))
        Z = griddata(points, vals, (Xg, Yg), method='cubic')
        dx = np.diff(x_unique).mean() if len(x_unique) > 1 else 1.0
        dy = np.diff(y_unique).mean() if len(y_unique) > 1 else 1.0
    Z = _soft_zero(Z, clamp_nonneg=True)
    extent = [float(Xg.min()), float(Xg.max()), float(Yg.min()), float(Yg.max())]
    return Z, extent, Xg, Yg, dx, dy


def _overlay_streamlines(ax, Xg, Yg, Z, dx, dy, color='white', density=1.1, linewidth=1.1, arrowsize=1.4):
    """Overlay continuous streamlines following the field's negative gradient
    (high -> low concentration), to emphasize the directionality of diffusive
    substrate cross-feeding/exchange -- rather than discrete arrows at sparse
    sample points.

    density: matplotlib streamplot density (roughly, lines per grid cell in
        each direction) -- lower this if there are too many lines.
    linewidth / arrowsize: absolute point sizes -- lower these if the lines
        or arrowheads look too big. These are independent of FIG_SCALE so
        they can be tuned without affecting other line weights in the figure.
    """
    Z_med = np.nanmedian(Z) if np.isfinite(Z).any() else 0.0
    Zg = np.where(np.isfinite(Z), Z, Z_med)
    dZdy, dZdx = np.gradient(Zg, dy, dx)
    U, V = -dZdx, -dZdy
    mag = np.hypot(U, V)
    finite = np.isfinite(mag)
    # Zero out the weakest 10% of gradients (near-flat regions), where the
    # local direction is numerical noise rather than a meaningful flow.
    th = np.percentile(mag[finite], 10) if finite.any() else 0.0
    weak = ~finite | (mag < th)
    U, V = np.where(weak, 0.0, U), np.where(weak, 0.0, V)
    ax.streamplot(
        Xg[0, :], Yg[:, 0], U, V,
        color=color, density=density,
        linewidth=linewidth, arrowsize=arrowsize,
        arrowstyle='-|>', zorder=3,
    )


def _nearest_snapshot(cell_by_time, t_target):
    if t_target in cell_by_time:
        return cell_by_time[t_target], t_target
    times = sorted(cell_by_time.keys())
    if not times:
        return None, None
    tn = min(times, key=lambda s: abs(s - t_target))
    return cell_by_time[tn], tn


def _panel_vmin_vmax(Z):
    finite = Z[np.isfinite(Z)]
    if finite.size == 0:
        return 0.0, 1.0
    lo, hi = float(np.nanmin(finite)), float(np.nanmax(finite))
    if hi - lo < 1e-12:
        hi = lo + 1e-6
    return lo, hi


def plot_spatial_composite(
    output_dir: str, save_dir: str, max_hours: int, dpi: int = 300, mid_hours: float | None = 25.0,
) -> list[str]:
    """Render the spatial composite twice: once as the original 2-column
    initial/final layout, and once (if mid_hours is reachable) as a 3-column
    initial/mid/final sequence. Both share identical growth-rate color
    scaling for consistency between the two outputs.
    """
    max_min = float(max_hours) * 60.0
    reader = open_reader(output_dir)
    substrate_names = [n for n, _u, _i in reader.microenvironment_columns]

    me_list = []
    for t, A in reader.microenvironment_as_matrix_iterator():
        if t <= 0 or t > max_min or A is None:
            continue
        me_list.append((t, A))
    if len(me_list) < 2:
        raise RuntimeError(f'Need ≥2 microenvironment snapshots within 0–{max_hours} h')

    (t0, A0), (t1, A1) = me_list[0], me_list[-1]

    # Optional middle snapshot (e.g. 25 h) for the extra 3-column output.
    # Skipped if unreachable (outside the window, or the nearest snapshot
    # coincides with t0/t1).
    mid_snapshot = None
    if mid_hours is not None:
        t_mid_target = float(mid_hours) * 60.0
        if t0 < t_mid_target < t1:
            t_mid, A_mid = min(me_list, key=lambda ta: abs(ta[0] - t_mid_target))
            if t_mid not in (t0, t1):
                mid_snapshot = (t_mid, A_mid)

    cell_by_time = {}
    for t, cells in reader.cells_as_matrix_iterator():
        if 0 < t <= max_min and cells is not None:
            cell_by_time[t] = cells

    h2_idx, h2_name = _resolve_substrate(substrate_names, ['H2', 'h2', 'hydrogen'])
    ac_idx, ac_name = _resolve_substrate(substrate_names, ['acetate', 'Acetate'])
    if h2_idx is None or ac_idx is None:
        raise RuntimeError('H₂ or acetate not found in microenvironment')

    x_min, x_max, y_min, y_max = get_domain_extent(output_dir)
    cols = reader.cell_columns
    name2idx = {n: i for i, n in enumerate(cols)}
    xml_idx = get_xml_cell_indices(output_dir)
    x_idx = xml_idx.get('x_position', name2idx.get('x_position', 1))
    y_idx = xml_idx.get('y_position', name2idx.get('y_position', 2))
    cell_type_idx = xml_idx.get('cell_type', name2idx.get('cell_type', 5))
    cb_gr = xml_idx.get('CB_growth_rate', name2idx.get('CB_growth_rate'))
    mb_gr = xml_idx.get('MB_growth_rate', name2idx.get('MB_growth_rate'))
    leg_gr = xml_idx.get('growth_rate', name2idx.get('growth_rate'))

    types_to_plot = [0, 1]
    cmap_gr = {0: 'Blues', 1: 'Oranges'}
    type_names = {0: 'CB', 1: 'MB'}

    def cell_growth_vectors(cells):
        x = cells[:, x_idx].astype(float)
        y = cells[:, y_idx].astype(float)
        ctype = cells[:, cell_type_idx].astype(int)
        gr = np.zeros(cells.shape[0], dtype=float)
        for tid in np.unique(ctype):
            m = ctype == tid
            if tid == 0 and cb_gr is not None and cb_gr < cells.shape[1]:
                gr[m] = cells[m, cb_gr]
            elif tid == 1 and mb_gr is not None and mb_gr < cells.shape[1]:
                gr[m] = cells[m, mb_gr]
            elif leg_gr is not None and leg_gr < cells.shape[1]:
                gr[m] = cells[m, leg_gr]
        return x, y, ctype, np.clip(gr, 0, None)

    # Growth-rate color normalization computed once over every snapshot that
    # could appear in either output (2-col or 3-col), so both figures share
    # identical color scaling.
    all_snapshots = [(t0, A0)] + ([mid_snapshot] if mid_snapshot is not None else []) + [(t1, A1)]
    gr_max = {tid: 0.0 for tid in types_to_plot}
    for t_snap, _A in all_snapshots:
        cmat, _ = _nearest_snapshot(cell_by_time, t_snap)
        if cmat is None or cmat.shape[0] == 0:
            continue
        _, _, ctype, gr = cell_growth_vectors(cmat)
        for tid in types_to_plot:
            m = ctype == tid
            if np.any(m):
                gr_max[tid] = max(gr_max[tid], float(np.max(gr[m])))
    for tid in types_to_plot:
        if gr_max[tid] <= 0:
            gr_max[tid] = 1.0
    norm_gr = {
        tid: PowerNorm(gamma=0.6, vmin=0.0, vmax=gr_max[tid])
        for tid in types_to_plot
    }

    # Field grids for H2 and acetate, computed once per distinct snapshot
    # and reused across both output figures.
    _field_cache: dict[float, tuple] = {}

    def get_fields(t_snap, A):
        if t_snap not in _field_cache:
            _field_cache[t_snap] = (_field_grid(A, h2_idx), _field_grid(A, ac_idx))
        return _field_cache[t_snap]

    cmap_h2 = SUBSTRATE_CMAPS.get(h2_name, SUBSTRATE_CMAPS['H2'])
    cmap_ac = SUBSTRATE_CMAPS.get(ac_name, SUBSTRATE_CMAPS['acetate'])

    FIG_SCALE = 2.0
    point_size = 1.0
    _fs_title = cfs.FS_TITLE * FIG_SCALE
    # Row labels (Growth rate / H2 / Acetate) and the growth-rate colorbar
    # labels share one standardized, larger size (previously 13 pt vs 12 pt).
    _fs_shared_label = 15
    _fs_rowlbl = _fs_shared_label
    _fs_cb_lbl = _fs_shared_label
    _fs_cb_tick = cfs.FS_CB_TICK * FIG_SCALE
    _lw_frame = 0.4 * FIG_SCALE
    _scatter_s = point_size * FIG_SCALE * FIG_SCALE
    _scatter_lw = 0.04 * FIG_SCALE

    # Streamline density/size -- fewer, smaller lines than the previous
    # defaults (density 1.1, linewidth 1.1, arrowsize 1.4). Tune these
    # directly if the flow-direction overlay still looks too dense/large.
    STREAM_DENSITY = 0.6
    STREAM_LINEWIDTH = 0.6
    STREAM_ARROWSIZE = 0.8

    def render(snapshots, stem):
        """Build and save one composite (2 or 3 columns, per len(snapshots))."""
        n_cols = len(snapshots)
        h2_fields = [get_fields(t, A)[0] for t, A in snapshots]
        ac_fields = [get_fields(t, A)[1] for t, A in snapshots]

        fig, axes = plt.subplots(
            3, n_cols, figsize=(cfs.FIG_W_IN * FIG_SCALE * (n_cols / 2.0), cfs.FIG_H_IN * FIG_SCALE),
            sharex=True, sharey=True, facecolor='white', squeeze=False,
        )
        for ax in axes.flat:
            ax.set_facecolor('white')
            ax.set_xlim(x_min, x_max)
            ax.set_ylim(y_min, y_max)
            ax.set_aspect('equal', adjustable='box')
            for spine in ax.spines.values():
                spine.set_color('#000000')
                spine.set_linewidth(_lw_frame)
            ax.set_xticks([])
            ax.set_yticks([])

        col_times_h = []
        for col, (t_snap, _A) in enumerate(snapshots):
            ax = axes[0, col]
            cmat, t_used = _nearest_snapshot(cell_by_time, t_snap)
            t_used = t_used if t_used is not None else t_snap
            col_times_h.append(t_used / 60.0)
            if cmat is None or cmat.shape[0] == 0:
                continue
            x, y, ctype, gr = cell_growth_vectors(cmat)
            for tid in types_to_plot:
                m = ctype == tid
                if not np.any(m):
                    continue
                ec = '#08306b' if tid == 0 else '#7f2704'
                ax.scatter(
                    x[m], y[m], c=gr[m],
                    cmap=plt.get_cmap(cmap_gr[tid]), norm=norm_gr[tid],
                    s=_scatter_s, edgecolors=ec, linewidths=_scatter_lw,
                    alpha=0.92, zorder=tid + 1,
                )

        micro_rows = [
            (1, h2_fields, cmap_h2, '#555555'),
            (2, ac_fields, cmap_ac, '#222222'),
        ]
        for row, fields, cmap, arrow_color in micro_rows:
            vranges = [_panel_vmin_vmax(Z) for (Z, _ext, _Xg, _Yg, _dx, _dy) in fields]
            for col, ((Z, extent, Xg, Yg, dx, dy), (vmin, vmax)) in enumerate(zip(fields, vranges)):
                ax = axes[row, col]
                ax.imshow(
                    Z, origin='lower', extent=extent, aspect='equal',
                    cmap=cmap, vmin=vmin, vmax=vmax, interpolation='bilinear',
                )
                _overlay_streamlines(
                    ax, Xg, Yg, Z, dx, dy, color=arrow_color,
                    density=STREAM_DENSITY, linewidth=STREAM_LINEWIDTH, arrowsize=STREAM_ARROWSIZE,
                )

        plt.subplots_adjust(left=0.09, right=0.84, top=0.90, bottom=0.06, wspace=0.01, hspace=0.06)
        gap_col = 0.040
        x0_ref = axes[0, 0].get_position().x0
        for row in range(3):
            pos0 = axes[row, 0].get_position()
            side = min(pos0.width, pos0.height)
            for col in range(n_cols):
                axes[row, col].set_position([x0_ref + col * (side + gap_col), pos0.y0, side, pos0.height])

        pos0 = axes[0, n_cols - 1].get_position()
        h_g = pos0.y1 - pos0.y0
        y0g = pos0.y0
        x_cb_g = pos0.x1 + 0.012
        wcb_g = 0.014
        cax_g0 = fig.add_axes([x_cb_g, y0g + 0.52 * h_g, wcb_g, 0.42 * h_g])
        cax_g1 = fig.add_axes([x_cb_g, y0g + 0.06 * h_g, wcb_g, 0.42 * h_g])
        fmt_gr = ScalarFormatter(useMathText=True)
        fmt_gr.set_scientific(False)
        for tid, cax in zip(types_to_plot, (cax_g0, cax_g1)):
            sm = ScalarMappable(cmap=plt.get_cmap(cmap_gr[tid]), norm=norm_gr[tid])
            sm.set_array([])
            cb = fig.colorbar(sm, cax=cax)
            cb.set_label(f'{type_names[tid]}\n(1/h)', fontsize=_fs_cb_lbl, labelpad=2, color='#222222', style='normal')
            cb.ax.tick_params(labelsize=_fs_cb_tick, colors='#222222', width=_lw_frame, length=2 * FIG_SCALE)
            cb.ax.yaxis.set_major_formatter(fmt_gr)
            cb.outline.set_edgecolor('#000000')
            cb.outline.set_linewidth(_lw_frame)

        row_lbl_pad = 0.040
        for row, lbl in enumerate(('Growth rate', 'H₂ (mM)', 'Acetate (mM)')):
            pos = axes[row, 0].get_position()
            y_mid = 0.5 * (pos.y0 + pos.y1)
            fig.text(
                pos.x0 - row_lbl_pad, y_mid, lbl,
                rotation=90, ha='center', va='center',
                fontsize=_fs_rowlbl, color='#222222', style='normal',
            )
        for col, t_h in enumerate(col_times_h):
            pos = axes[0, col].get_position()
            x_mid = 0.5 * (pos.x0 + pos.x1)
            fig.text(
                x_mid, pos.y1 + 0.012, f't = {t_h:.1f} hr',
                ha='center', va='bottom', fontsize=_fs_title, color='#222222', style='normal',
            )

        out_svg = os.path.join(save_dir, f'{stem}.svg')
        out_png = os.path.join(save_dir, f'{stem}.png')
        fig.savefig(out_svg, format='svg', dpi=dpi, facecolor='white')
        fig.savefig(out_png, format='png', dpi=dpi, facecolor='white')
        plt.close(fig)
        return [out_svg, out_png]

    base_stem = 'microenvironment_initial_final_growth_H2_acetate_per_frame'
    saved = render([(t0, A0), (t1, A1)], base_stem)
    if mid_snapshot is not None:
        mid_h = mid_snapshot[0] / 60.0
        saved += render([(t0, A0), mid_snapshot, (t1, A1)], f'{base_stem}_mid{mid_h:g}h')
    return saved
