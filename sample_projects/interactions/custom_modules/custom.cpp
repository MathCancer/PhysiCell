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

	// set up bacteria 

	Cell_Definition* pCD = find_cell_definition( "bacteria");
	pCD->functions.update_phenotype = bacteria_phenotype; 
	// pCD->functions.update_migration_bias = advanced_chemotaxis_function; 
	// pCD->phenotype.motility.chemotactic_sensitivity( "resource" ) = 1; 
	// pCD->phenotype.motility.chemotactic_sensitivity( "quorum" ) = 0.1; 

	// set up blood vessels 

	pCD = find_cell_definition( "blood vessel");
	pCD->is_movable = false; 

	// set up stem cells 

	pCD = find_cell_definition( "stem");
	pCD->functions.update_phenotype = stem_cell_phenotype; 
	// pCD->phenotype.cell_transformations.transformation_rate("differentiated") = 0.0001; 
	
	// set up differentiated cells 

	pCD = find_cell_definition( "differentiated");
	pCD->functions.update_phenotype = differentiated_cell_phenotype; 

	// set up macrophages 

	pCD = find_cell_definition( "macrophage");
	// pCD->phenotype.cell_interactions.dead_phagocytosis_rate = 0.05; 
	pCD->functions.update_phenotype = macrophage_phenotype; 
	// pCD->functions.update_migration_bias = advanced_chemotaxis_function; 
	// pCD->phenotype.motility.chemotactic_sensitivity( "debris" ) = 0.1; 
	// pCD->phenotype.motility.chemotactic_sensitivity( "quorum" ) = 1; 

	
	// set up CD8+ T cells 
	pCD = find_cell_definition( "CD8+ T cell");
	pCD->functions.update_phenotype = CD8Tcell_phenotype; 
	// pCD->phenotype.cell_interactions.attack_rate("bacteria") = 0.05; 

	// set up neutrophil  
	pCD = find_cell_definition( "neutrophil");
	pCD->functions.update_phenotype = neutrophil_phenotype; 
	// pCD->phenotype.cell_interactions.live_phagocytosis_rate("bacteria") = 0.05; 

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

	// stem cells 
	pCD = find_cell_definition("stem"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_stem_cells") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = Xmin + UniformRandom()*Xrange; 
		position[1] = Ymin + UniformRandom()*Yrange; 
		position[2] = Zmin + UniformRandom()*Zrange; 
		
		pC = create_cell( *pCD ); 
		pC->assign_position( position );
	}

	// differentiated cells 
	pCD = find_cell_definition("differentiated"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_differentiated_cells") ; n++ )
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
	
	// CD8+ T cells   
	pCD = find_cell_definition("CD8+ T cell"); 
	std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
	for( int n = 0 ; n < parameters.ints("number_of_CD8T_cells") ; n++ )
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

std::string my_coloring_function_for_substrate( double concentration, double max_conc, double min_conc )
{
	 return paint_by_density_percentage( concentration,  max_conc,  min_conc); 
	 
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

	// sample resource, quorum, and toxin 

	static int nR = microenvironment.find_density_index( "resource" ); 
	static int nDebris = microenvironment.find_density_index( "debris" ); 
	static int nQuorum = microenvironment.find_density_index( "quorum" );
	static int nToxin = microenvironment.find_density_index( "toxin" ); 

	// if dead: stop exporting quorum factor. 
	// also, replace phenotype function 
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nQuorum] = 0; 
		phenotype.secretion.net_export_rates[nToxin] = 0; 

		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		
		pCell->functions.update_phenotype = NULL; 
		return; 
	}

	std::vector<double> samples = pCell->nearest_density_vector(); 
	double R = samples[nR];
	double Q = samples[nQuorum]; 
	double Tox = samples[nToxin]; 

	// resource increases cycle entry 
	double base_val = pCD->phenotype.cycle.data.exit_rate(0); 
	double max_val = base_val * 10.0; 
	static double min_cycle_resource = pCD->custom_data["cycling_entry_threshold_resource"]; // 0.15 
	phenotype.cycle.data.exit_rate(0) = max_val * linear_response_function( R, min_cycle_resource, 1 );

	// resource decreses necrosis

	max_val = 0.0028;  
	static int nNecrosis = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model );
	static double saturation_necrosis_resource = pCD->custom_data["necrosis_saturation_resource"]; //0.075
	static double threshold_necrosis_resource = pCD->custom_data["necrosis_threshold_resource"]; // 0.15
	phenotype.death.rates[nNecrosis] = max_val * 
		decreasing_linear_response_function( R, saturation_necrosis_resource, threshold_necrosis_resource );

	// resource decreases motile speed  

	double signal = R; 
	base_val = pCD->phenotype.motility.migration_speed; 
	double max_response = 0.0; 
	static double motility_resource_halfmax = 
		pCD->custom_data["migration_speed_halfmax"]; // 0.25 // parameters.doubles("bacteria_motility_resource_halfmax");
	double hill = Hill_response_function( signal, motility_resource_halfmax , 1.5);  
	phenotype.motility.migration_speed = base_val + (max_response-base_val)*hill;

	// quorum and resource increases motility bias 
	signal = Q+R; 
	base_val = pCD->phenotype.motility.migration_speed; 
	max_response = 1.0; 
	static double bias_halfmax = pCD->custom_data["migration_bias_halfmax"]; 
		// 0.5 //  parameters.doubles("bacteria_migration_bias_halfmax");
	hill = Hill_response_function( signal, bias_halfmax , 1.5);  
	phenotype.motility.migration_bias = base_val + (max_response-base_val)*hill; 

	// damage increases death 
	static int nApoptosis = phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model );

	signal = pCell->phenotype.cell_integrity.damage;
	base_val = pCD->phenotype.death.rates[nApoptosis]; 

	static double damage_halfmax = pCD->custom_data["damage_halfmax"]; 
	static double relative_max_damage_death = pCD->custom_data["relative_max_damage_death"]; 
	max_response = base_val * relative_max_damage_death; 

		// 36 // parameters.doubles("bacteria_damage_halfmax");
	hill = Hill_response_function( signal , damage_halfmax , 1.5 ); 
	phenotype.death.rates[nApoptosis] = base_val + (max_response-base_val)*hill; 

	return; 
}

