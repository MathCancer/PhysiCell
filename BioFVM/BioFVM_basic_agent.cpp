/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the BioFVM Project              #
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

#include "BioFVM_basic_agent.h"
#include "BioFVM_agent_container.h"
#include "BioFVM_vector.h" 

namespace BioFVM{

std::vector<Basic_Agent*> all_basic_agents(0); 

Basic_Agent::Basic_Agent()
{
	//give the agent a unique ID  
	static int max_basic_agent_ID = 0; 
	ID = max_basic_agent_ID; // 
	max_basic_agent_ID++; 
	// initialize position and velocity
	is_active=true;
	
	volume = 1.0; 
	
	position.assign( 3 , 0.0 ); 
	velocity.assign( 3 , 0.0 );
	previous_velocity.assign( 3 , 0.0 ); 
	// link into the microenvironment, if one is defined 
	secretion_rates= new std::vector<double>(0);
	uptake_rates= new std::vector<double>(0);
	saturation_densities= new std::vector<double>(0);
	net_export_rates = new std::vector<double>(0); 
	// extern Microenvironment* default_microenvironment;
	// register_microenvironment( default_microenvironment ); 

	internalized_substrates = new std::vector<double>(0); // 
	fraction_released_at_death = new std::vector<double>(0); 
	fraction_transferred_when_ingested = new std::vector<double>(0); 
	register_microenvironment( get_default_microenvironment() );
	
	// these are done in register_microenvironment
	// internalized_substrates.assign( get_default_microenvironment()->number_of_densities() , 0.0 ); 
	
	return;	
}

void Basic_Agent::update_position(double dt){ 
//make sure to update current_voxel_index if you are implementing this function
};
bool Basic_Agent::assign_position(std::vector<double> new_position)
{
	return assign_position(new_position[0], new_position[1], new_position[2]);
}

bool Basic_Agent::assign_position(double x, double y, double z)
{
	if( !get_microenvironment()->mesh.is_position_valid(x,y,z))
	{	
		// std::cout<<"Error: the new position for agent "<< ID << " is invalid: "<<x<<","<<y<<","<<"z"<<std::endl;
		return false;
	}
	position[0]=x;
	position[1]=y;
	position[2]=z;
	update_voxel_index();
	
	// make sure the agent is not already registered
	get_microenvironment()->agent_container->register_agent(this);
	return true;
}

void Basic_Agent::update_voxel_index()
{
	if( !get_microenvironment()->mesh.is_position_valid(position[0],position[1],position[2]))
	{	
		current_voxel_index=-1;
		is_active=false;
		return;
	}
	current_voxel_index= microenvironment->nearest_voxel_index( position );
}

int mycount = 0; 

void Basic_Agent::set_internal_uptake_constants( double dt )
{
	// overall form: dp/dt = S*(T-p) - U*p 
	//   p(n+1) - p(n) = dt*S(n)*T(n) - dt*( S(n) + U(n))*p(n+1)
	//   p(n+1)*temp2 =  p(n) + temp1
	//   p(n+1) = (  p(n) + temp1 )/temp2
	//int nearest_voxel= current_voxel_index;
	
/*	
	// new for tracking internal densities
	if( use_internal_densities_as_targets == true )
	{
		*saturation_densities = *internalized_substrates;
		*saturation_densities /= ( 1e-15 + volume ); 
	}
*/
	
	double internal_constant_to_discretize_the_delta_approximation = dt * volume / ( (microenvironment->voxels(current_voxel_index)).volume ) ; // needs a fix 
	
	// temp1 = dt*(V_cell/V_voxel)*S*T 
	cell_source_sink_solver_temp1.assign( (*secretion_rates).size() , 0.0 ); 
	cell_source_sink_solver_temp1 += *secretion_rates; 
	cell_source_sink_solver_temp1 *= *saturation_densities; 
	cell_source_sink_solver_temp1 *= internal_constant_to_discretize_the_delta_approximation; 
	
//	total_extracellular_substrate_change.assign( (*secretion_rates).size() , 1.0 ); 

	// temp2 = 1 + dt*(V_cell/V_voxel)*( S + U )
	cell_source_sink_solver_temp2.assign( (*secretion_rates).size() , 1.0 ); 
	axpy( &(cell_source_sink_solver_temp2) , internal_constant_to_discretize_the_delta_approximation , *secretion_rates );
	axpy( &(cell_source_sink_solver_temp2) , internal_constant_to_discretize_the_delta_approximation , *uptake_rates );	
	
	// temp for net export 
	cell_source_sink_solver_temp_export1 = *net_export_rates; 
	cell_source_sink_solver_temp_export1 *= dt; // amount exported in dt of time 
		
	cell_source_sink_solver_temp_export2 = cell_source_sink_solver_temp_export1;
	cell_source_sink_solver_temp_export2 /= ( (microenvironment->voxels(current_voxel_index)).volume ) ; 
	// change in surrounding density 
	
	volume_is_changed = false; 
	
	return; 
}

void Basic_Agent::register_microenvironment( Microenvironment* microenvironment_in )
{
	microenvironment = microenvironment_in; 	
	secretion_rates->resize( microenvironment->density_vector(0).size() , 0.0 );
	saturation_densities->resize( microenvironment->density_vector(0).size() , 0.0 );
	uptake_rates->resize( microenvironment->density_vector(0).size() , 0.0 );	
	net_export_rates->resize( microenvironment->density_vector(0).size() , 0.0 ); 

	// some solver temporary variables 
	cell_source_sink_solver_temp1.resize( microenvironment->density_vector(0).size() , 0.0 );
	cell_source_sink_solver_temp2.resize( microenvironment->density_vector(0).size() , 1.0 );
	
	cell_source_sink_solver_temp_export1.resize( microenvironment->density_vector(0).size() , 0.0 );
	cell_source_sink_solver_temp_export2.resize( microenvironment->density_vector(0).size() , 0.0 );

	// new for internalized substrate tracking 
	internalized_substrates->resize( microenvironment->density_vector(0).size() , 0.0 );
	total_extracellular_substrate_change.resize( microenvironment->density_vector(0).size() , 1.0 );
	
	fraction_released_at_death->resize( microenvironment->density_vector(0).size() , 0.0 ); 
	fraction_transferred_when_ingested->resize( microenvironment->density_vector(0).size() , 0.0 ); 

	return; 
}

void Basic_Agent::release_internalized_substrates( void )
{
	Microenvironment* pS = get_default_microenvironment(); 
	
	// change in total in voxel: 
	// total_ext = total_ext + fraction*total_internal 
	// total_ext / vol_voxel = total_ext / vol_voxel + fraction*total_internal / vol_voxel 
	// density_ext += fraction * total_internal / vol_volume 
	
	// std::cout << "\t\t\t" << (*pS)(current_voxel_index) << "\t\t\t" << std::endl; 
	*internalized_substrates /=  pS->voxels(current_voxel_index).volume; // turn to density 
	*internalized_substrates *= *fraction_released_at_death;  // what fraction is released? 
	
	// release this amount into the environment 
	
	(*pS)(current_voxel_index) += *internalized_substrates; 
	
	// zero out the now-removed substrates 
	
	internalized_substrates->assign( internalized_substrates->size() , 0.0 ); 
	
	return; 
}

Microenvironment* Basic_Agent::get_microenvironment( void )
{ return microenvironment; }

Basic_Agent* create_basic_agent( void )
{
	Basic_Agent* pNew; 
	pNew = new Basic_Agent;	 
	all_basic_agents.push_back( pNew ); 
	pNew->index=all_basic_agents.size()-1;
	return pNew; 
}

void delete_basic_agent( int index )
{
	// deregister agent in microenvironment
	all_basic_agents[index]->get_microenvironment()->agent_container->remove_agent(all_basic_agents[index]);
	// de-allocate (delete) the Basic_Agent; 
	
	delete all_basic_agents[index]; 

	// next goal: remove this memory address. 

	// performance goal: don't delete in the middle -- very expensive reallocation
	// alternative: copy last element to index position, then shrink vector by 1 at the end O(constant)

	// move last item to index location  
	all_basic_agents[ all_basic_agents.size()-1 ]->index=index;
	all_basic_agents[index] = all_basic_agents[ all_basic_agents.size()-1 ];

	// shrink the vector
	all_basic_agents.pop_back();
	
	return; 
}

void delete_basic_agent( Basic_Agent* pDelete )
{
	// First, figure out the index of this agent. This is not efficient. 

	// int delete_index = 0; 
	// while( all_basic_agents[ delete_index ] != pDelete )
	// { delete_index++; }

	delete_basic_agent(pDelete->index);
	return; 
}

int Basic_Agent::get_current_voxel_index( void )
{
	return current_voxel_index;
}

std::vector<double>& Basic_Agent::nearest_density_vector( void ) 
{  
	return microenvironment->nearest_density_vector( current_voxel_index ); 
}


// directly access the gradient of substrate n nearest to the cell 
std::vector<double>& Basic_Agent::nearest_gradient( int substrate_index )
{
	return microenvironment->gradient_vector(current_voxel_index)[substrate_index]; 
}

