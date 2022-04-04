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

#include "./custom.h"

void create_cell_types( void )
{
	// set the random seed 
	SeedRandom( parameters.ints("random_seed") );  
	
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
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 

	Cell_Definition* pBacteria = find_cell_definition( "bacteria");
	pBacteria->functions.update_phenotype = bacteria_phenotype; 
	pBacteria->functions.update_migration_bias = advanced_chemotaxis_function; 
	
	pBacteria->phenotype.motility.chemotactic_sensitivity( "resource" ) = 1; 
	pBacteria->phenotype.motility.chemotactic_sensitivity( "quorum" ) = 0.1; 

	Cell_Definition* pCD = find_cell_definition( "blood vessel");
	pCD->is_movable = false; 
	
	pCD = find_cell_definition( "macrophage");
	pCD->phenotype.cell_interactions.dead_phagocytosis_rate = 0.1; 
	pCD->functions.update_phenotype = macrophage_phenotype; 
	
	pCD->functions.update_migration_bias = advanced_chemotaxis_function; 
	pCD->phenotype.motility.chemotactic_sensitivity( "pro-inflammatory" ) = 1; 
	pCD->phenotype.motility.chemotactic_sensitivity( "debris" ) = 0.1; 

	// pCD->phenotype.cell_interactions.live_phagocytosis_rate( "bacteria" ) = 0.001; 

	pCD = find_cell_definition( "neutrophil"); 
	pCD->functions.update_phenotype = neutrophil_phenotype; 	


	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 
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
		int num_cells = parameters.ints("number_of_cells"); 
		if( num_cells > 0 )
		{ std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; }
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

	// parameter-based placement 
	// bacteria 
	Cell_Definition* pCD = find_cell_definition("bacteria"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_bacteria") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// blood vessels 
	pCD = find_cell_definition("blood vessel"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_blood_vessels") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// macrophages 
	pCD = find_cell_definition("macrophage"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_macrophages") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// neutrophils  
	pCD = find_cell_definition("neutrophil"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_neutrophils") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}
	
	// number_of_dendritic_cells  
	pCD = find_cell_definition("dendritic cell"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_dendritic_cells") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}
	
	// number_of_CD8_T_cells  
	pCD = find_cell_definition("CD8+ T cell"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_CD8_T_cells") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// number_of_Tregs  
	pCD = find_cell_definition("Treg"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_Tregs") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml(); 	
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	 return paint_by_number_cell_coloring(pCell); 
	 
}

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ return; } 

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 

void bacteria_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 


	// sample resource and ROS

	static int nR = microenvironment.find_density_index( "resource" ); 
	static int nROS = microenvironment.find_density_index( "ROS" ); 
	static int nDebris = microenvironment.find_density_index( "debris" ); 
	static int nQuorum = microenvironment.find_density_index( "quorum" );

	// if dead: stop exporting quorum factor, start exporting debris 
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nQuorum] = 0; 
		phenotype.secretion.net_export_rates[nDebris] = 1; 
		return; 
	}

	std::vector<double> samples = pCell->nearest_density_vector(); 
	double R = samples[nR];
	double ROS = samples[nROS]; 
	double Q = samples[nQuorum]; 

	// resource increases cycle entry 
	double base_val = pCD->phenotype.cycle.data.exit_rate(0); 
	double max_val = base_val * 10.0; 
	phenotype.cycle.data.exit_rate(0) = max_val * linear_response_function( R, 0.15, 1 );

	// resource decreses necrosis

	max_val = 0.0028;  
	static int nNecrosis = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model );
	phenotype.death.rates[nNecrosis] = max_val * decreasing_linear_response_function( R, 0.075, 0.15 );

	// resource decreases motile speed  

	base_val = pCD->phenotype.motility.migration_speed; 
	double max_response = 0.0; 
	double hill = Hill_response_function( R, 0.25 , 1.5);  
	phenotype.motility.migration_speed = base_val + (max_response-base_val)*hill;

	// quorum increases motility bias 
	base_val = pCD->phenotype.motility.migration_speed; 
	max_response = 1.0; 
	hill = Hill_response_function( Q, 0.5 , 1.5);  
	phenotype.motility.migration_bias = base_val + (max_response-base_val)*hill; 

	// damage increases death 
	// so does ROS 
	static int nApoptosis = phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model );

	double signal = ROS + pCell->state.damage / 180.0; 
	base_val = pCD->phenotype.death.rates[nApoptosis]; 
	max_response = 100*base_val;
	hill = Hill_response_function( signal , 0.2 , 1.5 ); 
	phenotype.death.rates[nApoptosis] = base_val + (max_response-base_val)*hill; 

	return; 
}

/* https://www.karger.com/Article/Fulltext/494069 */ 

void macrophage_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nROS = microenvironment.find_density_index( "ROS");
	static int nPIF = microenvironment.find_density_index( "pro-inflammatory" ); 
	static int nAIF = microenvironment.find_density_index( "anti-inflammatory" ); 
	static int nDebris = microenvironment.find_density_index( "debris" ); 
	std::vector<double> samples = pCell->nearest_density_vector(); 
	double PIF = samples[nPIF];
	double AIF = samples[nAIF]; 
	double ROS = samples[nROS];
	double debris = samples[nDebris]; 

	// sample contacts 

	static int Treg_type = find_cell_definition( "Treg")->type; 
	static int bacteria_type = find_cell_definition( "bacteria")->type; 

	int num_Treg = 0; 
	int num_bacteria = 0; 
	int num_dead = 0; 
	for( int n=0; n < pCell->state.neighbors.size(); n++ )
	{
		Cell* pC = pCell->state.neighbors[n]; 
		if( pC->phenotype.death.dead == true )
		{ num_dead++; }
		else
		{ 
			if( pC->type == Treg_type )
			{ num_Treg++; }
			if( pC->type == bacteria_type )
			{ num_bacteria++; }
		}
	}

	// contact with bacteria increases secretion of pro-inflamatory 
	// contact with dead cells increases secretion of pro-inflammatory 

	double base_val = pCD->phenotype.secretion.net_export_rates[nPIF]; 
	double max_response = 10; 
	double signal = 0.1*num_dead + num_bacteria; 
	double hill = Hill_response_function( signal , 1.0 , 1.5 ); 
	phenotype.secretion.net_export_rates[nPIF] = base_val + (max_response-base_val)*hill; 

	// contact with Treg decreases secretion of pro-inflamatory

	// weight chemotactic_sensitivity
	double total = debris + PIF; 
	phenotype.motility.chemotactic_sensitivity("debris") = debris / (total+1e-16); 
	phenotype.motility.chemotactic_sensitivity("pro-inflammatory") = PIF / (total+1e-16); 

	// high pro-inflammatory decreases motility 

	// high anti-inflamatory decreases secretion of pro-inflammatory 



}

