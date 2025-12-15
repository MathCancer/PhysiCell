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

#include "BioFVM_basic_agent_PIMPL.h"

namespace BioFVM{

Basic_Agent_PIMPL::Basic_Agent_PIMPL()
	: pImpl(nullptr)
{
}

Basic_Agent_PIMPL::Basic_Agent_PIMPL(Basic_Agent_Interface* impl)
	: pImpl(impl)
{
}

Basic_Agent_PIMPL::~Basic_Agent_PIMPL()
{
	if (pImpl != nullptr)
	{
		delete pImpl;
		pImpl = nullptr;
	}
}

// Volume methods
double& Basic_Agent_PIMPL::get_total_volume()
{
	return pImpl->get_total_volume();
}

void Basic_Agent_PIMPL::set_total_volume(double v)
{
	pImpl->set_total_volume(v);
}

void Basic_Agent_PIMPL::update_voxel_index()
{
	pImpl->update_voxel_index();
}

// Internalized substrates
void Basic_Agent_PIMPL::release_internalized_substrates( void )
{
	pImpl->release_internalized_substrates();
}

void Basic_Agent_PIMPL::set_internal_uptake_constants( double dt )
{
	pImpl->set_internal_uptake_constants(dt);
}

// Microenvironment registration
void Basic_Agent_PIMPL::register_microenvironment( Microenvironment_Interface* me )
{
	pImpl->register_microenvironment(me);
}

Microenvironment_Interface* Basic_Agent_PIMPL::get_microenvironment( void )
{
	return pImpl->get_microenvironment();
}

// ID and type accessors
int Basic_Agent_PIMPL::get_ID() const
{
	return pImpl->get_ID();
}

void Basic_Agent_PIMPL::set_ID(int new_ID)
{
	pImpl->set_ID(new_ID);
}

int Basic_Agent_PIMPL::get_index() const
{
	return pImpl->get_index();
}

void Basic_Agent_PIMPL::set_index(int new_index)
{
	pImpl->set_index(new_index);
}

int Basic_Agent_PIMPL::get_type() const
{
	return pImpl->get_type();
}

void Basic_Agent_PIMPL::set_type(int new_type)
{
	pImpl->set_type(new_type);
}

// Position methods
bool Basic_Agent_PIMPL::assign_position(double x, double y, double z)
{
	return pImpl->assign_position(x, y, z);
}

bool Basic_Agent_PIMPL::assign_position(std::vector<double> new_position)
{
	return pImpl->assign_position(new_position);
}

double* Basic_Agent_PIMPL::get_position_internal()
{
	return pImpl->get_position_internal();
}

const std::vector<double>& Basic_Agent_PIMPL::get_position() const
{
	return pImpl->get_position();
}

void Basic_Agent_PIMPL::update_position( double dt )
{
	pImpl->update_position(dt);
}

// Velocity methods
std::vector<double>& Basic_Agent_PIMPL::get_velocity()
{
	return pImpl->get_velocity();
}

const std::vector<double>& Basic_Agent_PIMPL::get_velocity() const
{
	return pImpl->get_velocity();
}

std::vector<double>& Basic_Agent_PIMPL::get_previous_velocity( void )
{
	return pImpl->get_previous_velocity();
}

const std::vector<double>& Basic_Agent_PIMPL::get_previous_velocity( void ) const
{
	return pImpl->get_previous_velocity();
}

// Activity status
bool Basic_Agent_PIMPL::get_is_active() const
{
	return pImpl->get_is_active();
}

void Basic_Agent_PIMPL::set_is_active(bool active)
{
	pImpl->set_is_active(active);
}

// Getter and setter methods for vector pointers
double* Basic_Agent_PIMPL::get_secretion_rates()
{
	return pImpl->get_secretion_rates();
}

const double* Basic_Agent_PIMPL::get_secretion_rates() const
{
	return pImpl->get_secretion_rates();
}

double* Basic_Agent_PIMPL::get_saturation_densities()
{
	return pImpl->get_saturation_densities();
}

const double* Basic_Agent_PIMPL::get_saturation_densities() const
{
	return pImpl->get_saturation_densities();
}

double* Basic_Agent_PIMPL::get_uptake_rates()
{
	return pImpl->get_uptake_rates();
}

const double* Basic_Agent_PIMPL::get_uptake_rates() const
{
	return pImpl->get_uptake_rates();
}

double* Basic_Agent_PIMPL::get_net_export_rates()
{
	return pImpl->get_net_export_rates();
}

const double* Basic_Agent_PIMPL::get_net_export_rates() const
{
	return pImpl->get_net_export_rates();
}

double* Basic_Agent_PIMPL::get_internalized_total_substrates()
{
	return pImpl->get_internalized_total_substrates();
}

const double* Basic_Agent_PIMPL::get_internalized_total_substrates() const
{
	return pImpl->get_internalized_total_substrates();
}

double* Basic_Agent_PIMPL::get_fraction_released_at_death()
{
	return pImpl->get_fraction_released_at_death();
}

const double* Basic_Agent_PIMPL::get_fraction_released_at_death() const
{
	return pImpl->get_fraction_released_at_death();
}

double* Basic_Agent_PIMPL::get_fraction_transferred_when_ingested()
{
	return pImpl->get_fraction_transferred_when_ingested();
}

const double* Basic_Agent_PIMPL::get_fraction_transferred_when_ingested() const
{
	return pImpl->get_fraction_transferred_when_ingested();
}

// Secretion and uptake simulation
void Basic_Agent_PIMPL::simulate_secretion_and_uptake( double dt )
{
	pImpl->simulate_secretion_and_uptake(dt);
}

// Voxel access
int Basic_Agent_PIMPL::get_current_voxel_index( void )
{
	return pImpl->get_current_voxel_index();
}

double* Basic_Agent_PIMPL::nearest_density_vector( void )
{
	return pImpl->nearest_density_vector();
}

std::vector<double>& Basic_Agent_PIMPL::nearest_gradient( int substrate_index )
{
	return pImpl->nearest_gradient(substrate_index);
}

std::vector<std::vector<double>>& Basic_Agent_PIMPL::nearest_gradient_vector( void )
{
	return pImpl->nearest_gradient_vector();
}

};
