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


void do_nothing(Cell* pCell, Phenotype& phenotype, double dt){}

int main( int argc, char* argv[] )
{
	if(argc<=1)
	{
		std::cout<<"you need to provide dt as an argument"<<std::endl;
		return 0;
	}

	double t = 0.0; 
	double dt = strtod(argv[1],NULL);
	double t_output_interval = 1; // 1.0; 
	double t_max = 60;
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
	bounding_box[PhysiCell_constants::mesh_min_x_index] = 0; bounding_box[PhysiCell_constants::mesh_max_x_index] = 2000; 
	bounding_box[PhysiCell_constants::mesh_min_y_index] = 0; bounding_box[PhysiCell_constants::mesh_max_y_index] = 2000; 
	bounding_box[PhysiCell_constants::mesh_min_z_index] = 0; bounding_box[PhysiCell_constants::mesh_max_z_index] = 2000; 
	dx=20; dy=20; dz=20;
	
	// create a microenvironment;
	BioFVM::Microenvironment microenvironment;
	microenvironment.name="substrate scale";
	// add a microenvironment for simulating substrates 	
	microenvironment.set_density(0, "oxygen" , "mmHg" );
	microenvironment.resize_space( bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5] ,dx,dy,dz );
	microenvironment.spatial_units = "microns";
	microenvironment.time_units = "minutes";
	microenvironment.mesh.units = "microns";
	// Since this is just a test for mechanics, the following density initialization are not needed
	/* for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		microenvironment.density_vector(n)[0] = o2_conc; 	
	}*/
	
	// Cell_Container
	double mechanics_voxel_size = 30; 
	Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );
	//microenvironment.agent_container= (Agent_Container *) cell_container; // cell_container;
	
	initialize_default_cell_definition(); 	
	cell_defaults.type = 0; 
	cell_defaults.name = "tumor cell"; 
	cell_defaults.functions.cycle_model = Ki67_advanced; 
	cell_defaults.functions.update_phenotype = empty_function;
	cell_defaults.functions.volume_update_function = empty_function;
	
	
	int Q_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_negative );
	// register the diffusion solver 
	microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D; 
		
	microenvironment.display_information( std::cout );

	
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<double> point1(3), point2(3);

	double num1, num2;
	num1=100;
	num2= 105.73;
	double volume=4188.790204786391;
	point1[0]= num1; point1[1]= num1; point1[2]= num1; 
	point2[0]= num2; point2[1]= num2; point2[2]= num2; 
	
	Cell* pCell1 = create_cell();
	pCell1->register_microenvironment(&microenvironment);
	pCell1->assign_position(point1);
	pCell1->phenotype.cycle.data.current_phase_index = Q_index; 
	/* NOTE: for this experiment, you need to disable volume update function 
	 to make sure that volume change are not affecting the distance we measure for the cells.*/
	//pCell1->functions.volume_update_function=do_nothing;
	pCell1->set_total_volume(volume);
	
	Cell* pCell2 = create_cell();
	 pCell2->register_microenvironment(&microenvironment);
	pCell2->assign_position(point2);
	pCell2->phenotype.cycle.data.current_phase_index = Q_index; 
	//pCell2->functions.volume_update_function=do_nothing;
	pCell2->set_total_volume(volume);
	
	std::cout << pCell1->phenotype.geometry.radius<<std::endl;
	
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	
	std::string filename; 
	filename.resize( 1024 ); 
	sprintf( (char*) filename.c_str() , "mechanics_test1_result_dt=%f.txt" , dt ); 
	std::ofstream report_file (filename.c_str(), std::ofstream::out);
	
	pCell1->functions.update_velocity(pCell1,pCell1->phenotype, dt);
	pCell2->functions.update_velocity(pCell2,pCell2->phenotype, dt);
	
	pCell1->set_previous_velocity(pCell1->velocity[0],pCell1->velocity[1],pCell1->velocity[2]);
	pCell2->set_previous_velocity(pCell2->velocity[0],pCell2->velocity[1],pCell2->velocity[2]);
		
	for(int i=0;i<10;i++)
	{
		pCell1->position += (dt/10.0)*pCell1->velocity; 
		pCell2->position += (dt/10.0)*pCell2->velocity;
		t+=dt/10.0;
	}
	std::cout<<"time: "<< t<<std::endl;
	t_next_output_time=t_output_interval;
	std::cout<<pCell1->get_total_volume()<<std::endl;
	try 
	{		
		while( t < t_max )
		{
			
			if( t > t_next_output_time - 0.5 * dt )
			{
				report_file<<t<<"\t"<<dist(pCell1->position,pCell2->position)<<"\n";
				t_next_output_time += t_output_interval; 
			}
			
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt, dt);
			t += dt; 
		}
		report_file.close();
		std::cout<<pCell1->position<<"  "<< pCell2->position<< ", distance: " <<dist(pCell1->position,pCell2->position)<<  std::endl;
		
		std::cout<<pCell1->get_total_volume()<<std::endl;
		std::cout << "total number of agents: " << (*all_cells).size()<<std::endl << std::endl;
		BioFVM::RUNTIME_TOC();
		BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() );
	}
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
