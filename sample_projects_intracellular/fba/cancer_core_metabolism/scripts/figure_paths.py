"""Path setup for cancer-tissue figure scripts (pctk + local plot modules).

All defaults are relative to the current working directory or to this
``scripts/`` folder. Pass ``--output-dir`` explicitly when your PhysiCell
output is not ``./output``.
"""
from __future__ import annotations

import os
import sys
from pathlib import Path

_SCRIPTS = Path(__file__).resolve().parent
_PROJECT = _SCRIPTS.parent  # cancer_core_metabolism/


def _as_cwd_relative(path: Path) -> Path:
    """Return ``path`` expressed relative to the current working directory."""
    return Path(os.path.relpath(path.resolve(), Path.cwd().resolve()))


def find_scripts_dir(start: Path | None = None) -> Path:
    """Locate this scripts/ folder from cwd (or ``start``) via relative candidates."""
    start = Path(start or Path.cwd())
    rel_candidates = [
        Path("."),
        Path("scripts"),
        # From a PhysiCell / mn5sync checkout root:
        Path("sample_projects_intracellular/fba/cancer_core_metabolism/scripts"),
        Path("mn5sync_pdfba/sample_projects_intracellular/fba/cancer_core_metabolism/scripts"),
        Path("../sample_projects_intracellular/fba/cancer_core_metabolism/scripts"),
        Path("../mn5sync_pdfba/sample_projects_intracellular/fba/cancer_core_metabolism/scripts"),
        Path("../../mn5sync_pdfba/sample_projects_intracellular/fba/cancer_core_metabolism/scripts"),
    ]
    for rel in rel_candidates:
        cand = (start / rel).resolve()
        if (cand / "figure_paths.py").is_file():
            return _as_cwd_relative(cand)

    # Walk parents for a cancer_core_metabolism/scripts marker
    for parent in [start.resolve(), *start.resolve().parents]:
        if parent.name == "cancer_core_metabolism" and (
            parent / "scripts" / "figure_paths.py"
        ).is_file():
            return _as_cwd_relative(parent / "scripts")
        cand = (
            parent
            / "sample_projects_intracellular"
            / "fba"
            / "cancer_core_metabolism"
            / "scripts"
        )
        if (cand / "figure_paths.py").is_file():
            return _as_cwd_relative(cand)
        cand = (
            parent
            / "mn5sync_pdfba"
            / "sample_projects_intracellular"
            / "fba"
            / "cancer_core_metabolism"
            / "scripts"
        )
        if (cand / "figure_paths.py").is_file():
            return _as_cwd_relative(cand)

    return _as_cwd_relative(_SCRIPTS)


def ensure_paths(scripts_dir: Path | None = None) -> Path:
    """Insert scripts/ and python_libs on sys.path. Return scripts dir."""
    scripts_path = Path(scripts_dir) if scripts_dir is not None else find_scripts_dir()
    scripts_abs = str(scripts_path.resolve())
    if scripts_abs not in sys.path:
        sys.path.insert(0, scripts_abs)

    # python_libs (pctk): cwd-relative first, then walk up from this scripts/
    lib_candidates = [
        Path("python_libs"),
        Path("../python_libs"),
        Path("../../python_libs"),
        Path("mn5sync_pdfba/python_libs"),
        Path("../mn5sync_pdfba/python_libs"),
        Path("../../mn5sync_pdfba/python_libs"),
    ]
    for lib in lib_candidates:
        lib_abs = (Path.cwd() / lib).resolve()
        if lib_abs.is_dir():
            p = str(lib_abs)
            if p not in sys.path:
                sys.path.insert(0, p)
            return scripts_path

    for parent in _SCRIPTS.parents:
        lib_abs = parent / "python_libs"
        if lib_abs.is_dir():
            p = str(lib_abs)
            if p not in sys.path:
                sys.path.insert(0, p)
            break
    return scripts_path


def default_output_dir() -> Path:
    """Best-effort PhysiCell output folder (relative to cwd when possible).

    Prefers the standard sample-project ``output/`` directory. Override with
    ``--output-dir`` / notebook ``OUTPUT_DIR`` for custom run folders.
    """
    candidates = [
        Path("output"),
        Path("../output"),
        _PROJECT / "output",
        # Optional alternate names some local workflows use:
        Path("output_lit"),
        Path("../output_lit"),
    ]
    for c in candidates:
        probe = c if c.is_absolute() else (Path.cwd() / c)
        if probe.is_dir():
            return _as_cwd_relative(probe)
    # Fallback hint for callers — may not exist yet
    return Path("output")
