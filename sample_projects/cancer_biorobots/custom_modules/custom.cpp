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

	// ?? // 
	cell_defaults.parameters.o2_proliferation_saturation = 38.0;  
	cell_defaults.parameters.o2_reference = 38.0; 

	// cancer cells 
	Cell_Definition* pCD = find_cell_definition( "cancer cell"); 
	pCD->functions.update_phenotype = tumor_cell_phenotype_with_therapy; 

	pCD->parameters.o2_proliferation_saturation = 38.0;  
	pCD->parameters.o2_reference = 38.0; 

	// cargo cells 
	pCD = find_cell_definition( "cargo cell"); 

	// figure out mechanics parameters 
	
	pCD->phenotype.mechanics.relative_maximum_attachment_distance 
		= pCD->custom_data["max_attachment_distance"] / pCD->phenotype.geometry.radius ; 

	pCD->phenotype.mechanics.relative_detachment_distance 
		= pCD->custom_data["max_elastic_displacement"] / pCD->phenotype.geometry.radius ; 
		
	pCD->phenotype.mechanics.attachment_elastic_constant 
		= pCD->custom_data["elastic_coefficient"]; 
	
	// set functions 
	pCD->functions.update_phenotype = cargo_cell_phenotype_rule; 
	pCD->functions.custom_cell_rule = cargo_cell_rule; 
	pCD->functions.contact_function = biorobots_contact_function; 
	pCD->functions.update_migration_bias = NULL;	

	// worker cells 

	pCD = find_cell_definition( "worker cell");

	pCD->phenotype.mechanics.relative_maximum_attachment_distance 
		= pCD->custom_data["max_attachment_distance"] / pCD->phenotype.geometry.radius ; 

	pCD->phenotype.mechanics.relative_detachment_distance 
		= pCD->custom_data["max_elastic_displacement"] / pCD->phenotype.geometry.radius ; 
		
	pCD->phenotype.mechanics.attachment_elastic_constant 
		= pCD->custom_data["elastic_coefficient"]; 

	pCD->functions.update_phenotype = NULL; // worker_cell_rule; 
	pCD->functions.custom_cell_rule = worker_cell_rule;  
	pCD->functions.contact_function = biorobots_contact_function; 
	
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

	// custom placement 
	// place a cluster of tumor cells at the center 
	
	double cell_radius = cell_defaults.phenotype.geometry.radius; 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	double tumor_radius = parameters.doubles("tumor_radius"); // 200.0; 
	
	Cell* pCell = NULL; 
	Cell_Definition* pCD_cancer = find_cell_definition( "cancer cell"); 
	
	double x = 0.0; 
	double x_outer = tumor_radius; 
	double y = 0.0; 
	
	int n = 0; 
	while( y < tumor_radius )
	{
		x = 0.0; 
		if( n % 2 == 1 )
		{ x = 0.5*cell_spacing; }
		x_outer = sqrt( tumor_radius*tumor_radius - y*y ); 
		
		while( x < x_outer )
		{
			pCell = create_cell(); // tumor cell 
			pCell->assign_position( x , y , 0.0 );
			
			if( fabs( y ) > 0.01 )
			{
				pCell = create_cell(*pCD_cancer); // tumor cell 
				pCell->assign_position( x , -y , 0.0 );				
			}
			
			if( fabs( x ) > 0.01 )
			{ 
				pCell = create_cell(*pCD_cancer); // tumor cell 
				pCell->assign_position( -x , y , 0.0 );
				
				if( fabs( y ) > 0.01 )
				{
					pCell = create_cell(*pCD_cancer); // tumor cell 
					pCell->assign_position( -x , -y , 0.0 );
				}
			}
			x += cell_spacing; 
		}
		
		y += cell_spacing * sqrt(3.0)/2.0; 
		n++; 
	}

	
	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml(); 	
	
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

std::vector<std::string> cancer_biorobots_coloring_function( Cell* pCell )
{
	std::vector< std::string > output( 4, "black" ); 
	
	double damage = get_single_signal( pCell, "damage"); 

	static double max_damage = 1.0 * get_single_signal(pCell,"custom:damage_rate") 
		/ (1e-16 + get_single_signal(pCell,"custom:repair_rate" ) );

	static Cell_Definition* pCD_cargo = find_cell_definition( "cargo cell"); 
	static Cell_Definition* pCD_cancer = find_cell_definition( "cancer cell"); 
	static Cell_Definition* pCD_worker = find_cell_definition( "worker cell"); 
	
	// cargo cell 
	if( pCell->type == pCD_cargo->type )
	{
		output[0] = "blue";
		output[1] = "blue";
		output[2] = "blue"; 
		output[3] = "none"; // no nuclear outline color 
		return output;
	}
	
	// worker cell 
	if( pCell->type == pCD_worker->type )
	{
		output[0] = "red";
		output[1] = "red";
		output[2] = "red"; 
		output[3] = "none"; // no nuclear outline color 
		return output;
	}
	
	// apoptotic tumor - cyan 
	if( get_single_signal( pCell, "apoptotic" ) > 0.5 )  // Apoptotic - cyan
	{
		output[0] = "cyan";
		output[2] = "darkcyan"; 
		return output; 
	}	
	
	// Necrotic tumor - Brown
	if( get_single_signal( pCell, "necrotic") > 0.5 )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
		return output; 
	}		
	
	// live tumor -- shade by level of damage 
	
	
	// if live: color by damage 
	if( get_single_signal( pCell, "dead") < 0.5 )
	{
		int damage_int = (int) round( damage * 255.0 / max_damage ); 
		
		char szTempString [128];
		sprintf( szTempString , "rgb(%u,%u,%u)" , damage_int , 255-damage_int , damage_int );
		output[0].assign( szTempString );
		output[1].assign( szTempString );
		sprintf( szTempString , "rgb(%u,%u,%u)" , damage_int/4 , (255-damage_int)/4 , damage_int/4 );
		output[2].assign( szTempString );
	}
	return output; 
}

