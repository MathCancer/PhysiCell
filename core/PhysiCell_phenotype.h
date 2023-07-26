/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2022, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#ifndef __PhysiCell_phenotype_h__
#define __PhysiCell_phenotype_h__

#include <vector>
#include <string>
#include <unordered_map>
#include <map> 

#include "../BioFVM/BioFVM.h" 

#include "../modules/PhysiCell_settings.h"

using namespace BioFVM; 

namespace PhysiCell{
class Cell;
class Cycle_Model; 
class Phenotype; 

class Cell_Definition; 

/*
// future use?
class BM_Point
{
 private:
 public:
	std::vector<double> position; 
	std::vector<double> unit_normal; 
		
	BM_Point(); 
	
	double distance_from_point( std::vector<double>& point ); 
	void displacement_from_point( std::vector<double>& point, std::vector<double>& displacement ); 
};
*/

class Phase
{
 public:
	int index; // an internal index for the cycle model
	int code; // a global identifier code 
	std::string name; 
	
	bool division_at_phase_exit; // does this phase trigger division? 
	bool removal_at_phase_exit; // does this phase trigger removal? 
	
	void (*entry_function)( Cell* pCell, Phenotype& phenotype, double dt ); 
	
	Phase(); // done
};

class Phase_Link
{
 public:
	int start_phase_index;
	int end_phase_index; 
	
	bool fixed_duration; 
	
	bool (*arrest_function)( Cell* pCell, Phenotype& phenotype, double dt ); 
		// return true if arrested, false if not 
		
	void (*exit_function)( Cell* pCell, Phenotype& phenotype, double dt );
		// function to be excecuted when completing the phase transition 
	
	Phase_Link(); // done
};

class Cycle_Data
{
 private:
 
	// this maps the end_phase_index to the link index in each 
	// phase_links[i]
	// So, index_inverse_map[i][j] = k, corresponds to 
	// phases[i], phase_links[i][k] (which links from phase i to phase j)
	// transition_rates[i][k] (the transition rate from phase i to phase j) 
	std::vector< std::unordered_map<int,int> > inverse_index_maps; 
	
 public:
	Cycle_Model* pCycle_Model; 

	std::string time_units; 
	
	std::vector< std::vector<double> > transition_rates; 
	
	int current_phase_index; 
	double elapsed_time_in_phase; 
	
	Cycle_Data(); // done 
	
	// return current phase (by reference)
	Phase& current_phase( void ); // done 
	
	// make the data structures consistent with the corresponding cell cycle model
	void sync_to_cycle_model( void ); // done 

	// access the transition rate from phase i to phase j (by reference)
	double& transition_rate(int start_phase_index, int end_phase_index ); // done

	double& exit_rate(int phase_index ); // This returns the first transition rate out of 
		// phase # phase_index. It is only relevant if the phase has only one phase link 
		// (true for many cycle models). 
};

class Cycle_Model
{
 private:
 
	// this maps the end_phase_index to the link index in each 
	// phase_links[i]
	// So, index_inverse_map[i][j] = k, corresponds to 
	// phases[i], phase_links[i][k] (which links from phase i to phase j)
	// transition_rates[i][k] (the transition rate from phase i to phase j)
	std::vector< std::unordered_map<int,int> > inverse_index_maps; 
 
 public:
	std::string name; 
	int code; 

	std::vector<Phase> phases; 
	std::vector< std::vector<Phase_Link> > phase_links; 
	
	int default_phase_index; 
	
	Cycle_Data data; // this will be copied to individual cell agents 

	Cycle_Model(); 
	
	void advance_model( Cell* pCell, Phenotype& phenotype, double dt ); // done 
	
	int add_phase( int code, std::string name ); // done 
	
	int add_phase_link( int start_index, int end_index , 
		bool (*arrest_function)( Cell* pCell, Phenotype& phenotype, double dt ) ); // done 
	int add_phase_link( int start_index, int end_index , double rate , 
		bool (*arrest_function)( Cell* pCell, Phenotype& phenotype, double dte ) ); // done 
	
	int find_phase_index( int code ); // done 
	int find_phase_index( std::string name ); // done 
	
