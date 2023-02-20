/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the BioFVM Project              #
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

#ifndef __BioFVM_basic_agent_h__
#define __BioFVM_basic_agent_h__

#include <vector>
#include "BioFVM_microenvironment.h"
#include "BioFVM_matlab.h"
#include "BioFVM_vector.h"

namespace BioFVM{
	
class Basic_Agent
{
 private:
	Microenvironment* microenvironment; 
	int selected_microenvironment; 
	
	int current_microenvironment_voxel_index;
	double volume;
	bool volume_is_changed;
	int current_voxel_index;	
	
 protected:
	std::vector<double> cell_source_sink_solver_temp1;
	std::vector<double> cell_source_sink_solver_temp2;
	std::vector<double> cell_source_sink_solver_temp_export1; 
	std::vector<double> cell_source_sink_solver_temp_export2; 	
	std::vector<double> previous_velocity; 
//	bool is_active;
	
	std::vector<double> total_extracellular_substrate_change; 
	
 public:
	bool is_active;

	std::vector<double> * secretion_rates; 
	std::vector<double> * saturation_densities; 
	std::vector<double> * uptake_rates;  
	std::vector<double> * net_export_rates; 
	double get_total_volume();
	void set_total_volume(double);
	void update_voxel_index();

	/* new for internalized substrates in 1.5.0 */ 
	std::vector<double> * internalized_substrates; 
	std::vector<double> * fraction_released_at_death; 
	std::vector<double> * fraction_transferred_when_ingested; 
	void release_internalized_substrates( void ); 

	void set_internal_uptake_constants( double dt ); // any time you update the cell volume or rates, should call this function. 

	void register_microenvironment( Microenvironment* );
	Microenvironment* get_microenvironment( void ); 

	int ID; 
	int index; 
	int type;
	
	bool assign_position(double x, double y, double z);
	bool assign_position(std::vector<double> new_position);
	
	std::vector<double> position;  
	std::vector<double> velocity; 
	void update_position( double dt );
	
	Basic_Agent(); 

	// simulate secretion and uptake at the nearest voxel at the indicated microenvironment.
	// if no microenvironment indicated, use the currently selected microenvironment. 
	void simulate_secretion_and_uptake( Microenvironment* M, double dt ); 

	int get_current_voxel_index( void ); 
	// directly access the substrate vector at the nearest voxel at the indicated microenvironment 
	std::vector<double>& nearest_density_vector( int microenvironment_index ); // not implemented!
	std::vector<double>& nearest_density_vector( void );
	
	// directly access the gradient of substrate n nearest to the cell 
	std::vector<double>& nearest_gradient( int substrate_index );
	// directly access a vector of gradients, one gradient per substrate 
	std::vector<gradient>& nearest_gradient_vector( void ); 
};

extern std::vector<Basic_Agent*> all_basic_agents; 

Basic_Agent* create_basic_agent( void );
void delete_basic_agent( int ); 
void delete_basic_agent( Basic_Agent* ); 
void save_all_basic_agents_to_matlab( std::string filename ); 

};

#endif

