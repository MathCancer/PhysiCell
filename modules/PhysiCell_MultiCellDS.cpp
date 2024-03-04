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
 
#include "PhysiCell_MultiCellDS.h"

namespace PhysiCell{

void add_PhysiCell_cell_to_open_xml_pugi(  pugi::xml_document& xml_dom, Cell& C ); // not implemented -- future edition 

void add_PhysiCell_cells_to_open_xml_pugi( pugi::xml_document& xml_dom, std::string filename_base, Microenvironment& M  )
{
	static double temp_zero = 0.0; 
	
	if( BioFVM::save_cell_data == false )
	{ return; }
	
	pugi::xml_node root = xml_dom.child("MultiCellDS") ; 
	pugi::xml_node node = root.child( "cellular_information" ); 
	root = node; 
	
	// Let's reduce memory allocations and sprintf calls. 
	// This reduces execution time by around 30%. (e.g., write time for 1,000,000 cells decreases from 
	// 45 to 30 seconds on an older machine. 
	static char* temp; 
	static bool initialized = false; 
	
	static char rate_chars [1024]; 
	static char volume_chars [1024]; 
	static char diffusion_chars [1024]; 
	if( !initialized )
	{ 
		temp = new char [1024]; 
		initialized = true; 
		
		sprintf( rate_chars, "1/%s" , M.time_units.c_str() ); 
		sprintf( volume_chars, "%s^3" , M.spatial_units.c_str() ); 
		sprintf( diffusion_chars , "%s^2/%s", M.spatial_units.c_str() , M.time_units.c_str() ); 
	}
	
	node = node.child( "cell_populations" ); 
	if( !node )
	{
		node = root.append_child( "cell_populations" ); 
	}
	root = node; // root = cell_populations 
	
	// if we are using the customized matlab data, do it here. 
	if( BioFVM::save_cells_as_custom_matlab == true || 1 == 1 )
	{
		node = node.child( "cell_population" ); 
		if( !node )
		{
			node = root.append_child( "cell_population" ); 
			pugi::xml_attribute attrib = node.append_attribute( "type" ); 
			attrib.set_value( "individual" ); 
		}
		
		if( !node.child( "custom" ) ) 
		{
			node.append_child( "custom" ); 
		}
		node = node.child( "custom" ); 
		
		// look for a node called simplified_data, with source = PhysiCell 
		
		pugi::xml_node node_temp = node.child( "simplified_data" ); 
		bool temp_search_done = false;
		while( !temp_search_done && node_temp )
		{
			if( node_temp )
			{
				pugi::xml_attribute attribute_temp = node_temp.attribute( "source" ); 
				if( attribute_temp )
				{
					if( strcmp( attribute_temp.value() , "PhysiCell" ) == 0 )
					{
						temp_search_done = true; 
					}
					else
					{
						node_temp = node_temp.next_sibling(); 
					}
				}
			}
			else
			{
				node_temp = (pugi::xml_node) NULL; 
			}
		}
		
		if( !node_temp )
		{
			node_temp = node.append_child( "simplified_data" ); 
			pugi::xml_attribute attrib = node_temp.append_attribute( "type" ); 
			attrib.set_value( "matlab" ) ; 
			
			attrib = node_temp.append_attribute( "source" ); 
			attrib.set_value("PhysiCell"); 
			
			int index = 0; 
			int size = 1; 
			
			pugi::xml_node node_temp1 = node_temp.append_child( "labels" ); 
			
			// ID,x,y,z,total volume
			node_temp1 = node_temp1.append_child( "label" ); 
			node_temp1.append_child( pugi::node_pcdata ).set_value( "ID" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 

			size = 3; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "position" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 

			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "total_volume" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			// type, cycle model, current phase, elapsed time in phase, 
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "cell_type" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "cycle_model" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "current_phase" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "elapsed_time_in_phase" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			
			
			// nuclear volume, cytoplasmic volume, fluid fraction, calcified fraction, 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "nuclear_volume" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "cytoplasmic_volume" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "fluid_fraction" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 

			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "calcified_fraction" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			// orientation, polarity 			

			size = 3; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "orientation" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "polarity" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			// motility 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "migration_speed" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 

			size = 3; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "motility_vector" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "migration_bias" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 3; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "motility_bias_direction" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 
			
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "persistence_time" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "motility_reserved" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
			int number_of_cell_defs = cell_definition_indices_by_name.size(); 
			int number_of_substrates = microenvironment.number_of_densities(); 

			// new in 2022: chemotactic sensitivies
			size = number_of_substrates; // number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "chemotactic_sensitivities" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			// new in 2022: adhesive affinities 
			size = number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "adhesive_affinities" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 		


			// new in 2022: interactions : 
			// 	// phagocytosis parameters (e.g., macrophages)

			// dead_phagocytosis_rate 
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "dead_phagocytosis_rate" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			


			// live_phagocytosis_rates
			size = number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "live_phagocytosis_rates" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			// attack_rates
			size = number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "attack_rates" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			
			
			// damage_rate
			size = 1; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "damage_rate" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			
			
			// fusion_rates
			size = number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "fusion_rates" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			// new in 2022: transformations : 
			size = number_of_cell_defs; 
			node_temp1 = node_temp1.append_child( "label" );
			node_temp1.append_child( pugi::node_pcdata ).set_value( "transformation_rates" ); 
			attrib = node_temp1.append_attribute( "index" ); 
			attrib.set_value( index ); 
			attrib = node_temp1.append_attribute( "size" ); 
			attrib.set_value( size ); 
			node_temp1 = node_temp1.parent(); 
			index += size; 			

			// custom variables 
			for( int i=0; i < (*all_cells)[0]->custom_data.variables.size(); i++ )
			{
				size = 1; 
				char szTemp [1024]; 
				strcpy( szTemp, (*all_cells)[0]->custom_data.variables[i].name.c_str() ); 
				node_temp1 = node_temp1.append_child( "label" );
				node_temp1.append_child( pugi::node_pcdata ).set_value( szTemp ); 
				attrib = node_temp1.append_attribute( "index" ); 
				attrib.set_value( index ); 
				attrib = node_temp1.append_attribute( "size" ); 
				attrib.set_value( size ); 
				node_temp1 = node_temp1.parent(); 
				index += size; 			
			}
			// custom vector variables 
			for( int i=0; i < (*all_cells)[0]->custom_data.vector_variables.size(); i++ )
			{
				size = (*all_cells)[0]->custom_data.vector_variables[i].value.size(); 
;				char szTemp [1024]; 
				strcpy( szTemp, (*all_cells)[0]->custom_data.vector_variables[i].name.c_str() ); 
				node_temp1 = node_temp1.append_child( "label" );
				node_temp1.append_child( pugi::node_pcdata ).set_value( szTemp ); 
				attrib = node_temp1.append_attribute( "index" ); 
				attrib.set_value( index ); 
				attrib = node_temp1.append_attribute( "size" ); 
				attrib.set_value( size ); 
				node_temp1 = node_temp1.parent(); 
				index += size; 			
			}
			
		}
		node = node_temp; 
		
		if( !node.child( "filename" ) )
		{
			node.append_child( "filename" ); 
		}
		node = node.child( "filename" ); 
		
		// next, filename 
		char filename [1024]; 
		sprintf( filename , "%s_cells_physicell.mat" , filename_base.c_str() ); 
		
		/* store filename without the relative pathing (if any) */ 
		char filename_without_pathing [1024];
		char* filename_start = strrchr( filename , '/' ); 
		if( filename_start == NULL )
		{ filename_start = filename; }
		else	
		{ filename_start++; } 
		strcpy( filename_without_pathing , filename_start );  
		
		if( !node.first_child() )
		{
			node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
		}
		else
		{
			node.first_child().set_value( filename_without_pathing ); // filename ); 
		}
		
		// next, create a matlab structure and save it!
		
		// order: ID,x,y,z,total volume, (same as BioFVM custom data, but instead of secretions ...)
		// type, cycle model, current phase, elapsed time in phase, 
		// nuclear volume, cytoplasmic volume, fluid fraction, calcified fraction, 
		// orientation, polarity 
		
		int number_of_data_entries = (*all_cells).size(); 
		int size_of_each_datum = 1 + 3 + 1  // ID, x,y,z, total_volume 
			+1+1+1+1 // cycle information 
			+1+1+1+1 // volume information 
			+3+1 // orientation, polarity; 
			+1+3+1+3+1+1; // motility 

		// figure out size of 2022 phenotype items 
		extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
		int number_of_substrates = microenvironment.number_of_densities(); 
		int number_of_cell_defs = cell_definition_indices_by_name.size(); 

		// advanced chemotaxis
		size_of_each_datum += 
			number_of_substrates; // number_of_cell_defs;		

		// cell adhesion affinities 
		size_of_each_datum += 
			number_of_cell_defs;		

		// cell interactions: 
		size_of_each_datum += 
			1+number_of_cell_defs+number_of_cell_defs+1+number_of_cell_defs;

		// cell transformations;  
		size_of_each_datum += number_of_cell_defs;

		// figure out size of custom data. for now, 
		// assume all the cells have teh same custom data as 
		// cell #0
		int custom_data_size = (*all_cells)[0]->custom_data.variables.size();  
		for( int i=0; i < (*all_cells)[0]->custom_data.vector_variables.size(); i++ )
		{
			custom_data_size += (*all_cells)[0]->custom_data.vector_variables[i].value.size(); 
		}
		size_of_each_datum += custom_data_size; 
		

		FILE* fp = write_matlab_header( size_of_each_datum, number_of_data_entries,  filename, "cells" );  
		if( fp == NULL )
		{ 
			std::cout << std::endl << "Error: Failed to open " << filename << " for MAT writing." << std::endl << std::endl; 
	
			std::cout << std::endl << "Error: We're not writing data like we expect. " << std::endl
			<< "Check to make sure your save directory exists. " << std::endl << std::endl
			<< "I'm going to exit with a crash code of -1 now until " << std::endl 
			<< "you fix your directory. Sorry!" << std::endl << std::endl; 
			exit(-1); 
		} 
		Cell* pCell; 
		
		// storing data as cols (each column is a cell)
		for( int i=0; i < number_of_data_entries ; i++ )
		{
			// ID, x,y,z, total_volume 
			double ID_temp = (double) (*all_cells)[i]->ID;
			fwrite( (char*) &( ID_temp ) , sizeof(double) , 1 , fp ); 
			
			pCell = (*all_cells)[i]; 

			fwrite( (char*) &( pCell->position[0] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->position[1] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->position[2] ) , sizeof(double) , 1 , fp ); 
			double dTemp = pCell->phenotype.volume.total; // get_total_volume();
			fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); 
			
			// type, cycle model, current phase, elapsed time in phase, 
			dTemp = (double) pCell->type; 
			fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp );  // cell type 
			
			dTemp = (double) pCell->phenotype.cycle.model().code; 
			fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); // cycle model 
			
			dTemp = (double) pCell->phenotype.cycle.current_phase().code; 
			fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); // current phase 
			
