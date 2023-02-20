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
# Copyright (c) 2015-2021, Paul Macklin and the PhysiCell Project             #
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

#ifndef __PhysiCell_pathology__
#define __PhysiCell_pathology__

#include "../core/PhysiCell.h"

#include "./PhysiCell_SVG.h"
#include "../BioFVM/BioFVM_utilities.h"

namespace PhysiCell{
	
struct PhysiCell_SVG_options_struct {
	bool plot_nuclei = true; 

	std::string simulation_time_units = "min";
	std::string mu = "&#956;";
	std::string simulation_space_units = "&#956;m";
	
	std::string label_time_units = "days"; 
	
	double font_size = 200; 
	std::string font_color = "black";
	std::string font = "Arial";

	double length_bar = 100; 
}; 

extern PhysiCell_SVG_options_struct PhysiCell_SVG_options;

// done 
std::vector<double> transmission( std::vector<double>& incoming_light, std::vector<double>& absorb_color, double thickness , double stain );

// these give (in order) the cytoplasm color, cytoplasm outline color, nuclear color, nuclear outline color, 
// each string is either rgb(R,G,B) or none 

std::vector<std::string> simple_cell_coloring( Cell* pCell ); // done 
std::vector<std::string> false_cell_coloring_Ki67( Cell* pCell ); // done 
std::vector<std::string> false_cell_coloring_live_dead( Cell* pCell ); // done 

std::vector<std::string> false_cell_coloring_cycling_quiescent( Cell* pCell ); // done 

std::vector<std::string> false_cell_coloring_cytometry( Cell* pCell ); 

std::vector<std::string> hematoxylin_and_eosin_cell_coloring( Cell* pCell ); // done 
std::vector<std::string> hematoxylin_and_eosin_stroma_coloring( double& ECM_fraction , double& blood_vessel_fraction); // planned 

std::vector<std::string> paint_by_number_cell_coloring( Cell* pCell ); // done 

std::string formatted_minutes_to_DDHHMM( double minutes ); 

void SVG_plot( std::string filename , Microenvironment& M, double z_slice , double time, std::vector<std::string> (*cell_coloring_function)(Cell*) ); // done

void SVG_plot_with_stroma( std::string filename , Microenvironment& M, double z_slice , double time, std::vector<std::string> (*cell_coloring_function)(Cell*) , 
	int ECM_index, std::vector<std::string> (*ECM_coloring_function)(double) ); // planned
	
void create_plot_legend( std::string filename , std::vector<std::string> (*cell_coloring_function)(Cell*) ); 

};

#endif