	double& transition_rate( int start_index , int end_index ); // done 
	Phase_Link& phase_link(int start_index,int end_index ); // done 
	
	std::ostream& display( std::ostream& os ); // done 
};

class Cycle
{
 private:
 
 public:
	Cycle_Model* pCycle_Model; 
	Cycle_Data data; 
	
	Cycle();// done 
	
	void advance_cycle( Cell* pCell, Phenotype& phenotype, double dt ); // done 
	
	Cycle_Model& model( void ); // done 
	Phase& current_phase( void ); // done 
	int& current_phase_index( void ); // done 
	
	void sync_to_cycle_model( Cycle_Model& cm ); // done 
};

class Death_Parameters
{
 public:
	std::string time_units; 
 
	double unlysed_fluid_change_rate;
	double lysed_fluid_change_rate; 
	
	double cytoplasmic_biomass_change_rate;
	double nuclear_biomass_change_rate; 
	
	double calcification_rate; 
	
	double relative_rupture_volume; 
	
	Death_Parameters(); // done 
};

class Death
{
 private:
 public:
	std::vector<double> rates; 
	std::vector<Cycle_Model*> models; 
	std::vector<Death_Parameters> parameters; 
	
	bool dead; 
	int current_death_model_index;
	
	Death(); // done 
	
	int add_death_model( double rate, Cycle_Model* pModel );  // done
	int add_death_model( double rate, Cycle_Model* pModel, Death_Parameters& death_parameters); // done 
	
	int find_death_model_index( int code ); // done 
	int find_death_model_index( std::string name ); // done 
	
	bool check_for_death( double dt ); // done
	void trigger_death( int death_model_index ); // done 
	
	Cycle_Model& current_model( void ); // done
	Death_Parameters& current_parameters( void ); // done '

	// ease of access
	double& apoptosis_rate(void); 
	double& necrosis_rate(void); 
};

class Volume
{
 public:
	//
	// state variables 
	//
	double total;
	double solid;
	double fluid;
	double fluid_fraction; 
	
	double nuclear;
	double nuclear_fluid;
	double nuclear_solid; 

	double cytoplasmic;
	double cytoplasmic_fluid; 
	double cytoplasmic_solid; 
	
	double calcified_fraction;
	
	double cytoplasmic_to_nuclear_ratio;
	
	double rupture_volume; // in volume units 
	
	//
	// a function that can be set by the user. 
	//
	// void (*volume_update_function)( Cell* pCell, Phenotype& phenotype, double dt ); 
	
	//
	// parameters that can be set by users 
	//
	double cytoplasmic_biomass_change_rate; 
	double nuclear_biomass_change_rate; 
	double fluid_change_rate;

	double calcification_rate; 
	
	double target_solid_cytoplasmic;
	double target_solid_nuclear;
	double target_fluid_fraction;
	
	double target_cytoplasmic_to_nuclear_ratio;

	double relative_rupture_volume; 
	// the volume ratio (compared to initial volume at time of death) 
	// at which a cell ruptures / lyses / bursts. 

	//
	// functions 
	//
	Volume(); // done 
	
	void divide( void ); // done 
	void multiply_by_ratio(double); // done 
};

class Geometry
{
 public:
	double radius; 
	double nuclear_radius; 
	double surface_area; 
	
	double polarity; 
	
	Geometry(); // done 
	
	void update_radius( Cell* pCell, Phenotype& phenotype, double dt ); // done 
	void update_nuclear_radius( Cell* pCell, Phenotype& phenotype, double dt ); // done 
	void update_surface_area( Cell* pCell, Phenotype& phenotype, double dt ); // done 
	
	void update( Cell* pCell, Phenotype& phenotype, double dt ); // done 
};

class Mechanics
{
 private:
 public:
	double cell_cell_adhesion_strength; 
	double cell_BM_adhesion_strength;

	double cell_cell_repulsion_strength;
	double cell_BM_repulsion_strength; 

	std::vector<double> cell_adhesion_affinities; 
	double& cell_adhesion_affinity( std::string type_name ); // done 
	void sync_to_cell_definitions(); // done 
	void set_fully_heterotypic( void ); // done 
	void set_fully_homotypic( Cell* pCell ); // done 

