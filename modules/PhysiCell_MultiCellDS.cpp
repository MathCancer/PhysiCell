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
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
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
		
	// Lastly, save to the indicated filename 

	char filename[1024]; 
	sprintf( filename , "%s.xml" , filename_base.c_str() ); 
	BioFVM::biofvm_doc.save_file( filename );

	return; 
}

 

};
