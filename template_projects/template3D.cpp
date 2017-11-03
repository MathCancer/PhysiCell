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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>

// #include "./BioFVM/BioFVM.h" 
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
	double dt = 0.01; // reaction-diffusion time step
	double mechanics_dt = 0.1; // mechanics time step 
	double cell_cycle_dt = 6; // phenotype time step 
	
	double t_output_interval = 60.0; 
	double t_max = 60*24*10; // 10 days 
	double t_next_output_time = t; 
	int output_index = 0; // used for creating unique output filenames 

	/* Microenvironment setup */ 
	
	// microenvironment discretization 
	std::string spatial_units = "micron"; 
	double dx = 20; 
	double dy = 20;  
	double dz = 20; 

	// create and name a microenvironment; 
	BioFVM::Microenvironment microenvironment;
	microenvironment.name = "microenvironment";
	// register the diffusion solver 
	microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D; 
	
	// set the default substrate to oxygen (with typical units of mmHg)
	microenvironment.set_density(0, "oxygen" , "mmHg" );
	microenvironment.diffusion_coefficients[0] = 1e5; 
	microenvironment.decay_rates[0] = 0.1; 
	
	// resize the microenvironment to 1 mm^3, centered at (0,0,0)
	microenvironment.resize_space( -500.0, 500.0, -500.0, 500.0, -500,500, dx,dy,dz ); 
	
	// set units
	microenvironment.spatial_units = spatial_units;
	microenvironment.time_units = time_units;
	microenvironment.mesh.units = spatial_units;

	// set the initial oxygenation to 38 mmHg (a typical normoxic tissue value of 5% O2)
	double far_field_O2_value = 38; 
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{ microenvironment.density_vector(n)[0] = far_field_O2_value; }
	
	// set Dirichlet conditions along the 6 outer faces 
	std::vector<double> dirichlet_condition_vector( 1, far_field_O2_value ); 
	for( int i=0 ; i < microenvironment.mesh.x_coordinates.size() ; i++ )
	{
		int k = microenvironment.mesh.z_coordinates.size()-1;
		for( int j=0; j < microenvironment.mesh.y_coordinates.size() ; j++ )
		{
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(i,j,0) , dirichlet_condition_vector );
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(i,j,k) , dirichlet_condition_vector );
		}		
	}
	for( int i=0 ; i < microenvironment.mesh.x_coordinates.size() ; i++ )
	{
		int j = microenvironment.mesh.y_coordinates.size()-1;
		for( int k=1; k < microenvironment.mesh.z_coordinates.size()-1 ; k++ )
		{
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(i,0,k) , dirichlet_condition_vector );
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(i,j,k) , dirichlet_condition_vector );
		}		
	}
	for( int k=1 ; k < microenvironment.mesh.z_coordinates.size()-1 ; k++ )
	{
		int i = microenvironment.mesh.x_coordinates.size()-1;
		for( int j=1; j < microenvironment.mesh.y_coordinates.size()-1 ; j++ )
		{
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(0,j,k) , dirichlet_condition_vector );
			microenvironment.add_dirichlet_node( microenvironment.voxel_index(i,j,k) , dirichlet_condition_vector );
		}		
	}
	
	microenvironment.display_information( std::cout );
	
	/* PhysiCell setup */ 
 	
	// prepare PhysiCell mechanics data structures 

	// mechanics voxel size 
	double mechanics_voxel_size = 30; 
	Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );
	
	// Set up default cell models/functions and parameters 
	
	cell_defaults.update_cell_and_death_parameters = update_cell_and_death_parameters_O2_based;
	cell_defaults.advance_cell_current_phase = ki67_advanced_cycle_model_stochastic;
	cell_defaults.parameters.diffusion_dt = dt;
	cell_defaults.parameters.mechanics_dt = mechanics_dt;
	cell_defaults.parameters.cell_cycle_dt = cell_cycle_dt; 
	
	// create a Cancer cell line. Later templates will not require this. 

	Cell_Line cancer; 
	set_cancer_cell_line_MCF7( cancer ); 
	cancer.phenotypes[0].secretion_rates.rates.resize( microenvironment.number_of_densities() , 0.0 );
	cancer.phenotypes[0].uptake_rates.rates.resize( microenvironment.number_of_densities() , 0.0 ) ;
	cancer.phenotypes[0].saturation_densities.densities.resize( microenvironment.number_of_densities() , 0.0 );
	cancer.phenotypes[0].uptake_rates.rates[0] = 10; 

	// display information on the cell line 

	cancer.display_information( std::cout );
	
	// create some cells near the origin, and place them in various phases
	// of the Ki67 advanced model 
	
	Cell* pC;

	pC = create_cell(); 
	pC->assign_position( 1.0, 2.0, 0.0 );
	pC->set_phenotype(cancer.phenotypes[0]); 
	pC->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_premitotic );
	pC->type= PhysiCell_constants::TUMOR_TYPE; 
	
	pC = create_cell(); 
	pC->assign_position( -4.0, 5.0, 3.7 );
	pC->set_phenotype(cancer.phenotypes[0]); 
	pC->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_premitotic );
	pC->type= PhysiCell_constants::TUMOR_TYPE; 
	
	// set MultiCellDS save options 

	set_save_biofvm_mesh_as_matlab( true ); 
	set_save_biofvm_data_as_matlab( true ); 
	set_save_biofvm_cell_data( true ); 
	set_save_biofvm_cell_data_as_custom_matlab( true );
	
	// save a simulation snapshot 
	
	save_PhysiCell_to_MultiCellDS_xml_pugi( "initial" , microenvironment , t ); 
	
	// save a quick SVG cross section through z = 0, after setting its 
	// length bar to 250 microns 
	
	PhysiCell_SVG_options.length_bar = 250; 
	
	// for simplicity, set a pathology coloring function 
	
	std::vector<std::string> (*cell_coloring_function)(Cell*) = false_cell_coloring_Ki67;

	TIC();
	SVG_plot( "initial.svg" , microenvironment, 0.0 , t, cell_coloring_function );
	TOC(); 
	display_stopwatch_value( std::cout , stopwatch_value() ); 
	
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
			if(  fabs( t - t_next_output_time ) < 0.01 * dt )
			{
				log_output(t, output_index, microenvironment, report_file);
				t_next_output_time += t_output_interval;
				
				char filename[1024]; 
				sprintf( filename , "output%08u" , output_index ); 
				
				save_PhysiCell_to_MultiCellDS_xml_pugi( filename , microenvironment , t ); 
				
				sprintf( filename , "snapshot%08u.svg" , output_index ); 
				SVG_plot( filename , microenvironment, 0.0 , t, cell_coloring_function );
				
				output_index++; 
			}
			// update the microenvironment
			microenvironment.simulate_cell_sources_and_sinks( dt );
			microenvironment.simulate_diffusion_decay( dt );
			
			// run PhysiCell 
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, cell_cycle_dt, mechanics_dt);		
			
			t += dt; 
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
	
	return 0; 
}