/* https://www.karger.com/Article/Fulltext/494069 */ 

void macrophage_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nPIF = microenvironment.find_density_index( "pro-inflammatory" ); 
	static int nDebris = microenvironment.find_density_index( "debris"); 
	static int nQ = microenvironment.find_density_index("quorum");

	// if dead, release debris
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		pCell->functions.update_phenotype = NULL; 
		return;
	}	

	std::vector<double> samples = pCell->nearest_density_vector(); 
	double PIF = samples[nPIF];
	double debris = samples[nDebris]; 
	double Q = samples[nQ];

	// sample contacts 

	static int bacteria_type = find_cell_definition( "bacteria")->type; 

	int num_bacteria = 0; 
	int num_dead = 0; 
	for( int n=0; n < pCell->state.neighbors.size(); n++ )
	{
		Cell* pC = pCell->state.neighbors[n]; 
		if( pC->phenotype.death.dead == true )
		{ num_dead++; }
		else
		{ 
			if( pC->type == bacteria_type )
			{ num_bacteria++; }
		}
	}

	// contact with dead cells or bacteria, or debris 
	// increases secretion of pro-inflammatory 

	static double secretion_dead_sensitivity = 1; 
	static double secretion_bacteria_sensitivity = 1; 
	static double secretion_debris_sensitivity = 2; 
	static double secretion_quorum_sensitivity = 5; 

	double base_val = pCD->phenotype.secretion.secretion_rates[nPIF]; 
	double max_response = 10; // phenotype.volume.total; 
	double signal = 
		secretion_dead_sensitivity*num_dead + 
		secretion_bacteria_sensitivity*num_bacteria + 
		secretion_debris_sensitivity*debris + 
		secretion_quorum_sensitivity*Q; 
	double half_max = pCD->custom_data["secretion_halfmax"]; // 0.5; // 0.5; 
	double hill = Hill_response_function( signal , half_max , 1.5 ); 
	
	
	phenotype.secretion.secretion_rates[nPIF] = base_val + (max_response-base_val)*hill; 
	
/*	
	#pragma omp critical
	{
	std::cout << "secretion index: " << nPIF << " base: " << base_val << " max: " << max_response << " actual: " << phenotype.secretion.secretion_rates[nPIF] << std::endl; 
	std::cout << "\tsignal: " << signal << " vs halfmax: " << half_max << std::endl; 
	std::cout << "\t\tdead: " << num_dead << " bac: " << num_bacteria << " debris: " << debris << " Q: " << Q << std::endl; 
	std::cout << "\t\t\tsaturation: " << phenotype.secretion.saturation_densities[nPIF]<< std::endl; 
	}
*/	

	// chemotaxis bias increases with debris or quorum factor 

	static double bias_debris_sensitivity = 0.1; 
	static double bias_quorum_sensitivity = 1; 

	base_val = pCD->phenotype.motility.migration_bias; 
	max_response = 0.75; 
	signal = bias_debris_sensitivity*debris + 
		bias_quorum_sensitivity*Q ; // + 10 * PIF; 
	half_max = pCD->custom_data["migration_bias_halfmax"]; // 0.01 // 0.005 //0.1 // 0.05
	hill = Hill_response_function( signal , half_max , 1.5 ); 
	phenotype.motility.migration_bias = base_val + (max_response-base_val)*hill; 	