void DC_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// not sure what I want to do with these 

	// contact with bacteria increases secretion of pro-inflamatory 

	// contact with Treg decreases secretion of pro-inflamatory

	// high pro-inflammatory decreases motility 



}

void CD8Tcell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// high pro-inflammatory increases damage rate 

	// contact with dendritic cells increases rate of attacking bacteria 


	// contact with Treg decreases damage rate

	// high pro-inflammatory decreases motility 

	// high anti-inflammatory decreases motility 



}

void Treg_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// high pro-inflammatory increases secretion of anti-inflammatory 

	// high pro-inflammatory decreases motility 



}

void neutrophil_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nROS = microenvironment.find_density_index( "ROS");
	static int nPIF = microenvironment.find_density_index( "pro-inflammatory" ); 
	static int nAIF = microenvironment.find_density_index( "anti-inflammatory" ); 
	std::vector<double> samples = pCell->nearest_density_vector(); 
	double PIF = samples[nPIF];
	double AIF = samples[nAIF]; 
	double ROS = samples[nROS];

	// sample contacts 

	static int Treg_type = find_cell_definition( "Treg")->type; 
	static int bacteria_type = find_cell_definition( "bacteria")->type; 

	int num_Treg = 0; 
	int num_bacteria = 0; 
	int num_dead = 0; 
	for( int n=0; n < pCell->state.neighbors.size(); n++ )
	{
		Cell* pC = pCell->state.neighbors[n]; 
		if( pC->phenotype.death.dead == true )
		{ num_dead++; }
		else
		{ 
			if( pC->type == Treg_type )
			{ num_Treg++; }
			if( pC->type == bacteria_type )
			{ num_bacteria++; }
		}
	}

	// contact with bacteria increases ROS export 
	double base_val = pCD->phenotype.secretion.net_export_rates[nROS];
	double max_response = 10; 
	double hill = Hill_response_function( num_bacteria , 1.0 , 1.5 ); 

	phenotype.secretion.net_export_rates[nROS] = base_val + (max_response-base_val)*hill; 

	// contact with Treg decreases ROS secretion 
	hill = Hill_response_function( num_Treg , 1.0 , 1.5 ); 
	phenotype.secretion.net_export_rates[nROS] *= (1.0-hill); 

	// high pro-inflammatory increases phagocytosis rate of bacteria  

	base_val = pCD->phenotype.cell_interactions.live_phagocytosis_rate( "bacteria");
	max_response = 2*base_val; 
	hill = Hill_response_function( PIF , 0.15 , 1.5 ); 
	phenotype.cell_interactions.live_phagocytosis_rate( "bacteria") = 
		base_val + (max_response-base_val)*hill; 

	// high pro-inflammatory decreases motility 

	base_val = pCD->phenotype.motility.migration_speed; 
	max_response = 0; 
	// reuse Hill calculation 
	phenotype.motility.migration_speed = base_val + (max_response-base_val)*hill; 

	// high anti-inflammatory decreases motility 
	hill = Hill_response_function( AIF , 0.15 , 1.5 ); 
	phenotype.motility.migration_speed *= (1.0-hill); 
	
	return; 
}

