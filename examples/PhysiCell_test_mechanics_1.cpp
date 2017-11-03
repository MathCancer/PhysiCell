/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>


#include "../core/PhysiCell.h"
#include "../BioFVM/BioFVM.h" 

using namespace BioFVM;
using namespace PhysiCell;


Cell_Container *cell_container; 
 
int omp_num_threads = 8; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)
double pi=3.1415926535897932384626433832795;

double o2_conc=38.0;
double min_voxel_size=30;

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
	
	int number_of_voxels = ((bounding_box[PhysiCell_constants::mesh_max_x_index]-bounding_box[PhysiCell_constants::mesh_min_x_index])/dx) * ((bounding_box[PhysiCell_constants::mesh_max_y_index]-bounding_box[PhysiCell_constants::mesh_min_y_index])/dy) * ((bounding_box[PhysiCell_constants::mesh_max_z_index]-bounding_box[PhysiCell_constants::mesh_min_z_index])/dz);
	
	// create a Multiscale_Microenvironment; 
	//microenvironment; 
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
	// Cell_Container * 
	cell_container = new Cell_Container;
	cell_container->initialize(bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5], min_voxel_size );
	microenvironment.agent_container= (Agent_Container *) cell_container; // cell_container;
	// microenvironment[0].agent_container->initialize(number_of_voxels);
	
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		microenvironment.density_vector(n)[0] = o2_conc; 	
	}
	
	// register the diffusion solver 
	microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D; 
	
	Cell_Line cancer; 
	// Cell_Line endothelial; 
	set_cancer_cell_line_MCF7( cancer ); 
	// set_endothelial_cell_line( endothelial ); 
	cancer.display_information( std::cout );
		
	microenvironment.display_information( std::cout );

	
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<std::vector<double>> cell_positions;
	std::vector<double> point1(3), point2(3);

	double a1, a2;
	a1=100;
	a2= 105.73;
	double volume=4188.790204786391;
	point1[0]= a1; point1[1]= a1; point1[2]= a1; 
	point2[0]= a2; point2[1]= a2; point2[2]= a2; 
	
	
	Cell* pCell1 = create_cell();
	pCell1->register_microenvironment(&microenvironment);
	pCell1->assign_position(point1);
	pCell1->advance_cell_current_phase=do_nothing;
	pCell1->set_phenotype(cancer.phenotypes[0]);
	pCell1->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative);
	//NOTE: for this experiment, you need to disable update_volume function 
	// to make sure that volume change are not affecting the distance we measure for the cells.
	pCell1->update_volume=do_nothing;
	pCell1->set_total_volume(volume);
	pCell1->type=PhysiCell_constants::TUMOR_TYPE;
	
	Cell* pCell2 = create_cell();
	pCell2->register_microenvironment(&microenvironment);
	pCell2->assign_position(point2);
	pCell2->advance_cell_current_phase=do_nothing;
	pCell2->update_volume=do_nothing;
	pCell2->set_phenotype(cancer.phenotypes[0]);
	pCell2->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative);
	pCell2->set_total_volume(volume);
	pCell2->type=PhysiCell_constants::TUMOR_TYPE;
	
	
	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
	
	std::cout << pCell1->phenotype.geometry.radius<<std::endl;
	
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	
	std::string filename; 
	filename.resize( 1024 ); 
	sprintf( (char*) filename.c_str() , "mechanics_test1_result_dt=%f.txt" , dt ); 
	std::ofstream report_file (filename.c_str(), std::ofstream::out);
	
	pCell1->update_velocity(pCell1, dt);
	pCell2->update_velocity(pCell2, dt);
	
	pCell1->set_previous_velocity(pCell1->velocity[0],pCell1->velocity[1],pCell1->velocity[2]);
	pCell2->set_previous_velocity(pCell2->velocity[0],pCell2->velocity[1],pCell2->velocity[2]);
	
	std::cout<<pCell1->velocity<<std::endl;
	std::cout<<pCell2->velocity<<std::endl;
	
	for(int i=0;i<10;i++)
	{
		pCell1->position += (dt/10.0)*pCell1->velocity; 
		pCell2->position += (dt/10.0)*pCell2->velocity;
		t+=dt/10.0;
	}
	std::cout<<"time: "<< t<<std::endl;
	t_next_output_time=t_output_interval;
	try 
	{		
		while( t < t_max )
		{
			
			if(  fabs( t - t_next_output_time ) < dt/10.0 )
			{
				report_file<<t<<"\t"<<dist(pCell1->position,pCell2->position)<<"\n";
				t_next_output_time += t_output_interval; 
			}
			
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt);
			t += dt; 
		}
		report_file.close();
		std::cout<<pCell1->position<<"  "<< pCell2->position<< ", dist: " <<dist(pCell1->position,pCell2->position)<<  std::endl;
		double scale=1000;
		//writePov(*all_cells, t_max, scale);
		//writeCellReport(*all_cells, t_max);
		std::cout << "total number of agents: " << (*all_cells).size()<<std::endl << std::endl;
		BioFVM::RUNTIME_TOC();
		BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() );
	}
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
