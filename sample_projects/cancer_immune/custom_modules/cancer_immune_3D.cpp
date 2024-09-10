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

#include "./cancer_immune_3D.h"

Cell_Definition* pImmuneCell; 

void create_immune_cell_type( void )
{
	pImmuneCell = find_cell_definition( "immune cell" ); 
	
	static int oxygen_ID = microenvironment.find_density_index( "oxygen" ); 
	static int immuno_ID = microenvironment.find_density_index( "immunostimulatory factor" ); 
	
	// reduce o2 uptake 
	
	pImmuneCell->phenotype.secretion.uptake_rates[oxygen_ID] *= 
		parameters.doubles("immune_o2_relative_uptake");  
	
	pImmuneCell->phenotype.mechanics.cell_cell_adhesion_strength *= 
		parameters.doubles("immune_relative_adhesion"); 
	pImmuneCell->phenotype.mechanics.cell_cell_repulsion_strength *= 
		parameters.doubles("immune_relative_repulsion"); 
		
	// figure out mechanics parameters 
	
	pImmuneCell->phenotype.mechanics.relative_maximum_attachment_distance 
		= pImmuneCell->custom_data["max_attachment_distance"] / pImmuneCell->phenotype.geometry.radius ; 
		
	pImmuneCell->phenotype.mechanics.attachment_elastic_constant 
		= pImmuneCell->custom_data["elastic_coefficient"]; 		
	
	pImmuneCell->phenotype.mechanics.relative_detachment_distance 
		= pImmuneCell->custom_data["max_attachment_distance" ] / pImmuneCell->phenotype.geometry.radius ; 		
	
	// set functions 
	
	pImmuneCell->functions.update_phenotype = NULL; 
	pImmuneCell->functions.custom_cell_rule = immune_cell_rule; 
	pImmuneCell->functions.update_migration_bias = immune_cell_motility;
	pImmuneCell->functions.contact_function = adhesion_contact_function; 
	


	// set custom data values 
	
	return; 
}

void create_cell_types( void )
{
	// use the same random seed so that future experiments have the 
	// same initial histogram of oncoprotein, even if threading means 
	// that future division and other events are still not identical 
	// for all runs 
	// set the random seed 
	if (parameters.ints.find_index("random_seed") != -1)
	{
		SeedRandom(parameters.ints("random_seed"));
	}
	
	// housekeeping 
	
	initialize_default_cell_definition();
	
	
	cell_defaults.parameters.o2_proliferation_saturation = 38.0;  
	cell_defaults.parameters.o2_reference = 38.0; 

	static int oxygen_ID = microenvironment.find_density_index( "oxygen" ); // 0 
	static int immuno_ID = microenvironment.find_density_index( "immunostimulatory factor" ); // 1
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/

	initialize_cell_definitions_from_pugixml(); 
	
	// change the max cell-cell adhesion distance 
	cell_defaults.phenotype.mechanics.relative_maximum_attachment_distance = 
		cell_defaults.custom_data["max_attachment_distance"] / cell_defaults.phenotype.geometry.radius;
		
	cell_defaults.phenotype.mechanics.relative_detachment_distance 
		= cell_defaults.custom_data["max_attachment_distance"] / cell_defaults.phenotype.geometry.radius ; 
		
	cell_defaults.phenotype.mechanics.attachment_elastic_constant 
		= cell_defaults.custom_data[ "elastic_coefficient" ];	
		
	cell_defaults.functions.update_phenotype = tumor_cell_phenotype_with_and_immune_stimulation; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	cell_defaults.functions.contact_function = adhesion_contact_function; 
	cell_defaults.functions.update_migration_bias = NULL; 

	// create the immune cell type 
	create_immune_cell_type(); 

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
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		std::cout << "Warning: overriding 2D setting to return to 3D" << std::endl; 
		default_microenvironment_options.simulate_2D = false; 
	}
	
	initialize_microenvironment(); 	

	return; 
}	

