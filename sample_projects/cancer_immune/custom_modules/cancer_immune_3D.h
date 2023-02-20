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

#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM; 
using namespace PhysiCell;

// custom cell phenotype function to scale immunostimulatory factor with hypoxia 
void tumor_cell_phenotype_with_and_immune_stimulation( Cell* pCell, Phenotype& phenotype, double dt ); 

extern Cell_Definition* pImmuneCell; 

void create_immune_cell_type( void ); 

// set the tumor cell properties, then call the function 
// to set up the tumor cells 
void create_cell_types( void );

void setup_tissue(); 

void introduce_immune_cells( void ); 

// set up the microenvironment to include the immunostimulatory factor 
void setup_microenvironment( void );   

std::vector<std::string> cancer_immune_coloring_function( Cell* );

// cell rules for extra elastic adhesion

/*
void attach_cells( Cell* pCell_1, Cell* pCell_2 );
void dettach_cells( Cell* pCell_1 , Cell* pCell_2 );
*/
void add_elastic_velocity( Cell* pActingOn, Cell* pAttachedTo , double elastic_constant ); 
void extra_elastic_attachment_mechanics( Cell* pCell, Phenotype& phenotype, double dt );

// immune cell functions for attacking a cell 
Cell* immune_cell_check_neighbors_for_attachment( Cell* pAttacker , double dt ); 
bool immune_cell_attempt_attachment( Cell* pAttacker, Cell* pTarget , double dt ); // only attack if oncoprotein 
bool immune_cell_attempt_apoptosis( Cell* pAttacker, Cell* pTarget, double dt ); 
bool immune_cell_trigger_apoptosis( Cell* pAttacker, Cell* pTarget ); 

void immune_cell_rule( Cell* pCell, Phenotype& phenotype, double dt ); 

void immune_cell_attach( Cell* pAttacker, Cell* pTarget ); // use attach_cells?? 
void immune_cell_dettach( Cell* pAttacker, Cell* pTarget ); // use dettach_cells ?? 

void adhesion_contact_function( Cell* pActingOn, Phenotype& pao, Cell* pAttachedTo, Phenotype& pat , double dt );

// immune cell functions for motility 

void immune_cell_motility( Cell* pCell, Phenotype& phenotype, double dt ); 
