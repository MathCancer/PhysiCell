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
# Copyright (c) 2015-2023, Paul Macklin and the PhysiCell Project             #
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
 
#include <vector>
#include <string>

#ifndef __PhysiCell_signal_response__
#define __PhysiCell_signal_response__

#include "./PhysiCell_constants.h" 
#include "./PhysiCell_phenotype.h" 
#include "./PhysiCell_cell.h" 

namespace PhysiCell{

// scales for the signals 
extern std::vector<double> signal_scales; 
// easy access to get or set scales 
double& signal_scale( std::string signal_name ); // done 
double& signal_scale( int signal_index ); // done 

// create the signal and behavior dictionaries 
void setup_signal_behavior_dictionaries( void ); // done 

// display dictionaries 
void display_signal_dictionary( void ); // done 
void display_behavior_dictionary( void ); // done 

void display_signal_dictionary( std::ostream& os ); // done 
void display_behavior_dictionary( std::ostream& os ); // done 

void display_signal_dictionary_with_synonyms( void ); // done 
void display_behavior_dictionary_with_synonyms( void ); // done 

/* signal functions */ 

// find index for named signal (returns -1 if not found)
int find_signal_index( std::string signal_name ); // done 

// coming soon: 
std::vector<int> find_signal_indices( std::vector<std::string> signal_names ); // done 

// get the name of a signal index 
std::string signal_name( int i ); // done 

// create a full signal vector 
std::vector<double> get_signals( Cell* pCell ); // done 

// create a signal vector of only the cell contacts 
std::vector<double> get_cell_contact_signals( Cell* pCell ); // done 

// create a subset of the signal vector with the supplied indicies 
std::vector<double> get_selected_signals( Cell* pCell , std::vector<int> indices ); // done 
std::vector<double> get_selected_signals( Cell* pCell , std::vector<std::string> names );  // done 

// grab a single signal by its index or name 
double get_single_signal( Cell* pCell, int index ); // done 
double get_single_signal( Cell* pCell, std::string name ); // done 

/* behavior functions */ 

// find index for named behavior / response / parameter (returns -1 if not found)
int find_parameter_index( std::string response_name ); // done
int find_behavior_index( std::string response_name ); // done 

std::vector<int> find_behavior_indices( std::vector<std::string> behavior_names ); // done 

// get the name of a behavior index 
std::string behavior_name( int i ); // done 

// make a properly sized behavior vector 
std::vector<double> create_empty_behavior_vector(); // done 

// write a full behavior vector (phenotype parameters) to the cell 
void set_behaviors( Cell* pCell , std::vector<double> parameters ); // done 

// write a selected set of behavior parameters to the cell 
void set_selected_behaviors( Cell* pCell , std::vector<int> indices , std::vector<double> parameters ); // done 
void set_selected_behaviors( Cell* pCell , std::vector<std::string> names , std::vector<double> parameters ); // done 

// write a single behavior parameter 
void set_single_behavior( Cell* pCell, int index , double parameter ); // done  
void set_single_behavior( Cell* pCell, std::string name , double parameter ); // done 

/* get current behaviors */ 

// get all current behavior
std::vector<double> get_behaviors( Cell* pCell ); // done 

// get selected current behavior
std::vector<double> get_behaviors( Cell* pCell , std::vector<int> indices ); // doen 
std::vector<double> get_behaviors( Cell* pCell , std::vector<std::string> names ); // done 

// get single current behavior 
double get_single_behavior( Cell* pCell , int index ); // done 
double get_single_behavior( Cell* pCell , std::string name ); // done 

/* get base behaviors (from cell definition) */ 

// get all base behaviors (from cell's definition) 
std::vector<double> get_base_behaviors( Cell* pCell );  // done 

// get selected base behaviors (from cell's definition)
std::vector<double> get_base_behaviors( Cell* pCell , std::vector<int> indices ); // done 
std::vector<double> get_base_behaviors( Cell* pCell , std::vector<std::string> names ); // done 

// get single base behavior (from cell's definition)
double get_single_base_behavior( Cell* pCell , int index ); // done 
double get_single_base_behavior( Cell* pCell , std::string name ); // done 

double get_single_base_behavior( Cell_Definition* pCD , std::string name ); 


}; 

#endif 
