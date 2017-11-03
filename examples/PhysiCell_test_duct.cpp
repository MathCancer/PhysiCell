/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based simulator for multicellular systemssimulator, 	#
#	 J. Comput. Biol., 2016 (submitted). 									# 
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based multicellular simulator, J. Comput. Biol., 2016   # 
#   (submitted).                                                            #
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
#include <time.h>

#include "PhysiCell.h"
#include "BioFVM.h" 

using namespace BioFVM;
using namespace PhysiCell;


int omp_num_threads = 8; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)

double o2_conc=7.1930;   //check this value to make sure it has support from literature

double duct_radius= 158.75;
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


double distance_to_membrane_duct(Cell* pCell)
{
	//Note that this function assumes that duct cap center is located at <0, 0, 0>
	if(pCell->position[0]>=0)
	{
		double temp= sqrt(pCell->position[1]* pCell->position[1] + pCell->position[2]*pCell->position[2]);
		if(temp==0)
		{
			pCell->displacement[0]=0; pCell->displacement[1]=0; pCell->displacement[1]=0;
		}
		else
		{
			pCell->displacement[0]=0; 
			pCell->displacement[1]= -pCell->position[1]/ temp; 
			pCell->displacement[2]= -pCell->position[2]/ temp; 
		}
		return fabs(duct_radius- temp);
	}
	
	double temp= dist(pCell->position, {0.0,0.0,0.0});
		
	if(temp==0)
	{
		pCell->displacement[0]=0;pCell->displacement[1]=0;pCell->displacement[1]=0;
	}
	else
	{
		pCell->displacement[0]= -pCell->position[0]/ temp; 
		pCell->displacement[1]= -pCell->position[1]/ temp; 
		pCell->displacement[2]= -pCell->position[2]/ temp; 
	}
	return fabs(duct_radius- temp);
}

int main( int argc, char* argv[] )
{
	bool DEBUG=false;
	double t = 0.0; 
	double dt = 0.01; // reaction-diffusion time-step
	double mechanics_dt= 0.1;
	double cell_cycle_dt= 6;
	
	double t_output_interval = 60.0; // 1.0; 
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
	bounding_box[PhysiCell_constants::mesh_min_x_index] = -200; bounding_box[PhysiCell_constants::mesh_max_x_index] = 1000; 
	bounding_box[PhysiCell_constants::mesh_min_y_index] = -200; bounding_box[PhysiCell_constants::mesh_max_y_index] = 200; 
	bounding_box[PhysiCell_constants::mesh_min_z_index] = -200; bounding_box[PhysiCell_constants::mesh_max_z_index] = 200; 
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
	Cell_Container* cell_container = new Cell_Container;
	cell_container->initialize(bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5], min_voxel_size );
	microenvironment.agent_container= (Agent_Container *) cell_container; // cell_container;
	// microenvironment[0].agent_container->initialize(number_of_voxels);
	std::cout<<"num voxels4: "<<cell_container->underlying_mesh.voxels.size()<<std::endl;
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

	
	Cell_Line cancer; 
	// Cell_Line endothelial; 
	set_cancer_cell_line_MCF7( cancer ); 
	cancer.phenotypes[0].secretion_rates.rates.resize( microenvironment.number_of_densities() , 0.0 );
	cancer.phenotypes[0].uptake_rates.rates.resize( microenvironment.number_of_densities() , 0.0 ) ;
	cancer.phenotypes[0].saturation_densities.densities.resize( microenvironment.number_of_densities() , 0.0 );
	cancer.phenotypes[0].uptake_rates.rates[0] = 10; 
	
	cancer.display_information( std::cout );
		
	double cell_radius=10;
	double sphere_radius = duct_radius - 10;;
	// std::cout << __FILE__ << " custom " << __LINE__ << std::endl; 
	std::vector<std::vector<double>> cell_positions;
	cell_positions= create_sphere(cell_radius, sphere_radius);

	//add Dirichlet node for all the voxels located outside of the duct
	std::vector<double> dirichlet_o2( 1 , o2_conc );
	
	for( int i=0; i < microenvironment.number_of_voxels() ; i++ )
	{
		if(microenvironment.voxels(i).center[0]>=0)
		{
			if(sqrt(microenvironment.voxels(i).center[1]* microenvironment.voxels(i).center[1] + microenvironment.voxels(i).center[2]*microenvironment.voxels(i).center[2])>duct_radius)
				microenvironment.add_dirichlet_node( i , dirichlet_o2 );
		}
		else
		{
			if(dist(microenvironment.voxels(i).center, {0.0,0.0,0.0})>duct_radius)
				microenvironment.add_dirichlet_node( i , dirichlet_o2 );
		}
	}
	
	Cell* pCell;
	if(DEBUG)
	{
		std::vector<std::vector<double>> cells_data;
		cells_data= read_matlab( "cells_debug8.mat" );
		
		for(int i=0;i<cells_data.size();i++)
		{	
			if(!microenvironment.mesh.is_position_valid(cells_data[i][2], cells_data[i][3], cells_data[i][4]))
				continue;
			pCell = create_cell();
			pCell->register_microenvironment(&microenvironment);
			pCell->assign_position(cells_data[i][2], cells_data[i][3], cells_data[i][4]);
			// pCell->assign_position({0,65,0});
			pCell->advance_cell_current_phase= ki67_advanced_cycle_model;
			pCell->set_phenotype(cancer.phenotypes[0]);
			pCell->phenotype.set_current_phase(cells_data[i][12]);
			pCell->set_total_volume((4.0/3.0)*PhysiCell_constants::pi* pow(cells_data[i][5], 3));
			pCell->type=PhysiCell_constants::TUMOR_TYPE;		
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time= cells_data[i][13];
			pCell->distance_to_membrane= distance_to_membrane_duct;
		}
	}
	else
	{
		for(int i=0;i<cell_positions.size();i++)
		// for(int i=0;i<2;i++)
		{
			if(cell_positions[i][0]>0)
				continue;
			pCell = create_cell();
			pCell->register_microenvironment(&microenvironment);
			pCell->assign_position(cell_positions[i]);
			// pCell->assign_position({0,65,0});
			pCell->advance_cell_current_phase= ki67_advanced_cycle_model;
			pCell->set_phenotype(cancer.phenotypes[0]);
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative);	
			pCell->type=PhysiCell_constants::TUMOR_TYPE;		
			pCell->distance_to_membrane= distance_to_membrane_duct;
	}
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
	std::cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << "\n";
	
/*	clock_t begin = clock();
	double sum=0;
  for(int i=0;i<1000000000;i++)
  {
	  sum+= sqrt((double)i);
  }
	std::cout<<"sum: "<<sum<<std::endl;
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
 */
  
	std::ofstream report_file ("report.txt");
	report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time"<<std::endl;
	try 
	{		
		while( t < t_max )
		{
			if(  fabs( t - t_next_output_time ) < 0.0001 )
			{
				log_output(t, output_index, microenvironment, report_file);
				t_next_output_time += t_output_interval;						
			}
			// std::cout<<__LINE__<<std::endl;			
			microenvironment.simulate_cell_sources_and_sinks( dt );
			microenvironment.simulate_diffusion_decay( dt );
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, cell_cycle_dt, mechanics_dt);		
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