void introduce_immune_cells( void )
{
	double tumor_radius = -9e9; // 250.0; 
	double temp_radius = 0.0; 
	
	// for the loop, deal with the (faster) norm squared 
	for( int i=0; i < (*all_cells).size() ; i++ )
	{
		temp_radius = norm_squared( (*all_cells)[i]->position ); 
		if( temp_radius > tumor_radius )
		{ tumor_radius = temp_radius; }
	}
	// now square root to get to radius 
	tumor_radius = sqrt( tumor_radius ); 
	
	// if this goes wackadoodle, choose 250 
	if( tumor_radius < 250.0 )
	{ tumor_radius = 250.0; }
	
	std::cout << "current tumor radius: " << tumor_radius << std::endl; 
	
	// now seed immune cells 
	
	int number_of_immune_cells = 
		parameters.ints("number_of_immune_cells"); // 7500; // 100; // 40; 
	double radius_inner = tumor_radius + 
		parameters.doubles("initial_min_immune_distance_from_tumor"); 30.0; // 75 // 50; 
	double radius_outer = radius_inner + 
		parameters.doubles("thickness_of_immune_seeding_region"); // 75.0; // 100; // 1000 - 50.0; 
	
	double mean_radius = 0.5*(radius_inner + radius_outer); 
	double std_radius = 0.33*( radius_outer-radius_inner)/2.0; 
	
	for( int i=0 ;i < number_of_immune_cells ; i++ )
	{
		double theta = UniformRandom() * 6.283185307179586476925286766559; 
		double phi = acos( 2.0*UniformRandom() - 1.0 );  
		
		double radius = NormalRandom( mean_radius, std_radius ); 
		
		Cell* pCell = create_cell( *pImmuneCell ); 
		pCell->assign_position( radius*cos(theta)*sin(phi), radius*sin(theta)*sin(phi), radius*cos(phi) ); 
	}
	
	return; 
}


std::vector<std::vector<double>> create_cell_sphere_positions(double cell_radius, double sphere_radius)
{
	std::vector<std::vector<double>> cells;
	int xc=0,yc=0,zc=0;
	double x_spacing= cell_radius*sqrt(3);
	double y_spacing= cell_radius*2;
	double z_spacing= cell_radius*sqrt(3);
	
	std::vector<double> tempPoint(3,0.0);
	// std::vector<double> cylinder_center(3,0.0);
	
	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
	{
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
		{
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0]=x + (zc%2) * 0.5 * cell_radius;
				tempPoint[1]=y + (xc%2) * cell_radius;
				tempPoint[2]=z;
				
				if(sqrt(norm_squared(tempPoint))< sphere_radius)
				{ cells.push_back(tempPoint); }
			}
			
		}
	}
	return cells;
	
}

void setup_tissue( void )
{
	// place a cluster of tumor cells at the center 
	
	double cell_radius = cell_defaults.phenotype.geometry.radius; 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	double tumor_radius = 
		parameters.doubles("tumor_radius"); // 250.0; 
	
	Cell* pCell = NULL; 
	
	std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius,tumor_radius); 
	std::cout << "creating " << positions.size() << " closely-packed tumor cells ... " << std::endl; 
	
	static double imm_mean = parameters.doubles("tumor_mean_immunogenicity"); 
	static double imm_sd = parameters.doubles("tumor_immunogenicity_standard_deviation"); 
		
	for( int i=0; i < positions.size(); i++ )
	{
		pCell = create_cell(); // tumor cell 
		pCell->assign_position( positions[i] );
		pCell->custom_data["oncoprotein"] = NormalRandom( imm_mean, imm_sd );
		if( pCell->custom_data["oncoprotein"] < 0.0 )
		{ pCell->custom_data["oncoprotein"] = 0.0; } 
	}
	
	double sum = 0.0; 
	double min = 9e9; 
	double max = -9e9; 
	for( int i=0; i < all_cells->size() ; i++ )
	{
		double r = (*all_cells)[i]->custom_data["oncoprotein"]; 
		sum += r;
		if( r < min )
		{ min = r; } 
		if( r > max )
		{ max = r; }
	}
	double mean = sum / ( all_cells->size() + 1e-15 ); 
	// compute standard deviation 
	sum = 0.0; 
	for( int i=0; i < all_cells->size(); i++ )
	{
		sum +=  ( (*all_cells)[i]->custom_data["oncoprotein"] - mean )*( (*all_cells)[i]->custom_data["oncoprotein"] - mean ); 
	}
	double standard_deviation = sqrt( sum / ( all_cells->size() - 1.0 + 1e-15 ) ); 
	
	std::cout << std::endl << "Oncoprotein summary: " << std::endl
			  << "===================" << std::endl; 
	std::cout << "mean: " << mean << std::endl; 
	std::cout << "standard deviation: " << standard_deviation << std::endl; 
	std::cout << "[min max]: [" << min << " " << max << "]" << std::endl << std::endl; 
	
	return; 
}

