/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#include "PhysiCell_cell.h"
#include "PhysiCell_cell_container.h"
#include "PhysiCell_utilities.h"
#include "PhysiCell_constants.h"
#include "../BioFVM/BioFVM_vector.h"
#include<limits.h>

namespace PhysiCell{

void empty_function( Cell* pCell, double dt )
{ return; }

Cell::Cell()
{
	advance_cell_current_phase = empty_function;
	// advance_cell_death = empty_function; 
	set_motility = empty_function;
	custom_cell_rule = empty_function; 
	update_phenotype = empty_function; 
	update_volume = update_volume_default;
	update_velocity = update_velocity_default;
	add_cell_basement_membrane_interactions = add_basement_membrane_interactions_default;
	update_cell_and_death_parameters = update_cell_and_death_parameters_O2_based;
	current_mechanics_voxel_index=-1;
	polarity=0;
	set_orientation=NULL;
	distance_to_membrane= NULL;
	is_moving=true;
	is_out_of_domain=false;
	displacement.resize(3,0);
	motility.resize(3,0);
	assign_orientation();
	container=NULL;
	
}

void Cell::assign_orientation()
{
	orientation.resize(3);
	if(set_orientation!=NULL)
	{
		set_orientation(this);
	}
	else
	{
		//assign a random unit vector
		double theta= UniformRandom()*6.28318530717959; //rand*2*pi
		double z= 2* UniformRandom()-1;
		double temp= sqrt(1-z*z);
		orientation[0]= temp * cos(theta);
		orientation[1]= temp * sin(theta);
		orientation[2]= z;
	}
}

Cell* Cell::divide( )
{
	Cell* kid = create_cell();
	kid->copy_data( this );	
	kid->copy_function_pointers(this);
	kid->parameters = parameters;
	kid->register_microenvironment(get_microenvironment());
	// randomly place the new agent close to me
	double temp_angle = 6.28318530717959*UniformRandom();
	double temp_phi = 3.1415926535897932384626433832795*UniformRandom();
	// check the following operations to make sure it is the right way to do it
	double radius= phenotype.geometry.radius;
	std::vector<double> rand_vec (3, 0.0);
	
	rand_vec[0]= cos( temp_angle ) * sin( temp_phi );
	rand_vec[1]= sin( temp_angle ) * sin( temp_phi );
	rand_vec[2]= cos( temp_phi );
	rand_vec = rand_vec- polarity*(rand_vec[0]*orientation[0]+ rand_vec[1]*orientation[1]+rand_vec[2]*orientation[2])*orientation;
	if(norm(rand_vec)==0)
		std::cout<<"************ERROR********************"<<std::endl;
	rand_vec/= norm(rand_vec);
	kid->assign_position(position[0] + 0.5 * radius*rand_vec[0],
						 position[1] + 0.5 * radius*rand_vec[1],
						 position[2] + 0.5 * radius*rand_vec[2]);
	// get_container()->register_agent(kid); //assign_position takes care of this
	//change my position to keep the center of mass intact and then see if I need to update my voxel index
	position[0] -= 0.5*radius*rand_vec[0];
	position[1] -= 0.5*radius*rand_vec[1]; 
	position[2] -= 0.5*radius*rand_vec[2]; 
	update_voxel_in_container();
	phenotype.volume.divide(); 
	kid->phenotype.volume.divide();
	kid->set_total_volume(kid->phenotype.volume.total);
	set_total_volume(phenotype.volume.total);
	kid->set_phenotype(phenotype, base_phenotype);	
	return kid;
}

bool Cell::assign_position(std::vector<double> new_position)
{
	return assign_position(new_position[0], new_position[1], new_position[2]);
}

void Cell::set_previous_velocity(double xV, double yV, double zV)
{
	previous_velocity[0]=xV; previous_velocity[1]=yV; previous_velocity[2]=zV; 
}

bool Cell::assign_position(double x, double y, double z)
{
	if( !get_container()->underlying_mesh.is_position_valid(x,y,z))
	{	
		return false;
	}
	position[0]=x;
	position[1]=y;
	position[2]=z;
	
	// update microenvironment current voxel index
	update_voxel_index();
	// update current_mechanics_voxel_index
	current_mechanics_voxel_index= get_container()->underlying_mesh.nearest_voxel_index( position );
	get_container()->register_agent(this);
	return true;
}

void Cell::set_total_volume(double volume)
{
	Basic_Agent::set_total_volume(volume);
	if(phenotype.volume.total!=volume)
	{
		double ratio= volume/ phenotype.volume.total;
		phenotype.volume.multiply_by_ratio(ratio);
	}
	phenotype.update_radius();
	if(get_container()->max_cell_interactive_ditstance_in_voxel[get_current_mechanics_voxel_index()]< phenotype.geometry.radius * parameters.max_interaction_distance_factor )
		get_container()->max_cell_interactive_ditstance_in_voxel[get_current_mechanics_voxel_index()]= phenotype.geometry.radius*parameters.max_interaction_distance_factor;
	}

void Cell::set_phenotype(Phenotype phenotype, Phenotype base_phenotype)
{
	this->phenotype=phenotype;	
	this->base_phenotype= base_phenotype;
	secretion_rates= & this->phenotype.secretion_rates.rates;
	uptake_rates= & this->phenotype.uptake_rates.rates;
	saturation_densities= & this->phenotype.saturation_densities.densities;
	set_total_volume(phenotype.volume.total);
}

void Cell::set_phenotype(Phenotype phenotype)
{
	set_phenotype(phenotype, phenotype);
}

void Cell::turn_off_reactions(double dt)
{	
	// is_active=false;  /* uncomment this line if you want to completely turn off all the reactions*/
	
	// Reduce all the uptake and secretion rates by a factor of 10
	for(int i=0;i< phenotype.uptake_rates.rates.size();i++)
		phenotype.uptake_rates.rates[i]/=10.0;  
	for(int i=0;i< phenotype.secretion_rates.rates.size();i++)
		phenotype.secretion_rates.rates[i]/=10.0;  
	set_internal_uptake_constants(dt);
}

Cell_Container * Cell::get_container()
{
	if(container==NULL)
		container= (Cell_Container *)get_microenvironment()->agent_container;
	return container;
}

void Cell:: die()
{
	delete_cell(this);
} 

void Cell::update_position( double dt )
{
	Basic_Agent::update_position(dt);
	// use Adams-Bashforth 
	static double d1; 
	static double d2; 
	static bool constants_defined = false; 
	if( constants_defined == false )
	{
		d1 = dt; 
		d1 *= 1.5; 
		d2 = dt; 
		d2 *= -0.5; 
		constants_defined = true; 
	}
	std::vector<double> old_position(position); 
	axpy( &position , d1 , velocity );  
	axpy( &position , d2 , previous_velocity );  
	// overwrite previous_velocity for future use 
	// if(sqrt(dist(old_position, position))>3* phenotype.geometry.radius)
		// std::cout<<sqrt(dist(old_position, position))<<"old_position: "<<old_position<<", new position: "<< position<<", velocity: "<<velocity<<", previous_velocity: "<< previous_velocity<<std::endl;
	
	previous_velocity = velocity; 
	
	velocity[0]=0; velocity[1]=0; velocity[2]=0;
	// #pragma omp critical
	//{update_voxel_in_container();}
	if(get_container()->underlying_mesh.is_position_valid(position[0],position[1],position[2]))
		updated_current_mechanics_voxel_index=get_container()->underlying_mesh.nearest_voxel_index( position );
	else
		updated_current_mechanics_voxel_index=-1;
	return; 
}

int Cell::get_current_mechanics_voxel_index()
{
	return current_mechanics_voxel_index;
}

void Cell::update_voxel_in_container()
{
	// call the method from BioFVM_basic_agent to update microenvironment's voxel index
	update_voxel_index();
	// int temp_current_voxel_index;
	// Check to see if we need to remove agents that are pushed out of boundary
	// if(!get_container()->underlying_mesh.is_position_valid(position[0],position[1],position[2]))	
		
	if(updated_current_mechanics_voxel_index==-1)// updated_current_mechanics_voxel_index is updated in update_position
	{
		// check if this agent has a valid voxel index, if so, remove it from previous voxel
		if(get_current_mechanics_voxel_index()>=0)
		{
			// #pragma omp critical
			{get_container()->remove_agent_from_voxel(this, get_current_mechanics_voxel_index());}
		}
		// #pragma omp critical
		{get_container()->add_agent_to_outer_voxel(this);}
		// std::cout<<"cell out of boundary..."<< __LINE__<<" "<<ID<<std::endl;
		current_mechanics_voxel_index=-1;
		is_out_of_domain=true;
		is_active=false;
		return;
	}
	
	// temp_current_voxel_index= get_current_mechanics_voxel_index();
	// updated_current_mechanics_voxel_index=get_container()->underlying_mesh.nearest_voxel_index( position );
	
	// update mesh indices (if needed)
	if(updated_current_mechanics_voxel_index!= get_current_mechanics_voxel_index())
	{
		// #pragma omp critical
		{
			container->remove_agent_from_voxel(this, get_current_mechanics_voxel_index());
			container->add_agent_to_voxel(this, updated_current_mechanics_voxel_index);
		}
		current_mechanics_voxel_index=updated_current_mechanics_voxel_index;
	}
}

void Cell::copy_data(Cell* copy_me)
{
	// phenotype=copyMe-> phenotype; //it is taken care in set_phenotype
	type=copy_me->type;
	velocity = copy_me->velocity; 
	// expected_phenotype = copy_me-> expected_phenotype; //it is taken care in set_phenotype
	cell_source_sink_solver_temp1= std::vector<double>(copy_me->cell_source_sink_solver_temp1);
	cell_source_sink_solver_temp2= std::vector<double>(copy_me->cell_source_sink_solver_temp2);
}

void Cell::copy_function_pointers(Cell* copy_me)
{
	advance_cell_current_phase=copy_me-> advance_cell_current_phase;
	custom_cell_rule=copy_me-> custom_cell_rule;
	set_motility=copy_me-> set_motility;
	update_velocity=copy_me->update_velocity;
	update_phenotype=copy_me->update_phenotype;
	update_volume=copy_me->update_volume;
	distance_to_membrane= copy_me->distance_to_membrane;
	add_cell_basement_membrane_interactions= copy_me->add_cell_basement_membrane_interactions;
	set_orientation= copy_me->set_orientation;
	update_phenotype_parameters= copy_me->update_phenotype_parameters;
	update_cell_and_death_parameters= copy_me->update_cell_and_death_parameters;
}

void Cell::add_potentials(Cell* other_agent) {
	if( this->ID == other_agent->ID )
		return;	

	double distance = 0; 
	for( int i = 0 ; i < 3 ; i++ ) 
	{ 
		displacement[i] = position[i] - (*other_agent).position[i]; 
		distance += displacement[i] * displacement[i]; 
	}
	// Make sure that the distance is not zero
	
	distance=std::max(sqrt(distance), 0.00001); 
	
	//Repulsive
	double R = phenotype.geometry.radius+ (*other_agent).phenotype.geometry.radius; 
	
	double RN = phenotype.geometry.nuclear_radius + (*other_agent).phenotype.geometry.nuclear_radius;	
	double temp_r, c;
	if( distance > R ) 
		temp_r=0; 
	// else if( distance < RN ) 
	// {
		// double M = 1.0; 
		// c = 1.0 - RN/R; 
		// c *= c; 
		// c -= M; 

		// temp_r = ( c*distance/RN  + M  ); 
	// }
	else
	{
		temp_r = 1 - distance/R;
		temp_r *= temp_r;
	}
	temp_r*=parameters.Cccr;
	//////////////////////////////////////////////////////////////////
	
	// Adhesive
	double debug_temp_a=0;
	double max_interactive_distance = parameters.max_interaction_distance_factor * phenotype.geometry.radius + 
	(*other_agent).parameters.max_interaction_distance_factor * (*other_agent).phenotype.geometry.radius;
	if(distance<max_interactive_distance ) 
	{	
		double temp_a = 1 - distance/max_interactive_distance; 
		temp_a *= temp_a; 
		temp_a*=parameters.Ccca;
		debug_temp_a=temp_a;
		temp_r-= temp_a;
	}
	/////////////////////////////////////////////////////////////////
	if(temp_r==0)
		return;
	temp_r/=distance;
	for( int i = 0 ; i < 3 ; i++ ) 
	{
		velocity[i] += displacement[i] * temp_r; 
		// other_agent->velocity[i]-=displacement[i] * temp_r;
	}
	return;
}

Cell* create_cell( void )
{
	Cell* pNew; 
	pNew = new Cell;		
	(*all_cells).push_back( pNew ); 
	pNew->index=(*all_cells).size()-1;
	return pNew; 
}

void delete_cell( int index )
{
	// deregister agent in from the agent container
	(*all_cells)[index]->get_container()->remove_agent((*all_cells)[index]);
	// de-allocate (delete) the cell; 
	delete (*all_cells)[index]; 

	// performance goal: don't delete in the middle -- very expensive reallocation
	// alternative: copy last element to index position, then shrink vector by 1 at the end O(constant)

	// move last item to index location  
	(*all_cells)[ (*all_cells).size()-1 ]->index=index;
	(*all_cells)[index] = (*all_cells)[ (*all_cells).size()-1 ];
	// shrink the vector
	(*all_cells).pop_back();	
	return; 
}

void delete_cell( Cell* pDelete )
{
	delete_cell(pDelete->index);
	return; 
}


void update_volume_default(Cell* pCell, double dt)
{
	// std::cout<<dt * pCell->phenotype.volume.fluid_change_rate *  ( pCell->phenotype.volume.target_fluid_fraction*pCell->phenotype.volume.total - pCell->phenotype.volume.fluid)<<
	// ", "<<dt<<", "<< pCell->phenotype.volume.fluid_change_rate<<", "<<pCell->phenotype.volume.target_fluid_fraction*pCell->phenotype.volume.total<<", "<<pCell->phenotype.volume.fluid <<std::endl;
	pCell->phenotype.volume.fluid += dt * pCell->phenotype.volume.fluid_change_rate *  ( pCell->phenotype.volume.target_fluid_fraction*pCell->phenotype.volume.total - pCell->phenotype.volume.fluid);
	pCell->phenotype.volume.fluid=pCell->phenotype.volume.fluid<0?0:pCell->phenotype.volume.fluid;
	pCell->phenotype.volume.nuclear_fluid = (pCell->phenotype.volume.nuclear / pCell->phenotype.volume.total)*(  pCell->phenotype.volume.fluid  );
	pCell->phenotype.volume.cytoplasmic_fluid = pCell->phenotype.volume.fluid - pCell->phenotype.volume.nuclear_fluid; 
	
	pCell->phenotype.volume.nuclear_solid  += dt * pCell->phenotype.volume.nuclear_biomass_change_rate * (pCell->phenotype.volume.target_solid_nuclear - pCell->phenotype.volume.nuclear_solid );    
	pCell->phenotype.volume.nuclear_solid= pCell->phenotype.volume.nuclear_solid <0?0:pCell->phenotype.volume.nuclear_solid ;
	
	pCell->phenotype.volume.target_solid_cytoplasmic= pCell->phenotype.volume.cytoplasmic_to_nuclear_fraction* pCell->phenotype.volume.target_solid_nuclear;

	pCell->phenotype.volume.cytoplasmic_solid += dt* pCell->phenotype.volume.cytoplasmic_biomass_change_rate * ( pCell->phenotype.volume.target_solid_cytoplasmic - pCell->phenotype.volume.cytoplasmic_solid );	
	pCell->phenotype.volume.cytoplasmic_solid=pCell->phenotype.volume.cytoplasmic_solid<0?0:pCell->phenotype.volume.cytoplasmic_solid;
	
	pCell->phenotype.volume.solid = pCell->phenotype.volume.nuclear_solid + pCell->phenotype.volume.cytoplasmic_solid;
	
	pCell->phenotype.volume.nuclear = pCell->phenotype.volume.nuclear_solid + pCell->phenotype.volume.nuclear_fluid; 
	pCell->phenotype.volume.cytoplasmic = pCell->phenotype.volume.cytoplasmic_solid + pCell->phenotype.volume.cytoplasmic_fluid; 
	pCell->phenotype.volume.calcified_fraction = dt * pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].calcification_rate * (1- pCell->phenotype.volume.calcified_fraction);
   
