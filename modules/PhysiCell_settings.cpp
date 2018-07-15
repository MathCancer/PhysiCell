/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version 1.3.0) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version 1.3.0) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#    llelized diffusive transport solver for 3-D biological simulations,      #
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730   #
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
 
#include "./PhysiCell_settings.h"

namespace PhysiCell{
	
PhysiCell_Settings PhysiCell_settings; 

bool physicell_config_dom_initialized = false; 
pugi::xml_document physicell_config_doc; 	
pugi::xml_node physicell_config_root; 
	
bool load_PhysiCell_config_file( std::string filename )
{
	std::cout << "Using config file " << filename << " ... " << std::endl ; 
	pugi::xml_parse_result result = physicell_config_doc.load_file( filename.c_str()  );
	
	if( result.status != pugi::xml_parse_status::status_ok )
	{
		std::cout << "Error loading " << filename << "!" << std::endl; 
		return false;
	}
	
	physicell_config_root = physicell_config_doc.child("PhysiCell_settings");
	physicell_config_dom_initialized = true; 
	
	PhysiCell_settings.read_from_pugixml(); 
	
	return true; 	
}

PhysiCell_Settings::PhysiCell_Settings()
{
	// units 
	time_units = "min"; 
	space_units = "micron"; 
	
	// save options
	folder = "."; 
	max_time = 60*24*45;   

	full_save_interval = 60;  
	enable_full_saves = true; 
	enable_legacy_saves = false; 
	
	SVG_save_interval = 60; 
	enable_SVG_saves = true; 
	
	// parallel options 
	
	omp_num_threads = 4; 
	 
	return; 
}
 	
void PhysiCell_Settings::read_from_pugixml( void )
{
	pugi::xml_node node; 
	
	// overall options 
	
	node = xml_find_node( physicell_config_root , "overall" );

	max_time = xml_get_double_value( node , "max_time" );
	time_units = xml_get_string_value( node, "time_units" ) ;
	space_units = xml_get_string_value( node, "space_units" ) ;

	node = node.parent(); 
	
	// save options 
	
	node = xml_find_node( physicell_config_root , "save" ); 
	
	folder = xml_get_string_value( node, "folder" ) ;
	
	node = xml_find_node( node , "full_data" ); 
	full_save_interval = xml_get_double_value( node , "interval" );
	enable_full_saves = xml_get_bool_value( node , "enable" ); 
	node = node.parent(); 
	
	node = xml_find_node( node , "SVG" ); 
	SVG_save_interval = xml_get_double_value( node , "interval" );
	enable_SVG_saves = xml_get_bool_value( node , "enable" ); 
	node = node.parent(); 
	
	node = xml_find_node( node , "legacy_data" ); 
	enable_legacy_saves = xml_get_bool_value( node , "enable" );
	node = node.parent(); 

	// parallel options 
	
	node = xml_find_node( physicell_config_root , "parallel" ); 		
	omp_num_threads = xml_get_int_value( node, "omp_num_threads" ); 

	// random seed options 
		
	
	return; 
}


PhysiCell_Globals PhysiCell_globals; 

 

}; 
 

 
