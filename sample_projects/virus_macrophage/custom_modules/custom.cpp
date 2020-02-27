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
	cell_defaults.phenotype.molecular.sync_to_microenvironment( &microenvironment );
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
	macrophage.functions.update_migration_bias = macrophage_chemotaxis; 
	macrophage.phenotype.sync_to_functions( macrophage.functions ); 

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
	
	// no birth 
	macrophage.phenotype.cycle.data.transition_rate(G0G1_index, S_index ) = 0.0 ; 
	
	// macrophages do not uptake viral particles 
	macrophage.phenotype.secretion.uptake_rates[virus_index] = 
		parameters.doubles("viral_internalization_rate"); 
		
	build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 

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
	
/*
	// in XML since version 1.6.0
	
	// no gradients need for this example 

	default_microenvironment_options.calculate_gradients = false; 
	
	// do not use oxygen as first substrate 
	default_microenvironment_options.use_oxygen_as_first_field = false; 
	
	// set properties 
	microenvironment.set_density( 0 , "virus", "particles/micron^3" ); 
	microenvironment.diffusion_coefficients[0] = parameters.doubles("viral_diffusion_coefficient"); 
	microenvironment.decay_rates[0] = 0; 
	
	// set Dirichlet conditions 

	default_microenvironment_options.outer_Dirichlet_conditions = false;
	
	// track internalized total substrates
	
	
	// if there are more substrates, resize accordingly 
	std::vector<double> bc_vector( 1 , 0.0 ); // 5% o2
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	
	default_microenvironment_options.initial_condition_vector = { 0.0 }; 
	// 
*/

	// override BioFVM setup with user parameters 

	int virus_ID = microenvironment.find_density_index( "virus" ); 
	microenvironment.diffusion_coefficients[virus_ID] = parameters.doubles("viral_diffusion_coefficient"); 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
	int nVirus = microenvironment.find_density_index( "virus" ); 
	// create some cells near the origin
	
	double length_x = microenvironment.mesh.bounding_box[3] - 
		microenvironment.mesh.bounding_box[0]; 
		
	double length_y = microenvironment.mesh.bounding_box[4] - 
		microenvironment.mesh.bounding_box[1]; 
		
	Cell* pC;
	
	int number_of_infected_cells = parameters.ints( "number_of_infected_cells" ); 

	for( int n = 0 ; n < number_of_infected_cells; n++ )
	{
		pC = create_cell(); 
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC->assign_position( x,y, 0.0 );
		pC->phenotype.molecular.internalized_total_substrates[ nVirus ] = 1; 
	}

	for( int n = number_of_infected_cells ; n < 500 ; n++ )
	{
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC = create_cell(); 
		pC->assign_position( x,y, 0.0 );
//		pC->phenotype.molecular.internalized_total_substrates[ nVirus ] = 0; 
	}
	// now create a motile cell 
	
	for( int n= 0 ; n < parameters.ints( "number_of_macrophages" ); n++ )
	{
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC = create_cell( macrophage ); 
		pC->assign_position( x,y, 0.0 );
	}
	
	
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
	
	std::vector<std::string> output = { "magenta" , "black" , "magenta", "black" }; 
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	
	static double min_virus = parameters.doubles( "min_virion_count" );
	static double max_virus = parameters.doubles( "burst_virion_count" ); 
	static double denominator = max_virus - min_virus + 1e-15; 
				
	// dead cells 
	if( pCell->phenotype.death.dead == true )
	{
		 output[0] = "red"; 
		 output[2] = "darkred"; 
		 return output; 
	}
	
	if( pCell->type != macrophage.type )
	{
		output[0] = "blue"; 
		output[2] = "darkblue"; 
		
		double virus = pCell->phenotype.molecular.internalized_total_substrates[nVirus]; 
		
		if( pCell->phenotype.molecular.internalized_total_substrates[nVirus] >= min_virus )
		{
			double interp = (virus - min_virus )/ denominator;  
			if( interp > 1.0 )
			{ interp = 1.0; } 
		
			int Red   = (int) floor( 255.0*interp ); 
			int Green = (int) floor( 255.0*interp ); 
			int Blue  = (int) floor( 255.0 *(1-interp) ); 
			
			char szTempString [128];
			sprintf( szTempString , "rgb(%u,%u,%u)", Red, Green, Blue );
			output[0].assign( szTempString );
			output[2].assign( szTempString );
		}
		
	}
	
	return output; 
}

