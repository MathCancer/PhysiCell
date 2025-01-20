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

#ifndef __PhysiCell_geometry_h__
#define __PhysiCell_geometry_h__

#include <string>
#include <vector>
#include <sstream>

#include "../core/PhysiCell.h"
#include "./PhysiCell_settings.h"


namespace PhysiCell
{
// loaders 
	
void load_cells_csv_v1( std::string filename ); // done 


std::vector<std::string> split_csv_labels( std::string labels_line ); // done 
Cell* process_csv_v2_line( std::string line , std::vector<std::string> labels ); // done 
void load_cells_csv_v2( std::string filename ); // done 


void load_cells_csv( std::string filename ); 



void load_cells_mat( std::string filename ); 
void load_cells_physicell( std::string filename ); 

bool load_cells_from_pugixml( pugi::xml_node root ); 
bool load_cells_from_pugixml( void ); // load cells based on default config XML root

void set_parameters_from_distributions( const pugi::xml_node root );
void set_parameters_from_distributions(void);
void set_distributed_parameters(pugi::xml_node node, Cell_Definition *pCD);
void set_distributed_parameter(pugi::xml_node node_dist, Cell_Definition *pCD);
void set_distributed_parameter(Cell_Definition *pCD, std::string behavior, std::string type, pugi::xml_node node_dist);

void get_log_normal_bounds(pugi::xml_node node_dist, std::string behavior, Cell_Definition *pCD, double &lb, double &ub, double base_value, bool check_base);
void set_distributed_parameter(Cell* pCell, std::string behavior, double val);
void print_drawing_expectations(double mu, double sigma, double lb, double ub, int n);

bool is_in(std::string x, std::vector<std::string> A);
bool strcmpi(std::string x, std::string y);

//	
// 2D functions 
//
void fill_circle( std::vector<double> center , double radius , Cell_Definition* pCD , double compression ); 
void fill_circle( std::vector<double> center , double radius , Cell_Definition* pCD ); 

void fill_circle( std::vector<double> center , double radius , int cell_type , double compression );
void fill_circle( std::vector<double> center , double radius , int cell_type ); 


void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, Cell_Definition* pCD , double compression ); 
void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, Cell_Definition* pCD ); 

void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, int cell_type , double compression );
void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, int cell_type ); 


// bounds = { xmin, ymin, zmin, xmax, ymax, zmax } 
void fill_rectangle( std::vector<double> bounds , Cell_Definition* pCD , double compression ); 
void fill_rectangle( std::vector<double> bounds , Cell_Definition* pCD ); 

void fill_rectangle( std::vector<double> bounds , int cell_type , double compression );  
void fill_rectangle( std::vector<double> bounds , int cell_type ); 


//
// 3D functions
//
void fill_sphere( std::vector<double> center , double radius , Cell_Definition* pCD , double compression ); 
void fill_sphere( std::vector<double> center , double radius , Cell_Definition* pCD ); 

void fill_sphere( std::vector<double> center , double radius , int cell_type , double compression ); 
void fill_sphere( std::vector<double> center , double radius , int cell_type ); 

// bounds = { xmin, ymin, zmin, xmax, ymax, zmax } 
void fill_box( std::vector<double> bounds , Cell_Definition* pCD , double compression ); 
void fill_box( std::vector<double> bounds , Cell_Definition* pCD ); 

void fill_box( std::vector<double> bounds , int cell_type , double compression ); 
void fill_box( std::vector<double> bounds , int cell_type ); 

void draw_line( std::vector<double> start , std::vector<double> end , Cell_Definition* pCD , double compression ); 
void draw_line( std::vector<double> start , std::vector<double> end , Cell_Definition* pCD ); 

void draw_line( std::vector<double> start , std::vector<double> end , int cell_type , double compression ); 
void draw_line( std::vector<double> start , std::vector<double> end , int cell_type ); 



};

#endif
