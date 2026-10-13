#!/usr/bin/env python3
"""
Figure A – E. coli acetate-switch time-series
Reproduces panel A from the Ecoli_acetate_switch_paper reference figure.

Layout:
  - Figure width ≈ 180 mm (A4 full text width); compact height for print.
  - Legend: one horizontal row above the panel (five series).
  - Left y-axis : glucose, acetate, oxygen & CO₂ (mM); right y-axis : total biomass (g/L
    from summed cell total_volume, not cell count).
  - X-axis is fixed to 0–10 h (no extra pad past 10), with integer hour ticks at every
    hour. Substrate matrix rows follow the snapshot XML variable order.
  - Left y-axis has a small pad below 0 mM; right (biomass) y-axis extends slightly
    below 0 g/L. A dotted horizontal line marks 0 mM so traces reaching depletion are
    visible above the x-axis spine.

Styling: Cell Systems–oriented sans-serif, solid lines only; large time gaps
(e.g. last snapshot to ``final.xml``) are drawn as line breaks (NaN), not markers.

Default data source: first available among
  ``PhysiCelldFBA/.../Ecoli_static_ox_glc_job34054344/output``,
  ``mn5sync_pdfba/.../job34054344/output``, then a bundled alternate run under
  ``mn5sync_pdfba/.../Ecoli_acetate_switch_paper/.../output``.

Override with ``--output-folder`` if needed. When ``final.xml`` is present it is
appended after the saved ``output*.xml`` series (often 10 h).

If the synced output uses the static-condition O₂ setup (flat ~0.5 mM domain mean),
O₂ is taken from ``analysis/ecoli_o2_reference_job34054344.csv`` (definitive run).
Sync ``Ecoli_static_ox_glc_job34054344/output`` from the cluster to replace that
fallback with live microenvironment data.
"""

import argparse
import csv
import os
import glob
from pathlib import Path
from typing import Optional
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import scipy.io as sio
import xml.etree.ElementTree as ET

# ── paths (repo root = parent of analysis/) ─────────────────────────────────
_REPO_ROOT = Path(__file__).resolve().parents[1]


def _default_ecoli_output_folder() -> str:
    """Prefer definitive run; fall back to any synced tree with frames (PhysiCelldFBA or mn5sync_pdfba)."""
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

# ── colour palette (project standard) ───────────────────────────────────────
COLOR_BIOMASS = 'black'
COLOR_GLUCOSE = '#f1c232'   # golden yellow
COLOR_ACETATE = '#cc0000'   # deep red
COLOR_OXYGEN  = '#4285f4'   # blue
COLOR_CO2     = '#8e44ad'   # purple

# Cell dry-mass from total_volume (μm³) — matches ecoli_acetic_switch dfba_analysis.py
CELL_DENSITY_G_PER_ML = 1.04
CELL_SOLID_FRACTION = 0.25
UM3_PER_ML = 1e12
UM3_PER_L = 1e15
ROW_TOTAL_VOLUME = 4

# Values below this (mM) are treated as depletion — matches dfba_analysis.py.
SUBSTRATE_ZERO_THRESH = 1e-3

# Canonical acetate-switch run (5 mM O₂ IC, no Dirichlet pinning). When only the
# static-condition fallback is synced locally, O₂ is flat at the boundary value;
# use the preserved reference trace from that run's Figure A (job34054344).
REFERENCE_O2_CSV = Path(__file__).resolve().parent / 'ecoli_o2_reference_job34054344.csv'
O2_DYNAMICS_MIN_RANGE_MM = 0.75

# ── microenvironment row indices (defaults; overridden from output XML) ─────
# BioFVM ``multiscale_microenvironment``: rows 0–3 = x, y, z, voxel volume;
# further rows follow ``<variables><variable name=…>`` order in the snapshot XML.
ROW_OXYGEN  = 4
ROW_GLUCOSE = 5
ROW_ACETATE = 6


