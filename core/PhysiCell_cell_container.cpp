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

#include "../BioFVM/BioFVM_agent_container.h"
#include "PhysiCell_constants.h"
#include "../BioFVM/BioFVM_vector.h"
#include "PhysiCell_cell.h"

#include <algorithm>
#include <iterator> 

using namespace BioFVM;

namespace PhysiCell{

std::vector<Cell*> *all_cells;

Cell_Container::Cell_Container()
{
	all_cells = (std::vector<Cell*> *) &all_basic_agents;	
	boundary_condition_for_pushed_out_agents= PhysiCell_constants::default_boundary_condition_for_pushed_out_agents;
	std::vector<Cell*> cells_ready_to_divide;
	std::vector<Cell*> cells_ready_to_die;
	
	return; 
}	
	
void Cell_Container::initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double voxel_size)
{
	initialize(x_start, x_end, y_start, y_end, z_start, z_end , voxel_size, voxel_size, voxel_size);
	
	return; 
}

void Cell_Container::initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx, double dy, double dz)
{
	all_cells = (std::vector<Cell*> *) &all_basic_agents;	
	boundary_condition_for_pushed_out_agents= PhysiCell_constants::default_boundary_condition_for_pushed_out_agents;
	std::vector<Cell*> cells_ready_to_divide;
	std::vector<Cell*> cells_ready_to_die;

	underlying_mesh.resize(x_start, x_end, y_start, y_end, z_start, z_end , dx, dy, dz);
	agent_grid.resize(underlying_mesh.voxels.size());
	max_cell_interactive_distance_in_voxel.resize(underlying_mesh.voxels.size(), 0.0);
	agents_in_outer_voxels.resize(6);
	
	return; 
}
 
void Cell_Container::update_all_cells(double t)
{
	// update_all_cells(t, dt_settings.cell_cycle_dt_default, dt_settings.mechanics_dt_default);
	
	update_all_cells(t, phenotype_dt, mechanics_dt , diffusion_dt );
	
	return; 
}

// deprecate me JULY 2017
void Cell_Container::update_all_cells(double t, double dt)
{
	std::cout << "WARNING : " << __FUNCTION__ << " in " << __FILE__ << " is deprecated." 
		<< "\tIt returns without execution." << std::endl; 
	return; 
	update_all_cells(t, dt,dt);
	
	return; 
}

