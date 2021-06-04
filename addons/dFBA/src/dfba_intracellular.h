#ifndef _dFBA_Intracellular_h_
#define _dFBA_Intracellular_h_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>   // for setw

#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../modules/PhysiCell_pugixml.h"
#include "./FBA_model.h"

using namespace std;


class dFBAIntracellular : public PhysiCell::Intracellular 
{
 private:
    std::string sbml_filename;

 	std::map<std::string, double> parameters;
	std::map<std::string, std::string> substrate_fluxes;
	std::map<std::string, std::string> custom_data_species;
	std::map<std::string, std::string> phenotype_species;
	std::map<std::string, int> species_result_column_index;

    double next_model_run = 0;

	FBA_model model;

 public:
	

    dFBAIntracellular();

	dFBAIntracellular(pugi::xml_node& node);
	
	dFBAIntracellular(dFBAIntracellular* copy);
	
    // rwh: review this
	dFBAIntracellular* clone()
    {
		dFBAIntracellular* clone = new dFBAIntracellular(this);
		clone->sbml_filename = this->sbml_filename;
		clone->substrate_species = this->substrate_species;
        clone->phenotype_species = this->phenotype_species;
		clone->custom_data_species = this->custom_data_species;
		return static_cast<Intracellular*>(clone);
	}

	Intracellular* getIntracellularModel() 
    {
        // std::cout << "------ librr_intracellular: getIntracellularModel called\n";
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
    int start();

	bool need_update();
    
	int update();
    
    int update_phenotype_parameters(PhysiCell::Phenotype& phenotype);
  
    int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype);

    int validate_SBML_species();
	
	double get_parameter_value(std::string name);

	int set_parameter_value(std::string name, double value);
	
	
    std::string get_state();

    // for now, define dummy methods for these in the abstract parent class
    bool has_node(std::string name) { return false; }
    bool get_boolean_node_value(std::string name) { return false; }
	void set_boolean_node_value(std::string name, bool value)  {}
    void print_current_nodes() {}
	
    // static void save_PhysiBoSS(std::string path, std::string index);
	static void save_dFBA(std::string path, std::string index);
};



#endif