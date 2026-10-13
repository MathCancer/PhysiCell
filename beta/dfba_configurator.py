#!/usr/bin/env python3
"""
dfba_configurator.py

Usage example:
    python dfba_configurator.py \
      --output config_updated.xml \
      --config dfba_config.yaml \
      --sbml-folder ./models \
      --cell-prefix dfba_

Requirements:
    pip install lxml pandas pyyaml physicell-settings
"""
import os
import argparse
import yaml

from pathlib import Path
from lxml import etree

from physicell_config import PhysiCellConfig

# ----------------------
# Utilities
# ----------------------
def indent(elem, level=0):
    """
    In-place indentation of XML element tree.
    """
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

def xml_pretty_write(tree, path):
    # Ensure indentation is applied
    indent(tree.getroot())
    tree.write(path, pretty_print=True, xml_declaration=True, encoding="UTF-8")

# ----------------------
# Secretion block utilities (per cell_definition)
# ----------------------
# (Removed ensure_secretion_has_substrate and _ensure_secretion_fields as physicell-settings handles this)

# ----------------------
# Chemotactic sensitivities utilities (per cell_definition)
# ----------------------
# (Removed ensure_chemotactic_sensitivities as physicell-settings handles this)

# ----------------------
# Volume update utility
# ----------------------
def set_cell_volume(cell_def, total_volume=None, nuclear_volume=None):
    """
    Ensure phenotype->volume exists and update total and nuclear volumes if provided.
    """
    phenotype = cell_def.find("./phenotype")
    if phenotype is None:
        phenotype = etree.SubElement(cell_def, "phenotype")

    volume = phenotype.find("volume")
    if volume is None:
        volume = etree.SubElement(phenotype, "volume")

    if total_volume is not None:
        total_node = volume.find("total")
        if total_node is None:
            total_node = etree.SubElement(volume, "total", units="micron^3")
        total_node.text = str(total_volume)

    if nuclear_volume is not None:
        nuclear_node = volume.find("nuclear")
        if nuclear_node is None:
            nuclear_node = etree.SubElement(volume, "nuclear", units="micron^3")
        nuclear_node.text = str(nuclear_volume)


# ----------------------
# Intracellular dfba insertion
# ----------------------
def make_exchange_block_xml(exchange_config):
    """Create an exchange XML block from a YAML exchange configuration."""
    ex = etree.Element("exchange", substrate=str(exchange_config["substrate"]))
    etree.SubElement(ex, "fba_flux").text = str(exchange_config["fba_flux"])
    etree.SubElement(ex, "Km", units="mM").text = str(exchange_config["Km"])
    etree.SubElement(ex, "Vmax", units="fmol/pg DW cell/min").text = str(exchange_config["Vmax"])
    return ex


def add_intracellular_dfba(cell_def, sbml_path, model_config):
    """
    Adds (or replaces) phenotype->intracellular type="dfba" block in `cell_def`.
    
    Args:
        cell_def: XML element for cell_definition
        sbml_path: Path to SBML file
        model_config: Dictionary with model configuration from YAML:
            - exchanges: list of exchange dictionaries
            - growth_model: dict with cell_density, reference_volume, max_growth_rate, objective_reaction
            - settings: dict with intracellular_dt
            - death_model: dict with enabled flag
    """
    phenotype = cell_def.find("./phenotype")
    if phenotype is None:
        phenotype = etree.SubElement(cell_def, "phenotype")

    # Remove existing intracellular dfba node if any
    for old in phenotype.findall("intracellular"):
        if old.get("type") == "dfba":
            phenotype.remove(old)

    intracellular = etree.SubElement(phenotype, "intracellular", type="dfba")

    settings_dict = model_config.get("settings", {})

    # Settings
    settings = etree.SubElement(intracellular, "settings")

    sbml_node = etree.SubElement(settings, "sbml_filename")
    sbml_node.text = sbml_path.as_posix()

    
    dt = settings_dict.get("intracellular_dt")
    if dt is None:
        dt = "0.01"
    etree.SubElement(settings, "intracellular_dt", units="min").text = str(dt)

    # Transport model (exchanges)
    transport = etree.SubElement(intracellular, "transport_model")
    exchanges = model_config.get("exchanges", [])
    for exchange in exchanges:
        transport.append(make_exchange_block_xml(exchange))

    # Growth model
    growth = etree.SubElement(intracellular, "growth_model")
    growth_config = model_config.get("growth_model", {})
    
    cell_density = growth_config.get("cell_density", "1.04")
    reference_volume = growth_config.get("reference_volume", "2494")
    nuclear_volume = growth_config.get("nuclear_volume", "540")
    max_growth_rate = growth_config.get("max_growth_rate", "0.01")
    objective_reaction = growth_config.get("objective_reaction", "R_biomass_reactions")
    
    etree.SubElement(growth, "cell_density", units="g/ml").text = str(cell_density)
    etree.SubElement(growth, "reference_volume", units="pg").text = str(reference_volume)
    etree.SubElement(growth, "max_growth_rate", units="1/min").text = str(max_growth_rate)
    etree.SubElement(growth, "objective_reaction").text = str(objective_reaction)

    # Death model
    death_config = model_config.get("death_model", {})
    enabled = death_config.get("enabled", False) if death_config else False
    death_model = etree.SubElement(intracellular, "death_model", enabled=str(enabled).lower())
    death_type = death_config.get("death_type", "necrosis")
    death_trigger_flux = death_config.get("death_trigger_flux", "0.0")
    death_flux_threshold = death_config.get("death_flux_threshold", "0.0")
    death_rate_increase = death_config.get("death_rate_increase", "1.67e-5")

    etree.SubElement(death_model, "death_type", units="g/ml").text = str(death_type)
    etree.SubElement(death_model, "death_trigger_flux", units="1/min").text = str(death_trigger_flux)
    etree.SubElement(death_model, "death_flux_threshold").text = str(death_flux_threshold)
    etree.SubElement(death_model, "death_rate_increase").text = str(death_rate_increase)