std::vector<Cell*> get_possible_neighbors( Cell* pCell )
{
	std::vector<Cell*> neighbors = {}; 

	// First check the neighbors in my current voxel
	std::vector<Cell*>::iterator neighbor;
	std::vector<Cell*>::iterator end =
		pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].end();
	for( neighbor = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].begin(); neighbor != end; ++neighbor)
	{ neighbors.push_back( *neighbor ); }

	std::vector<int>::iterator neighbor_voxel_index;
	std::vector<int>::iterator neighbor_voxel_index_end = 
		pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].end();

	for( neighbor_voxel_index = 
		pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].begin();
		neighbor_voxel_index != neighbor_voxel_index_end; 
		++neighbor_voxel_index )
	{
		if(!is_neighbor_voxel(pCell, pCell->get_container()->underlying_mesh.voxels[pCell->get_current_mechanics_voxel_index()].center, pCell->get_container()->underlying_mesh.voxels[*neighbor_voxel_index].center, *neighbor_voxel_index))
			continue;
		end = pCell->get_container()->agent_grid[*neighbor_voxel_index].end();
		for(neighbor = pCell->get_container()->agent_grid[*neighbor_voxel_index].begin();neighbor != end; ++neighbor)
		{ neighbors.push_back( *neighbor ); }
	}
	
	return neighbors; 
}

void macrophage_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// bookkeeping 
	
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	
	// digest virus particles inside me 
	
	static double implicit_Euler_constant = 
		(1.0 + dt * parameters.doubles("virus_digestion_rate") );
	phenotype.molecular.internalized_total_substrates[nVirus] /= implicit_Euler_constant; 
	
	// check for contact with a cell
	
	Cell* pTestCell = NULL; 
	std::vector<Cell*> neighbors = get_possible_neighbors(pCell);
	
//	for( int n=0; n < pCell->cells_in_my_container().size() ; n++ )
	for( int n=0; n < neighbors.size() ; n++ )
	{
		pTestCell = neighbors[n]; 
		// if it is not me and not a macrophage 
		if( pTestCell != pCell && pTestCell->type != macrophage.type )
		{
			// calculate distance to the cell 
			std::vector<double> displacement = pTestCell->position;
			displacement -= pCell->position;
			double distance = norm( displacement ); 
			
			double max_distance = pCell->phenotype.geometry.radius + 
				pTestCell->phenotype.geometry.radius; 
			max_distance *= 1.1; 
			
			// if it is not a macrophage, test for viral load 
			// if high viral load, eat it. 
		
			if( pTestCell->phenotype.molecular.internalized_total_substrates[nVirus] 
				> parameters.doubles("min_virion_detection_threshold") &&
				distance < max_distance )
			{
				std::cout << "\t\tnom nom nom" << std::endl; 
				pCell->ingest_cell( pTestCell ); 
			}
		}
	}
	
	return; 
}

void epithelial_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// bookkeeping
	
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	
	// compare against viral load. Should I commit apoptosis? 
	
	double virus = phenotype.molecular.internalized_total_substrates[nVirus]; 
	if( virus >= parameters.doubles("burst_virion_count") )
	{
		std::cout << "\t\tburst!" << std::endl; 
		pCell->lyse_cell(); // start_death( apoptosis_model_index );
		pCell->functions.update_phenotype = NULL; 
		return; 
	}

	// replicate virus particles inside me 
	
	if( virus >= parameters.doubles("min_virion_count") ) 
	{
		double new_virus = parameters.doubles( "viral_replication_rate" ); 
		new_virus *= dt;
		phenotype.molecular.internalized_total_substrates[nVirus] += new_virus; 
	}
//	static double implicit_Euler_constant = 
//		(1.0 + dt * parameters.doubles("virus_digestion_rate") );
//	phenotype.molecular.internalized_total_substrates[nVirus] /= implicit_Euler_constant; 
	
	
	// if I have too many 
	// if I have too many 

	return; 
} 

void macrophage_chemotaxis( Cell* pCell, Phenotype& phenotype, double dt )
{
	static double bias = parameters.doubles("macrophage_migration_bias");
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	
	phenotype.motility.migration_bias = bias; 
	
	phenotype.motility.migration_bias_direction = pCell->nearest_gradient( nVirus ); 
	double denominator =  norm( phenotype.motility.migration_bias_direction ) + 1e-17; 
	
	phenotype.motility.migration_bias_direction /= denominator; 
	
	return; 
}

std::vector<double> integrate_total_substrates( void )
{
	// start with 0 vector 
	std::vector<double> out( microenvironment.number_of_densities() , 0.0 ); 

	// integrate extracellular substrates 
	for( unsigned int n = 0; n < microenvironment.number_of_voxels() ; n++ )
	{
		// out = out + microenvironment(n) * dV(n) 
		axpy( &out , microenvironment.mesh.voxels[n].volume , microenvironment(n) ); 
	}

	// inte
	for( unsigned int n=0; n < (*all_cells).size(); n++ )
	{
		Cell* pC = (*all_cells)[n];
		out += pC->phenotype.molecular.internalized_total_substrates;
	}
	
	return out; 
}
