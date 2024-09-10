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
# Copyright (c) 2015-2023, Paul Macklin and the PhysiCell Project             #
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
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 

	Cell_Definition* pCD = find_cell_definition( "director cell"); 
	pCD->functions.update_phenotype = director_cell_rule; 

	pCD = find_cell_definition( "cargo cell");
	pCD->functions.update_phenotype = cargo_cell_rule; 
	pCD->functions.contact_function = standard_elastic_contact_function; 

	pCD = find_cell_definition( "worker cell");
	pCD->functions.update_phenotype = worker_cell_rule; 
	pCD->functions.contact_function = standard_elastic_contact_function; 

	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
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
	double Xmin = microenvironment.mesh.bounding_box[0]; 
	double Ymin = microenvironment.mesh.bounding_box[1]; 
	double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin; 
	
	// create some of each type of cell 
	
	Cell* pC;
	
	for( int k=0; k < cell_definitions_by_index.size() ; k++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[k]; 
		std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
		for( int n = 0 ; n < parameters.ints("number_of_cells") ; n++ )
		{
			std::vector<double> position = {0,0,0}; 
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			pC = create_cell( *pCD ); 
			pC->assign_position( position );
		}
	}
	std::cout << std::endl; 
	
	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml(); 	

	/* custom loading here */

	int number_of_directors = parameters.ints("number_of_directors"); // 15;  
	int number_of_cargo_clusters = parameters.ints("number_of_cargo_clusters"); // 100;  
	int number_of_workers = parameters.ints("number_of_workers"); // 50;  
	
	static Cell_Definition* pCargoDef = find_cell_definition("cargo cell");	
	static Cell_Definition* pDirectorDef = find_cell_definition("director cell");	
	static Cell_Definition* pWorkerDef = find_cell_definition("worker cell");	
	

	std::cout << "Placing cells ... " << std::endl; 
	
	// randomly place seed cells 
	
	std::vector<double> position(3,0.0); 

	double relative_margin = 0.2;  
	double relative_outer_margin = 0.02; 
	
	std::cout << "\tPlacing " << number_of_directors << " director cells ... " << std::endl; 
	for( int i=0; i < number_of_directors ; i++ )
	{
		// pick a random location 
		position[0] = default_microenvironment_options.X_range[0] + Xrange*( relative_margin + (1.0-2*relative_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + Yrange*( relative_outer_margin + (1.0-2*relative_outer_margin)*UniformRandom() ); 
		
		// place the cell
		Cell* pC;
		pC = create_cell( *pDirectorDef ); 
		pC->assign_position( position );
		set_single_behavior( pC, "movable" , false); 
	}
	
	// place cargo clusters on the fringes 
	
	std::cout << "\tPlacing cargo cells ... " << std::endl; 
	for( int i=0; i < number_of_cargo_clusters ; i++ )
	{
		// pick a random location 
		
		position[0] = default_microenvironment_options.X_range[0] + 
				Xrange*( relative_outer_margin + (1-2.0*relative_outer_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + 
				Yrange*( relative_outer_margin + (1-2.0*relative_outer_margin)*UniformRandom() ); 
		
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
		
		position[0] = default_microenvironment_options.X_range[0] + Xrange*( relative_margin + (1.0-2*relative_margin)*UniformRandom() ); 
		
		position[1] = default_microenvironment_options.Y_range[0] + Yrange*( relative_outer_margin + (1.0-2*relative_outer_margin)*UniformRandom() ); 
		
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

std::vector<std::string> my_coloring_function( Cell* pCell )
{ return paint_by_number_cell_coloring(pCell); }

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ return; } 

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 

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

void director_cell_rule( Cell* pCell , Phenotype& phenotype , double dt )
{ return; }

void cargo_cell_rule( Cell* pCell , Phenotype& phenotype , double dt )
{
	static double elastic_coefficient = parameters.doubles( "elastic_coefficient" ); 

	set_single_behavior( pCell ,"cell-cell adhesion elastic constant" , elastic_coefficient ); 
	return; 
}

void worker_cell_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	static double threshold = parameters.doubles("drop_threshold"); // 0.4; 

	static double attached_worker_migration_bias = 
		parameters.doubles("attached_worker_migration_bias"); 
	static double unattached_worker_migration_bias = 
		parameters.doubles("unattached_worker_migration_bias"); 

	static double elastic_coefficient = parameters.doubles( "elastic_coefficient" ); 

	double director_signal = get_single_signal( pCell, "director signal" ); 
	double cargo_signal = get_single_signal( pCell, "cargo signal"); 

	set_single_behavior( pCell ,"cell-cell adhesion elastic constant" , elastic_coefficient ); 
	
	// have I arrived? If so, release my cargo 
	// set chemotaxis weights to seek cargo 
	// set migration bias 
	if( director_signal > threshold )
	{
		// set receptor = 0 for cells we're detaching from 
		// and set their cycle rate to zero 
		for( int k=0; k < pCell->state.attached_cells.size() ; k++ )
		{
			Cell* pTemp = pCell->state.attached_cells[k]; 

			set_single_behavior( pTemp, "custom:receptor" , 0.0 ); 
			set_single_behavior( pTemp, "cycle entry" , 0.0 ); 
		}
		
		pCell->remove_all_attached_cells(); 

		set_single_behavior( pCell , "chemotactic response to director signal" , 0.0 ); 
		set_single_behavior( pCell , "chemotactic response to cargo signal" , 1.0 ); 
		set_single_behavior( pCell , "migration bias" , unattached_worker_migration_bias ); 
	}
	
	// am I searching for cargo? if so, see if I've found it
	// if( pCell->state.neighbors.size() == 0 ) // pre 1.8.0 
	if( pCell->state.number_of_attached_cells() == 0 )
	{
		std::vector<Cell*> nearby = pCell->cells_in_my_container(); 
		for( int i=0; i < nearby.size(); i++ )
		{
			// if it is expressing the receptor, dock with it
			// set chemotaxis weights
			// set migration bias 

			double receptor = get_single_signal( nearby[i] , "custom:receptor" );
			if( receptor > 0.5 )
			{
				attach_cells( pCell, nearby[i] );
				set_single_behavior( nearby[i] , "custom:receptor" , 0.0 ); 

				set_single_behavior( nearby[i] , "director signal secretion" , 0.0 ); 
				set_single_behavior( nearby[i] , "cargo signal secretion" , 0.0 ); 

				set_single_behavior( pCell , "chemotactic response to director signal" , 1.0 ); 
				set_single_behavior( pCell , "chemotactic response to cargo signal" , 0.0 ); 
				set_single_behavior( pCell , "migration bias" , attached_worker_migration_bias ); 

			}
		}
		
	}
	
	return; 
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

