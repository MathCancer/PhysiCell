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

#ifndef __BioFVM_basic_agent_PIMPL_h__
#define __BioFVM_basic_agent_PIMPL_h__

#include "BioFVM_basic_agent_interface.h"

namespace BioFVM{

/**
 * @brief PIMPL (Pointer to Implementation) base class for agents
 *
 * This class implements the Basic_Agent_Interface by delegating to a wrapped
 * Basic_Agent_Interface* implementation. It provides the PIMPL pattern foundation
 * that derived classes (like PhysiCell::Cell) can use to add their own functionality
 * while maintaining the interface contract.
 *
 * Derived classes inherit from this and get automatic delegation to pImpl.
 * They can override specific methods to add custom behavior while still delegating
 * to the underlying implementation for basic agent operations.
 */
class Basic_Agent_PIMPL : public Basic_Agent_Interface
{
 protected:
	// Pointer to the actual implementation (adapter wrapping Basic_Agent)
	Basic_Agent_Interface* pImpl;

	double* get_position_internal() override;
	
 public:
	/**
	 * @brief Default constructor - derived class must set pImpl
	 */
	Basic_Agent_PIMPL();
	
	/**
	 * @brief Constructor with implementation
	 * @param impl Pointer to the implementation (takes ownership)
	 */
	explicit Basic_Agent_PIMPL(Basic_Agent_Interface* impl);
	
	virtual ~Basic_Agent_PIMPL();
	
	/**
	 * @brief Get the wrapped implementation
	 */
	Basic_Agent_Interface* get_implementation() { return pImpl; }
	const Basic_Agent_Interface* get_implementation() const { return pImpl; }
	
	// Volume methods - delegate to pImpl
	virtual double& get_total_volume() override;
	virtual void set_total_volume(double) override;
	virtual void update_voxel_index() override;

	// Internalized substrates - delegate to pImpl
	virtual void release_internalized_substrates( void ) override; 
	virtual void set_internal_uptake_constants( double dt ) override; 

	// Microenvironment registration and access - delegate to pImpl
	virtual void register_microenvironment( Microenvironment_Interface* ) override;
	virtual Microenvironment_Interface* get_microenvironment_interface( void ) override;

	// ID and type accessors - delegate to pImpl
	virtual int get_ID() const override;
	virtual void set_ID(int new_ID) override;
	virtual int get_index() const override;
	virtual void set_index(int new_index) override;
	virtual int get_type() const override;
	virtual void set_type(int new_type) override;
	
	// Position methods - delegate to pImpl
	virtual bool assign_position(double x, double y, double z) override;
	virtual bool assign_position(std::vector<double> new_position) override;
	virtual const std::vector<double>& get_position() const override;
	virtual void update_position( double dt ) override;
	
	// Velocity methods - delegate to pImpl
	virtual std::vector<double>& get_velocity() override;
	virtual const std::vector<double>& get_velocity() const override;
	virtual std::vector<double>& get_previous_velocity( void ) override;
	virtual const std::vector<double>& get_previous_velocity( void ) const override;
	
	// Activity status - delegate to pImpl
	virtual bool get_is_active() const override;
	virtual void set_is_active(bool active) override;
	
	// Getter methods for vector pointers - delegate to pImpl
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
	
	// Secretion and uptake simulation - delegate to pImpl
	virtual void simulate_secretion_and_uptake( double dt ) override; 

	// Voxel access - delegate to pImpl
	virtual int get_current_voxel_index( void ) override; 
	
	// Density and gradient access - delegate to pImpl
	virtual double* nearest_density_vector( void ) override;
	virtual std::vector<double>& nearest_gradient( int substrate_index ) override;
	virtual std::vector<std::vector<double>>& nearest_gradient_vector( void ) override;
};

};

#endif