// deprecate me JULY 2017 
void Cell_Container::update_all_cells(double t, double phenotype_dt_ , double mechanics_dt_ )
{
	std::cout << "WARNING : " << __FUNCTION__ << " in " << __FILE__ << " is deprecated." 
		<< "\tIt returns without execution." << std::endl; 
	return; 
	
	//if it is the time for running cell cycle, do it!
	double time_since_last_cycle= t- last_cell_cycle_time;

	static double phenotype_tolerance = 0.001 * phenotype_dt_; 
	static double mechanics_tolerance = 0.001 * mechanics_dt_; 
	
	if( fabs(time_since_last_cycle- phenotype_dt_ ) < phenotype_tolerance || !initialzed)
	{
		// Reset the max_radius in each voxel. It will be filled in set_total_volume
		// It might be better if we calculate it before mechanics each time 
		std::fill(max_cell_interactive_distance_in_voxel.begin(), max_cell_interactive_distance_in_voxel.end(), 0.0);
		
		if(!initialzed)
		{
			time_since_last_cycle = phenotype_dt_;
		}
		
		// old functions prior to 1.2.1
		/*
		#pragma omp parallel for
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			if((*all_cells)[i]->is_out_of_domain)
				continue;
			(*all_cells)[i]->update_cell_and_death_parameters((*all_cells)[i],cell_cycle_dt);
			(*all_cells)[i]->advance_cell_current_phase((*all_cells)[i],time_since_last_cycle);
			(*all_cells)[i]->update_volume((*all_cells)[i], time_since_last_cycle ); 
		}
		*/
		
		// new as of 1.2.1 -- bundles cell phenotype parameter update, volume update, geometry update, 
		// checking for death, and advancing the cell cycle. Not motility, though. (that's in mechanics)
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "phenotype" << std::endl; 
		#pragma omp parallel for
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			if( (*all_cells)[i]->is_out_of_domain == false )
			{
				(*all_cells)[i]->advance_bundled_phenotype_functions( time_since_last_cycle ); 
			}
		}
		
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "divide / die " << std::endl; 
		// process divides / removes 
		for( int i=0; i < cells_ready_to_divide.size(); i++ )
		{
			cells_ready_to_divide[i]->divide();
		}
		for( int i=0; i < cells_ready_to_die.size(); i++ )
		{	
			cells_ready_to_die[i]->die();	
		}
		num_divisions_in_current_step+=  cells_ready_to_divide.size();
		num_deaths_in_current_step+=  cells_ready_to_die.size();
		
		cells_ready_to_die.clear();
		cells_ready_to_divide.clear();
		last_cell_cycle_time= t;
	}
	
	double time_since_last_mechanics= t- last_mechanics_time;
	
	// if( time_since_last_mechanics>= mechanics_dt || !initialzed)
	if( fabs(time_since_last_mechanics - mechanics_dt_)< mechanics_tolerance || !initialzed)
	{
		if(!initialzed)
		{
			time_since_last_mechanics = mechanics_dt_;
		}
		// Compute velocities
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{

			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable)
			{
				// update_velocity already includes the motility update 
				//(*all_cells)[i]->phenotype.motility.update_motility_vector( (*all_cells)[i] ,(*all_cells)[i]->phenotype , time_since_last_mechanics ); 
				(*all_cells)[i]->functions.update_velocity( (*all_cells)[i], (*all_cells)[i]->phenotype, time_since_last_mechanics);
			}

			if( (*all_cells)[i]->functions.custom_cell_rule )
			{
				(*all_cells)[i]->functions.custom_cell_rule((*all_cells)[i], (*all_cells)[i]->phenotype, time_since_last_mechanics);
			}
			
			if( (*all_cells)[i]->functions.contact_function )
			{
				evaluate_interactions( (*all_cells)[i], (*all_cells)[i]->phenotype , time_since_last_mechanics);
			}
		}
		// Calculate new positions
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable)
			{
				(*all_cells)[i]->update_position(time_since_last_mechanics);
			}
		}
		
		// When somebody reviews this code, let's add proper braces for clarity!!! 
		
		// Update cell indices in the container
		for( int i=0; i < (*all_cells).size(); i++ )
			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable)
				(*all_cells)[i]->update_voxel_in_container();
		last_mechanics_time=t;
	}
	initialzed=true;
	return;
}