			// dTemp = pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time; 
			fwrite( (char*) &( pCell->phenotype.cycle.data.elapsed_time_in_phase ) , sizeof(double) , 1 , fp ); // elapsed time in phase 
			
			// volume information
			// nuclear volume, cytoplasmic volume, fluid fraction, calcified fraction, 
			fwrite( (char*) &( pCell->phenotype.volume.nuclear ) , sizeof(double) , 1 , fp );  // nuclear volume 

			fwrite( (char*) &( pCell->phenotype.volume.cytoplasmic ) , sizeof(double) , 1 , fp );  // cytoplasmic volume 
			
			fwrite( (char*) &( pCell->phenotype.volume.fluid_fraction ) , sizeof(double) , 1 , fp );  // fluid fraction 

			fwrite( (char*) &( pCell->phenotype.volume.calcified_fraction ) , sizeof(double) , 1 , fp );  // calcified fraction 
			
			// orientation, polarity; 
			fwrite( (char*) &( pCell->state.orientation[0] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->state.orientation[1] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->state.orientation[2] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->phenotype.geometry.polarity ) , sizeof(double) , 1 , fp ); 
			

			
			// motility information 
			fwrite( (char*) &( pCell->phenotype.motility.migration_speed ) , sizeof(double) , 1 , fp ); // speed
			fwrite( (char*) &( pCell->phenotype.motility.motility_vector[0] ) , sizeof(double) , 1 , fp ); // velocity 
			fwrite( (char*) &( pCell->phenotype.motility.motility_vector[1] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->phenotype.motility.motility_vector[2] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->phenotype.motility.migration_bias ) , sizeof(double) , 1 , fp );  // bias (0 to 1)
			fwrite( (char*) &( pCell->phenotype.motility.migration_bias_direction[0] ) , sizeof(double) , 1 , fp ); // bias direction 
			fwrite( (char*) &( pCell->phenotype.motility.migration_bias_direction[1] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->phenotype.motility.migration_bias_direction[2] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( pCell->phenotype.motility.persistence_time ) , sizeof(double) , 1 , fp ); // persistence 
			fwrite( (char*) &( temp_zero ) , sizeof(double) , 1 , fp ); // reserved for "time in this direction" 


			// new in 2022: interactions : 
			// fwrite( (char*) &( pCell->phenotype.motility.chemotactic_sensitivities ) , sizeof(double) , number_of_cell_defs , fp ); // chemotactic_sensitivities 
			fwrite( (char*) &( pCell->phenotype.motility.chemotactic_sensitivities ) , sizeof(double) , number_of_substrates , fp ); // chemotactic_sensitivities 
			fwrite( (char*) &( pCell->phenotype.mechanics.cell_adhesion_affinities ) , sizeof(double) , number_of_cell_defs , fp ); // cell_adhesion_affinities 
			fwrite( (char*) &( pCell->phenotype.cell_interactions.dead_phagocytosis_rate ) , sizeof(double) , 1 , fp ); // dead_phagocytosis_rate 
			fwrite( (char*) &( pCell->phenotype.cell_interactions.live_phagocytosis_rates ) , sizeof(double) , number_of_cell_defs , fp ); // live_phagocytosis_rates 
			fwrite( (char*) &( pCell->phenotype.cell_interactions.attack_rates ) , sizeof(double) , number_of_cell_defs , fp ); // attack_rates 
			fwrite( (char*) &( pCell->phenotype.cell_interactions.damage_rate ) , sizeof(double) , 1 , fp ); // damage_rate 
			fwrite( (char*) &( pCell->phenotype.cell_interactions.fusion_rates ) , sizeof(double) , number_of_cell_defs , fp ); // fusion_rates 
			fwrite( (char*) &( pCell->phenotype.cell_transformations.transformation_rates ) , sizeof(double) , number_of_cell_defs , fp ); // transformation_rates 
			
			// custom variables 
			for( int j=0 ; j < pCell->custom_data.variables.size(); j++ )
			{
				fwrite( (char*) &( pCell->custom_data.variables[j].value ) , sizeof(double) , 1 , fp );  
			}
			
			// custom vector variables 
			for( int j=0 ; j < pCell->custom_data.vector_variables.size(); j++ )
			{
				for( int k=0; k < pCell->custom_data.vector_variables[j].value.size(); k++ )
				{
					fwrite( (char*) &( pCell->custom_data.vector_variables[j].value[k] ) , sizeof(double) , 1 , fp );  
				}
			}
			
		}

