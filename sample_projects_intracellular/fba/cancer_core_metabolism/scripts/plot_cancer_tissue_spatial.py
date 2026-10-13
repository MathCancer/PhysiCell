#!/usr/bin/env python3
"""
Uptake / growth figures for the cancer-tissue column.

Produces the same metabolite maps as the classic ``plot_cells_svg`` block in
``cancer_tissue_figures_v2.ipynb`` (O2, glucose, glutamine, glycine, lactate),
plus growth rate, using ``plot_cells_spatial`` (blob cells, vessel bar, zones).

Outputs are named ``{stem}_{t}h.svg`` (and ``uptake_stack_{t}h.png`` /
``substrate_profiles_{t}h`` / ``net_export_rates_grid_{t}h`` /
``substrate_concentration_grid_{t}h`` with the corresponding flags).

Examples
--------
    python scripts/plot_cancer_tissue_spatial.py \\
        --output-dir ./output \\
        --time-point 72

    python scripts/plot_cancer_tissue_spatial.py \\
        --output-dir ./output \\
        --time-point 72 --uptake-stack

    python scripts/plot_cancer_tissue_spatial.py \\
        --time-point 72 --substrate-profiles

    python scripts/plot_cancer_tissue_spatial.py \\
        --time-point 72 --net-export-rates --concentration-grid

    python scripts/figure_generation.py --time-point 72 --uptake-stack --substrate-profiles
"""

from __future__ import annotations

import argparse
import glob
import os
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Callable, Optional

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.colors import LinearSegmentedColormap
from PIL import Image

# Local imports (pctk + plotter)
import figure_paths  # noqa: E402

figure_paths.ensure_paths()

from pctk import multicellds  # noqa: E402
from plot_cells_spatial import plot_cells_spatial  # noqa: E402

# Default rows for --uptake-stack (same as GIF script)
_DEFAULT_STACK_STEMS = (
    "oxygen_uptake",
    "glucose_uptake",
    "glutamine_uptake",
    "glycine_uptake",
    "lactate_secretion",
)
# ---------------------------------------------------------------------------
# Cell loading (same helpers as cancer_tissue_figures_v2.ipynb)
# ---------------------------------------------------------------------------

_DROP_COLS = [
    "cytoplasmic_biomass_change_rate",
    "nuclear_biomass_change_rate",
    "fluid_change_rate",
    "calcification_rate",
    "target_solid_cytoplasmic",
    "target_solid_nuclear",
    "target_fluid_fraction",
    "radius",
    "surface_area",
    "cell_cell_adhesion_strength",
    "cell_BM_adhesion_strength",
    "cell_cell_repulsion_strength",
    "cell_BM_repulsion_strength",
    "cell_adhesion_affinities",
    "relative_maximum_adhesion_distance",
    "maximum_number_of_attachments",
    "attachment_elastic_constant",
    "attachment_rate",
    "detachment_rate",
    "is_motile",
    "persistence_time",
    "migration_speed",
]


def _get_simplified_labels_node(tree):
    root = tree.getroot()
    cellular_info = root.find("cellular_information")
    if cellular_info is None:
        raise ValueError("cellular_information node not found in MultiCellDS XML")

    def _walk(node):
        for child in node:
            if child.tag == "simplified_data" and child.attrib.get("source") == "PhysiCell":
                return child.find("labels")
            labels = _walk(child)
            if labels is not None:
                return labels
        return None

    labels = _walk(cellular_info)
    if labels is None:
        raise ValueError("PhysiCell simplified_data labels not found in MultiCellDS XML")
    return labels


