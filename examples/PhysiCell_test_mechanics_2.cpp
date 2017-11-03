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
	if(argc<=1)
	{
		std::cout<<"you need to provide dt as an argument"<<std::endl;
		return 0;
	}

	double t = 0.0; 
	double dt = strtod(argv[1],NULL);
	double t_output_interval = 5.0; // 1.0; 
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

	double cell_radius=10;
	double volume=4188.790204786391;
	double sphere_radius = 80;
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<std::vector<double>> cell_positions;
	cell_positions= create_sphere(cell_radius/5, sphere_radius);
	
	std::vector<double> tumor_center(3);
		tumor_center[0]=1000;
		tumor_center[1]=1000;
		tumor_center[2]=1000;
	
	for(int i=0;i<cell_positions.size();i++)
	{
		Cell* pCell = create_cell();
		pCell->register_microenvironment(&microenvironment);
		pCell->assign_position(tumor_center+ cell_positions[i]);
		pCell->advance_cell_current_phase=do_nothing;
		pCell->set_phenotype(cancer.phenotypes[0]);
		pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative);	
		pCell->set_total_volume(cancer.phenotypes[0].volume.total);
		pCell->type=PhysiCell_constants::TUMOR_TYPE;		
		pCell->phenotype.geometry.radius=cell_radius;		
	}
	
	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
		
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	try 
	{		
		while( t < t_max )
		{
			if(  fabs( t - t_next_output_time ) < 0.001 )
			{
				std::cout<<"time: "<<t<<std::endl;
				writeCellReport(*all_cells, t);
				t_next_output_time += t_output_interval;
			}
				
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt);
			t += dt; 
		}
		double scale=1000;
		writeCellReport(*all_cells, t_max);
		std::cout << "total number of agents: " << (*all_cells).size()<<std::endl << std::endl;
		BioFVM::RUNTIME_TOC();
		BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() );
	}
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
