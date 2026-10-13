#!/usr/bin/env python3
"""Regenerate crossfeeding paper figure panels from PhysiCell MultiCellDS output."""

from __future__ import annotations

import argparse
import os
import sys

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
if _SCRIPT_DIR not in sys.path:
    sys.path.insert(0, _SCRIPT_DIR)

import config
from lib.io import open_reader, results_paths, validate_output_dir
from plots.biomass import plot_biomass_combined
from plots.fluxes import plot_fluxes_combined
from plots.spatial_composite import plot_spatial_composite
from plots.substrate_timeseries import plot_substrate_time_series_reduced


def main():
    parser = argparse.ArgumentParser(
        description='Regenerate crossfeeding paper figure panels (0–50 h).',
    )
    parser.add_argument(
        '--output-dir', required=True,
        help='Path to PhysiCell MultiCellDS output (e.g. output_exp4/)',
    )
    parser.add_argument(
        '--results-dir',
        default=os.path.join(_SCRIPT_DIR, 'figures'),
        help='Directory for generated figures (default: ./figures)',
    )
    parser.add_argument(
        '--max-hours', type=int, default=config.MAX_HOURS_DEFAULT,
        help=f'Time window in hours (default: {config.MAX_HOURS_DEFAULT})',
    )
    args = parser.parse_args()

    output_dir = os.path.abspath(args.output_dir)
    results_dir = os.path.abspath(args.results_dir)
    max_hours = args.max_hours

    validate_output_dir(output_dir)
    open_reader(output_dir)  # verify pctk import

    paths = results_paths(results_dir, max_hours)
    saved = []

    print('1/4 Biomass combined panel...')
    saved += plot_biomass_combined(output_dir, paths['biomass'], max_hours)

    print('2/4 Substrate time series (reduced)...')
    saved += plot_substrate_time_series_reduced(output_dir, paths['time_series'], max_hours)

    print('3/4 Combined net-export fluxes...')
    saved += plot_fluxes_combined(output_dir, paths['fluxes'], max_hours)

    print('4/4 Spatial composite (growth + H₂ + acetate)...')
    saved += plot_spatial_composite(output_dir, paths['gradient_fields'], max_hours)

    print('\nDone. Generated files:')
    for p in saved:
        print(f'  {p}')


if __name__ == '__main__':
    main()
