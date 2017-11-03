/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.1) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.1) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the PhysiCell Project           #
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
#include "PhysiCell_digital_cell_line.h"
#include "PhysiCell_cell_container.h"
#include "PhysiCell_constants.h"

using namespace BioFVM; 

namespace PhysiCell{
	class Cell_Container;

class Cell_Parameters
{
	private:
	
	public:
		// oxygen values (in mmHg) for critical phenotype changes
		double o2_necrotic;
		double o2_critical;
		
		double o2_no_proliferation;
		double o2_hypoxic;
		double o2_reference;
		double o2_saturation;

		// necrosis parameters 
		
		double max_necrosis_rate;
		int necrosis_type;
		
		// double maximum_stretch; // not used 
		// cell_adhesion_parameter; // not used 
		// cell_repulsion_parameter=1; // not used 
		
		// mechanics parameters 

		double Ccca; //used in calculating cell-cell adhesion
		double Cccr; //used in calculating cell-cell repulsion
		double Ccba; //used in calculating cell-basement membrane repulsion	
		double Ccbr; //used in calculating cell-basement membrane repulsion	

		double max_interaction_distance_factor;
		
		double diffusion_dt; // reaction-diffusion time step
		double mechanics_dt; // mechanics time step 
		double cell_cycle_dt; // phenotype time step 
		
		Cell_Parameters(); 
}; 

class Cell_Defaults
{
	private:
	public: 
		Microenvironment* pMicroenvironment; 
		Cell_Parameters parameters; 
		
		bool defaults_set; 
		
		void (*advance_cell_current_phase)( Cell* pCell, double); 
		void (*set_motility)( Cell* pCell, double ); 
		void (*custom_cell_rule)( Cell* pCell, double); 
		
		void (*update_volume)( Cell* pCell, double); 
		void (*update_phenotype)( Cell* pCell, double); // never used 
		void (*update_velocity)( Cell* pCell, double); 
		
		void (*add_cell_basement_membrane_interactions)(Cell* pCell, double);
		double (*distance_to_membrane)(Cell* pCell);
		
		void (*update_cell_and_death_parameters)(Cell* pCell, double); // hidden in container 
		void (*update_phenotype_parameters)(Cell* pCell, double); // never used 
		void (*set_orientation)(Cell* pCell, double);

		Cell_Defaults(); 
};

extern Cell_Defaults cell_defaults; 

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
		void set_previous_velocity(double xV, double yV, double zV);
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
		
		void (*advance_cell_current_phase)( Cell* pCell, double); 
		void (*set_motility)( Cell* pCell, double ); 
		void (*custom_cell_rule)( Cell* pCell, double); 
		void (*update_volume)( Cell* pCell, double); 
		void (*update_phenotype)( Cell* pCell, double); 
		void (*update_velocity)( Cell* pCell, double); 
		void (*add_cell_basement_membrane_interactions)(Cell* pCell, double);
		double (*distance_to_membrane)(Cell* pCell);
		void (*update_cell_and_death_parameters)(Cell* pCell, double);
		void (*update_phenotype_parameters)(Cell* pCell, double);
		void (*set_orientation)(Cell* pCell, double);
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
void add_basement_membrane_interactions_default( Cell* pCell, double dt );

// these belong in standard models 

void update_cell_and_death_parameters_O2_based( Cell* pCell, double dt ); 
void update_cell_and_death_parameters_O2_based_density_arrest( Cell* pCell, double dt ); 
void update_cell_and_death_parameters_O2_based_volume_arrest( Cell* pCell, double dt ); 
void set_3D_random_motility( Cell* pCell, double dt );
// void distance_to_membrane_2D(Cell* pCell);
bool is_neighbor_voxel(Cell* pCell, std::vector<double> myVoxelCenter, std::vector<double> otherVoxelCenter, int otherVoxelIndex);  //function to check if a neighbor voxel contains any cell that can interact with me
};
#endif