void introduce_biorobots( void )
{
	// idea: we'll "inject" them in a little column
		
	static double worker_fraction = 
		parameters.doubles("worker_fraction"); // 0.10; /* param */ 
	static int number_of_injected_cells = 
		parameters.ints("number_of_injected_cells"); // 500; /* param */ 
	
	// make these vary with domain size 
	double left_coordinate = default_microenvironment_options.X_range[1] - 150.0; // 600.0; 
	double right_cooridnate = default_microenvironment_options.X_range[1] - 50.0; // 700.0;

	double bottom_coordinate = default_microenvironment_options.Y_range[0] + 50.0; // -700; 
	double top_coordinate = default_microenvironment_options.Y_range[1] - 50.0; // 700; 

	Cell_Definition* pCD_worker = find_cell_definition( "worker cell");
	Cell_Definition* pCD_cargo = find_cell_definition( "cargo cell");
		
	for( int i=0 ;i < number_of_injected_cells ; i++ )
	{
		std::vector<double> position = {0,0,0}; 
		position[0] = left_coordinate + (right_cooridnate-left_coordinate)*UniformRandom(); 
		position[1] = bottom_coordinate + (top_coordinate-bottom_coordinate)*UniformRandom(); 
		
		Cell* pCell;  
		if( UniformRandom() <= worker_fraction )
		{ pCell = create_cell( *pCD_worker ); }
		else
		{ pCell = create_cell( *pCD_cargo ); }
		pCell->assign_position( position ); 
	}
	
	return; 
}

void cargo_cell_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	if( get_single_signal( pCell, "dead" ) > 0.5 )
	{
		// the cell death functions don't automatically turn off custom functions, 
		// since those are part of mechanics. 
		
		// Let's just fully disable now. 
		pCell->functions.custom_cell_rule = NULL; 
		return; 
	}
	
	// if I'm docked
	if( pCell->state.number_of_attached_cells() > 0 )
	{
		set_single_behavior( pCell, "migration speed" , 0.0 ); 
		return; 
	}
	
	return; 
}

void cargo_cell_phenotype_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if dettached and receptor on, secrete signal 
	
	// if dettached and receptor off, secrete chemo

	double receptor = get_single_signal( pCell , "custom:receptor" ); 
	
	if( pCell->state.number_of_attached_cells() == 0 )
	{
		if( receptor > 0.1 )
		{
			set_single_behavior( pCell , "chemoattractant secretion" , 10); 
			set_single_behavior( pCell , "therapeutic secretion" , 0); 
		}
		else
		{
			set_single_behavior( pCell , "chemoattractant secretion" , 0); 
			set_single_behavior( pCell , "therapeutic secretion" , 10); 
		}
		return; 
	}
	
	// if you reach this point of the code, the cell is attached 
	

	// if attached and oxygen high, secrete nothing, receptor off 
	
	// if attached and oxygen low, dettach, start secreting chemo, receptor off   

	double o2 = get_single_signal( pCell, "oxygen"); 
	double o2_drop = get_single_signal( pCell , "custom:cargo_release_o2_threshold"); 
	
	if( o2 > o2_drop )
	{
		set_single_behavior( pCell , "chemoattractant secretion" , 0); 
		set_single_behavior( pCell , "therapeutic secretion" , 0); 
		set_single_behavior( pCell , "custom:receptor" , 0 ); 
	}
	else
	{
		set_single_behavior( pCell , "chemoattractant secretion" , 0); 
		set_single_behavior( pCell , "therapeutic secretion" , 10); 
		set_single_behavior( pCell , "custom:receptor" , 0 ); 
		
		pCell->remove_all_attached_cells(); 
	}
	
	return; 
}

void biorobots_contact_function( Cell* pActingOn, Phenotype& pao, Cell* pAttachedTo, Phenotype& pat , double dt )
{
	std::vector<double> displacement = pAttachedTo->position - pActingOn->position; 
	
	static double max_elastic_displacement = pao.geometry.radius * pao.mechanics.relative_detachment_distance; 
	static double max_displacement_squared = max_elastic_displacement*max_elastic_displacement; 
	
	// detach cells if too far apart 
	
	if( norm_squared( displacement ) > max_displacement_squared )
	{
		detach_cells( pActingOn , pAttachedTo );
		return; 
	}
	
	axpy( &(pActingOn->velocity) , pao.mechanics.attachment_elastic_constant , displacement ); 
	
	return; 
}