	// this is a multiple of the cell (equivalent) radius
	double relative_maximum_adhesion_distance; 
	// double maximum_adhesion_distance; // needed? 

	/* for spring attachments */ 

	int maximum_number_of_attachments; 
	double attachment_elastic_constant; 

	double attachment_rate; 
	double detachment_rate; 

	/* to be deprecated */ 

	double relative_maximum_attachment_distance; 
	double relative_detachment_distance; 
	double maximum_attachment_rate; 
	
	Mechanics(); // done 
	
	void set_relative_maximum_adhesion_distance( double new_value ); // done 
	void set_relative_equilibrium_distance( double new_value ); // done 
	
	void set_absolute_equilibrium_distance( Phenotype& phenotype, double new_value ); // done 
	
	
};

class Motility
{
 public:
	bool is_motile; 
 
	double persistence_time; // mean time to keep going in one direction 
		// before resampling for a new direction. 
	double migration_speed; // migration speed along chosen direction, 
		// in absence of all other adhesive / repulsive forces 
	
	std::vector<double> migration_bias_direction; // a unit vector
		// random motility is biased in this direction (e.g., chemotaxis)
	double migration_bias; // how biased is motility
		// if 0, completely random. if 1, deterministic along the bias vector 
		
	bool restrict_to_2D; 
		// if true, set random motility to 2D only. 
		
	std::vector<double> motility_vector; 
	
	int chemotaxis_index; 
	int chemotaxis_direction; 
	
	// advanced chemotaxis 
	std::vector<double> chemotactic_sensitivities; 
	double& chemotactic_sensitivity( std::string name ); 
	
	void sync_to_current_microenvironment( void ); 
	void sync_to_microenvironment( Microenvironment* pNew_Microenvironment ); 
	
		
	Motility(); // done 
};

class Secretion
{
 private:
 public:
	Microenvironment* pMicroenvironment; 
	
	std::vector<double> secretion_rates; 
	std::vector<double> uptake_rates; 
	std::vector<double> saturation_densities;
	std::vector<double> net_export_rates; 
	
	// in the default constructor, we'll size to the default microenvironment, if 
	// specified. (This ties to BioFVM.) 
	Secretion(); // done 

	// use this to properly size the secretion parameters to the microenvironment in 
	// pMicroenvironment
	void sync_to_current_microenvironment( void ); // done 
	
	void advance( Basic_Agent* pCell, Phenotype& phenotype , double dt ); 
	
	// use this to properly size the secretion parameters to the microenvironment 
	void sync_to_microenvironment( Microenvironment* pNew_Microenvironment ); // done 
	
	void set_all_secretion_to_zero( void ); // NEW
	void set_all_uptake_to_zero( void ); // NEW
	void scale_all_secretion_by_factor( double factor ); // NEW
	void scale_all_uptake_by_factor( double factor ); // NEW

	// ease of access
	double& secretion_rate( std::string name ); 
	double& uptake_rate( std::string name ); 
	double& saturation_density( std::string name ); 
	double& net_export_rate( std::string name );  	
};

class Cell_Functions
{
 private:
 public:
	Cell* (*instantiate_cell)(); 

	Cycle_Model cycle_model; 

	void (*volume_update_function)( Cell* pCell, Phenotype& phenotype , double dt ); // used in cell 
	void (*update_migration_bias)( Cell* pCell, Phenotype& phenotype, double dt ); 
	
	void (*custom_cell_rule)( Cell* pCell, Phenotype& phenotype, double dt ); 
	void (*update_phenotype)( Cell* pCell, Phenotype& phenotype, double dt ); // used in celll
	
	void (*pre_update_intracellular) ( Cell* pCell, Phenotype& phenotype, double dt );
	void (*post_update_intracellular) ( Cell* pCell, Phenotype& phenotype, double dt );

	void (*update_velocity)( Cell* pCell, Phenotype& phenotype, double dt ); 
	
	void (*add_cell_basement_membrane_interactions)(Cell* pCell, Phenotype& phenotype, double dt );
	double (*calculate_distance_to_membrane)( Cell* pCell, Phenotype& phenotype, double dt );
	
	void (*set_orientation)(Cell* pCell, Phenotype& phenotype, double dt );
	
