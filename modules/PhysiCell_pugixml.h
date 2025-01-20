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
# Copyright (c) 2015-2025, Paul Macklin and the PhysiCell Project             #
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

#ifndef __PhysiCell_pugixml_h__
#define __PhysiCell_pugixml_h__

#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <chrono>

#include "../BioFVM/pugixml.hpp"

namespace PhysiCell{
	
// find the first <find_me> child in <parent_node> 
pugi::xml_node xml_find_node( pugi::xml_node& parent_node , std::string find_me ); // done 

// get the std:string in <parent_node> <find_me>string_value</find_me> </parent_node> 
std::string xml_get_string_value( pugi::xml_node& parent_node , std::string find_me ); // done 

// get the double value stored in <parent_node> <find_me>double_value</find_me> </parent_node> 
double xml_get_double_value( pugi::xml_node& parent_node , std::string find_me ); // done 

// get the integer value in <parent_node> <find_me>int_value</find_me> </parent_node> 
int xml_get_int_value( pugi::xml_node& parent_node , std::string find_me ); // done 

// get the Boolean value in <parent_node> <find_me>int_value</find_me> </parent_node> 
bool xml_get_bool_value( pugi::xml_node& parent_node , std::string find_me );// done 


// get the name of the element in <my_node> (the name would be my_node) 
std::string xml_get_my_name( pugi::xml_node node ); 


bool xml_get_my_bool_value( pugi::xml_node node ); 
int xml_get_my_int_value( pugi::xml_node node ); 
double xml_get_my_double_value( pugi::xml_node node ); 
std::string xml_get_my_string_value( pugi::xml_node node ); 





// get the string attribute named "attribute" in the first std:string in <parent_node> <find_me>string_value</find_me> </parent_node> 
std::string get_string_attribute_value( pugi::xml_node& parent_node , std::string find_me , std::string attribute ); 

// get the int attribute named "attribute" in the first std:string in <parent_node> <find_me>string_value</find_me> </parent_node> 
int get_int_attribute_value( pugi::xml_node& parent_node , std::string find_me , std::string attribute ); 

// get the double attribute named "attribute" in the first std:string in <parent_node> <find_me>string_value</find_me> </parent_node> 
double get_double_attribute_value( pugi::xml_node& parent_node , std::string find_me , std::string attribute ); 

};

#endif 