	pCell->phenotype.volume.total = pCell->phenotype.volume.cytoplasmic + pCell->phenotype.volume.nuclear; 
   
	pCell->phenotype.update_radius();
	
	// The following line is needed in every volume regulation method (it sets BioFVM total_volume)
	pCell->set_total_volume(pCell->phenotype.volume.total);	
}


void update_velocity_default(Cell* pCell, double dt)
{
	pCell->add_cell_basement_membrane_interactions(pCell, dt);
	//First check the neighbors in my current voxel
	std::vector<Cell*>::iterator neighbor;
	std::vector<Cell*>::iterator end = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].end();
	for(neighbor = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].begin(); neighbor != end; ++neighbor)
	{
		pCell->add_potentials(*neighbor);
	}
	std::vector<int>::iterator neighbor_voxel_index;
	std::vector<int>::iterator neighbor_voxel_index_end=pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].end();
	// int counter=0;
	for(neighbor_voxel_index=pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].begin();
	neighbor_voxel_index != neighbor_voxel_index_end; ++ neighbor_voxel_index)
	{
		if(!is_neighbor_voxel(pCell, pCell->get_container()->underlying_mesh.voxels[pCell->get_current_mechanics_voxel_index()].center, pCell->get_container()->underlying_mesh.voxels[*neighbor_voxel_index].center, *neighbor_voxel_index))
			continue;
		end = pCell->get_container()->agent_grid[*neighbor_voxel_index].end();
		for(neighbor = pCell->get_container()->agent_grid[*neighbor_voxel_index].begin();neighbor != end; ++neighbor)
		{
			pCell->add_potentials(*neighbor);
		}
		// counter++;
	}
