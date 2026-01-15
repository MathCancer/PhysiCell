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

#ifndef __BioFVM_basic_agent_adapter_h__
#define __BioFVM_basic_agent_adapter_h__

#include "BioFVM_basic_agent_interface.h"

namespace BioFVM{

class Basic_Agent;

/**
 * @brief Adapter class that wraps BioFVM::Basic_Agent
 *
 * This class implements the Basic_Agent_Interface using BioFVM::Basic_Agent
 * as the underlying implementation. It allows PhysiCell to work with the
 * abstract interface while delegating to the concrete BioFVM implementation.
 */
class Basic_Agent_Adapter : public Basic_Agent_Interface
{
private:
	Microenvironment_Interface* microenvironment_interface;
	Basic_Agent* wrapped_agent;
	bool owns_agent;

protected:
	double* get_position_internal() override;
	
public:
	/**
	 * @brief Construct adapter that wraps an existing Basic_Agent
	 * @param agent Pointer to the Basic_Agent to wrap 
	 * @param take_ownership If true, this adapter will delete the agent when destroyed
	 */
	explicit Basic_Agent_Adapter(Basic_Agent* agent, bool take_ownership = false);
	
	virtual ~Basic_Agent_Adapter();
	
	/**
	 * @brief Get the wrapped Basic_Agent
	 * @return Pointer to the wrapped Basic_Agent
	 */
	Basic_Agent* get_wrapped_agent() { return wrapped_agent; }
	const Basic_Agent* get_wrapped_agent() const { return wrapped_agent; }
	
	// Volume methods
	virtual double& get_total_volume() override;
	virtual void set_total_volume(double) override;
	virtual void update_voxel_index() override;

	// Internalized substrates
	virtual void release_internalized_substrates( void ) override; 
	virtual void set_internal_uptake_constants( double dt ) override; 

	// Microenvironment registration and access
	virtual void register_microenvironment( Microenvironment_Interface* ) override;
	virtual Microenvironment_Interface* get_microenvironment( void ) override;

	// ID and type accessors
	virtual int get_ID() const override;
	virtual void set_ID(int new_ID) override;
	virtual int get_index() const override;
	virtual void set_index(int new_index) override;
	virtual int get_type() const override;
	virtual void set_type(int new_type) override;
	
	// Position methods
	virtual bool assign_position(double x, double y, double z) override;
	virtual bool assign_position(std::vector<double> new_position) override;
	virtual const std::vector<double>& get_position() const override;
	virtual void update_position( double dt ) override;
	
	// Velocity methods
	virtual std::vector<double>& get_velocity() override;
	virtual const std::vector<double>& get_velocity() const override;
	virtual std::vector<double>& get_previous_velocity( void ) override;
	virtual const std::vector<double>& get_previous_velocity( void ) const override;
	
	// Activity status
	virtual bool get_is_active() const override;
	virtual void set_is_active(bool active) override;
	
	// Getter and setter methods for vector pointers
	virtual double* get_secretion_rates() override;
	virtual const double* get_secretion_rates() const override;
	virtual double* get_saturation_densities() override;
	virtual const double* get_saturation_densities() const override;
	virtual double* get_uptake_rates() override;
	virtual const double* get_uptake_rates() const override;
	virtual double* get_net_export_rates() override;
	virtual const double* get_net_export_rates() const override;
	virtual double* get_internalized_total_substrates() override;
	virtual const double* get_internalized_total_substrates() const override;
	virtual double* get_fraction_released_at_death() override;
	virtual const double* get_fraction_released_at_death() const override;
	virtual double* get_fraction_transferred_when_ingested() override;
	virtual const double* get_fraction_transferred_when_ingested() const override;
	
	// Secretion and uptake simulation
	virtual void simulate_secretion_and_uptake( double dt ) override; 

	// Voxel access
	virtual int get_current_voxel_index( void ) override; 
	
	// Density and gradient access
	virtual double* nearest_density_vector( void ) override;
	virtual std::vector<double>& nearest_gradient( int substrate_index ) override;
	virtual std::vector<std::vector<double>>& nearest_gradient_vector( void ) override;
};

};

#endif