// custom cell phenotype function to scale immunostimulatory factor with hypoxia 
void tumor_cell_phenotype_with_and_immune_stimulation( Cell* pCell, Phenotype& phenotype, double dt )
{
	static int cycle_start_index = live.find_phase_index( PhysiCell_constants::live ); 
	static int cycle_end_index = live.find_phase_index( PhysiCell_constants::live ); 
	static int oncoprotein_i = pCell->custom_data.find_variable_index( "oncoprotein" ); 
	
	// update secretion rates based on hypoxia 
	
	static int o2_index = microenvironment.find_density_index( "oxygen" ); 
	static int immune_factor_index = microenvironment.find_density_index( "immunostimulatory factor" ); 
	double o2 = pCell->nearest_density_vector()[o2_index];	

	phenotype.secretion.secretion_rates[immune_factor_index] = 10.0; 
	
	update_cell_and_death_parameters_O2_based(pCell,phenotype,dt);
	
	// if cell is dead, don't bother with future phenotype changes. 
	// set it to secrete the immunostimulatory factor 
	if( phenotype.death.dead == true )
	{
		phenotype.secretion.secretion_rates[immune_factor_index] = 10; 
		pCell->functions.update_phenotype = NULL; 		
		return; 
	}

	// multiply proliferation rate by the oncoprotein 
	phenotype.cycle.data.transition_rate( cycle_start_index ,cycle_end_index ) *= pCell->custom_data[oncoprotein_i] ; 
	
	return; 
}

std::vector<std::string> cancer_immune_coloring_function( Cell* pCell )
{
	static int oncoprotein_i = pCell->custom_data.find_variable_index( "oncoprotein" ); 
	
	// immune are black
	std::vector< std::string > output( 4, "black" ); 
	
	if( pCell->type == 1 )
	{ 
		output[0] = "lime";
		output[1] = "lime";
		output[2] = "green"; 
		return output;
	} 

	// if I'm under attack, color me 
	if( pCell->state.attached_cells.size() > 0 )
	{
		output[0] = "darkcyan"; // orangered // "purple"; // 128,0,128
		output[1] = "black"; // "magenta"; 
		output[2] = "cyan"; // "magenta"; //255,0,255
		return output; 
	}
	
	// live cells are green, but shaded by oncoprotein value 
	if( pCell->phenotype.death.dead == false )
	{
		int oncoprotein = (int) round( 0.5 * pCell->custom_data[oncoprotein_i] * 255.0 ); 
		char szTempString [128];
		sprintf( szTempString , "rgb(%u,%u,%u)", oncoprotein, oncoprotein, 255-oncoprotein );
		output[0].assign( szTempString );
		output[1].assign( szTempString );

		sprintf( szTempString , "rgb(%u,%u,%u)", (int)round(output[0][0]/2.0) , (int)round(output[0][1]/2.0) , (int)round(output[0][2]/2.0) );
		output[2].assign( szTempString );
		
		return output; 
	}

	// if not, dead colors 
	
	if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )  // Apoptotic - Red
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
	}
	
	// Necrotic - Brown
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
	}	
	
	return output; 
}

