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

#include <map>

void create_cell_types(void)
{
	// set the random seed 
	if (parameters.ints.find_index("random_seed") != -1)
	{
		SeedRandom(parameters.ints("random_seed"));
	}
	
	/*
	   Put any modifications to default cell definition here if you
	   want to have "inherited" by other cell types.

	   This is a good place to set default functions.
	*/

	// PhysiCell's default cell definition (standard cycle and death models, default
	// functions, SVG plotting). Required; the lines below override some of it.
	initialize_default_cell_definition();
	// Inactive: this sync sat after the comment on the line above and never ran.
	// It is redundant, since initialize_default_cell_definition() already syncs
	// secretion to the microenvironment.
	// cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
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

	setup_behavior_rules();

	/*
	   Put any modifications to individual cell definitions here.

	   This is a good place to set custom functions.
	*/

	cell_defaults.functions.update_phenotype = phenotype_function;
	cell_defaults.functions.contact_function = contact_function;

	// using biofvm to do ecm_density

	/*
	   This builds the map of cell definitions and summarizes the setup.
	*/

	display_cell_definitions(std::cout);

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

void setup_tissue()
{
	// Disabled: setup_tissue_domain() read the domain bounds into locals and never
	// used them.
	// setup_tissue_domain();
	// load cells from your CSV file (if enabled)
	// load_initial_cells() rather than load_cells_from_pugixml(): it honors a
	// -i path when one is passed (as PCMM does, one IC folder per ROI) and
	// otherwise falls through to the config's <cell_positions> block, so
	// standalone runs behave exactly as before.
	load_initial_cells();

	return;
}

// Disabled, kept for reference: computed the domain bounds and ranges but never
// used them. See setup_tissue().
/*
void setup_tissue_domain(void)
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
}
*/

// Model-specific SVG palette, keyed by cell type NAME.
//
// This lives here rather than in modules/PhysiCell_pathology.cpp deliberately.
// The palette was originally applied there (commit 124a30be, "colors") and was
// silently destroyed when the drbergman 1.14.2 merge rewrote
// paint_by_number_cell_coloring(). user_projects/antigen_presentation is ours
// alone, so upstream merges cannot clobber it.
//
// Keyed by name, not index, so reordering or adding a <cell_definition> cannot
// silently shift every color. The htan_singlecell model shares these names but
// has no duct_filler; unlisted names fall back to the engine default.
//
// Keep in sync with PhysiCell-Studio/bin/cmaps.py (paint_clist) and the copies
// under data/inputs/custom_codes/. tools/check_cell_colors.py verifies this.
static const std::map<std::string, std::string> kCellTypeColors = {
	{"epithelial_normal", "palegreen"},
	{"mesenchymal_normal", "lightcyan"},
	{"CAF", "yellow"},
	{"epithelial_tumor", "green"},
	{"mesenchymal_tumor", "blue"},
	{"other_tissue", "magenta"},
	{"CD4_Tcell", "darkorange"},
	{"CD8_Tcell", "maroon"},
	{"Treg", "plum"},
	{"CD8_exhausted", "lightcoral"},
	{"B cell", "papayawhip"},
	{"macrophage", "lightpink"},
	{"epithelial_tumor_class1", "chartreuse"},
	{"epithelial_tumor_class1_class2", "darkolivegreen"},
	{"epithelial_tumor_class2", "seagreen"},
	{"mesenchymal_tumor_class1", "lightskyblue"},
	{"mesenchymal_tumor_class1_class2", "dodgerblue"},
	{"mesenchymal_tumor_class2", "royalblue"},
	{"apCAF", "grey"},
	{"PDAC_unclassified", "white"},
	{"duct_filler", "tan"}};

std::vector<std::string> my_coloring_function(Cell *pCell)
{
	auto it = kCellTypeColors.find(pCell->type_name);
	if (it == kCellTypeColors.end())
	{
		// Unknown type: defer to the engine rather than guessing.
		return paint_by_number_cell_coloring(pCell);
	}

	std::string interior_color = it->second;

	// Preserve the engine's death-state semantics -- without this, dead cells
	// would render in their live color. Mirrors paint_by_number_cell_coloring().
	int phase_code = pCell->phenotype.cycle.current_phase().code;
	if (phase_code == PhysiCell_constants::necrotic_swelling ||
		phase_code == PhysiCell_constants::necrotic_lysed ||
		phase_code == PhysiCell_constants::necrotic)
	{
		interior_color = "saddlebrown";
	}
	if (phase_code == PhysiCell_constants::apoptotic)
	{
		interior_color = "black";
	}

	// {cytoplasm fill, cytoplasm outline, nucleus fill, nucleus outline}
	return {interior_color, "black", interior_color, interior_color};
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
