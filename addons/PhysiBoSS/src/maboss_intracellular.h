#ifndef _MaBoSS_Intracellular_h_
#define _MaBoSS_Intracellular_h_

#include <string>
#include <map>
#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../modules/PhysiCell_pugixml.h"
#include "maboss_network.h"
#include "utils.h"

static std::string PhysiBoSS_Version = "2.2.3"; 
static std::string PhysiBoSS_DOI = "10.1038/s41540-023-00314-4"; 
static std::string PhysiBoSS_URL = "https://github.com/PhysiBoSS/PhysiBoSS"; 

class MaBoSSIntracellular : public PhysiCell::Intracellular {
 private:
 public:

	static long counter;
	
	std::string bnd_filename;
	std::string cfg_filename;
	
	double time_step = 12;
	bool discrete_time = false;
	double time_tick = 0.5;
	double scaling = 1.0;
	double time_stochasticity = 0.0;
	bool inherit_state = false;
	std::map<std::string, bool> inherit_nodes;
	double start_time = 0.0;

	std::map<std::string, double> initial_values;
	std::map<std::string, double> mutations;
	std::map<std::string, double> parameters;

	std::map<std::string, MaBoSSInput> listOfInputs;
	std::vector<int> indicesOfInputs;
	std::map<std::string, MaBoSSOutput> listOfOutputs;
	std::vector<int> indicesOfOutputs;
	MaBoSSNetwork maboss;

	double next_physiboss_run = 0.0;

	MaBoSSIntracellular();
	
	MaBoSSIntracellular(pugi::xml_node& node);
	
	MaBoSSIntracellular(MaBoSSIntracellular* copy);
	
	Intracellular* clone() {
		return static_cast<Intracellular*>(new MaBoSSIntracellular(this));
	}
	Intracellular* getIntracellularModel() {
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
	void start() {
		this->maboss.restart_node_values();
		this->next_physiboss_run = std::max(this->start_time, PhysiCell::PhysiCell_globals.current_time);
	}
	
	void update() {
		this->maboss.run_simulation();
		this->next_physiboss_run += this->maboss.get_time_to_update();
	}

	void update(PhysiCell::Cell * cell, PhysiCell::Phenotype& phenotype, double dt) {
		this->update_inputs(cell, phenotype, dt);
		this->maboss.run_simulation();
		this->update_outputs(cell, phenotype, dt);
		this->next_physiboss_run += this->maboss.get_time_to_update();
	}
	
	bool need_update() {
		return PhysiCell::PhysiCell_globals.current_time >= this->next_physiboss_run;
	}
	
	void inherit(PhysiCell::Cell * cell) {
		maboss.inherit_state(
			static_cast<MaBoSSIntracellular*>(cell->phenotype.intracellular)->maboss.get_maboss_state(), 
			inherit_state, inherit_nodes
		);
	}
	void update_inputs(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt);
	void update_outputs(PhysiCell::Cell * cell, PhysiCell::Phenotype& phenotype, double dt);

	bool has_variable(std::string name) {
		return this->maboss.has_node(name);
	}
	
	bool get_boolean_variable_value(std::string name) {
		return this->maboss.get_node_value(name);
	}
	

	void set_boolean_variable_value(std::string name, bool value) {
		this->maboss.set_node_value(name, value);
	}
	
	double get_parameter_value(std::string name) {
		return this->maboss.get_parameter_value(name);
	}
	
	void set_parameter_value(std::string name, double value) {
		this->maboss.set_parameter_value(name, value);
	}
	
	std::string get_state() {
		return this->maboss.get_state();
	}
	
	void print_current_nodes(){
		this->maboss.print_nodes();
	}
	void save_current_nodes(std::ostream& out_stream){
		this->maboss.save_nodes(out_stream); 
	}

	void save_current_parameters_maboss(std::ostream& out_stream){
		this->maboss.save_current_parameters(out_stream); 
	}
	void read_current_parameter_maboss(std::ifstream& in_stream){
		this->maboss.read_current_parameter(in_stream); 
	}
	int get_number_of_nodes(){
		return this->maboss.get_nodes_number().size(); 
	}

	void reinit_maboss(std::string networkFile, std::string configFile){
		this->maboss.init_maboss(networkFile, configFile);
	}

	void save_current_parameters(std::ostream& out_stream){

		out_stream << "maboss_intracellular_parameters:" << std::endl;
		out_stream << "time_step: " << this->time_step << std::endl;
		out_stream << "discrete_time: " << (this->discrete_time ? "true" : "false") << std::endl;
		out_stream << "time_tick: " << this->time_tick << std::endl;
		out_stream << "scaling: " << this->scaling << std::endl;
		out_stream << "time_stochasticity: " << this->time_stochasticity << std::endl;
		out_stream << "inherit_state: " << (this->inherit_state ? "true" : "false") << std::endl;
		out_stream << "start_time: " << this->start_time << std::endl;
		out_stream << "next_physiboss_run: " << this->next_physiboss_run << std::endl;
		/*
		// Print the contents of the inherit_nodes map 
		for(const auto& pair : inherit_nodes) {
			out_stream << "Key: " << pair.first << " Value: " << (pair.second ? "true" : "false") << std::endl;
		}
		
		// Print the contents of the initial_values map
		for(const auto& pair : initial_values) {
			out_stream << "Key: " << pair.first << " Value: " << pair.second << std::endl;
		}

		// Print the contents of the mutations map
		for(const auto& pair : mutations) {
			out_stream << "Key: " << pair.first << " Value: " << pair.second << std::endl;
		}

		// Print the contents of the parameters map
		for(const auto& pair : parameters) {
			out_stream << "Key: " << pair.first << " Value: " << pair.second << std::endl;
		}*/
		out_stream << std::endl;
		
	}

	void read_current_parameter(std::ifstream& in_stream){
		std::string dummy;

		//skip header
		std::getline(in_stream, dummy);

		//time_step
		std::getline(in_stream, dummy);
		this->time_step = read_number_in_line(dummy);

		//discrete_time
		std::getline(in_stream, dummy);
		this->discrete_time = read_number_in_line_bool(dummy);

		//time_tick
		std::getline(in_stream, dummy);
		this->time_tick = read_number_in_line(dummy);

		//scaling
		std::getline(in_stream, dummy);
		this->scaling = read_number_in_line(dummy);

		//time_stochasticity
		std::getline(in_stream, dummy);
		this->time_stochasticity = read_number_in_line(dummy);

		//inherit_state
		std::getline(in_stream, dummy);
		this->inherit_state = read_number_in_line_bool(dummy);

		//start_time
		std::getline(in_stream, dummy);
		this->start_time = read_number_in_line(dummy);

		//next_physiboss_run
		std::getline(in_stream, dummy);
		this->next_physiboss_run = read_number_in_line(dummy);

		// skip empty line
		std::getline(in_stream, dummy);
	
	}

	void display(std::ostream& os);
	
	static void save(std::string filename);

    // unneeded for this type
    int update_phenotype_parameters(PhysiCell::Phenotype& phenotype) {return 0;}
    int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype) {return 0; }
    int validate_SBML_species() {return 0; }
    int create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype) {return 0; }

};

#endif
