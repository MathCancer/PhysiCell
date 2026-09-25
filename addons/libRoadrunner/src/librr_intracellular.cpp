#include "librr_intracellular.h"

#include <sstream>
#include <iostream>

void RoadRunnerMapping::initialize_mapping( void )
{
    bool use_for_input = io_type == "input";

    int behavior_ind = PhysiCell::find_behavior_index(physicell_name);
    int signal_ind = PhysiCell::find_signal_index(physicell_name);
    if (behavior_ind != -1)
    {
        physicell_dictionary_name = "behaviors";
        index = behavior_ind;
        if (use_for_input)
        {
            if (is_delayed)
            {
                value_map = [this](PhysiCell::Cell *pCell)
                {
                    double value = PhysiCell::get_single_behavior(pCell, this->index);
                    double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, this->sbml_species);
                    pCell->phenotype.intracellular->set_parameter_value(this->sbml_species, next_value);
                };
            }
            else
            {
                value_map = [this](PhysiCell::Cell *pCell)
                { pCell->phenotype.intracellular->set_parameter_value(this->sbml_species, PhysiCell::get_single_behavior(pCell, this->index)); };
            }
        }
        else
        {
            if (is_delayed)
            {
                value_map = [this](PhysiCell::Cell *pCell)
                {
                    double value = pCell->phenotype.intracellular->get_parameter_value(this->sbml_species);
                    double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, this->physicell_name);
                    PhysiCell::set_single_behavior(pCell, this->index, next_value);
                };
            }
            else
            {
                value_map = [this](PhysiCell::Cell *pCell)
                { PhysiCell::set_single_behavior(pCell, this->index, pCell->phenotype.intracellular->get_parameter_value(this->sbml_species)); };
            }
        }
    }
    else if (signal_ind != -1)
    {
        physicell_dictionary_name = "signals";
        index = signal_ind;
        if (use_for_input)
        {
            if (is_delayed)
            {
                value_map = [this](PhysiCell::Cell *pCell)
                {
                    double value = PhysiCell::get_single_signal(pCell, this->index);
                    double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, this->sbml_species);
                    pCell->phenotype.intracellular->set_parameter_value(this->sbml_species, next_value);
                };
            }
            else
            {
                value_map = [this](PhysiCell::Cell *pCell)
                { pCell->phenotype.intracellular->set_parameter_value(this->sbml_species, PhysiCell::get_single_signal(pCell, this->index)); };
            }
        }
        else
        {
            value_map = select_signal_setter(physicell_name, sbml_species, is_delayed);
        }
    }
    else if (is_physicell_phenotype_token(physicell_name))
    {
        physicell_dictionary_name = "tokens";
        if (use_for_input)
        {
            value_map = select_phenotype_by_token_inputter(physicell_name, sbml_species, is_delayed);
        }
        else
        {
            value_map = select_phenotype_by_token_outputter(physicell_name, sbml_species, is_delayed);
        }
    }
    else
    {
        std::cerr << "ERROR: " << physicell_name << " is not a valid entry for the libRoadRunner mapping." << std::endl
                  << "    The available entries are: " << std::endl
                  << "    - signals (see dictionaries.txt in output)" << std::endl
                  << "    - behaviors (see dictionaries.txt in output)" << std::endl
                  << "    - tokens: " << std::endl
                  << "        - mms, mpt, mmb, da, dn, vtsc, vtsn, vff" << std::endl
                  << "        - ctr_<start_index>_<end_index>" << std::endl
                  << "        - <prefix>_<substrate_name> where prefix is one of: sur, ssr, ssd, ser" << std::endl << std::endl
                  << "You can also defined your own pre- and post-update functions in the custom.cpp file." << std::endl
                  << "    - Set them using `pCD->functions.pre_update_intracellular = foo;` and `pCD->functions.post_update_intracellular = bar;`" << std::endl
                  << "    - These functions should have the signature `void foo(Cell* pCell, Phenotype& phenotype, double dt)`" << std::endl;
        exit(-1);
    }
    mapping_initialized = true;
    return;
}

