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
# Copyright (c) 2015-2022, Paul Macklin and the PhysiCell Project             #
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

#include "./biorobots.h"

void setup_microenvironment( void )
{
	// set domain parameters
	
	initialize_microenvironment(); 	
	
	// these will ***overwrite*** values specified in the 
	// microenvironment_setup part of the XML,
	// based on what's in the user_parameters section 
	
	microenvironment.name = "synthetic tissue"; 
	
	int cargo_index = microenvironment.find_density_index( "cargo signal" ); 
	int director_index = microenvironment.find_density_index( "director signal" ); 
	
	microenvironment.diffusion_coefficients[cargo_index] = 
		parameters.doubles("cargo_signal_D");  
	microenvironment.decay_rates[cargo_index] = 
		parameters.doubles("cargo_signal_decay");  
	
	microenvironment.diffusion_coefficients[director_index] = 
		parameters.doubles("director_signal_D");  
	microenvironment.decay_rates[director_index] = 
		parameters.doubles("director_signal_decay"); 
	
	// display the microenvironment again 
	
	microenvironment.display_information( std::cout ); 
	
	return; 
}

void create_cell_types( void )
{
	SeedRandom( parameters.ints("random_seed") ); 
	// housekeeping 
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
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

	cell_defaults.phenotype.mechanics.attachment_elastic_constant = parameters.doubles( "elastic_coefficient" );
	
	static int worker_ID = find_cell_definition( "worker cell" )->type; 
	static int cargo_ID = find_cell_definition( "cargo cell" )->type; 
	static int director_ID = find_cell_definition( "director cell" )->type; 

	Cell_Definition* pCD = find_cell_definition( "director cell" ); 
	pCD->functions.update_phenotype = director_cell_rule; 
	pCD->phenotype.mechanics.attachment_elastic_constant = parameters.doubles( "elastic_coefficient" );
	
	pCD = find_cell_definition( "cargo cell" ); 
	pCD->functions.update_phenotype = cargo_cell_rule; 
	pCD->functions.custom_cell_rule = NULL; // extra_elastic_attachment_mechanics;  // pre-1.8.0
	pCD->functions.contact_function = standard_elastic_contact_function; 
	pCD->phenotype.mechanics.attachment_elastic_constant = parameters.doubles( "elastic_coefficient" );

	pCD = find_cell_definition( "worker cell" ); 
	pCD->phenotype.mechanics.cell_cell_adhesion_strength = 0.0; 
	
	pCD->functions.update_phenotype = worker_cell_rule; 
	pCD->functions.custom_cell_rule = NULL; // extra_elastic_attachment_mechanics; // pre 1.8.0
	pCD->functions.contact_function = standard_elastic_contact_function; 
	pCD->functions.update_migration_bias = worker_cell_motility;
	pCD->phenotype.mechanics.attachment_elastic_constant = parameters.doubles( "elastic_coefficient" );
	
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 
	
	return; 
}

void director_cell_rule( Cell* pCell , Phenotype& phenotype , double dt )
{
	return; 
	std::vector<Cell*> nearby = pCell->cells_in_my_container(); 
	
	// if at least 2 neighbors, turn off secretion 
		// if size >= 3, then we have "self" and at least two more 
	if( nearby.size() > 2 )
	{
		pCell->phenotype.secretion.set_all_secretion_to_zero(); 
		pCell->custom_data[ "secreting" ] = 0.0; 
		
		pCell->functions.update_phenotype = NULL; 
	}
	
	return; 
}

