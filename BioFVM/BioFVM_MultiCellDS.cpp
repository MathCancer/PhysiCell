/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the BioFVM Project              #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#include "BioFVM.h" 
// #include "BioFVM_strings.h" 
#include "BioFVM_MultiCellDS.h"

namespace BioFVM{
std::string BioFVM_Version = "1.1.7";
std::string BioFVM_URL = "http://BioFVM.MathCancer.org"; 
		
bool biofvm_dom_initialized = false; 
pugi::xml_document biofvm_doc; 

std::string MultiCellDS_version_string = "0.5"; 

std::string MultiCellDS_clinical_snapshot_type_string = "snapshot/clinical";  
int MultiCellDS_clinical_snapshot_code = 4; 

std::string MultiCellDS_experimental_snapshot_type_string = "snapshot/experimental";  
int MultiCellDS_experimental_snapshot_code = 3; 

std::string MultiCellDS_simulation_snapshot_type_string = "snapshot/simulation"; 
int MultiCellDS_simulation_snapshot_code = 2; 

std::string MultiCellDS_digital_cell_line_type_string = "digital_cell_line";
int MultiCellDS_digital_cell_line_code = 1; 

std::string MultiCellDS_generic_type_string = "generic";
int MultiCellDS_generic_code = 0; 

bool biofvm_multicellds_BioFVM_metadata_initialized = false; 
MultiCellDS_Metadata BioFVM_metadata; 

/* options */ 

bool save_mesh_as_matlab = true; 
bool save_density_data_as_matlab = true;
bool save_cells_as_custom_matlab = true; 
bool save_cell_data = true; 
		
Person_Metadata::Person_Metadata()
{
	is_empty = true; 
	type = "user";
	surname = ""; 
	given_names = ""; 
	email = "";
	URL = ""; 
	organization = "";
	department = "";
	ORCID = "";		
}

void Person_Metadata::display_information( std::ostream& os )
{
	os 	<< this->type << " name: " << given_names << " " << surname << std::endl 
		<< "organization: " << organization << std::endl 
		<< "department: " << department << std::endl 
		<< "email: " << email << std::endl 
		<< "URL: " << URL << std::endl 
		<< "ORCID: " << ORCID << std::endl << std::endl ; 
}

		
void Person_Metadata::insert_in_open_xml_pugi( pugi::xml_node& insert_here )
{
	if( ORCID.size() )
	{ is_empty = false; } 
	if( given_names.size() )
	{ is_empty = false; } 
	if( surname.size() )
	{ is_empty = false; } 
	if( email.size() )
	{ is_empty = false; } 
	if( URL.size() )
	{ is_empty = false; } 
	if( organization.size() )
	{ is_empty = false; } 
	if( department.size() )
	{ is_empty = false; } 

	// user information
	pugi::xml_node node = insert_here.append_child( type.c_str() ); 

	if( is_empty == true )
	{ return; }
	node = node.append_child( "orcid-identifier" );
	
	if( ORCID.size() )
	{
		node = node.append_child( "path" ); 
		node.append_child( pugi::node_pcdata ).set_value( ORCID.c_str() ); 
		
		node = node.parent(); 
	}
	if( given_names.size() )
	{
		node = node.append_child( "given-names" ); 
		node.append_child( pugi::node_pcdata ).set_value( given_names.c_str() ); 
		
		node = node.parent(); 
	}
	if( surname.size() )
	{
		node = node.append_child( "family-name" ); 
		node.append_child( pugi::node_pcdata ).set_value( surname.c_str() ); 
		
		node = node.parent(); 
	}
	if( email.size() )
	{
		node = node.append_child( "email" ); 
		node.append_child( pugi::node_pcdata ).set_value( email.c_str() ); 
		
		node = node.parent(); 
	}
	if( URL.size() )
	{
		node = node.append_child( "url" ); 
		node.append_child( pugi::node_pcdata ).set_value( URL.c_str() ); 
		
		node = node.parent(); 
	}		
	if( organization.size() )
	{
		node = node.append_child( "organization-name" ); 
		node.append_child( pugi::node_pcdata ).set_value( organization.c_str() ); 
		
		node = node.parent(); 
	}		
	if( department.size() )
	{
		node = node.append_child( "department-name" ); 
		node.append_child( pugi::node_pcdata ).set_value( department.c_str() ); 
		
		node = node.parent(); 
	}	

	return; 
}

Citation_Metadata::Citation_Metadata()
{
	DOI = "";
	PMID = "";
	PMCID = "";
	text = "";
	URL = "";
	notes = "";
	return; 	
}

void Citation_Metadata::display_information( std::ostream& os )
{
	os << 	"citation: " << std::endl
		<<	"---------" << std::endl
		<<  "DOI: " << DOI << std::endl 
		<< 	"PMID: " << PMID << std::endl 
		<< 	"PMCID: " << PMCID << std::endl 
		<< 	"URL: " << URL << std::endl 
		<< 	"text: " << text << std::endl 
		<< 	"notes: " << notes << std::endl << std::endl; 
	return; 
}

void Citation_Metadata::insert_in_open_xml_pugi( pugi::xml_node& insert_here )
{
	// citation information
	pugi::xml_node node = insert_here.append_child( "citation" ); 
	
	if( text.size() )
	{
		node = node.append_child( "text" ); 
		node.append_child( pugi::node_pcdata ).set_value( text.c_str() ); 
		
		node = node.parent(); 
	}		
	if( DOI.size() )
	{
		node = node.append_child( "DOI" ); 
		node.append_child( pugi::node_pcdata ).set_value( DOI.c_str() ); 
		
		node = node.parent(); 
	}
	if( URL.size() )
	{
		node = node.append_child( "URL" ); 
		node.append_child( pugi::node_pcdata ).set_value( URL.c_str() ); 
		
		node = node.parent(); 
	}
	if( PMID.size() )
	{
		node = node.append_child( "PMID" ); 
		node.append_child( pugi::node_pcdata ).set_value( PMID.c_str() ); 
		
		node = node.parent(); 
	}
	if( PMCID.size() )
	{
		node = node.append_child( "PMCID" ); 
		node.append_child( pugi::node_pcdata ).set_value( PMCID.c_str() ); 
		
		node = node.parent(); 
	}
	if( notes.size() )
	{
		node = node.append_child( "notes" ); 
		node.append_child( pugi::node_pcdata ).set_value( notes.c_str() ); 
		
		node = node.parent(); 
	}		

	return; 
}

Software_Metadata::Software_Metadata()
{
	program_name = "BioFVM";
	program_version = BioFVM_Version; 
	program_URL = BioFVM_URL;
	
	creator.type = "creator"; 
	
	return; 
}

void Software_Metadata::display_information( std::ostream& os )
{
	os 	<< "Program: " << program_name << " (" << program_version << ")" << std::endl 
		<< "Project URL: " << program_URL << std::endl; 
	creator.display_information( os );
	user.display_information( os ); 
	citation.display_information( os ); 
	return; 
}
		
void Software_Metadata::insert_in_open_xml_pugi( pugi::xml_node& insert_here )
{
	// user information
	pugi::xml_node node = insert_here.append_child( "software" ); 
	
	if( program_name.size() )
	{
		node = node.append_child( "name" ); 
		node.append_child( pugi::node_pcdata ).set_value( program_name.c_str() ); 	
		node = node.parent();
		
		node = node.append_child( "version" );
		node.append_child( pugi::node_pcdata ).set_value( program_version.c_str() ); 
		node = node.parent(); // software level 
		
		node = node.append_child( "URL" );
		node.append_child( pugi::node_pcdata ).set_value( program_URL.c_str() );
		node = node.parent();
	}
	
	creator.insert_in_open_xml_pugi( node ); 
	citation.insert_in_open_xml_pugi( node ); 
	user.insert_in_open_xml_pugi( node );

	return; 
}


MultiCellDS_Metadata::MultiCellDS_Metadata()
{
	MultiCellDS_type = MultiCellDS_simulation_snapshot_type_string;  
	
	current_time = 0;  
	
	spatial_units = "micron";
	time_units = "min";
	runtime_units = "sec";
	
	current_runtime = 0; 
	RUNTIME_TIC();
}


void MultiCellDS_Metadata::restart_runtime( void )
{
	RUNTIME_TIC();
	RUNTIME_TOC();
	current_runtime = runtime_stopwatch_value(); 
	return; 	
}

void MultiCellDS_Metadata::display_information( std::ostream& os)
{
	RUNTIME_TOC();
	current_runtime = runtime_stopwatch_value(); 
	
	program.display_information( os );
	data_citation.display_information( os ); 
	
	os	<< "MultiCellDS type: " << MultiCellDS_type << " (Version " << MultiCellDS_version_string << ")" << std::endl 
		<< "Spatial units: " << spatial_units << std::endl 
		<< "Time units: " << time_units << std::endl 
		<< "Current simulation time: " << current_time << " " << time_units << std::endl 
		<< "Program run time: " << current_runtime << " " << runtime_units << std::endl; 
}

void MultiCellDS_Metadata::sync_to_microenvironment( Microenvironment& M )
{
	spatial_units = M.spatial_units; 
	time_units = M.time_units; 
	
	return; 
}

void MultiCellDS_Metadata::add_to_open_xml_pugi( double current_simulation_time , pugi::xml_document& xml_dom )
{
	static bool metadata_initialized_in_dom = false; 
	
	// update the current runtime 
	RUNTIME_TOC();
	BioFVM_metadata.current_runtime = runtime_stopwatch_value(); 
	
	// update the current simulation runtime 
	
	current_time = current_simulation_time; 
	
	// make sure the main structure is there. 
	
	add_MultiCellDS_main_structure_to_open_xml_pugi( xml_dom ); 
	
	// Find the root.  
	pugi::xml_node root = xml_dom.child( "MultiCellDS");
	
	// if the root is non-empty, just update the times. 
	if( metadata_initialized_in_dom )
	{
		// simulation time 
		pugi::xml_node node = root.child( "metadata");
		node = node.child( "current_time" ); 
		char buffer [1024];
		sprintf( buffer , "%f" , BioFVM_metadata.current_time ); 
		node.first_child().set_value( buffer ); // BioFVM_metadata.current_time ); 

		// current wall time 
		
		RUNTIME_TOC();
		BioFVM_metadata.current_runtime = runtime_stopwatch_value(); 		
		
		node = node.parent(); 
		node = node.child( "current_runtime" ); 
		sprintf( buffer , "%f" , BioFVM_metadata.current_runtime ); 
		node.first_child( ).set_value( buffer ); 
		
		node = node.parent(); 
		node = node.child( "created" ); 
		
		std::time_t t_now = std::time(NULL);
		std::strftime(buffer , 1024 , "%Y-%m-%dT%H:%M:%SZ" , std::gmtime(&t_now));
	//	std::strftime(buffer , 1024 , "%Y-%m-%dT%H:%M:%S%z" , std::gmtime(&t_now));		
		
		node.first_child( ).set_value( buffer ); 
		
		node = node.parent(); 
		node = node.child( "last_modified" ); 
		node.first_child( ).set_value( buffer ); 		
		
		return; 
	}
	
	// The metadata haven't been properly initialized in the DOM. 
		
	pugi::xml_attribute attrib = root.attribute( "version" ); 
	attrib.set_value( MultiCellDS_version_string.c_str() ); 
	attrib = root.attribute( "type" ); 
	attrib.set_value( MultiCellDS_type.c_str() ); 

	// fill out metadata 
	pugi::xml_node node = root.child( "metadata" ); 
	
	// program information 
	program.insert_in_open_xml_pugi( node ); 

	// data citation 
	data_citation.insert_in_open_xml_pugi( node ); 

	char* buffer;
	buffer = new char [1024];

	// simulation time 
	node = node.append_child( "current_time" ); 
	attrib = node.append_attribute( "units" ); 
	attrib.set_value( BioFVM_metadata.time_units.c_str() ); 
	sprintf( buffer , "%f" , BioFVM_metadata.current_time ); 
	node.append_child( pugi::node_pcdata ).set_value( buffer ); 

	// current wall time 
	node = node.parent(); 
	node = node.append_child( "current_runtime" ); 
	attrib = node.append_attribute( "units" ); 
	attrib.set_value( BioFVM_metadata.runtime_units.c_str() ); 
	sprintf( buffer , "%f" , BioFVM_metadata.current_runtime ); 
	node.append_child( pugi::node_pcdata ).set_value( buffer ); 
	
	// created (timestamp)
	
	node = node.parent();
	node = node.append_child( "created" ); 

	std::time_t t_now = std::time(NULL);
	std::strftime(buffer , 1024 , "%Y-%m-%dT%H:%M:%SZ" , std::gmtime(&t_now));
//	std::strftime(buffer , 1024 , "%Y-%m-%dT%H:%M:%S%z" , std::gmtime(&t_now));

	node.append_child( pugi::node_pcdata ).set_value( buffer ); 
	
	// last modified (timestamp)
		
	node = node.parent();
	node = node.append_child( "last_modified" ); 
	node.append_child( pugi::node_pcdata ).set_value( buffer ); 
	
	delete [] buffer; 
	
	metadata_initialized_in_dom = true; 
}

/* setting up the main MultiCellDS tree structure */ 

void add_MultiCellDS_main_structure_to_open_xml_pugi( pugi::xml_document& xml_dom )
{
	pugi::xml_node root = xml_dom.child( "MultiCellDS" ); 
	if( !root )
	{
		root = xml_dom.append_child( "MultiCellDS" ); 
		pugi::xml_attribute attrib = root.append_attribute( "version" ); 
		attrib.set_value( MultiCellDS_version_string.c_str() ); 
		attrib = root.append_attribute( "type" ); 
		attrib.set_value( MultiCellDS_simulation_snapshot_type_string.c_str() ); 
	}
	
	pugi::xml_node node = root.child( "metadata" ); 
	if( !node )
	{ root.append_child( "metadata" ); }
	
	node = root.child( "microenvironment" ); 
	if( !node )
	{ node = root.append_child( "microenvironment" ); }

	node = root.child( "microenvironment" ); 
	if( !node )
	{ node = root.append_child( "microenvironment" ); }
	
	node = root.child( "cellular_information" ); 
	if( !node )
	{ node = root.append_child( "cellular_information" ); }
	
	return; 
}

/* set options */ 

void set_save_biofvm_mesh_as_matlab( bool newvalue )
{ save_mesh_as_matlab = newvalue; }

void set_save_biofvm_data_as_matlab( bool newvalue )
{ save_density_data_as_matlab = newvalue; }

void set_save_biofvm_cell_data( bool newvalue )
{ save_cell_data = newvalue; }

void set_save_biofvm_cell_data_as_custom_matlab( bool newvalue )
{ save_cells_as_custom_matlab = newvalue; }

/* writing parts of BioFVM to a MultiCellDS file */ 

void add_BioFVM_substrates_to_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, Microenvironment& M )
{
	add_MultiCellDS_main_structure_to_open_xml_pugi( xml_dom ); 
	
	pugi::xml_node root = biofvm_doc.child( "MultiCellDS" );
	pugi::xml_node node = root.child( "microenvironment" ); 
	
	static bool BioFVM_substrates_initialized_in_dom = false; 
	
	// if the TME has not yet been initialized in the DOM, create all the 
	// right data elements, and populate the meshes. 
	if( !BioFVM_substrates_initialized_in_dom )
	{
		char* buffer; 
		buffer = new char [1024]; 
		
		// add a domain 
		node = node.append_child( "domain" ); 
		pugi::xml_attribute attrib = node.append_attribute( "name" ); 
		attrib.set_value( M.name.c_str() ); 
		
		// add the mesh
		
		node = node.append_child( "mesh" ); 
		// information about the mesh 
		attrib = node.append_attribute( "type" ); 
		if( M.mesh.Cartesian_mesh == true )
		{ attrib.set_value( "Cartesian" ); }
		else
		{ attrib.set_value( "general"); }
		attrib = node.append_attribute( "uniform" ); 
		attrib.set_value( M.mesh.uniform_mesh ); 
		attrib = node.append_attribute( "regular" ); 
		attrib.set_value( M.mesh.regular_mesh ); 		
		attrib = node.append_attribute( "units" ); 
		attrib.set_value( M.mesh.units.c_str() );
		// add the bounding box 
		node = node.append_child( "bounding_box" ); 
		attrib = node.append_attribute( "type" ); 
		attrib.set_value( "axis-aligned" ); 
		attrib = node.append_attribute( "units" ); 
		attrib.set_value( M.mesh.units.c_str() ); 
		sprintf( buffer , "%f %f %f %f %f %f" , M.mesh.bounding_box[0] , M.mesh.bounding_box[1] , M.mesh.bounding_box[2] , 
		M.mesh.bounding_box[3] , M.mesh.bounding_box[4] , M.mesh.bounding_box[5] ); 
		node.append_child( pugi::node_pcdata ).set_value( buffer ); 
		node = node.parent(); 		
		// if Cartesian, add the x, y, and z coordinates 
		if( M.mesh.Cartesian_mesh == true )
		{
			char temp [10240];
			int position = 0; 
			for( unsigned int k=0 ; k < M.mesh.x_coordinates.size()-1 ; k++ )
			{ position += sprintf( temp+position, "%f " , M.mesh.x_coordinates[k] ); }
			sprintf( temp+position , "%f" , M.mesh.x_coordinates[ M.mesh.x_coordinates.size()-1] ); 
			node = node.append_child( "x_coordinates" ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			attrib = node.append_attribute("delimiter");
			attrib.set_value( " " ); 
			
			node = node.parent();
			position = 0; 
			for( unsigned int k=0 ; k < M.mesh.y_coordinates.size()-1 ; k++ )
			{ position += sprintf( temp+position, "%f " , M.mesh.y_coordinates[k] ); }
			sprintf( temp+position , "%f" , M.mesh.y_coordinates[ M.mesh.y_coordinates.size()-1] ); 
			node = node.append_child( "y_coordinates" ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			attrib = node.append_attribute("delimiter");
			attrib.set_value( " " ); 
			
			node = node.parent();
			position = 0; 
			for( unsigned int k=0 ; k < M.mesh.z_coordinates.size()-1 ; k++ )
			{ position += sprintf( temp+position, "%f " , M.mesh.z_coordinates[k] ); }
			sprintf( temp+position , "%f" , M.mesh.z_coordinates[ M.mesh.z_coordinates.size()-1] ); 
			node = node.append_child( "z_coordinates" ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			attrib = node.append_attribute("delimiter");
			attrib.set_value( " " ); 
			node = node.parent(); 
		}
		// write out the voxels -- minimal data, even if redundant for cartesian 
		if( save_mesh_as_matlab == false )
		{
			node = node.append_child( "voxels" ); 
			attrib = node.append_attribute("type");
			attrib.set_value( "xml" ); 
			char temp [1024]; 
			for( unsigned int k=0; k < M.mesh.voxels.size() ; k++ )
			{
				node = node.append_child( "voxel" );
				
				attrib = node.append_attribute( "ID" ); 
				attrib.set_value( M.mesh.voxels[k].mesh_index ); 
				attrib = node.append_attribute( "type" ); 
				attrib.set_value( "cube" ); // allowed: cube or unknown 

				node = node.append_child( "center" );
				attrib = node.append_attribute( "delimiter" );
				attrib.set_value( " " );
				sprintf( temp , "%f %f %f" , M.mesh.voxels[k].center[0] , M.mesh.voxels[k].center[1], M.mesh.voxels[k].center[2] );
				node.append_child( pugi::node_pcdata ).set_value( temp ); 
				node = node.parent(); 
				
				node = node.append_child( "volume" );
				sprintf( temp , "%f" , M.mesh.voxels[k].volume );
				node.append_child( pugi::node_pcdata ).set_value( temp ); 
				node = node.parent(); 

				node = node.parent(); 
			}
			node = node.parent(); 
		}
		else
		{
			node = node.append_child( "voxels" ); 
			attrib = node.append_attribute("type");
			attrib.set_value( "matlab"); 
	
			char filename [1024]; 
			sprintf( filename , "%s_mesh%d.mat" , filename_base.c_str() , 0 ); 
			M.mesh.write_to_matlab( filename ); 
			
			node = node.append_child( "filename" ); 
			
			/* store filename without the relative pathing (if any) */ 
			char filename_without_pathing [1024];
			char* filename_start = strrchr( filename , '/' ); 
			if( filename_start == NULL )
			{ filename_start = filename; }
			else	
			{ filename_start++; } 
			strcpy( filename_without_pathing , filename_start ); 
			
			node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename ); 
			
			node = node.parent(); 
			
			node = node.parent(); 
		}
		node = node.parent(); // back to level of "microenvironment"  
		
		// define the variables 

		node = node.append_child( "variables" ); 
		
		char temp [1024]; 
		for( unsigned int j=0 ; j < M.number_of_densities() ; j++ )
		{
			node = node.append_child( "variable" ); 
			attrib = node.append_attribute( "name" ); 
			attrib.set_value( M.density_names[j].c_str() ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( M.density_units[j].c_str() ); 
			attrib = node.append_attribute( "ID" ); 
			attrib.set_value( j ); 
			
			node = node.append_child( "physical_parameter_set" ); 
			node = node.append_child( "conditions" ); 
			node = node.parent(); 
			
			node = node.append_child( "diffusion_coefficient" ); 
			sprintf( temp , "%f" , M.diffusion_coefficients[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp );
			attrib = node.append_attribute( "units" ); 
			sprintf( temp , "%s^2/%s" , M.spatial_units.c_str() , M.time_units.c_str() ); 
			attrib.set_value( temp ); 
			node = node.parent(); 

			node = node.append_child( "decay_rate" ); 
			sprintf( temp , "%f" , M.decay_rates[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp );
			attrib = node.append_attribute( "units" ); 
			sprintf( temp , "1/%s" , M.time_units.c_str() ); 
			attrib.set_value( temp ); 
			node = node.parent(); 

		   node = node.parent(); 
		   node = node.parent();
		}
		node = node.parent(); 
		
		// create space for the data 
		// write out the voxels -- minimal data, even if redundant for cartesian 
		node = node.append_child( "data" ); 
		attrib = node.append_attribute("type");
		if( save_density_data_as_matlab == false )
		{
			attrib.set_value( "xml" ); 
			
			// create the DOM structure, make it big enough 
			int datum_size = 16; // enough for sprintf default 6 decimal places + period + 5 leading figs (safety) + delimiter + 2 chars safety = 15
			int data_size = datum_size * M.number_of_densities(); 
			
			char* buffer; 
			buffer = new char [data_size]; 
			for( unsigned int j=0 ; j < M.mesh.voxels.size() ; j++ )
			{
				vector_to_list( M.density_vector(j) , buffer , ' ' ); 
				node = node.append_child( "data_vector"); 
				attrib = node.append_attribute( "voxel_ID" ); 
				attrib.set_value( M.mesh.voxels[j].mesh_index ); 
				attrib = node.append_attribute( "delimiter" ); 
				attrib.set_value( " " ); 
				
				node.append_child( pugi::node_pcdata ).set_value( buffer ); 
				node = node.parent(); 
			}
			delete [] buffer; 			
			
		}
		else
		{
			attrib.set_value( "matlab"); 
			
			node = node.append_child( "filename" ); 
			// say where the data are stored, and store them;
			char filename [1024]; 
			sprintf( filename , "%s_microenvironment%d.mat" , filename_base.c_str() , 0 ); 
			M.write_to_matlab( filename ); 
			
			/* store filename without the relative pathing (if any) */ 
			char filename_without_pathing [1024];
			char* filename_start = strrchr( filename , '/' ); 
			if( filename_start == NULL )
			{ filename_start = filename; }
			else	
			{ filename_start++; } 
			strcpy( filename_without_pathing , filename_start ); 
			
			node.append_child( pugi::node_pcdata ).set_value( filename_without_pathing ); // filename );				
			
			node = node.parent(); 
		}
		node = node.parent(); 
		
		BioFVM_substrates_initialized_in_dom = true; 
		
		delete [] buffer; 
		
		return; 
	}
	
	// populate the data values 
	
	node = node.child( "domain" ); 
	node = node.child( "data" ); 
	
	if( save_density_data_as_matlab == false )
	{
		// create the DOM structure, make it big enough 
		int datum_size = 16; // enough for sprintf default 6 decimal places + period + 5 leading figs (safety) + delimiter + 2 chars safety = 15
		int data_size = datum_size * M.number_of_densities(); 
		
		char* buffer; 
		buffer = new char [data_size]; 
		node = node.child( "data_vector" );
		for( unsigned int j=0 ; j < M.mesh.voxels.size() ; j++ )
		{
			vector_to_list( M.density_vector(j) , buffer , ' ' ); 
			node = node.first_child(); 
			
			node.set_value( buffer ); 
			node = node.parent(); 
			
			node = node.next_sibling(); 
		}
		node = node.parent(); 
		delete [] buffer; 			
		
	}
	else
	{
		// say where the data are stored, and store them;
		
		node = node.child( "filename" );
		
		char filename [1024]; 
		sprintf( filename , "%s_microenvironment%d.mat" , filename_base.c_str() , 0 ); 
		M.write_to_matlab( filename ); 
		
		/* store filename without the relative pathing (if any) */ 
		char filename_without_pathing [1024];
		char* filename_start = strrchr( filename , '/' ); 
		if( filename_start == NULL )
		{ filename_start = filename; }
		else	
		{ filename_start++; } 
		strcpy( filename_without_pathing , filename_start ); 
		
		node = node.first_child(); 
		node.set_value( filename_without_pathing ); // filename ); 
		node = node.parent(); 
	}
	
	return; 
}

// not yet implemented 
void add_BioFVM_basic_agent_to_open_xml_pugi( pugi::xml_document& xml_dom, Basic_Agent& BA  ); 

void add_BioFVM_agents_to_open_xml_pugi( pugi::xml_document& xml_dom, std::string filename_base, Microenvironment& M )
{
	if( save_cell_data == false )
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
	if( save_cells_as_custom_matlab == true )
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
		
		if( !node.child( "simplified_data" ) )
		{
			pugi::xml_node node1 = node.append_child( "simplified_data" ); 
			pugi::xml_attribute attrib = node1.append_attribute( "type" ); 
			attrib.set_value( "matlab" ) ; 
			
			attrib = node1.append_attribute( "source" ); 
			attrib.set_value("BioFVM"); 
			
		}
		node = node.child( "simplified_data" ); 
		
		if( !node.child( "filename" ) )
		{
			node.append_child( "filename" ); 
		}
		node = node.child( "filename" ); 
		
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
		
		// next, create a matlab structure and save it!
		
		// order: ID,x,y,z,volume,radius, 
		int number_of_data_entries = all_basic_agents.size(); 
		int size_of_each_datum = 1 + 3 + 1 + 3*M.number_of_densities(); // ID, x,y,z, volume,  src,sink,saturation (multiple) 

		FILE* fp = write_matlab_header( size_of_each_datum, number_of_data_entries,  filename, "basic_agents" );  

		// storing data as cols 
		for( int i=0; i < number_of_data_entries ; i++ )
		{
			double ID_temp = (double) all_basic_agents[i]->ID;
			fwrite( (char*) &( ID_temp ) , sizeof(double) , 1 , fp ); 

			fwrite( (char*) &( all_basic_agents[i]->position[0] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( all_basic_agents[i]->position[1] ) , sizeof(double) , 1 , fp ); 
			fwrite( (char*) &( all_basic_agents[i]->position[2] ) , sizeof(double) , 1 , fp ); 
			double volTemp=all_basic_agents[i]->get_total_volume();
			fwrite( (char*) &( volTemp ) , sizeof(double) , 1 , fp ); 
			
			// add variables and their source/sink/saturation values (per-cell basis)
			for( unsigned int j=0; j < M.number_of_densities() ; j++ ) 
			{
				double dTemp = all_basic_agents[i]->get_total_volume() * (*all_basic_agents[i]->secretion_rates)[j]; 
				fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); 
				dTemp = all_basic_agents[i]->get_total_volume() * (*all_basic_agents[i]->uptake_rates)[j]; 
				fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); 
				dTemp = (*all_basic_agents[i]->saturation_densities)[j]; 
				fwrite( (char*) &( dTemp ) , sizeof(double) , 1 , fp ); 
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
	for( unsigned int i=0; i < all_basic_agents.size(); i++ )
	{
		node = node.append_child( "cell" ); 
		attrib = node.append_attribute( "ID" ); 
		attrib.set_value(  all_basic_agents[i]->ID ); 
		
		node = node.append_child( "phenotype_dataset" ); 
		node = node.append_child( "phenotype" ); // add a type? 
		
		// add all the transport information 
		node = node.append_child( "transport_processes" ); 
		
		// add variables and their source/sink/saturation values (per-cell basis)
		for( unsigned int j=0; j < M.number_of_densities() ; j++ ) 
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
			sprintf( temp , "%f" , all_basic_agents[i]->get_total_volume() * (*all_basic_agents[i]->secretion_rates)[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			node = node.parent( ); 
			
			node = node.append_child( "import_rate" ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( rate_chars ); 
			sprintf( temp,  "%f" , all_basic_agents[i]->get_total_volume() * (*all_basic_agents[i]->uptake_rates)[j] ); 
			node.append_child( pugi::node_pcdata ).set_value( temp ); 
			node = node.parent(); 
			
			node = node.append_child( "saturation_density" ); 
			attrib = node.append_attribute( "units" ); 
			attrib.set_value( M.density_units[j].c_str() ); 
			sprintf( temp, "%f" , (*all_basic_agents[i]->saturation_densities)[j] ); 
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
		sprintf( temp,  "%f" , all_basic_agents[i]->get_total_volume()  ); 
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
		sprintf( temp , "%.7e %.7e %.7e" , all_basic_agents[i]->position[0], all_basic_agents[i]->position[1], all_basic_agents[i]->position[2] ); 
		node.append_child( pugi::node_pcdata ).set_value( temp ); 		
		
		node = root; 
	}
	
	return; 
}

void add_BioFVM_to_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, double current_simulation_time , Microenvironment& M )
{
	
	add_MultiCellDS_main_structure_to_open_xml_pugi( xml_dom ); 
	
	BioFVM_metadata.add_to_open_xml_pugi( current_simulation_time , xml_dom ); 

	add_BioFVM_substrates_to_open_xml_pugi( xml_dom , filename_base, M  ); 

	// add vessels (if there) 
	// add basic agents (if there)
	add_BioFVM_agents_to_open_xml_pugi( xml_dom , filename_base, M); 
	
	return; 
}

void save_BioFVM_to_MultiCellDS_xml_pugi( std::string filename_base , Microenvironment& M , double current_simulation_time)
{
	add_BioFVM_to_open_xml_pugi( biofvm_doc , filename_base , current_simulation_time , M ); 
		
	char filename[1024]; 
	sprintf( filename , "%s.xml" , filename_base.c_str() ); 
	biofvm_doc.save_file( filename );
	
	std::cout << "done!" << std::endl; 
	
	return; 
}

/* future / not yet supported */

void read_BioFVM_from_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, double& current_simulation_time , Microenvironment& M );
void read_BioFVM_to_MultiCellDS_xml_pugi( std::string filename_base , Microenvironment& M , double& current_simulation_time ); 

/* partly-implemented code snippets -- not to be used as of February 2016 */

// not yet supported 
void read_microenvironment_from_MultiCellDS_xml( Microenvironment& M_destination , std::string filename )
{
	std::cout << "Reading data from file " << filename << " ... " ; 
	pugi::xml_document doc; 
	// pugi::xml_parse_result result = // g++ warning: set but not used 
	doc.load_file( filename.c_str()  );
	
	read_microenvironment_from_MultiCellDS_xml( M_destination , doc ); 
}

// not yet supported 
void read_microenvironment_from_MultiCellDS_xml( Microenvironment& M_destination , pugi::xml_document& xml_dom )
{
        size_t result;

	// find the first microenvironment 
	
	using namespace pugi;
	xml_node root = xml_dom.child("MultiCellDS");
	root = root.child( "microenvironment");
	root = root.child("domain");

	// read all the microenvironments 
	
	// int microenvironment_index = -1; // g++ warning: set but not used 
	while( root )
	{
		M_destination.name = root.attribute("name").value(); 
		
		// read the mesh 
		bool cartesian = true; 
		
		xml_node node = root.child( "mesh" );
		if( strcmp(  node.attribute( "type" ).value()  , "Cartesian" ) != 0 && strcmp(  node.attribute( "type" ).value()  , "cartesian" ) != 0  ) 
		{ cartesian = false; }

		M_destination.mesh.units = node.attribute("units").value(); 
		M_destination.spatial_units = node.attribute("units").value(); 

		// if the dataset doesn't specify uniform or regular at all, assume it's very simple (regular Cartesian)
		if( node.attribute("uniform" ) )
		{ M_destination.mesh.uniform_mesh = node.attribute("uniform").as_bool(); }
		else
		{ M_destination.mesh.uniform_mesh = true; } 
		
		if( node.attribute("regular" ) )
		{ M_destination.mesh.regular_mesh = node.attribute("regular").as_bool(); }
		else
		{ M_destination.mesh.regular_mesh = true; }

		// get the bounding box 
		node = node.child( "bounding_box");
		
		// int i=0; // g++ warning: set but not used 
		
		csv_to_vector( node.text().get() , M_destination.mesh.bounding_box ); 
		
		// if Cartesian, try to get the mesh just by reading the x, y, z coordinates 
		if( cartesian == true )
		{
			// bool read_coordinates = true; // g++ warning: set but not used 
			
			// read the x coordinates 
			node = node.parent();
			node = node.child("x_coordinates"); 
			M_destination.mesh.x_coordinates.clear();
			// i=0;// g++ warning: set but not used 
			
			csv_to_vector( node.text().get() , M_destination.mesh.x_coordinates ); 
			
			if( M_destination.mesh.x_coordinates.size() > 1 )
			{ M_destination.mesh.dx = M_destination.mesh.x_coordinates[1] - M_destination.mesh.x_coordinates[0]; }
			else
			{ M_destination.mesh.dx = 1.0; }
		
			// read the y coordinates 
			node = node.parent();
			node = node.child("y_coordinates"); 
			M_destination.mesh.y_coordinates.clear();
			// i=0; // g++ warning: set but not used 
			
			csv_to_vector( node.text().get() , M_destination.mesh.y_coordinates );
			
			if( M_destination.mesh.y_coordinates.size() > 1 )
			{ M_destination.mesh.dy = M_destination.mesh.y_coordinates[1] - M_destination.mesh.y_coordinates[0]; }
			else
			{ M_destination.mesh.dy = 1.0; }

			// read the z coordinates 
			node = node.parent();
			node = node.child("z_coordinates"); 
			M_destination.mesh.z_coordinates.clear();
			// i=0; // g++ warning: set but not used 
			csv_to_vector( node.text().get() , M_destination.mesh.z_coordinates );
			if( M_destination.mesh.z_coordinates.size() > 1 )
			{ M_destination.mesh.dz = M_destination.mesh.z_coordinates[1] - M_destination.mesh.z_coordinates[0]; }
			else
			{ M_destination.mesh.dz = 1.0; }
			
			M_destination.mesh.dV = M_destination.mesh.dx * M_destination.mesh.dy * M_destination.mesh.dz; 
			M_destination.mesh.dS = M_destination.mesh.dx * M_destination.mesh.dy; 
			M_destination.mesh.dS_xy = M_destination.mesh.dx * M_destination.mesh.dy;
			M_destination.mesh.dS_yz = M_destination.mesh.dy * M_destination.mesh.dz;
			M_destination.mesh.dS_xz = M_destination.mesh.dx * M_destination.mesh.dz;
			
			// now, use this mesh information to properly initialize M_destination.mesh
			
			if( M_destination.mesh.x_coordinates.size() < 2 )
			{ M_destination.mesh.dx = M_destination.mesh.bounding_box[3] - M_destination.mesh.bounding_box[0]; } 
			if( M_destination.mesh.y_coordinates.size() < 2 )
			{ M_destination.mesh.dy = M_destination.mesh.bounding_box[4] - M_destination.mesh.bounding_box[1]; } 
			if( M_destination.mesh.z_coordinates.size() < 2 )
			{ M_destination.mesh.dz = M_destination.mesh.bounding_box[5] - M_destination.mesh.bounding_box[2]; } 

			if( M_destination.mesh.regular_mesh || M_destination.mesh.uniform_mesh )
			{
				M_destination.resize_space( M_destination.mesh.bounding_box[0], M_destination.mesh.bounding_box[3], 
					M_destination.mesh.bounding_box[1], M_destination.mesh.bounding_box[4], 
					M_destination.mesh.bounding_box[2], M_destination.mesh.bounding_box[5],
					M_destination.mesh.dx, M_destination.mesh.dy, M_destination.mesh.dz ); 			
			}
			else
			{
				std::cout << "Warning! BioFVM / MultiCellDS cannot currently fully initialize to non-regular Cartesian data!" << std::endl; 
			}
		}
		
		if( cartesian == false || M_destination.mesh.regular_mesh == false )
		{
			// Read in the voxels here and create them.
			// If non-regular Cartesian, create and populate them here, one by one. 
			
			node = node.parent(); // now we're at the mesh level 
			pugi::xml_node node1 = node; // set a "bookmark" at the mesh level 

			node = node.child( "voxels" );  // now we're at the voxels level 
			
			// are the voxels written in matlab format or as xml? 
			// pugi::xml_attribute attrib = node.attribute( "type" ); // g++ warning: set but not used 
			
			if( strcmp(  node.attribute( "type" ).value()  , "matlab" ) == 0 )
			{
				std::cout << "matlab" << std::endl; 				
				M_destination.mesh.Cartesian_mesh = false; 
				M_destination.mesh.uniform_mesh = false; 
				M_destination.mesh.regular_mesh = false; 
				M_destination.mesh.use_voxel_faces = false; 				
				
				// determine the number of voxels 
				unsigned int rows; 
				unsigned int columns; 
				FILE* fp = read_matlab_header( &rows, &columns, node.text().get() ); 
				unsigned int voxel_count = columns; 
				
				// resize the appropriate data structure 
				M_destination.resize_voxels( voxel_count );	

				// read the data directly into the voxels  
				for( unsigned int j=0; j < columns ; j++ )
				{
					// read x, y, z, dV
					result = fread( (char*) & (M_destination.mesh.voxels[j].center[0])   , sizeof(double) , 1 , fp );
					result = fread( (char*) & (M_destination.mesh.voxels[j].center[1])   , sizeof(double) , 1 , fp );
					result = fread( (char*) & (M_destination.mesh.voxels[j].center[2])   , sizeof(double) , 1 , fp );
					result = fread( (char*) & (M_destination.mesh.voxels[j].volume)   , sizeof(double) , 1 , fp );
				} 
				fclose( fp );				
			}
			else
			{
				// Need to set the mesh to non-Cartesian. 
				// We're in very, very basic mode here. 
				
				M_destination.mesh.Cartesian_mesh = false; 
				M_destination.mesh.uniform_mesh = false; 
				M_destination.mesh.regular_mesh = false; 
				M_destination.mesh.use_voxel_faces = false; 
				
				// first, figure out how many voxels. 
				node = node.child( "voxel" ); 
				std::cout << node.name() << std::endl; 
				int voxel_count = 0; 
				while( node )
				{
					voxel_count++; 
					node = node.next_sibling( "voxel" ); 
				}
				
				// now, resize the data structures 
				M_destination.resize_voxels( voxel_count ); 
				
				// now, go back and read in the data 
				node = root; // microenvironment; 
				node = node.child( "mesh" ); 
				node = node.child( "voxels" );
				node = node.child( "voxel"); 
				
				int voxel_index = 0; 
				while( node ) 
				{
					M_destination.mesh.voxels[voxel_index].mesh_index = node.attribute( "ID" ).as_int(); 
					
					// now, get the coordinates 
					node = node.child("center"); 
					csv_to_vector( node.first_child().value() , M_destination.mesh.voxels[voxel_index].center ); 
					node = node.parent(); 
					
					// now, get the volume
					node = node.child( "volume"); 
					M_destination.mesh.voxels[voxel_index].volume = strtod( node.first_child().value() , NULL );  
					node = node.parent(); 

					voxel_index++; 
					node = node.next_sibling( "voxel" ); 
				}	
			}
			
			node = node1; // now we're at the mesh level 
		}
		else
		{
			node = node.parent(); // now we're at the mesh level 
		}	
		
		// if not Cartesian, or if we couldn't read the coordinates, read all the voxels manually 
		
		// after reading the mesh, get the data
		node=node.parent(); // now we're at the microenvironment level 
		node=node.child("densities"); 
		
		// first, read in all the densities names and properties, and resize the data appropriately. 

		node=node.child("density");
		bool added_first_density = false; 
		int substrate_index = 0; 
		while( node )
		{
			if( added_first_density == false )
			{
				M_destination.set_density( 0 , node.attribute("name").value() , node.attribute("units").value() );
				added_first_density = true;
				substrate_index = 0; 
			}
			else
			{
				M_destination.add_density( node.attribute("name").value() , node.attribute("units").value() ); 
				substrate_index++; 
			}
			
			// get the diffusivity 
			node = node.child( "diffusion_constant" ); 
			M_destination.diffusion_coefficients[substrate_index] = strtod(  node.first_child().value() , NULL ); 
			node = node.parent();

			// get the decay rate 
			node = node.child( "decay_rate" ); 
			M_destination.decay_rates[substrate_index] = strtod(  node.first_child().value() , NULL ); 
			node = node.parent(); 
			
			node = node.next_sibling("density");
		}
		
		// lastly, read in all the density data 
		node = root.child( "data" ); 
		
		// read in if stored as matlab 		
		if( strcmp(  node.attribute( "type" ).value()  , "matlab" ) == 0 ) 
		{  
			unsigned int rows; 
			unsigned int columns; 
			FILE* fp = read_matlab_header( &rows, &columns, node.text().get() ); 			
			unsigned int start_row = 0; 
			if( rows > M_destination.number_of_densities() )
			{ start_row = 4; }
			

			// read the data directly into the microenvironment 
			for( unsigned int j=0; j < columns ; j++ )
			{
				double temp; 
				// read x,y,z,dV to a temp (throwaway) variable (if start_row == 4)
				for( unsigned int i=0; i < start_row ; i++ )
				{ result = fread( (char*) &temp , sizeof(double) , 1 , fp ); }

				// now, read the actual data 
				for( unsigned int i=start_row; i < rows ; i++ )
				{ result = fread( (char*) &( M_destination.density_vector(j)[i-start_row] ) , sizeof(double) , 1 , fp ); }
			} 
			
			fclose( fp );
		}	
		else
		{
			// attempt to read it in as XML data, voxel by voxel 
			node = node.child( "density_vector" ); 
			for( unsigned int j=0 ; j < M_destination.mesh.voxels.size() ; j++ )
			{
				csv_to_vector( node.first_child().value() , M_destination.density_vector(j)  ); 
				if( node.next_sibling( "density_vector" ) ) 
				{ node = node.next_sibling( "density_vector" ); }		
			}
			node = node.parent(); 
		}
		root = root.next_sibling(); 
	}		

	std::cout << "done!" << std::endl; 
	return; 
} 

};