// PhysiCell does not have an API for setting signals, but libRoadRunner does in limited cases
MappingFunction select_signal_setter(const std::string& name, const std::string& sbml_species, bool is_delayed)
{
    // if "intracellular <substrate>" or "internalized <substrate>", set the internalized substrate amount
    if (name.find("intracellular") == 0 || name.find("internalized") == 0)
    {
        size_t space_ind = name.find(" ");
        if (space_ind != std::string::npos)
        {
            std::string substrate_name = name.substr(space_ind + 1, std::string::npos);
            int density_index = microenvironment.find_density_index(substrate_name);
            if (density_index != -1)
            {
                if (is_delayed)
                {
                    std::cerr << "ERROR: The libRoadRunner addon does not support delayed output to intracellular substrate concentrations." << std::endl
                              << "    The libRoadRunner addon only supports delayed input **from** the intracellular substrate concentrations." << std::endl
                              << "    This is because BioFVM maintains these values in the present and so this would overwrite those, creating some hybrid, time-traveling monster." << std::endl
                              << "    Nonetheless, if you really want that, the code is here waiting for you in " << __FUNCTION__ << "()" << std::endl
                              << "    Just remove the `exit(-1);` call. Better yet, set a custom `post_update_intracellular` function in the custom.cpp file." << std::endl
                              << "    Or who knows? Maybe someone deleted it..." << std::endl;
                    exit(-1);
                    return [density_index, name, sbml_species](PhysiCell::Cell *pCell)
                    { 
                        double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
                        double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, name);
                        pCell->phenotype.molecular.internalized_total_substrates[density_index] = next_value * pCell->phenotype.volume.total;
                    };
                }
                {
                    return [density_index, sbml_species](PhysiCell::Cell *pCell)
                    { pCell->phenotype.molecular.internalized_total_substrates[density_index] = pCell->phenotype.intracellular->get_parameter_value(sbml_species) * pCell->phenotype.volume.total; };
                }
            }
        }
        std::cerr << "ERROR: \"" << name << "\" is not a valid signal that can be set using libRoadRunner." << std::endl
                  << "    Somehow, this signal was recognized by the signals dictionary but now it seems malformed." << std::endl
                  << "    I honestly don't know what to say...or how to help you :/" << std::endl;
        exit(-1);
    }

    // if "volume", set the cell volume
    else if (name == "volume")
    {
        if (is_delayed)
        {
            std::cerr << "ERROR: The libRoadRunner addon does not support delayed output to cell volume." << std::endl
                      << "    The libRoadRunner addon only supports delayed input **from** the cell volume." << std::endl
                      << "    This is because BioFVM maintains these values in the present and so this would overwrite those, creating some hybrid, time-traveling monster." << std::endl
                      << "    Nonetheless, if you really want that, similar code is here waiting for you in " << __FUNCTION__ << "()" << std::endl
                      << "    Look at how the intracellular substrate concentrations are set for an example (if that's still there)." << std::endl
                      << "    You're best option (in my view) is to set a custom `post_update_intracellular` function in the custom.cpp file." << std::endl;
            exit(-1);
        }
        std::cout << "WARNING: setting the volume using libRoadRunner will do so by rescaling ALL cell volumes, not just setting the total volume."
                  << "    To only set the total volume (or to set other components of the volume), use the `pCell->functions.post_update_intracellular`." << std::endl;
        return [sbml_species](PhysiCell::Cell *pCell)
        { pCell->phenotype.volume.multiply_by_ratio(pCell->phenotype.intracellular->get_parameter_value(sbml_species) / pCell->phenotype.volume.total); };
    }

    // if "damage", set the cell damage
    else if (name == "damage")
    {
        if (!is_delayed)
        {
            return [sbml_species](PhysiCell::Cell *pCell)
            { pCell->phenotype.cell_integrity.damage = pCell->phenotype.intracellular->get_parameter_value(sbml_species); };
        }

        return [sbml_species](PhysiCell::Cell *pCell)
        {
            double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
            double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, "damage");
            pCell->phenotype.cell_integrity.damage = next_value;
        };
    }

    // if begins with "custom", check that it is custom data and set that
    else if (name.find("custom") == 0)
    {
        std::cerr << "ERROR: custom data should be handled using the behavior dictionary, not the signal dictionary." << std::endl
                  << "    It is simpler to use the behavior setters." << std::endl
                  << "    How did you even end up here?" << std::endl;
        exit(-1);
    }
    else
    {
        std::cerr << "ERROR: \"" << name << "\" is not a signal that can be set using libRoadRunner." << std::endl
                  << "    The available signals are: " << std::endl
                  << "        - intracellular <substrate_name>" << std::endl
                  << "        - internalized <substrate_name>" << std::endl
                  << "        - volume" << std::endl
                  << "        - damage" << std::endl
                  << "        - custom:<custom_data_name>" << std::endl;
        exit(-1);
    }
}