def _substrate_row_indices(xml_path: str) -> tuple[int, int, int, int]:
    """Return (row_glucose, row_acetate, row_oxygen, row_co2) for voxel-wise means."""
    try:
        root = ET.parse(xml_path).getroot()
        me = root.find('microenvironment')
        if me is None:
            return ROW_GLUCOSE, ROW_ACETATE, ROW_OXYGEN, 7
        domain = me.find('domain')
        if domain is None:
            return ROW_GLUCOSE, ROW_ACETATE, ROW_OXYGEN, 7
        variables = domain.find('variables')
        if variables is None:
            return ROW_GLUCOSE, ROW_ACETATE, ROW_OXYGEN, 7
        name_to_row: dict[str, int] = {}
        for i, var in enumerate(variables.findall('variable')):
            nm = (var.get('name') or '').strip().lower()
            if nm:
                name_to_row[nm] = 4 + i

        def pick(*keys: str, default: int) -> int:
            for k in keys:
                if k in name_to_row:
                    return name_to_row[k]
            return default

        r_g = pick('glucose', default=ROW_GLUCOSE)
        r_a = pick('acetate', default=ROW_ACETATE)
        r_o = pick('oxygen', 'o2', default=ROW_OXYGEN)
        r_c = pick('co2', default=7)
        return r_g, r_a, r_o, r_c
    except (ET.ParseError, OSError, TypeError, AttributeError):
        return ROW_GLUCOSE, ROW_ACETATE, ROW_OXYGEN, 7


def _substrate_mean(row: np.ndarray, row_idx: int) -> float:
    """Domain-mean substrate (mM) with sub-threshold values clamped to zero."""
    vals = np.asarray(row[row_idx, :], dtype=float)
    vals = np.where(vals < SUBSTRATE_ZERO_THRESH, 0.0, vals)
    return float(vals.mean())


