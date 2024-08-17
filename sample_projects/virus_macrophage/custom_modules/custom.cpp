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

void create_cell_types( void )
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
	
	initialize_default_cell_definition(); 
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 

	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 

	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 
	
	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	// first find index for a few key variables. 
	int virus_index = microenvironment.find_density_index( "virus" ); 
	int nInterferon = microenvironment.find_density_index( "interferon" ); 
	
	Cell_Definition* pEpithelial = find_cell_definition( "epithelial cell" ); 
	Cell_Definition* pMacrophage = find_cell_definition( "macrophage" ); 
		
	pEpithelial->functions.update_phenotype = epithelial_function;
	
	pEpithelial->phenotype.molecular.fraction_released_at_death[ virus_index ] = 
		parameters.doubles("fraction_released_at_death"); 
	pEpithelial->phenotype.molecular.fraction_transferred_when_ingested[ virus_index ] = 
		parameters.doubles("fraction_transferred_when_ingested"); 
/*		
	pEpithelial->phenotype.molecular.fraction_released_at_death[ nInterferon ] = 0;
	pEpithelial->phenotype.molecular.fraction_transferred_when_ingested[ nInterferon ] = 0; 		
*/		
	pMacrophage->phenotype.mechanics.cell_cell_adhesion_strength *= parameters.doubles( "macrophage_relative_adhesion" ); 
	pMacrophage->phenotype.molecular.fraction_released_at_death[ virus_index ]= 0.0; 
	pMacrophage->phenotype.molecular.fraction_transferred_when_ingested[ virus_index ]= 0.0; 
		
	pMacrophage->functions.update_phenotype = macrophage_function; 
	pMacrophage->functions.custom_cell_rule = avoid_boundaries;
	
	// pMacrophage->phenotype.sync_to_functions( macrophage.functions ); 
	
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



	display_cell_definitions( std::cout ); 
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
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
	
	Cell_Definition* pCD = find_cell_definition( "epithelial cell" ); 

	for( int n = 0 ; n < number_of_infected_cells; n++ )
	{
		pC = create_cell(*pCD); 
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC->assign_position( x,y, 0.0 );
		pC->phenotype.molecular.internalized_total_substrates[ nVirus ] = 1; 
	}

	int number_of_uninfected_cells = parameters.ints( "number_of_uninfected_cells" ); 

	for( int n = 0 ; n < number_of_uninfected_cells ; n++ )
	{
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC = create_cell(*pCD); 
		pC->assign_position( x,y, 0.0 );
	}
	
	pCD = find_cell_definition( "macrophage" );
	
	for( int n= 0 ; n < parameters.ints( "number_of_macrophages" ); n++ )
	{
		double x = microenvironment.mesh.bounding_box[0] + UniformRandom() * length_x; 
		double y = microenvironment.mesh.bounding_box[1] + UniformRandom() * length_y; 
		pC = create_cell( *pCD ); 
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
	
	static double min_virus = pCell->custom_data[ "min_virion_count" ];
	static double max_virus = pCell->custom_data[ "burst_virion_count" ]; 
	static double denominator = max_virus - min_virus + 1e-15; 
	
	static Cell_Definition* pMacrophage = find_cell_definition( "macrophage" ); 
				
	// dead cells 
	if( pCell->phenotype.death.dead == true )
	{
		 output[0] = "red"; 
		 output[2] = "darkred"; 
		 return output; 
	}
	
	if( pCell->type != pMacrophage->type )
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

std::vector<std::string> viral_coloring_function_bar( Cell* pCell )
{
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = { "magenta" , "black" , "magenta", "black" }; 
	static int nVirus = microenvironment.find_density_index( "virus" ); 
	
	static double min_virus = pCell->custom_data[ "min_virion_count" ];
	static double max_virus = pCell->custom_data[ "burst_virion_count" ]; 
	static double denominator = max_virus - min_virus + 1e-15; 
	
	static Cell_Definition* pMacrophage = find_cell_definition( "macrophage" ); 
				
	// dead cells 
	if( pCell->phenotype.death.dead == true )
	{
		 output[0] = "red"; 
		 output[2] = "darkred"; 
		 return output; 
	}
	
	if( pCell->type != pMacrophage->type )
	{
		output[0] = "blue"; 
		output[2] = "darkblue"; 
		
		double virus = pCell->phenotype.molecular.internalized_total_substrates[nVirus]; 
		
		if( pCell->phenotype.molecular.internalized_total_substrates[nVirus] >= min_virus )
		{
			double interp = (virus - min_virus )/ denominator;  
			if( interp > 1.0 )
			{ interp = 1.0; } 
			
			if( interp > 0.75 )
			{ interp = 1; } 
			if( interp > 0.5 && interp <= 0.75 )
			{ interp = 0.75; } 
			if( interp > 0.25 && interp <= 0.5 )
			{ interp = 0.5; } 
			if( interp > 0.01 && interp <= 0.25 )
			{ interp = 0.25; } 
		
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
	
	static Cell_Definition* pMacrophage = find_cell_definition( "macrophage" ); 
	
	// digest virus particles inside me 
	
	static double implicit_Euler_constant = 
		(1.0 + dt * pCell->custom_data["virus_digestion_rate"] );
	phenotype.molecular.internalized_total_substrates[nVirus] /= implicit_Euler_constant; 
	
	// check for contact with a cell
	
	Cell* pTestCell = NULL; 
	std::vector<Cell*> neighbors = get_possible_neighbors(pCell);
	
//	for( int n=0; n < pCell->cells_in_my_container().size() ; n++ )
	for( int n=0; n < neighbors.size() ; n++ )
	{
		pTestCell = neighbors[n]; 
		// if it is not me and not a macrophage 
		if( pTestCell != pCell && pTestCell->type != pMacrophage->type )
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
				> pCell->custom_data["min_virion_detection_threshold"] &&
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
	static int nInterferon = microenvironment.find_density_index( "interferon" ); 
	int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	
	// compare against viral load. Should I commit apoptosis? 
	
	double virus = phenotype.molecular.internalized_total_substrates[nVirus]; 
	if( virus >= pCell->custom_data["burst_virion_count"] )
	{
		std::cout << "\t\tburst!" << std::endl; 
		pCell->lyse_cell(); // start_death( apoptosis_model_index );
		pCell->functions.update_phenotype = NULL; 
		return; 
	}

	// replicate virus particles inside me 
	
	if( virus >= pCell->custom_data["min_virion_count"] ) 
	{
		double new_virus = pCell->custom_data["viral_replication_rate"]; 
		new_virus *= dt;
		phenotype.molecular.internalized_total_substrates[nVirus] += new_virus; 
	}
	
	if( virus >= pCell->custom_data["virion_threshold_for_interferon"] )
	{
		phenotype.secretion.secretion_rates[nInterferon] = pCell->custom_data["max_interferon_secretion_rate"];
	}
	
//	static double implicit_Euler_constant = 
//		(1.0 + dt * pCell->custom_data["virus_digestion_rate"] );
//	phenotype.molecular.internalized_total_substrates[nVirus] /= implicit_Euler_constant; 
	
	
	// if I have too many 
	// if I have too many 

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

void avoid_boundaries( Cell* pCell )
{
	// add velocity to steer clear of the boundaries 
	static double Xmin = microenvironment.mesh.bounding_box[0]; 
	static double Ymin = microenvironment.mesh.bounding_box[1]; 
	static double Zmin = microenvironment.mesh.bounding_box[2]; 

	static double Xmax = microenvironment.mesh.bounding_box[3]; 
	static double Ymax = microenvironment.mesh.bounding_box[4]; 
	static double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	static double avoid_zone = 25; 
	static double avoid_speed = -0.5; // must be negative 
	
	// near edge: 
	bool near_edge = false; 
	if( pCell->position[0] < Xmin + avoid_zone || pCell->position[0] > Xmax - avoid_zone )
	{ near_edge = true; } 
	
	if( pCell->position[1] < Ymin + avoid_zone || pCell->position[1] > Ymax - avoid_zone )
	{ near_edge = true; } 
	
	if( default_microenvironment_options.simulate_2D == false )
	{
		if( pCell->position[2] < Zmin + avoid_zone || pCell->position[2] > Zmax - avoid_zone )
		{ near_edge = true; } 
	}
	
	if( near_edge )
	{
		pCell->velocity = pCell->position; // move towards origin 
		pCell->velocity *= avoid_speed; // move towards origin 
	}
	
	return; 
}

void avoid_boundaries( Cell* pCell , Phenotype& phenotype, double dt )
{ return avoid_boundaries( pCell ); } 