bool is_physicell_phenotype_token(const std::string& name)
{
    if (name[0] == 'm')
    {
        std::cout << "WARNING: The token \"" << name << "\" can be replaced with the relevant behavior name. The mapping is as follows:" << std::endl
                  << "    - mms -> migration speed" << std::endl
                  << "    - mpt -> persistence time" << std::endl
                  << "    - mmb -> migration bias" << std::endl << std::endl
                  << "Doing so will allow the libRoadRunner addon to validate that there are not competing mappings." << std::endl;
        return name == "mms" || name == "mpt" || name == "mmb";
    }
    else if (name[0] == 'd')
    {
        std::cout << "WARNING: The token \"" << name << "\" can be replaced with the relevant death model name. The mapping is as follows:" << std::endl
                  << "    - da -> apoptosis" << std::endl
                  << "    - dn -> necrosis" << std::endl << std::endl
                  << "Doing so will allow the libRoadRunner addon to validate that there are not competing mappings." << std::endl;
        return name == "da" || name == "dn";
    }
    else if (name[0] == 's')
    {
        std::cout << "WARNING: The token \"" << name << "\" can be replaced with the relevant substrate name. The mapping is as follows:" << std::endl
                  << "    - sur -> <substrate_name> uptake" << std::endl
                  << "    - ssr -> <substrate_name> secretion" << std::endl
                  << "    - ssd -> <substrate_name> secretion target" << std::endl
                  << "    - ser -> <substrate_name> export" << std::endl
                  << "Doing so will allow the libRoadRunner addon to validate that there are not competing mappings." << std::endl;
        if (name.substr(0, 3) != "sur" && name.substr(0, 3) != "ssr" && name.substr(0, 3) != "ssd" && name.substr(0, 3) != "ser")
        {
            return false;
        }
        if (name[3] != '_')
        {
            return false;
        }
        return microenvironment.find_density_index(name.substr(4, std::string::npos)) != -1;
    }
    else if (name[0] == 'c')
    {
        std::cout << "WARNING: The token \"" << name << "\" can be replaced with the relevant cycle transition rate name. The mapping is as follows:" << std::endl
                  << "    - ctr_<start_index>_<end_index> -> exit from cycle phase <start_index>" << std::endl
                  << "Doing so will allow the libRoadRunner addon to validate that there are not competing mappings." << std::endl;
        std::vector<int> indices = parse_ctr_token(name);
        return indices[0] >= 0 && indices[1] >= 0;
    }
    else if (name[0] == 'v')
    {
        return name == "vtsc" || name == "vtsn" || name == "vff";
    }
    return false;
}

std::vector<int> parse_ctr_token(const std::string &name)
{
    if (name.substr(0, 3) != "ctr" || name[3] != '_')
    {
        throw_invalid_ctr_token(name);
    }
    size_t pos = name.find("_", 4);
    if (pos == std::string::npos)
    {
        throw_invalid_ctr_token(name);
    }
    int start_index;
    int end_index;
    try
    {
        start_index = atoi(name.substr(4, pos - 1).c_str());
        end_index = atoi(name.substr(pos + 1, std::string::npos).c_str());
    }
    catch(const std::exception& e)
    {
        throw_invalid_ctr_token(name);
    }
    return {start_index, end_index};
}

void throw_invalid_ctr_token(const std::string& name)
{
    std::cerr << "ERROR: \"" << name << "\" is not a valid token format." << std::endl
              << "The available cycle tranisition rate (ctr) tokens are \"ctr_<start_index>_<end_index>\"." << std::endl
              << "For example: \"ctr_0_1\" or \"ctr_2_3\"." << std::endl << std::endl
              << "You may have ended up here trying to put in a signal/behavior that begins with a 'c', e.g. a custom data." << std::endl
              << "Check your spelling and make sure that the custom data is included in the first cell definition in your model." << std::endl;
    exit(-1);
}

MappingFunction select_phenotype_by_token_inputter(const std::string& name, const std::string& sbml_species, bool is_delayed)
{
    // the volume ones are not targetable by a behavior (yet?)
    if (name[0] == 'v')
    {
        if (name == "vtsc")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.intracellular->set_parameter_value(sbml_species, pCell->phenotype.volume.target_solid_cytoplasmic); };
            }
            return [sbml_species](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.volume.target_solid_cytoplasmic;
                double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, sbml_species);
                pCell->phenotype.intracellular->set_parameter_value(sbml_species, next_value);
            };
        }
        else if (name == "vtsn")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.intracellular->set_parameter_value(sbml_species, pCell->phenotype.volume.target_solid_nuclear); };
            }
            return [sbml_species](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.volume.target_solid_nuclear;
                double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, sbml_species);
                pCell->phenotype.intracellular->set_parameter_value(sbml_species, next_value);
            };
        }
        else if (name == "vff")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.intracellular->set_parameter_value(sbml_species, pCell->phenotype.volume.target_fluid_fraction); };
            }
            return [sbml_species](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.volume.target_fluid_fraction;
                double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, sbml_species);
                pCell->phenotype.intracellular->set_parameter_value(sbml_species, next_value);
            };
        }
        else
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << name << "\" is not a recognized volume token. The available volume tokens are \"vtsc\", \"vtsn\", and \"vff\"." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }
    }
    std::string behavior = behavior_from_token(name);
    int behavior_index = PhysiCell::find_behavior_index(behavior);
    if (!is_delayed)
    {
        return [sbml_species, behavior_index](PhysiCell::Cell *pCell)
        { pCell->phenotype.intracellular->set_parameter_value(sbml_species, PhysiCell::get_single_behavior(pCell, behavior_index)); };
    }
    return [sbml_species, behavior_index](PhysiCell::Cell *pCell)
    {
        double value = PhysiCell::get_single_behavior(pCell, behavior_index);
        double next_value = getRoadRunnerModel(pCell)->update_input_delay_terms(value, sbml_species);
        pCell->phenotype.intracellular->set_parameter_value(sbml_species, next_value);
    };
}

