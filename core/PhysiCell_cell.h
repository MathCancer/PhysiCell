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
# Copyright (c) 2015-2021, Paul Macklin and the PhysiCell Project             #
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

#ifndef __PhysiCell_cell_h__
#define __PhysiCell_cell_h__

#include "./PhysiCell_custom.h" 

#include "../BioFVM/BioFVM.h"
#include "./PhysiCell_phenotype.h"
#include "./PhysiCell_cell_container.h"
#include "./PhysiCell_constants.h"

#include "../modules/PhysiCell_settings.h" 

#include "./PhysiCell_standard_models.h" 

using namespace BioFVM; 

namespace PhysiCell{
class Cell_Container;

class Cell_Parameters
{
 private:
 public:
	// oxygen values (in mmHg) for critical phenotype changes
	double o2_hypoxic_threshold; // value at which hypoxic signaling starts
	double o2_hypoxic_response; // value at which omics changes are observed 
	double o2_hypoxic_saturation; // value at which hypoxic signalign saturates 
	// o2_hypoxic_saturation < o2_hypoxic_threshold
	
	double o2_proliferation_saturation; // value at which extra o2 does not increase proliferation
	double o2_proliferation_threshold; // value at which o2 is sufficient for proliferation

	double o2_reference; // physioxic reference value, in the linked reference Phenotype
	// o2_proliferation_threshold < o2_reference < o2_proliferation_saturation; 
	
	double o2_necrosis_threshold; // value at which cells start experiencing necrotic death 
	double o2_necrosis_max; // value at which necrosis reaches its maximum rate 
	// o2_necrosis_max < o2_necrosis_threshold
	
	Phenotype* pReference_live_phenotype; // reference live phenotype (typically physioxic) 
	// Phenotype* pReference_necrotic_phenotype; // reference live phenotype (typically physioxic) 

	// necrosis parameters (may evenually be moved into a reference necrotic phenotype 
	double max_necrosis_rate; // deprecate
	int necrosis_type; // deprecate 
	
	Cell_Parameters(); 
}; 

class Cell_Definition
{
 private:
 public: 
	int type; 
	std::string name; 
 
	Microenvironment* pMicroenvironment; 
	
	Cell_Parameters parameters; 
	Custom_Cell_Data custom_data; 
	Cell_Functions functions; 
	Phenotype phenotype; 

	Cell_Definition();  // done 
	Cell_Definition( Cell_Definition& cd ); // copy constructor 
	Cell_Definition& operator=( const Cell_Definition& cd ); // copy assignment 
};

extern Cell_Definition cell_defaults; 

class Cell_State
{
 private:
 public:
	std::vector<Cell*> attached_cells; 

	std::vector<Cell*> neighbors; // not currently tracked! 
	std::vector<double> orientation;
	
	double simple_pressure; 
	
	int number_of_attached_cells( void ); 
	
	Cell_State(); 
};

class Cell : public Basic_Agent 
{
 private: 
	Cell_Container * container;
	int current_mechanics_voxel_index;
	int updated_current_mechanics_voxel_index; // keeps the updated voxel index for later adjusting of current voxel index
		
 public:
	std::string type_name; 
 
	Custom_Cell_Data custom_data;
	Cell_Parameters parameters;
	Cell_Functions functions; 

	Cell_State state; 
	Phenotype phenotype; 
	
	void update_motility_vector( double dt_ );
	void advance_bundled_phenotype_functions( double dt_ ); 
	
	void add_potentials(Cell*);       // Add repulsive and adhesive forces.
	void set_previous_velocity(double xV, double yV, double zV);
	int get_current_mechanics_voxel_index();
	void turn_off_reactions(double); 		  // Turn off all the reactions of the cell
	
	bool is_out_of_domain;
	bool is_movable;
	
	void flag_for_division( void ); // done 
	void flag_for_removal( void ); // done 
	
	void start_death( int death_model_index ); 
	void lyse_cell( void ); 

	Cell* divide( void );
	void die( void ); 
	void step(double dt);
	Cell();
	
	~Cell(); 
	
	bool assign_position(std::vector<double> new_position);
	bool assign_position(double, double, double);
	void set_total_volume(double);
	
	double& get_total_volume(void); // NEW
	
	void set_target_volume(double); 
	void set_target_radius(double); 
	void set_radius(double); 
	
	
	
	// mechanics 
	void update_position( double dt ); //
	std::vector<double> displacement; // this should be moved to state, or made private  

	
	void assign_orientation();  // if set_orientaion is defined, uses it to assign the orientation
								// otherwise, it assigns a random orientation to the cell.
	
	void copy_function_pointers(Cell*);
	
	void update_voxel_in_container(void);
	void copy_data(Cell *);
	
	void ingest_cell( Cell* pCell_to_eat ); // for use in predation, e.g., immune cells 

	void attach_cell( Cell* pAddMe ); // done 
	void detach_cell( Cell* pRemoveMe ); // done 
	void remove_all_attached_cells( void ); // done 

	// I want to eventually deprecate this, by ensuring that 
	// critical BioFVM and PhysiCell data elements are synced when they are needed 
	
	void set_phenotype( Phenotype& phenotype ); // no longer needed?
	void update_radius();
	Cell_Container * get_container();
	
	std::vector<Cell*>& cells_in_my_container( void ); 
	std::vector<Cell*> nearby_cells( void ); // new in 1.8.0 
	std::vector<Cell*> nearby_interacting_cells( void ); // new in 1.8.0 
	
	void convert_to_cell_definition( Cell_Definition& cd ); 
};

Cell* create_cell( void );  
Cell* create_cell( Cell_Definition& cd );  


void delete_cell( int ); 
void delete_cell( Cell* ); 
void save_all_cells_to_matlab( std::string filename ); 

//function to check if a neighbor voxel contains any cell that can interact with me
bool is_neighbor_voxel(Cell* pCell, std::vector<double> myVoxelCenter, std::vector<double> otherVoxelCenter, int otherVoxelIndex);  


extern std::unordered_map<std::string,Cell_Definition*> cell_definitions_by_name; 
extern std::unordered_map<int,Cell_Definition*> cell_definitions_by_type; 
extern std::vector<Cell_Definition*> cell_definitions_by_index; // works 

void display_cell_definitions( std::ostream& os ); // done 
void build_cell_definitions_maps( void ); // done 

Cell_Definition* find_cell_definition( std::string search_string ); // done 
Cell_Definition* find_cell_definition( int search_type );  

Cell_Definition& get_cell_definition( std::string search_string ); // done 
Cell_Definition& get_cell_definition( int search_type );  

Cell_Definition* initialize_cell_definition_from_pugixml( pugi::xml_node cd_node ); 
void initialize_cell_definitions_from_pugixml( pugi::xml_node root ); 
void initialize_cell_definitions_from_pugixml( void );

extern std::vector<double> (*cell_division_orientation)(void);

void attach_cells( Cell* pCell_1, Cell* pCell_2 );
void detach_cells( Cell* pCell_1 , Cell* pCell_2 );

std::vector<Cell*> find_nearby_cells( Cell* pCell ); // new in 1.8.0
std::vector<Cell*> find_nearby_interacting_cells( Cell* pCell ); // new in 1.8.0

};

#endif