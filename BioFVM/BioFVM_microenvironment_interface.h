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

#ifndef __BioFVM_microenvironment_interface_h__
#define __BioFVM_microenvironment_interface_h__

#include <string>
#include <vector>
#include "BioFVM_agent_container.h"
#include "BioFVM_mesh.h"
#include "pugixml.hpp"

namespace BioFVM{

/**
 * @brief Abstract interface for microenvironment implementations
 *
 * This interface provides an abstraction layer around the microenvironment,
 * allowing different implementations (e.g., BioFVM) to be plugged in without
 * modifying the rest of the codebase.
 */
class Microenvironment_Interface
{
public:
	virtual ~Microenvironment_Interface() = default;

	// ========================================================================
	// Units access
	// ========================================================================

	/** @brief Get time units */
	virtual std::string& get_time_units() = 0;

	/** @brief Get const time units */
	virtual const std::string& get_time_units() const = 0;

	/** @brief Get spatial units */
	virtual std::string& get_spatial_units() = 0;

	/** @brief Get const spatial units */
	virtual const std::string& get_spatial_units() const = 0;

	// ========================================================================
	// Query methods - Size information
	// ========================================================================

	/** @brief Get the number of density substrates */
	virtual unsigned int number_of_densities() const = 0;

	/** @brief Get the number of voxels in the mesh */
	virtual unsigned int number_of_voxels() const = 0;

	/** @brief Get the number of voxel faces */
	virtual unsigned int number_of_voxel_faces() const = 0;

	// ========================================================================
	// Substrate/Density management
	// ========================================================================

	/** @brief Find the index of a substrate by name */
	virtual int find_density_index(const std::string& name) const = 0;

	/** @brief Add a new density substrate */
	virtual void add_density() = 0;

	/** @brief Add a new density with name and units */
	virtual void add_density(const std::string& name, const std::string& units) = 0;

	/** @brief Add a new density with full parameters */
	virtual void add_density(const std::string& name, const std::string& units,
	                         double diffusion_constant, double decay_rate) = 0;

	/** @brief Set density properties at given index */
	virtual void set_density(int index, const std::string& name, const std::string& units) = 0;

	/** @brief Set density properties with coefficients at given index */
	virtual void set_density(int index, const std::string& name, const std::string& units,
	                         double diffusion_constant, double decay_rate) = 0;

	/** @brief Resize the number of densities */
	virtual void resize_densities(int new_size) = 0;

	// ========================================================================
	// Voxel/Position access
	// ========================================================================

	/** @brief Get the voxel index from Cartesian indices */
	virtual int voxel_index(int i, int j, int k) const = 0;

	/** @brief Get Cartesian indices from voxel index */
	virtual std::vector<unsigned int> cartesian_indices(int n) const = 0;

	/** @brief Find the nearest voxel index to a position */
	virtual int nearest_voxel_index(const std::vector<double>& position) const = 0;

	/** @brief Get the nearest Cartesian indices to a position */
	virtual std::vector<unsigned int> nearest_cartesian_indices(const std::vector<double>& position) const = 0;

	/** @brief Get a reference to a voxel by index */
	virtual Voxel& voxels(int voxel_index) = 0;

	/** @brief Get a const reference to a voxel by index */
	virtual const Voxel& voxels(int voxel_index) const = 0;

	/** @brief Get a reference to the nearest voxel to a position */
	virtual Voxel& nearest_voxel(const std::vector<double>& position) = 0;

	// ========================================================================
	// Density vector access
	// ========================================================================

	/** @brief Get density vector at voxel index */
	virtual double* density_vector(int n) = 0;

	/** @brief Get density vector at Cartesian position (i,j) */
	virtual double* density_vector(int i, int j) = 0;

	/** @brief Get density vector at Cartesian position (i,j,k) */
	virtual double* density_vector(int i, int j, int k) = 0;

	/** @brief Get the nearest density vector to a position */
	virtual double* nearest_density_vector(const std::vector<double>& position) = 0;

	/** @brief Get the nearest density vector by voxel index */
	virtual double* nearest_density_vector(int voxel_index) = 0;

	/** @brief Const version of density_vector at voxel index */
	virtual const double* density_vector(int n) const = 0;

	// ========================================================================
	// Gradient computation and access
	// ========================================================================

	/** @brief Compute gradient at a specific voxel */
	virtual void compute_gradient_vector(int n) = 0;

	/** @brief Compute all gradient vectors */
	virtual void compute_all_gradient_vectors() = 0;

	/** @brief Reset all gradient vectors */
	virtual void reset_all_gradient_vectors() = 0;

	/** @brief Get gradient vector at voxel index */
	virtual std::vector<std::vector<double>>& gradient_vector(int n) = 0;

	/** @brief Get gradient vector at Cartesian position (i,j) */
	virtual std::vector<std::vector<double>>& gradient_vector(int i, int j) = 0;

	/** @brief Get gradient vector at Cartesian position (i,j,k) */
	virtual std::vector<std::vector<double>>& gradient_vector(int i, int j, int k) = 0;

	/** @brief Get the nearest gradient vector to a position */
	virtual std::vector<std::vector<double>>& nearest_gradient_vector(const std::vector<double>& position) = 0;

	// ========================================================================
	// Simulation methods
	// ========================================================================

	/** @brief Simulate a time step of duration dt */
	virtual void simulate_time_step(double dt) = 0;

	/** @brief Simulate diffusion and decay for time step dt */
	virtual void simulate_diffusion_decay(double dt) = 0;

	/** @brief Simulate bulk sources and sinks for time step dt */
	virtual void simulate_bulk_sources_and_sinks(double dt) = 0;

	/** @brief Simulate cell sources and sinks using global agent list */
	virtual void simulate_cell_sources_and_sinks(double dt) = 0;

