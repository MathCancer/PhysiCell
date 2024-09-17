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
       Cell rule definitions 
	*/

	setup_cell_rules(); 


	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 

/*
	Cell_Definition* pCD = find_cell_definition( "cancer"); 
	pCD->phenotype.mechanics.maximum_number_of_attachments = 6; 
	pCD->phenotype.mechanics.attachment_elastic_constant = 0.002; // 0.00142; // 0.1; // 0.00142; // 0.1;  
	pCD->phenotype.mechanics.attachment_rate = 1;  
	pCD->phenotype.mechanics.detachment_rate = 0.01; // 0.01
	pCD->functions.update_phenotype = cancer_phenotype_function; 
	
	pCD = find_cell_definition( "BM"); 
	pCD->phenotype.mechanics.maximum_number_of_attachments = 6; 
	pCD->phenotype.mechanics.attachment_elastic_constant = 1; // 0.0142; // 100; // 0.0142 ; // 1;  
	pCD->phenotype.mechanics.attachment_rate = 1;  
	pCD->phenotype.mechanics.detachment_rate = 0; 
*/

	Cell_Definition* pCD = find_cell_definition( "cancer"); 
	pCD->phenotype.mechanics.maximum_number_of_attachments = 6; 
	pCD->phenotype.mechanics.attachment_elastic_constant = 0.00142; // 0.00142; // 0.1 for confluent version; // 0.002
	pCD->phenotype.mechanics.attachment_rate = 1;  
	pCD->phenotype.mechanics.detachment_rate = 0.01; // 0.01
	pCD->functions.update_phenotype = cancer_phenotype_function; 
	
	pCD = find_cell_definition( "BM"); 
	pCD->phenotype.mechanics.maximum_number_of_attachments = 6; 
	pCD->phenotype.mechanics.attachment_elastic_constant = 0.05; // 0.0142; // 1;   
	pCD->phenotype.mechanics.attachment_rate = 1;  
	pCD->phenotype.mechanics.detachment_rate = 0; 
	
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

	for( int n=0; n < (*all_cells).size() ; n++ )
	{
		Cell* pC = (*all_cells)[n]; 
		if( fabs( pC->position[0]) > 450 )
		{ set_single_behavior( pC, "is movable" , 0); }

	}
	
	return; 
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{ 
	std::vector<std::string> out = { "black" , "black" , "black" , "black"};

	int n_springs = pCell->state.spring_attachments.size(); 
	if( pCell->type_name != "BM" )
	{
		if( n_springs == 0 )
		{ out[0] = "grey"; }
		if( n_springs == 1 )
		{ out[0] = "indigo"; }
		if( n_springs == 2 )
		{ out[0] = "blue"; }
		if( n_springs == 3 )
		{ out[0] = "green"; }
		if( n_springs == 4 )
		{ out[0] = "yellow"; }
		if( n_springs == 5 )
		{ out[0] = "orange"; }
		if( n_springs == 6 )
		{ out[0] = "red"; }
		if( n_springs > 6 )
		{ out[0] = "magenta"; }
	}

	out[2] = out[0];
	out[3] = out[0];

	// std::vector<std::string> out = paint_by_number_cell_coloring(pCell); 
	return out; 
}

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	 return;	 
}

void cancer_phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	if( get_single_signal(pCell,"dead") > 0.5 )
	{ return; }

	double b = get_single_base_behavior( pCell , "cycle entry" ); 
	if( get_single_signal( pCell , "pressure") > 0.75 )
	{ b = 0; }
	set_single_behavior( pCell , "cycle entry" , b ); 	

	if( get_single_signal(pCell,"time") > 10000 )
	{
		set_single_behavior(pCell,"apoptosis",9e99); 
		set_single_behavior(pCell,"cell detachment rate",9e9); 
	}
	return;	 
}


void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ return; } 

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 