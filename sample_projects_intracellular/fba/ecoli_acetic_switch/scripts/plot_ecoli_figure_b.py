#!/usr/bin/env python3
"""
Figure B – E. coli acetate-switch spatial panels
Reproduces panel B from the Ecoli_acetate_switch_paper reference figure.

Layout: 2 rows × 3 columns
  Rows    : Glucose (top), Acetate (bottom); substrate name on the shared row colorbar.
  Columns : T1 = 0.5 h, T2 = 2 h, T3 = 4.5 h (nearest saved frame used).
  Color   : One shared mM scale (and one colorbar) per substrate across the three columns.

Styling: "cell-systems" look matching the project's existing plot suite.

Default run uses the same PhysiCell output as Figure A
(``Ecoli_static_ox_glc_job34054344``). If a timepoint has no saved frame within
10 min of target, that panel is left empty with a note.
"""

import argparse
import os
import glob
from pathlib import Path
from typing import Optional
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.ticker as mticker
from matplotlib.cm import ScalarMappable
from matplotlib.ticker import ScalarFormatter
import scipy.io as sio
import xml.etree.ElementTree as ET

# ── paths (repo root = parent of analysis/) ─────────────────────────────────
_REPO_ROOT = Path(__file__).resolve().parents[1]


def _default_ecoli_output_folder() -> str:
    """Same discovery order as ``plot_ecoli_figure_a``."""
    candidates = [
        _REPO_ROOT
        / 'PhysiCelldFBA/marco_outputs/ecoli/ecoli/Ecoli_static_ox_glc_job34054344/output',
        _REPO_ROOT
        / 'mn5sync_pdfba/marco_outputs/ecoli/ecoli/Ecoli_static_ox_glc_job34054344/output',
        _REPO_ROOT
        / 'mn5sync_pdfba/marco_outputs/ecoli/ecoli/Ecoli_acetate_switch_paper'
        / 'Ecoli_static_ox_glc_job37998445/output',
    ]
    for p in candidates:
        if p.is_dir() and any(p.glob('output*.xml')):
            return str(p)
    return str(candidates[0])


OUTPUT_FOLDER = _default_ecoli_output_folder()
RESULTS_DIR = str(_REPO_ROOT / 'analysis/results/ecoli')

# ── timepoints and substrates ─────────────────────────────────────────────────
# Representative timepoint within each phase window of the canonical run
# (oxygen resupply at t3=3.5h): T1 in the aerobic phase (0-1.2h), T2 in the
# fermentative phase (1.2-3.4h), T3 in the aerobic-reutilisation phase
# (3.5-5.5h, was 6.5-8.5h before oxygen resupply moved to 3.5h).
TIMEPOINTS = [('T1', 0.5), ('T2', 2.0), ('T3', 4.5)]
SUBSTRATES = ['glucose', 'acetate']
SUB_ROW    = {'oxygen': 4, 'glucose': 5, 'acetate': 6, 'CO2': 7}
SUB_LABEL  = {'glucose': 'Glucose', 'acetate': 'Acetate'}

# ── colour maps (project standard substrate palette) ─────────────────────────
def _light_cmap(hex_color: str, name: str):
    return mcolors.LinearSegmentedColormap.from_list(
        name, ['#fafafa', hex_color], N=256
    )

CMAP = {
    'glucose': _light_cmap('#d4a017', 'glucose_map'),
    'acetate': _light_cmap('#cc0000', 'acetate_map'),
}

# ── gradient-arrow settings ───────────────────────────────────────────────────
ARROW_TARGET   = 8      # arrows per axis
ARROW_LEN_FRAC = 0.05   # as fraction of domain size
GRAD_THRESH_PCT = 10    # suppress arrows below this gradient %-ile
CONTOUR_LEVELS  = 60

# ── styling constants – two-column journal (≈170 mm / 6.8 in wide) ───────────
# 2 rows × 3 cols → each panel ≈ 1.5 × 1.7 in after colorbar.
# Font sizes tuned for print legibility at that density.
FS_TITLE     = 9
FS_ROW_LABEL = 9
FS_AXIS      = 8
FS_TICK      = 7
LW_SPINE     = 0.9


