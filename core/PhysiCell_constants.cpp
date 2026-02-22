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

#include "PhysiCell_constants.h" 

namespace PhysiCell{

std::string time_units = "min";
std::string space_units = "micron";
double diffusion_dt = 0.01; 
double mechanics_dt = 0.1;
double phenotype_dt = 6.0;
double intracellular_dt = 0.01;


// currently recognized cell cycle models 
const int PhysiCell_constants::advanced_Ki67_cycle_model= 0;
const int PhysiCell_constants::basic_Ki67_cycle_model=1;
const int PhysiCell_constants::flow_cytometry_cycle_model=2;
const int PhysiCell_constants::live_apoptotic_cycle_model=3;
const int PhysiCell_constants::total_cells_cycle_model=4;
const int PhysiCell_constants::live_cells_cycle_model = 5; 
const int PhysiCell_constants::flow_cytometry_separated_cycle_model = 6; 
const int PhysiCell_constants::cycling_quiescent_model = 7; 

// currently recognized death models 
const int PhysiCell_constants::apoptosis_death_model = 100; 
const int PhysiCell_constants::necrosis_death_model = 101; 
const int PhysiCell_constants::autophagy_death_model = 102; 

const int PhysiCell_constants::custom_cycle_model=9999; 

// currently recognized cell cycle and death phases 
// cycle phases
const int PhysiCell_constants::Ki67_positive_premitotic=0; 
const int PhysiCell_constants::Ki67_positive_postmitotic=1; 
const int PhysiCell_constants::Ki67_positive=2; 
const int PhysiCell_constants::Ki67_negative=3; 
const int PhysiCell_constants::G0G1_phase=4;
const int PhysiCell_constants::G0_phase=5;
const int PhysiCell_constants::G1_phase=6; 
const int PhysiCell_constants::G1a_phase=7; 
const int PhysiCell_constants::G1b_phase=8;
const int PhysiCell_constants::G1c_phase=9;
const int PhysiCell_constants::S_phase=10;
const int PhysiCell_constants::G2M_phase=11;
const int PhysiCell_constants::G2_phase=12;
const int PhysiCell_constants::M_phase=13;
const int PhysiCell_constants::live=14;

const int PhysiCell_constants::G1pm_phase = 15;
const int PhysiCell_constants::G1ps_phase = 16; 

const int PhysiCell_constants::cycling = 17; 
const int PhysiCell_constants::quiescent = 18; 


const int PhysiCell_constants::custom_phase = 9999;
// death phases
const int PhysiCell_constants::apoptotic=100;
const int PhysiCell_constants::necrotic_swelling=101;
const int PhysiCell_constants::necrotic_lysed=102;
const int PhysiCell_constants::necrotic=103; 
const int PhysiCell_constants::debris=104; 

std::unordered_map<std::string,int> cycle_model_codes = {

    { "Ki67 (advanced)", PhysiCell_constants::advanced_Ki67_cycle_model}, 
    { "Ki67 (basic)" ,PhysiCell_constants::basic_Ki67_cycle_model},
    { "Flow cytometry model (basic)",PhysiCell_constants::flow_cytometry_cycle_model},
// { ,PhysiCell_constants::live_apoptotic_cycle_model}, // not implemented 
	// { ,PhysiCell_constants::total_cells_cycle_model}, // not implemented 
	{ "Live",PhysiCell_constants::live_cells_cycle_model}, 
	{ "Flow cytometry model (separated)",PhysiCell_constants::flow_cytometry_separated_cycle_model}, 
	{ "Cycling-Quiescent model",PhysiCell_constants::cycling_quiescent_model}, 
	
	// currently recognized death models 
	{ "Apoptosis",PhysiCell_constants::apoptosis_death_model}, 
	{ "Necrosis",PhysiCell_constants::necrosis_death_model} , 
	// { ,PhysiCell_constants::autophagy_death_model}, // not implemented 
	
	{ "ki67 (advanced)", PhysiCell_constants::advanced_Ki67_cycle_model}, 
	{ "ki67 (basic)" ,PhysiCell_constants::basic_Ki67_cycle_model},
	{ "flow cytometry model (basic)",PhysiCell_constants::flow_cytometry_cycle_model},
	{ "live",PhysiCell_constants::live_cells_cycle_model}, 
	{ "flow cytometry model (separated)",PhysiCell_constants::flow_cytometry_separated_cycle_model}, 
	{ "cycling-quiescent model",PhysiCell_constants::cycling_quiescent_model}, 
	{ "apoptosis",PhysiCell_constants::apoptosis_death_model}, 
	{ "necrosis",PhysiCell_constants::necrosis_death_model} 
	
}; 

int find_cycle_model_code( std::string model_name )
{
	auto search = cycle_model_codes.find( model_name );
	if( search == cycle_model_codes.end() )
	{ return -1; }
	else
	{ return search->second; }
}

};