MappingFunction select_phenotype_by_token_outputter(const std::string& name, const std::string& sbml_species, bool is_delayed)
{
    // the volume ones are not targetable by a behavior (yet?)
    if (name[0] == 'v')
    {
        if (name == "vtsc")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.volume.target_solid_cytoplasmic = pCell->phenotype.intracellular->get_parameter_value(sbml_species); };
            }
            return [sbml_species, name](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
                double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, name);
                pCell->phenotype.volume.target_solid_cytoplasmic = next_value;
            };
        }
        else if (name == "vtsn")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.volume.target_solid_nuclear = pCell->phenotype.intracellular->get_parameter_value(sbml_species); };
            }
            return [sbml_species, name](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
                double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, name);
                pCell->phenotype.volume.target_solid_nuclear = next_value;
            };
        }
        else if (name == "vff")
        {
            if (!is_delayed)
            {
                return [sbml_species](PhysiCell::Cell *pCell)
                { pCell->phenotype.volume.target_fluid_fraction = pCell->phenotype.intracellular->get_parameter_value(sbml_species); };
            }
            return [sbml_species, name](PhysiCell::Cell *pCell)
            {
                double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
                double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, name);
                pCell->phenotype.volume.target_fluid_fraction = next_value;
            };
        }
        else
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << name << "\" is not a recognized volume token. The available volume tokens are \"vtsc\", \"vtsn\", and \"vff\"." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }
    }
    
    // the remaining tokens are all now behaviors
    std::string behavior = behavior_from_token(name);
    int behavior_index = PhysiCell::find_behavior_index(behavior);
    if (!is_delayed)
    {
        return [sbml_species, behavior_index](PhysiCell::Cell *pCell)
        { PhysiCell::set_single_behavior(pCell, behavior_index, pCell->phenotype.intracellular->get_parameter_value(sbml_species)); };
    }
    return [name, sbml_species, behavior_index](PhysiCell::Cell *pCell)
    {
        double value = pCell->phenotype.intracellular->get_parameter_value(sbml_species);
        double next_value = getRoadRunnerModel(pCell)->update_output_delay_terms(value, name);
        PhysiCell::set_single_behavior(pCell, behavior_index, next_value);
    };
}

std::string behavior_from_token(const std::string& name)
{
    if (name[0] == 'm')
    {
        if (name == "mms")
        { return "migration speed"; }
        else if (name == "mpt")
        { return "persistence time"; }
        else if (name == "mmb")
        { return "migration bias"; }
        else
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << name << "\" is not a recognized motility token. The available motility tokens are \"mms\", \"mpt\", and \"mmb\"." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }
    }
    else if (name[0] == 'd')
    {
        if (name != "da" && name != "dn")
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << name << "\" is not a recognized death token. The available death tokens are \"da\" and \"dn\"." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }

        return name == "da" ? "apoptosis" : "necrosis";
    }
    else if (name[0] == 's')
    {
        size_t pos = name.find("_");
        std::string substrate_name = name.substr(pos + 1, std::string::npos);
        int substrate_index = microenvironment.find_density_index(substrate_name);

        if (substrate_index == -1)
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << substrate_name << "\" is not a recognized substrate in this model." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }

        std::string token_prefix = name.substr(0, 3);

        if (token_prefix == "sur")
        { return substrate_name + " uptake"; }
        else if (token_prefix == "ssr")
        { return substrate_name + " secretion"; }
        else if (token_prefix == "ssd")
        { return substrate_name + " secretion target"; }
        else if (token_prefix == "ser")
        { return substrate_name + " export"; }
        else
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: \"" << name << "\" is not a recognized secretion token. The available secretion tokens are \"sur\", \"ssr\", \"ssd\", and \"ser\" followed \"_<substrate_name>\"." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }
    }
    else if (name[0] == 'c')
    {
        std::vector<int> indices = parse_ctr_token(name);
        int start_index = indices[0];
        int end_index = indices[1];
        return "exit from cycle phase " + std::to_string(start_index);
    }
    else
    {
        std::cerr<< std::endl;
        std::cerr << "ERROR: \"" << name << "\" is not a recognized token. It must start with \"m\", \"d\", \"s\", \"c\", or \"v\"." << std::endl;
        std::cerr<< std::endl;
        std::cerr<< std::endl;
        exit(-1);
    }
}