//	std::cout<<"number of neighbor voxels: "<<counter<<std::endl;
	pCell->set_motility(pCell, dt);
	pCell->velocity+= pCell->motility;
	// std::cout<<"pCell: "<< pCell->ID<<", dt: "<< dt<<", "<<pCell->velocity<<std::endl;
}

bool is_neighbor_voxel(Cell* pCell, std::vector<double> my_voxel_center, std::vector<double> other_voxel_center, int other_voxel_index)
{
	// std::cout<<pCell->ID<<", "<<other_voxel_center << ", "<<pCell->get_container()->max_cell_interactive_ditstance_in_voxel[other_voxel_index]<<std::endl;
	double max_interactive_distance = pCell->parameters.max_interaction_distance_factor * pCell->phenotype.geometry.radius + pCell->get_container()->max_cell_interactive_ditstance_in_voxel[other_voxel_index];
	
	int comparing_dimension=-1, comparing_dimension2=-1;
	if(my_voxel_center[0]== other_voxel_center[0] && my_voxel_center[1]== other_voxel_center[1])
		comparing_dimension=2;
	else if(my_voxel_center[0]== other_voxel_center[0] && my_voxel_center[2]== other_voxel_center[2])
		comparing_dimension=1;
	else if(my_voxel_center[1]== other_voxel_center[1] && my_voxel_center[2]== other_voxel_center[2])
		comparing_dimension=0;
	
	if(comparing_dimension!=-1) 
	{ //then it is an immediate neighbor (through side faces)
		double surface_coord= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		// std::cout<<pCell->ID<<", "<<max_interactive_distance<<", "<< surface_coord<<std::endl;
		if(std::fabs(pCell->position[comparing_dimension] - surface_coord) > max_interactive_distance)
			return false;
		// std::cout<<"NEIGHBOR FOUND! "<<pCell->ID<<", "<<max_interactive_distance<<", "<< surface_coord<<", "<<pCell->position <<std::endl;
		return true;
	}
	comparing_dimension=-1;
	
	if(my_voxel_center[0]== other_voxel_center[0])
	{
		comparing_dimension=1; comparing_dimension2=2;
	}
	else if(my_voxel_center[1]== other_voxel_center[1])
	{
		comparing_dimension=0; comparing_dimension2=2;
	}
	else if(my_voxel_center[2]== other_voxel_center[2])
	{
		comparing_dimension=0; comparing_dimension2=1;
	}
	if(comparing_dimension!=-1)
	{
		double line_coord1= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		double line_coord2= 0.5*(my_voxel_center[comparing_dimension2] + other_voxel_center[comparing_dimension2]);
		double distance_squared= std::pow( pCell->position[comparing_dimension] - line_coord1,2)+ std::pow( pCell->position[comparing_dimension2] - line_coord2,2);
		if(distance_squared > max_interactive_distance * max_interactive_distance)
			return false;
		return true;
	}
	std::vector<double> corner_point= 0.5*(my_voxel_center+other_voxel_center);
	double distance_squared= (corner_point[0]-pCell->position[0])*(corner_point[0]-pCell->position[0])
		+(corner_point[1]-pCell->position[1])*(corner_point[1]-pCell->position[1]) 
		+(corner_point[2]-pCell->position[2]) * (corner_point[2]-pCell->position[2]);
	if(distance_squared > max_interactive_distance * max_interactive_distance)
		return false;
	return true;
}


