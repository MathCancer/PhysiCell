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
# Copyright (c) 2015-2025, Paul Macklin and the BioFVM Project              #
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
#include "BioFVM_basic_agent_interface.h"

namespace BioFVM{

void reset_max_basic_agent_ID( void );

class Basic_Agent final : public Basic_Agent_Interface
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

	// Interface implementation
	double* get_position_internal() override;
	
 public:
	bool is_active;

	std::vector<double> secretion_rates; 
	std::vector<double> saturation_densities; 
	std::vector<double> uptake_rates;  
	std::vector<double> net_export_rates; 
	double& get_total_volume() override;
	void set_total_volume(double) override;
	void update_voxel_index() override;

	/* new for internalized substrates in 1.5.0 */ 
	std::vector<double> internalized_substrates; 
	std::vector<double> fraction_released_at_death; 
	std::vector<double> fraction_transferred_when_ingested; 
	void release_internalized_substrates( void ) override; 

	void set_internal_uptake_constants( double dt ) override; // any time you update the cell volume or rates, should call this function. 

	void register_microenvironment( Microenvironment* );
	void register_microenvironment( Microenvironment_Interface* ) override;
	Microenvironment* get_microenvironment( void );
	Microenvironment_Interface* get_microenvironment_interface( void ) override; 

	int ID; 
	int index; 
	int type;
	
	// ID and type accessors
	int get_ID() const override;
	void set_ID(int new_ID) override;
	int get_index() const override;
	void set_index(int new_index) override;
	int get_type() const override;
	void set_type(int new_type) override;
	
	bool assign_position(double x, double y, double z) override;
	bool assign_position(std::vector<double> new_position) override;
	const std::vector<double>& get_position() const override;
	
	std::vector<double> position;  
	std::vector<double> velocity; 
	std::vector<double>& get_velocity() override;
	const std::vector<double>& get_velocity() const override;
	std::vector<double>& get_previous_velocity( void ) override;
	const std::vector<double>& get_previous_velocity( void ) const override;
	void update_position( double dt ) override;
	
	// Activity status
	bool get_is_active() const override;
	void set_is_active(bool active) override;
	
	// Getter methods for vector pointers
	double* get_secretion_rates() override;
	const double* get_secretion_rates() const override;
	double* get_saturation_densities() override;
	const double* get_saturation_densities() const override;
	double* get_uptake_rates() override;
	const double* get_uptake_rates() const override;
	double* get_net_export_rates() override;
	const double* get_net_export_rates() const override;
	double* get_internalized_total_substrates() override;
	const double* get_internalized_total_substrates() const override;
	double* get_fraction_released_at_death() override;
	const double* get_fraction_released_at_death() const override;
	double* get_fraction_transferred_when_ingested() override;
	const double* get_fraction_transferred_when_ingested() const override;
	
	Basic_Agent(); 
	virtual ~Basic_Agent(){};
	// simulate secretion and uptake at the nearest voxel at the indicated microenvironment.
	// if no microenvironment indicated, use the currently selected microenvironment. 
	void simulate_secretion_and_uptake( double dt ) override; 

	int get_current_voxel_index( void ) override; 
	// directly access the substrate vector at the nearest voxel at the indicated microenvironment 
	std::vector<double>& nearest_density_vector( int microenvironment_index ); // not implemented!
	double* nearest_density_vector( void ) override;
	
	// directly access the gradient of substrate n nearest to the cell 
	std::vector<double>& nearest_gradient( int substrate_index ) override;
	// directly access a vector of gradients, one gradient per substrate 
	std::vector<std::vector<double>>& nearest_gradient_vector( void ) override;
};

extern std::vector<Basic_Agent*> all_basic_agents; 

Basic_Agent* create_basic_agent( void );
void delete_basic_agent( int ); 
void delete_basic_agent( Basic_Agent* ); 
void save_all_basic_agents_to_matlab( std::string filename ); 

};

#endif

