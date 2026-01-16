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

#ifndef __BioFVM_basic_agent_interface_h__
#define __BioFVM_basic_agent_interface_h__

#include <vector>

namespace BioFVM{

class Microenvironment_Interface;

class Basic_Agent_Interface
{
	friend class Basic_Agent_PIMPL;
protected:
	// Direct access to internal position array for performance
	// Used in performance-critical sections only
	// We are not exposing this publicly since its size can vary (2D vs 3D)
	// and we want to maintain safety for general users
	// The public variants of position access are always 3D
	virtual double* get_position_internal() = 0;
 
public:
	// Volume methods
	virtual double& get_total_volume() = 0;
	virtual void set_total_volume(double) = 0;
	virtual void update_voxel_index() = 0;

	// Internalized substrates
	virtual void release_internalized_substrates( void ) = 0; 
	virtual void set_internal_uptake_constants( double dt ) = 0; 

	// Microenvironment registration and access
	virtual void register_microenvironment( Microenvironment_Interface* ) = 0;
	virtual Microenvironment_Interface* get_microenvironment_interface( void ) = 0;

	// ID and type accessors
	virtual int get_ID() const = 0;
	virtual void set_ID(int new_ID) = 0;
	virtual int get_index() const = 0;
	virtual void set_index(int new_index) = 0;
	virtual int get_type() const = 0;
	virtual void set_type(int new_type) = 0;
	
	// Position methods
	virtual bool assign_position(double x, double y, double z) = 0;
	virtual bool assign_position(std::vector<double> new_position) = 0;
	virtual const std::vector<double>& get_position() const = 0;
	virtual void update_position( double dt ) = 0;
	
	// Velocity methods
	virtual std::vector<double>& get_velocity() = 0;
	virtual const std::vector<double>& get_velocity() const = 0;
	virtual std::vector<double>& get_previous_velocity( void ) = 0;
	virtual const std::vector<double>& get_previous_velocity( void ) const = 0;
	
	// Activity status
	virtual bool get_is_active() const = 0;
	virtual void set_is_active(bool active) = 0;
	
	// Getter methods for vector pointers
	virtual double* get_secretion_rates() = 0;
	virtual const double* get_secretion_rates() const = 0;
	virtual double* get_saturation_densities() = 0;
	virtual const double* get_saturation_densities() const = 0;
	virtual double* get_uptake_rates() = 0;
	virtual const double* get_uptake_rates() const = 0;
	virtual double* get_net_export_rates() = 0;
	virtual const double* get_net_export_rates() const = 0;
	virtual double* get_internalized_total_substrates() = 0;
	virtual const double* get_internalized_total_substrates() const = 0;
	virtual double* get_fraction_released_at_death() = 0;
	virtual const double* get_fraction_released_at_death() const = 0;
	virtual double* get_fraction_transferred_when_ingested() = 0;
	virtual const double* get_fraction_transferred_when_ingested() const = 0;
	
	virtual ~Basic_Agent_Interface(){};
	
	// Secretion and uptake simulation
	virtual void simulate_secretion_and_uptake( double dt ) = 0; 

	// Voxel access
	virtual int get_current_voxel_index( void ) = 0; 
	
	// Density and gradient access
	virtual double* nearest_density_vector( void ) = 0;
	virtual std::vector<double>& nearest_gradient( int substrate_index ) = 0;
	virtual std::vector<std::vector<double>>& nearest_gradient_vector( void ) = 0;
};

};

#endif

