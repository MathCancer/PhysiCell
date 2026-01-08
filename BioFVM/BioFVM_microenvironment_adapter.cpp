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

#include "BioFVM_microenvironment_adapter.h"
#include "BioFVM.h"
#include "BioFVM_agent_container.h"
#include "BioFVM_basic_agent.h"
#include "BioFVM_microenvironment.h"
#include "BioFVM_implementation.h"
#include "BioFVM_basic_agent_adapter.h"

// ============================================================================
// BioFVM_Microenvironment_Adapter implementation
// ============================================================================

namespace BioFVM{

Microenvironment_Adapter::Microenvironment_Adapter(Microenvironment* env, bool take_ownership)
	: biofvm_microenvironment(env), owns_microenvironment(take_ownership)
{
	if (biofvm_microenvironment == nullptr)
	{
		throw std::invalid_argument("BioFVM_Microenvironment_Adapter: Cannot wrap a null microenvironment");
	}
}

Microenvironment_Adapter::Microenvironment_Adapter()
	: biofvm_microenvironment(new Microenvironment()), owns_microenvironment(true)
{
}

Microenvironment_Adapter::~Microenvironment_Adapter()
{
	if (owns_microenvironment && biofvm_microenvironment != nullptr)
	{
		delete biofvm_microenvironment;
	}
}

// Units access
std::string& Microenvironment_Adapter::get_time_units()
{
	return biofvm_microenvironment->time_units;
}

const std::string& Microenvironment_Adapter::get_time_units() const
{
	return biofvm_microenvironment->time_units;
}

std::string& Microenvironment_Adapter::get_spatial_units()
{
	return biofvm_microenvironment->spatial_units;
}

const std::string& Microenvironment_Adapter::get_spatial_units() const
{
	return biofvm_microenvironment->spatial_units;
}

// Query methods
unsigned int Microenvironment_Adapter::number_of_densities() const
{
	return biofvm_microenvironment->number_of_densities();
}

unsigned int Microenvironment_Adapter::number_of_voxels() const
{
	return biofvm_microenvironment->number_of_voxels();
}

unsigned int Microenvironment_Adapter::number_of_voxel_faces() const
{
	return biofvm_microenvironment->number_of_voxel_faces();
}

// Substrate/Density management
int Microenvironment_Adapter::find_density_index(const std::string& name) const
{
	return biofvm_microenvironment->find_density_index(name);
}

void Microenvironment_Adapter::add_density()
{
	biofvm_microenvironment->add_density();
}

void Microenvironment_Adapter::add_density(const std::string& name, const std::string& units)
{
	biofvm_microenvironment->add_density(name, units);
}

void Microenvironment_Adapter::add_density(const std::string& name, const std::string& units,
                                                  double diffusion_constant, double decay_rate)
{
	biofvm_microenvironment->add_density(name, units, diffusion_constant, decay_rate);
}

void Microenvironment_Adapter::set_density(int index, const std::string& name, const std::string& units)
{
	biofvm_microenvironment->set_density(index, name, units);
}

void Microenvironment_Adapter::set_density(int index, const std::string& name, const std::string& units,
                                                  double diffusion_constant, double decay_rate)
{
	biofvm_microenvironment->set_density(index, name, units, diffusion_constant, decay_rate);
}

void Microenvironment_Adapter::resize_densities(int new_size)
{
	biofvm_microenvironment->resize_densities(new_size);
}

// Voxel/Position access
int Microenvironment_Adapter::voxel_index(int i, int j, int k) const
{
	return biofvm_microenvironment->voxel_index(i, j, k);
}

std::vector<unsigned int> Microenvironment_Adapter::cartesian_indices(int n) const
{
	return biofvm_microenvironment->cartesian_indices(n);
}

int Microenvironment_Adapter::nearest_voxel_index(const std::vector<double>& position) const
{
	// Need to cast away const for BioFVM's API
	return biofvm_microenvironment->nearest_voxel_index(const_cast<std::vector<double>&>(position));
}

std::vector<unsigned int> Microenvironment_Adapter::nearest_cartesian_indices(const std::vector<double>& position) const
{
	// Need to cast away const for BioFVM's API
	return biofvm_microenvironment->nearest_cartesian_indices(const_cast<std::vector<double>&>(position));
}

Voxel& Microenvironment_Adapter::voxels(int voxel_index)
{
	return biofvm_microenvironment->voxels(voxel_index);
}

const Voxel& Microenvironment_Adapter::voxels(int voxel_index) const
{
	return biofvm_microenvironment->voxels(voxel_index);
}

Voxel& Microenvironment_Adapter::nearest_voxel(const std::vector<double>& position)
{
	// Need to cast away const for BioFVM's API
	return biofvm_microenvironment->nearest_voxel(const_cast<std::vector<double>&>(position));
}

// Density vector access
double* Microenvironment_Adapter::density_vector(int n)
{
	return (*biofvm_microenvironment)(n).data();
}

double* Microenvironment_Adapter::density_vector(int i, int j)
{
	return (*biofvm_microenvironment)(i, j).data();
}

double* Microenvironment_Adapter::density_vector(int i, int j, int k)
{
	return (*biofvm_microenvironment)(i, j, k).data();
}

double* Microenvironment_Adapter::nearest_density_vector(const std::vector<double>& position)
{
	return biofvm_microenvironment->nearest_density_vector(position);
}

double* Microenvironment_Adapter::nearest_density_vector(int voxel_index)
{
	return biofvm_microenvironment->nearest_density_vector(voxel_index);
}

const double* Microenvironment_Adapter::density_vector(int n) const
{
	return (*biofvm_microenvironment)(n).data();
}

// Gradient computation and access
void Microenvironment_Adapter::compute_gradient_vector(int n)
{
	biofvm_microenvironment->compute_gradient_vector(n);
}

void Microenvironment_Adapter::compute_all_gradient_vectors()
{
	biofvm_microenvironment->compute_all_gradient_vectors();
}

void Microenvironment_Adapter::reset_all_gradient_vectors()
{
	biofvm_microenvironment->reset_all_gradient_vectors();
}

std::vector<gradient>& Microenvironment_Adapter::gradient_vector(int n)
{
	return biofvm_microenvironment->gradient_vector(n);
}

std::vector<gradient>& Microenvironment_Adapter::gradient_vector(int i, int j)
{
	return biofvm_microenvironment->gradient_vector(i, j);
}

std::vector<gradient>& Microenvironment_Adapter::gradient_vector(int i, int j, int k)
{
	return biofvm_microenvironment->gradient_vector(i, j, k);
}

std::vector<gradient>& Microenvironment_Adapter::nearest_gradient_vector(const std::vector<double>& position)
{
	// Need to cast away const for BioFVM's API
	return biofvm_microenvironment->nearest_gradient_vector(const_cast<std::vector<double>&>(position));
}

// Simulation methods
void Microenvironment_Adapter::simulate_time_step(double dt)
{
	biofvm_microenvironment->simulate_diffusion_decay(dt);
	biofvm_microenvironment->simulate_cell_sources_and_sinks(dt);
	// biofvm_microenvironment->simulate_bulk_sources_and_sinks(dt);
}

void Microenvironment_Adapter::simulate_diffusion_decay(double dt)
{
	biofvm_microenvironment->simulate_diffusion_decay(dt);
}

void Microenvironment_Adapter::simulate_bulk_sources_and_sinks(double dt)
{
	biofvm_microenvironment->simulate_bulk_sources_and_sinks(dt);
}

void Microenvironment_Adapter::simulate_cell_sources_and_sinks(double dt)
{
	biofvm_microenvironment->simulate_cell_sources_and_sinks(dt);
}

// Dirichlet boundary conditions
void Microenvironment_Adapter::add_dirichlet_node(int voxel_index, std::vector<double>& value)
{
	biofvm_microenvironment->add_dirichlet_node(voxel_index, value);
}

void Microenvironment_Adapter::update_dirichlet_node(int voxel_index, std::vector<double>& new_value)
{
	biofvm_microenvironment->update_dirichlet_node(voxel_index, new_value);
}

void Microenvironment_Adapter::update_dirichlet_node(int voxel_index, int substrate_index, double new_value)
{
	biofvm_microenvironment->update_dirichlet_node(voxel_index, substrate_index, new_value);
}

void Microenvironment_Adapter::remove_dirichlet_node(int voxel_index)
{
	biofvm_microenvironment->remove_dirichlet_node(voxel_index);
}

void Microenvironment_Adapter::apply_dirichlet_conditions()
{
	biofvm_microenvironment->apply_dirichlet_conditions();
}

void Microenvironment_Adapter::set_substrate_dirichlet_activation(int substrate_index, bool new_value)
{
	biofvm_microenvironment->set_substrate_dirichlet_activation(substrate_index, new_value);
}

void Microenvironment_Adapter::set_substrate_dirichlet_activation(int substrate_index, int index, bool new_value)
{
	biofvm_microenvironment->set_substrate_dirichlet_activation(substrate_index, index, new_value);
}

void Microenvironment_Adapter::set_substrate_dirichlet_activation(int index, std::vector<bool>& new_value)
{
	biofvm_microenvironment->set_substrate_dirichlet_activation(index, new_value);
}

bool Microenvironment_Adapter::get_substrate_dirichlet_activation(int substrate_index) const
{
	return biofvm_microenvironment->get_substrate_dirichlet_activation(substrate_index);
}

bool Microenvironment_Adapter::get_substrate_dirichlet_activation(int substrate_index, int index) const
{
	return biofvm_microenvironment->get_substrate_dirichlet_activation(substrate_index, index);
}

double Microenvironment_Adapter::get_substrate_dirichlet_value(int substrate_index, int index) const
{
	return biofvm_microenvironment->get_substrate_dirichlet_value(substrate_index, index);
}

bool& Microenvironment_Adapter::is_dirichlet_node(int voxel_index)
{
	return biofvm_microenvironment->is_dirichlet_node(voxel_index);
}

// Mesh access
const Cartesian_Mesh& Microenvironment_Adapter::get_mesh() const
{
	return biofvm_microenvironment->mesh;
}

Cartesian_Mesh& Microenvironment_Adapter::get_mesh()
{
	return biofvm_microenvironment->mesh;
}

// Agent container access
Agent_Container* Microenvironment_Adapter::get_agent_container()
{
	return biofvm_microenvironment->agent_container;
}

const Agent_Container* Microenvironment_Adapter::get_agent_container() const
{
	return biofvm_microenvironment->agent_container;
}

void Microenvironment_Adapter::set_agent_container(Agent_Container* container)
{
	biofvm_microenvironment->agent_container = container;
}

// Metadata access
std::vector<std::string>& Microenvironment_Adapter::get_density_names()
{
	return biofvm_microenvironment->density_names;
}

const std::vector<std::string>& Microenvironment_Adapter::get_density_names() const
{
	return biofvm_microenvironment->density_names;
}

std::vector<std::string>& Microenvironment_Adapter::get_density_units()
{
	return biofvm_microenvironment->density_units;
}

const std::vector<std::string>& Microenvironment_Adapter::get_density_units() const
{
	return biofvm_microenvironment->density_units;
}

std::vector<double>& Microenvironment_Adapter::get_diffusion_coefficients()
{
	return biofvm_microenvironment->diffusion_coefficients;
}

const double* Microenvironment_Adapter::get_diffusion_coefficients() const
{
	return biofvm_microenvironment->diffusion_coefficients.data();
}

std::vector<double>& Microenvironment_Adapter::get_decay_rates()
{
	return biofvm_microenvironment->decay_rates;
}

const double* Microenvironment_Adapter::get_decay_rates() const
{
	return biofvm_microenvironment->decay_rates.data();
}

// Name access
std::string& Microenvironment_Adapter::get_name()
{
	return biofvm_microenvironment->name;
}

const std::string& Microenvironment_Adapter::get_name() const
{
	return biofvm_microenvironment->name;
}

// Display and I/O
void Microenvironment_Adapter::display_information(std::ostream& os) const
{
	biofvm_microenvironment->display_information(os);
}

void Microenvironment_Adapter::write_to_matlab(std::string filename)
{
	biofvm_microenvironment->write_to_matlab(filename);
}

// Spatial setup methods
void Microenvironment_Adapter::resize_space(int x_nodes, int y_nodes, int z_nodes)
{
	biofvm_microenvironment->resize_space(x_nodes, y_nodes, z_nodes);
}

void Microenvironment_Adapter::resize_space(double x_start, double x_end, double y_start, double y_end,
                                                   double z_start, double z_end, int x_nodes, int y_nodes, int z_nodes)
{
	biofvm_microenvironment->resize_space(x_start, x_end, y_start, y_end, z_start, z_end, x_nodes, y_nodes, z_nodes);
}

void Microenvironment_Adapter::resize_space(double x_start, double x_end, double y_start, double y_end,
                                                   double z_start, double z_end, double dx_new, double dy_new, double dz_new)
{
	biofvm_microenvironment->resize_space(x_start, x_end, y_start, y_end, z_start, z_end, dx_new, dy_new, dz_new);
}

void Microenvironment_Adapter::resize_space_uniform(double x_start, double x_end, double y_start, double y_end,
                                                           double z_start, double z_end, double dx_new)
{
	biofvm_microenvironment->resize_space_uniform(x_start, x_end, y_start, y_end, z_start, z_end, dx_new);
}

void Microenvironment_Adapter::resize_voxels(int new_number_of_voxels)
{
	biofvm_microenvironment->resize_voxels(new_number_of_voxels);
}

// Update methods
void Microenvironment_Adapter::update_rates()
{
	biofvm_microenvironment->update_rates();
}

// Configuration query methods
bool Microenvironment_Adapter::simulate_2D() const
{
	return default_microenvironment_options.simulate_2D;
}

bool Microenvironment_Adapter::calculate_gradients() const
{
	return default_microenvironment_options.calculate_gradients;
}

bool Microenvironment_Adapter::setup_microenvironment_from_XML( const std::string& filename )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( filename.c_str() );
	if ( !result )
	{
		std::cerr << "Error: Could not load XML file " << filename << ": " << result.description() << std::endl;
		return false;
	}
	pugi::xml_node root_node = doc.child("PhysiCell_settings");

	return setup_microenvironment_from_XML_node(root_node);
};

void Microenvironment_Adapter::initialize()
{
	initialize_microenvironment();
}

// Static adapter instance that wraps the global BioFVM microenvironment
static Microenvironment_Adapter* global_adapter = nullptr;

Microenvironment_Interface* get_microenvironment_i()
{
	return global_adapter;
}

void initialize_microenvironment_interface()
{
	// Get the global BioFVM microenvironment
	Microenvironment* biofvm_env = &microenvironment;

	// Create an adapter wrapping it (don't take ownership since it's global)
	if (global_adapter == nullptr)
	{
		global_adapter = new Microenvironment_Adapter(biofvm_env, false);
	}
}

void BioFVM::BioFVM_Implementation::initialize_microenvironment(){
    initialize_microenvironment_interface();
}

BioFVM::Basic_Agent_Interface* BioFVM::BioFVM_Implementation::create_basic_agent(){
    return new BioFVM::Basic_Agent_Adapter(new BioFVM::Basic_Agent(), true);
}

std::vector<Basic_Agent_Interface*>* BioFVM::BioFVM_Implementation::get_all_basic_agents(){
    return (std::vector<Basic_Agent_Interface*>*)&all_basic_agents;
}
} // namespace BioFVM
