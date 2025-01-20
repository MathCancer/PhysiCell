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

#ifndef __PhysiCell_cell_container_h__
#define __PhysiCell_cell_container_h__

#include <vector>
#include "PhysiCell_cell.h"
#include "../BioFVM/BioFVM_agent_container.h"
#include "../BioFVM/BioFVM_mesh.h"
#include "../BioFVM/BioFVM_microenvironment.h"

namespace PhysiCell{

class Cell; 

class Cell_Container : public BioFVM::Agent_Container
{
 private:	
	std::vector<Cell*> cells_ready_to_divide; // the index of agents ready to divide
	std::vector<Cell*> cells_ready_to_die;
	int boundary_condition_for_pushed_out_agents; 	// what to do with pushed out cells
	bool initialzed = false;
	
 public:
	BioFVM::Cartesian_Mesh underlying_mesh;
	std::vector<double> max_cell_interactive_distance_in_voxel;
	int num_divisions_in_current_step = 0;
	int num_deaths_in_current_step = 0;

	double last_diffusion_time  = 0.0; 
	double last_cell_cycle_time = 0.0;
	double last_mechanics_time  = 0.0;
	Cell_Container();
 	void initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double voxel_size);
	void initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx, double dy, double dz);
	std::vector<std::vector<Cell*> > agent_grid;
	std::vector<std::vector<Cell*> > agents_in_outer_voxels;
	
	void update_all_cells(double t);
	void update_all_cells(double t, double dt);
	void update_all_cells(double t, double phenotype_dt, double mechanics_dt);
	void update_all_cells(double t, double phenotype_dt, double mechanics_dt, double diffusion_dt ); 

	void register_agent( Cell* agent );
	void add_agent_to_outer_voxel(Cell* agent);
	void remove_agent(Cell* agent );
	void remove_agent_from_voxel(Cell* agent, int voxel_index);
	void add_agent_to_voxel(Cell* agent, int voxel_index);
	
	void flag_cell_for_division( Cell* pCell ); 
	void flag_cell_for_removal( Cell* pCell ); 
	bool contain_any_cell(int voxel_index);
};

int find_escaping_face_index(Cell* agent);
extern std::vector<Cell*> *all_cells; 

Cell_Container* create_cell_container_for_microenvironment( BioFVM::Microenvironment& m , double mechanics_voxel_size );



};
#endif