	void (*contact_function)(Cell* pMyself, Phenotype& my_phenotype, 
		Cell* pOther, Phenotype& other_phenotype, double dt ); 
		
	/* prototyping / beta in 1.5.0 */ 
/*	
	void (*internal_substrate_function)(Cell* pCell, Phenotype& phenotype , double dt ); 
	void (*molecular_model_function)(Cell* pCell, Phenotype& phenotype , double dt ); 
*/
	
	
	void (*plot_agent_SVG)(std::ofstream& os, Cell* pCell, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower);
	void (*plot_agent_legend)(std::ofstream& os, Cell_Definition* cell_def, double& cursor_x, double& cursor_y, std::vector<std::string> (*cell_coloring_function)(Cell*), double temp_cell_radius);

	
	Cell_Functions(); // done 
};

class Bools
{
	public:
		std::vector<bool> values; 
		std::unordered_map<std::string,int> name_map; 
		std::string& name( int i ); 
		std::vector<std::string> units; 
		
		int size( void ); 
		void resize( int n ); 
		int add( std::string name , std::string units , bool value ); 
		
		bool& operator[]( int i ); 
		bool& operator[]( std::string name ); 
		
		Bools(); 
};

class Molecular
{
	private:
	public: 
		Microenvironment* pMicroenvironment; 
	
		// model much of this from Secretion 
		Molecular(); 
 	
		// we'll set this to replace BioFVM's version		
		std::vector<double> internalized_total_substrates; 

		// for each substrate, a fraction 0 <= f <= 1 of the 
		// total internalized substrate is released back inot
		// the environment at death 
		std::vector<double> fraction_released_at_death; 

		// for each substrate, a fraction 0 <= f <= 1 of the 
		// total internalized substrate is transferred to the  
		// predatory cell when ingested 
		std::vector<double> fraction_transferred_when_ingested; 
		
		/* prototyping / beta in 1.5.0 */ 
		// Boolean, Integer, and Double parameters
/*		
		std::vector<bool> bools; 
		std::unordered_map<std::string,int> bool_name_map; 
		std::string& bool_name( int i ); 
		std::vector<std::string> bool_units; 
		void resize_bools( int n ); 
		int add_bool( std::string name , std::string units , bool value ); 
		bool& access_bool( std::string name ); 
		
		std::vector<int> ints; 
		std::unordered_map<std::string,int> int_name_map; 
		std::string& int_name( int i ); 
		std::vector<std::string> int_units; 
		int& access_int( std::string name ); 
		
		std::vector<int> doubles; 
		std::unordered_map<std::string,int> double_name_map; 
		std::string& double_name( int i ); 
		std::vector<std::string> double_units; 
		double& access_double( std::string name ); 
*/
	
		// use this to properly size the secretion parameters to the 
		// microenvironment in molecular.pMicroenvironment. 
		void sync_to_current_microenvironment( void ); // done 
		
//		void advance( Basic_Agent* pCell, Phenotype& phenotype , double dt ); 
		
		// use this to properly size the secretion parameters to the microenvironment in 
		// pMicroenvironment
		void sync_to_microenvironment( Microenvironment* pNew_Microenvironment ); // done 
		
		// use this 
		void sync_to_cell( Basic_Agent* pCell ); 

		// ease of access 
		double&  internalized_total_substrate( std::string name ); 
		
};

class Intracellular
{
 private:
 public:
    std::string intracellular_type;  // specified in XML <intracellular type="...">:  "maboss", "sbml", ...
	// bool enabled; 

    // ==========  specific to SBML ==============
    // std::string sbml_filename;

	
    // ================  generic  ================
	// This function parse the xml cell definition
	virtual void initialize_intracellular_from_pugixml(pugi::xml_node& node) = 0;
	
	// This function initialize the model, needs to be called on each cell once created
	virtual void start() = 0;
	
	// This function checks if it's time to update the model
	virtual bool need_update() = 0;

	// This function update the model for the time_step defined in the xml definition
	virtual void update() = 0;
	virtual void update(Cell* cell, Phenotype& phenotype, double dt) = 0;

	// This function deals with inheritance from mother to daughter cells
	virtual void inherit(Cell* cell) = 0;