def cell_column_names_from_tree(tree, sep="_"):
    labels = _get_simplified_labels_node(tree)
    n_cols = max(
        int(label.attrib["index"]) + int(label.attrib["size"]) for label in labels
    )
    columns = [None] * n_cols
    for label in labels:
        name = label.text
        idx = int(label.attrib["index"])
        size = int(label.attrib["size"])
        if size == 1:
            columns[idx] = name
        elif size == 2:
            for i, axis in enumerate(["x", "y"]):
                columns[idx + i] = f"{axis}{sep}{name}"
        elif size == 3:
            for i, axis in enumerate(["x", "y", "z"]):
                columns[idx + i] = f"{axis}{sep}{name}"
        else:
            for i in range(size):
                columns[idx + i] = f"{name}_{i}"
    for i, col in enumerate(columns):
        if col is None:
            columns[i] = f"col_{i}"
    return columns


def read_cells(reader, drop_cols=None, time_interval=60, max_time=-1):
    if drop_cols is None:
        drop_cols = _DROP_COLS
    df_list = []
    col_names = cell_column_names_from_tree(reader._tree)
    xml_list = sorted(glob.glob(os.path.join(reader._output_folder, "output*.xml")))
    for xml_fname in xml_list:
        tree = ET.parse(xml_fname)
        cell_matrix = reader.get_cells_matrix(tree)
        df = pd.DataFrame(cell_matrix, columns=col_names)
        if "ID" in df.columns:
            df = df.set_index("ID")
        t = reader.get_time(tree)
        df = df.assign(time=float(t))
        if drop_cols:
            df = df.drop(
                columns=[c for c in drop_cols if c in df.columns], errors="ignore"
            )
        df_list.append(df)
        if max_time > 0 and t >= max_time:
            break
    df_cells = pd.concat(df_list)
    df_cells.loc[:, "time"] /= time_interval
    return df_cells


# ---------------------------------------------------------------------------
# Flux → fmol/min (matches notebook cell 8)
# ---------------------------------------------------------------------------

_RHO = 1.04  # g/mL
_SOLID = 0.75
_MIN_PER_H = 60.0


def _vol_flux(row, col: str, sign: float = -1.0) -> float:
    return sign * float(getattr(row, col)) * float(row.total_volume) * _RHO * _SOLID / _MIN_PER_H


def _o2_uptake(row) -> float:
    # Notebook omits total_volume for O2; keep that convention.
    return -float(row.R_EX_o2_e) * _RHO * _SOLID / _MIN_PER_H


def _figure_specs() -> list[dict]:
    glc_cmap = LinearSegmentedColormap.from_list("yellow", ["#ffffff", "#f1c232ff"])
    gly_cmap = LinearSegmentedColormap.from_list("glycine", ["#ffffff", "hotpink"])
    lac_cmap = LinearSegmentedColormap.from_list("firebrick", ["#ffffff", "firebrick"])
    return [
        {
            "stem": "growth_rate",
            "required": ["growth_rate"],
            "color_config": {
                "value_fn": lambda x: x.growth_rate,
                "cmap": plt.cm.Greens,
                "label": r"Growth rate",
            },
            "vmax": 0.026803,  # phenotype max_growth_rate ceiling (1/min)
            "vmin": 0.0,
        },
        {
            "stem": "oxygen_uptake",
            "required": ["R_EX_o2_e"],
            "color_config": {
                "value_fn": _o2_uptake,
                "cmap": plt.cm.Blues,
                "label": r"$O_2$ uptake (fmol/min)",
            },
            "vmax": None,
        },
        {
            "stem": "glucose_uptake",
            "required": ["R_EX_glc_e", "total_volume"],
            "color_config": {
                "value_fn": lambda x: _vol_flux(x, "R_EX_glc_e", -1.0),
                "cmap": glc_cmap,
                "label": r"Glucose uptake (fmol/min)",
            },
            "vmax": None,
        },
        {
            "stem": "glutamine_uptake",
            "required": ["R_EX_gln_L_e", "total_volume"],
            "color_config": {
                "value_fn": lambda x: _vol_flux(x, "R_EX_gln_L_e", -1.0),
                "cmap": plt.cm.Purples,
                "label": r"Gln-L uptake (fmol/min)",
            },
            "vmax": None,
        },
        {
            "stem": "glycine_uptake",
            "required": ["R_EX_gly_e", "total_volume"],
            "color_config": {
                "value_fn": lambda x: _vol_flux(x, "R_EX_gly_e", -1.0),
                "cmap": gly_cmap,
                "label": r"Glycine uptake (fmol/min)",
            },
            "vmax": None,
        },
        {
            "stem": "lactate_secretion",
            "required": ["R_EX_lac_L_e", "total_volume"],
            "color_config": {
                "value_fn": lambda x: _vol_flux(x, "R_EX_lac_L_e", +1.0),
                "cmap": lac_cmap,
                "label": r"Lactate secretion (fmol/min)",
            },
            "vmax": None,
        },
    ]