RoadRunnerIntracellular::RoadRunnerIntracellular() : Intracellular()
{
	intracellular_type = "sbml";
    std::cout << "====== " << __FUNCTION__ << "() intracellular_type=" << intracellular_type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "() sbml_filename = " <<  sbml_filename << std::endl;
	parameters.clear();
}

// constructor using XML node
RoadRunnerIntracellular::RoadRunnerIntracellular(pugi::xml_node& node)
{
	intracellular_type = "roadrunner";
	initialize_intracellular_from_pugixml(node);
}

RoadRunnerIntracellular::RoadRunnerIntracellular(RoadRunnerIntracellular* copy) 
{
    update_time_step = copy->update_time_step;
    previous_update_time = PhysiCell::PhysiCell_globals.current_time;
    next_librr_run = PhysiCell::PhysiCell_globals.current_time + update_time_step;
	intracellular_type = copy->intracellular_type;
	sbml_filename = copy->sbml_filename;
	parameters = copy->parameters;
    input_mappings = copy->input_mappings;
    output_mappings = copy->output_mappings;
    mappings_initialized = copy->mappings_initialized;
    input_delay_terms = copy->input_delay_terms;
    output_delay_terms = copy->output_delay_terms;
    // rrHandle and result are left at their in-class initializers; a RoadRunner instance
    // cannot be copied, so start() creates this clone's own.
}

RoadRunnerIntracellular::~RoadRunnerIntracellular()
{
    // input_mappings / output_mappings are deliberately not deleted: the copy constructor
    // above copies those vectors of raw pointers, so every clone shares the pointees.
    rrc::freeRRInstance( rrHandle );
    rrc::freeRRCData( result );
}

// Fatal, like the setup-time check in validate_SBML_species(): there is no value to return
// and nothing sensible to write, so continuing would feed a fabricated number into the model.
static void librr_unknown_species_fatal(const std::string& species_name, const char* caller)
{
    std::cerr << std::endl
              << "ERROR: " << caller << "() was asked for the SBML species \"" << species_name
              << "\", which is not present in this model." << std::endl
              << "       Check the spelling against the species in the SBML file." << std::endl
              << std::endl;
    exit(-1);
}

void RoadRunnerIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        sbml_filename = PhysiCell::xml_get_my_string_value (node_sbml); 
        std::cout << "\n------------- "  << __FUNCTION__ << ": sbml_filename = " << sbml_filename << std::endl;
    }

    update_time_step = PhysiCell::intracellular_dt; // default to this, but overwrite below if defined in XML
    pugi::xml_node node_update_time_step = node.child( "intracellular_dt" );
    if ( node_update_time_step )
    { 
        update_time_step = PhysiCell::xml_get_my_double_value (node_update_time_step); 
        std::cout << "\n------------- "  << __FUNCTION__ << ": intracellular_dt = " << update_time_step << std::endl;
    }

    std::vector<RoadRunnerMapping*> new_input_mappings;
    std::vector<RoadRunnerMapping*> new_output_mappings;

    // delay terms will be appended to these below. make sure they do not retain values from previous cell definitions
    input_delay_terms.clear();
    output_delay_terms.clear();

    pugi::xml_node node_map = node.child( "map" );
    while ( node_map )
    {
        std::string io_type = node_map.attribute( "type" ).value();
        if (io_type != "io" && io_type != "input" && io_type != "output")
        {
            std::cout << "\n------------- "  << __FUNCTION__ << ": ERROR: type must be io, input, or output" << std::endl;
            exit(-1);
        }

        std::string physicell_name = node_map.attribute( "physicell_name" ).value();
        std::string sbml_species = node_map.attribute( "sbml_species" ).value();

        std::vector<bool> delays = parse_delay_terms(node_map, physicell_name, sbml_species, io_type);
        if (io_type == "input" || io_type == "io")
        {
            new_input_mappings.push_back(new RoadRunnerMapping(physicell_name, sbml_species, "input", delays[0]));
        }
        if (io_type == "output" || io_type == "io")
        {
            new_output_mappings.push_back(new RoadRunnerMapping(physicell_name, sbml_species, "output", delays[1]));
        }

        node_map = node_map.next_sibling("map");
    }
    validate_mappings(new_input_mappings, true);
    validate_mappings(new_output_mappings, false);

    input_mappings = std::move(new_input_mappings);
    output_mappings = std::move(new_output_mappings);
}

