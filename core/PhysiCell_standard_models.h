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

#ifndef __PhysiCell_standard_models_h__
#define __PhysiCell_standard_models_h__

#include "./PhysiCell_constants.h" 
#include "./PhysiCell_phenotype.h" 

namespace PhysiCell
{

// standard cycle models: 

extern Cycle_Model Ki67_advanced, Ki67_basic, live, flow_cytometry_cycle_model, flow_cytometry_separated_cycle_model, cycling_quiescent; 
extern Cycle_Model apoptosis, necrosis; 
extern Death_Parameters apoptosis_parameters, necrosis_parameters; 

extern bool PhysiCell_standard_models_initialized; 
extern bool PhysiCell_standard_death_models_initialized; 
extern bool PhysiCell_standard_cycle_models_initialized; 

// standard entry function for the cycle models 

void standard_Ki67_positive_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 
void standard_Ki67_negative_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 
void standard_live_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 

void G1_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); 
void G0_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); 
void S_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 

void standard_apoptosis_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 
void standard_necrosis_entry_function( Cell* pCell, Phenotype& phenotype, double dt );  // done 
void standard_lysis_entry_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 

bool standard_necrosis_arrest_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 

// standard volume functions 

void standard_volume_update_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 
void basic_volume_model( Cell* pCell, Phenotype& phenotype, double dt ); // done 

// standard mechanics functions 

void standard_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt); // done 
void standard_add_basement_membrane_interactions( Cell* pCell, Phenotype& phenotype, double dt );

// bounary avoidance functions 

void standard_domain_edge_avoidance_interactions( Cell* pCell, Phenotype& phenotype, double dt ); 
double distance_to_domain_edge(Cell* pCell, Phenotype& phenotype, double dt); 

// other standard functions 

void empty_function( Cell* pCell, Phenotype& phenotype, double dt ); // done 
void up_orientation( Cell* pCell, Phenotype& phenotype, double dt ); // done

// standard o2-based phenotype changes 

void update_cell_and_death_parameters_O2_based( Cell* pCell, Phenotype& phenotype, double dt ); 

// create standard models 

bool create_standard_cell_cycle_models( void ); // done 
bool create_standard_cell_death_models( void ); // done 
bool create_standard_cycle_and_death_models( void ); // done 

void initialize_default_cell_definition( void ); // done 

void chemotaxis_function( Cell* pCell, Phenotype& phenotype , double dt ); 

void standard_elastic_contact_function( Cell* pC1, Phenotype& p1, Cell* pC2, Phenotype& p2 , double dt );
void standard_elastic_contact_function_confluent_rest_length( Cell* pC1, Phenotype& p1, Cell* pC2, Phenotype& p2 , double dt );
void evaluate_interactions( Cell* pCell, Phenotype& phenotype, double dt );

// new in 1.10.0 
	
// automated cell phagocytosis, attack, and fusion 
void standard_cell_cell_interactions( Cell* pCell, Phenotype& phenotype, double dt ); 
void standard_cell_transformations( Cell* pCell, Phenotype& phenotype, double dt ); 
void standard_asymmetric_division_function( Cell* pCell_parent, Cell* pCell_daughter );

void advanced_chemotaxis_function_normalized( Cell* pCell, Phenotype& phenotype , double dt ); 
void advanced_chemotaxis_function( Cell* pCell, Phenotype& phenotype , double dt ); 

void dynamic_attachments( Cell* pCell , Phenotype& phenotype, double dt ); 
void dynamic_spring_attachments( Cell* pCell , Phenotype& phenotype, double dt ); 

	
};

#endif 
