#ifndef _dFBA_Intracellular_h_
#define _dFBA_Intracellular_h_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>   // for setw

#include <coin/CoinPackedMatrix.hpp>
#include <coin/CoinPackedVector.hpp>
#include <coin/ClpSimplex.hpp>

#include "../../../core/PhysiCell.h"
#include "../../../core/PhysiCell_phenotype.h"
#include "../../../core/PhysiCell_cell.h"
#include "../../../core/PhysiCell_utilities.h"
#include "../../../modules/PhysiCell_pugixml.h"
#include "../../../core/PhysiCell_constants.h"

#include "dfba_Model.h"
#include "dfba_Reaction.h"


using namespace std;

namespace PhysiCelldFBA {

static std::string PhysiCelldFBA_Version = "0.0.1"; 

static double hours_to_minutes = 1.0 / 60.0;
static float PI = PhysiCell::PhysiCell_constants::pi;
static double fba_epsilon_tolerance = 1e-7;

struct KineticParam
{
	string name;
	string units;
	float value;
};

struct ExchangeFluxData
{
	string density_name;
	string fba_flux_id;
	int density_index;
	KineticParam Km;
	KineticParam Vmax;
};

class dFBAIntracellular : public PhysiCell::Intracellular 
{
 private:
 public:

    double dfba_time_step = PhysiCell::diffusion_dt; 

 	std::string sbml_filename;
	std::string objective_reaction;
	double reference_volume = 0.0;
	double cell_density = 0.0;
	double max_growth_rate = 0;
	double current_growth_rate = 0;
	double next_dfba_run = 0;
	bool use_metabolic_death = true;
	std::string death_type;
	std::string death_trigger_flux;
	double death_flux_threshold = 0.0;
	double death_rate_increase = 0.0;
	bool flag_for_death = false;
	dFBAModel sbml_model;
	bool is_initialized = false;

	bool skip_standard_optimization = false;

	double liter_micron_cubes_conversion = 1e15; // 1 liter = 1e15 micron^3

	/** \brief map between density IDs and exchange reactions */
	std::map<std::string, ExchangeFluxData> substrate_exchanges;

    dFBAIntracellular();

	dFBAIntracellular(pugi::xml_node& node);
	
	dFBAIntracellular(const dFBAIntracellular& copy);

	//	~Intracellular();

	// rwh: review this
	Intracellular* clone(){
		return static_cast<Intracellular*>(new dFBAIntracellular(*this));
	}

	Intracellular* getIntracellularModel() 
	{
		return static_cast<Intracellular*>(this);
	}
	
    // ================  generic  ================
	// This function parse the xml cell definition
	void initialize_intracellular_from_pugixml(pugi::xml_node& node);
	
	// This function checks if it's time to update the model
	bool need_update() { return std::abs(PhysiCell::PhysiCell_globals.current_time -this->next_dfba_run) <= 0.0001; }

	// This function deals with inheritance from mother to daughter cells
	void inherit(PhysiCell::Cell* cell){ return;};

	// Get value for model parameter
	double get_parameter_value(std::string name) { return 0;}
	
	// Set value for model parameter
	void set_parameter_value(std::string name, double value) { return; }

	std::string get_state(){ return ""; }
	
	void display(std::ostream& os){ return; }

	void start();

	virtual void update();

	void update(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt){
		 // STEP 1.
		// date exchange fluxes lower bound using concentration values of the
		// corresponding densities at the agent voxel
		if (cell->phenotype.death.dead == false)
		{
			this->update_dfba_inputs(cell, phenotype, dt);

			// STEP 2: Run optimization (custom or standard)
			if (cell->functions.custom_optimization != NULL) {
				// Use custom optimization function if set
				cell->functions.custom_optimization(cell, phenotype, dt);
			} else {
				// Standard biomass optimization
				this->update();
			}

			// STEP 3. Update the cell volumne using the growth rate from FBA
			// STEP 4. rescale exchange fluxes from the dfba model and use them to update the net_export_rates
			// STEP 5. remove the internalized substrates if needed
			this->update_dfba_outputs(cell, phenotype, dt);

			this->next_dfba_run = PhysiCell::PhysiCell_globals.current_time + dfba_time_step;

			// if (phenotype.volume.total	>= 2 * this->reference_volume){
			// 	cell->flag_for_division();
			// }
			double v = phenotype.volume.total;
			double v_ref = this->reference_volume;

			// Only start checking after ~1.6 × reference volume
			if (v >= 1.6 * v_ref) {
				// normalized progress from 0 (at 1.6×) to 1 (at 2×)
				double progress = (v - 1.6 * v_ref) / (0.4 * v_ref);
				if (progress < 0.0) progress = 0.0;
				if (progress > 1.0) progress = 1.0;

				// map progress to probability of dividing this timestep
				// e.g. sigmoid or just linear:
				double prob = progress;   // 0 at 1.6×, 1 at 2×

				// stochastic check
				if (PhysiCell::UniformRandom() < prob * dt) {
					cell->flag_for_division();
				}
			}
		}
		else
		{
			this->current_growth_rate = 0.0;
			// update the cell volume using the standard volume update function
			PhysiCell::standard_volume_update_function(cell, phenotype, dt); // to update volume during necrosis, during necrotitc swelling, volume increases until rupture
			return ;
		}
	};

 	void update_dfba_inputs( PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt );
	void update_dfba_outputs( PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt );

	dFBASolution optimize_for_objective(std::string reaction_id, double coefficient);

	// =============== dFBA specific functions ===============

	
	int parse_transport_model(pugi::xml_node& node);
	void parse_growth_model(pugi::xml_node& node);
	void parse_death_model(pugi::xml_node& node);
	void initLpSolver();

	void setObjectiveCoefficient(std::string reaction_id, double coefficient) {
		this->sbml_model.setObjectiveCoefficient(reaction_id, coefficient);
	}

	void clearObjective() {
		this->sbml_model.clearObjective();
	}

	void restoreObjectiveState() {
		this->sbml_model.restoreObjectiveState();
	}

	// libroadrunner specifics
		
	// for now, define dummy methods for these in the abstract parent class
	
	// This function initialize the model, needs to be called on each cell once created
	
	
	
        // static void save_PhysiBoSS(std::string path, std::string index);
	static void save_dFBA(std::string path, std::string index);

	void update_volume(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double growth_rate, double dt);
	void standard_update_cell_volume(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double growth_rate, double dt);



	// ================  specific to "dFBA" ================

	double get_flux_value(std::string name);
	double get_growth_rate(){ return this->current_growth_rate; }
	void print_model(PhysiCell::Cell* pCell, double current_time, std::string output_folder = "./output");

    // ================  specific to "maboss" ================
	bool has_variable(std::string name) { return false; }
	bool get_boolean_variable_value(std::string name) { return false; }
	void set_boolean_variable_value(std::string name, bool value) {	}
	void print_current_nodes(){	}
	

    // ================  specific to "roadrunner" ================
    int update_phenotype_parameters(PhysiCell::Phenotype& phenotype) {return 0; }
    int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype) {return 0; }
    int validate_SBML_species() {return 0; }
    int create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype) {return 0; }

};

}

#endif
