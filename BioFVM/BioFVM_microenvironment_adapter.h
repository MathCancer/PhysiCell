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

#ifndef __BioFVM_microenvironment_adapter_h__
#define __BioFVM_microenvironment_adapter_h__

#include "BioFVM_microenvironment_interface.h"

namespace BioFVM{

class Microenvironment;

/**
 * @brief Adapter class that wraps BioFVM::Microenvironment
 *
 * This class implements the Microenvironment_Interface using BioFVM
 * as the underlying implementation. It delegates all method calls to
 * the wrapped BioFVM::Microenvironment object.
 */
class Microenvironment_Adapter : public Microenvironment_Interface
{
private:
	BioFVM::Microenvironment* biofvm_microenvironment;
	bool owns_microenvironment;  // Track if we should delete the wrapped object

public:
	/**
	 * @brief Constructor that wraps an existing BioFVM::Microenvironment
	 * @param env Pointer to the BioFVM microenvironment to wrap
	 * @param take_ownership If true, this adapter will delete the microenvironment when destroyed
	 */
	Microenvironment_Adapter(BioFVM::Microenvironment* env, bool take_ownership = false);

	/**
	 * @brief Constructor that creates a new BioFVM::Microenvironment
	 */
	Microenvironment_Adapter();

	/**
	 * @brief Destructor
	 */
	virtual ~Microenvironment_Adapter();

	/**
	 * @brief Get the underlying BioFVM::Microenvironment pointer
	 */
	BioFVM::Microenvironment* get_biofvm_microenvironment() { return biofvm_microenvironment; }

	/**
	 * @brief Get the underlying BioFVM::Microenvironment pointer (const)
	 */
	const BioFVM::Microenvironment* get_biofvm_microenvironment() const { return biofvm_microenvironment; }

	// ========================================================================
	// Implementation of Microenvironment_Interface methods
	// ========================================================================

	// Units access
	std::string& get_time_units() override;
	const std::string& get_time_units() const override;
	std::string& get_spatial_units() override;
	const std::string& get_spatial_units() const override;

	// Query methods
	unsigned int number_of_densities() const override;
	unsigned int number_of_voxels() const override;
	unsigned int number_of_voxel_faces() const override;

	// Substrate/Density management
	int find_density_index(const std::string& name) const override;
	void add_density() override;
	void add_density(const std::string& name, const std::string& units) override;
	void add_density(const std::string& name, const std::string& units,
	                 double diffusion_constant, double decay_rate) override;
	void set_density(int index, const std::string& name, const std::string& units) override;
	void set_density(int index, const std::string& name, const std::string& units,
	                 double diffusion_constant, double decay_rate) override;
	void resize_densities(int new_size) override;

	// Voxel/Position access
	int voxel_index(int i, int j, int k) const override;
	std::vector<unsigned int> cartesian_indices(int n) const override;
	int nearest_voxel_index(const std::vector<double>& position) const override;
	std::vector<unsigned int> nearest_cartesian_indices(const std::vector<double>& position) const override;
	Voxel& voxels(int voxel_index) override;
	const Voxel& voxels(int voxel_index) const override;
	Voxel& nearest_voxel(const std::vector<double>& position) override;

	// Density vector access
	double* density_vector(int n) override;
	double* density_vector(int i, int j) override;
	double* density_vector(int i, int j, int k) override;
	double* nearest_density_vector(const std::vector<double>& position) override;
	double* nearest_density_vector(int voxel_index) override;
	const double* density_vector(int n) const override;

	// Gradient computation and access
	void compute_gradient_vector(int n) override;
	void compute_all_gradient_vectors() override;
	void reset_all_gradient_vectors() override;
	std::vector<std::vector<double>>& gradient_vector(int n) override;
	std::vector<std::vector<double>>& gradient_vector(int i, int j) override;
	std::vector<std::vector<double>>& gradient_vector(int i, int j, int k) override;
	std::vector<std::vector<double>>& nearest_gradient_vector(const std::vector<double>& position) override;

	// Simulation methods
	void simulate_time_step(double dt) override;
	void simulate_diffusion_decay(double dt) override;
	void simulate_bulk_sources_and_sinks(double dt) override;
	void simulate_cell_sources_and_sinks(double dt) override;

	// Dirichlet boundary conditions
	void add_dirichlet_node(int voxel_index, std::vector<double>& value) override;
	void update_dirichlet_node(int voxel_index, std::vector<double>& new_value) override;
	void update_dirichlet_node(int voxel_index, int substrate_index, double new_value) override;
	void remove_dirichlet_node(int voxel_index) override;
	void apply_dirichlet_conditions() override;
	void set_substrate_dirichlet_activation(int substrate_index, bool new_value) override;
	void set_substrate_dirichlet_activation(int substrate_index, int index, bool new_value) override;
	void set_substrate_dirichlet_activation(int index, std::vector<bool>& new_value) override;
	bool get_substrate_dirichlet_activation(int substrate_index) const override;
	bool get_substrate_dirichlet_activation(int substrate_index, int index) const override;
	double get_substrate_dirichlet_value(int substrate_index, int index) const override;
	bool& is_dirichlet_node(int voxel_index) override;

	// Mesh access
	const Cartesian_Mesh& get_mesh() const override;
	Cartesian_Mesh& get_mesh() override;

	// Agent container access
	Agent_Container* get_agent_container() override;
	const Agent_Container* get_agent_container() const override;
	void set_agent_container(Agent_Container* container) override;

	// Metadata access
	std::vector<std::string>& get_density_names() override;
	const std::vector<std::string>& get_density_names() const override;
	std::vector<std::string>& get_density_units() override;
	const std::vector<std::string>& get_density_units() const override;
	std::vector<double>& get_diffusion_coefficients() override;
	const double* get_diffusion_coefficients() const override;
	std::vector<double>& get_decay_rates() override;
	const double* get_decay_rates() const override;

	// Name access
	std::string& get_name() override;
	const std::string& get_name() const override;

	// Display and I/O
	void display_information(std::ostream& os) const override;
	void write_to_matlab(std::string filename) override;

	// Spatial setup methods
	void resize_space(int x_nodes, int y_nodes, int z_nodes) override;
	void resize_space(double x_start, double x_end, double y_start, double y_end,
	                  double z_start, double z_end, int x_nodes, int y_nodes, int z_nodes) override;
	void resize_space(double x_start, double x_end, double y_start, double y_end,
	                  double z_start, double z_end, double dx_new, double dy_new, double dz_new) override;
	void resize_space_uniform(double x_start, double x_end, double y_start, double y_end,
	                          double z_start, double z_end, double dx_new) override;
	void resize_voxels(int new_number_of_voxels) override;

	// Update methods
	void update_rates() override;

	// Configuration query methods
	bool simulate_2D() const override;
	bool calculate_gradients() const override;

	bool setup_microenvironment_from_XML( const std::string& filename ) override;
	void initialize() override;
};

} // namespace BioFVM

#endif // __BioFVM_microenvironment_adapter_h__