void add_basement_membrane_interactions_default(Cell* pCell, double dt)
{
	if(pCell->distance_to_membrane==NULL)
		return;
	double max_interactive_distance = pCell->parameters.max_interaction_distance_factor * pCell->phenotype.geometry.radius;
	double distance= pCell->distance_to_membrane(pCell); //Note that the distance_to_membrane function must set displacement values (as a normal vector)
		
	double temp_a=0;
	// Adhesion to basement membrane
	if(distance< max_interactive_distance)
	{
		temp_a= (1- distance/max_interactive_distance);
		temp_a*=temp_a;
		temp_a*=-pCell->parameters.Ccba;
	}
	// Repulsion from basement membrane
	double temp_r=0;
	if(distance<pCell->phenotype.geometry.radius)
	{
		temp_r= (1- distance/pCell->phenotype.geometry.radius);
		temp_r*=temp_r;
		temp_r*= pCell->parameters.Ccbr;
	}
	temp_r+=temp_a;
	if(temp_r==0)
		return;
	
	for( int i = 0 ; i < 3 ; i++ ) 
	{
		pCell->velocity[i] += pCell->displacement[i] * temp_r;
	}
	return;	
}

void set_3D_random_motility( Cell* pCell, double dt )
{
	double probability= UniformRandom();
	
	if(probability< dt/pCell->custom_data.motility_t_persistence)
	{
		double temp_angle = 6.28318530717959*UniformRandom();
		double temp_phi = 3.1415926535897932384626433832795*UniformRandom();
		
		pCell->motility[0]= pCell->custom_data.motility_magnitude * cos( temp_angle ) * sin( temp_phi );
		pCell->motility[1]= pCell->custom_data.motility_magnitude * sin( temp_angle ) * sin( temp_phi );
		pCell->motility[2]= pCell->custom_data.motility_magnitude * cos( temp_phi );
	}
}

