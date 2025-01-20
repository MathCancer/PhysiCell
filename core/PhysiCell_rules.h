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

#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

#include <typeinfo>

// using namespace BioFVM; 
// using namespace PhysiCell;

#ifndef __PhysiCell_rules_h__
#define __PhysiCell_rules_h__

using namespace BioFVM; 
namespace PhysiCell{

class Hypothesis_Rule
{
 private:
    std::unordered_map<std::string,int> signals_map;  
 public:
    std::string cell_type; 
    Cell_Definition* pCell_Definition; 

    std::string behavior; 
    double base_value; 
    double max_value; 
    double min_value; 

    std::vector< std::string > signals; 
    std::vector<bool> responses; 
    std::vector<double> half_maxes; 
    std::vector<double> hill_powers; 
    std::vector<bool> applies_to_dead_cells; 

    std::vector< std::string > up_signals; 
    std::vector<double> up_half_maxes; 
    std::vector<double> up_hill_powers; 
    std::vector<bool> up_applies_to_dead_cells; 

    std::vector< std::string > down_signals; 
    std::vector<double> down_half_maxes; 
    std::vector<double> down_hill_powers; 
    std::vector<bool> down_applies_to_dead_cells; 

    Hypothesis_Rule(); // done 

    void sync_to_cell_definition( Cell_Definition* pCD ); // done 
    void sync_to_cell_definition( std::string cell_name ); // done 

    void add_signal( std::string signal , double half_max , double hill_power , std::string response ); // done 
    void add_signal( std::string signal , std::string response ); // done 

    double evaluate( std::vector<double> signal_values , bool dead ); // done 
    double evaluate( std::vector<double> signal_values ); // done 
    double evaluate( Cell* pCell ); // done 
    void apply( Cell* pCell ); // done 

    int find_signal( std::string name ); // done 

    void set_half_max( std::string , double hm ); // done 
    void set_hill_power( std::string , double hp ); // done 
    void set_response( std::string , std::string response ); // done 

    void reduced_display( std::ostream& os ); // done 
    void display( std::ostream& os ); // done 
    void detailed_display( std::ostream& os ); // done 

    void English_display( std::ostream& os ); 
    void English_display_HTML( std::ostream& os ); 
    void English_detailed_display( std::ostream& os ); 
    void English_detailed_display_HTML( std::ostream& os ); 
}; 

class Hypothesis_Ruleset
{
 private:
    std::unordered_map<std::string,Hypothesis_Rule*> rules_map;  
 public:
    std::string cell_type; 
    Cell_Definition* pCell_Definition; 

    std::vector< Hypothesis_Rule* > rules; 

    Hypothesis_Ruleset(); // done 

    Hypothesis_Rule* add_behavior( std::string behavior , double min_behavior, double max_behavior ); // done 
    Hypothesis_Rule* add_behavior( std::string behavior ); // done 

    // ease of access functions 

    Hypothesis_Rule* find_behavior( std::string name ); // done 
    Hypothesis_Rule& operator[]( std::string name ); // done 

    void apply( Cell* pCell ); 

    void sync_to_cell_definition( Cell_Definition* pCD ); // done 
    void sync_to_cell_definition( std::string cell_name ); // done 

