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

#ifndef __PhysiCell_settings_h__
#define __PhysiCell_settings_h__

#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>

#include "./PhysiCell_pugixml.h"
#include "../BioFVM/BioFVM.h"

#include "../core/PhysiCell_constants.h" 
#include "../core/PhysiCell_utilities.h"

using namespace BioFVM; 

namespace PhysiCell{
 	
extern pugi::xml_node physicell_config_root; 

bool load_PhysiCell_config_file( std::string filename );

class PhysiCell_Settings
{
 private:
 public:
	// overall 
	double max_time = 60*24*45;   

	// units
	std::string time_units = "min"; 
	std::string space_units = "micron"; 
 
	// parallel options 
	int omp_num_threads = 2; 
	
	// save options
	std::string folder = "."; 

	double full_save_interval = 60;  
	bool enable_full_saves = true; 
	bool enable_legacy_saves = false; 
	
	double SVG_save_interval = 60; 
	bool enable_SVG_saves = true; 
	
	PhysiCell_Settings();
	
	void read_from_pugixml( void ); 
};

class PhysiCell_Globals
{
 private:
 public:
	double current_time = 0.0; 
	double next_full_save_time = 0.0; 
	double next_SVG_save_time = 0.0; 
	int full_output_index = 0; 
	int SVG_output_index = 0; 
};

template <class T> 
class Parameter
{
 private:
	template <class Y>
	friend std::ostream& operator<<(std::ostream& os, const Parameter<Y>& param); 

 public: 
	std::string name; 
	std::string units; 
	T value; 
	
	Parameter();
	Parameter( std::string my_name ); 
	
	void operator=( T& rhs ); 
	void operator=( T rhs ); 
	void operator=( Parameter& p ); 
};

template <class T>
class Parameters
{
 private:
	std::unordered_map<std::string,int> name_to_index_map; 
	
	template <class Y>
	friend std::ostream& operator<<( std::ostream& os , const Parameters<Y>& params ); 

 public: 
	Parameters(); 
 
	std::vector< Parameter<T> > parameters; 
	
	void add_parameter( std::string my_name ); 
	void add_parameter( std::string my_name , T my_value ); 
//	void add_parameter( std::string my_name , T my_value ); 
	void add_parameter( std::string my_name , T my_value , std::string my_units ); 
//	void add_parameter( std::string my_name , T my_value , std::string my_units ); 
	
	void add_parameter( Parameter<T> param );
	
	int find_index( std::string search_name ); 
	
	// these access the values 
	T& operator()( int i );
	T& operator()( std::string str ); 

	// these access the full, raw parameters 
	Parameter<T>& operator[]( int i );
	Parameter<T>& operator[]( std::string str ); 
	
	int size( void ) const; 
};

class User_Parameters
{
 private:
	friend std::ostream& operator<<( std::ostream& os , const User_Parameters up ); 
 
 public:
	Parameters<bool> bools; 
	Parameters<int> ints; 
	Parameters<double> doubles; 
	Parameters<std::string> strings; 
	
	void read_from_pugixml( pugi::xml_node parent_node );
}; 

extern PhysiCell_Globals PhysiCell_globals; 

extern PhysiCell_Settings PhysiCell_settings; 

extern User_Parameters parameters; 

bool setup_microenvironment_from_XML( pugi::xml_node root_node );
bool setup_microenvironment_from_XML( void );

}

#endif 

