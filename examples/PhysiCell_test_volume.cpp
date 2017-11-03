/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1],  #
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
 
int omp_num_threads = 1; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)
double pi=3.1415926535897932384626433832795;
double min_voxel_size=30;

double o2_conc=5.01;

int main( int argc, char* argv[] )
{
	double t = 0.0; 
	double dt;
	char test_type[2];
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
	double t_output_interval = 60.0;//std::max(1.0, dt); // 1.0; 
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
	std::vector<double> point1(3);

	double a1=100;
	point1[0]= a1; point1[1]= a1; point1[2]= a1; 
	
	Cell* pCell1 = create_cell();
	pCell1->register_microenvironment(&microenvironment);
	pCell1->assign_position(point1);
	pCell1->set_phenotype(cancer.phenotypes[0]);
	// pCell1->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative);
	if(test_type[1]=='A' || test_type[1]=='a')
	{
		pCell1->phenotype.set_current_phase(PhysiCell_constants::apoptotic);
		pCell1->advance_cell_current_phase= death_apoptosis_model_deterministic;
	}
	else if(test_type[1]=='N' || test_type[1]=='n')
	{
		pCell1->phenotype.set_current_phase(PhysiCell_constants::necrotic_swelling);
		pCell1->advance_cell_current_phase= death_necrosis_swelling_model;
		
	}
	else if(test_type[1]=='c' || test_type[1]=='C')
	{
		pCell1->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_premitotic);
		pCell1->advance_cell_current_phase= ki67_advanced_cycle_model_legacy;
		// set Q duration to INT_MAX
		//pCell1->phenotype.cycle.phases[pCell1->phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration= INT_MAX;
		
	}
	
	
	std::cout<<pCell1->get_total_volume()<<std::endl;
	pCell1->type=PhysiCell_constants::TUMOR_TYPE;
	
	for(int i=0;i<all_basic_agents.size();i++){
		all_basic_agents[i]->set_internal_uptake_constants(dt); 
	}
		
	std::cout << (*all_cells).size() <<" agents created successfully." <<std::endl;
	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	std::ofstream vol_report ("vol_report.txt", std::ofstream::out);
	try 
	{		
		while( t < t_max )
		{
			//std::cout<<pCell1->get_total_volume()<<std::endl;

			if(  fabs( t - t_next_output_time ) < 0.001 )
			{
				// std::cout<<(*all_cells).size()<<std::endl;
				vol_report<<t<<"\t"<<pCell1->get_total_volume()<<"\t"<<pCell1->phenotype.volume.fluid<<"\t"<<pCell1->phenotype.volume.nuclear_solid<<"\t"<<pCell1->phenotype.volume.cytoplasmic_solid<<"\n";
				t_next_output_time += t_output_interval;
			}
			if((*all_cells).size()>1)
			{
				delete_cell((Cell*)all_basic_agents[1]);
				std::cout << "cell deleted " << std::endl;
			}
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(t, dt, dt);
			t += dt; 
		}
		//report_file.close();

		std::cout << "total number of agents: " << (*all_cells).size()<<std::endl << std::endl;
		BioFVM::RUNTIME_TOC();
		BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() );
		vol_report.close();
	}
	
	catch( const std::exception& e ) { // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	return 0; 
}