# ----------------------
# Cell interactions matrix update
# ----------------------
# (Removed ensure_cell_interactions_for_all as physicell-settings handles this)

# ----------------------
# Validation
# ----------------------
def validate_yaml_config(config, config_yaml_path, sbml_folder):
    """
    Lighter validation of YAML configuration.
    Checks for existence of critical files and sections.
    """
    if "models" not in config or not config["models"]:
        raise ValueError("YAML config must contain a non-empty 'models' section")

    # Validate models and files
    for model_name, model_config in config["models"].items():
        # Check SBML file existence
        settings_dict = model_config.get("settings", {})
        settings_dict = model_config.get("settings", {})
        sbml_path_rel = settings_dict.get("sbml_path")

        if not sbml_path_rel:
            raise ValueError(f"Model '{model_name}': missing 'sbml_path'")

        sbml_path = Path(sbml_path_rel)
        sbml_folder = Path(sbml_folder)

        # Only prepend sbml_folder if needed
        if not sbml_path.is_absolute() and sbml_path.parts[0] != sbml_folder.name:
            sbml_path = sbml_folder / sbml_path

        if not sbml_path.exists():
            raise ValueError(
                f"Model '{model_name}': SBML file not found: {sbml_path.as_posix()}"
            )

        # Check exchanges existence
        if "exchanges" not in model_config:
            raise ValueError(f"Model '{model_name}': missing 'exchanges' section")

    print(f"[OK] YAML configuration structure validated")


