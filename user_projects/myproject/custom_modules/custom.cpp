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


	Cell_Definition* pCD = find_cell_definition( "cancer cell");
	pCD->functions.update_phenotype = cancer_cell_phenotype; 

	pCD = find_cell_definition( "immune cell"); 
	pCD->functions.update_phenotype = NULL; 
	pCD->functions.custom_cell_rule = immune_cell_rule; 

	pCD->phenotype.mechanics.relative_maximum_attachment_distance 
		= pCD->custom_data["max_attachment_distance"] / pCD->phenotype.geometry.radius ; 
		
	pCD->phenotype.mechanics.attachment_elastic_constant 
		= pCD->custom_data["elastic_coefficient"]; 		
	
	pCD->phenotype.mechanics.relative_detachment_distance 
		= pCD->custom_data["max_attachment_distance" ] / pCD->phenotype.geometry.radius ; 		
	
	// set functions 

/*	
	pCD->functions.update_phenotype = NULL; 
	pCD->functions.custom_cell_rule = immune_cell_rule; 
	pCD->functions.update_migration_bias = immune_cell_motility;
	pCD->functions.contact_function = adhesion_contact_function; 
*/


	
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
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	double op = get_single_signal( pCell , "custom:oncoprotein"); 
	
	// immune are black
	std::vector< std::string > output( 4, "black" ); 

	if( pCell->type_name == "immune cell" )
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
	if( get_single_signal( pCell , "dead") < 0.5 )
	{
		int oncoprotein = (int) round( 0.5 * op * 255.0 ); 
		char szTempString [128];
		sprintf( szTempString , "rgb(%u,%u,%u)", oncoprotein, oncoprotein, 255-oncoprotein );
		output[0].assign( szTempString );
		output[1].assign( szTempString );

		sprintf( szTempString , "rgb(%u,%u,%u)", (int)round(output[0][0]/2.0) , (int)round(output[0][1]/2.0) , (int)round(output[0][2]/2.0) );
		output[2].assign( szTempString );
		
		return output; 
	}

	// if not, dead colors 
	
	if( get_single_signal( pCell , "apoptotic" ) > 0.5 )  // Apoptotic - Red
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
	}
	
	// Necrotic - Brown
	if( get_single_signal( pCell , "necrotic" ) > 0.5 ) 
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
	}	
	
	return output; 
//	return paint_by_number_cell_coloring(pCell); 
}

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ return; } 

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 

// cancer cell functions 

void cancer_cell_phenotype( Cell* pCell, Phenotype& phenotype , double dt )
{
	double oncoprotein = get_single_signal( pCell , "custom:oncoprotein"); 
	
	// update secretion rates based on hypoxia 
	
	double o2 = get_single_signal( pCell , "oxygen" ); 
	
	update_cell_and_death_parameters_O2_based(pCell,phenotype,dt);
	
	// if cell is dead, don't bother with future phenotype changes. 
	// set it to secrete the immunostimulatory factor 
	if( get_single_signal( pCell , "dead" ) > 0.5 )
	{
		set_single_behavior( pCell , "immunostimulatory factor secretion" , 10 ); 
		pCell->functions.update_phenotype = NULL; 		
		return; 
	}

	// multiply proliferation rate by the oncoprotein 
	double rate = get_single_behavior( pCell , "cycle entry"); 
	rate *= get_single_signal( pCell , "custom:oncoprotein"); 
	set_single_behavior( pCell , "cycle entry" , rate); 

	return; 
}

void immune_cell_rule( Cell* pCell, Phenotype& phenotype , double dt )
{
	if( get_single_signal( pCell , "dead" ) > 0.5  )
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
		
		if( UniformRandom() < dt / ( get_single_signal(pCell,"custom:attachment lifetime") + 1e-15 ) )
		{ detach_me = true; }
		
		// if I dettach, resume motile behavior 
		
		if( detach_me )
		{
			detach_cells( pCell, pCell->state.attached_cells[0] ); 
			double speed_0 = get_single_base_behavior( pCell , "migration speed"); 
			set_single_behavior( pCell , "migration speed" , speed_0 ); 
		}
		return; 
	}
	
	// I'm not docked, look for cells nearby and try to docked
	
	// if this returns non-NULL, we're now attached to a cell 
	if( immune_cell_check_neighbors_for_attachment( pCell , dt) )
	{
		// set motility off 
		set_single_behavior( pCell , "migration speed" , 0.0 ); 
		return; 
	}
	double speed_0 = get_single_base_behavior( pCell , "migration speed"); 
	set_single_behavior( pCell , "migration speed" , speed_0 ); 	
	
	return; 
}

bool immune_cell_attempt_apoptosis( Cell* pAttacker, Cell* pTarget, double dt )
{
	double oncoprotein_saturation = get_single_signal( pCell , "custom:oncoprotein_saturation"); 
	double oncoprotein_threshold =  get_single_signal( pCell , "custom:oncoprotein_threshold"); 
	double oncoprotein_difference = oncoprotein_saturation - oncoprotein_threshold;

	// new 
	if( get_single_signal( pTarget , "custom:oncoprotein")  < oncoprotein_threshold )
	{ return false; }
	
	// new 
	double scale = get_single_signal( pTarget , "custom:oncoprotein");
	scale -= oncoprotein_threshold; 
	scale /= oncoprotein_difference;
	if( scale > 1.0 )
	{ scale = 1.0; } 
	
	if( UniformRandom() < get_single_signal(pAttacker, "custom:kill_rate") * scale * dt )
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