void tumor_cell_phenotype_with_therapy( Cell* pCell, Phenotype& phenotype, double dt )
{
	double damage = get_single_signal( pCell, "damage"); 

	double damage_rate = get_single_signal( pCell , "custom:damage_rate"); 
	double repair_rate = get_single_signal( pCell , "custom:repair_rate"); 
	double drug_death_rate = get_single_signal( pCell , "custom:drug_death_rate" ); 

	double drug = get_single_signal( pCell , "therapeutic"); 
	
	static double max_damage = 1.0 * damage_rate / (1e-16 + repair_rate );
	
	// if I'm dead, don't bother. disable my phenotype rule
	if( get_single_signal( pCell, "dead") > 0.5 )
	{
		pCell->functions.update_phenotype = NULL; 
		return; 
	}
	
	// first, vary the cell birth and death rates with oxygenation
	
	// std::cout << get_single_behavior( pCell , "cycle entry") << " vs "; 
	update_cell_and_death_parameters_O2_based(pCell,phenotype,dt);
	// std::cout << get_single_behavior( pCell , "cycle entry") << std::endl; 

	// the update the cell damage 
	
	// dD/dt = alpha*c - beta-D by implicit scheme 
	
	double temp = drug;
	
	// reuse temp as much as possible to reduce memory allocations etc. 
	temp *= dt; 
	temp *= damage_rate; 
	
	damage += temp; // d_prev + dt*chemo*damage_rate 
	
	temp = repair_rate;
	temp *= dt; 
	temp += 1.0; 
	damage /= temp;  // (d_prev + dt*chemo*damage_rate)/(1 + dt*repair_rate)
	
	// then, see if the cell undergoes death from the therapy 
	
	temp = dt; 
	temp *= damage; 
	temp *= drug_death_rate; 
	temp /= max_damage; // dt*(damage/max_damage)*death_rate 

	// make sure we write the damage (not current a behavior)
	pCell->phenotype.cell_integrity.damage = damage; 

	if( UniformRandom() <= temp )
	{
		// pCell->start_death( apoptosis_model_index );
		set_single_behavior( pCell, "apoptosis" , 9e99 ); 
		pCell->functions.update_phenotype = NULL; 		
		pCell->functions.custom_cell_rule = NULL; 
	}

	return; 
}

void worker_cell_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if I am dead, don't bother

	if( get_single_signal( pCell , "dead") > 0.5 )
	{
		// the cell death functions don't automatically turn off custom functions, 
		// since those are part of mechanics. 
		
		// Let's just fully disable now. 
		pCell->functions.custom_cell_rule = NULL; 
		return; 
	}
	
	// am I searching for cargo? if so, see if I've found it
	if( pCell->state.number_of_attached_cells() == 0 )
	{
		std::vector<Cell*> nearby = pCell->cells_in_my_container(); 
		bool attached = false; // want to limit to one attachment 
		int i =0;
		while( i < nearby.size() && attached == false )
		{
			// if it is expressing the receptor, dock with it 
			if( get_single_signal(nearby[i],"custom:receptor") > 0.5 && attached == false )
			{
				attach_cells( pCell, nearby[i] ); 
				// nearby[i]->custom_data["receptor"] = 0.0; // put into cargo cell rule instead? 
				// nearby[i]->phenotype.secretion.set_all_secretion_to_zero(); // put into cargo rule instead? 
				attached = true; 
			}
			i++; 
		}
	}

	// from prior motility function 

	double o2 = get_single_signal( pCell, "oxygen");
	double chemoattractant = get_single_signal( pCell , "chemoattractant"); 

	static double detection_threshold = get_single_signal( pCell, "custom:motility_shutdown_detection_threshold"); 
	
	// if attached, biased motility towards director chemoattractant 
	// otherwise, biased motility towards cargo chemoattractant 
	
	static double attached_worker_migration_bias = get_single_signal( pCell, "custom:attached_worker_migration_bias"); 
	static double unattached_worker_migration_bias = get_single_signal( pCell , "custom:unattached_worker_migration_bias"); 
	
	if( pCell->state.number_of_attached_cells() > 0 )
	{
		set_single_behavior( pCell , "migration bias" , attached_worker_migration_bias ); 

		set_single_behavior( pCell , "chemotactic response to oxygen" , -1 ); 
		set_single_behavior( pCell , "chemotactic response to chemoattractant" , 0 ); 
	}
	else
	{
		// if there is no detectable signal, shut down motility (permanently)
		if( chemoattractant < detection_threshold )
		{
			set_single_behavior( pCell, "migration speed" , 0 ); 
		}
		
		set_single_behavior( pCell , "migration bias" , unattached_worker_migration_bias ); 
		
 		set_single_behavior( pCell , "chemotactic response to oxygen" , 0 ); 
		set_single_behavior( pCell , "chemotactic response to chemoattractant" , 1 ); 
	}
	
	return; 
}