void validate_mappings(std::vector<RoadRunnerMapping *> mappings, bool is_inputs)
{
    if (mappings.empty())
    { return; }
    std::vector<std::string> values_already_set;
    std::string name_to_add;
    for (auto mapping : mappings)
    {
        name_to_add = is_inputs ? mapping->sbml_species : mapping->physicell_name;
        if (std::find(values_already_set.begin(), values_already_set.end(), name_to_add) != values_already_set.end())
        {
            std::cout << "ERROR: the " << (is_inputs ? "SBML species " : "PhysiCell name ") << name_to_add << " is set by multiple " << (is_inputs ? "inputs" : "outputs") << std::endl;
            exit(-1);
        }
        values_already_set.push_back(name_to_add);
    }
}

void RoadRunnerIntracellular::start()
{
    // called when a new cell is created; creates the unique 'rrHandle'
    rrc::RRVectorPtr vptr;

    rrHandle = createRRInstance();

    if ( !rrc::loadSBML(rrHandle, (sbml_filename).c_str() ) )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        exit(-1);
    }

    int r = rrc::getNumberOfReactions(rrHandle);
    int m = rrc::getNumberOfFloatingSpecies(rrHandle);
    int b = rrc::getNumberOfBoundarySpecies(rrHandle);
    int p = rrc::getNumberOfGlobalParameters(rrHandle);
    int c = rrc::getNumberOfCompartments(rrHandle);

    std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(rrHandle));
    std::stringstream iss(species_names_str);
    std::string species_name;
    int idx = 0;
    while (iss >> species_name)
    {
        species_result_column_index[species_name] = idx;
        idx++;
    }

    vptr = rrc::getFloatingSpeciesConcentrations(rrHandle);
    
    rrc::freeVector(vptr);

	initialize_mappings();
}

bool RoadRunnerIntracellular::need_update()
{
    return PhysiCell::PhysiCell_globals.current_time >= this->next_librr_run - 0.5 * PhysiCell::diffusion_dt;
}

void RoadRunnerIntracellular::update(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt)
{
    pre_update(pCell);
    // update the intracellular model
    static double start_time = 0.0;
    static int num_vals = 2; // start time and end time

    rrc::freeRRCData (this->result);

    this->result = rrc::simulateEx (this->rrHandle, start_time, PhysiCell::PhysiCell_globals.current_time - previous_update_time, num_vals);  // start time, end time, and number of points
    this->previous_update_time = PhysiCell::PhysiCell_globals.current_time;
    this->next_librr_run = PhysiCell::PhysiCell_globals.current_time + update_time_step;

    post_update(pCell);
}

void RoadRunnerIntracellular::initialize_mappings()
{
    for (auto mapping : input_mappings)
    {
        if (mapping->mapping_initialized)
        { continue; }
        mapping->initialize_mapping();
    }
    for (auto mapping : output_mappings)
    {
        if (mapping->mapping_initialized)
        { continue; }
        mapping->initialize_mapping();
    }
    mappings_initialized = true;
}

void RoadRunnerIntracellular::pre_update(PhysiCell::Cell* pCell)
{
    for (auto mapping : input_mappings)
    {
        mapping->value_map(pCell);
    }
}

void RoadRunnerIntracellular::post_update(PhysiCell::Cell* pCell)
{
    for (auto mapping : output_mappings)
    {
        mapping->value_map(pCell);
    }
}

RoadRunnerMapping* RoadRunnerIntracellular::find_input_mapping(std::string sbml_species)
{
    for (auto mapping : input_mappings)
    {
        if (mapping->sbml_species == sbml_species)
        {
            return mapping;
        }
    }
    return nullptr;
}

RoadRunnerMapping* RoadRunnerIntracellular::find_output_mapping(std::string physicell_name)
{
    for (auto mapping : output_mappings)
    {
        if (mapping->physicell_name == physicell_name)
        {
            return mapping;
        }
    }
    return nullptr;
}

double RoadRunnerIntracellular::get_parameter_value(std::string param_name)
{
    rrc::RRVectorPtr vptr;

    // find(), not operator[]: the inserting form silently yields column 0 for an unknown
    // species, so a typo reads a different species than the caller named, and a read
    // mutates the map.
    std::map<std::string,int>::const_iterator it = species_result_column_index.find( param_name );
    if( it == species_result_column_index.end() )
    {
        librr_unknown_species_fatal( param_name, __FUNCTION__ );
    }

    vptr = rrc::getFloatingSpeciesConcentrations(this->rrHandle);
    double res = vptr->Data[it->second];
    rrc::freeVector(vptr);
    return res;
}
	