    void display( std::ostream& os ); // done 
    void detailed_display( std::ostream& os ); // done 
}; 

// access 

Hypothesis_Ruleset& access_ruleset( Cell_Definition* pCD ); 
Hypothesis_Ruleset* find_ruleset( Cell_Definition* pCD ); 

// initializing for all cell definitions 

void intialize_hypothesis_rulesets( void ); 

// adding and editing rules (easy eccess)

void add_rule( std::string cell_type, std::string signal, std::string behavior , std::string response ); 
void add_rule( std::string cell_type, std::string signal, std::string behavior , std::string response , bool use_for_dead ); 

void set_hypothesis_parameters(std::string cell_type, std::string signal, std::string behavior , 
   double half_max, double hill_power ); 
void set_behavior_parameters( std::string cell_type, std::string behavior, 
   double min_value, double max_value ); 
void set_behavior_parameters( std::string cell_type, std::string behavior, 
   double min_value, double base_value , double max_value ); 

void set_behavior_base_value( std::string cell_type, std::string behavior, double base_value ); 
void set_behavior_min_value( std::string cell_type, std::string behavior, double min_value ); 
void set_behavior_max_value( std::string cell_type, std::string behavior, double max_value ); 

// display 

void display_hypothesis_rulesets( std::ostream& os ); 
void detailed_display_hypothesis_rulesets( std::ostream& os ); 

// applying to a cell 

void apply_ruleset( Cell* pCell ); 
void rule_phenotype_function( Cell* pCell, Phenotype& phenotype, double dt ); 


// parsing to / from CSV 

// split a line of a CSV into a vector of strings based on the delimiter 

void split_csv( std::string input , std::vector<std::string>& output , char delim );
void spaces_to_underscore( std::string& str );
std::string convert_bool_to_response( bool input ); 

void parse_csv_rule_v0( std::vector<std::string> input ); // parse a tokenized string (vector of strings)
void parse_csv_rule_v0( std::string input ); // parse a single string (a single line from CSV)
void parse_csv_rules_v0( std::string filename ); // parse all rules in a CSV file 

void parse_csv_rule_v1( std::vector<std::string> input ); // parse a tokenized string (vector of strings)
void parse_csv_rule_v1( std::string input ); // parse a single string (a single line from CSV)
void parse_csv_rules_v1( std::string filename ); // parse all rules in a CSV file 

void parse_csv_rule_v3( std::vector<std::string> input ); // parse a tokenized string (vector of strings)
void parse_csv_rule_v3( std::string input ); // parse a single string (a single line from CSV)
void parse_csv_rules_v3( std::string filename ); // parse all rules in a CSV file 

void parse_rules_from_pugixml( void );

// needs fixing March 2023 // probably deprecate
void parse_rules_from_parameters_v0( void ); 

std::string csv_strings_to_English( std::vector<std::string> strings , bool include_cell_header ); 
std::string csv_strings_to_English_v1( std::vector<std::string> strings , bool include_cell_header );
std::string csv_strings_to_English_v3( std::vector<std::string> strings , bool include_cell_header );

std::string csv_strings_to_English_HTML( std::vector<std::string> strings , bool include_cell_header ); 

// v1, v2, and v0? 
void export_rules_csv_v0( std::string filename ); 
void export_rules_csv_v1( std::string filename ); 
void export_rules_csv_v3( std::string filename ); 

// streamed outputs in human-readable format

void stream_annotated_English_rules( std::ostream& os ); 
void stream_annotated_detailed_English_rules( std::ostream& os ); 
void save_annotated_English_rules( void );
void save_annotated_English_rules_HTML( void );

void stream_annotated_English_rules_HTML( std::ostream& os ); 
void stream_annotated_detailed_English_rules_HTML( std::ostream& os ); 
void save_annotated_detailed_English_rules( void ); 
void save_annotated_detailed_English_rules_HTML( void ); 

// add these to PhysiCell_utilities.cpp 

std::vector<double> UniformInUnitDisc( void );
std::vector<double> UniformInUnitSphere( void );

std::vector<double> UniformInAnnulus( double r1, double r2 );
std::vector<double> UniformInShell( double r1, double r2 );

// add this to cell behaviors 

// double get_single_base_behavior( Cell_Definition* pCD , std::string name ); 


// add these to basic signaling 

/*
double multivariate_Hill_response_function( std::vector<double> signals, std::vector<double> half_maxes , std::vector<double> hill_powers ); 

double multivariate_linear_response_function( std::vector<double> signals, std::vector<double> min_thresholds , std::vector<double> max_thresholds ); 

std::vector<double> linear_response_to_Hill_parameters( double s0, double s1 ); 
std::vector<double> Hill_response_to_linear_parameters( double half_max , double Hill_power ); 
*/

void setup_cell_rules( void );

}; 

#endif 