		fclose( fp ); 
		
		
		return; 
	}
	
	// if there's a list, clear it out 
	node = node.child( "cell_population" ); 
	if( node )
	{
		node = node.parent(); 
		node.remove_child( node.child( "cell_population" ) ); 
	}
	node = root.append_child( "cell_population" ); 
	pugi::xml_attribute attrib = node.append_attribute( "type" ); 
	attrib.set_value( "individual" ); 

	// now go through all cells 

	root = node; 
	for( int i=0; i < (*all_cells).size(); i++ )
	{
		node = node.append_child( "cell" ); 
		attrib = node.append_attribute( "ID" ); 
		attrib.set_value(  (*all_cells)[i]->ID ); 
		
		node = node.append_child( "phenotype_dataset" ); 
		node = node.append_child( "phenotype" ); // add a type? 
		
		// add all the transport information 
		node = node.append_child( "transport_processes" ); 
		
		// add variables and their source/sink/saturation values (per-cell basis)
		for( int j=0; j < M.number_of_densities() ; j++ ) 
		{
			node = node.append_child( "variable" ); 
			attrib = node.append_attribute( "name" ); 
			attrib.set_value( M.density_names[j].c_str() ); 
			// ChEBI would go here later 
			attrib = node.append_attribute( "ID" );
			attrib.set_value( j ); 
			
			node = node.append_child( "export_rate" ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( rate_chars ); 
			// sprintf( temp , "%f" , all_basic_agents[i]->get_total_volume() * (*all_basic_agents[i]->secretion_rates)[j] ); 
			sprintf( temp , "%f" , (*all_cells)[i]->phenotype.volume.total * (*(*all_cells)[i]->secretion_rates)[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			node = node.parent( ); 
			
			node = node.append_child( "import_rate" ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( rate_chars ); 
			sprintf( temp,  "%f" , (*all_cells)[i]->phenotype.volume.total * (*(*all_cells)[i]->uptake_rates)[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			node = node.parent(); 
			
			node = node.append_child( "saturation_density" ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( M.density_units[j].c_str() ); 
			sprintf( temp, "%f" , (*(*all_cells)[i]->saturation_densities)[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			node = node.parent(); 
			
			node = node.parent(); // back up to transport processes 
		}
		
		node = node.parent(); // back up to phenotype 

		// add size information 
		node = node.append_child( "geometrical_properties" ); 
		node = node.append_child("volumes");
		node = node.append_child("total_volume"); 
		attrib = node.append_attribute("units"); 
		attrib.set_value( volume_chars ); 
		sprintf( temp,  "%f" , (*all_cells)[i]->phenotype.volume.total ); 
		node.append_child( pugi::node_pcdata ).set_value( temp ); 		
		node = node.parent(); 
		node = node.parent(); 
		
		node = node.parent(); // back up to geometrical_properties 
		
		node = node.parent(); // back up to phenotype 
		
		node = node.parent(); // back up to phenotype_dataset 
		
		// add position information 
		node = node.append_child( "state"); 
		node = node.append_child( "position" ); 
		attrib = node.append_attribute( "units" ); 
		attrib.set_value( M.spatial_units.c_str() ); 
		
		// vector3_to_list( all_basic_agents[i]->position , temp , ' ');
		sprintf( temp , "%.7e %.7e %.7e" , (*all_cells)[i]->position[0], (*all_cells)[i]->position[1], (*all_cells)[i]->position[2] ); 
		node.append_child( pugi::node_pcdata ).set_value( temp ); 		
		
		node = root; 
	}
	
	return; 
}

void add_PhysiCell_to_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, double current_simulation_time , Microenvironment& M );

void save_PhysiCell_to_MultiCellDS_xml_pugi( std::string filename_base , Microenvironment& M , double current_simulation_time)
{
	// start with a standard BioFVM save
	
	add_BioFVM_to_open_xml_pugi( BioFVM::biofvm_doc , filename_base , current_simulation_time , M ); 
	
	// now, add the PhysiCell data 

	add_PhysiCell_cells_to_open_xml_pugi( BioFVM::biofvm_doc , filename_base , M  ); 
	// add_PhysiCell_cells_to_open_xml_pugi_v2( BioFVM::biofvm_doc , filename_base , M  ); 
		
	// Lastly, save to the indicated filename 

	char filename[1024]; 
	sprintf( filename , "%s.xml" , filename_base.c_str() ); 
	BioFVM::biofvm_doc.save_file( filename );

	return; 
}


void save_PhysiCell_to_MultiCellDS_v2( std::string filename_base , Microenvironment& M , double current_simulation_time)
{
	// set some metadata

	BioFVM::MultiCellDS_version_string = "2"; 
	BioFVM::BioFVM_metadata.program.program_name = "PhysiCell"; 
	BioFVM::BioFVM_metadata.program.program_version = PhysiCell_Version; 
	BioFVM::BioFVM_metadata.program.program_URL = "http://physicell.org"; 

	BioFVM::BioFVM_metadata.program.creator.type = "creator"; 	
	BioFVM::BioFVM_metadata.program.creator.surname = "Macklin"; 	
	BioFVM::BioFVM_metadata.program.creator.given_names = "Paul"; 	
	BioFVM::BioFVM_metadata.program.creator.email = "macklinp@iu.edu"; 	
	BioFVM::BioFVM_metadata.program.creator.URL = "http://MathCancer.org"; 	
	BioFVM::BioFVM_metadata.program.creator.organization = "Indiana University & PhysiCell Project"; 	
	BioFVM::BioFVM_metadata.program.creator.department = "Intelligent Systems Engineering"; 	
	BioFVM::BioFVM_metadata.program.creator.ORCID = "0000-0002-9925-0151"; 	
	
	BioFVM::BioFVM_metadata.program.citation.DOI = "10.1371/journal.pcbi.1005991"; 
	BioFVM::BioFVM_metadata.program.citation.PMID = "29474446"; 
	BioFVM::BioFVM_metadata.program.citation.PMCID = "PMC5841829"; 
	BioFVM::BioFVM_metadata.program.citation.text = "A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin. PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: 10.1371/journal.pcbi.1005991"; 
	BioFVM::BioFVM_metadata.program.citation.notes = ""; 
	BioFVM::BioFVM_metadata.program.citation.URL = "https://dx.doi.org/PMC5841829"; 

	// start with a standard BioFVM save
		// overall XML structure 
	add_MultiCellDS_main_structure_to_open_xml_pugi( BioFVM::biofvm_doc ); 
		// save metadata 
	BioFVM_metadata.add_to_open_xml_pugi( current_simulation_time , BioFVM::biofvm_doc ); 
		// save diffusing substrates 
	add_BioFVM_substrates_to_open_xml_pugi( BioFVM::biofvm_doc , filename_base, M  ); 

		// add_BioFVM_agents_to_open_xml_pugi( xml_dom , filename_base, M); 
	
	// now, add the PhysiCell data 

	// add_PhysiCell_cells_to_open_xml_pugi( BioFVM::biofvm_doc , filename_base , M  ); 
	add_PhysiCell_cells_to_open_xml_pugi_v2( BioFVM::biofvm_doc , filename_base , M  ); 

	// Lastly, save to the indicated filename 

	char filename[1024]; 
	sprintf( filename , "%s.xml" , filename_base.c_str() ); 
	BioFVM::biofvm_doc.save_file( filename );

	return; 
}

void add_PhysiCell_cells_to_open_xml_pugi_v2( pugi::xml_document& xml_dom, std::string filename_base, Microenvironment& M  ) 
{
	// get number of substrates 
	static int m =  microenvironment.number_of_densities(); // number_of_substrates  
	// get number of cell types
	static int n = cell_definition_indices_by_name.size(); // number_of_cell_types
	// get number of death models 
	static int nd = (*all_cells)[0]->phenotype.death.rates.size(); // 
	// get number of custom data 
	static int nc = 0; // 
	static int nc_scalar = 0; 
	static int nc_vector = 0; 

	static int cell_data_size = 0; 


	static bool legends_done= false; 
	static std::vector<std::string> data_names; 
	static std::vector<std::string> data_units; 
	static std::vector<int> data_start_indices; 
	static std::vector<int> data_sizes; 

	// set up the labels 
	if( legends_done == false )
	{
		data_names.clear(); 
		data_sizes.clear(); 
		data_start_indices.clear(); 
		data_units.clear(); 

		std::string name; 
		std::string units; 
		int size; 
		int index = 0; 


// compatibilty : first 17 entries 
		// ID 					<label index="0" size="1">ID</label>
		name = "ID"; 
		size = 1; 
		units="none";
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 
		//					<label index="1" size="3">position</label>
		name = "position"; 
		size = 3; 
		units="microns";
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 
		//					<label index="4" size="1">total_volume</label>
		name = "total_volume"; 
		units = "cubic microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 
		//					<label index="5" size="1">cell_type</label>
		name = "cell_type"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="6" size="1">cycle_model</label>
		name = "cycle_model"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="7" size="1">current_phase</label>
		name = "current_phase"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="8" size="1">elapsed_time_in_phase</label>
		name = "elapsed_time_in_phase"; 
		units = "min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="9" size="1">nuclear_volume</label>
		name = "nuclear_volume"; 
		units = "cubic microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 				
		//					<label index="10" size="1">cytoplasmic_volume</label>
		name = "cytoplasmic_volume"; 
		units = "cubic microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="11" size="1">fluid_fraction</label>
		name = "fluid_fraction"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="12" size="1">calcified_fraction</label>
		name = "calcified_fraction"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="13" size="3">orientation</label>
		name = "orientation"; 
		units = "none"; 
		size = 3; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
		//					<label index="16" size="1">polarity</label>
		name = "polarity"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		

/* state variables to save */ 
// state 
	// velocity // 3 
		name = "velocity"; 
		units = "micron/min"; 		
		size = 3; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
	// pressure // 1
		name = "pressure"; 
		units = "none"; 		
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
	// number of nuclei // 1
		name = "number_of_nuclei"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
	// damage // 1
		name = "damage"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
	// total attack time // 1
		name = "total_attack_time"; 
		units = "min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 		
	// contact_with_basement_membrane // 1 
		name = "contact_with_basement_membrane"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 	


/* now go through phenotype and state */ 
// cycle 
  // cycle model // already above 
  // current phase // already above 
  // current exit rate // 1 
		name = "current_cycle_phase_exit_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 	  
  // elapsed time in phase // 1 
		name = "elapsed_time_in_phase"; 
		units = "min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size; 	  

// death 
  // live or dead state // 1 
		name = "dead"; 
		units = "none"; 		
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;   
  // current death model // 1
		name = "current_death_model"; // 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;   
  // death rates // nd 
		name = "death_rates"; 
		units = "1/min"; 
		size = nd; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;     
// 

// volume ()
  // cytoplasmic_biomass_change_rate // 1
		name = "cytoplasmic_biomass_change_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;    
  //  nuclear_biomass_change_rate;  // 1
		name = "nuclear_biomass_change_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;    
	 //  fluid_change_rate; // 1
		name = "fluid_change_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	 
	// calcification_rate; // 1
		name = "calcification_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// target_solid_cytoplasmic; // 1
		name = "target_solid_cytoplasmic"; 
		units = "cubic microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// target_solid_nuclear; // 1 
		name = "target_solid_nuclear"; 
		units = "cubic microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// target_fluid_fraction; // 1
		name = "target_fluid_fraction"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  

  // geometry 
     // radius //1 
		name = "radius"; 
		units = "microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	 
	 // nuclear_radius // 1
		name = "nuclear_radius"; 
		units = "microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	 
	 // surface_area //1
		name = "surface_area"; 
		units = "square microns"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	 // polarity // arleady done 

  // mechanics 
	// cell_cell_adhesion_strength; // 1
		name = "cell_cell_adhesion_strength"; 
		units = "micron/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  	
	// cell_BM_adhesion_strength; // 1
		name = "cell_BM_adhesion_strength"; 
		units = "micron/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  		
	// cell_cell_repulsion_strength; // 1
		name = "cell_cell_repulsion_strength"; 
		units = "micron/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  	
	// cell_BM_repulsion_strength; // 1
		name = "cell_BM_repulsion_strength"; 
		units = "micron/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// std::vector<double> cell_adhesion_affinities; // n 
		name = "cell_adhesion_affinities"; 
		units = "none"; 
		size = n; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// relative_maximum_adhesion_distance; // 1
		name = "relative_maximum_adhesion_distance"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// maximum_number_of_attachments; // 1
		name = "maximum_number_of_attachments"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// attachment_elastic_constant; // 1
		name = "attachment_elastic_constant"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// attachment_rate; // 1
		name = "attachment_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// detachment_rate; // 1 
 		name = "detachment_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
 // Motility
	// is_motile // 1
 		name = "is_motile"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	//  persistence_time; // 1
 		name = "persistence_time"; 
		units = "min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// migration_speed; // 1
 		name = "migration_speed"; 
		units = "micron/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// std::vector<double> migration_bias_direction; // 3 // motility_bias_direction originally 
 		name = "migration_bias_direction"; 
		units = "none"; 
		size = 3; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// migration_bias; //1
 		name = "migration_bias"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  	
	// std::vector<double> motility_vector; // 3
 		name = "motility_vector"; 
		units = "micron/min"; 
		size = 3; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  	
	// chemotaxis_index; // 1
 		name = "chemotaxis_index"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	// chemotaxis_direction; // 1 
 		name = "chemotaxis_direction"; 
		units = "none"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  	
	// advanced chemotaxis 
	// std::vector<double> chemotactic_sensitivities;  // m 
 		name = "chemotactic_sensitivities"; 
		units = "none"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	  
	
// secretion 
	// std::vector<double> secretion_rates; // m
 		name = "secretion_rates"; 
		units = "1/min"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// std::vector<double> uptake_rates; // m 
 		name = "uptake_rates"; 
		units = "1/min"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
	// std::vector<double> saturation_densities; // m 
 		name = "saturation_densities"; 
		units = "stuff/cubic micron"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
	// std::vector<double> net_export_rates; // m 
 		name = "net_export_rates"; 
		units = "stuff/min"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  

// molecular 
	// internalized_total_substrates // m 
 		name = "internalized_total_substrates"; 
		units = "stuff"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// 	fraction_released_at_death // m 
 		name = "fraction_released_at_death"; 
		units = "none"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// fraction_transferred_when_ingested //m 
 		name = "fraction_transferred_when_ingested"; 
		units = "none"; 
		size = m; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
// interactions 
	// dead_phagocytosis_rate; // 1 
 		name = "dead_phagocytosis_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
	// std::vector<double> live_phagocytosis_rates; // n 
 		name = "live_phagocytosis_rates"; 
		units = "1/min"; 
		size = n; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// std::vector<double> attack_rates; // n 
 		name = "attack_rates"; 
		units = "1/min"; 
		size = n; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// double damage_rate;  1 
 		name = "damage_rate"; 
		units = "1/min"; 
		size = 1; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  	
	// std::vector<double> fusion_rates; // n 
 		name = "fusion_rates"; 
		units = "1/min"; 
		size = n; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
// transformations 
	// std::vector<double> transformation_rates; // n 
  		name = "transformation_rates"; 
		units = "1/min"; 
		size = n; 
		data_names.push_back( name ); 
		data_units.push_back(units); 
		data_sizes.push_back( size ); 
		data_start_indices.push_back( index ); 
		cell_data_size += size; 
		index += size;  
 
// custom 
		for( int j=0 ; j < (*all_cells)[0]->custom_data.variables.size(); j++ )
		{
			name = (*all_cells)[0]->custom_data.variables[j].name; 
			units = (*all_cells)[0]->custom_data.variables[j].units; 
			size = 1; 
			data_names.push_back( name ); 
			data_units.push_back(units); 
			data_sizes.push_back( size ); 
			data_start_indices.push_back( index ); 
			cell_data_size += size; 
			index += size;  
		}
		
		// custom vector variables 
		for( int j=0 ; j < (*all_cells)[0]->custom_data.vector_variables.size(); j++ )
		{
			name = (*all_cells)[0]->custom_data.vector_variables[j].name; 
			units = (*all_cells)[0]->custom_data.vector_variables[j].units; 
			size = (*all_cells)[0]->custom_data.vector_variables[j].value.size(); 
			data_names.push_back( name ); 
			data_units.push_back(units); 
			data_sizes.push_back( size ); 
			data_start_indices.push_back( index ); 
			cell_data_size += size; 
			index += size; 
		}

		legends_done = true; 
	}

	// get ready for XML navigation 
	// pugi::xml_document& xml_dom = BioFVM::biofvm_doc; 

	pugi::xml_node root = xml_dom.child("MultiCellDS") ; 
	pugi::xml_node node = root.child( "cellular_information" ); // root = cellular_information
	root = node; 	

	// Let's reduce memory allocations and sprintf calls. 
	// This reduces execution time. 
	static char* temp; 
	static bool initialized = false; 
	
	static char rate_chars [1024]; 
	static char volume_chars [1024]; 
	static char diffusion_chars [1024]; 
	if( !initialized )
	{ 
		temp = new char [1024]; 
		initialized = true; 
		
		sprintf( rate_chars, "1/%s" , M.time_units.c_str() ); 
		sprintf( volume_chars, "%s^3" , M.spatial_units.c_str() ); 
		sprintf( diffusion_chars , "%s^2/%s", M.spatial_units.c_str() , M.time_units.c_str() ); 
	}

	node = node.child( "cell_populations" ); 
	if( !node )
	{
		node = root.append_child( "cell_populations" ); 
	}
	root = node; // root = cellular_information.cell_populations 

	node = root.child( "cell_population" ); 
	if( !node )
	{
		node = root.append_child( "cell_population" ); 
		pugi::xml_attribute attrib = node.append_attribute( "type" ); 
		attrib.set_value( "individual" ); 
	}
	root = node; // root = cellular_information.cell_populations.cell_population  

	node = root.child( "custom" ); 
	if( !node )
	{
		node = root.append_child( "custom" ); 
	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom 

	node = root.child( "simplified_data" ); 
	if( !node )
	{
		node = root.append_child( "simplified_data" ); 
		pugi::xml_attribute attrib = node.append_attribute( "type" ); 
		attrib.set_value( "matlab" ); 

		attrib = node.append_attribute( "source" ); 
		attrib.set_value( "PhysiCell" ); 

		attrib = node.append_attribute( "data_version" ); 
		attrib.set_value( "2" ); 
	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom.simplified_data 

	// write legend 

	node = root.child( "labels" ); 
	if( !node )
	{
		node = root.append_child( "labels" ); 
		root = node; // root = cellular_information.cell_populations.cell_population.custom.simplified_data.labels  

		for( int i=0; i < data_names.size(); i++ )
		{
			node = root.append_child( "label" ); 	
			pugi::xml_attribute attrib = node.append_attribute( "index" ); 
			attrib.set_value( data_start_indices[i] ); 	

			attrib = node.append_attribute( "size" ); 
			attrib.set_value( data_sizes[i] ); 				

			attrib = node.append_attribute( "units" ); 
			attrib.set_value( data_units[i].c_str() ); 	

			node.append_child( pugi::node_pcdata ).set_value( data_names[i].c_str() ); 
		}
		root = root.parent(); // root = cellular_information.cell_populations.cell_population.custom.simplified_data  
	}

	// write data 
	node = root.child( "filename" ); 
	if( !node )
	{
		node = root.append_child( "filename" ); 
	}

	// write the filename 

	// next, filename 
	char filename [1024]; 
	sprintf( filename , "%s_cells.mat" , filename_base.c_str() ); 
	
	/* store filename without the relative pathing (if any) */ 
	char filename_without_pathing [1024];
	char* filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	
	if( !node.first_child() )
	{
		node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
	}
	else
	{
		node.first_child().set_value( filename_without_pathing ); // filename ); 
	}

	// now write the actual data 

	int size_of_each_datum = cell_data_size;
	int number_of_data_entries = (*all_cells).size();  

	FILE* fp = write_matlab_header( size_of_each_datum, number_of_data_entries,  filename, "cells" );  
	if( fp == NULL )
	{ 
		std::cout << std::endl << "Error: Failed to open " << filename << " for MAT writing." << std::endl << std::endl; 

		std::cout << std::endl << "Error: We're not writing data like we expect. " << std::endl
		<< "Check to make sure your save directory exists. " << std::endl << std::endl
		<< "I'm going to exit with a crash code of -1 now until " << std::endl 
		<< "you fix your directory. Sorry!" << std::endl << std::endl; 
		exit(-1); 
	} 


	Cell* pCell; 
	
	double dTemp; 
	// storing data as cols (each column is a cell)
	for( int i=0; i < number_of_data_entries ; i++ )
	{
		pCell = (*all_cells)[i]; 

		int writes = 0; 

		// compatibilty : first 17 entries 
		// ID 					<label index="0" size="1">ID</label>
		// double ID_temp = (double) (*all_cells)[i]->ID;
		// fwrite( (char*) &( ID_temp ) , sizeof(double) , 1 , fp ); 

		// name = "ID"; 
		dTemp = (double) pCell->ID;
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "position";    NOTE very different syntax for writing vectors!
        std::fwrite( pCell->position.data() , sizeof(double) , 3 , fp );
		// name = "total_volume"; 
		std::fwrite( &( pCell->phenotype.volume.total ) , sizeof(double) , 1 , fp ); 
		// name = "cell_type"; 
		dTemp = (double) pCell->type;
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "cycle_model"; 
		dTemp = (double) pCell->phenotype.cycle.model().code; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); // cycle model 
		// name = "current_phase"; 
		dTemp = (double) pCell->phenotype.cycle.current_phase().code; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); // cycle model 
		// name = "elapsed_time_in_phase"; 
		std::fwrite( &( pCell->phenotype.cycle.data.elapsed_time_in_phase ) , sizeof(double) , 1 , fp ); 
		// name = "nuclear_volume"; 
		std::fwrite( &( pCell->phenotype.volume.nuclear ) , sizeof(double) , 1 , fp );   
		// name = "cytoplasmic_volume"; 
		std::fwrite( &( pCell->phenotype.volume.cytoplasmic ) , sizeof(double) , 1 , fp );
		// name = "fluid_fraction"; 
		std::fwrite( &( pCell->phenotype.volume.fluid_fraction ) , sizeof(double) , 1 , fp );
		// name = "calcified_fraction"; 
		std::fwrite( &( pCell->phenotype.volume.calcified_fraction ) , sizeof(double) , 1 , fp ); 
		// name = "orientation"; 
		std::fwrite( pCell->state.orientation.data() , sizeof(double) , 3 , fp ); 
		// name = "polarity"; 
		std::fwrite( &( pCell->phenotype.geometry.polarity ) , sizeof(double) , 1 , fp ); 

 /* state variables to save */ 
// state
		// name = "velocity"; 
		std::fwrite( pCell->velocity.data() , sizeof(double) , 3 , fp ); 
		// name = "pressure"; 
		std::fwrite( &( pCell->state.simple_pressure ) , sizeof(double) , 1 , fp ); 
		// name = "number_of_nuclei"; 
		dTemp = (double) pCell->state.number_of_nuclei; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "damage"; 
		std::fwrite( &( pCell->state.damage ) , sizeof(double) , 1 , fp ); 
		// name = "total_attack_time"; 
		std::fwrite( &( pCell->state.total_attack_time ) , sizeof(double) , 1 , fp ); 
		// name = "contact_with_basement_membrane"; 
		dTemp = (double) pCell->state.contact_with_basement_membrane; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 

/* now go through phenotype and state */ 
// cycle 
  // current exit rate // 1 
		// name = "current_cycle_phase_exit_rate"; 
		int phase_index = pCell->phenotype.cycle.data.current_phase_index; 
		std::fwrite( &( pCell->phenotype.cycle.data.exit_rate(phase_index) ) , sizeof(double) , 1 , fp ); 
		// name = "elapsed_time_in_phase"; 
		std::fwrite( &( pCell->phenotype.cycle.data.elapsed_time_in_phase ) , sizeof(double) , 1 , fp ); 

// death 
  // live or dead state // 1 
		// name = "dead"; 
		dTemp = (double) pCell->phenotype.death.dead; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "current_death_model"; // 
		dTemp = (double) pCell->phenotype.death.current_death_model_index; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "death_rates"; 
		std::fwrite( pCell->phenotype.death.rates.data() , sizeof(double) , nd , fp ); 
		
	// volume ()
		// name = "cytoplasmic_biomass_change_rate"; 
		std::fwrite( &( pCell->phenotype.volume.cytoplasmic_biomass_change_rate ) , sizeof(double) , 1 , fp ); 
		// name = "nuclear_biomass_change_rate"; 
		std::fwrite( &( pCell->phenotype.volume.nuclear_biomass_change_rate ) , sizeof(double) , 1 , fp ); 
		// name = "fluid_change_rate"; 
		std::fwrite( &( pCell->phenotype.volume.fluid_change_rate ) , sizeof(double) , 1 , fp ); 
		// name = "calcification_rate"; 
		std::fwrite( &( pCell->phenotype.volume.calcification_rate ) , sizeof(double) , 1 , fp ); 
		// name = "target_solid_cytoplasmic"; 
		std::fwrite( &( pCell->phenotype.volume.target_solid_cytoplasmic ) , sizeof(double) , 1 , fp ); 
		// name = "target_solid_nuclear"; 
		std::fwrite( &( pCell->phenotype.volume.target_solid_nuclear ) , sizeof(double) , 1 , fp ); 
		// name = "target_fluid_fraction"; 
		std::fwrite( &( pCell->phenotype.volume.target_fluid_fraction ) , sizeof(double) , 1 , fp ); 

  // geometry 
     // radius //1 
		// name = "radius"; 
		std::fwrite( &( pCell->phenotype.geometry.radius ) , sizeof(double) , 1 , fp ); 
		// name = "nuclear_radius"; 
		std::fwrite( &( pCell->phenotype.geometry.nuclear_radius ) , sizeof(double) , 1 , fp ); 
		// name = "surface_area"; 
		std::fwrite( &( pCell->phenotype.geometry.surface_area ) , sizeof(double) , 1 , fp ); 

  // mechanics 
	// cell_cell_adhesion_strength; // 1
		// name = "cell_cell_adhesion_strength"; 
		std::fwrite( &( pCell->phenotype.mechanics.cell_cell_adhesion_strength ) , sizeof(double) , 1 , fp ); 
		// name = "cell_BM_adhesion_strength"; 
		std::fwrite( &( pCell->phenotype.mechanics.cell_BM_adhesion_strength ) , sizeof(double) , 1 , fp ); 
		// name = "cell_cell_repulsion_strength"; 
		std::fwrite( &( pCell->phenotype.mechanics.cell_cell_repulsion_strength ) , sizeof(double) , 1 , fp ); 
		// name = "cell_BM_repulsion_strength"; 
		std::fwrite( &( pCell->phenotype.mechanics.cell_BM_repulsion_strength ) , sizeof(double) , 1 , fp ); 
		// name = "cell_adhesion_affinities"; 
		std::fwrite( pCell->phenotype.mechanics.cell_adhesion_affinities.data() , sizeof(double) , n , fp ); 
		// name = "relative_maximum_adhesion_distance"; 
		std::fwrite( &( pCell->phenotype.mechanics.relative_maximum_adhesion_distance ) , sizeof(double) , 1 , fp ); 
		// name = "maximum_number_of_attachments"; 
		dTemp = (double) pCell->phenotype.mechanics.maximum_number_of_attachments; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
		// name = "attachment_elastic_constant"; 
		std::fwrite( &( pCell->phenotype.mechanics.attachment_elastic_constant ) , sizeof(double) , 1 , fp ); 
		// name = "attachment_rate"; 
		std::fwrite( &( pCell->phenotype.mechanics.attachment_rate ) , sizeof(double) , 1 , fp ); 
 		// name = "detachment_rate"; 
		std::fwrite( &( pCell->phenotype.mechanics.detachment_rate ) , sizeof(double) , 1 , fp ); 

 // Motility
 		// name = "is_motile"; 
		dTemp = (double) pCell->phenotype.motility.is_motile; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
 		// name = "persistence_time"; 
		std::fwrite( &( pCell->phenotype.motility.persistence_time ) , sizeof(double) , 1 , fp ); 
 		// name = "migration_speed"; 
		std::fwrite( &( pCell->phenotype.motility.migration_speed ) , sizeof(double) , 1 , fp ); 
 		// name = "migration_bias_direction"; 
		std::fwrite( pCell->phenotype.motility.migration_bias_direction.data() , sizeof(double) , 3 , fp ); 
 		// name = "migration_bias"; 
		std::fwrite( &( pCell->phenotype.motility.migration_bias ) , sizeof(double) , 1 , fp ); 
 		// name = "motility_vector"; 
		std::fwrite( pCell->phenotype.motility.motility_vector.data() , sizeof(double) , 3 , fp ); 
 		// name = "chemotaxis_index"; 
		dTemp = (double) pCell->phenotype.motility.chemotaxis_index; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
 		// name = "chemotaxis_direction"; 
		dTemp = (double) pCell->phenotype.motility.chemotaxis_direction; 
		std::fwrite( &( dTemp ) , sizeof(double) , 1 , fp ); 
 		// name = "chemotactic_sensitivities"; 
		std::fwrite( pCell->phenotype.motility.chemotactic_sensitivities.data() , sizeof(double) , m , fp ); 

// secretion 
 		// name = "secretion_rates"; 
		std::fwrite( pCell->phenotype.secretion.secretion_rates.data() , sizeof(double) , m , fp ); 
	 	// name = "uptake_rates"; 
		std::fwrite( pCell->phenotype.secretion.uptake_rates.data() , sizeof(double) , m , fp ); 
 		// name = "saturation_densities"; 
		std::fwrite( pCell->phenotype.secretion.saturation_densities.data() , sizeof(double) , m , fp ); 
 		// name = "net_export_rates"; 
		std::fwrite( pCell->phenotype.secretion.net_export_rates.data() , sizeof(double) , m , fp ); 

// molecular 
 		// name = "internalized_total_substrates"; 
		std::fwrite( pCell->phenotype.molecular.internalized_total_substrates.data() , sizeof(double) , m , fp ); 
 		// name = "fraction_released_at_death"; 
		std::fwrite( pCell->phenotype.molecular.fraction_released_at_death.data() , sizeof(double) , m , fp ); 
 		// name = "fraction_transferred_when_ingested"; 
		std::fwrite( pCell->phenotype.molecular.fraction_transferred_when_ingested.data() , sizeof(double) , m , fp ); 

// interactions 
 		// name = "dead_phagocytosis_rate"; 
		std::fwrite( &( pCell->phenotype.cell_interactions.dead_phagocytosis_rate ) , sizeof(double) , 1 , fp ); 
 		// name = "live_phagocytosis_rates"; 
		std::fwrite( pCell->phenotype.cell_interactions.live_phagocytosis_rates.data() , sizeof(double) , n , fp ); 
 		// name = "attack_rates"; 
		std::fwrite( pCell->phenotype.cell_interactions.attack_rates.data() , sizeof(double) , n , fp ); 
 		// name = "damage_rate"; 
		std::fwrite( &( pCell->phenotype.cell_interactions.damage_rate ) , sizeof(double) , 1 , fp ); 
 		// name = "fusion_rates"; 
		std::fwrite( pCell->phenotype.cell_interactions.fusion_rates.data() , sizeof(double) , n , fp ); 

// transformations 
  		// name = "transformation_rates"; 
		std::fwrite( pCell->phenotype.cell_transformations.transformation_rates.data() , sizeof(double) , n , fp ); 

// custom 
		// custom scalar variables 
		for( int j=0 ; j < (*all_cells)[0]->custom_data.variables.size(); j++ )
		{ std::fwrite( &( pCell->custom_data.variables[j].value ) , sizeof(double) , 1 , fp ); }

		// custom vector variables 
		for( int j=0 ; j < (*all_cells)[0]->custom_data.vector_variables.size(); j++ )
		{
			int size_temp = pCell->custom_data.vector_variables[j].value.size(); 
			std::fwrite( pCell->custom_data.vector_variables[j].value.data() , sizeof(double) , size_temp , fp );
		}
	}

	fclose( fp ); 

	// neighbor graph 
	node = node.parent().parent();  // custom 

	root = node; 
	node = node.child( "neighbor_graph" );  
	if( !node )
	{
		node = root.append_child( "neighbor_graph" ); 

		pugi::xml_attribute attrib = node.append_attribute( "type" ); 
		attrib.set_value( "text" ); 		

		attrib = node.append_attribute( "source" ); 
		attrib.set_value( "PhysiCell" ); 		

		attrib = node.append_attribute( "data_version" ); 
		attrib.set_value( "2" ); 	
	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom.neighbor_graph
	node = root.child( "filename"); 
	if( !node )
	{
		node = root.append_child( "filename" ); 

	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom.neighbor_graph.filename 


	// next, filename 
	sprintf( filename , "%s_cell_neighbor_graph.txt" , filename_base.c_str() ); 
		
	/* store filename without the relative pathing (if any) */ 
	filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	
	if( !node.first_child() )
	{
		node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
	}
	else
	{
		node.first_child().set_value( filename_without_pathing ); // filename ); 
	}	

	write_neighbor_graph( filename ); 


	// attached cell graph 
	node = root; 
	node = node.parent().parent(); // root = cellular_information.cell_populations.cell_population.custom

	root = node; 
	node = node.child( "attached_cells_graph" );  
	if( !node )
	{
		node = root.append_child( "attached_cells_graph" ); 

		pugi::xml_attribute attrib = node.append_attribute( "type" ); 
		attrib.set_value( "text" ); 		

		attrib = node.append_attribute( "source" ); 
		attrib.set_value( "PhysiCell" ); 		

		attrib = node.append_attribute( "data_version" ); 
		attrib.set_value( "2" ); 	
	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom.attached_cells_graph
	node = root.child( "filename"); 
	if( !node )
	{ node = root.append_child( "filename" ); }
	root = node; // root = cellular_information.cell_populations.cell_population.custom.attached_cells_graph.filename 


	// next, filename 
	sprintf( filename , "%s_attached_cells_graph.txt" , filename_base.c_str() ); 
		
	/* store filename without the relative pathing (if any) */ 
	filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	
	if( !node.first_child() )
	{
		node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
	}
	else
	{
		node.first_child().set_value( filename_without_pathing ); // filename ); 
	}	

	write_attached_cells_graph( filename );	

	// spring attached cell graph 
	node = root; 
	node = node.parent().parent(); // root = cellular_information.cell_populations.cell_population.custom

	root = node; 
	node = node.child( "spring_attached_cells_graph" );  
	if( !node )
	{
		node = root.append_child( "spring_attached_cells_graph" ); 

		pugi::xml_attribute attrib = node.append_attribute( "type" ); 
		attrib.set_value( "text" ); 		

		attrib = node.append_attribute( "source" ); 
		attrib.set_value( "PhysiCell" ); 		

		attrib = node.append_attribute( "data_version" ); 
		attrib.set_value( "2" ); 	
	}
	root = node; // root = cellular_information.cell_populations.cell_population.custom.spring_attached_cells_graph
	node = root.child( "filename"); 
	if( !node )
	{ node = root.append_child( "filename" ); }
	root = node; // root = cellular_information.cell_populations.cell_population.custom.spring_attached_cells_graph.filename 


	// next, filename 
	sprintf( filename , "%s_spring_attached_cells_graph.txt" , filename_base.c_str() ); 
		
	/* store filename without the relative pathing (if any) */ 
	filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	
	if( !node.first_child() )
	{
		node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
	}
	else
	{
		node.first_child().set_value( filename_without_pathing ); // filename ); 
	}	

	write_spring_attached_cells_graph( filename ); 

	return; 
}

void write_neighbor_graph( std::string filename )
{
	/*
	char filename [1024]; 
	sprintf( filename , "%s_cell_neighbor_graph.txt" , filename_base.c_str() ); 
	
	// store filename without the relative pathing (if any) 
	char filename_without_pathing [1024];
	char* filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	*/

	std::ofstream of( filename , std::ios::out ); 
	std::stringstream buffer; 

	for( int i=0 ; i < (*all_cells).size(); i++ )
	{
		buffer << (*all_cells)[i]->ID << ": " ; 
		int size = (*all_cells)[i]->state.neighbors.size(); 
		for( int j=0 ; j < size; j++ )
		{
			buffer << (*all_cells)[i]->state.neighbors[j]->ID; 
			if( j != size-1 )
			{ buffer << ","; }
		}
		if( i != (*all_cells).size()-1 )
		{ buffer << std::endl; }
		of << buffer.rdbuf(); 
	}
	of.close(); 

	return; 
}


void write_attached_cells_graph( std::string filename ) 
{
	/*
	char filename [1024]; 
	sprintf( filename , "%s_cell_attached_graph.txt" , filename_base.c_str() ); 
	
	// store filename without the relative pathing (if any) 
	char filename_without_pathing [1024];
	char* filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	*/

	std::ofstream of( filename , std::ios::out ); 
	std::stringstream buffer; 

	for( int i=0 ; i < (*all_cells).size(); i++ )
	{
		buffer << (*all_cells)[i]->ID << ": " ; 
		int size = (*all_cells)[i]->state.attached_cells.size(); 
		for( int j=0 ; j < size; j++ )
		{
			buffer << (*all_cells)[i]->state.attached_cells[j]->ID; 
			if( j != size-1 )
			{ buffer << ","; }
		}
		if( i != (*all_cells).size()-1 )
		{ buffer << std::endl; }
		of << buffer.rdbuf(); 
	}
	of.close(); 

	return; 
}
 
void write_spring_attached_cells_graph( std::string filename ) 
{
	/*
	char filename [1024]; 
	sprintf( filename , "%s_cell_attached_graph.txt" , filename_base.c_str() ); 
	
	// store filename without the relative pathing (if any) 
	char filename_without_pathing [1024];
	char* filename_start = strrchr( filename , '/' ); 
	if( filename_start == NULL )
	{ filename_start = filename; }
	else	
	{ filename_start++; } 
	strcpy( filename_without_pathing , filename_start );  
	*/

	std::ofstream of( filename , std::ios::out ); 
	std::stringstream buffer; 

	for( int i=0 ; i < (*all_cells).size(); i++ )
	{
		buffer << (*all_cells)[i]->ID << ": " ; 
		int size = (*all_cells)[i]->state.spring_attachments.size();
		for( int j=0 ; j < size; j++ )
		{
			buffer << (*all_cells)[i]->state.spring_attachments[j]->ID; 
			if( j != size-1 )
			{ buffer << ","; }
		}
		if( i != (*all_cells).size()-1 )
		{ buffer << std::endl; }
		of << buffer.rdbuf(); 
	}
	of.close(); 

	return; 
}
};