// rwh: might consider doing a multi-[species_name, value] "set" method
void RoadRunnerIntracellular::set_parameter_value(std::string species_name, double value)
{
    rrc::RRVectorPtr vptr;

    // see get_parameter_value(): operator[] would silently write column 0 for an unknown
    // species, i.e. corrupt a different species than the caller named.
    std::map<std::string,int>::const_iterator it = species_result_column_index.find( species_name );
    if( it == species_result_column_index.end() )
    {
        librr_unknown_species_fatal( species_name, __FUNCTION__ );
    }

    vptr = rrc::getFloatingSpeciesConcentrations(this->rrHandle);
    vptr->Data[it->second] = value;
    rrc::setFloatingSpeciesConcentrations(this->rrHandle, vptr);
    rrc::freeVector(vptr);
}

std::vector<bool> RoadRunnerIntracellular::parse_delay_terms(pugi::xml_node &node_map, std::string physicell_name, std::string sbml_species, std::string io_type)
{
    pugi::xml_node node_delay = node_map.child("delay");
    bool input_is_delayed = false;
    bool output_is_delayed = false;
    while (node_delay)
    {
        if (node_delay.attribute("enabled").empty())
        {
            std::cerr << "ERROR: The delay model is missing the \"enabled\" attribute in the XML file." << std::endl
                      << "    Update the delay element as follows:" << std::endl << std::endl;
            display_sample_delay_element(std::cerr);
            exit(-1);
        }
        if (!node_delay.attribute("enabled").as_bool())
        {
            node_delay = node_delay.next_sibling("delay");
            continue;
        }
        pugi::xml_attribute attribute_type = node_delay.attribute("type");
        std::string attribute_type_value = attribute_type.value();
        bool io_type_is_io = io_type=="io";
        // map is io and the delay type is either not set or is not set properly (input or output), then throw error
        if (io_type_is_io && (!attribute_type || (attribute_type_value != "input" && attribute_type_value != "output")))
        {
            std::cerr << "ERROR: The delay type is not specified for the delay model in the XML file." << std::endl
                      << "    The mapping is both input and output, so the type of delay (input or output) must be specified." << std::endl
                      << "    Update the delay element as follows if, for example, the input is delayed:" << std::endl << std::endl;
            display_sample_delay_element(std::cerr);
            exit(-1);
        }

        std::string delay_io_type = io_type_is_io ? attribute_type_value : io_type;
        std::deque<double>* delay_vector_ptr = nullptr;
        if (delay_io_type == "input")
        {
            if (input_delay_terms.find(sbml_species) != input_delay_terms.end())
            {
                std::cerr << "ERROR: The input delay terms for " << sbml_species << " are multiply defined." << std::endl
                          << "    Update the XML file to remove the duplicate delay terms." << std::endl;
                exit(-1);
            }
            input_is_delayed = true;
            delay_vector_ptr = &input_delay_terms[sbml_species];
        }
        else
        {
            if (output_delay_terms.find(physicell_name) != output_delay_terms.end())
            {
                std::cerr << "ERROR: The output delay terms for " << physicell_name << " are multiply defined." << std::endl
                          << "    Update the XML file to remove the duplicate delay terms." << std::endl;
                exit(-1);
            }
            output_is_delayed = true;
            delay_vector_ptr = &output_delay_terms[physicell_name];
        }

        pugi::xml_node node_delay_time = node_delay.child("delay_time");
        if (!node_delay_time)
        {
            std::cerr << "ERROR: delay_time is not specified for the delay model in the XML file." << std::endl
                      << "    Update the delay element with the 'delay_time' element as in this example:" << std::endl << std::endl;
            display_sample_delay_element(std::cerr);
            exit(-1);
        }
        double delay_time = PhysiCell::xml_get_my_double_value(node_delay_time);

        double initial_value = 0.0; // default to 0.0's being in this vector
        pugi::xml_node node_initial_values = node_delay.child("initial_values");
        if (node_initial_values)
        {
            initial_value = PhysiCell::xml_get_my_double_value(node_initial_values);
        }

        int n_terms = num_delay_terms(delay_time);

        for (int i = 0; i < n_terms; i++)
        {
            (*delay_vector_ptr).push_back(initial_value);
        }
        node_delay = node_delay.next_sibling("delay");
    }
    return {input_is_delayed, output_is_delayed};
}