# ─────────────────────────────────────────────────────────────────────────────
def _frame_mats_exist(xml_path: str) -> bool:
    return (
        os.path.isfile(xml_path.replace('.xml', '_cells.mat'))
        and os.path.isfile(xml_path.replace('.xml', '_microenvironment0.mat'))
    )


def find_closest_frame(target_h: float, output_folder: str):
    """Return XML path nearest target_h, or None if gap > 10 min."""
    xml_files = [
        xf for xf in sorted(glob.glob(os.path.join(output_folder, 'output*.xml')))
        if _frame_mats_exist(xf)
    ]
    best, best_diff = None, float('inf')
    for xf in xml_files:
        t_h = float(ET.parse(xf).getroot().findtext('.//current_time')) / 60.0
        d = abs(t_h - target_h)
        if d < best_diff:
            best_diff, best = d, xf
    return best if best_diff <= 0.167 else None


def load_2d_field(xml_path: str, substrate: str):
    """Return (Xg, Yg, Z, t_h) – 2-D grid at the z = 0 slice."""
    t_h  = float(ET.parse(xml_path).getroot().findtext('.//current_time')) / 60.0
    menv = sio.loadmat(xml_path.replace('.xml', '_microenvironment0.mat')
                       )['multiscale_microenvironment']
    x, y, z = menv[0], menv[1], menv[2]
    vals     = menv[SUB_ROW[substrate]]

    z_target = np.unique(z)[np.argmin(np.abs(np.unique(z)))]
    mask     = z == z_target
    xs, ys, vs = x[mask], y[mask], vals[mask]

    x_uniq, y_uniq = np.unique(xs), np.unique(ys)
    Xg, Yg = np.meshgrid(x_uniq, y_uniq)
    Z = np.full((len(y_uniq), len(x_uniq)), np.nan)
    Z[np.searchsorted(y_uniq, ys), np.searchsorted(x_uniq, xs)] = vs
    return Xg, Yg, Z, t_h