# ----------------------
# Main orchestration
# ----------------------
def update_config_with_dfba(output_xml_path,
                           config_yaml_path,
                           sbml_folder,
                           cell_prefix="",
                           verbose=False):
    """
    Create PhysiCell config from scratch with dFBA models from YAML configuration.
    
    Args:
        output_xml_path: Path to write updated XML
        config_yaml_path: Path to YAML configuration file
        sbml_folder: Folder containing SBML files (for resolving relative paths)
        cell_prefix: Optional prefix for new cell definition names
    """
    # Load YAML configuration
    with open(config_yaml_path, 'r') as f:
        yaml_config = yaml.safe_load(f)
    
    # Validate configuration before processing
    validate_yaml_config(yaml_config, config_yaml_path, sbml_folder)
    
    # Initialize PhysiCellConfig from scratch
    config = PhysiCellConfig()
    print(f"[INFO] Initialized new PhysiCell configuration")

    # Handle Substrates
    global_substrates = set()
    if "substrates" in yaml_config:
        # Sort substrates by name
        sorted_substrates = sorted(yaml_config["substrates"], key=lambda x: x.get("name", ""))
        
        for sub_config in sorted_substrates:
            name = sub_config.get("name")
            global_substrates.add(name)
            
            diff = sub_config.get("diffusion_coefficient", 50000.0)
            decay = sub_config.get("decay_rate", 0.0)
            init = sub_config.get("initial_condition", 0.0)
            
            print(f"[INFO] Adding microenvironment variable for substrate '{name}' "
                  f"(diffusion={diff}, decay={decay}, init={init})")
            
            config.add_simple_substrate(
                name=name,
                diffusion_coeff=diff,
                decay_rate=decay,
                initial_value=init
            )
    
    global_substrates_sorted = sorted(global_substrates)

    # Create new cells
    models = yaml_config["models"]
    created_cell_names = []
    next_id = 0

    for model_name, model_config in models.items():
        new_name = f"{cell_prefix}{model_name}"
        
        # Create default cell type
        config.cell_types.add_cell_type(new_name)
        
        # Set ID
        config.cell_types.cell_types[new_name]['ID'] = str(next_id)

        # Set cell cycle to "live"
        config.cell_types.set_cycle_model(new_name, "live")
        
        # Update volume
        growth_cfg = model_config.get("growth_model", {})
        total_volume = growth_cfg.get("reference_volume")
        nuclear_volume = growth_cfg.get("nuclear_volume", 0.0)
        
        if total_volume is not None:
            # Use helper if available or direct access
            if hasattr(config.cell_types, 'set_volume_parameters'):
                config.cell_types.set_volume_parameters(new_name, total=float(total_volume), nuclear=float(nuclear_volume))
            else:
                # Fallback to direct access
                if 'phenotype' not in config.cell_types.cell_types[new_name]:
                    config.cell_types.cell_types[new_name]['phenotype'] = {}
                if 'volume' not in config.cell_types.cell_types[new_name]['phenotype']:
                    config.cell_types.cell_types[new_name]['phenotype']['volume'] = {}
                
                config.cell_types.cell_types[new_name]['phenotype']['volume']['total'] = str(total_volume)
                config.cell_types.cell_types[new_name]['phenotype']['volume']['nuclear'] = str(nuclear_volume)

        print(f"[INFO] Created cell_definition '{new_name}' with ID={next_id}")
        
        created_cell_names.append(new_name)
        next_id += 1

    # Add random_seed to user params
    config.add_user_parameter("random_seed", parameter_type="int", description="Random seed for simulation", value="0")

    # Generate XML string to perform low-level dFBA injections
    print("[INFO] Generating intermediate XML for dFBA injection...")
    xml_str = config.generate_xml()
    
    # Parse with lxml
    root = etree.fromstring(xml_str.encode('utf-8'))
    
    # Get all cell names for interaction matrix
    all_cell_defs = root.findall(".//cell_definition")
    all_cell_names = [cd.get("name") for cd in all_cell_defs]
    
    # Inject dFBA blocks and ensure interactions
    for cell_def in all_cell_defs:
        name = cell_def.get("name")
        
        # Identify if this cell corresponds to a model
        model_key = None
        if name in created_cell_names:
            if name.startswith(cell_prefix):
                potential_model = name[len(cell_prefix):]
                if potential_model in models:
                    model_key = potential_model
        
        if model_key:
            model_config = models[model_key]

            settings_dict = model_config.get("settings", None)
            if not settings_dict:
                print(f"[WARNING] Model '{model_key}' missing 'settings' section; using defaults.")
                continue

            sbml_path = Path(settings_dict.get("sbml_path"))
            sbml_folder = Path(sbml_folder)

            if not sbml_path.is_absolute() and sbml_path.parts[0] != sbml_folder.name:
                sbml_path = sbml_folder / sbml_path

            # Resolve SBML path
            print((f"DEBUGG: {sbml_path}"))
            add_intracellular_dfba(cell_def, sbml_path, model_config)
            
            # Ensure volume (redundant but safe)
            growth_cfg = model_config.get("growth_model", {})
            total_volume = growth_cfg.get("reference_volume")
            nuclear_volume = growth_cfg.get("nuclear_volume", 0.0)
            if total_volume is not None:
                set_cell_volume(cell_def, total_volume=total_volume, nuclear_volume=nuclear_volume)
        else:
            print(f"[WARNING] Skipping dFBA injection for cell_definition '{name}' (no matching model)")
    # Write final output
    tree = etree.ElementTree(root)
    xml_pretty_write(tree, output_xml_path)
    print(f"[OK] Wrote updated config to: {output_xml_path}")


# ----------------------
# CLI
# ----------------------
def main():
    print("[INFO] Running dfba_configurator.py (refactored with physicell-settings)")
    parser = argparse.ArgumentParser(description="Generate PhysiCell config with dFBA blocks from scratch.")
    parser.add_argument("--output", "-o", required=True, help="Path to write updated PhysiCell XML.")
    parser.add_argument("--config", "-c", required=True,
                        help="YAML configuration file with models, SBML paths, exchanges, and parameters.")
    parser.add_argument("--sbml-folder", "-s", default="", help="Folder containing SBML files (for resolving relative paths).")
    parser.add_argument("--cell-prefix", default="", help="Optional prefix to prepend to new cell_definition names.")
    parser.add_argument("--verbose", action="store_true",
                        help="Enable verbose debug output.")
    args = parser.parse_args()

    update_config_with_dfba(
        args.output,
        args.config,
        args.sbml_folder,
        cell_prefix=args.cell_prefix,
        verbose=args.verbose
    )


if __name__ == "__main__":
    main()

