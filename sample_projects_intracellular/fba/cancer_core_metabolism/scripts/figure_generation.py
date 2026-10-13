#!/usr/bin/env python3
"""
Cancer cord figure generation (static panels, uptake stack, profiles, GIFs).

Thin CLI around the plot modules in this ``scripts/`` folder. Typical use from
the sample project or repo root:

    # Growth + metabolite maps + uptake stack + profiles at 72 h
    python scripts/figure_generation.py \\
        --output-dir ./output \\
        --time-point 72 \\
        --panels \\
        --uptake-stack \\
        --substrate-profiles

    # Only the stacked uptake PNG
    python scripts/figure_generation.py --time-point 72 --uptake-stack

    # Static substrate profiles
    python scripts/figure_generation.py --time-point 72 --substrate-profiles

    # Hourly GIF of the uptake stack
    python scripts/figure_generation.py --gif-uptake-stack --duration-ms 250

Default ``--output-dir`` is ``./output`` (or the first existing candidate from
``figure_paths.default_output_dir``). Pass an explicit path for other runs.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import figure_paths

figure_paths.ensure_paths()


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Generate cancer-tissue figures, stacks, profiles, and GIFs."
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=None,
        help="PhysiCell output folder (default: ./output if present).",
    )
    parser.add_argument("--fig-dir", type=Path, default=None)
    parser.add_argument("--time-point", type=float, default=72.0)
    parser.add_argument("--time-interval", type=float, default=60.0)
    parser.add_argument(
        "--format",
        dest="fig_format",
        default="svg",
        choices=("svg", "png", "pdf"),
    )
    parser.add_argument(
        "--panels",
        action="store_true",
        help="Write individual spatial panels (growth + uptakes) at --time-point.",
    )
    parser.add_argument(
        "--only",
        default=None,
        help="Comma-separated panel stems (default: all when --panels).",
    )
    parser.add_argument(
        "--uptake-stack",
        action="store_true",
        help="Write uptake_stack_{t}h.png at --time-point.",
    )
    parser.add_argument(
        "--stack-stems",
        default=None,
        help="Comma-separated stems for the uptake stack.",
    )
    parser.add_argument(
        "--no-colorbar",
        action="store_true",
        help="Hide colorbars on uptake-stack panels (static PNG and GIF).",
    )
    parser.add_argument(
        "--substrate-profiles",
        action="store_true",
        help="Write substrate_profiles_{t}h.{format} at --time-point.",
    )
    parser.add_argument(
        "--profile-substrates",
        default=None,
        help="Comma-separated microenvironment names for substrate profiles.",
    )
    parser.add_argument(
        "--net-export-rates",
        action="store_true",
        help="Write net_export_rates_grid_{t}h.{format} (flux-only).",
    )
    parser.add_argument(
        "--concentration-grid",
        action="store_true",
        help="Write substrate_concentration_grid_{t}h.{format} (concentration-only).",
    )
    parser.add_argument(
        "--gif-uptake-stack",
        action="store_true",
        help="Write uptake_stack.gif over the full time series.",
    )
    parser.add_argument(
        "--gif-growth",
        action="store_true",
        help="Write growth_rate.gif.",
    )
    parser.add_argument(
        "--gif-substrate-profiles",
        action="store_true",
        help="Write substrate_profiles.gif.",
    )
    parser.add_argument("--t-min", type=float, default=None)
    parser.add_argument("--t-max", type=float, default=None)
    parser.add_argument("--stride", type=int, default=1)
    parser.add_argument("--duration-ms", type=int, default=250)
    parser.add_argument("--loop", type=int, default=0)
    args = parser.parse_args(argv)

    if not any(
        [
            args.panels,
            args.uptake_stack,
            args.substrate_profiles,
            args.net_export_rates,
            args.concentration_grid,
            args.gif_uptake_stack,
            args.gif_growth,
            args.gif_substrate_profiles,
        ]
    ):
        parser.error(
            "Select at least one of: --panels, --uptake-stack, --substrate-profiles, "
            "--net-export-rates, --concentration-grid, "
            "--gif-uptake-stack, --gif-growth, --gif-substrate-profiles"
        )

    output_dir = args.output_dir or figure_paths.default_output_dir()
    fig_dir = args.fig_dir

    if (
        args.panels
        or args.uptake_stack
        or args.substrate_profiles
        or args.net_export_rates
        or args.concentration_grid
    ):
        from plot_cancer_tissue_spatial import main as static_main

        static_argv = [
            "--output-dir",
            str(output_dir),
            "--time-point",
            str(args.time_point),
            "--time-interval",
            str(args.time_interval),
            "--format",
            args.fig_format,
        ]
        if fig_dir is not None:
            static_argv += ["--fig-dir", str(fig_dir)]
        if args.panels:
            if args.only is not None:
                static_argv += ["--only", args.only]
            else:
                # Named stems so other flags do not suppress individual panels
                from plot_cancer_tissue_spatial import _figure_specs

                stems = ",".join(s["stem"] for s in _figure_specs())
                static_argv += ["--only", stems]
        else:
            static_argv += ["--only", ""]
        if args.uptake_stack:
            static_argv.append("--uptake-stack")
            if args.stack_stems:
                static_argv += ["--stack-stems", args.stack_stems]
            if args.no_colorbar:
                static_argv.append("--no-colorbar")
        if args.substrate_profiles:
            static_argv.append("--substrate-profiles")
            if args.profile_substrates:
                static_argv += ["--profile-substrates", args.profile_substrates]
        if args.net_export_rates:
            static_argv.append("--net-export-rates")
        if args.concentration_grid:
            static_argv.append("--concentration-grid")
        rc = static_main(static_argv)
        if rc:
            return rc

    if args.gif_uptake_stack or args.gif_growth or args.gif_substrate_profiles:
        from plot_cancer_tissue_spatial_gif import main as gif_main

        gif_argv = [
            "--output-dir",
            str(output_dir),
            "--time-interval",
            str(args.time_interval),
            "--duration-ms",
            str(args.duration_ms),
            "--loop",
            str(args.loop),
            "--stride",
            str(args.stride),
        ]
        if fig_dir is not None:
            gif_argv += ["--fig-dir", str(fig_dir)]
        if args.t_min is not None:
            gif_argv += ["--t-min", str(args.t_min)]
        if args.t_max is not None:
            gif_argv += ["--t-max", str(args.t_max)]

        if args.gif_growth:
            gif_argv += ["--only", "growth_rate"]
        else:
            gif_argv += ["--only", ""]
        if args.gif_uptake_stack:
            gif_argv.append("--uptake-stack")
            if args.stack_stems:
                gif_argv += ["--stack-stems", args.stack_stems]
            if args.no_colorbar:
                gif_argv.append("--no-colorbar")
        if args.gif_substrate_profiles:
            gif_argv.append("--substrate-profiles")
            if args.profile_substrates:
                gif_argv += ["--profile-substrates", args.profile_substrates]

        rc = gif_main(gif_argv)
        if rc:
            return rc

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
