#ifndef _dFBA_Intracellular_h_
#define _dFBA_Intracellular_h_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>   // for setw

#include "../../../core/PhysiCell.h"
#include "../../../BioFVM/BioFVM.h"
#include "../../../modules/PhysiCell_pugixml.h"

#include "./FBA_model.h"

using namespace std;


struct kinetic_parm
{
	string name;
	string untis;
	float value;
};

struct exchange_data
{
	string density_name;
	string fba_flux_id;
	int density_index;
	kinetic_parm Km;
	kinetic_parm Vmax;
};


class dFBAIntracellular : public PhysiCell::Intracellular 
{
 private:
    std::string sbml_filename;
	FBA_model model;

 	std::map<std::string, double> parameters;
	std::map<std::string, exchange_data> substrate_exchanges;

    double next_model_run = 0;

 public:
	

    dFBAIntracellular();

	dFBAIntracellular(pugi::xml_node& node);
	
	dFBAIntracellular(dFBAIntracellular* copy);
	
    // rwh: review this
	Intracellular* clone()
    {
		dFBAIntracellular* clone = new dFBAIntracellular(this);
		clone->sbml_filename = this->sbml_filename;
		clone->substrate_exchanges = this->substrate_exchanges;
		return static_cast<Intracellular*>(clone);
	}

	Intracellular* getIntracellularModel() 
        {
		return static_cast<Intracellular*>(this);
	}
	
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
    void start();

	bool need_update();
    
	void update(){ };

	void update(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt);
    void inherit(PhysiCell::Cell * cell) {}
	
	int update_phenotype_parameters(PhysiCell::Phenotype& phenotype);
	bool has_variable(std::string name) { return true; }

	// libroadrunner specifics
        int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype){ return true;}
        int validate_SBML_species(){ return true;}
		int create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype) {return 0; }
	std::string get_state(){ return "none";}
	double get_parameter_value(std::string name){ return -1.0; }
	void set_parameter_value(std::string name, double value){  }
	
        // for now, define dummy methods for these in the abstract parent class
        bool has_node(std::string name) { return false; }
        bool get_boolean_variable_value(std::string name) { return false; }
	void set_boolean_variable_value(std::string name, bool value)  {}
         void print_current_nodes() {}
	
        // static void save_PhysiBoSS(std::string path, std::string index);
	void display(std::ostream&os) {}
	static void save_dFBA(std::string path, std::string index);
};



#endif
