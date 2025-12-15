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

#include "BioFVM_basic_agent_adapter.h"
#include "BioFVM_basic_agent.h"

namespace BioFVM{

Basic_Agent_Adapter::Basic_Agent_Adapter(Basic_Agent* agent, bool take_ownership) 
	: wrapped_agent(agent), owns_agent(take_ownership)
{
}

Basic_Agent_Adapter::~Basic_Agent_Adapter()
{
	if (owns_agent && wrapped_agent != nullptr)
	{
		delete wrapped_agent;
		wrapped_agent = nullptr;
	}
}

double& Basic_Agent_Adapter::get_total_volume()
{
	return wrapped_agent->get_total_volume();
}

void Basic_Agent_Adapter::set_total_volume(double vol)
{
	wrapped_agent->set_total_volume(vol);
}

void Basic_Agent_Adapter::update_voxel_index()
{
	wrapped_agent->update_voxel_index();
}

void Basic_Agent_Adapter::release_internalized_substrates( void )
{
	wrapped_agent->release_internalized_substrates();
}

void Basic_Agent_Adapter::set_internal_uptake_constants( double dt )
{
	wrapped_agent->set_internal_uptake_constants(dt);
}

void Basic_Agent_Adapter::register_microenvironment( Microenvironment_Interface* mi )
{
	wrapped_agent->register_microenvironment(mi);
	microenvironment_adapter = dynamic_cast<Microenvironment_Adapter*>(mi);
}

Microenvironment_Interface* Basic_Agent_Adapter::get_microenvironment( void )
{
	return microenvironment_adapter;
}

int Basic_Agent_Adapter::get_ID() const
{
	return wrapped_agent->ID;
}

void Basic_Agent_Adapter::set_ID(int new_ID)
{
	wrapped_agent->ID = new_ID;
}

int Basic_Agent_Adapter::get_index() const
{
	return wrapped_agent->index;
}

void Basic_Agent_Adapter::set_index(int new_index)
{
	wrapped_agent->index = new_index;
}

int Basic_Agent_Adapter::get_type() const
{
	return wrapped_agent->type;
}

void Basic_Agent_Adapter::set_type(int new_type)
{
	wrapped_agent->type = new_type;
}

bool Basic_Agent_Adapter::assign_position(double x, double y, double z)
{
	return wrapped_agent->assign_position(x, y, z);
}

bool Basic_Agent_Adapter::assign_position(std::vector<double> new_position)
{
	return wrapped_agent->assign_position(new_position);
}

double* Basic_Agent_Adapter::get_position_internal()
{
	return wrapped_agent->position.data();
}

const std::vector<double>& Basic_Agent_Adapter::get_position() const
{
	return wrapped_agent->position;
}

void Basic_Agent_Adapter::update_position( double dt )
{
	wrapped_agent->update_position(dt);
}

std::vector<double>& Basic_Agent_Adapter::get_velocity()
{
	return wrapped_agent->velocity;
}

const std::vector<double>& Basic_Agent_Adapter::get_velocity() const
{
	return wrapped_agent->velocity;
}

std::vector<double>& Basic_Agent_Adapter::get_previous_velocity( void )
{
	return wrapped_agent->previous_velocity;
}

const std::vector<double>& Basic_Agent_Adapter::get_previous_velocity( void ) const
{
	return wrapped_agent->previous_velocity;
}

bool Basic_Agent_Adapter::get_is_active() const
{
	return wrapped_agent->is_active;
}

void Basic_Agent_Adapter::set_is_active(bool active)
{
	wrapped_agent->is_active = active;
}

// Getter and setter methods for vector pointers
double* Basic_Agent_Adapter::get_secretion_rates()
{
	return wrapped_agent->secretion_rates.data();
}

const double* Basic_Agent_Adapter::get_secretion_rates() const
{
	return wrapped_agent->secretion_rates.data();
}

double* Basic_Agent_Adapter::get_saturation_densities()
{
	return wrapped_agent->saturation_densities.data();
}

const double* Basic_Agent_Adapter::get_saturation_densities() const
{
	return wrapped_agent->saturation_densities.data();
}

double* Basic_Agent_Adapter::get_uptake_rates()
{
	return wrapped_agent->uptake_rates.data();
}

const double* Basic_Agent_Adapter::get_uptake_rates() const
{
	return wrapped_agent->uptake_rates.data();
}

double* Basic_Agent_Adapter::get_net_export_rates()
{
	return wrapped_agent->net_export_rates.data();
}

const double* Basic_Agent_Adapter::get_net_export_rates() const
{
	return wrapped_agent->net_export_rates.data();
}

double* Basic_Agent_Adapter::get_internalized_total_substrates()
{
	return wrapped_agent->internalized_substrates.data();
}

const double* Basic_Agent_Adapter::get_internalized_total_substrates() const
{
	return wrapped_agent->internalized_substrates.data();
}

double* Basic_Agent_Adapter::get_fraction_released_at_death()
{
	return wrapped_agent->fraction_released_at_death.data();
}

const double* Basic_Agent_Adapter::get_fraction_released_at_death() const
{
	return wrapped_agent->fraction_released_at_death.data();
}

double* Basic_Agent_Adapter::get_fraction_transferred_when_ingested()
{
	return wrapped_agent->fraction_transferred_when_ingested.data();
}

const double* Basic_Agent_Adapter::get_fraction_transferred_when_ingested() const
{
	return wrapped_agent->fraction_transferred_when_ingested.data();
}

void Basic_Agent_Adapter::simulate_secretion_and_uptake( double dt )
{
	wrapped_agent->simulate_secretion_and_uptake(dt);
}

int Basic_Agent_Adapter::get_current_voxel_index( void )
{
	return wrapped_agent->get_current_voxel_index();
}

double* Basic_Agent_Adapter::nearest_density_vector( void )
{
	return wrapped_agent->nearest_density_vector().data();
}

std::vector<double>& Basic_Agent_Adapter::nearest_gradient( int substrate_index )
{
	return wrapped_agent->nearest_gradient(substrate_index);
}

std::vector<std::vector<double>>& Basic_Agent_Adapter::nearest_gradient_vector( void )
{
	return wrapped_agent->nearest_gradient_vector();
}

};