int RoadRunnerIntracellular::num_delay_terms(double delay_time)
{
    int n_terms = static_cast<int>(round(delay_time / update_time_step));
    double tolerance = 1e-3; // make sure we're within this tolerance of the time step
    if (fabs(delay_time - n_terms * update_time_step) > tolerance)
    {
        std::cerr << "ERROR: The delay time is not a multiple of the intracellular dt for this model." << std::endl
                  << "  - delay_time: " << delay_time << std::endl
                  << "  - update_time_step: " << update_time_step << std::endl
                  << "  - num terms to be saved: " << n_terms << std::endl
                  << "  - effective delay (update_time_step * n_terms): " << n_terms * update_time_step << std::endl
                  << std::endl
                  << "Update these time scales so they meet this criterion." << std::endl
                  << std::endl;
        exit(-1);
    }
    return n_terms;
}

void display_sample_delay_element(std::ostream& os)
{
    os << "<delay type=\"input\" enabled=\"true\">" << std::endl
       << "    <delay_time>10.0</delay_time>" << std::endl
       << "    <initial_values>0.0</initial_values>" << std::endl
       << "</delay>" << std::endl;
}

double RoadRunnerIntracellular::update_input_delay_terms(double value, std::string sbml_species)
{ return update_delay_terms(input_delay_terms[sbml_species], value); }

double RoadRunnerIntracellular::update_output_delay_terms(double value, std::string physicell_name)
{ return update_delay_terms(output_delay_terms[physicell_name], value); }

double update_delay_terms(std::deque<double> &delay_vector, double value)
{
    double next_value = delay_vector.front();
    delay_vector.pop_front();
    delay_vector.push_back(value);
    return next_value;
}

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<RoadRunnerIntracellular*>(phenotype.intracellular);
}

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Cell* pCell) {
    return getRoadRunnerModel(pCell->phenotype);
}

void RoadRunnerIntracellular::save_libRR(std::string path, std::string index)
{
	std::string state_file_name = path + "/states_" + index + ".dat";
	std::ofstream state_file( state_file_name );
	state_file << "---------  dummy output from save_libRR  ---------" << std::endl;
	state_file << "ID,state" << std::endl;
	for( auto cell : *PhysiCell::all_cells )
		state_file << cell->ID << "," << cell->phenotype.intracellular->get_state() << std::endl;
	state_file.close();
}

std::string RoadRunnerIntracellular::get_state()
{
    return sbml_filename;
}

int RoadRunnerIntracellular::validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype)
{
    // the mappings are not yet intialized by this time because the signals/behaviors dictionaries are not yet initialized
    int num_of_phases = (&(phenotype.cycle.model()))->phases.size();
    validate_cycle_mappings(input_mappings, num_of_phases);
    validate_cycle_mappings(output_mappings, num_of_phases);
    return 0;
}

void validate_cycle_mappings(std::vector<RoadRunnerMapping*> mappings, int num_of_phases)
{
    for (auto mapping : mappings)
    {
        // check that the mapping uses a cycle transition rate, i.e. starts with "ctr_"
        if (mapping->physicell_name.find("ctr_") != 0)
        { continue; }

        std::vector<int> indices = parse_ctr_token(mapping->physicell_name);
        if (indices[0] > num_of_phases - 1 || indices[1] > num_of_phases - 1)
        {
            std::cerr << "ERROR: The token \"" << mapping->physicell_name << "\" is invalid for this cell type. The indices are out of range." << std::endl
                      << "  The phase indices must be between 0 and " << num_of_phases - 1 << "." << std::endl;
            exit(-1);
        }
    }
}

int RoadRunnerIntracellular::validate_SBML_species()
{
    // reading SBML
    rrHandle = createRRInstance();
    if ( !rrc::loadSBML(rrHandle, (sbml_filename).c_str() ) )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        exit(-1);
    } 
    // getting Species Names
    std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(rrHandle));
    std::stringstream iss(species_names_str);
    std::string species_name;
    
    std::vector<std::string> all_species {};
    
    int idx = 0;
    while (iss >> species_name)
    {
        species_result_column_index[species_name] = idx;
        all_species.push_back(species_name);
        idx++;
    }

    validate_SBML_species(all_species, input_mappings);
    validate_SBML_species(all_species, output_mappings);

    std::cout << "---- Specified SBML species at config file are validated. ----- " << std::endl;
    return 0;
}

void RoadRunnerIntracellular::validate_SBML_species(std::vector<std::string> all_species, std::vector<RoadRunnerMapping*> mappings)
{
    for (auto mapping : mappings)
    {
        if (std::find(all_species.begin(), all_species.end(), mapping->sbml_species) == all_species.end())
        {
            std::cerr<< std::endl;
            std::cerr << "ERROR: The SBML species \"" << mapping->sbml_species << "\" is not present in the given model." << std::endl;
            std::cerr<< std::endl;
            std::cerr<< std::endl;
            exit(-1);
        }
    }
}

int RoadRunnerIntracellular::create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype)
{
    return 0; 
}