	// directly access a vector of gradients, one gradient per substrate 
std::vector<gradient>& Basic_Agent::nearest_gradient_vector( void )
{
	return microenvironment->gradient_vector(current_voxel_index); 
}

void Basic_Agent::set_total_volume(double volume)
{
	this->volume = volume;
	volume_is_changed = true;
}

double Basic_Agent::get_total_volume()
{
	return volume;
}

void Basic_Agent::simulate_secretion_and_uptake( Microenvironment* pS, double dt )
{
	if(!is_active)
	{ return; }
	
	if( volume_is_changed )
	{
		set_internal_uptake_constants(dt);
		volume_is_changed = false;
	}
	
	if( default_microenvironment_options.track_internalized_substrates_in_each_agent == true )
	{
		total_extracellular_substrate_change.assign( total_extracellular_substrate_change.size() , 1.0 ); // 1

		total_extracellular_substrate_change -= cell_source_sink_solver_temp2; // 1-c2
		total_extracellular_substrate_change *= (*pS)(current_voxel_index); // (1-c2)*rho 
		total_extracellular_substrate_change += cell_source_sink_solver_temp1; // (1-c2)*rho+c1 
		total_extracellular_substrate_change /= cell_source_sink_solver_temp2; // ((1-c2)*rho+c1)/c2
		total_extracellular_substrate_change *= pS->voxels(current_voxel_index).volume; // W*((1-c2)*rho+c1)/c2 
		
		*internalized_substrates -= total_extracellular_substrate_change; // opposite of net extracellular change 	
	}
	
	(*pS)(current_voxel_index) += cell_source_sink_solver_temp1; 
	(*pS)(current_voxel_index) /= cell_source_sink_solver_temp2; 
	
	// now do net export 
	(*pS)(current_voxel_index) += cell_source_sink_solver_temp_export2; 
	if( default_microenvironment_options.track_internalized_substrates_in_each_agent == true ) 
	{
		*internalized_substrates -= cell_source_sink_solver_temp_export1; 
	}

	return; 
}

};