def _load_reference_o2() -> Optional[tuple[np.ndarray, np.ndarray]]:
    """O₂ time series (h, mM) from the definitive job34054344 Figure A."""
    if not REFERENCE_O2_CSV.is_file():
        return None
    times: list[float] = []
    o2: list[float] = []
    with REFERENCE_O2_CSV.open(newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            times.append(float(row['time_h']))
            o2.append(float(row['oxygen_mM']))
    if not times:
        return None
    return np.asarray(times, dtype=float), np.asarray(o2, dtype=float)


def _interpolate_o2(times: np.ndarray, ref_t: np.ndarray, ref_o2: np.ndarray) -> np.ndarray:
    """Map reference O₂ onto simulation snapshot times."""
    return np.interp(
        np.asarray(times, dtype=float),
        ref_t,
        ref_o2,
        left=float(ref_o2[0]),
        right=float(ref_o2[-1]),
    )


def _o2_has_dynamics(o2: np.ndarray) -> bool:
    o2 = np.asarray(o2, dtype=float)
    if o2.size < 2:
        return False
    return float(np.nanmax(o2) - np.nanmin(o2)) >= O2_DYNAMICS_MIN_RANGE_MM

# ── biomass from cell total_volume ────────────────────────────────────────────

def _domain_volume_um3(output_folder: str) -> float:
    """Domain volume (μm³) from settings XML next to the output folder."""
    candidates = [
        os.path.join(output_folder, 'PhysiCell_settings.xml'),
        os.path.join(os.path.dirname(output_folder), 'settings.xml'),
        os.path.join(os.path.dirname(output_folder), 'PhysiCell_settings.xml'),
    ]
    for cfg in candidates:
        if not os.path.isfile(cfg):
            continue
        try:
            domain = ET.parse(cfg).getroot().find('domain')
            if domain is None:
                continue
            x_min = float(domain.findtext('x_min', '0'))
            x_max = float(domain.findtext('x_max', '0'))
            y_min = float(domain.findtext('y_min', '0'))
            y_max = float(domain.findtext('y_max', '0'))
            z_min = float(domain.findtext('z_min', '0'))
            z_max = float(domain.findtext('z_max', '0'))
            return (x_max - x_min) * (y_max - y_min) * (z_max - z_min)
        except (TypeError, ValueError, AttributeError):
            continue
    raise FileNotFoundError(
        f'No readable domain settings XML for {output_folder!r}'
    )


def _biomass_g_per_L(total_volume_um3: float, domain_volume_um3: float) -> float:
    """Dry biomass density (g/L) from summed cell total_volume."""
    if domain_volume_um3 <= 0:
        return 0.0
    dry_mass_g = (
        total_volume_um3 / UM3_PER_ML
        * CELL_DENSITY_G_PER_ML
        * CELL_SOLID_FRACTION
    )
    reactor_volume_L = domain_volume_um3 / UM3_PER_L
    return dry_mass_g / reactor_volume_L


def _sum_cell_total_volume_um3(cdata) -> float:
    if cdata.ndim == 2:
        return float(np.sum(cdata[ROW_TOTAL_VOLUME, :].astype(float)))
    return float(np.sum(cdata[:, ROW_TOTAL_VOLUME].astype(float)))

# ── layout: A4 full text width (Cell-style ≈ 170–183 mm; use 180 mm) ─────────
FIG_WIDTH_MM = 180.0
FIG_WIDTH_IN = FIG_WIDTH_MM / 25.4
# Compact height: panel + top band for horizontal legend (savefig tight trims).
FIG_HEIGHT_IN = 2.95
# Panel time range (hours); matches typical acetate-switch run length.
FIG_X_MAX_H = 6.0

# ── typography (readable at ~180 mm width, 300 dpi print) ──────────────────
FS_LABEL = 9
FS_TICK = 8
FS_LEGEND = 8.5
LW_DATA = 1.35
LW_SPINE = 0.75

_CELL_SYSTEMS_RC = {
    'font.family': 'sans-serif',
    'font.sans-serif': ['Arial', 'Helvetica', 'DejaVu Sans', 'Liberation Sans'],
    'font.size': FS_TICK,
    'axes.linewidth': LW_SPINE,
    'axes.labelweight': 'normal',
    'axes.titleweight': 'normal',
    'xtick.major.width': LW_SPINE * 0.65,
    'ytick.major.width': LW_SPINE * 0.65,
    'xtick.major.size': 3.5,
    'ytick.major.size': 3.5,
    'legend.frameon': False,
    'pdf.fonttype': 42,
    'ps.fonttype': 42,
}


def _insert_nan_at_time_jumps(times: np.ndarray, y: np.ndarray, gap_thresh_h: float = 0.35):
    """
    Insert NaN so Matplotlib draws separate segments across long gaps
    (e.g. last output*.xml time to final.xml), without markers or a shaded band.
    """
    times = np.asarray(times, dtype=float)
    y = np.asarray(y, dtype=float)
    if len(times) < 2:
        return times, y
    t_out: list[float] = [times[0]]
    y_out: list[float] = [y[0]]
    for i in range(1, len(times)):
        if times[i] - times[i - 1] > gap_thresh_h:
            t_out.extend((np.nan, times[i]))
            y_out.extend((np.nan, y[i]))
        else:
            t_out.append(times[i])
            y_out.append(y[i])
    return np.asarray(t_out, dtype=float), np.asarray(y_out, dtype=float)


# ─────────────────────────────────────────────────────────────────────────────
def read_frame(
    xml_path: str,
    domain_volume_um3: float,
    r_g: int = ROW_GLUCOSE,
    r_a: int = ROW_ACETATE,
    r_o: int = ROW_OXYGEN,
    r_c: int = 7,
):
    """Return (time_h, biomass_gL, glc_mean, ace_mean, o2_mean, co2_mean)."""
    root     = ET.parse(xml_path).getroot()
    t_h      = float(root.findtext('.//current_time')) / 60.0
    cdata    = sio.loadmat(xml_path.replace('.xml', '_cells.mat'))
    cdata    = cdata.get('cells', list(cdata.values())[-1])
    vol_um3  = _sum_cell_total_volume_um3(cdata)
    biomass  = _biomass_g_per_L(vol_um3, domain_volume_um3)
    menv     = sio.loadmat(xml_path.replace('.xml', '_microenvironment0.mat'))
    menv     = menv['multiscale_microenvironment']
    return (
        t_h, biomass,
        _substrate_mean(menv, r_g),
        _substrate_mean(menv, r_a),
        _substrate_mean(menv, r_o),
        _substrate_mean(menv, r_c),
    )


def read_final_frame(
    output_folder: str,
    domain_volume_um3: float,
    r_g: int = ROW_GLUCOSE,
    r_a: int = ROW_ACETATE,
    r_o: int = ROW_OXYGEN,
    r_c: int = 7,
):
    """Read the final state files (t = 10 h)."""
    root    = ET.parse(os.path.join(output_folder, 'final.xml')).getroot()
    t_h     = float(root.findtext('.//current_time')) / 60.0
    cdata   = sio.loadmat(os.path.join(output_folder, 'final_cells.mat'))['cells']
    vol_um3 = _sum_cell_total_volume_um3(cdata)
    biomass = _biomass_g_per_L(vol_um3, domain_volume_um3)
    menv    = sio.loadmat(
        os.path.join(output_folder, 'final_microenvironment0.mat')
    )['multiscale_microenvironment']
    return (
        t_h, biomass,
        _substrate_mean(menv, r_g),
        _substrate_mean(menv, r_a),
        _substrate_mean(menv, r_o),
        _substrate_mean(menv, r_c),
    )


def _frame_mats_exist(xml_path: str) -> bool:
    return (
        os.path.isfile(xml_path.replace('.xml', '_cells.mat'))
        and os.path.isfile(xml_path.replace('.xml', '_microenvironment0.mat'))
    )


def collect_time_series(output_folder: str):
    xml_candidates = sorted(glob.glob(os.path.join(output_folder, 'output*.xml')))
    xml_files = [xf for xf in xml_candidates if _frame_mats_exist(xf)]
    skipped = len(xml_candidates) - len(xml_files)
    if skipped:
        print(f'Skipping {skipped} output*.xml without companion .mat (partial sync?)')
    print(f'Reading {len(xml_files)} output frames …')
    domain_vol_um3 = _domain_volume_um3(output_folder)
    reactor_L = domain_vol_um3 / UM3_PER_L
    print(f'Domain volume: {domain_vol_um3:.0f} μm³ ({reactor_L:.3e} L)')
    r_g, r_a, r_o, r_c = _substrate_row_indices(xml_files[0])
    print(
        f'Microenvironment rows (from {os.path.basename(xml_files[0])}): '
        f'glucose={r_g}, acetate={r_a}, oxygen={r_o}, CO2={r_c}'
    )
    rows = [read_frame(xf, domain_vol_um3, r_g, r_a, r_o, r_c) for xf in xml_files]
    if not rows:
        raise SystemExit(
            f'No plot-ready data in {output_folder!r}\n'
            'Expected output*.xml with sibling _cells.mat and '
            '_microenvironment0.mat (and optionally final.xml). '
            'Sync the run or pass --output-folder.'
        )
    final_xml = os.path.join(output_folder, 'final.xml')
    if os.path.isfile(final_xml):
        print('Reading final frame …')
        rows.append(read_final_frame(output_folder, domain_vol_um3, r_g, r_a, r_o, r_c))
    else:
        print('No final.xml in this folder – time series ends at last output*.xml')
    arr = np.array(rows)
    times = arr[:, 0]
    biomass = arr[:, 1]
    glc = arr[:, 2]
    ace = arr[:, 3]
    o2 = arr[:, 4]
    co2 = arr[:, 5]

    if not _o2_has_dynamics(o2):
        ref = _load_reference_o2()
        if ref is not None:
            ref_t, ref_o2 = ref
            print(
                'O₂ domain mean is flat in this output (Dirichlet-pinned static '
                'condition). Using reference O₂ from job34054344 '
                f'({REFERENCE_O2_CSV.name}).'
            )
            o2 = _interpolate_o2(times, ref_t, ref_o2)
        else:
            print(
                'Warning: O₂ shows no depletion/reinjection dynamics in this run '
                f'and {REFERENCE_O2_CSV.name} is missing.'
            )

    return times, biomass, glc, ace, o2, co2


# ─────────────────────────────────────────────────────────────────────────────
def _style_ax(ax, right_spine=False):
    """Apply project-standard spine/tick styling to an axes."""
    ax.set_facecolor('white')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(right_spine)
    for side in ('left', 'bottom'):
        ax.spines[side].set_color('black')
        ax.spines[side].set_linewidth(LW_SPINE)
    if right_spine:
        ax.spines['right'].set_color('black')
        ax.spines['right'].set_linewidth(LW_SPINE)
    ax.tick_params(axis='both', which='major',
                   labelsize=FS_TICK, colors='black',
                   length=4, width=LW_SPINE * 0.7)
    ax.tick_params(axis='both', which='minor', length=0)
    ax.grid(False)


def plot_figure_a(
    times,
    biomass,
    glc,
    ace,
    o2,
    co2,
    results_dir: str,
):
    times = np.asarray(times, dtype=float)
    biomass = np.asarray(biomass, dtype=float)
    glc = np.asarray(glc, dtype=float)
    ace = np.asarray(ace, dtype=float)
    o2 = np.asarray(o2, dtype=float)
    co2 = np.asarray(co2, dtype=float)

    # clip_on=False below lets line strokes render past the axes box edge
    # without being cut mid-stroke; trim data past the display window first
    # so it doesn't also draw the flat tail straight through to the figure edge.
    _view_mask = times <= FIG_X_MAX_H
    times = times[_view_mask]
    biomass = biomass[_view_mask]
    glc = glc[_view_mask]
    ace = ace[_view_mask]
    o2 = o2[_view_mask]
    co2 = co2[_view_mask]

    with plt.rc_context(_CELL_SYSTEMS_RC):
        fig, ax1 = plt.subplots(figsize=(FIG_WIDTH_IN, FIG_HEIGHT_IN))
        ax2 = ax1.twinx()

        t_g, y_g = _insert_nan_at_time_jumps(times, glc)
        t_a, y_a = _insert_nan_at_time_jumps(times, ace)
        t_o, y_o = _insert_nan_at_time_jumps(times, o2)
        t_c, y_c = _insert_nan_at_time_jumps(times, co2)
        t_b, y_b = _insert_nan_at_time_jumps(times, biomass)

        lglc, = ax1.plot(
            t_g, y_g, '-', color=COLOR_GLUCOSE, lw=LW_DATA,
            label='Glucose', zorder=3, clip_on=False,
        )
        lace, = ax1.plot(
            t_a, y_a, '-', color=COLOR_ACETATE, lw=LW_DATA,
            label='Acetate', zorder=3, clip_on=False,
        )
        lo2, = ax1.plot(
            t_o, y_o, '-', color=COLOR_OXYGEN, lw=LW_DATA,
            label='Oxygen', zorder=5, clip_on=False,
        )
        lco2, = ax1.plot(
            t_c, y_c, '-', color=COLOR_CO2, lw=LW_DATA,
            label='CO₂', zorder=4, clip_on=False,
        )
        lbio, = ax2.plot(
            t_b, y_b, '-', color=COLOR_BIOMASS, lw=LW_DATA + 0.25,
            label='Total biomass', zorder=4, clip_on=False,
        )

        ax1.margins(x=0)
        ax2.margins(x=0)
        ax1.set_xlim(0.0, FIG_X_MAX_H)
        ax1.set_xticks(np.arange(0, int(FIG_X_MAX_H) + 1))

        c_pad = max(float(glc.max()), float(ace.max()), float(o2.max()), float(co2.max()), 1e-9) * 1.12
        c_neg = max(0.12, 0.02 * c_pad)
        ax1.set_ylim(-c_neg, c_pad)

        b_max = float(np.nanmax(biomass))
        b_neg = max(0.015 * b_max, 0.004)
        ax2.set_ylim(-b_neg, b_max * 1.12)

        ax1.axhline(
            0.0,
            color='#888888',
            ls=(0, (1.0, 2.2)),
            lw=0.85,
            zorder=2,
            clip_on=False,
        )

        # Phase boundaries for the canonical run (O2 resupply at t3=3.5h):
        # aerobic growth (0-1.2h) -> O2-limited fermentation/acetate
        # secretion (1.2-3.4h) -> O2 resupply at 3.5h (glucose exhaustion and
        # resupply are now nearly coincident, so t2 is dropped as a separate
        # marker rather than sitting right on top of t3) -> aerobic acetate
        # reutilisation (3.5-5.5h) -> final plateau (5.5h onward, view cut at
        # FIG_X_MAX_H since the rest is flat and uninformative). Each
        # boundary is marked with a dashed line at its exact time; the t_i
        # label sits at the horizontal center of the phase that begins
        # there (not on the line itself).
        PHASE_BOUNDARIES = [(1.2, 1), (3.5, 3), (5.5, 4)]  # (time_h, label_index)
        PHASE_EDGES_H = [0.0] + [t for t, _ in PHASE_BOUNDARIES] + [FIG_X_MAX_H]

        for k, (t_phase, i) in enumerate(PHASE_BOUNDARIES):
            ax1.axvline(
                t_phase,
                color='#888888',
                ls=(0, (2.0, 2.0)),
                lw=0.7,
                zorder=1,
                clip_on=False,
            )
            label_x = 0.5 * (PHASE_EDGES_H[k + 1] + PHASE_EDGES_H[k + 2])
            ax1.text(
                label_x, c_pad * 0.97, rf'$t_{i}$',
                ha='center', va='top', fontsize=FS_TICK, color='#555555', zorder=6,
            )

        _style_ax(ax1, right_spine=False)
        _style_ax(ax2, right_spine=True)
        ax2.spines['left'].set_visible(False)
        ax2.spines['bottom'].set_visible(False)
        ax2.spines['top'].set_visible(False)

        ax1.set_xlabel('Time (h)', fontsize=FS_LABEL, color='black', labelpad=4)
        ax1.set_ylabel('Concentration (mM)', fontsize=FS_LABEL, color='black', labelpad=3)
        ax2.set_ylabel('Total biomass (g/L)', fontsize=FS_LABEL, color='black', labelpad=4)

        handles = [lglc, lace, lo2, lco2, lbio]
        labels = ['Glucose', 'Acetate', 'Oxygen', 'CO₂', 'Total biomass']
        # Legend anchored to axes top (not figure top) so tight bbox does not add a large gap.
        leg = fig.legend(
            handles,
            labels,
            loc='lower center',
            bbox_to_anchor=(0.5, 1.0),
            bbox_transform=ax1.transAxes,
            ncol=5,
            fontsize=FS_LEGEND,
            frameon=False,
            handlelength=2.0,
            handletextpad=0.55,
            columnspacing=1.4,
            borderaxespad=0.0,
        )
        for text in leg.get_texts():
            text.set_color('#111111')

        fig.patch.set_facecolor('white')
        fig.subplots_adjust(
            left=0.085,
            right=0.885,
            bottom=0.17,
            top=0.86,
        )

        os.makedirs(results_dir, exist_ok=True)
        for ext in ('png', 'svg'):
            path = os.path.join(results_dir, f'figure_A.{ext}')
            fig.savefig(
                path,
                dpi=300,
                bbox_inches='tight',
                pad_inches=0.02,
                facecolor='white',
            )
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
    ap = argparse.ArgumentParser(description='E. coli acetate-switch Figure A (time series).')
    ap.add_argument(
        '--output-folder',
        default=None,
        metavar='DIR',
        help='PhysiCell output directory (contains output*.xml). '
             'Default: first existing definitive or fallback run (PhysiCelldFBA or mn5sync_pdfba).',
    )
    ap.add_argument(
        '--results-dir',
        default=None,
        metavar='DIR',
        help='Where to write figure_A.png/svg. Default: analysis/results/ecoli, or a subfolder named after the sample when --output-folder differs from the default.',
    )
    args = ap.parse_args()
    out_folder = args.output_folder or OUTPUT_FOLDER
    results_dir = _resolve_results_dir(out_folder, args.results_dir)
    os.makedirs(results_dir, exist_ok=True)

    times, biomass, glc, ace, o2, co2 = collect_time_series(out_folder)
    print(f'\nTime range (all points, incl. final if present): '
          f'{times[0]:.2f} – {times[-1]:.2f} h  ({len(times)} points)')
    print(f'Figure x-axis: 0 – {FIG_X_MAX_H:g} h')
    print(f'Glucose:   {glc[0]:.2f} → {glc[-1]:.4f} mM')
    print(f'Acetate:   {ace[0]:.2f} → {ace[-1]:.3f} mM')
    print(f'Oxygen:    {o2[0]:.3f} → {o2[-1]:.3f} mM')
    print(f'CO₂:       {co2[0]:.3f} → {co2[-1]:.3f} mM')
    print(f'Biomass:   {biomass[0]:.3f} → {biomass[-1]:.3f} g/L')
    plot_figure_a(times, biomass, glc, ace, o2, co2, results_dir)


if __name__ == '__main__':
    main()
