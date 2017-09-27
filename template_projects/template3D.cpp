/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.2.1) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, PLoS Comput. Biol. 2017 (in revision).         #
#     preprint DOI: 10.1101/088773                                          #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.2.1) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, PLoS Comput. Biol. 2017 (in revision).         #
#     preprint DOI: 10.1101/088773                                          #
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>

#include "./core/PhysiCell.h"
#include "./modules/PhysiCell_standard_modules.h" 
	
using namespace BioFVM;
using namespace PhysiCell;

// set number of threads for OpenMP (parallel computing)
int omp_num_threads = 8; // set this to # of CPU cores x 2 (for hyperthreading)

int main( int argc, char* argv[] )
{
	// OpenMP setup
	omp_set_num_threads(omp_num_threads);
	
	// PNRG setup 
	SeedRandom(); 
	
	// time setup 
	std::string time_units = "min"; 
	double t = 0.0; // current simulation time 
	
	double t_output_interval = 60.0; 
	double t_max = 60*24*30; // 30 days 
	double t_next_output_time = t; 
	int output_index = 0; // used for creating unique output filenames 

	/* Microenvironment setup */ 
	
	std::vector<double> bc_vector( 1 , 38.0 ); // 5% o2
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	default_microenvironment_options.outer_Dirichlet_conditions = true;
	
	default_microenvironment_options.X_range = {-750, 750}; 
	default_microenvironment_options.Y_range = {-750, 750}; 	
	default_microenvironment_options.Z_range = {-750, 750}; 	
	
	initialize_microenvironment(); 	
	
	// this example does not use chemical gradients 
	default_microenvironment_options.calculate_gradients = false; 
	
	/* PhysiCell setup */ 
 	
	// set mechanics voxel size, and match the data structure to BioFVM
	double mechanics_voxel_size = 30; 
	Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );
	
	// initialize data structure for cell defaults 
	initialize_default_cell_definition(); 	
	
	/* Users typically start modifying here. START USERMODS */ 
	
	// Set up default cell models/functions and parameters 
	
	cell_defaults.type = 0; 
	cell_defaults.name = "tumor cell"; 
	
	// set default cell cycle model 

	cell_defaults.functions.cycle_model = Ki67_advanced; 
	
	// set default_cell_functions; 
	
	cell_defaults.functions.update_phenotype = update_cell_and_death_parameters_O2_based; 
	
	// make sure the defaults are self-consistent. 
	
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions ); 

	// set the rate terms in the default phenotype 

	// first find index for a few key variables. 
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Necrosis" );
	int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" ); 

	int K1_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_premitotic );
	int K2_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_postmitotic );
	int Q_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_negative );

	// initially no necrosis 
	cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 

	// make sure the cells uptake oxygen at the right rate 
	cell_defaults.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 10; 
	
	// create some cells near the origin
	
	Cell* pC;

	pC = create_cell(); 
	pC->assign_position( 0.0, 0.0, 0.0 );
	pC->phenotype.cycle.data.current_phase_index = Q_index; 

	pC = create_cell(); 
	pC->assign_position( -2.0, -6.0, 1.0 );
	pC->phenotype.cycle.data.current_phase_index = Q_index; 
	
	pC = create_cell(); 
	pC->assign_position( -5.0, 8.0, -7.0 );
	pC->phenotype.cycle.data.current_phase_index = Q_index; 
	
	pC = create_cell(); 
	pC->assign_position( 5.0, -8.0, 3.0 );
	pC->phenotype.cycle.data.current_phase_index = Q_index; 
	
	// make this last cell randomly motile, less adhesive, greater survival 
	pC->phenotype.motility.is_motile = true; 
	pC->phenotype.motility.persistence_time = 15.0; // 15 minutes
	pC->phenotype.motility.migration_speed = 0.25; // 0.25 micron/minute 
	pC->phenotype.motility.migration_bias = 0.0;// completely random 
	
	pC->phenotype.mechanics.cell_cell_adhesion_strength *= 0.05; 
	
	pC->phenotype.death.rates[apoptosis_model_index] = 0.0; 

	/* Users typically stop modifying here. END USERMODS */ 
	
	// set MultiCellDS save options 

	set_save_biofvm_mesh_as_matlab( true ); 
	set_save_biofvm_data_as_matlab( true ); 
	set_save_biofvm_cell_data( true ); 
	set_save_biofvm_cell_data_as_custom_matlab( true );
	
	// save a simulation snapshot 

	save_PhysiCell_to_MultiCellDS_xml_pugi( "initial" , microenvironment , t ); 
	
	// save a quick SVG cross section through z = 0, after setting its 
	// length bar to 200 microns 

	PhysiCell_SVG_options.length_bar = 200; 

	// for simplicity, set a pathology coloring function 
	
	std::vector<std::string> (*cell_coloring_function)(Cell*) = false_cell_coloring_Ki67;
	
	SVG_plot( "initial.svg" , microenvironment, 0.0 , t, cell_coloring_function );
	
	// set the performance timers 

	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	
	std::ofstream report_file ("simulation_report.txt"); 	// create the data log file 
	report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time"<<std::endl;
	
	// main loop 
	
	try 
	{		
		while( t < t_max )
		{
			// save data if it's time. 
			if(  fabs( t - t_next_output_time ) < 0.01 * diffusion_dt )
			{
				log_output(t, output_index, microenvironment, report_file);
				
				char filename[1024]; 
				sprintf( filename , "output%08u" , output_index ); 
				
				save_PhysiCell_to_MultiCellDS_xml_pugi( filename , microenvironment , t ); 
				
				sprintf( filename , "snapshot%08u.svg" , output_index ); 
				SVG_plot( filename , microenvironment, 0.0 , t, cell_coloring_function );
				
				output_index++; 
				t_next_output_time += t_output_interval;
			}
			// update the microenvironment
			microenvironment.simulate_diffusion_decay( diffusion_dt );
			if( default_microenvironment_options.calculate_gradients )
			{ microenvironment.compute_all_gradient_vectors(); }
			
			// run PhysiCell 
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t);
			
			t += diffusion_dt; 
		}
		log_output(t, output_index, microenvironment, report_file);
		report_file.close();
	}
	catch( const std::exception& e )
	{ // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	
	// save a final simulation snapshot 
	
	save_PhysiCell_to_MultiCellDS_xml_pugi( "final" , microenvironment , t ); 
	SVG_plot( "final.svg" , microenvironment, 0.0 , t, cell_coloring_function );

	// timer 
	
	std::cout << std::endl << "Total simulation runtime: " << std::endl; 
	BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() ); 	
	
	return 0; 
}