/*
	#pragma omp critical 
	{
	std::cout << "signal: " << signal << " halfmax: " << half_max 
	<< " hill: " << hill << std::endl; 
	
	std::cout << "\tbase: " << base_val 
	<< " max: " << max_response 
	<< " actual: " << phenotype.motility.migration_bias << std::endl; 
	}
*/

	// migration speed slows down in the presence of debris or quorum factor 

	base_val = pCD->phenotype.motility.migration_speed; 
	max_response = 0.1 * base_val; 
	signal = bias_debris_sensitivity*debris + 
		bias_quorum_sensitivity*Q ; // + 10 * PIF; 
	half_max = pCD->custom_data["migration_speed_halfmax"]; // 0.1 // 0.05 
	hill = Hill_response_function( signal , half_max , 1.5 ); 
	phenotype.motility.migration_speed = base_val + (max_response-base_val)*hill; 	

	return; 
}

void CD8Tcell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nR = microenvironment.find_density_index( "resource");
	static int nTox = microenvironment.find_density_index( "toxin");
	static int nDebris = microenvironment.find_density_index( "debris" );
	static int nPIF = microenvironment.find_density_index( "pro-inflammatory"); 
	
	std::vector<double> samples = pCell->nearest_density_vector(); 
	double PIF = samples[nPIF];	
	
	// if dead, release debris
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		pCell->functions.update_phenotype = NULL; 
		return;
	}
	
	// migration bias increases with pro-inflammatory 

	double signal = PIF; 
	double base_val = pCD->phenotype.motility.migration_bias; 
	double max_val = 0.75; 
	double half_max = pCD->custom_data["migration_bias_halfmax"]; // 0.05 // 0.25 
	double hill = Hill_response_function( PIF , half_max , 1.5 ); 

	phenotype.motility.migration_bias = base_val + (max_val-base_val)*hill; 
	
/*	
	#pragma omp critical 
	{
		std::cout << "signal: " << signal << " halfmax: " << half_max 
		<< " hill: " << hill << std::endl; 
		
		std::cout << "\tbase: " << base_val 
		<< " max: " << max_val 
		<< " actual: " << phenotype.motility.migration_bias << std::endl; 
	}	
*/	

	return; 
}

void neutrophil_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nR = microenvironment.find_density_index( "resource");
	static int nTox = microenvironment.find_density_index( "toxin");
	static int nDebris = microenvironment.find_density_index( "debris" );
	static int nPIF = microenvironment.find_density_index( "pro-inflammatory"); 
	
	std::vector<double> samples = pCell->nearest_density_vector(); 
	double PIF = samples[nPIF];	
	
	// if dead, release debris
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		pCell->functions.update_phenotype = NULL; 
		return;
	}

	// migration bias increases with pro-inflammatory 

	double signal = PIF; 
	double base_val = pCD->phenotype.motility.migration_bias; 
	double max_val = 0.75; 
	double half_max = pCD->custom_data["migration_bias_halfmax"]; // 0.25 
	double hill = Hill_response_function( PIF , half_max , 1.5 ); 

	phenotype.motility.migration_bias = base_val + (max_val-base_val)*hill; 

	return; 
}