void Cell_Container::update_all_cells(double t, double phenotype_dt_ , double mechanics_dt_ , double diffusion_dt_ )
{
	// secretions and uptakes. Syncing with BioFVM is automated. 

	// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "secretion" << std::endl; 
	#pragma omp parallel for 
	for( int i=0; i < (*all_cells).size(); i++ )
	{
		if( (*all_cells)[i]->is_out_of_domain == false )
		{
			(*all_cells)[i]->phenotype.secretion.advance( (*all_cells)[i], (*all_cells)[i]->phenotype , diffusion_dt_ );
		}
	}
	
	//if it is the time for running cell cycle, do it!
	double time_since_last_cycle= t- last_cell_cycle_time;

	static double phenotype_dt_tolerance = 0.001 * phenotype_dt_; 
	static double mechanics_dt_tolerance = 0.001 * mechanics_dt_; 
	
	if( fabs(time_since_last_cycle-phenotype_dt_ ) < phenotype_dt_tolerance || !initialzed)
	{
		// Reset the max_radius in each voxel. It will be filled in set_total_volume
		// It might be better if we calculate it before mechanics each time 
		// std::fill(max_cell_interactive_distance_in_voxel.begin(), max_cell_interactive_distance_in_voxel.end(), 0.0);
		
		if(!initialzed)
		{
			time_since_last_cycle = phenotype_dt_;
		}
		
		// new as of 1.2.1 -- bundles cell phenotype parameter update, volume update, geometry update, 
		// checking for death, and advancing the cell cycle. Not motility, though. (that's in mechanics)
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "bundled phenotype" << std::endl; 
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			if( (*all_cells)[i]->is_out_of_domain == false )
			{
				(*all_cells)[i]->advance_bundled_phenotype_functions( time_since_last_cycle ); 
			}
		}
		
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "divide / die " << std::endl; 
		// process divides / removes 
		for( int i=0; i < cells_ready_to_divide.size(); i++ )
		{
			cells_ready_to_divide[i]->divide();
		}
		for( int i=0; i < cells_ready_to_die.size(); i++ )
		{	
			cells_ready_to_die[i]->die();	
		}
		num_divisions_in_current_step+=  cells_ready_to_divide.size();
		num_deaths_in_current_step+=  cells_ready_to_die.size();
		
		cells_ready_to_die.clear();
		cells_ready_to_divide.clear();
		last_cell_cycle_time= t;
	}
		
	double time_since_last_mechanics= t- last_mechanics_time;
	
	// if( time_since_last_mechanics>= mechanics_dt || !initialzed)
	if( fabs(time_since_last_mechanics - mechanics_dt_) < mechanics_dt_tolerance || !initialzed)
	{
		if(!initialzed)
		{
			time_since_last_mechanics = mechanics_dt_;
		}
		
		// new February 2018 
		// if we need gradients, compute them
		if( default_microenvironment_options.calculate_gradients ) 
		{ microenvironment.compute_all_gradient_vectors();  }
		// end of new in Feb 2018 
		
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "interactions" << std::endl; 
		// perform interactions -- new in June 2020 
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			Cell* pC = (*all_cells)[i]; 
			if( pC->functions.contact_function && pC->is_out_of_domain == false )
			{ evaluate_interactions( pC,pC->phenotype,time_since_last_mechanics ); }
		}
		
		// perform custom computations 

		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "custom" << std::endl; 
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			Cell* pC = (*all_cells)[i]; 
			if( pC->functions.custom_cell_rule && pC->is_out_of_domain == false )
			{ pC->functions.custom_cell_rule( pC,pC->phenotype,time_since_last_mechanics ); }
		}
		
		// update velocities 
		
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "velocity" << std::endl; 
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			Cell* pC = (*all_cells)[i]; 
			if( pC->functions.update_velocity && pC->is_out_of_domain == false && pC->is_movable )
			{ pC->functions.update_velocity( pC,pC->phenotype,time_since_last_mechanics ); }
		}

		// update positions 
		
		// std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "position" << std::endl; 
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			Cell* pC = (*all_cells)[i]; 
			if( pC->is_out_of_domain == false && pC->is_movable)
			{ pC->update_position(time_since_last_mechanics); }
		}
		
/*		
		// Compute custom functions, interations, and velocities
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{

			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable && (*all_cells)[i]->functions.update_velocity )
			{
				// update_velocity already includes the motility update 
				//(*all_cells)[i]->phenotype.motility.update_motility_vector( (*all_cells)[i] ,(*all_cells)[i]->phenotype , time_since_last_mechanics ); 
				(*all_cells)[i]->functions.update_velocity( (*all_cells)[i], (*all_cells)[i]->phenotype, time_since_last_mechanics);
			}

			if( (*all_cells)[i]->functions.custom_cell_rule )
			{
				(*all_cells)[i]->functions.custom_cell_rule((*all_cells)[i], (*all_cells)[i]->phenotype, time_since_last_mechanics);
			}
			
			// contact interactions 
			
			if( (*all_cells)[i]->functions.contact_function )
			{
				evaluate_interactions( (*all_cells)[i] , (*all_cells)[i]->phenotype, time_since_last_mechanics );
			}
			
		}
		// Calculate new positions
		#pragma omp parallel for 
		for( int i=0; i < (*all_cells).size(); i++ )
		{
			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable)
			{
				(*all_cells)[i]->update_position(time_since_last_mechanics);
			}
		}
*/		
		
		// When somebody reviews this code, let's add proper braces for clarity!!! 
		
		// Update cell indices in the container
		for( int i=0; i < (*all_cells).size(); i++ )
			if(!(*all_cells)[i]->is_out_of_domain && (*all_cells)[i]->is_movable)
				(*all_cells)[i]->update_voxel_in_container();
		last_mechanics_time=t;
	}
	
	initialzed=true;
	return;
}

