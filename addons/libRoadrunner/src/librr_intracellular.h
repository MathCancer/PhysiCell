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
#include "../roadrunner/include/rr/C/rrc_api.h"
#include "../roadrunner/include/rr/C/rrc_types.h"
// #include "rrc_utilities.h"
extern "C" rrc::RRHandle createRRInstance();
// #endif

class RoadRunnerIntracellular : public PhysiCell::Intracellular 
{
 private:
 public:
	
	// static long counter;

    std::string sbml_filename;
    // bool enabled = false;


	int num_rows_result_table = 1;
	
	// double time_step = 12;
	// bool discrete_time = false;
	// double time_tick = 0.5;
	// double scaling = 1.0;
	
	// std::map<std::string, double> initial_values;
	std::map<std::string, double> parameters;
	std::map<std::string, std::string> substrate_species;
	std::map<std::string, std::string> custom_data_species;
	std::map<std::string, std::string> phenotype_species;
	std::map<std::string, int> species_result_column_index;
	
    // rrc::RRHandle rrHandle = createRRInstance();
    rrc::RRHandle rrHandle;
    // rrc::RRHandle rrHandle;
    // rrc::RRVectorPtr vptr;
	rrc::RRCDataPtr result = 0;  // start time, end time, and number of points

	double next_librr_run = 0;

    RoadRunnerIntracellular();

	RoadRunnerIntracellular(pugi::xml_node& node);
	
	RoadRunnerIntracellular(RoadRunnerIntracellular* copy);
	
    // rwh: review this
	Intracellular* clone()
    {
		// return static_cast<Intracellular*>(new RoadRunnerIntracellular(this));
		RoadRunnerIntracellular* clone = new RoadRunnerIntracellular(this);
		clone->sbml_filename = this->sbml_filename;
		clone->substrate_species = this->substrate_species;
        clone->phenotype_species = this->phenotype_species;
		clone->custom_data_species = this->custom_data_species;
		return static_cast<Intracellular*>(clone);
	}

	Intracellular* getIntracellularModel() 
    {
        std::cout << "------ librr_intracellular: getIntracellularModel called\n";
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
    // Need 'int' return type to avoid bizarre compile errors? But 'void' to match MaBoSS.
	void start();

	bool need_update();

    // Need 'int' return type to avoid bizarre compile errors.
	void update();
	void update(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt) {
		update();
		update_phenotype_parameters(phenotype);
	}
    
	void inherit(PhysiCell::Cell * cell) {}
	
    int update_phenotype_parameters(PhysiCell::Phenotype& phenotype);
    int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype);
    int validate_SBML_species();
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

#endif