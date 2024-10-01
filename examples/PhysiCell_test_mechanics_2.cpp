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

double min_voxel_size=30;

void do_nothing(Cell* pCell, Phenotype& phenotype, double dt){}

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
	
	microenvironment.display_information( std::cout );

	initialize_default_cell_definition(); 	
	cell_defaults.type = 0; 
	cell_defaults.name = "tumor cell"; 
	cell_defaults.functions.cycle_model = Ki67_advanced; 
	cell_defaults.functions.update_phenotype = empty_function;
	cell_defaults.functions.volume_update_function = empty_function;
	
	int Q_index = Ki67_advanced.find_phase_index( PhysiCell_constants::Ki67_negative );
	

	double sample_cell_radius=10;
	double volume=2.4943e+03;
	double sphere_radius = 80;
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<std::vector<double>> cell_positions;
	cell_positions= create_sphere(sample_cell_radius/5, sphere_radius);
	
	std::vector<double> tumor_center(3);
		tumor_center[0]=1000;
		tumor_center[1]=1000;
		tumor_center[2]=1000;
	
	for(int i=0;i<cell_positions.size();i++)
	{
		Cell* pCell = create_cell();
		pCell->register_microenvironment(&microenvironment);
		pCell->assign_position(tumor_center+ cell_positions[i]);
		// pCell->functions.volume_update_function=empty_function;
		// pCell->functions.update_phenotype=do_nothing;
		pCell->phenotype.cycle.data.current_phase_index = Q_index; 
		pCell->set_total_volume(volume);	
	}
	
	std::cout << (*all_cells)[0]->phenotype.geometry.radius<<std::endl;	
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	try 
	{		
		while( t < t_max )
		{
			if( t > t_next_output_time - 0.5 * dt )
			{
				std::cout<<"time: "<<t<<std::endl;
				writeCellReport(*all_cells, t);
				t_next_output_time += t_output_interval;
			}
				
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt, dt);
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
