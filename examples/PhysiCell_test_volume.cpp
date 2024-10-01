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

int omp_num_threads = 1; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)

double o2_conc=5.01;

int main( int argc, char* argv[] )
{
	double t = 0.0; 
	double dt;
	char test_type[3];
	// -A or -a for apoptotic, -N of -n for necrotic, and -c for live cells; the command should be like "program_name dt -x" where x is from (c,n,a)
	if(argc>=2)
	{
		dt = strtod(argv[1], NULL);
		if(argc==3)
			strcpy(test_type, argv[2]);
		else
			strcpy(test_type, "-c");
	}
	else
	{
		dt=0.1;
		strcpy(test_type, "-c");
	}
	std::cout<<test_type<<", "<<dt<<std::endl;
	double t_output_interval = 60.0;
	
	// set t_max to a larger value if you want to check volume change in necrotic phase
	double t_max = 1201;
	double t_next_output_time = 0; 
	int next_output_index = 0; 
	
	double dx; 
	double dy;  
	double dz; 
	
	// openmp setup
	omp_set_num_threads(omp_num_threads);
	
	// PNRG setup 	
	SeedRandom(-35); 
	
	// figure out the bounding box 
	std::vector<double> bounding_box( 6, 0.0 );
	bounding_box[PhysiCell_constants::mesh_min_x_index] = 0; bounding_box[PhysiCell_constants::mesh_max_x_index] = 2000; 
	bounding_box[PhysiCell_constants::mesh_min_y_index] = 0; bounding_box[PhysiCell_constants::mesh_max_y_index] = 2000; 
	bounding_box[PhysiCell_constants::mesh_min_z_index] = 0; bounding_box[PhysiCell_constants::mesh_max_z_index] = 2000; 
	dx=20; dy=20; dz=20;
	
	// create a microenvironment
	BioFVM::Microenvironment microenvironment;
	microenvironment.name="substrate scale";
	// add a microenvironment for simulating substrates 	
	microenvironment.set_density(0, "oxygen" , "mmHg" );
	
	std::cout << bounding_box << std::endl; 
	
	microenvironment.resize_space( bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5] ,dx,dy,dz );
	microenvironment.spatial_units = "microns";
	microenvironment.time_units = "minutes";
	microenvironment.mesh.units = "microns";
	// Cell_Container * 
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
	//cell_defaults.functions.volume_update_function = standard_volume_update_function;
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
	
	microenvironment.display_information( std::cout );
	std::vector<double> point1(3);
	
	double a1=500;
	point1[0]= a1; point1[1]= a1; point1[2]= a1; 
	
	Cell* pCell1 = create_cell();
	pCell1->register_microenvironment(&microenvironment);
	pCell1->assign_position(point1);
	if(test_type[1]=='A' || test_type[1]=='a')
	{
		pCell1->phenotype.cycle.data.current_phase_index = A_index; 
		pCell1->phenotype.death.trigger_death( apoptosis_model_index );
		pCell1->phenotype.cycle.sync_to_cycle_model( pCell1->phenotype.death.current_model() );
	}
	else if(test_type[1]=='N' || test_type[1]=='n')
	{
		pCell1->phenotype.cycle.data.current_phase_index = N_index; 
		pCell1->phenotype.death.trigger_death( necrosis_model_index );
		pCell1->phenotype.cycle.sync_to_cycle_model( pCell1->phenotype.death.current_model() );
	}
	else if(test_type[1]=='C' || test_type[1]=='c')
	{
		pCell1->phenotype.cycle.data.current_phase_index = K1_index; 
		// disable apoptosis
		cell_defaults.phenotype.death.rates[apoptosis_model_index] =0;
		// set Q duration to a large value
		cell_defaults.phenotype.cycle.data.transition_rate(Q_index,K1_index) = 1e9;
		
	}

	pCell1->phenotype.cycle.current_phase().entry_function( pCell1, pCell1->phenotype, dt);

	
	
	
	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
		
	std::cout << (*all_cells).size() <<" agent(s) created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	std::ofstream vol_report ("vol_report.txt", std::ofstream::out);
	try 
	{		
		while( t < t_max )
		{
			if( t > t_next_output_time - 0.5 * dt )
			{
				vol_report<<t<<"\t"<<pCell1->get_total_volume()<<"\t"<<pCell1->phenotype.volume.fluid<<"\t"<<pCell1->phenotype.volume.nuclear_solid<<"\t"<<pCell1->phenotype.volume.cytoplasmic_solid<<"\n";
				t_next_output_time += t_output_interval;
			}
			if((*all_cells).size()>1)
			{
				delete_cell((Cell*)all_basic_agents[1]);
				std::cout << "cell deleted " << std::endl;
			}
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt, dt);
			t += dt; 
		}

		std::cout << "total number of agents: " << (*all_cells).size()<<std::endl << std::endl;
		BioFVM::RUNTIME_TOC();
		BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() );
		vol_report.close();
		std::cout<<"\nTotal volume: " << pCell1->get_total_volume()<<std::endl;
	}
	
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
