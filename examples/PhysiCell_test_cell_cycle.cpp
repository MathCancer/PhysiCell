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
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>

#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM;
using namespace PhysiCell;


int omp_num_threads = 8; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)
double pi=3.1415926535897932384626433832795;

double o2_conc=6.06;
double min_voxel_size=30;

int write_test_report(std::vector<Cell*> all_cells, double timepoint)
{
    std::string filename; 
    filename.resize( 1024 ); 
    sprintf( (char*) filename.c_str() , "test_cycle\\cells_%08i.txt" , (int)round(timepoint) ); 
    std::ofstream outputFile (filename.c_str(), std::ofstream::out);
    //outputFile<<"\tID\tx\ty\tz\tradius\tphenotype\telapsed_time\n";
    int phenotype_code;
    for(int i=0;i<all_cells.size();i++)
    {
        phenotype_code=all_cells[i]->phenotype.cycle.current_phase().code;
        outputFile<<i<<"\t"<<all_cells[i]->ID<<"\t"<<all_cells[i]->position[0]<<"\t" << all_cells[i]->position[1] <<"\t"<< all_cells[i]->position[2]<<"\t";
        outputFile<<all_cells[i]->phenotype.geometry.radius<<"\t"<<phenotype_code<< "\t"<< all_cells[i]->
        phenotype.cycle.data.elapsed_time_in_phase <<std::endl;
         
        // std::cout<<"Cell ID #"<<all_cells[i]->ID<<" position: "<< all_cells[i]->position<<std::endl;
    }
    outputFile.close();
    return 0;
}

int main( int argc, char* argv[] )
{
	if(argc<=1)
	{
		std::cout<<"you need to provide dt as an argument"<<std::endl;
		return 0;
	}
	double t = 0.0; 
	double dt = strtod(argv[1], NULL);
	std::cout<<dt<<std::endl;
	
	double t_output_interval = std::max(60.0,dt); // 1.0; 
	double t_max = 60*24*6;
	double t_next_output_time = 0; 
	int next_output_index = 0; 
	
	double dx; 
	double dy;  
	double dz; 
	
	// openmp setup
	omp_set_num_threads(omp_num_threads);
	
	// PNRG setup 	
	SeedRandom(); 
	
	// figure out the bounding box 
	double side_length= 2000;
	std::vector<double> bounding_box( 6, 0.0 );
	bounding_box[PhysiCell_constants::mesh_min_x_index] = 0; bounding_box[PhysiCell_constants::mesh_max_x_index] = side_length; 
	bounding_box[PhysiCell_constants::mesh_min_y_index] = 0; bounding_box[PhysiCell_constants::mesh_max_y_index] = side_length; 
	bounding_box[PhysiCell_constants::mesh_min_z_index] = 0; bounding_box[PhysiCell_constants::mesh_max_z_index] = side_length; 
	dx=20; dy=20; dz=20;
		
	// create a microenvironment; 
	BioFVM::Microenvironment microenvironment;
	microenvironment.name="substrate scale";
	// add a microenvironment for simulating substrates 	
	microenvironment.set_density(0, "oxygen" , "mmHg" );
	
	std::cout << bounding_box << std::endl; 
	
	microenvironment.resize_space( bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5] ,dx,dy,dz );
	microenvironment.spatial_units = "microns";
	microenvironment.time_units = "minutes";
	microenvironment.mesh.units = "microns";
	// Cell_Container 
	double mechanics_voxel_size = 30; 
	Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );
	
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		microenvironment.density_vector(n)[0] = o2_conc; 	
	}
	
	// register the diffusion solver 
	microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D; 
	
	initialize_default_cell_definition(); 	
	cell_defaults.type = 0; 
	cell_defaults.name = "tumor cell"; 
	// set default cell cycle model 
	cell_defaults.functions.cycle_model = Ki67_advanced; 	
	// set default_cell_functions; 
	cell_defaults.functions.update_phenotype = update_cell_and_death_parameters_O2_based; 
	
	// disable cell's movement
	cell_defaults.functions.update_velocity=empty_function;
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions ); 
	// first find index for a few key variables. 
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Necrosis" );
	int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" ); 

	int K1_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_premitotic );
	int K2_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_postmitotic );
	int Q_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_negative );
	int A_index = Ki67_advanced.find_phase_index( PhysiCell_constants::apoptotic );
	int N_index = Ki67_advanced.find_phase_index( PhysiCell_constants::necrotic_swelling );
	
		// cells apoptose after about 7 days 
	cell_defaults.phenotype.death.rates[apoptosis_model_index] = 1.0 / (7.0 * 24.0 * 60.0); 
	// initially no necrosis 
	cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 

	// make sure the cells uptake oxygen at the right rate 
	cell_defaults.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 0; 

	// cells leave the Q phase and enter the K1 phase after 5 hours 
	cell_defaults.phenotype.cycle.data.transition_rate(Q_index,K1_index) = 1.0 / ( 5.0 * 60.0 ); 
	
	// let's make necrotic cells survive 6 hours in minimal oxygen conditions  
	cell_defaults.parameters.max_necrosis_rate = 1.0 / (6.0 * 60.0); 
	

	microenvironment.display_information( std::cout );

	std::vector<double> temp_position(3);
	
	int num_ki67_positive_pre= 0;
	int num_ki67_positive_post= 0;
	int num_ki67_q=1000;//;
	int num_apoptotic= 0;
	int total= num_ki67_positive_pre + num_ki67_positive_post + num_ki67_q + num_apoptotic;
	double T1= 13 * 60;
	double T2= 2.5 * 60;
	double TQ= 74.35 * 60;
	double TA= 8.6 * 60;
	double T;
	int phase_index;
	for(int i=0;i<total;i++)
	{
		
		for(int j=0;j<3;j++)
			temp_position[j]= uniform_random()*1000+500;
		Cell* pCell = create_cell();
		pCell->register_microenvironment(&microenvironment);
		pCell->assign_position(temp_position);
		//pCell->advance_cell_current_phase=ki67_advanced_cycle_model_stochastic;
		if(i<num_ki67_positive_pre)
		{
			phase_index=K1_index;
			T = T1;
		}
		else if(i< num_ki67_positive_pre+ num_ki67_positive_post)
		{
			phase_index = K2_index;	
			T = T2;			
		}
		else if(i< num_ki67_positive_pre+ num_ki67_positive_post+ num_apoptotic)
		{
			phase_index = A_index;	
			T = TA;	
			pCell->phenotype.death.trigger_death( apoptosis_model_index );
			pCell->phenotype.cycle.sync_to_cycle_model( pCell->phenotype.death.current_model() );
		}
		else
		{
			phase_index = Q_index;	
			T = TQ;			
		}
		pCell->phenotype.cycle.data.current_phase_index = phase_index; 
		if( pCell->phenotype.cycle.current_phase().entry_function )                      
			pCell->phenotype.cycle.current_phase().entry_function( pCell, pCell->phenotype, dt);
		// std::cout<<__FILE__<<","<<__LINE__<<std::endl;

	}
	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
		
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	int output_index =0; 
	// std::ofstream report_file ("report.txt");
	// report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time"<<std::endl;
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();

	try 
	{		
		while( t < t_max )
		{
			if( t > t_next_output_time - 0.5 * dt )
			{
				write_test_report(*all_cells,t);
				t_next_output_time += t_output_interval;	
				std::cout << "t: "<<t<<", cell count: "<<(*all_cells).size()<<std::endl;
			}
	
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt, dt);
			t += dt; 
			output_index++;
		}
		write_test_report(*all_cells,t);
		// report_file.close();
	}
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
