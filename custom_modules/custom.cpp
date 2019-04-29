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

Cell_Definition macrophage; 

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
	
	// Name the default cell type 
	
	cell_defaults.type = 0; 
	cell_defaults.name = "epithelial cell"; 
	
	// set default cell cycle model 

	cell_defaults.functions.cycle_model = flow_cytometry_separated_cycle_model; 
	
	// set default_cell_functions; 
	
	cell_defaults.functions.update_phenotype = epithelial_function;
	
	// needed for a 2-D simulation: 
	
	/* grab code from heterogeneity */ 
	
	cell_defaults.functions.set_orientation = up_orientation; 
	cell_defaults.phenotype.geometry.polarity = 1.0;
	cell_defaults.phenotype.motility.restrict_to_2D = true; 
	
	// make sure the defaults are self-consistent. 
	
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions ); 

	// set the rate terms in the default phenotype 

	// first find index for a few key variables. 
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Necrosis" );
	int virus_index = microenvironment.find_density_index( "virus" ); 

	int G0G1_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::G0G1_phase );
	int S_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::S_phase );

	// initially no death 
	cell_defaults.phenotype.death.rates[apoptosis_model_index] = 0.0; 
	cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 
	
	// initially no birth 
	cell_defaults.phenotype.cycle.data.transition_rate(G0G1_index, S_index ) = 0.0 ; 
	
	// not motile 
	cell_defaults.phenotype.motility.is_motile = false; 

	// set virus uptake / secretion parameters for the default (epithelial) cell type 
	cell_defaults.phenotype.secretion.uptake_rates[virus_index] = 
		parameters.doubles("viral_internalization_rate"); 
	cell_defaults.phenotype.secretion.secretion_rates[virus_index] = 0; 
	cell_defaults.phenotype.secretion.saturation_densities[virus_index] = 0; 
	
	// add custom data here, if any 
	
	// release virus at death or when eaten 
	cell_defaults.phenotype.molecular.fraction_released_at_death[ virus_index ]= 1.0; 
	cell_defaults.phenotype.molecular.fraction_transferred_when_ingested[ virus_index ]= 1.0; 

	// Now, let's define another cell type. 
	// It's best to just copy the default and modify it. 
	
	macrophage = cell_defaults; 
	macrophage.functions.update_phenotype = macrophage_function; 
	
	// make this cell type randomly motile, less adhesive, greater survival, 
	// and less proliferative 
	
	macrophage.type = 1; 
	macrophage.name = "macrophage"; 
	
	// make sure the new cell type has its own reference phenotype
	
	macrophage.parameters.pReference_live_phenotype = &( macrophage.phenotype ); 
	
	// macrophages do not release virus at death 
	macrophage.phenotype.molecular.fraction_released_at_death[ virus_index ]= 0.0; 
		
	// macrophages don't get eaten 
	macrophage.phenotype.molecular.fraction_transferred_when_ingested[ virus_index ]= 0.0; 
	
	// enable random motility 
	macrophage.phenotype.motility.is_motile = true; 
	macrophage.phenotype.motility.persistence_time = parameters.doubles( "macrophage_persistence_time" );
	macrophage.phenotype.motility.migration_speed = parameters.doubles( "macrophage_migration_speed" ); 
	macrophage.phenotype.motility.migration_bias = 0.0;// completely random 
	
	// Set cell-cell adhesion relative to other cells 
	macrophage.phenotype.mechanics.cell_cell_adhesion_strength *= parameters.doubles( "macrophage_relative_adhesion" ); 
	
	// macrophages do not uptake viral particles 
	macrophage.phenotype.secretion.uptake_rates[virus_index] = 
		parameters.doubles("viral_internalization_rate"); 
	
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
	
	// do not use oxygen as first substrate 
	default_microenvironment_options.use_oxygen_as_first_field = false; 
	
	// set properties 
	microenvironment.set_density( 0 , "virus", "particles/micron^3" ); 
	microenvironment.diffusion_coefficients[0] = 1e4; 
	microenvironment.decay_rates[0] = 0; 
	
	// set Dirichlet conditions 

	default_microenvironment_options.outer_Dirichlet_conditions = false;
	
	// if there are more substrates, resize accordingly 
	std::vector<double> bc_vector( 1 , 0.0 ); // 5% o2
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	
	// 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
	// create some cells near the origin
	
	Cell* pC;

	pC = create_cell(); 
	pC->assign_position( 0.0, 0.0, 0.0 );

	pC = create_cell(); 
	pC->assign_position( -100, 0, 0.0 );
	
	pC = create_cell(); 
	pC->assign_position( 0, 100, 0.0 );
	
	// now create a motile cell 
	
	pC = create_cell( macrophage ); 
	pC->assign_position( 15.0, -18.0, 0.0 );
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = false_cell_coloring_cytometry(pCell); 
		
	if( pCell->phenotype.death.dead == false && pCell->type == 1 )
	{
		 output[0] = "black"; 
		 output[2] = "black"; 
	}
	
	return output; 
}

std::vector<std::string> viral_coloring_function( Cell* pCell )
{
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = { "white" , "black" , "grey", "black" }; 
		
	// dead cells 
	if( pCell->phenotype.death.dead == false )
	{
		 output[0] = "black"; 
		 output[2] = "black"; 
	}
	
	if( pCell->type != macrophage.type )
	{
		output[0] = "red"; 
		output[2] = "darkred"; 
	}
	
	return output; 
}

void macrophage_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// bookeeping 
	
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	
	// digest virus particles inside me 
	
	static double implicit_Euler_constant = 
		(1.0 + dt * parameters.doubles("virus_digestion_rate") );
	phenotype.molecular.internalized_total_substrates[nVirus] /= implicit_Euler_constant; 
	
	// check for contact with a cell
	
	Cell* pTestCell = NULL; 
	
	for( int n=0; n < pCell->cells_in_my_container().size() ; n++ )
	{
		pTestCell = pCell->cells_in_my_container()[n]; 
		// if it is not me and not a macrophage 
		if( pTestCell != pCell && pTestCell->type != macrophage.type )
		{
			// if it is not a macrophage, test for viral load 
			// if high viral load, eat it. 
		
			if( pTestCell->phenotype.molecular.internalized_total_substrates[nVirus] 
				> parameters.doubles("min_virion_detection_threshold") )
			{
				std::cout << "nom nom nom" << std::endl; 
				pCell->ingest_cell( pTestCell ); 
			}
		}
	}
	
	return; 
}

void epithelial_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; } 