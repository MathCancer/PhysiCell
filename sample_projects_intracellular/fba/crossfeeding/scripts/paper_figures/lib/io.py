"""MultiCellDS I/O helpers for paper figure scripts.

Requires pctk: pip install -e /path/to/pctk

On the MN5 cluster pctk isn't pip-installed -- it's rsynced to
<pDFBA>/python_libs/pctk and made importable via sys.path, the same
convention used by analysis/_pctk_path.py elsewhere in this repo (no pip
needed on the login/compute nodes). The path below is a no-op wherever pctk
is already pip-installed (e.g. locally).
"""

from __future__ import annotations

import glob
import os
import sys
import xml.etree.ElementTree as ET

_HERE = os.path.dirname(os.path.abspath(__file__))
# lib/ -> paper_figures/ -> scripts/ -> crossfeeding/ -> fba/ ->
# sample_projects_intracellular/ -> <PhysiCelldFBA repo root> -> pDFBA/
_python_libs = os.path.abspath(os.path.join(_HERE, *(['..'] * 7), 'python_libs'))
if os.path.isdir(_python_libs) and _python_libs not in sys.path:
    sys.path.insert(0, _python_libs)


def require_pctk():
    try:
        from pctk import multicellds  # noqa: F401
    except ImportError as exc:
        raise ImportError(
            'pctk is required to read PhysiCell MultiCellDS output. '
            'Install with: pip install -e /path/to/pctk'
        ) from exc


def validate_output_dir(output_dir: str) -> None:
    if not os.path.isdir(output_dir):
        raise FileNotFoundError(f'Output directory not found: {output_dir}')
    if not os.path.isfile(os.path.join(output_dir, 'initial.xml')):
        raise FileNotFoundError(
            f'Not a MultiCellDS folder (missing initial.xml): {output_dir}'
        )
    mats = glob.glob(os.path.join(output_dir, 'output*_microenvironment0.mat'))
    if not mats:
        raise FileNotFoundError(
            f'No microenvironment snapshots found in: {output_dir}'
        )


def open_reader(output_dir: str):
    require_pctk()
    from pctk import multicellds
    validate_output_dir(output_dir)
    return multicellds.MultiCellDS(output_folder=output_dir)


def cell_type_name(raw) -> str:
    s = str(raw)
    if s in ('0', '0.0'):
        return 'C. beijerinckii'
    if s in ('1', '1.0'):
        return 'M. barkeri'
    return f'Cell type {s}'


def get_domain_extent(output_dir: str):
    """Read 2D domain [x_min, x_max, y_min, y_max] from first output XML."""
    try:
        xml_files = sorted(glob.glob(os.path.join(output_dir, 'output*.xml')))
        if not xml_files:
            return -120.0, 120.0, -120.0, 120.0
        tree = ET.parse(xml_files[0])
        bbox = tree.find('.//bounding_box')
        if bbox is None or not bbox.text:
            return -120.0, 120.0, -120.0, 120.0
        parts = bbox.text.strip().split()
        if len(parts) >= 6:
            x_min, y_min = float(parts[0]), float(parts[1])
            x_max, y_max = float(parts[3]), float(parts[4])
            return x_min, x_max, y_min, y_max
    except Exception:
        pass
    return -120.0, 120.0, -120.0, 120.0


def get_xml_cell_indices(output_dir: str) -> dict:
    indices = {}
    try:
        xml_files = sorted(glob.glob(os.path.join(output_dir, 'output*.xml')))
        if not xml_files:
            return indices
        tree = ET.parse(xml_files[0])
        labels = tree.find('.//labels')
        if labels is not None:
            for label in labels:
                if label.text and label.attrib.get('index') is not None:
                    indices[label.text] = int(label.attrib['index'])
    except Exception:
        pass
    return indices


def get_xml_column_index(output_dir: str, column_name: str) -> int | None:
    old_to_new = {
        'hydrogen_flux': ['CB_h2_flux', 'hydrogen_flux'],
        'h2_flux': ['MB_h2_flux', 'h2_flux'],
        'acetate_flux': ['MB_acetate_flux', 'CB_acetate_flux', 'acetate_flux'],
        'CB_co2_flux': ['CB_co2_flux', 'co2_flux'],
        'MB_co2_flux': ['MB_co2_flux', 'co2_flux'],
    }
    search_names = list(old_to_new.get(column_name, [column_name]))
    if column_name in ('methane_flux', 'ch4_flux'):
        search_names.extend(['ch4_flux', 'methane_flux'])
    search_names = list(dict.fromkeys(search_names))
    try:
        xml_files = sorted(glob.glob(os.path.join(output_dir, 'output*.xml')))
        if not xml_files:
            return None
        tree = ET.parse(xml_files[0])
        labels = tree.find('.//labels')
        if labels is None:
            return None
        for search_name in search_names:
            for label in labels:
                if label.text == search_name:
                    idx = int(label.attrib.get('index', -1))
                    if idx >= 0:
                        return idx
    except Exception:
        pass
    return None


def ensure_dir(path: str) -> str:
    os.makedirs(path, exist_ok=True)
    return path


def results_paths(results_dir: str, max_hours: int) -> dict:
    h = int(max_hours)
    return {
        'biomass': ensure_dir(os.path.join(results_dir, 'biomass')),
        'fluxes': ensure_dir(os.path.join(results_dir, 'fluxes')),
        'time_series': ensure_dir(os.path.join(results_dir, 'time_series', f'0_{h}h')),
        'gradient_fields': ensure_dir(os.path.join(results_dir, 'gradient_fields', f'0_{h}h')),
    }