/*
void add_elastic_velocity( Cell* pActingOn, Cell* pAttachedTo , double elastic_constant )
{
	std::vector<double> displacement = pAttachedTo->position - pActingOn->position; 
	axpy( &(pActingOn->velocity) , elastic_constant , displacement ); 
	
	return; 
}

void extra_elastic_attachment_mechanics( Cell* pCell, Phenotype& phenotype, double dt )
{
	for( int i=0; i < pCell->state.attached_cells.size() ; i++ )
	{
		add_elastic_velocity( pCell, pCell->state.attached_cells[i], pCell->custom_data["elastic_coefficient"] ); 
	}

	return; 
}	

void attach_cells( Cell* pCell_1, Cell* pCell_2 )
{
	#pragma omp critical
	{
		
	bool already_attached = false; 
	for( int i=0 ; i < pCell_1->state.attached_cells.size() ; i++ )
	{
		if( pCell_1->state.attached_cells[i] == pCell_2 )
		{ already_attached = true; }
	}
	if( already_attached == false )
	{ pCell_1->state.attached_cells.push_back( pCell_2 ); }
	
	already_attached = false; 
	for( int i=0 ; i < pCell_2->state.attached_cells.size() ; i++ )
	{
		if( pCell_2->state.attached_cells[i] == pCell_1 )
		{ already_attached = true; }
	}
	if( already_attached == false )
	{ pCell_2->state.attached_cells.push_back( pCell_1 ); }

	}

	return; 
}

void dettach_cells( Cell* pCell_1 , Cell* pCell_2 )
{
	#pragma omp critical
	{
		bool found = false; 
		int i = 0; 
		while( !found && i < pCell_1->state.attached_cells.size() )
		{
			// if cell 2 is in cell 1's list, remove it
			if( pCell_1->state.attached_cells[i] == pCell_2 )
			{
				int n = pCell_1->state.attached_cells.size(); 
				// copy last entry to current position 
				pCell_1->state.attached_cells[i] = pCell_1->state.attached_cells[n-1]; 
				// shrink by one 
				pCell_1->state.attached_cells.pop_back(); 
				found = true; 
			}
			i++; 
		}
	
		found = false; 
		i = 0; 
		while( !found && i < pCell_2->state.attached_cells.size() )
		{
			// if cell 1 is in cell 2's list, remove it
			if( pCell_2->state.attached_cells[i] == pCell_1 )
			{
				int n = pCell_2->state.attached_cells.size(); 
				// copy last entry to current position 
				pCell_2->state.attached_cells[i] = pCell_2->state.attached_cells[n-1]; 
				// shrink by one 
				pCell_2->state.attached_cells.pop_back(); 
				found = true; 
			}
			i++; 
		}

	}
	
	return; 
}
*/

void immune_cell_motility( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if attached, biased motility towards director chemoattractant 
	// otherwise, biased motility towards cargo chemoattractant 
	
	static int immune_factor_index = microenvironment.find_density_index( "immunostimulatory factor" ); 

	// if not docked, attempt biased chemotaxis 
	if( pCell->state.attached_cells.size() == 0 )
	{
		phenotype.motility.is_motile = true; 
		
		phenotype.motility.migration_bias_direction = pCell->nearest_gradient(immune_factor_index);	
		normalize( &( phenotype.motility.migration_bias_direction ) );			
	}
	else
	{
		phenotype.motility.is_motile = false; 
	}
	
	return; 
}

Cell* immune_cell_check_neighbors_for_attachment( Cell* pAttacker , double dt )
{
	std::vector<Cell*> nearby = pAttacker->cells_in_my_container(); 
	int i = 0; 
	while( i < nearby.size() )
	{
		// don't try to kill yourself 
		if( nearby[i] != pAttacker )
		{
			if( immune_cell_attempt_attachment( pAttacker, nearby[i] , dt ) )
			{ return nearby[i]; }
		}
		i++; 
	}
	
	return NULL; 
}

bool immune_cell_attempt_attachment( Cell* pAttacker, Cell* pTarget , double dt )
{
	static int oncoprotein_i = pTarget->custom_data.find_variable_index( "oncoprotein" ); 
	static int attach_rate_i = pAttacker->custom_data.find_variable_index( "attachment_rate" ); 

	double oncoprotein_saturation = 
		pAttacker->custom_data["oncoprotein_saturation"];  
	double oncoprotein_threshold =  
		pAttacker->custom_data["oncoprotein_threshold"];   
	double oncoprotein_difference = oncoprotein_saturation - oncoprotein_threshold;
	
	double max_attachment_distance = 
		pAttacker->custom_data["max_attachment_distance"];   
	double min_attachment_distance = 
		pAttacker->custom_data["min_attachment_distance"];   
	double attachment_difference = max_attachment_distance - min_attachment_distance; 
	
	if( pTarget->custom_data[oncoprotein_i] > oncoprotein_threshold && pTarget->phenotype.death.dead == false )
	{
		std::vector<double> displacement = pTarget->position - pAttacker->position;
		double distance_scale = norm( displacement ); 
		if( distance_scale > max_attachment_distance )
		{ return false; } 
	
		double scale = pTarget->custom_data[oncoprotein_i];
		scale -= oncoprotein_threshold; 
		scale /= oncoprotein_difference;
		if( scale > 1.0 )
		{ scale = 1.0; } 
		
		distance_scale *= -1.0; 
		distance_scale += max_attachment_distance; 
		distance_scale /= attachment_difference; 
		if( distance_scale > 1.0 )
		{ distance_scale = 1.0; } 
		
		if( UniformRandom() < pAttacker->custom_data[attach_rate_i] * scale * dt * distance_scale )
		{
//			std::cout << "\t attach!" << " " << pTarget->custom_data[oncoprotein_i] << std::endl; 
			attach_cells( pAttacker, pTarget ); 
		}
		
		return true; 
	}
	
	return false; 
}

