/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based simulator for multicellular systemssimulator, 	#
#	 J. Comput. Biol., 2016 (submitted). 									# 
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based multicellular simulator, J. Comput. Biol., 2016   # 
#   (submitted).                                                            #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#ifndef __PhysiCell_cell_h__
#define __PhysiCell_cell_h__

#include "../custom_modules/PhysiCell_custom.h" 

#include "../BioFVM/BioFVM.h"
#include "./PhysiCell_digital_cell_line.h"
#include "./PhysiCell_cell_container.h"

using namespace BioFVM; 

namespace PhysiCell{
	class Cell_Container;

typedef struct
{
	double o2_necrotic = 5; // mmHg
	//double o2_critical = 0.076; // mmHg
	double o2_critical = 2.5; // mmHg
	
	double o2_no_proliferation = 5; // mmHg
	double o2_hypoxic = 15; // mmHg
	double o2_ref = 6.06; // mmHg
	double o2_saturation = 38; // mmHg
	
	double max_necrosis_rate=1 / (24*60.0); // 1 day^-1

	
	double glucose_necrosis_threshold = 0.1;  // dimensionless 
	double maximum_stretch; 
	double cell_adhesion_parameter; 
	double cell_repulsion_parameter=1;
	double max_interaction_distance_factor = 1.5;

	double Ccca= 0.170577155519015; //used in calculating cell-cell adhesion
	double Cccr= 10.0; //used in calculating cell-cell repulsion			
	double Ccba= 1.70577155519015; //used in calculating cell-basement membrane repulsion	
	double Ccbr= 10.0; //used in calculating cell-basement membrane repulsion	
	
	
} Cell_Parameters; 

class Cell : public Basic_Agent 
{
	private: 
		Cell_Container * container;
		int current_mechanics_voxel_index;
		int updated_current_mechanics_voxel_index; // keeps the updated voxel index for later adjusting of current voxel index
		
		
	public:
		Custom_Cell_Data custom_data;
		std::vector<Cell*> neighbors; 
		std::vector<double> orientation;   
		std::vector<double> motility;  
		double polarity;            	  // A number between 1 and 0, showing how polarized the cell is
		void add_potentials(Cell*);       // Add repulsive and adhesive forces.
		int get_current_mechanics_voxel_index();
		void turn_off_reactions(double); 		  // Turn off all the reactions of the cell
		
		
		bool is_out_of_domain;
		bool is_moving;
		Phenotype phenotype;			  // current phenotype
		Phenotype base_phenotype;		  // base phenotype assigned to the cell	
		Cell_Parameters parameters;
		Cell* divide( void );
		void die( void );
		void step(double dt);
		Cell();
		bool assign_position(std::vector<double> new_position);
		bool assign_position(double, double, double);
		void set_total_volume(double);
		// mechanics 
		void update_position( double dt ); //
		std::vector<double> displacement; 

		void intialize_functions( void ); 		
		
		void (*advance_cell_current_phase)( Cell* pCell, double); 
		// void (*advance_cell_death)( Cell* pCell, double); 
		void (*set_motility)( Cell* pCell, double ); 
		void (*custom_cell_rule)( Cell* pCell, double); 
		void (*update_volume)( Cell* pCell, double); 
		void (*update_phenotype)( Cell* pCell, double); 
		void (*update_velocity)( Cell* pCell, double); 
		void (*add_cell_basement_membrane_interactions)(Cell* pCell, double);
		double (*distance_to_membrane)(Cell* pCell);
		void (*update_cell_and_death_parameters)(Cell* pCell, double);
		void (*update_phenotype_parameters)(Cell* pCell, double);
		void (*set_orientation)(Cell* pCell);
		void assign_orientation();  // if set_orientaion is defined, uses it to assign the orientation
									// otherwise, it assigns a random orientation to the cell.
		void copy_function_pointers(Cell*);
		void update_voxel_in_container(void);
		void copy_data(Cell *);
		void set_phenotype(Phenotype);
		void set_phenotype(Phenotype, Phenotype);
		void update_radius();
		Cell_Container * get_container();
		
};


Cell* create_cell( void );  
void delete_cell( int ); 
void delete_cell( Cell* ); 
void save_all_cells_to_matlab( std::string filename ); 
void update_volume_default( Cell* pCell, double dt ); 
void update_volume_default_old( Cell* pCell, double dt ); 
void update_velocity_default( Cell* pCell, double dt );
void update_velocity_default( Cell* pCell, double dt );
void add_basement_membrane_interactions_default( Cell* pCell, double dt );
void update_cell_and_death_parameters_O2_based( Cell* pCell, double dt ); 
void update_cell_and_death_parameters_O2_based_density_arrest( Cell* pCell, double dt ); 
void update_cell_and_death_parameters_O2_based_volume_arrest( Cell* pCell, double dt ); 
// void distance_to_membrane_2D(Cell* pCell);
bool is_neighbor_voxel(Cell* pCell, std::vector<double> myVoxelCenter, std::vector<double> otherVoxelCenter, int otherVoxelIndex);  //function to check if a neighbor voxel contains any cell that can interact with me
};
#endif