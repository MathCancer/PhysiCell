/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2021, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./custom.h"

void create_cell_types(void)
{
	// set the random seed
	SeedRandom(parameters.ints("random_seed"));

	/*
	   Put any modifications to default cell definition here if you
	   want to have "inherited" by other cell types.

	   This is a good place to set default functions.
	*/

	initialize_default_cell_definition();
	cell_defaults.phenotype.secretion.sync_to_microenvironment(&microenvironment);

	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL;
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based;
	cell_defaults.functions.custom_cell_rule = NULL;
	cell_defaults.functions.contact_function = NULL;

	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL;
	cell_defaults.functions.calculate_distance_to_membrane = NULL;

	/*
	   This parses the cell definitions in the XML config file.
	*/

	initialize_cell_definitions_from_pugixml();

	/*
	   This builds the map of cell definitions and summarizes the setup.
	*/

	build_cell_definitions_maps();

	/*
	   This intializes cell signal and response dictionaries
	*/

	setup_signal_behavior_dictionaries();

	/*
	   Cell rule definitions
	*/

	setup_cell_rules();

	/*
	   Put any modifications to individual cell definitions here.

	   This is a good place to set custom functions.
	*/

	cell_defaults.functions.update_phenotype = phenotype_function; // phenotype_function;
	cell_defaults.functions.custom_cell_rule = custom_function;
	cell_defaults.functions.contact_function = contact_function;

	// now apply the ruelset function to all cell definitions
	// for (int n = 0; n < cell_definitions_by_index.size(); n++)
	// {
	// 	cell_definitions_by_index[n]->functions.update_phenotype = rule_phenotype_function;
	// }

	/*
	   This builds the map of cell definitions and summarizes the setup.
	*/

	// display_cell_definitions( std::cout );

	display_cell_definitions(std::cout);

	// save_annotated_detailed_English_rules();
	// save_annotated_detailed_English_rules_HTML();
	// save_annotated_English_rules();
	// save_annotated_English_rules_HTML();

	return;
}

void setup_microenvironment(void)
{
	// set domain parameters

	// put any custom code to set non-homogeneous initial conditions or
	// extra Dirichlet nodes here.

	// initialize BioFVM

	initialize_microenvironment();

	return;
}

void setup_tissue(void)
{
	double Xmin = microenvironment.mesh.bounding_box[0];
	double Ymin = microenvironment.mesh.bounding_box[1];
	double Zmin = microenvironment.mesh.bounding_box[2];

	double Xmax = microenvironment.mesh.bounding_box[3];
	double Ymax = microenvironment.mesh.bounding_box[4];
	double Zmax = microenvironment.mesh.bounding_box[5];

	if (default_microenvironment_options.simulate_2D == true)
	{
		Zmin = 0.0;
		Zmax = 0.0;
	}

	double Xrange = Xmax - Xmin;
	double Yrange = Ymax - Ymin;
	double Zrange = Zmax - Zmin;

	double Xmiddle = 0.5 * (Xmin + Xmax);
	double Ymiddle = 0.5 * (Ymin + Ymax);
	double Zmiddle = 0.5 * (Zmin + Zmax);

	std::vector<double> center = {Xmiddle, Ymiddle, Zmiddle};

	double radius = std::min(Xrange, Yrange);
	if (Zrange > microenvironment.mesh.dz - 1e-5)
	{
		radius = std::min(radius, Zrange);
	}
	radius *= 0.5;

	// create some of each type of cell

	Cell *pC;

	double r1_default = 0;
	double r2_default = radius;

	std::string optional_parameter_name = "min_position_cells";
	if (parameters.doubles.find_index(optional_parameter_name) > -1)
	{
		r1_default = parameters.doubles(optional_parameter_name);
	}

	optional_parameter_name = "max_position_cells";
	if (parameters.doubles.find_index(optional_parameter_name) > -1)
	{
		r2_default = parameters.doubles(optional_parameter_name);
	}

	for (int k = 0; k < cell_definitions_by_index.size(); k++)
	{
		Cell_Definition *pCD = cell_definitions_by_index[k];

		int number_of_cells = parameters.ints("number_of_cells");

		// optional: number_of_{cell type X} : number of cells of this particular type

		optional_parameter_name = "number_of_" + pCD->name;
		// spaces_to_underscore(optional_parameter_name);
		if (parameters.ints.find_index(optional_parameter_name) > -1)
		{
			number_of_cells = parameters.ints(optional_parameter_name);
		}

		std::cout << "Placing " << number_of_cells << " cells of type " << pCD->name << " ... " << std::endl;

		double r1 = r1_default;
		optional_parameter_name = "min_position_" + pCD->name;
		// spaces_to_underscore(optional_parameter_name);
		if (parameters.doubles.find_index(optional_parameter_name) > -1)
		{
			r1 = parameters.doubles(optional_parameter_name);
		}

		double r2 = r2_default;
		optional_parameter_name = "max_position_" + pCD->name;
		// spaces_to_underscore(optional_parameter_name);
		if (parameters.doubles.find_index(optional_parameter_name) > -1)
		{
			r2 = parameters.doubles(optional_parameter_name);
		}

		for (int n = 0; n < number_of_cells; n++)
		{
			std::vector<double> position;
			if (default_microenvironment_options.simulate_2D)
			{
				position = UniformInAnnulus(r1, r2);
			}
			else
			{
				position = UniformInShell(r1, r2);
			}

			position += center;
			/*
			position[0] = Xmin + UniformRandom()*Xrange;
			position[1] = Ymin + UniformRandom()*Yrange;
			position[2] = Zmin + UniformRandom()*Zrange;
			*/

			pC = create_cell(*pCD);
			pC->assign_position(position);
		}
	}
	std::cout << std::endl;

	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml();

	return;
}

