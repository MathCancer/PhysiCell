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
	
	cell_defaults.functions.update_phenotype = NULL; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 
	
	cell_defaults.phenotype.mechanics.attachment_elastic_constant = 
		parameters.doubles("attachment_elastic_constant"); 
		
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
	
	// set the initial value of all the cells 
	for( int n=0; n < (*all_cells).size(); n++ )
	{
		Cell* pC = (*all_cells)[n]; 
		pC->custom_data["head"] = UniformRandom(); 
		pC->custom_data["head_initial"] = pC->custom_data["head"];
	}
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	if( pCell->state.number_of_attached_cells() == 0 )
	{ return { "grey", "black", "grey", "grey"}; }

	if( pCell->state.number_of_attached_cells() == 1 )
	{
		if( pCell->custom_data["head"] > pCell->state.attached_cells[0]->custom_data["head"] )
		{ return { "red", "black", "red", "red"};  } 
		
		return { "orange", "black", "orange", "orange"}; 
	}

	if( pCell->state.number_of_attached_cells() >= 2 )
	{
		// shaed by head protein value 
		int intensity = (int) floor( 255.0 * pCell->custom_data["head"] ); 
		std::string strColor = std::to_string(intensity); 
		std::string color = "rgb(" + strColor + "," + strColor + ",255)"; 
		
		if( pCell->state.number_of_attached_cells() > 2 )
		{ return { "yellow", "black" , color, color }; }

		return { color , "black", color, color}; 	
	}

	return { "yellow", "black", "yellow", "yellow" }; 
}

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{
	// bookkeeping 
	
	static int nSignal = microenvironment.find_density_index("signal");
	
	// look for cells to form attachments, if 0 attachments
	int number_of_attachments = pCell->state.number_of_attached_cells(); 
	std::vector<Cell*> nearby = pCell->nearby_interacting_cells(); 
	
	if( number_of_attachments == 0 )
	{
		int n = 0; 
		while( number_of_attachments < (int) pCell->custom_data["max_attachments"] && n < nearby.size() )
		{
			if( nearby[n]->state.number_of_attached_cells() < nearby[n]->custom_data["max_attachments"] )
			{
				attach_cells( nearby[n] , pCell ); 
				number_of_attachments++;
			}
			n++; 
		}
	}

	// if no attachments, use chemotaxis 
	if( number_of_attachments == 0 )
	{ pCell->functions.update_migration_bias = chemotaxis_function; } 
	
	// if 1 attachment, do some logic  
	if( number_of_attachments == 1 )
	{
		// constant expression in end cells 
		pCell->custom_data["head"] = pCell->custom_data["head_initial"];

		// am I the head? 
		bool head = false; 
		if( pCell->custom_data["head"] > pCell->state.attached_cells[0]->custom_data["head"] )
		{ head = true; } 
		
		if( head )
		{ pCell->functions.update_migration_bias = head_migration_direction; }
		else
		{ pCell->functions.update_migration_bias = tail_migration_direction; }
		phenotype.secretion.secretion_rates[nSignal] = 100; 
	} 
	
	// if 2 or more attachments, use middle 
	if( number_of_attachments > 1 )
	{
		pCell->functions.update_migration_bias = middle_migration_direction;
		phenotype.secretion.secretion_rates[nSignal] = 1; 
	} 
	
	return; 
} 

void contact_function( Cell* pMe, Phenotype& phenoMe, 
	Cell* pOther, Phenotype& phenoOther, double dt )
{
	// spring-like adhesion 
	standard_elastic_contact_function(pMe,phenoMe,pOther,phenoOther,dt);

	// juxtacrine 
	if( pMe->state.number_of_attached_cells() > 0 )
	{
		double head_me = pMe->custom_data["head"];
		double head_other = pOther->custom_data["head"]; 
		
		// avoid double-counting transfer: 
		// Only do the high -> low transfers
		// One cell of each pair will satisfy this. 
			
		// make the transfer 
		if( head_me > head_other )
		{
			double amount_to_transfer = dt * pMe->custom_data["transfer_rate"] 
				* (head_me - head_other ); 
			pMe->custom_data["head"] -= amount_to_transfer; 
			#pragma omp critical
			{ pOther->custom_data["head"] += amount_to_transfer; }
		}	
	}

}

void head_migration_direction( Cell* pCell, Phenotype& phenotype, double dt )
{
	phenotype.motility.chemotaxis_direction = parameters.doubles("head_migration_direction"); 
	
	phenotype.motility.migration_speed = parameters.doubles("head_migration_speed"); 
	phenotype.motility.migration_bias = parameters.doubles("head_migration_bias");
	phenotype.motility.persistence_time =parameters.doubles("head_migration_persistence"); 
	
	// use this for fun rotational paths 
	/*
	double r = norm( pCell->position ) + 1e-16; 
	phenotype.motility.migration_bias_direction[0] = - pCell->position[1] / r; 
	phenotype.motility.migration_bias_direction[1] = pCell->position[0] / r; 

	normalize( &(phenotype.motility.migration_bias_direction) ); 
	return; 
	*/
	
	return chemotaxis_function( pCell,phenotype,dt); 
}

void tail_migration_direction( Cell* pCell, Phenotype& phenotype, double dt )
{
	phenotype.motility.chemotaxis_direction = parameters.doubles("tail_migration_direction"); 
	
	phenotype.motility.migration_speed = parameters.doubles("tail_migration_speed");  0; 
	phenotype.motility.migration_bias = parameters.doubles("tail_migration_bias");0.5; 
	phenotype.motility.persistence_time = parameters.doubles("tail_migration_persistence"); 100; 

	return chemotaxis_function( pCell,phenotype,dt); 
}

void middle_migration_direction( Cell* pCell, Phenotype& phenotype , double dt )
{
	// get velocity from "Upstream" 
	Cell* pUpstream = pCell->state.attached_cells[0]; 

	if( pCell->state.attached_cells[1]->custom_data["head"] > 
		pCell->state.attached_cells[0]->custom_data["head"] )
	{ pUpstream = pCell->state.attached_cells[1]; }
	
	phenotype.motility.migration_speed = parameters.doubles("middle_migration_speed"); 
	phenotype.motility.migration_bias_direction = 
		pUpstream->phenotype.motility.migration_bias_direction;
		
	normalize( &(phenotype.motility.migration_bias_direction) ); 
		
	return; 
}


