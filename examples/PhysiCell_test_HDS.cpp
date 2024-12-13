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

double o2_conc=38.0;   //check this value to make sure it has support from literature

std::vector<std::vector<double>> create_sphere(double cell_radius, double sphere_radius)
{
	std::vector<std::vector<double>> cells;
	int xc=0,yc=0,zc=0;
	double x_spacing= cell_radius*sqrt(3);
	double y_spacing= cell_radius*2;
	double z_spacing= cell_radius*sqrt(3);
	
	std::vector<double> tempPoint(3,0.0);
	// std::vector<double> cylinder_center(3,0.0);
	
	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0]=x + (zc%2) * 0.5 * cell_radius;
				tempPoint[1]=y + (xc%2) * cell_radius;
				tempPoint[2]=z;
				
				if(sqrt(norm_squared(tempPoint))< sphere_radius)
				{
					cells.push_back(tempPoint);
				}
			}
	return cells;
	
}

int main( int argc, char* argv[] )
{
	bool DEBUG=false;
	double t = 0.0; 
	double dt = 0.01; // reaction-diffusion time-step
	double mechanics_dt= 0.1;
	double cell_cycle_dt= 6;
	
	double t_output_interval = 600.0; // 1.0; 
	double t_max = 365*24*60;
	double t_next_output_time = 0; 
	int next_output_index = 0; 
	
	double dx; 
	double dy;  
	double dz; 
	
	// openmp setup
	omp_set_num_threads(omp_num_threads);
	
	// PNRG setup 	
	SeedRandom(3); 
	
	// figure out the bounding box 
	std::vector<double> bounding_box( 6, 0.0 );
	bounding_box[PhysiCell_constants::mesh_min_x_index] = -1000; bounding_box[PhysiCell_constants::mesh_max_x_index] = 1000; 
	bounding_box[PhysiCell_constants::mesh_min_y_index] = -1000; bounding_box[PhysiCell_constants::mesh_max_y_index] = 1000; 
	bounding_box[PhysiCell_constants::mesh_min_z_index] = -1000; bounding_box[PhysiCell_constants::mesh_max_z_index] = 1000; 
	dx=20; dy=20; dz=20;
	

	// create a microenvironment
	BioFVM::Microenvironment microenvironment;
	microenvironment.name="substrate scale";
	// add a microenvironment for simulating substrates 	
	// microenvironment.create_microenvironment( "substrate scale"); 
	microenvironment.set_density(0, "oxygen" , "mmHg" );
	// microenvironment.add_density( "glucose" , "dimensionless" );
	
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
	
	// register substrates properties 
	microenvironment.diffusion_coefficients[0] = 1.0e5; // microns^2 / min 
	microenvironment.decay_rates[0] = 0.1;

	microenvironment.display_information( std::cout );

	
	initialize_default_cell_definition(); 	
	cell_defaults.type = 0; 
	cell_defaults.name = "tumor cell"; 
	// set default cell cycle model 
	cell_defaults.functions.cycle_model = Ki67_advanced; 	
	// set default_cell_functions; 
	cell_defaults.functions.update_phenotype = update_cell_and_death_parameters_O2_based; 
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions );
	
	int Q_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_negative );
	int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" ); 
	int K1_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_premitotic );
	int K2_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_positive_postmitotic );
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "apoptosis" );
	int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "necrosis" );
	// cells apoptose after about 7 days 
	cell_defaults.phenotype.death.rates[apoptosis_model_index] = 1.0 / (7.0 * 24.0 * 60.0); 
	// initially no necrosis 
	cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 

	// make sure the cells uptake oxygen at the right rate 
	cell_defaults.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 10; 

	// update transition times 
	cell_defaults.phenotype.cycle.data.transition_rate(Q_index,K1_index) = 1.0 / ( 8.5 * 60.0 ); 
	cell_defaults.phenotype.cycle.data.transition_rate(K1_index,K2_index) = 1.0 / ( 13.0 * 60.0 ); 
	cell_defaults.phenotype.cycle.data.transition_rate(K2_index,Q_index) = 1.0 / ( 2.5 * 60.0 ); 
	
	// let's make necrotic cells survive 6 hours in minimal oxygen conditions  
	cell_defaults.parameters.max_necrosis_rate = 1.0 / (6.0 * 60.0); 
	
	double cell_radius=10;
	double sphere_radius = 150;
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<std::vector<double>> cell_positions;
	cell_positions= create_sphere(cell_radius, sphere_radius);

	//add Dirichlet node for all the voxels located outside of the duct
	std::vector<double> dirichlet_o2( 1 , o2_conc );
	
	double min_x=microenvironment.mesh.bounding_box[0];
	double max_x=microenvironment.mesh.bounding_box[3];
	double min_y=microenvironment.mesh.bounding_box[1];
	double max_y=microenvironment.mesh.bounding_box[4];
	double min_z=microenvironment.mesh.bounding_box[2];
	double max_z=microenvironment.mesh.bounding_box[5];
	double strip_width=40;	

	for( int i=0; i < microenvironment.number_of_voxels() ; i++ )
	{
		if( abs(max_x-microenvironment.voxels(i).center[0]) < strip_width || abs(microenvironment.voxels(i).center[0]- min_x)< strip_width  
			|| abs(max_y-microenvironment.voxels(i).center[1]) < strip_width || abs(microenvironment.voxels(i).center[1]- min_y)< strip_width  
				|| abs(max_z-microenvironment.voxels(i).center[2]) < strip_width || abs(microenvironment.voxels(i).center[2]- min_z)< strip_width )
				{
					microenvironment.add_dirichlet_node( i , dirichlet_o2 );
				}		
	}

	
	Cell* pCell;

for(int i=0;i<cell_positions.size();i++)
	// for(int i=0;i<2;i++)
	{
		if(cell_positions[i][0]>0)
			continue;
		pCell = create_cell();
		pCell->register_microenvironment(&microenvironment);
		pCell->assign_position(cell_positions[i]);
		pCell->phenotype.cycle.data.current_phase_index = Q_index; 
		if( pCell->phenotype.cycle.current_phase().entry_function )                      
			pCell->phenotype.cycle.current_phase().entry_function( pCell, pCell->phenotype, dt);
		// pCell->parameters.necrosis_type= PhysiCell_constants::deterministic_necrosis;
	}

	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
	
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	std::vector<double> position (3, 0.0);
	position[0]=0;
	position[1]=0;
	position[2]=0;
	
	int output_index =0; 
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	
	std::ofstream report_file ("report_spheroid.txt");
	report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time"<<std::endl;
	try 
	{		
		while( t < t_max )
		{
			if( t > t_next_output_time - 0.5 * dt )
			{
				log_output(t, output_index, microenvironment, report_file);
				t_next_output_time += t_output_interval;						
			}
			microenvironment.simulate_cell_sources_and_sinks( dt );
			microenvironment.simulate_diffusion_decay( dt );
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, cell_cycle_dt, mechanics_dt, dt);		
			t += dt; 
			output_index++;
		}
		log_output(t, output_index, microenvironment, report_file);
		report_file.close();
	}
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
