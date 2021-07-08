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

#ifndef __PhysiCell_constants_h__
#define __PhysiCell_constants_h__

#include <string>
#include <unordered_map>

namespace PhysiCell
{
	
// class PhysiCell_constants
namespace PhysiCell_constants
{
//  public:
	extern const double pi;
	
	extern const double cell_removal_threshold_volume; // 20 cubic microns -- about 1% of typical cell 
	extern const int keep_pushed_out_cells_in_outer_voxel;
	extern const int solid_boundary;
	extern const int default_boundary_condition_for_pushed_out_agents;		
	
	extern const int deterministic_necrosis;
	extern const int stochastic_necrosis;
	
	extern const int mesh_min_x_index;
	extern const int mesh_min_y_index;
	extern const int mesh_min_z_index;
	extern const int mesh_max_x_index;
	extern const int mesh_max_y_index;
	extern const int mesh_max_z_index;			
	
	extern const int mesh_lx_face_index;
	extern const int mesh_ly_face_index;
	extern const int mesh_lz_face_index;
	extern const int mesh_ux_face_index;
	extern const int mesh_uy_face_index;
	extern const int mesh_uz_face_index;
	
	// currently recognized cell cycle models 
	extern const int advanced_Ki67_cycle_model;
	extern const int basic_Ki67_cycle_model;
	extern const int flow_cytometry_cycle_model;
	extern const int live_apoptotic_cycle_model;
	extern const int total_cells_cycle_model;
	extern const int live_cells_cycle_model; 
	extern const int flow_cytometry_separated_cycle_model; 
	extern const int cycling_quiescent_model; 
	
	// currently recognized death models 
	extern const int apoptosis_death_model; 
	extern const int necrosis_death_model; 
	extern const int autophagy_death_model; 
	
	extern const int custom_cycle_model; 
	
	// currently recognized cell cycle and death phases 
	// cycle phases
	extern const int Ki67_positive_premitotic; 
	extern const int Ki67_positive_postmitotic; 
	extern const int Ki67_positive; 
	extern const int Ki67_negative; 
	extern const int G0G1_phase;
	extern const int G0_phase;
	extern const int G1_phase; 
	extern const int G1a_phase; 
	extern const int G1b_phase;
	extern const int G1c_phase;
	extern const int S_phase;
	extern const int G2M_phase;
	extern const int G2_phase;
	extern const int M_phase;
	extern const int live;
	
	extern const int G1pm_phase;
	extern const int G1ps_phase; 
	
	extern const int cycling; 
	extern const int quiescent; 
	
	
	extern const int custom_phase;
	// death phases
	extern const int apoptotic;
	extern const int necrotic_swelling;
	extern const int necrotic_lysed;
	extern const int necrotic; 
	extern const int debris; 
}
extern std::string time_units;
extern std::string space_units;
extern double diffusion_dt; 
extern double mechanics_dt;
extern double phenotype_dt;


extern std::unordered_map<std::string,int> cycle_model_codes;
int find_cycle_model_code( std::string model_name ); 

};

#endif
