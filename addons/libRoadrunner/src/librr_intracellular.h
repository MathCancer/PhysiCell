#ifndef _RoadRunner_Intracellular_h_
#define _RoadRunner_Intracellular_h_

#include <string>
#include <map>
#include <iomanip>   // for setw

#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../modules/PhysiCell_pugixml.h"
// #include "maboss_network.h"

// #ifdef ADDON_ROADRUNNER
// These are for C
// #define STATIC_RRC
// #include "rrc_api.h"
// #include "rrc_types.h"
#include "rrc_api.h"
#include "rrc_types.h"
#include <functional>
#include <algorithm>

// #include "rrc_utilities.h"
extern "C" rrc::RRHandle createRRInstance();
// #endif

typedef std::function<void(PhysiCell::Cell* pCell)> MappingFunction;

class RoadRunnerMapping
{
public:
	std::string physicell_name;
	std::string sbml_species;
	std::string io_type;
	std::string physicell_dictionary_name;
	int index;
	MappingFunction value_map = [] (PhysiCell::Cell *pCell) {}; // default to a function that does nothing
	bool mapping_initialized = false;

	RoadRunnerMapping() {};
	RoadRunnerMapping(std::string physicell_name, std::string sbml_species, std::string io_type)
		: physicell_name(physicell_name), sbml_species(sbml_species), io_type(io_type) {};

	void initialize_mapping(void);
};

MappingFunction select_signal_setter(const std::string& name, const std::string& sbml_species);

bool is_physicell_phenotype_token(const std::string& name);
MappingFunction select_phenotype_by_token_inputter(const std::string& name, const std::string& sbml_species);
MappingFunction select_phenotype_by_token_outputter(const std::string& name, const std::string& sbml_species);

void validate_mappings(std::vector<RoadRunnerMapping *> mappings, bool is_inputs);

std::vector<int> parse_ctr_token(const std::string &name);
void throw_invalid_ctr_token(const std::string& name);
void validate_cycle_mappings(std::vector<RoadRunnerMapping*> mappings, int num_of_phases);

class RoadRunnerIntracellular : public PhysiCell::Intracellular 
{
 private:
 public:
    std::string sbml_filename;

	int num_rows_result_table = 1;

	std::map<std::string, double> parameters;
	bool mappings_initialized = false;
	void initialize_mappings();
	std::vector<RoadRunnerMapping *> input_mappings;
	std::vector<RoadRunnerMapping *> output_mappings;
	std::map<std::string, int> species_result_column_index;
	
    rrc::RRHandle rrHandle;
	rrc::RRCDataPtr result = 0;  // start time, end time, and number of points

	double update_time_step = 0.01;
	double previous_update_time = 0.0;
	double next_librr_run = 0.0;

    RoadRunnerIntracellular();

	RoadRunnerIntracellular(pugi::xml_node& node);
	
	RoadRunnerIntracellular(RoadRunnerIntracellular* copy);
	
	Intracellular* clone()
    {
		RoadRunnerIntracellular* clone = new RoadRunnerIntracellular(this);
		return static_cast<Intracellular*>(clone);
	}

	Intracellular* getIntracellularModel() 
    {
        std::cout << "------ librr_intracellular: getIntracellularModel called\n";
		return static_cast<Intracellular*>(this);
	}

	void initialize_intracellular_from_pugixml(pugi::xml_node &node);

	// Need 'int' return type to avoid bizarre compile errors? But 'void' to match MaBoSS.
	void start();

	bool need_update();

	void update() {}; // needed because the base class has this function
	void update(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt);

	void pre_update(PhysiCell::Cell* cell);
	void post_update(PhysiCell::Cell* cell);
    
	void inherit(PhysiCell::Cell * cell) {}

	// These find_<IO>_mapping functions are not currently used, but since I made them, we'll keep them around.
	RoadRunnerMapping *find_input_mapping(std::string sbml_species); // sbml_species is unique for inputs (below is for convenience)
	RoadRunnerMapping *find_input_mapping(std::string physicell_name, std::string sbml_species)
	{ return find_input_mapping(sbml_species); } // sbml_species is unique for inputs

	RoadRunnerMapping *find_output_mapping(std::string physicell_name); // physicell_name is unique for outputs (below is for convenience)
	RoadRunnerMapping *find_output_mapping(std::string physicell_name, std::string sbml_species)
	{ return find_output_mapping(physicell_name); } // physicell_name is unique for outputs

	int update_phenotype_parameters(PhysiCell::Phenotype& phenotype) {return 0;}; // all handled within update
    int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype);
    int validate_SBML_species();
	void validate_SBML_species(std::vector<std::string> all_species, std::vector<RoadRunnerMapping*> mappings);
    int create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype);
	
	double get_parameter_value(std::string name);
	void set_parameter_value(std::string name, double value);
	
	std::string get_state();
	void display(std::ostream&os) {}
    // for now, define dummy methods for these in the abstract parent class
    bool has_variable(std::string name) { return false; }
	bool get_boolean_variable_value(std::string name) { return false; }
	void set_boolean_variable_value(std::string name, bool value)  {}
    void print_current_nodes() {}

	static void save_libRR(std::string path, std::string index);
};

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Phenotype& phenotype);
RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Cell* pCell);
#endif