void Cell_Container::register_agent( Cell* agent )
{
	agent_grid[agent->get_current_mechanics_voxel_index()].push_back(agent);
	return; 
}

void Cell_Container::remove_agent(Cell* agent )
{
	remove_agent_from_voxel(agent, agent->get_current_mechanics_voxel_index());
	return; 
}

void Cell_Container::add_agent_to_outer_voxel(Cell* agent)
{
	int escaping_face= find_escaping_face_index(agent);
	agents_in_outer_voxels[escaping_face].push_back(agent);
	agent->is_out_of_domain=true;
	return; 
}

void Cell_Container::remove_agent_from_voxel(Cell* agent, int voxel_index)
{
	int delete_index = 0; 
	while( agent_grid[voxel_index][ delete_index ] != agent )
	{
		delete_index++; 
	}
	// move last item to index location  
	agent_grid[agent->get_current_mechanics_voxel_index()][delete_index] = agent_grid[agent->get_current_mechanics_voxel_index()][agent_grid[agent->get_current_mechanics_voxel_index()].size()-1 ]; 
	// shrink the vector
	agent_grid[agent->get_current_mechanics_voxel_index()].pop_back(); 
	return; 
}		

void Cell_Container::add_agent_to_voxel(Cell* agent, int voxel_index)
{
	agent_grid[voxel_index].push_back(agent); 
	return; 
}	

bool Cell_Container::contain_any_cell(int voxel_index)
{
	// Let's replace this with clearer statements. 
	return agent_grid[voxel_index].size()==0?false:true;
}

int find_escaping_face_index(Cell* agent)
{
	if(agent->position[0] <= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_min_x_index])
	{ return PhysiCell_constants::mesh_lx_face_index; }
	if(agent->position[0] >= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_max_x_index])
	{ return PhysiCell_constants::mesh_ux_face_index; }
	if(agent->position[1] <= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_min_y_index])
	{ return PhysiCell_constants::mesh_ly_face_index; }
	if(agent->position[1] >= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_max_y_index])
	{ return PhysiCell_constants::mesh_uy_face_index; }
	if(agent->position[2] <= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_min_z_index])
	{ return PhysiCell_constants::mesh_lz_face_index; }
	if(agent->position[2] >= agent->get_container()->underlying_mesh.bounding_box[PhysiCell_constants::mesh_max_z_index])
	{ return PhysiCell_constants::mesh_uz_face_index; }
	return -1; 
}

void Cell_Container::flag_cell_for_division( Cell* pCell )
{ 
	#pragma omp critical
	{
		auto result = std::find(std::begin(cells_ready_to_divide), std::end(cells_ready_to_divide), pCell );
		if( result == std::end(cells_ready_to_divide) )
		{ cells_ready_to_divide.push_back( pCell ); }
	} 
	return; 
}

void Cell_Container::flag_cell_for_removal( Cell* pCell )
{ 
	#pragma omp critical
	{
		auto result = std::find(std::begin(cells_ready_to_die), std::end(cells_ready_to_die), pCell );
		if( result == std::end(cells_ready_to_die) )
		{ cells_ready_to_die.push_back( pCell ); }		
	} 
	return; 
}

Cell_Container* create_cell_container_for_microenvironment( BioFVM::Microenvironment& m , double mechanics_voxel_size )
{
	Cell_Container* cell_container = new Cell_Container;
	cell_container->initialize( m.mesh.bounding_box[0], m.mesh.bounding_box[3], 
		m.mesh.bounding_box[1], m.mesh.bounding_box[4], 
		m.mesh.bounding_box[2], m.mesh.bounding_box[5],  mechanics_voxel_size );
	m.agent_container = (Agent_Container*) cell_container; 
	
	if( BioFVM::get_default_microenvironment() == NULL )
	{ 
		BioFVM::set_default_microenvironment( &m ); 
	}
	
	return cell_container; 
}

};