std::vector<std::string> robot_coloring_function( Cell* pCell )
{
	std::string color = "black"; 
	std::vector< std::string > output( 4 , color ); 
	
	// black cells if necrotic 
	if( pCell->phenotype.death.dead == true )
	{ return output; }

	output[3] = "none"; // no nuclear outline color 
	
	static std::string worker_color = parameters.strings( "worker_color" ); 
	static std::string cargo_color = parameters.strings( "cargo_color" ); 
	static std::string director_color = parameters.strings( "director_color" ); 
	
	static int worker_ID = find_cell_definition( "worker cell" )->type; 
	static int cargo_ID = find_cell_definition( "cargo cell" )->type; 
	static int director_ID = find_cell_definition( "director cell" )->type; 

	if( pCell->type == worker_ID )
	{ color = worker_color; }
	else if( pCell->type == cargo_ID )
	{ color = cargo_color; }
	else if( pCell->type == director_ID )
	{ color = director_color; }
	else
	{ color = "white"; } 
	
	output[0] = color; 
	output[2] = color; 
	
	return output; 
}

void create_cargo_cluster_6( std::vector<double>& center )
{
	// create a hollow cluster at position, with random orientation 
	
	static Cell_Definition* pCargoDef = find_cell_definition("cargo cell");
	
	static double spacing = 0.95 * pCargoDef->phenotype.geometry.radius * 2.0; 
	static double d_Theta = 1.047197551196598 ; // 2*pi / 6.0 
	
	double theta = 6.283185307179586 * UniformRandom(); 
	
	static std::vector<double> position(3,0.0); 
	
	Cell* pC; 
	for( int i=0; i < 6; i++ )
	{
		pC = create_cell( *pCargoDef ); 
		
		position[0] = center[0] + spacing*cos( theta ); 
		position[1] = center[1] + spacing*sin( theta ); 
		
		pC->assign_position( position ); 
		
		theta += d_Theta; 
	}
	
	return; 
}

void create_cargo_cluster_7( std::vector<double>& center )
{
	static Cell_Definition* pCargoDef = find_cell_definition("cargo cell");	
	
	// create a filled cluster at position, with random orientation 

	create_cargo_cluster_6( center );
	Cell* pC = create_cell( *pCargoDef ); 
	pC->assign_position( center ); 
	
	return; 
}


void create_cargo_cluster_3( std::vector<double>& center )
{
	static Cell_Definition* pCargoDef = find_cell_definition("cargo cell");	
	
	// create a small cluster at position, with random orientation 
	
	static double spacing = 0.95 * pCargoDef->phenotype.geometry.radius * 1.0; 
	static double d_Theta = 2.094395102393195 ; // 2*pi / 3.0 
	
	double theta = 6.283185307179586 * UniformRandom(); 
	
	static std::vector<double> position(3,0.0); 
	
	Cell* pC; 
	for( int i=0; i < 3; i++ )
	{
		pC = create_cell( *pCargoDef ); 
		
		position[0] = center[0] + spacing*cos( theta ); 
		position[1] = center[1] + spacing*sin( theta ); 
		
		pC->assign_position( position ); 
		
		theta += d_Theta; 
	}
	
	return; 
}


