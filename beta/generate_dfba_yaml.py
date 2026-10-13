#!/usr/bin/env python3
"""
Script to generate a dfba YAML model from an SBML input file.
Based on the preprocess_dfba_model.ipynb notebook.
"""

import argparse
import yaml
import pandas as pd

from cobra.io import read_sbml_model
from cobra.flux_analysis import flux_variability_analysis

def main(sbml_file, output_yaml, cell_type="MCF7_core", cell_volume=2494.0, ex_prefix="R_EX"):
    # Default values
    init_concentration_default = 5.0
    diffusion_coefficient_default = 50000.0
    decay_rate_default = 0.0
    Km_default = 0.001  # mM

    # Load SBML model
    model = read_sbml_model(sbml_file, f_replace={'F_REACTION': lambda x: x})

    # Find objective reaction
    objective_reaction = None
    for r in model.reactions:
        if r.objective_coefficient != 0:
            objective_reaction = r.id
            break
    if objective_reaction is None:
        raise ValueError("No objective reaction found in the model.")
    print(f"Objective reaction: {objective_reaction}")

    # Optimize model
    solution = model.optimize()
    max_growth_rate = solution.objective_value
    print(f"Max growth rate: {max_growth_rate} 1/hr")

    # Run FVA
    print("Running FVA...")
    fva_df = flux_variability_analysis(model, processes=10, fraction_of_optimum=0.0)

    # Process exchange reactions
    fva_ex_df = fva_df.filter(regex=f'^{ex_prefix}', axis=0)

    eps = 1e-7
    mask = fva_ex_df.abs().max(axis=1) < eps
    to_remove = fva_ex_df.index[mask].tolist()

    # Find exchange reactions for protons and waters
    additional_to_remove = []
    for r in model.exchanges:
        if r.id.startswith(ex_prefix) and len(r.metabolites) == 1:
            met = list(r.metabolites.keys())[0]
            met_name_lower = met.name.lower()
            if ('h+' in met_name_lower or 'proton' in met_name_lower or 
                'h2o' in met_name_lower or 'water' in met_name_lower or
                met.id.lower() in ['h_e', 'h2o_e', 'oh_e', 'oh1_e']):
                additional_to_remove.append(r.id)

    to_remove.extend(additional_to_remove)
    to_remove = list(set(to_remove))  # remove duplicates

    fva_ex_df = fva_ex_df.drop(to_remove, axis=0, errors='ignore')

    substrates_list = []
    exchanges_list = []

    for i in fva_ex_df.index:
        r = model.reactions.get_by_id(i)
        substrate = list(r.metabolites)[0].name

        Vmax = 0.0
        init_concentration = 0.0
        lb = fva_ex_df.loc[i, 'minimum'].round(5)
        if lb < 0.0:
            Vmax = -1.0 * float(lb)
            init_concentration = init_concentration_default

        substrates_dict = {
            "name": substrate,
            "diffusion_coefficient": diffusion_coefficient_default,
            "decay_rate": decay_rate_default,
            "initial_condition": init_concentration
        }

        substrates_list.append(substrates_dict)

        exchange_dict = {
            "substrate": substrate,
            "fba_flux": r.id,
            "Km": Km_default,
            "Vmax": float(Vmax)
        }

        exchanges_list.append(exchange_dict)

    # Create model dictionary
    model_dict = {
        "settings": {
            "sbml_path": sbml_file,  # Use the input path
            "intracellular_dt": 0.01
        },
        "growth_model": {
            "cell_density": 1.04,              # Cell density (g/ml)
            "reference_volume": cell_volume,          # Reference volume for cell division (pg)
            "nuclear_volume": 540,            # Nuclear volume (pg)
            "max_growth_rate": max_growth_rate,           # Maximum growth rate (1/min)
            "objective_reaction": objective_reaction  # Objective reaction ID from SBML model
        },
        "death_model": {
            "enabled": False,
            "death_type": "necrosis",          # death type string
            "death_trigger_flux": 0.0,       # 1/min
            "death_flux_threshold": 0.0,     # dimensionless
            "death_rate_increase": 1.67e-5  # rate increase
        },
        "exchanges": exchanges_list
    }

    dfba_config_dict = {
        "substrates": substrates_list,
        "models": {
            cell_type: model_dict
        }
    }

    # Write to YAML
    with open(output_yaml, "w") as f:
        yaml.dump(dfba_config_dict, f, sort_keys=False, default_flow_style=False)

    print(f"YAML model written to {output_yaml}")
    print("Substrates:")
    print("\n".join([i['name'] for i in substrates_list]))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate dfba YAML model from SBML file.")
    parser.add_argument("sbml_file", help="Path to the SBML file")
    parser.add_argument("output_yaml", help="Path to the output YAML file")
    parser.add_argument("--cell_type", default="MCF7_core", help="Cell type name (default: MCF7_core)")
    parser.add_argument("--cell_volume", type=float, default=2494.0, help="Cell volume in um^3 (default: 2494.0)")
    parser.add_argument("--ex_prefix", default="R_EX", help="Prefix for exchange reactions (default: R_EX)")

    args = parser.parse_args()
    main(args.sbml_file, args.output_yaml, args.cell_type, args.cell_volume, args.ex_prefix)