void stem_cell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nR = microenvironment.find_density_index( "resource");
	static int nTox = microenvironment.find_density_index( "toxin");
	static int nDebris = microenvironment.find_density_index( "debris" ); 

	// if dead, release debris
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		pCell->functions.update_phenotype = NULL; 
		return;
	}

	std::vector<double> samples = pCell->nearest_density_vector(); 
	double R = samples[nR];
	double toxin = samples[nTox];

	// sample contacts 

	static int stem_type = find_cell_definition( "stem")->type; 
	static int diff_type = find_cell_definition( "differentiated")->type; 
	static int bacteria_type = find_cell_definition( "bacteria")->type; 

	int num_stem = 0; 
	int num_differentiated = 0; 
	int num_bacteria = 0; 
	int num_dead = 0; 
	for( int n=0; n < pCell->state.neighbors.size(); n++ )
	{
		Cell* pC = pCell->state.neighbors[n]; 
		if( pC->phenotype.death.dead == true )
		{ num_dead++; }
		else
		{ 
			if( pC->type == stem_type )
			{ num_stem++; }
			if( pC->type == num_differentiated )
			{ num_differentiated++; }
			if( pC->type == bacteria_type )
			{ num_bacteria++; }
		}
	}

	// contact with a stem cell increases differentiation 
	static double max_stem_diff = parameters.doubles("max_stem_differentiation"); // 0.0075 
	static double stem_diff_halfmax = pCD->custom_data["differentiation_contact_halfmax"]; // 0.1 

	double base_val = 0; // phenotype.cell_transformations.transformation_rates[diff_type]; 
	double max_val = max_stem_diff; // 0.0075;
	double signal = num_stem; 
	double half_max = stem_diff_halfmax; // 0.1; 
	double hill = Hill_response_function( signal, half_max , 1.5 ); 
	phenotype.cell_transformations.transformation_rates[diff_type] = base_val + (max_val-base_val)*hill; 

	// contact with a differentiated cell reduces proliferation 
	// high rate of proliferation unless in contact with a differentiated cell 

	static double stem_cycling_halfmax = pCD->custom_data["cycling_contact_halfmax"]; // 0.1; 

	base_val = pCD->phenotype.cycle.data.exit_rate(0); // 0.002; 
	max_val = 0.0; 
	signal = num_differentiated; 
	half_max = stem_cycling_halfmax; //  0.1; 
	hill = Hill_response_function( signal, half_max , 1.5 ); 
	phenotype.cycle.data.exit_rate(0) = base_val + (max_val-base_val)*hill; 

	// resource reduces necrotic death 

	max_val = 0.0028;  
	static int nNecrosis = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model );
	static double stem_saturation_necrosis = pCD->custom_data["necrosis_saturation_resource"];
	static double stem_threshold_necrosis = pCD->custom_data["necrosis_threshold_resource"];

	phenotype.death.rates[nNecrosis] = max_val * 
		decreasing_linear_response_function( R, stem_saturation_necrosis, stem_threshold_necrosis );

	// toxin increases apoptotic death 
	
	static int nApoptosis = phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model );

	static double toxicity_halfmax = pCD->custom_data["toxicity_halfmax"]; // 0.4 
	static double relative_max_toxicity = pCD->custom_data["relative_max_toxicity"]; 

	signal = toxin; 
	base_val = pCD->phenotype.death.rates[nApoptosis]; 
	max_val = base_val * relative_max_toxicity; // 100*base_val;

	hill = Hill_response_function( signal , toxicity_halfmax , 1.5 ); 
	phenotype.death.rates[nApoptosis] = base_val + (max_val-base_val)*hill; 
	
	return; 
}

void differentiated_cell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// find my cell definition 
	static Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	// sample environment 

	static int nR = microenvironment.find_density_index( "resource");
	static int nTox = microenvironment.find_density_index( "toxin");
	static int nDebris = microenvironment.find_density_index( "debris" );
	
	// if dead, release debris
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.net_export_rates[nDebris] = phenotype.volume.total; 
		pCell->functions.update_phenotype = NULL; 
		return;
	}
	
	std::vector<double> samples = pCell->nearest_density_vector(); 
	double R = samples[nR];
	double toxin = samples[nTox];


	double signal = 0.0; 
	double hill = 0.0; 

	// pressure reduces proliferation 
	signal = pCell->state.simple_pressure;  
	static double pressure_halfmax = pCD->custom_data["cycling_pressure_halfmax"]; // 0.5 
	hill = Hill_response_function( signal, pressure_halfmax , 1.5 );  
	double base_val = pCD->phenotype.cycle.data.exit_rate(0); 

	phenotype.cycle.data.exit_rate(0) = (1-hill)*base_val; 

	// resource reduces necrotic death 

	double max_val = 0.0028;  
	static int nNecrosis = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model );

	// get same from bacteria
	static double necrosis_saturation = pCD->custom_data["necrosis_saturation_resource"]; // 0.075 
	static double necrosis_threshold = pCD->custom_data["necrosis_threshold_resource"]; // 0.15 

	phenotype.death.rates[nNecrosis] = max_val * 
		decreasing_linear_response_function( R, necrosis_saturation, necrosis_threshold ); 

	// toxin increases apoptotic death 
	
	static int nApoptosis = phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model );

	static double toxicity_halfmax = pCD->custom_data["toxicity_halfmax"]; // 0.2 
	static double relative_max_tox_death = pCD->custom_data["relative_max_toxicity"]; // 100 

	signal = toxin; 
	base_val = pCD->phenotype.death.rates[nApoptosis]; 
	double max_response = base_val * relative_max_tox_death; 
	hill = Hill_response_function( signal , toxicity_halfmax , 1.5 ); 
	// std::cout << "tox: " << signal << " " << hill << std::endl; 
	phenotype.death.rates[nApoptosis] = base_val + (max_response-base_val)*hill; 

	return; 
}