std::vector<std::string> my_coloring_function(Cell *pCell)
{
	std::vector<std::string> output = paint_by_number_cell_coloring(pCell);

	output[0] = output[2];
	return output;
}

void phenotype_function(Cell *pCell, Phenotype &phenotype, double dt)
{
	return;
}

void custom_function(Cell *pCell, Phenotype &phenotype, double dt)
{
	return;
}

void contact_function(Cell *pMe, Phenotype &phenoMe, Cell *pOther, Phenotype &phenoOther, double dt)
{
	return;
}

void turn_on_systemic_therapy(std::string substrate, double time_on, double time_off, double dose)
{

	static bool therapy_on = false;
	static double tol = 0.001;

	if (PhysiCell_globals.current_time >= time_on - tol && therapy_on == false)
	{
		therapy_on = true;
		set_Dirichlet_boundary(substrate, dose, true);
	}

	if (PhysiCell_globals.current_time >= time_off - tol && therapy_on == true)
	{
		therapy_on = false;
		set_Dirichlet_boundary(substrate, dose, false);
	}

	return;
}

void set_Dirichlet_boundary(std::string substrate, double value, bool activated)
{
	static int substrate_index = microenvironment.find_density_index(substrate);

	// add the Dirichlet nodes in the right places
	// now, go in and set the values
	for (unsigned int k = 0; k < microenvironment.mesh.z_coordinates.size(); k++)
	{
		int I = 0;
		// set Dirichlet conditions along the xmin outer edges
		for (unsigned int j = 0; j < microenvironment.mesh.y_coordinates.size(); j++)
		{
			int voxel_index = microenvironment.voxel_index(I, j, k);

			if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
			{
				microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
			}

			microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
			microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
		}
	}

	for (unsigned int k = 0; k < microenvironment.mesh.z_coordinates.size(); k++)
	{
		int I = microenvironment.mesh.x_coordinates.size() - 1;
		;
		// set Dirichlet conditions along the xmax outer edges
		for (unsigned int j = 0; j < microenvironment.mesh.y_coordinates.size(); j++)
		{
			int voxel_index = microenvironment.voxel_index(I, j, k);

			if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
			{
				microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
			}

			microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
			microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
		}
	}

	for (unsigned int k = 0; k < microenvironment.mesh.z_coordinates.size(); k++)
	{
		int J = 0; // microenvironment.mesh.x_coordinates.size()-1;;
		// set Dirichlet conditions along the ymin outer edges
		for (unsigned int i = 0; i < microenvironment.mesh.x_coordinates.size(); i++)
		{
			int voxel_index = microenvironment.voxel_index(i, J, k);

			if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
			{
				microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
			}
			microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
			microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
		}
	}

	for (unsigned int k = 0; k < microenvironment.mesh.z_coordinates.size(); k++)
	{
		int J = microenvironment.mesh.y_coordinates.size() - 1;
		;
		// set Dirichlet conditions along the ymin outer edges
		for (unsigned int i = 0; i < microenvironment.mesh.x_coordinates.size(); i++)
		{
			int voxel_index = microenvironment.voxel_index(i, J, k);

			if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
			{
				microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
			}
			microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
			microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
		}
	}

	// if not 2D:
	if (default_microenvironment_options.simulate_2D == false)
	{
		for (unsigned int j = 0; j < microenvironment.mesh.y_coordinates.size(); j++)
		{
			int K = 0; // microenvironment.mesh.z_coordinates.size()-1;;
			// set Dirichlet conditions along the ymin outer edges
			for (unsigned int i = 0; i < microenvironment.mesh.x_coordinates.size(); i++)
			{
				int voxel_index = microenvironment.voxel_index(i, j, K);

				if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
				{
					microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
				}
				microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
				microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
			}
		}

		for (unsigned int j = 0; j < microenvironment.mesh.y_coordinates.size(); j++)
		{
			int K = microenvironment.mesh.z_coordinates.size() - 1;
			;
			// set Dirichlet conditions along the ymin outer edges
			for (unsigned int i = 0; i < microenvironment.mesh.x_coordinates.size(); i++)
			{
				int voxel_index = microenvironment.voxel_index(i, j, K);

				if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && activated == true)
				{
					microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
				}
				microenvironment.update_dirichlet_node(voxel_index, substrate_index, value);
				microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, activated);
			}
		}
	}
	return;
}

void turn_on_injected_therapy(std::string substrate, std::vector<double> position, double time_on, double time_off, double dose)
{
	int substrate_index = microenvironment.find_density_index(substrate);
	int voxel_index = microenvironment.nearest_voxel_index(position);

	static bool therapy_on = false;
	static double tol = 0.001;

	if (PhysiCell_globals.current_time >= time_on - tol && therapy_on == false)
	{
		therapy_on = true;

		if (microenvironment.mesh.voxels[voxel_index].is_Dirichlet == false && therapy_on == true)
		{
			microenvironment.mesh.voxels[voxel_index].is_Dirichlet = true;
		}
		microenvironment.update_dirichlet_node(voxel_index, substrate_index, dose);
		microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, true);
	}

	if (PhysiCell_globals.current_time >= time_off - tol && therapy_on == true)
	{
		therapy_on = false;

		microenvironment.update_dirichlet_node(voxel_index, substrate_index, dose);
		microenvironment.set_substrate_dirichlet_activation(substrate_index, voxel_index, false);
	}
}