void update_cell_and_death_parameters_O2_based( Cell* pCell, double dt )
{
	double factor=0;
	// Update T_Q
	if((pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index] > pCell->parameters.o2_no_proliferation)
	{
		if((pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index] <= pCell->parameters.o2_saturation )
			factor= (pCell->parameters.o2_ref - pCell->parameters.o2_no_proliferation)/ 
			((pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index] - pCell->parameters.o2_no_proliferation);
		else
			factor= ( pCell->parameters.o2_ref- pCell->parameters.o2_no_proliferation)/ 
			(pCell->parameters.o2_saturation - pCell->parameters.o2_no_proliferation);
				
		pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration= 
			pCell->base_phenotype.cycle.phases[pCell->base_phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration * factor;
		
		// std::cout<<pCell->ID<<", oxygen: "<<(pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index]<<", "<<factor<<", "<< pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration<<", "<<pCell->base_phenotype.cycle.phases[pCell->base_phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration<<std::endl;
	}
	else
		pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration= INT_MAX;
	//update birth_rate of live_apoptotic model
	
	// pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::live)].duration= pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::Ki67_negative)].duration + pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::Ki67_positive)].duration;
	// pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::live)].birth_rate= 1.0/pCell->phenotype.cycle.phases[pCell->phenotype.get_phase_index(PhysiCell_constants::live)].duration;
	
	// Update necrosis rate
	if((pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index]<= pCell->parameters.o2_critical)
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].necrosis_rate= pCell->parameters.max_necrosis_rate;
	else if ((pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index] < pCell->parameters.o2_necrotic)
	{
		factor= ( pCell->parameters.o2_necrotic- (pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index])/ 
			(pCell->parameters.o2_necrotic - pCell->parameters.o2_critical);
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].necrosis_rate = 
		pCell->parameters.max_necrosis_rate* factor;
	}
	else
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].necrosis_rate= 0;
	
	// std::cout<<(pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index] <<", "<<pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].necrosis_rate<<std::endl;
}
};