void setup_tissue( void )
{
	int number_of_directors = parameters.ints("number_of_directors"); // 15;  
	int number_of_cargo_clusters = parameters.ints("number_of_cargo_clusters"); // 100;  
	int number_of_workers = parameters.ints("number_of_workers"); // 50;  
	
	static Cell_Definition* pCargoDef = find_cell_definition("cargo cell");	
	static Cell_Definition* pDirectorDef = find_cell_definition("director cell");	
	static Cell_Definition* pWorkerDef = find_cell_definition("worker cell");	
	

	std::cout << "Placing cells ... " << std::endl; 
	
	// randomly place seed cells 
	
	std::vector<double> position(3,0.0); 
	
	double x_range = default_microenvironment_options.X_range[1] - default_microenvironment_options.X_range[0]; 
	double y_range = default_microenvironment_options.Y_range[1] - default_microenvironment_options.Y_range[0]; 

	double relative_margin = 0.2;  
	double relative_outer_margin = 0.02; 
	
	std::cout << "\tPlacing " << number_of_directors << " director cells ... " << std::endl; 
	for( int i=0; i < number_of_directors ; i++ )
	{
		// pick a random location 
		position[0] = default_microenvironment_options.X_range[0] + x_range*( relative_margin + (1.0-2*relative_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + y_range*( relative_outer_margin + (1.0-2*relative_outer_margin)*UniformRandom() ); 
		
		// place the cell
		Cell* pC;
		pC = create_cell( *pDirectorDef ); 
		pC->assign_position( position );
		pC->is_movable = false; 
	}
	
	// place cargo clusters on the fringes 
	
	std::cout << "\tPlacing cargo cells ... " << std::endl; 
	for( int i=0; i < number_of_cargo_clusters ; i++ )
	{
		// pick a random location 
		
		position[0] = default_microenvironment_options.X_range[0] + 
				x_range*( relative_outer_margin + (1-2.0*relative_outer_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + 
				y_range*( relative_outer_margin + (1-2.0*relative_outer_margin)*UniformRandom() ); 
		
		if( UniformRandom() < 0.5 )
		{
			Cell* pCell = create_cell( *pCargoDef ); 
			pCell->assign_position( position ); 
		}
		else
		{
			create_cargo_cluster_7( position ); 
		}
	}
	
	// place "workersworkers"

	std::cout << "\tPlacing worker cells ... " << std::endl; 
	for( int i=0; i < number_of_workers ; i++ )
	{
		// pick a random location 
		
		position[0] = default_microenvironment_options.X_range[0] + x_range*( relative_margin + (1.0-2*relative_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + y_range*( relative_outer_margin + (1.0-2*relative_outer_margin)*UniformRandom() ); 
		
		// place the cell
		Cell* pC;

		pC = create_cell( *pWorkerDef ); 
		pC->assign_position( position );
	}	
	

	std::cout << "done!" << std::endl; 
	// make a plot 
	
	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml(); 		
	
	PhysiCell_SVG_options.length_bar = 200; 
	SVG_plot( "initial.svg" , microenvironment, 0.0 , 0.0 , robot_coloring_function );	
	
	return; 
}


void cargo_cell_rule( Cell* pCell , Phenotype& phenotype , double dt )
{
	
	return; 
}

/*
void attach_cells( Cell* pCell_1, Cell* pCell_2 )
{
	#pragma omp critical
	{
		
	bool already_attached = false; 
	for( int i=0 ; i < pCell_1->state.neighbors.size() ; i++ )
	{
		if( pCell_1->state.neighbors[i] == pCell_2 )
		{ already_attached = true; }
	}
	if( already_attached == false )
	{ pCell_1->state.neighbors.push_back( pCell_2 ); }
	
	already_attached = false; 
	for( int i=0 ; i < pCell_2->state.neighbors.size() ; i++ )
	{
		if( pCell_2->state.neighbors[i] == pCell_1 )
		{ already_attached = true; }
	}
	if( already_attached == false )
	{ pCell_2->state.neighbors.push_back( pCell_1 ); }

	}

	return; 
}
*/

/*
void dettach_cells( Cell* pCell_1 , Cell* pCell_2 )
{
	#pragma omp critical
	{
		bool found = false; 
		int i = 0; 
		while( !found && i < pCell_1->state.neighbors.size() )
		{
			// if cell 2 is in cell 1's list, remove it
			if( pCell_1->state.neighbors[i] == pCell_2 )
			{
				int n = pCell_1->state.neighbors.size(); 
				// copy last entry to current position 
				pCell_1->state.neighbors[i] = pCell_1->state.neighbors[n-1]; 
				// shrink by one 
				pCell_1->state.neighbors.pop_back(); 
				found = true; 
			}
			i++; 
		}
	
		found = false; 
		i = 0; 
		while( !found && i < pCell_2->state.neighbors.size() )
		{
			// if cell 1 is in cell 2's list, remove it
			if( pCell_2->state.neighbors[i] == pCell_1 )
			{
				int n = pCell_2->state.neighbors.size(); 
				// copy last entry to current position 
				pCell_2->state.neighbors[i] = pCell_2->state.neighbors[n-1]; 
				// shrink by one 
				pCell_2->state.neighbors.pop_back(); 
				found = true; 
			}
			i++; 
		}

	}
	
	return; 
}
*/

/*
void add_elastic_velocity( Cell* pActingOn, Cell* pAttachedTo , double elastic_constant )
{
	std::vector<double> displacement = pAttachedTo->position - pActingOn->position; 
	axpy( &(pActingOn->velocity) , elastic_constant , displacement ); 
	
	return; 
}
*/

/*
void extra_elastic_attachment_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if I am 
	std::vector<double> velocity(3,0.0); 
	
	for( int i=0; i < pCell->state.neighbors.size() ; i++ )
	{
		add_elastic_velocity( pCell, pCell->state.neighbors[i], pCell->custom_data["elastic coefficient"] ); 
	}

	return; 
}	
*/

void worker_cell_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	static double threshold = parameters.doubles("drop_threshold"); // 0.4; 
	
	static int cargo_index = microenvironment.find_density_index( "cargo signal" ); // 1 
	static int director_index = microenvironment.find_density_index( "director signal" ); // 0 
	
	// have I arrived? If so, release my cargo 
	if( pCell->nearest_density_vector()[director_index] > threshold )
	{
		// set receptor = 0 for cells we're detaching from 
		// and set their cycle rate to zero 
		for( int k=0; k < pCell->state.attached_cells.size() ; k++ )
		{
			Cell* pTemp = pCell->state.attached_cells[k]; 
			
			pTemp->custom_data[ "receptor" ] = 0.0; 
			pTemp->phenotype.cycle.data.transition_rate( 0,0 ) = 0; 
		}
		
		pCell->remove_all_attached_cells(); 
		/* // prior to 1.8.0 
		for( int i=0; i < pCell->state.neighbors.size(); i++ )
		{
			Cell* pTemp = pCell->state.neighbors[i]; 
			dettach_cells( pCell, pTemp ); 
			
			pTemp->custom_data[ "receptor" ] = 0.0; 
			pTemp->phenotype.cycle.data.transition_rate( 0,0 ) = 0; 
		}
		*/
	}
	
	// am I searching for cargo? if so, see if I've found it
	// if( pCell->state.neighbors.size() == 0 ) // pre 1.8.0 
	if( pCell->state.number_of_attached_cells() == 0 )
	{
		std::vector<Cell*> nearby = pCell->cells_in_my_container(); 
		for( int i=0; i < nearby.size(); i++ )
		{
			// if it is expressing the receptor, dock with it 
			if( nearby[i]->custom_data["receptor"] > 0.5 )
			{
				attach_cells( pCell, nearby[i] );
				nearby[i]->custom_data["receptor"] = 0.0; 
				nearby[i]->phenotype.secretion.set_all_secretion_to_zero(); 
			}
		}
		
	}
	
	return; 
}

void worker_cell_motility( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if attached, biased motility towards director chemoattractant 
	// otherwise, biased motility towards cargo chemoattractant 
	
	static double attached_worker_migration_bias = 
		parameters.doubles("attached_worker_migration_bias"); 
	static double unattached_worker_migration_bias = 
		parameters.doubles("unattached_worker_migration_bias"); 
		
	static int cargo_index = microenvironment.find_density_index( "cargo signal" ); // 1 
	static int director_index = microenvironment.find_density_index( "director signal" ); // 0 
	
	// if( pCell->state.neighbors.size() > 0 ) // pre-1.8.0
	if( pCell->state.number_of_attached_cells() > 0 )
	{
		phenotype.motility.migration_bias = attached_worker_migration_bias; 

		phenotype.motility.migration_bias_direction = pCell->nearest_gradient(director_index);	
		normalize( &( phenotype.motility.migration_bias_direction ) );			
	}
	else
	{
		phenotype.motility.migration_bias = unattached_worker_migration_bias; 
		
		phenotype.motility.migration_bias_direction = pCell->nearest_gradient(cargo_index);	
		normalize( &( phenotype.motility.migration_bias_direction ) );			
	}
	
	return; 
}