def _gradient_arrows(Xg, Yg, Z):
    """Return (Xs, Ys, dU, dV, mask) for quiver overlay (−∇C direction)."""
    dx = float(np.diff(Xg[0]).mean())
    dy = float(np.diff(Yg[:, 0]).mean())
    Zf = np.where(np.isfinite(Z), Z, float(np.nanmedian(Z)))
    dZdy, dZdx = np.gradient(Zf, dy, dx)
    U, V  = -dZdx, -dZdy
    mag   = np.hypot(U, V)

    sx = max(1, Z.shape[1] // ARROW_TARGET)
    sy = max(1, Z.shape[0] // ARROW_TARGET)
    Xs, Ys = Xg[::sy, ::sx], Yg[::sy, ::sx]
    Us, Vs = U[::sy, ::sx], V[::sy, ::sx]
    Ms     = mag[::sy, ::sx]

    finite = np.isfinite(Ms)
    th     = np.percentile(Ms[finite], GRAD_THRESH_PCT) if finite.any() else np.inf
    mask   = finite & (Ms > th)
    Msafe  = np.where(Ms == 0, 1.0, Ms)
    L = min(Xg.max() - Xg.min(), Yg.max() - Yg.min()) * ARROW_LEN_FRAC
    return Xs, Ys, Us / Msafe * L, Vs / Msafe * L, mask


def _style_spatial_ax(ax):
    """Project-standard styling for a spatial panel axes."""
    ax.set_facecolor('white')
    for spine in ax.spines.values():
        spine.set_color('black')
        spine.set_linewidth(LW_SPINE)
    ax.tick_params(axis='both', which='major',
                   labelsize=FS_TICK, colors='black',
                   length=3, width=LW_SPINE * 0.6)
    ax.tick_params(axis='both', which='minor', length=0)


def _style_row_colorbar(cb):
    fmt = ScalarFormatter(useMathText=True)
    fmt.set_scientific(False)
    cb.ax.yaxis.set_major_formatter(fmt)
    cb.ax.tick_params(labelsize=FS_TICK - 1, colors='black', length=2, pad=2)
    cb.outline.set_linewidth(0.6)
    cb.outline.set_edgecolor('black')
    cb.locator = mticker.MaxNLocator(nbins=5)
    cb.update_ticks()


def _norm_for_row(z_arrays):
    """Shared Normalize (0 … vmax) for one substrate across all columns with data."""
    zs = [np.asarray(z, dtype=float) for z in z_arrays if z is not None and np.size(z)]
    if not zs:
        return mcolors.Normalize(0.0, 1.0)
    zall = np.concatenate([z[np.isfinite(z)].ravel() for z in zs])
    zall = zall[zall >= 0]
    if zall.size == 0:
        return mcolors.Normalize(0.0, 1.0)
    vmax = float(np.max(zall))
    if vmax <= 0.0:
        vmax = 1e-15
    return mcolors.Normalize(0.0, vmax)


def add_data_panel(
    ax,
    Xg,
    Yg,
    Z,
    substrate: str,
    norm: mcolors.Normalize,
    show_xlabel: bool,
    show_yticks: bool,
):
    """Filled contour + gradient arrows; color scale is fixed by ``norm`` (row-wide)."""
    vmin, vmax = float(norm.vmin), float(norm.vmax)
    Z = np.asarray(Z, dtype=float)
    Z = np.clip(np.clip(Z, 0, None), vmin, vmax)
    if not np.isfinite(vmax) or vmax <= vmin:
        levels = np.linspace(0.0, 1.0, CONTOUR_LEVELS + 1)
    else:
        levels = np.linspace(vmin, vmax, CONTOUR_LEVELS + 1)
    ax.contourf(Xg, Yg, Z, levels=levels, cmap=CMAP[substrate])

    # gradient arrows
    Xs, Ys, dU, dV, mask = _gradient_arrows(Xg, Yg, Z)
    if mask.any():
        ax.quiver(
            Xs[mask], Ys[mask], dU[mask], dV[mask],
            color='white', alpha=0.90,
            angles='xy', scale_units='xy', scale=1.0,
            width=0.005, headwidth=4.5, headlength=5,
            headaxislength=5.0, pivot='middle',
        )

    _style_spatial_ax(ax)
    ax.set_aspect('equal')
    if show_xlabel:
        ax.set_xlabel('X (μm)', fontsize=FS_AXIS, fontweight='normal', color='black')
    else:
        ax.set_xticks([])
    if show_yticks:
        ax.set_ylabel('Y (μm)', fontsize=FS_AXIS, fontweight='normal', color='black')
    else:
        ax.set_yticks([])


def add_empty_panel(ax, t_label: str, t_h: float, show_xlabel: bool, show_yticks: bool):
    """Styled empty panel for unavailable timepoints."""
    ax.set_facecolor('#f4f4f4')
    for spine in ax.spines.values():
        spine.set_color('#cccccc')
        spine.set_linewidth(LW_SPINE)
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_aspect('equal')
    ax.text(0.5, 0.5,
            f'No data\n(t = {t_h} h\nnot saved)',
            transform=ax.transAxes, ha='center', va='center',
            fontsize=FS_TICK, color='#aaaaaa', fontstyle='italic',
            linespacing=1.6)
    if show_xlabel:
        ax.set_xlabel('X (μm)', fontsize=FS_AXIS, fontweight='normal', color='black')
    if show_yticks:
        ax.set_ylabel('Y (μm)', fontsize=FS_AXIS, fontweight='normal', color='black')


# ─────────────────────────────────────────────────────────────────────────────
def plot_figure_b(output_folder: str, results_dir: str):
    n_rows, n_cols = len(SUBSTRATES), len(TIMEPOINTS)

    # ── Pass 1: nearest frame per column + field grids ───────────────────────
    grids = {}
    for c, (t_label, t_h) in enumerate(TIMEPOINTS):
        xml_path = find_closest_frame(t_h, output_folder)
        for r, sub in enumerate(SUBSTRATES):
            if xml_path is None:
                grids[(r, c)] = None
            else:
                Xg, Yg, Z, actual_t = load_2d_field(xml_path, sub)
                Z = np.clip(np.asarray(Z, dtype=float), 0, None)
                grids[(r, c)] = (Xg, Yg, Z, actual_t)

    row_norms = {}
    for r, sub in enumerate(SUBSTRATES):
        z_list = [grids[(r, c)][2] for c in range(n_cols) if grids.get((r, c)) is not None]
        row_norms[r] = _norm_for_row(z_list)

    # Slightly narrower width: one colorbar per row instead of per panel.
    fig, axes = plt.subplots(
        n_rows,
        n_cols,
        figsize=(6.45, 2.1 * n_rows),
        squeeze=False,
        layout='constrained',
    )
    fig.patch.set_facecolor('white')

    for c, (t_label, t_h) in enumerate(TIMEPOINTS):
        axes[0, c].set_title(
            f'{t_label}  (t = {t_h:g} h)',
            fontsize=FS_TITLE, fontweight='normal', color='black', pad=6,
        )

    for c, (t_label, t_h) in enumerate(TIMEPOINTS):
        for r, sub in enumerate(SUBSTRATES):
            ax = axes[r, c]
            show_xlabel = r == n_rows - 1
            show_yticks = c == 0
            cell = grids.get((r, c))
            if cell is None:
                add_empty_panel(ax, t_label, t_h, show_xlabel, show_yticks)
            else:
                Xg, Yg, Z, _actual_t = cell
                add_data_panel(
                    ax,
                    Xg,
                    Yg,
                    Z,
                    sub,
                    row_norms[r],
                    show_xlabel,
                    show_yticks,
                )

    # One colorbar per substrate row (shared mM scale).
    for r, sub in enumerate(SUBSTRATES):
        row_axes = axes[r, :].ravel().tolist()
        has_data = any(grids.get((r, c)) is not None for c in range(n_cols))
        if not has_data:
            continue
        norm = row_norms[r]
        sm = ScalarMappable(norm=norm, cmap=CMAP[sub])
        sm.set_array([])
        cb = fig.colorbar(
            sm,
            ax=row_axes,
            orientation='vertical',
            fraction=0.046,
            pad=0.03,
        )
        _style_row_colorbar(cb)
        cb.set_label(
            SUB_LABEL[sub],
            fontsize=FS_ROW_LABEL,
            fontweight='normal',
            color='black',
            rotation=-90,
            labelpad=12,
        )

    os.makedirs(results_dir, exist_ok=True)
    for ext in ('png', 'svg'):
        path = os.path.join(results_dir, f'figure_B.{ext}')
        fig.savefig(path, dpi=300, bbox_inches='tight', facecolor='white')
        print(f'Saved: {path}')
    plt.close(fig)


def _infer_sample_tag(output_folder: str) -> str:
    p = Path(output_folder).resolve()
    return p.parent.name if p.name == 'output' else p.name


def _resolve_results_dir(output_folder: str, results_dir_arg: Optional[str]) -> str:
    if results_dir_arg is not None:
        return results_dir_arg
    if os.path.abspath(output_folder) == os.path.abspath(OUTPUT_FOLDER):
        return RESULTS_DIR
    return os.path.join(RESULTS_DIR, _infer_sample_tag(output_folder))


# ─────────────────────────────────────────────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(description='E. coli acetate-switch Figure B (spatial panels).')
    ap.add_argument(
        '--output-folder',
        default=None,
        metavar='DIR',
        help='PhysiCell output directory (contains output*.xml). '
             'Default: same as Figure A (PhysiCelldFBA or mn5sync_pdfba job34054344, then fallback).',
    )
    ap.add_argument(
        '--results-dir',
        default=None,
        metavar='DIR',
        help='Where to write figure_B.png/svg. Default: analysis/results/ecoli, or a subfolder named after the sample when --output-folder differs from the default.',
    )
    args = ap.parse_args()
    out_folder = args.output_folder or OUTPUT_FOLDER
    results_dir = _resolve_results_dir(out_folder, args.results_dir)
    os.makedirs(results_dir, exist_ok=True)

    print('Building Figure B …')
    for t_label, t_h in TIMEPOINTS:
        xf = find_closest_frame(t_h, out_folder)
        if xf:
            actual = float(ET.parse(xf).getroot().findtext('.//current_time')) / 60.0
            print(f'  {t_label} ({t_h} h) → {os.path.basename(xf)} (t = {actual:.3f} h)')
        else:
            print(f'  {t_label} ({t_h} h) → NO DATA')
    plot_figure_b(out_folder, results_dir)


if __name__ == '__main__':
    main()
