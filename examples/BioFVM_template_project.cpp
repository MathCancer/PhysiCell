/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.2) [1]        #
#                                                                           #
# [1] A. Ghaffarizaeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient  #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the BioFVM Project              #
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

#include "BioFVM.h"

#include <omp.h>

using namespace BioFVM; 

void supply_function( Microenvironment* microenvironment, int voxel_index, std::vector<double>* write_here )
{
	// use this syntax to access the jth substrate write_here
	// (*write_here)[j]
	// use this syntax to access the jth substrate in voxel voxel_index of microenvironment: 
	// microenvironment->density_vector(voxel_index)[j]

	return; 
}

void supply_target_function( Microenvironment* microenvironment, int voxel_index, std::vector<double>* write_here )
{
	// use this syntax to access the jth substrate write_here
	// (*write_here)[j]
	// use this syntax to access the jth substrate in voxel voxel_index of microenvironment: 
	// microenvironment->density_vector(voxel_index)[j]

	return; 
}

void uptake_function( Microenvironment* microenvironment, int voxel_index, std::vector<double>* write_here )
{
	// use this syntax to access the jth substrate write_here
	// (*write_here)[j]
	// use this syntax to access the jth substrate in voxel voxel_index of microenvironment: 
	// microenvironment->density_vector(voxel_index)[j]

	return; 
}

int main( int argc, char* argv[] )
{
	RUNTIME_TIC();
	omp_set_num_threads( 8 );
	
	std::cout << "Starting program ... " << std::endl;
	
	// create a microenvironment, and set units 
		
	Microenvironment M; 
	M.name = "microenvironment"; 
	M.time_units = "min"; 
	M.spatial_units = "micron"; 
	M.mesh.units = M.spatial_units;
	
	// set up and add all the densities you plan 

	M.set_density( 0 , "oxygen" , "mmHg" ); 
	
	// here's how you add a new substrate 
	M.add_density( "crayons" , "Megacrayola" ); 
	
	
	// set the properties of the diffusing substrates 
	
	M.diffusion_coefficients[0] = 1e5;   
	M.decay_rates[0] = 10; // 100 micron length scale 
	M.diffusion_coefficients[1] = 1e4;   
	M.decay_rates[1] = 1.0/9.0; // 300 micron length scale 
	
	// set the mesh size 
	
	double dx = 20; // 
	M.resize_space( 0 , 1000.0 , 0, 1000.0 , 0.0 , 1000.0 , dx, dx, dx );  
	
	// display summary information 
	
	M.display_information( std::cout ); 
	
	// set up metadata 

	BioFVM_metadata.program.user.surname = "Kirk";
	BioFVM_metadata.program.user.given_names = "James T.";
	BioFVM_metadata.program.user.email = "Jimmy.Kirk@starfleet.mil";
	BioFVM_metadata.program.user.organization = "Starfleet";
	BioFVM_metadata.program.user.department = "U.S.S. Enterprise (NCC 1701)";

	BioFVM_metadata.program.creator.surname = "Roykirk";
	BioFVM_metadata.program.creator.given_names = "Jackson";
	BioFVM_metadata.program.creator.organization = "Yoyodyne Corporation";
	
	BioFVM_metadata.program.program_name = "Nomad";
	BioFVM_metadata.program.program_version = "MK-15c";
	BioFVM_metadata.program.program_URL = "";

	// set initial conditions 
	
	// use this syntax to create a zero vector of length 3
	// std::vector<double> zero(3,0.0); 
	
	// use this syntax for a parallelized loop over all the 
	// voxels in your mesh: 	
	#pragma omp parallel for 
	for( int i=0 ; i < M.number_of_voxels() ; i++ )
	{
		// use this syntax to access the coordinates (as a vector) of 
		// the ith voxel; 
		// M.mesh.voxels[i].center 
		
		// use this access the jth substrate at the ith voxel
		// M.density_vector(i)[j]
		
	}

	// save the initial profile 
	
	// M.write_to_matlab( "initial.mat" ); // barebones 
	save_BioFVM_to_MultiCellDS_xml_pugi( "initial" , M , 0.0 ); // MultiCellDS digital snapshot

	// set up the diffusion solver, sources and sinks 
	
	M.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D;
	
	M.bulk_supply_rate_function = supply_function;
	M.bulk_supply_target_densities_function = supply_target_function;
	M.bulk_uptake_rate_function = uptake_function;
	
	double t     = 0.0; 
	double t_max = 100.0;
	double dt    = 0.1; 
	
	double output_interval  = 10.0;  // how often you save data 
	double next_output_time = t;     // next time you save data 
	
	while( t < t_max )
	{
		// if it's time, save the simulation 
		if( fabs( t - next_output_time ) < dt/2.0 )
		{
			std::cout << "simulation time: " << t << " " << M.time_units << " (" << t_max << " " << M.time_units << " max)" << std::endl; 
			
			char* filename; 
			filename = new char [1024];
			
			// sprintf( filename, "output_%6f.mat" , next_output_time ); 
			// M.write_to_matlab( filename ); 

			sprintf( filename, "output_%6f" , next_output_time ); 
			save_BioFVM_to_MultiCellDS_xml_pugi( filename , M , 0.0 ); // MultiCellDS digital snapshot
			
			delete [] filename; 
			next_output_time += output_interval; 
		}
		
		M.simulate_bulk_sources_and_sinks( dt );
		M.simulate_diffusion_decay( dt );
		M.simulate_cell_sources_and_sinks( dt ); 
		
		t += dt;
	}
	
	// M.write_to_matlab( "final.mat"); // barebones 
	save_BioFVM_to_MultiCellDS_xml_pugi( "final" , M , 0.0 ); // MultiCellDS digital snapshot

	RUNTIME_TOC();
	std::cout << std::endl << "Program wall time: "; 
	display_stopwatch_value( std::cout , runtime_stopwatch_value() ); 
	std::cout << std::endl << "Done!" << std::endl; 
	
	return 0; 
}
