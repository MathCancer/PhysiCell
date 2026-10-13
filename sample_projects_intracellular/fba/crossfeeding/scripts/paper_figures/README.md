# Crossfeeding paper figure reproduction

Minimal scripts to regenerate the four simulation-derived panels used in the main
crossfeeding figure (experiment 4, 0–50 h window):

| Panel | Output file |
|-------|-------------|
| Biomass | `figures/biomass/biomass_combined_0_50h.png` |
| Substrate time series | `figures/time_series/0_50h/substrate_time_series_reduced.png` |
| Net-export fluxes | `figures/fluxes/fluxes_combined_net_export_fmol_s_0_50h.png` |
| Spatial composite | `figures/gradient_fields/0_50h/microenvironment_initial_final_growth_H2_acetate_per_frame.png` |

Panel A (schematic) is not generated here.

## Prerequisites

- Python 3.9+
- **pctk** — MultiCellDS reader for PhysiCell output  
  Install from the PhysiCelldFBA repository:
  ```bash
  pip install -e /path/to/pctk
  ```
- Python packages:
  ```bash
  pip install -r requirements.txt
  ```

## Input

A PhysiCell **MultiCellDS** output folder from the crossfeeding simulation
(e.g. `output_exp4/`), containing `initial.xml` and snapshot `.mat` files.
This folder is **not** included in the repository.

## Run

```bash
cd sample_projects_intracellular/fba/crossfeeding/scripts/paper_figures

python run_paper_figures.py \
  --output-dir /path/to/output_exp4 \
  --results-dir ./figures \
  --max-hours 50
```

## Output layout

```
figures/
├── biomass/
│   └── biomass_combined_0_50h.png
├── time_series/0_50h/
│   └── substrate_time_series_reduced.png
├── fluxes/
│   └── fluxes_combined_net_export_fmol_s_0_50h.png
└── gradient_fields/0_50h/
    └── microenvironment_initial_final_growth_H2_acetate_per_frame.png
```

SVG versions are written alongside each PNG.

The `figures/` directory is **gitignored** — regenerate it locally with
`run_paper_figures.py`; do not commit generated outputs.

## Troubleshooting

- **`pctk` import error** — install pctk (see above).
- **Missing flux columns** — recompile the project with the crossfeeding
  `custom_modules/custom.cpp` that writes `CB_h2_flux`, `MB_h2_flux`, etc.
- **Empty microenvironment** — verify `--output-dir` points to a complete
  simulation output, not just `initial.*` files.
- **Missing `.mat` snapshot** — if pctk prints `cannot read mat file ...`, that
  time point is skipped (often a partial sync). Re-sync the output folder or
  regenerate figures from the full cluster run for the 0–50 h window.

## Source

Plotting logic is extracted from the PhysiCelldFBA analysis scripts
(`plot_biomass_evolution.py`, `plot_fluxes.py`, `plot_microenvironment.py`)
and styled with `composite_figure_style.py`.
