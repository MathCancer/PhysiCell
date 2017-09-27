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

#ifndef __BioFVM_MultiCellDS_h__
#define __BioFVM_MultiCellDS_h__

#include "pugixml.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime> 
#include <cstring>
#include <vector>
#include <fstream> 

namespace BioFVM{
extern std::string MultiCellDS_version_string; 

extern std::string MultiCellDS_clinical_snapshot_type_string;  
extern int MultiCellDS_clinical_snapshot_code; 

extern std::string MultiCellDS_experimental_snapshot_type_string; 
extern int MultiCellDS_experimental_snapshot_code; 

extern std::string MultiCellDS_simulation_snapshot_type_string; 
extern int MultiCellDS_simulation_snapshot_code; 

extern std::string MultiCellDS_digital_cell_line_type_string; 
extern int MultiCellDS_digital_cell_line_code; 

/* options */ 

extern bool save_mesh_as_matlab; 
extern bool save_density_data_as_matlab;
extern bool save_cells_as_custom_matlab; 
extern bool save_cell_data; 


class Microenvironment; 

extern pugi::xml_document biofvm_doc; 

class Person_Metadata
{
	private: 
		bool is_empty; 
	public: 
		std::string type; // author, creator, user, curator 
		std::string surname;
		std::string given_names; 
		std::string email;
		std::string URL; 
		std::string organization; 
		std::string department; 
		std::string ORCID; 
		
		Person_Metadata( ); 
		void display_information( std::ostream& os );  		
		void insert_in_open_xml_pugi( pugi::xml_node& insert_here );  
};

class Citation_Metadata
{
	private:
	public:
		std::string DOI;
		std::string PMID;
		std::string PMCID; 
		std::string text;
		std::string notes; 
		std::string URL; 
		
		Citation_Metadata();  
		void display_information( std::ostream& os );  
		void insert_in_open_xml_pugi( pugi::xml_node& insert_here );    
};

class Software_Metadata
{
	private:
	public:
		// basic program information   
		std::string program_name; 
		std::string program_version; 
		std::string program_URL;
			
		Person_Metadata creator; 
		Person_Metadata user; 
		Citation_Metadata citation;
		
		Software_Metadata();
		
		void display_information( std::ostream& os ); 
		void insert_in_open_xml_pugi( pugi::xml_node& insert_here ); 		
};

class MultiCellDS_Metadata
{
	private:
	public:
		std::string MultiCellDS_type; 
	
		Software_Metadata program; 
		Citation_Metadata data_citation; 

		// scientific information  
		std::string spatial_units; 
		std::string time_units;
		std::string runtime_units; 
		double current_time; 
		double current_runtime; 
		
		std::string description; // any optional text -- not implemented 
	
		MultiCellDS_Metadata();	
		void display_information( std::ostream& os); 
		void sync_to_microenvironment( Microenvironment& M );  
		void restart_runtime( void );  
	
		void add_to_open_xml_pugi( double current_simulation_time, pugi::xml_document& xml_dom ); 
};

extern MultiCellDS_Metadata BioFVM_metadata; 

/* setting up the main MultiCellDS tree structure */ 

void add_MultiCellDS_main_structure_to_open_xml_pugi( pugi::xml_document& xml_dom ); 

/* set options */ 

void set_save_biofvm_mesh_as_matlab( bool newvalue ); // default: true
void set_save_biofvm_data_as_matlab( bool newvalue ); // default: true 
void set_save_biofvm_cell_data( bool newvalue ); // default: true
void set_save_biofvm_cell_data_as_custom_matlab( bool newvalue ); // default: true

/* writing parts of BioFVM to a MultiCellDS file */ 

void add_BioFVM_substrates_to_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base , Microenvironment& M ); 
void add_BioFVM_basic_agent_to_open_xml_pugi(  pugi::xml_document& xml_dom, Basic_Agent& BA ); // not implemented -- future edition 
void add_BioFVM_agents_to_open_xml_pugi( pugi::xml_document& xml_dom, std::string filename_base, Microenvironment& M  ); 
void add_BioFVM_to_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, double current_simulation_time , Microenvironment& M );

void save_BioFVM_to_MultiCellDS_xml_pugi( std::string filename_base , Microenvironment& M , double current_simulation_time); 

/* future / not yet supported */

void read_BioFVM_from_open_xml_pugi( pugi::xml_document& xml_dom , std::string filename_base, double& current_simulation_time , Microenvironment& M );
void read_BioFVM_to_MultiCellDS_xml_pugi( std::string filename_base , Microenvironment& M , double& current_simulation_time ); 

/* partly-implemented code snippets -- not to be used as of March 2016 */

// functions to read multiscale_microenvironment from MultiCellDS file (requires pugixml)
void read_microenvironment_from_MultiCellDS_xml( Microenvironment& M_destination , std::string filename );
void read_microenvironment_from_MultiCellDS_xml( Microenvironment& M_destination , pugi::xml_document& xml_dom ); 

};

#endif 