_DEFAULT_STYLE = dict(
    X_MIN=0.0,
    X_MAX=320.0,
    Y_MIN=-55.0,
    Y_MAX=55.0,
    skip_dead=False,
    show_vessel=True,
    vessel_style="bar",
    vessel_label=True,
    show_zones=True,
    # Target tumour-cord phenotype bands (not fit to a single run)
    zone_x=(120.0, 200.0),
    zone_labels=True,
    zone_bands=False,
    show_nuclei=True,
    nucleus_scale=1.0,
    cell_shape="blob",
    roughness=0.05,
    shape_seed=0,
    perspective=0.1,
    depth_fade=0.05,
    show_x_ruler=True,
    show_scale_bar=False,
    x_major=50.0,
    x_minor=10.0,
    xlabel="Distance from vessel (µm)",
    fontsize=20,
    annotation_fontsize=25,
    figsize=(16, 5.6),
)


def _parse_only(s: Optional[str]) -> Optional[set[str]]:
    if s is None:
        return None
    return {p.strip() for p in s.split(",") if p.strip()}


def main(argv: Optional[list[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description="Cancer-tissue uptake / growth figures."
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=None,
        help="PhysiCell output folder (default: ./output if present).",
    )
    parser.add_argument("--time-point", type=float, default=48.0, help="Time in hours.")
    parser.add_argument(
        "--fig-dir",
        type=Path,
        default=None,
        help="Figure output directory (default: <output-dir>/fig).",
    )
    parser.add_argument(
        "--format",
        dest="fig_format",
        default="svg",
        choices=("svg", "png", "pdf"),
    )
    parser.add_argument(
        "--only",
        default=None,
        help=(
            "Comma-separated stems to plot, e.g. "
            "glucose_uptake,glutamine_uptake,glycine_uptake. "
            "Use '' with --uptake-stack to skip individual panels."
        ),
    )
    parser.add_argument(
        "--uptake-stack",
        action="store_true",
        help=(
            "Also write a vertical stack of spatial uptake maps at --time-point "
            "(uptake_stack_{t}h.png)."
        ),
    )
    parser.add_argument(
        "--stack-stems",
        default=None,
        help=(
            "Comma-separated stems for --uptake-stack "
            "(default: oxygen,glucose,glutamine,glycine,lactate)."
        ),
    )
    parser.add_argument(
        "--no-colorbar",
        action="store_true",
        help="Hide colorbars on uptake-stack panels.",
    )
    parser.add_argument(
        "--substrate-profiles",
        action="store_true",
        help=(
            "Write stacked microenvironment concentration vs distance "
            "(substrate_profiles_{t}h.{format})."
        ),
    )
    parser.add_argument(
        "--profile-substrates",
        default=None,
        help=(
            "Comma-separated microenvironment names for --substrate-profiles "
            "(default: O2,D-Glucose,L-Glutamine,Glycine,L-Lactate)."
        ),
    )
    parser.add_argument(
        "--net-export-rates",
        action="store_true",
        help=(
            "Write flux-only distance grid (net_export_rates_grid_{t}h.{format})."
        ),
    )
    parser.add_argument(
        "--concentration-grid",
        action="store_true",
        help=(
            "Write concentration-only distance grid "
            "(substrate_concentration_grid_{t}h.{format})."
        ),
    )
    parser.add_argument(
        "--time-interval",
        type=float,
        default=60.0,
        help="PhysiCell time unit conversion to hours (minutes per hour).",
    )
    args = parser.parse_args(argv)

    output_dir = (args.output_dir or figure_paths.default_output_dir()).resolve()
    if not output_dir.is_dir():
        raise SystemExit(f"Output directory not found: {output_dir}")

    fig_dir = (args.fig_dir or (output_dir / "fig")).resolve()
    fig_dir.mkdir(parents=True, exist_ok=True)

    only = _parse_only(args.only)
    all_specs = {s["stem"]: s for s in _figure_specs()}
    if only is None:
        # Bare stack/profile/grid flags → skip individual spatial panels.
        specs = (
            []
            if (
                args.uptake_stack
                or args.substrate_profiles
                or args.net_export_rates
                or args.concentration_grid
            )
            else list(_figure_specs())
        )
    elif not only:
        specs = []
    else:
        unknown = only - set(all_specs)
        if unknown:
            raise SystemExit(f"Unknown --only stems: {sorted(unknown)}")
        specs = [s for s in _figure_specs() if s["stem"] in only]

    need_cells = bool(specs) or args.uptake_stack or args.net_export_rates
    need_micro = args.substrate_profiles or args.concentration_grid
    if not need_cells and not need_micro:
        raise SystemExit(
            "Nothing to plot: set --only, --uptake-stack, --substrate-profiles, "
            "--net-export-rates, and/or --concentration-grid"
        )

    reader = multicellds.MultiCellDS(output_folder=str(output_dir))
    time_point = float(args.time_point)
    df_cells = None

    if need_cells:
        print(f"- Reading cells from {output_dir}")
        df_cells = read_cells(
            reader, drop_cols=_DROP_COLS, max_time=-1, time_interval=args.time_interval
        )
        available = sorted(df_cells["time"].unique())
        if time_point not in available:
            time_point = float(available[-1])
            print(
                f"- Requested time not in output; using last frame: "
                f"{time_point:.0f} h ({available[0]:.0f}–{available[-1]:.0f} h)"
            )
        else:
            print(f"- Using time_point={time_point:.0f} h")

    for spec in specs:
        missing = [c for c in spec["required"] if c not in df_cells.columns]
        if missing:
            print(f"- Skipping {spec['stem']}: missing columns {missing}")
            continue

        print(f"- Plotting {spec['stem']} …")
        kwargs = dict(_DEFAULT_STYLE)
        if spec.get("vmax") is not None:
            kwargs["vmax"] = spec["vmax"]
        if "vmin" in spec and spec["vmin"] is not None:
            kwargs["vmin"] = spec["vmin"]
        if spec.get("power_gamma") is not None:
            kwargs["power_gamma"] = spec["power_gamma"]

        fig = plot_cells_spatial(
            df_cells,
            time_point=time_point,
            color_config=spec["color_config"],
            **kwargs,
        )
        out = fig_dir / f"{spec['stem']}_{time_point}h.{args.fig_format}"
        fig.savefig(out, dpi=300, bbox_inches="tight", pad_inches=0.05)
        plt.close(fig)
        print(f"  wrote {out}")

    if args.uptake_stack:
        # Lazy import avoids circular init at module load (gif imports this file).
        from plot_cancer_tissue_spatial_gif import _render_uptake_stack

        if args.stack_stems:
            stems = [s.strip() for s in args.stack_stems.split(",") if s.strip()]
        else:
            stems = list(_DEFAULT_STACK_STEMS)
        stack_specs = []
        for stem in stems:
            if stem not in all_specs:
                raise SystemExit(f"Unknown stack stem: {stem}")
            spec = all_specs[stem]
            missing = [c for c in spec["required"] if c not in df_cells.columns]
            if missing:
                print(f"- Skipping stack row {stem}: missing {missing}")
                continue
            stack_specs.append(spec)
        if not stack_specs:
            raise SystemExit("No valid stems for --uptake-stack")

        print(
            f"- Rendering uptake stack at t = {time_point:.0f} h "
            f"({len(stack_specs)} rows) …"
        )
        frames = _render_uptake_stack(
            df_cells,
            [time_point],
            stack_specs,
            _DEFAULT_STYLE,
            show_colorbar=not args.no_colorbar,
        )
        stem = "uptake_stack_no_cbar" if args.no_colorbar else "uptake_stack"
        out = fig_dir / f"{stem}_{time_point:.0f}h.png"
        Image.fromarray(frames[0]).save(out)
        print(f"  wrote {out}")

    if args.substrate_profiles:
        from plot_cancer_tissue_spatial_gif import (
            _DEFAULT_SUBSTRATE_PROFILES,
            _load_substrate_profiles,
            plot_substrate_profiles,
        )

        if args.profile_substrates:
            substrates = [
                s.strip() for s in args.profile_substrates.split(",") if s.strip()
            ]
        else:
            substrates = list(_DEFAULT_SUBSTRATE_PROFILES)

        print(f"- Loading microenvironment profiles from {output_dir}")
        times, profiles, ylims = _load_substrate_profiles(
            reader, substrates, time_interval=args.time_interval
        )
        if not times:
            raise SystemExit("No microenvironment frames found")
        t_prof = time_point
        if t_prof not in times:
            nearest = float(min(times, key=lambda u: abs(u - t_prof)))
            print(
                f"- Requested profile time {t_prof:g} h not found; "
                f"using {nearest:g} h"
            )
            t_prof = nearest
        else:
            print(f"- Using profile time_point={t_prof:.0f} h")

        print(f"- Rendering substrate profiles at t = {t_prof:.0f} h …")
        fig = plot_substrate_profiles(profiles, substrates, t_prof, ylims)
        out = fig_dir / f"substrate_profiles_{t_prof:.0f}h.{args.fig_format}"
        fig.savefig(out, dpi=300, bbox_inches="tight", pad_inches=0.05)
        plt.close(fig)
        print(f"  wrote {out}")

    if args.net_export_rates or args.concentration_grid:
        from plot_exchange_profiles import (
            plot_exchange_profile_grid,
            read_microenvironment_step,
        )

        df_micro = None
        if args.concentration_grid:
            print(f"- Reading microenvironment at t = {time_point:.0f} h")
            df_micro = read_microenvironment_step(
                reader,
                time_step=time_point,
                z_slice=None,
                time_interval=args.time_interval,
            )
            # If exact time missing, iterator returns last; attrs may differ
            t_micro = df_micro.attrs.get("time", time_point)
            if abs(float(t_micro) - float(time_point)) > 1e-6:
                print(
                    f"- Microenvironment frame snapped to t = {float(t_micro):.0f} h"
                )

        if args.net_export_rates:
            print(f"- Rendering net export rates grid at t = {time_point:.0f} h …")
            fig = plot_exchange_profile_grid(
                mode="flux",
                time_point=time_point,
                df_cells=df_cells,
            )
            out = fig_dir / f"net_export_rates_grid_{time_point}h.{args.fig_format}"
            fig.savefig(out, dpi=300, bbox_inches="tight", pad_inches=0.1)
            plt.close(fig)
            print(f"  wrote {out}")

        if args.concentration_grid:
            print(
                f"- Rendering substrate concentration grid at t = {time_point:.0f} h …"
            )
            fig = plot_exchange_profile_grid(
                mode="concentration",
                time_point=time_point,
                df_micro=df_micro,
            )
            out = (
                fig_dir
                / f"substrate_concentration_grid_{time_point}h.{args.fig_format}"
            )
            fig.savefig(out, dpi=300, bbox_inches="tight", pad_inches=0.1)
            plt.close(fig)
            print(f"  wrote {out}")

    if not any(
        [
            specs,
            args.uptake_stack,
            args.substrate_profiles,
            args.net_export_rates,
            args.concentration_grid,
        ]
    ):
        raise SystemExit(
            "Nothing to plot: set --only, --uptake-stack, --substrate-profiles, "
            "--net-export-rates, and/or --concentration-grid"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