	// Get value for model parameter
	virtual double get_parameter_value(std::string name) = 0;
	
	// Set value for model parameter
	virtual void set_parameter_value(std::string name, double value) = 0;

	virtual std::string get_state() = 0;  
	
	virtual void display(std::ostream& os) = 0;

	virtual Intracellular* clone() = 0;
	
	virtual ~Intracellular(){};
	

    // ================  specific to "maboss" ================
	virtual bool has_variable(std::string name) = 0; 
	virtual bool get_boolean_variable_value(std::string name) = 0;
	virtual void set_boolean_variable_value(std::string name, bool value) = 0;
	// virtual bool get_double_variable_value(std::string name) = 0;
	// virtual void set_double_variable_value(std::string name, bool value) = 0;
	virtual void print_current_nodes() = 0;
	

    // ================  specific to "roadrunner" ================
    virtual int update_phenotype_parameters(PhysiCell::Phenotype& phenotype) = 0;
    virtual int validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype) = 0;
    virtual int validate_SBML_species() = 0;
    virtual int create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype) = 0;
};

class Cell_Interactions
{
 private:
 public: 
	// phagocytosis parameters (e.g., macrophages)
	double dead_phagocytosis_rate; 
	std::vector<double> live_phagocytosis_rates; 
	// attack parameters (e.g., T cells)

	std::vector<double> attack_rates;
		// do I attack cell type j? 

	std::vector<double> immunogenicities; // new! 
		// how immnogenic am I to cell type j? 

	double damage_rate;  
	// cell fusion parameters 
	std::vector<double> fusion_rates;
	
	// initialization 
	Cell_Interactions(); // done 
	void sync_to_cell_definitions(); // done 
	
	// ease of access 
	double& live_phagocytosis_rate( std::string type_name ); // done 
	double& attack_rate( std::string type_name ); // done 
	double& fusion_rate( std::string type_name ); // done 
	double& immunogenicity( std::string type_name ); // done 
	
	// automated cell phagocytosis, attack, and fusion 
//	void perform_interactions( Cell* pCell, Phenotype& phenotype, double dt ); 
};

class Cell_Transformations
{
 private:
 public: 
	// rates of transforming into different cell types 
	std::vector<double> transformation_rates; 
	
	// initialization
	Cell_Transformations(); // done 
	void sync_to_cell_definitions(); // done 
	
	// ease of access 
	double& transformation_rate( std::string type_name ); // done
	
	// automated cell transformations
	// void perform_transformations( Cell* pCell, Phenotype& phenotype, double dt ); 
};

// pre-beta functionality in 1.10.3 
class Integrity
{
 private:
 public: 
	// generic damage variable
	double damage; 
	double damage_rate; 
	double damage_repair_rate; 

	// lipid damage (e.g, cell membrane, organelles)
	double lipid_damage; 
	double lipid_damage_rate; 
	double lipid_damage_repair_rate; 

	// DNA damage 
	double DNA_damage; 
	double DNA_damage_rate; 
	double DNA_damage_repair_rate; 

	// other damages?
	// mitochondrial? spindle? other? 

	Integrity(); 

	void advance_damage_models( double dt ); 
};

class Phenotype
{
 private:
 public:
	bool flagged_for_division; 
	bool flagged_for_removal; 
 
	Cycle cycle; 
	Death death; 
	Volume volume; 
	Geometry geometry; 
	Mechanics mechanics; 
	Motility motility; 
	Secretion secretion; 
	
	Molecular molecular; 

    // We need it to be a pointer to allow polymorphism
	// then this object could be a MaBoSSIntracellular, or a RoadRunnerIntracellular
	Intracellular* intracellular;
	
	Cell_Interactions cell_interactions; 
	Cell_Transformations cell_transformations; 
	
	Phenotype(); // done 
	Phenotype(const Phenotype &p);
	~Phenotype();
	Phenotype& operator=(const Phenotype &p );

	void sync_to_functions( Cell_Functions& functions ); // done 
	
	void sync_to_microenvironment( Microenvironment* pMicroenvironment ); 
	
	// make sure cycle, death, etc. are synced to the defaults. 
	void sync_to_default_functions( void ); // done 
};

};

#endif