bool immune_cell_attempt_apoptosis( Cell* pAttacker, Cell* pTarget, double dt )
{
	static int oncoprotein_i = pTarget->custom_data.find_variable_index( "oncoprotein" ); 
	static int apoptosis_model_index = pTarget->phenotype.death.find_death_model_index( "apoptosis" );	
	static int kill_rate_index = pAttacker->custom_data.find_variable_index( "kill_rate" ); 
	
	double oncoprotein_saturation = 
		pAttacker->custom_data["oncoprotein_saturation"]; // 2.0; 
	double oncoprotein_threshold =  
		pAttacker->custom_data["oncoprotein_threshold"]; // 0.5; // 0.1; 
	double oncoprotein_difference = oncoprotein_saturation - oncoprotein_threshold;

	// new 
	if( pTarget->custom_data[oncoprotein_i] < oncoprotein_threshold )
	{ return false; }
	
	// new 
	double scale = pTarget->custom_data[oncoprotein_i];
	scale -= oncoprotein_threshold; 
	scale /= oncoprotein_difference;
	if( scale > 1.0 )
	{ scale = 1.0; } 
	
	if( UniformRandom() < pAttacker->custom_data[kill_rate_index] * scale * dt )
	{ 
//		std::cout << "\t\t kill!" << " " << pTarget->custom_data[oncoprotein_i] << std::endl; 
		return true; 
	}
	return false; 
}

bool immune_cell_trigger_apoptosis( Cell* pAttacker, Cell* pTarget )
{
	static int apoptosis_model_index = pTarget->phenotype.death.find_death_model_index( "apoptosis" );	
	
	// if the Target cell is already dead, don't bother!
	if( pTarget->phenotype.death.dead == true )
	{ return false; }

	pTarget->start_death( apoptosis_model_index );
	return true; 
}

void immune_cell_rule( Cell* pCell, Phenotype& phenotype, double dt )
{
	static int attach_lifetime_i = pCell->custom_data.find_variable_index( "attachment_lifetime" ); 
	
	if( phenotype.death.dead == true )
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
		// attempt to kill my attached cell
		
		bool detach_me = false; 
		
		if( immune_cell_attempt_apoptosis( pCell, pCell->state.attached_cells[0], dt ) )
		{
			immune_cell_trigger_apoptosis( pCell, pCell->state.attached_cells[0] ); 
			detach_me = true; 
		}
		
		// decide whether to detach 
		
		if( UniformRandom() < dt / ( pCell->custom_data[attach_lifetime_i] + 1e-15 ) )
		{ detach_me = true; }
		
		// if I dettach, resume motile behavior 
		
		if( detach_me )
		{
			detach_cells( pCell, pCell->state.attached_cells[0] ); 
			phenotype.motility.is_motile = true; 
		}
		return; 
	}
	
	// I'm not docked, look for cells nearby and try to docked
	
	// if this returns non-NULL, we're now attached to a cell 
	if( immune_cell_check_neighbors_for_attachment( pCell , dt) )
	{
		// set motility off 
		phenotype.motility.is_motile = false; 
		return; 
	}
	phenotype.motility.is_motile = true; 
	
	return; 
}

void adhesion_contact_function( Cell* pActingOn, Phenotype& pao, Cell* pAttachedTo, Phenotype& pat , double dt )
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

