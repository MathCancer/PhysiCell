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
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
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

// declare cell definitions here 

Cell_Definition stem_cell; 
Cell_Definition tumor_cell_a; 
Cell_Definition tumor_cell_b; 


void create_cell_types( void )
{
	// use the same random seed so that future experiments have the 
	// same initial histogram of oncoprotein, even if threading means 
	// that future division and other events are still not identical 
	// for all runs 
	
	SeedRandom( parameters.ints("random_seed") ); // or specify a seed here 
	
	// housekeeping 
	
	initialize_default_cell_definition();
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	// Force 2D sim BEFORE using cell_defaults to define our custom cell types
	cell_defaults.functions.set_orientation = up_orientation; 
	cell_defaults.phenotype.geometry.polarity = 1.0;
	cell_defaults.phenotype.motility.restrict_to_2D = true; 
	
	// make sure the defaults are self-consistent. 
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions );
	
	// Name the default cell type 
	
	stem_cell = cell_defaults;
	stem_cell.type = 0;
	stem_cell.phenotype.motility.migration_speed = 0.75;
	stem_cell.parameters.o2_proliferation_threshold = 30;
	
	tumor_cell_a = cell_defaults;
	tumor_cell_a.type = 1;
	tumor_cell_a.phenotype.motility.migration_speed = 1.5;
	
	
	stem_cell.phenotype.differentiation.differentiation_possible = true;
	
	double p = MAX_DIFFERENTIATION;
	double q = 1 - p;
	
	std::vector<double> probabilities;
	probabilities.push_back(p*p);
	probabilities.push_back(2*p*q);
	probabilities.push_back(q*q);
	stem_cell.phenotype.differentiation.probabilities = probabilities;
	
	std::vector<Differentiation_Outcome> outcomes;
	Differentiation_Outcome* symmetric_a = new Differentiation_Outcome(&tumor_cell_a, &tumor_cell_a);
	Differentiation_Outcome* asymmetric_a = new Differentiation_Outcome(&stem_cell, &tumor_cell_a);
	Differentiation_Outcome* symmetric_stem = new Differentiation_Outcome(&stem_cell, &stem_cell);
	outcomes.push_back(*symmetric_a);
	outcomes.push_back(*asymmetric_a);
	outcomes.push_back(*symmetric_stem);
	stem_cell.phenotype.differentiation.outcomes = outcomes;
	
	stem_cell.functions.update_phenotype = update_differentiation_O2_based;
	
	
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
/* now this is in XML 
	default_microenvironment_options.X_range = {-1000, 1000}; 
	default_microenvironment_options.Y_range = {-1000, 1000}; 
	default_microenvironment_options.simulate_2D = true; 
*/
	// make sure to override and go back to 2D 
	if( default_microenvironment_options.simulate_2D == false )
	{
		std::cout << "Warning: overriding XML config option and setting to 2D!" << std::endl; 
		default_microenvironment_options.simulate_2D = true; 
	}
	
	// no gradients need for this example 

	default_microenvironment_options.calculate_gradients = false; 
	
	// set Dirichlet conditions 

	default_microenvironment_options.outer_Dirichlet_conditions = true;
	
	// if there are more substrates, resize accordingly 
	std::vector<double> bc_vector( 1 , 38.0 ); // 5% o2
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
	// create some cells near the origin
	
	double cell_radius = cell_defaults.phenotype.geometry.radius; 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	double tumor_radius = 100; 
	
	int z = 0;
	for(int i = -tumor_radius; i < tumor_radius; i+=cell_spacing)
	{
		for(int j = -tumor_radius; j < tumor_radius; j+=15){
			if(pow(i,2) + pow(j,2) <=  pow(tumor_radius,2))
			{
				Cell* bC;
				if(z % 10 == 0){
					bC = create_cell( stem_cell );
				}else{
					bC = create_cell( tumor_cell_a );
				}
				bC->assign_position( j+25, i, 0.0 );
			}
			z++;
		}
	}
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	std::vector<std::string> output = false_cell_coloring_cytometry(pCell); 
	
	if(pCell->type == 0 )
	{
		 output[0] = "white";
		 output[2] = "green"; 
	} else if(pCell->type == 1 ){
		 output[0] = "white";
		 output[2] = "red"; 
	}
	
	static int O2_index = microenvironment.find_density_index( "oxygen" );
	double o2 = pCell->nearest_density_vector()[O2_index];
	
	
	//border goes from blue -> purple -> red as cell gets more hypoxic
	if( o2 < pCell->parameters.o2_hypoxic_threshold )
	{
		int range = pCell->parameters.o2_hypoxic_threshold  - pCell->parameters.o2_hypoxic_saturation;
		double ratio = (o2 - pCell->parameters.o2_hypoxic_saturation)/ range;
		int color = ratio * 255;
		char szTempString [128];		
		sprintf( szTempString , "rgb(%u,%u,%u)", color, color, color); 
		output[0].assign( szTempString );
	}
	
	return output; 
	 
}


void update_differentiation_O2_based(Cell* pCell, Phenotype& phenotype, double dt)
{
	
	static int oxygen_substrate_index = pCell->get_microenvironment()->find_density_index("oxygen");
	double o2 = pCell->nearest_density_vector()[oxygen_substrate_index];
	
	
	if( o2 < pCell->parameters.o2_hypoxic_saturation)
	{ 
		return; 
	}
	
	int range = pCell->parameters.o2_hypoxic_threshold  - pCell->parameters.o2_hypoxic_saturation;
	double ratio = (o2 - pCell->parameters.o2_hypoxic_saturation)/ range;
	
	//prevent outlier oxygen values from creating abnormally high probabilities
	if(ratio > 1)
	{
		ratio = 1;
	}
	
	double p = MAX_DIFFERENTIATION * ratio;
	double q = 1 - p;
	
	std::vector<double> updated_probabilities;
	updated_probabilities.push_back(p*p);
	updated_probabilities.push_back(2*p*q);
	updated_probabilities.push_back(q*q);
	
	pCell->phenotype.differentiation.probabilities = updated_probabilities;
}