	// ========================================================================
	// Dirichlet boundary conditions
	// ========================================================================

	/** @brief Add a Dirichlet node at a voxel */
	virtual void add_dirichlet_node(int voxel_index, std::vector<double>& value) = 0;

	/** @brief Update a Dirichlet node value */
	virtual void update_dirichlet_node(int voxel_index, std::vector<double>& new_value) = 0;

	/** @brief Update a Dirichlet node value for a specific substrate */
	virtual void update_dirichlet_node(int voxel_index, int substrate_index, double new_value) = 0;

	/** @brief Remove a Dirichlet node */
	virtual void remove_dirichlet_node(int voxel_index) = 0;

	/** @brief Apply Dirichlet boundary conditions */
	virtual void apply_dirichlet_conditions() = 0;

	/** @brief Set Dirichlet activation for all nodes of a substrate */
	virtual void set_substrate_dirichlet_activation(int substrate_index, bool new_value) = 0;

	/** @brief Set Dirichlet activation for a specific substrate at a specific voxel */
	virtual void set_substrate_dirichlet_activation(int substrate_index, int index, bool new_value) = 0;

	/** @brief Set Dirichlet activation vector for a voxel */
	virtual void set_substrate_dirichlet_activation(int index, std::vector<bool>& new_value) = 0;

	/** @brief Get Dirichlet activation status for a substrate */
	virtual bool get_substrate_dirichlet_activation(int substrate_index) const = 0;

	/** @brief Get Dirichlet activation status for a substrate at a specific voxel */
	virtual bool get_substrate_dirichlet_activation(int substrate_index, int index) const = 0;

	/** @brief Get Dirichlet value for a substrate at a specific voxel */
	virtual double get_substrate_dirichlet_value(int substrate_index, int index) const = 0;

	/** @brief Check if a voxel is a Dirichlet node */
	virtual bool& is_dirichlet_node(int voxel_index) = 0;

	// ========================================================================
	// Mesh access
	// ========================================================================

	/** @brief Get const reference to the mesh */
	virtual const Cartesian_Mesh& get_mesh() const = 0;

	virtual Cartesian_Mesh& get_mesh() = 0;

	// ========================================================================
	// Agent container access
	// ========================================================================

	/** @brief Get pointer to the agent container */
	virtual Agent_Container* get_agent_container() = 0;

	/** @brief Get const pointer to the agent container */
	virtual const Agent_Container* get_agent_container() const = 0;

	/** @brief Set the agent container */
	virtual void set_agent_container(Agent_Container* container) = 0;

	// ========================================================================
	// Metadata access
	// ========================================================================

	/** @brief Get substrate names */
	virtual std::vector<std::string>& get_density_names() = 0;

	/** @brief Get const substrate names */
	virtual const std::vector<std::string>& get_density_names() const = 0;

	/** @brief Get substrate units */
	virtual std::vector<std::string>& get_density_units() = 0;

	/** @brief Get const substrate units */
	virtual const std::vector<std::string>& get_density_units() const = 0;

	/** @brief Get diffusion coefficients */
	virtual std::vector<double>& get_diffusion_coefficients() = 0;

	/** @brief Get const diffusion coefficients */
	virtual const double* get_diffusion_coefficients() const = 0;

	/** @brief Get decay rates */
	virtual std::vector<double>& get_decay_rates() = 0;

	/** @brief Get const decay rates */
	virtual const double* get_decay_rates() const = 0;

	// ========================================================================
	// Name access
	// ========================================================================

	/** @brief Get microenvironment name */
	virtual std::string& get_name() = 0;

	/** @brief Get const microenvironment name */
	virtual const std::string& get_name() const = 0;

	// ========================================================================
	// Display and I/O
	// ========================================================================

	/** @brief Display microenvironment information */
	virtual void display_information(std::ostream& os) const = 0;

	/** @brief Write microenvironment data to MATLAB file */
	virtual void write_to_matlab(std::string filename) = 0;

	// ========================================================================
	// Spatial setup methods
	// ========================================================================

	/** @brief Resize the spatial domain */
	virtual void resize_space(int x_nodes, int y_nodes, int z_nodes) = 0;

	/** @brief Resize the spatial domain with specific bounds and node counts */
	virtual void resize_space(double x_start, double x_end, double y_start, double y_end,
	                          double z_start, double z_end, int x_nodes, int y_nodes, int z_nodes) = 0;

	/** @brief Resize the spatial domain with specific bounds and spacing */
	virtual void resize_space(double x_start, double x_end, double y_start, double y_end,
	                          double z_start, double z_end, double dx_new, double dy_new, double dz_new) = 0;

	/** @brief Resize the spatial domain uniformly */
	virtual void resize_space_uniform(double x_start, double x_end, double y_start, double y_end,
	                                  double z_start, double z_end, double dx_new) = 0;

	/** @brief Resize the number of voxels */
	virtual void resize_voxels(int new_number_of_voxels) = 0;

	// ========================================================================
	// Update methods
	// ========================================================================

	/** @brief Update supply and uptake rates based on current state */
	virtual void update_rates() = 0;

	// ========================================================================
	// Configuration query methods
	// ========================================================================

	/** @brief Check if simulating in 2D mode */
	virtual bool simulate_2D() const = 0;

	/** @brief Check if gradient calculation is enabled */
	virtual bool calculate_gradients() const = 0;

	/** @brief Setup microenvironment from XML configuration */
	virtual bool setup_microenvironment_from_XML( const std::string& filename ) = 0;

	/** @brief Initialize the microenvironment */
	virtual void initialize() = 0;
};

// Global accessors for the microenvironment interface
Microenvironment_Interface* get_microenvironment_i();

} // namespace BioFVM

#endif // __BioFVM_microenvironment_interface_h__
