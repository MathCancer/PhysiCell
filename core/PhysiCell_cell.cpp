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

#include "PhysiCell_cell.h"
#include "PhysiCell_cell_container.h"
#include "PhysiCell_utilities.h"
#include "PhysiCell_constants.h"
#include "../BioFVM/BioFVM_vector.h" 
#include<limits.h>

namespace PhysiCell{

std::unordered_map<std::string,Cell_Definition*> cell_definitions_by_name; 
std::unordered_map<int,Cell_Definition*> cell_definitions_by_type; 
std::vector<Cell_Definition*> cell_definitions_by_index;

Cell_Parameters::Cell_Parameters()
{
	o2_hypoxic_threshold = 15.0; // HIF-1alpha at half-max around 1.5-2%, and tumors often are below 2%
	o2_hypoxic_response = 8.0; // genomic / proteomic changes observed at 7-8 mmHg 
	o2_hypoxic_saturation = 4.0; // maximum HIF-1alpha at 0.5% o2 (McKeown)
	
	o2_necrosis_threshold = 5.0; 
	o2_necrosis_max = 2.5; 
	
	o2_proliferation_threshold = 5.0; // assume no proliferation at same level as starting necrosis 
	o2_proliferation_saturation = 160.0; // 5% = 38, 21% = 160 mmHg 
	o2_reference = 160.0; // assume all was measured in normoxic 21% o2 
	
	pReference_live_phenotype = NULL; // reference live (usually physioxic) phenotype 
	
	// necrosis parameters 
	
	max_necrosis_rate = 1.0 / (6.0 * 60.0); // assume cells survive 6 hours in very low oxygen 
	necrosis_type = PhysiCell_constants::deterministic_necrosis;;

	return; 
}

Cell_Definition::Cell_Definition()
{
	// set the microenvironment pointer 
	pMicroenvironment = NULL;
	if( BioFVM::get_default_microenvironment() != NULL )
	{ pMicroenvironment = BioFVM::get_default_microenvironment(); }

	// set up the default parameters 
		// the default Cell_Parameters constructor should take care of this
		
	type = 0; 
	name = "unnamed"; 

	parameters.pReference_live_phenotype = &phenotype; 
		
	// set up the default custom data 
		// the default Custom_Cell_Data constructor should take care of this
		
	// set up the default functions 
	functions.volume_update_function = NULL; // standard_volume_update_function;
	functions.update_migration_bias = NULL; 
	
	functions.update_phenotype = NULL; 
	functions.custom_cell_rule = NULL; 
	
	functions.update_velocity = NULL; // standard_update_cell_velocity;
	functions.add_cell_basement_membrane_interactions = NULL; 
	functions.calculate_distance_to_membrane = NULL; 
	
	functions.set_orientation = NULL;
	
	cell_definitions_by_index.push_back( this ); 
	
	return; 
}

Cell_Definition::Cell_Definition( Cell_Definition& cd )
{
	// set the microenvironment pointer 
	pMicroenvironment = cd.pMicroenvironment;

	// set up the default parameters 
		// the default Cell_Parameters constructor should take care of this
		
	type = cd.type; 
	name = cd.name; 
	 
	parameters = cd.parameters;
	custom_data = cd.custom_data; 
	functions = cd.functions; 
	phenotype = cd.phenotype; 
	
	// this is the whole reason we need ot make a copy constructor 
	parameters.pReference_live_phenotype = &phenotype; 
	
	cell_definitions_by_index.push_back( this ); 
	
	return; 
}

Cell_Definition& Cell_Definition::operator=( const Cell_Definition& cd )
{
	// set the microenvironment pointer 
	pMicroenvironment = cd.pMicroenvironment;

	// set up the default parameters 
		// the default Cell_Parameters constructor should take care of this
		
	type = cd.type; 
	name = cd.name; 
	 
	parameters = cd.parameters;
	custom_data = cd.custom_data; 
	functions = cd.functions; 
	phenotype = cd.phenotype; 
	
	// this is the whole reason we need ot make a copy constructor 
	parameters.pReference_live_phenotype = &phenotype; 
	
	cell_definitions_by_index.push_back( this ); 
	
	return *this; 
}


Cell_Definition cell_defaults; 

Cell_State::Cell_State()
{
	neighbors.resize(0); 
	orientation.resize( 3 , 0.0 ); 
	
	simple_pressure = 0.0; 
	
	return; 
}

void Cell::update_motility_vector( double dt_ )
{
	if( phenotype.motility.is_motile == false )
	{
		phenotype.motility.motility_vector.assign( 3, 0.0 ); 
		return; 
	}
	
	if( UniformRandom() < dt_ / phenotype.motility.persistence_time || phenotype.motility.persistence_time < dt_ )
	{
		// choose a uniformly random unit vector 
		double temp_angle = 6.28318530717959*UniformRandom();
		double temp_phi = 3.1415926535897932384626433832795*UniformRandom();
		
		double sin_phi = sin(temp_phi);
		double cos_phi = cos(temp_phi);
		
		if( phenotype.motility.restrict_to_2D == true )
		{ 
			sin_phi = 1.0; 
			cos_phi = 0.0;
		}
		
		std::vector<double> randvec; 
		randvec.resize(3,sin_phi); 
		
		randvec[0] *= cos( temp_angle ); // cos(theta)*sin(phi)
		randvec[1] *= sin( temp_angle ); // sin(theta)*sin(phi)
		randvec[2] = cos_phi; //  cos(phi)
		
		// if the update_bias_vector function is set, use it  
		if( functions.update_migration_bias )
		{
			functions.update_migration_bias( this,phenotype,dt_ ); 
		}
		
		phenotype.motility.motility_vector = phenotype.motility.migration_bias_direction; // motiltiy = bias_vector
		phenotype.motility.motility_vector *= phenotype.motility.migration_bias; // motility = bias*bias_vector 
		
		double one_minus_bias = 1.0 - phenotype.motility.migration_bias; 
		
		axpy( &(phenotype.motility.motility_vector), one_minus_bias, randvec ); // motility = (1-bias)*randvec + bias*bias_vector
		
		normalize( &(phenotype.motility.motility_vector) ); 
		
		phenotype.motility.motility_vector *= phenotype.motility.migration_speed; 
	}	
	return; 
} 

void Cell::advance_bundled_phenotype_functions( double dt_ )
{
	// call the custom code to update the phenotype 
	if( functions.update_phenotype )
	{	functions.update_phenotype( this , phenotype , dt_ ); }
	
	// update volume 
	if( functions.volume_update_function )
	{
		functions.volume_update_function(this,phenotype,dt_); 
		
		// The following line is needed in every volume 
		// regulation method (it sets BioFVM total_volume)
		
		set_total_volume( phenotype.volume.total ); 
	}
	
	// update geometry
	phenotype.geometry.update( this, phenotype, dt_ );
	
	// check for new death events 
	if( phenotype.death.check_for_death( dt_ ) == true )
	{
		// if so, change the cycle model to the current death model 
		phenotype.cycle.sync_to_cycle_model( phenotype.death.current_model() ); 
		
		// also, turn off motility.
		
		phenotype.motility.is_motile = false; 
		phenotype.motility.motility_vector.assign( 3, 0.0 ); 
		functions.update_migration_bias = NULL;
		
		// turn off secretion, and reduce uptake by a factor of 10 
		phenotype.secretion.set_all_secretion_to_zero();
		phenotype.secretion.scale_all_uptake_by_factor( 0.10 );
		
		// make sure to run the death entry function 
		if( phenotype.cycle.current_phase().entry_function )
		{
			phenotype.cycle.current_phase().entry_function( this, phenotype, dt_ ); 
		}
	}
	
	// advance cycle model (for both cell cycle and death cycle models)
	phenotype.cycle.advance_cycle( this, phenotype, dt_ ); 
	if( phenotype.flagged_for_removal )
	{
		flag_for_removal(); 
		phenotype.flagged_for_removal = false; 
	}
	if( phenotype.flagged_for_division )
	{
		flag_for_division(); 
		phenotype.flagged_for_division = false; 
	}
	
	return; 
}

Cell::Cell()
{
	// use the cell defaults; 
	
	type = cell_defaults.type; 
	type_name = cell_defaults.name; 
	
	custom_data = cell_defaults.custom_data; 
	parameters = cell_defaults.parameters; 
	functions = cell_defaults.functions; 
	
	phenotype = cell_defaults.phenotype; 
	
	phenotype.molecular.sync_to_cell( this ); 
	
	// cell state should be fine by the default constructor 
	
	current_mechanics_voxel_index=-1;
	
	updated_current_mechanics_voxel_index = 0;
	
	is_movable = true;
	is_out_of_domain = false;
	displacement.resize(3,0.0); // state? 
	
	assign_orientation();
	container = NULL;
	
	set_total_volume( phenotype.volume.total ); 
	
	return; 
}

void Cell::flag_for_division( void )
{
	get_container()->flag_cell_for_division( this );
	return; 
}

void Cell::flag_for_removal( void )
{
	get_container()->flag_cell_for_removal( this );
	return;
}

void Cell::start_death( int death_model_index )
{
	// set the death data struture to the indicated death model 
	phenotype.death.trigger_death( death_model_index ); 
	// change the cycle model to the current death model 
	phenotype.cycle.sync_to_cycle_model( phenotype.death.current_model() ); 
		
	// turn off secretion, and reduce uptake by a factor of 10 
	phenotype.secretion.set_all_secretion_to_zero();
	phenotype.secretion.scale_all_uptake_by_factor( 0.10 );
		
	// turn off motility.
	phenotype.motility.is_motile = false; 
	phenotype.motility.motility_vector.assign( 3, 0.0 ); 
	functions.update_migration_bias = NULL;
		
	// make sure to run the death entry function 
	if( phenotype.cycle.current_phase().entry_function )
	{
		phenotype.cycle.current_phase().entry_function( this, phenotype, 0.0 ); 
	}

	return; 
}

void Cell::assign_orientation()
{
	state.orientation.resize(3,0.0);
	if( functions.set_orientation != NULL )
	{
		functions.set_orientation(this, phenotype, 0.0 );
	}
	else
	{
		//assign a random unit vector
		double theta= UniformRandom()*6.28318530717959; //rand*2*pi
		double z= 2* UniformRandom()-1;
		double temp= sqrt(1-z*z);
		state.orientation[0]= temp * cos(theta);
		state.orientation[1]= temp * sin(theta);
		state.orientation[2]= z;
	}
	
	return; 
}

Cell* Cell::divide( )
{
	// phenotype.flagged_for_division = false; 
	// phenotype.flagged_for_removal = false; 
	
	Cell* child = create_cell();
	child->copy_data( this );	
	child->copy_function_pointers(this);
	child->parameters = parameters;
	
	// evenly divide internalized substrates 
	// if these are not actively tracked, they are zero anyway 
	*internalized_substrates *= 0.5; 
	*(child->internalized_substrates) = *internalized_substrates ; 
	
	// The following is already performed by create_cell(). JULY 2017 ***
	// child->register_microenvironment( get_microenvironment() );
	
	// randomly place the new agent close to me, accounting for orientation and 
	// polarity (if assigned)
		
	double temp_angle = 6.28318530717959*UniformRandom();
	double temp_phi = 3.1415926535897932384626433832795*UniformRandom();
	
	double radius= phenotype.geometry.radius;
	std::vector<double> rand_vec (3, 0.0);
	
	rand_vec[0]= cos( temp_angle ) * sin( temp_phi );
	rand_vec[1]= sin( temp_angle ) * sin( temp_phi );
	rand_vec[2]= cos( temp_phi );
	rand_vec = rand_vec- phenotype.geometry.polarity*(rand_vec[0]*state.orientation[0]+ 
		rand_vec[1]*state.orientation[1]+rand_vec[2]*state.orientation[2])*state.orientation;
	
	if( norm(rand_vec) < 1e-16 )
	{
		std::cout<<"************ERROR********************"<<std::endl;
	}
	normalize( &rand_vec ); 
	// rand_vec/= norm(rand_vec);
	child->assign_position(position[0] + 0.5 * radius*rand_vec[0],
						 position[1] + 0.5 * radius*rand_vec[1],
						 position[2] + 0.5 * radius*rand_vec[2]);
	//change my position to keep the center of mass intact and then see if I need to update my voxel index
	static double negative_one_half = -0.5; 
	naxpy( &position, negative_one_half , rand_vec );// position = position - 0.5*rand_vec; 
	// position[0] -= 0.5*radius*rand_vec[0];
	// position[1] -= 0.5*radius*rand_vec[1]; 
	// position[2] -= 0.5*radius*rand_vec[2]; 
	
	//If this cell has been moved outside of the boundaries, mark it as such.
	//(If the child cell is outside of the boundaries, that has been taken care of in the assign_position function.)
	if( !get_container()->underlying_mesh.is_position_valid(position[0], position[1], position[2])){
		is_out_of_domain = true;
		is_active = false;
		is_movable = false;
	}	
	 
	update_voxel_in_container();
	phenotype.volume.divide(); 
	child->phenotype.volume.divide();
	child->set_total_volume(child->phenotype.volume.total);
	set_total_volume(phenotype.volume.total);
	
	// child->set_phenotype( phenotype ); 
	child->phenotype = phenotype; 
	
	return child;
}

bool Cell::assign_position(std::vector<double> new_position)
{
	return assign_position(new_position[0], new_position[1], new_position[2]);
}

void Cell::set_previous_velocity(double xV, double yV, double zV)
{
	previous_velocity[0] = xV;
	previous_velocity[1] = yV;
	previous_velocity[2] = zV;

	return; 
}

bool Cell::assign_position(double x, double y, double z)
{
	position[0]=x;
	position[1]=y;
	position[2]=z;
	
	// update microenvironment current voxel index
	update_voxel_index();
	// update current_mechanics_voxel_index
	current_mechanics_voxel_index= get_container()->underlying_mesh.nearest_voxel_index( position );
	get_container()->register_agent(this);
	
	if( !get_container()->underlying_mesh.is_position_valid(x,y,z) )
	{	
		is_out_of_domain = true; 
		is_active = false; 
		is_movable = false; 
		
		return false;
	}
	
	return true;
}

void Cell::set_total_volume(double volume)
{
	Basic_Agent::set_total_volume(volume);
	
	// If the new volume is significantly different than the 
	// current total volume, adjust all the sub-volumes 
	// proportionally. 
	
	// if( fabs( phenotype.volume.total - volume ) < 1e-16 )
	if( fabs( phenotype.volume.total - volume ) > 1e-16 )
	{
		double ratio= volume/ phenotype.volume.total;
		phenotype.volume.multiply_by_ratio(ratio);
	}
	
	phenotype.geometry.update( this, phenotype, 0.0 ); 
	// phenotype.update_radius();
	//if( get_container()->max_cell_interactive_distance_in_voxel[get_current_mechanics_voxel_index()] < 
	//	phenotype.geometry.radius * parameters.max_interaction_distance_factor )
	if( get_container()->max_cell_interactive_distance_in_voxel[get_current_mechanics_voxel_index()] < 
		phenotype.geometry.radius * phenotype.mechanics.relative_maximum_adhesion_distance )
	{
		// get_container()->max_cell_interactive_distance_in_voxel[get_current_mechanics_voxel_index()]= phenotype.geometry.radius*parameters.max_interaction_distance_factor;
		get_container()->max_cell_interactive_distance_in_voxel[get_current_mechanics_voxel_index()] = phenotype.geometry.radius
			* phenotype.mechanics.relative_maximum_adhesion_distance;
	}
	
	return; 
}


void Cell::set_target_volume( double new_volume )
{
	
	// this function will keep the prior ratios (from targets)
	
	// first compute the actual raw totals on all these things 
	double old_target_solid = phenotype.volume.target_solid_nuclear + 
		phenotype.volume.target_solid_cytoplasmic; 
	double old_target_total = old_target_solid / ( 1.0 - phenotype.volume.target_fluid_fraction ); 
	double old_target_fluid = phenotype.volume.target_fluid_fraction * old_target_total; 
	
	// next whats the relative new size? 
	double ratio = new_volume / (1e-16 + old_target_total ); 
	
	// scale the target solid cyto and target solid nuclear by this ratio 
	phenotype.volume.target_solid_cytoplasmic *= ratio; 
	phenotype.volume.target_solid_nuclear *= ratio; 
	
	return; 
}

void Cell::set_target_radius(double new_radius )
{
	static double four_thirds_pi =  4.188790204786391;

	// calculate the new target volume 
	double new_volume = four_thirds_pi; 
	new_volume *= new_radius; 
	new_volume *= new_radius; 
	new_volume *= new_radius; 
	
	// now call the set_target_volume funciton 
	this->set_target_volume( new_volume ); 
	return; 
}

void Cell::set_radius(double new_radius )
{
	static double four_thirds_pi =  4.188790204786391;

	// calculate the new target volume 
	double new_volume = four_thirds_pi; 
	new_volume *= new_radius; 
	new_volume *= new_radius; 
	new_volume *= new_radius; 
	
	this->set_total_volume( new_volume ); 
	return; 
}



double& Cell::get_total_volume(void)
{
	static bool I_warned_you = false; 
	if( I_warned_you == false )
	{
		std::cout << "Warning! Do not use " << __FUNCTION__ << "!" << std::endl 
			<< "Use (some_cell).phenotype.volume.total instead!" << std::endl; 
		I_warned_you = true; 
	}
	return phenotype.volume.total; 
}

void Cell::turn_off_reactions(double dt)
{	
	is_active = false;  
	
	for(int i=0;i< phenotype.secretion.uptake_rates.size();i++)
	{
		phenotype.secretion.uptake_rates[i] = 0.0;  
		phenotype.secretion.secretion_rates[i] = 0.0; 
		phenotype.secretion.net_export_rates[i] = 0.0; 
	}
	set_internal_uptake_constants(dt);
	
	return; 
}

Cell_Container * Cell::get_container()
{
	if(container == NULL)
	{
		container = (Cell_Container *)get_microenvironment()->agent_container;
	}
	
	return container;
}

void Cell::die()
{
	delete_cell(this);
	return; 
} 

void Cell::update_position( double dt )
{
	// BioFVM Basic_Agent::update_position(dt) returns without doing anything. 
	// So we remove this to avoid any future surprises. 
	// 
	// Basic_Agent::update_position(dt);
		
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
	
	// new AUgust 2017
	if( default_microenvironment_options.simulate_2D == true )
	{ velocity[2] = 0.0; }
	
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
	{
		updated_current_mechanics_voxel_index=get_container()->underlying_mesh.nearest_voxel_index( position );
	}
	else
	{
		updated_current_mechanics_voxel_index=-1;
		
		is_out_of_domain = true; 
		is_active = false; 
		is_movable = false; 
	}
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
		if( get_current_mechanics_voxel_index() >= 0)
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
	
	return; 
}

void Cell::copy_data(Cell* copy_me)
{
	// phenotype=copyMe-> phenotype; //it is taken care in set_phenotype
	type = copy_me->type; 
	type_name = copy_me->type_name; 
	
	custom_data = copy_me->custom_data; 
	parameters = copy_me->parameters; 
	
	velocity = copy_me->velocity; 
	// expected_phenotype = copy_me-> expected_phenotype; //it is taken care in set_phenotype
	cell_source_sink_solver_temp1 = std::vector<double>(copy_me->cell_source_sink_solver_temp1);
	cell_source_sink_solver_temp2 = std::vector<double>(copy_me->cell_source_sink_solver_temp2);
	
	return; 
}

void Cell::copy_function_pointers(Cell* copy_me)
{
	functions = copy_me->functions; 
	return; 
}

void Cell::add_potentials(Cell* other_agent)
{
	if( this->ID == other_agent->ID )
	{ return; }

	// 12 uniform neighbors at a close packing distance, after dividing out all constants
	static double simple_pressure_scale = 0.027288820670331; // 12 * (1 - sqrt(pi/(2*sqrt(3))))^2 
	// 9.820170012151277; // 12 * ( 1 - sqrt(2*pi/sqrt(3)))^2

	double distance = 0; 
	for( int i = 0 ; i < 3 ; i++ ) 
	{ 
		displacement[i] = position[i] - (*other_agent).position[i]; 
		distance += displacement[i] * displacement[i]; 
	}
	// Make sure that the distance is not zero
	
	distance = std::max(sqrt(distance), 0.00001); 
	
	//Repulsive
	double R = phenotype.geometry.radius+ (*other_agent).phenotype.geometry.radius; 
	
	double RN = phenotype.geometry.nuclear_radius + (*other_agent).phenotype.geometry.nuclear_radius;	
	double temp_r, c;
	if( distance > R ) 
	{
		temp_r=0;
	}
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
		// temp_r = 1 - distance/R;
		temp_r = -distance; // -d
		temp_r /= R; // -d/R
		temp_r += 1.0; // 1-d/R
		temp_r *= temp_r; // (1-d/R)^2 
		
		// add the relative pressure contribution 
		state.simple_pressure += ( temp_r / simple_pressure_scale ); // New July 2017 
	}
	
	// August 2017 - back to the original if both have same coefficient 
	double effective_repulsion = sqrt( phenotype.mechanics.cell_cell_repulsion_strength * other_agent->phenotype.mechanics.cell_cell_repulsion_strength ); 
	temp_r *= effective_repulsion; 
	
	// temp_r *= phenotype.mechanics.cell_cell_repulsion_strength; // original 
	//////////////////////////////////////////////////////////////////
	
	// Adhesive
	//double max_interactive_distance = parameters.max_interaction_distance_factor * phenotype.geometry.radius + 
	//	(*other_agent).parameters.max_interaction_distance_factor * (*other_agent).phenotype.geometry.radius;
		
	double max_interactive_distance = phenotype.mechanics.relative_maximum_adhesion_distance * phenotype.geometry.radius + 
		(*other_agent).phenotype.mechanics.relative_maximum_adhesion_distance * (*other_agent).phenotype.geometry.radius;
		
	if(distance < max_interactive_distance ) 
	{	
		// double temp_a = 1 - distance/max_interactive_distance; 
		double temp_a = -distance; // -d
		temp_a /= max_interactive_distance; // -d/S
		temp_a += 1.0; // 1 - d/S 
		temp_a *= temp_a; // (1-d/S)^2 
		// temp_a *= phenotype.mechanics.cell_cell_adhesion_strength; // original 
		
		// August 2017 - back to the original if both have same coefficient 
		double effective_adhesion = sqrt( phenotype.mechanics.cell_cell_adhesion_strength * other_agent->phenotype.mechanics.cell_cell_adhesion_strength ); 
		temp_a *= effective_adhesion; 
		
		temp_r -= temp_a;
	}
	/////////////////////////////////////////////////////////////////
	if( fabs(temp_r) < 1e-16 )
	{ return; }
	temp_r /= distance;
	// for( int i = 0 ; i < 3 ; i++ ) 
	// {
	//	velocity[i] += displacement[i] * temp_r; 
	// }
	axpy( &velocity , temp_r , displacement ); 
	
	return;
}

Cell* create_cell( void )
{
	Cell* pNew; 
	pNew = new Cell;		
	(*all_cells).push_back( pNew ); 
	pNew->index=(*all_cells).size()-1;
	
	// new usability enhancements in May 2017 
	
	if( BioFVM::get_default_microenvironment() )
	{
		pNew->register_microenvironment( BioFVM::get_default_microenvironment() );
	}

	// All the phenotype and other data structures are already set 
	// by virtue of the default Cell constructor. 
	
	pNew->set_total_volume( pNew->phenotype.volume.total ); 
	
	return pNew; 
}

// in the future, I might swap this with create_cell(): 
// In that "create_cell()" uses "create_cell( cell_defaults )" 
Cell* create_cell( Cell_Definition& cd )
{
	Cell* pNew = create_cell(); 
	
	// use the cell defaults; 
	pNew->type = cd.type; 
	pNew->type_name = cd.name; 
	
	pNew->custom_data = cd.custom_data; 
	pNew->parameters = cd.parameters; 
	pNew->functions = cd.functions; 
	
	pNew->phenotype = cd.phenotype; 
	pNew->is_movable = true;
	pNew->is_out_of_domain = false;
	pNew->displacement.resize(3,0.0); // state? 
	
	pNew->assign_orientation();
	
	pNew->set_total_volume( pNew->phenotype.volume.total ); 
	
	return pNew; 
}

void Cell::convert_to_cell_definition( Cell_Definition& cd )
{
	
	// use the cell defaults; 
	type = cd.type; 
	type_name = cd.name; 
	
	custom_data = cd.custom_data; 
	parameters = cd.parameters; 
	functions = cd.functions; 
	
	phenotype = cd.phenotype; 
	// is_movable = true;
	// is_out_of_domain = false;
	
	// displacement.resize(3,0.0); // state? 
	
	assign_orientation();	
	
	set_total_volume( phenotype.volume.total ); 
	
	return; 
}

void delete_cell( int index )
{
	// released internalized substrates (as of 1.5.x releases)
	(*all_cells)[index]->release_internalized_substrates(); 
	
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

bool is_neighbor_voxel(Cell* pCell, std::vector<double> my_voxel_center, std::vector<double> other_voxel_center, int other_voxel_index)
{
	double max_interactive_distance = pCell->phenotype.mechanics.relative_maximum_adhesion_distance * pCell->phenotype.geometry.radius 
		+ pCell->get_container()->max_cell_interactive_distance_in_voxel[other_voxel_index];
	
	int comparing_dimension = -1, comparing_dimension2 = -1;
	if(my_voxel_center[0] == other_voxel_center[0] && my_voxel_center[1] == other_voxel_center[1])
	{
		comparing_dimension = 2;
	}
	else if(my_voxel_center[0] == other_voxel_center[0] && my_voxel_center[2] == other_voxel_center[2])
	{
		comparing_dimension = 1;
	}
	else if(my_voxel_center[1] == other_voxel_center[1] && my_voxel_center[2] == other_voxel_center[2])
	{
		comparing_dimension = 0;
	}
	
	if(comparing_dimension != -1) 
	{ //then it is an immediate neighbor (through side faces)
		double surface_coord= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		if(std::fabs(pCell->position[comparing_dimension] - surface_coord) > max_interactive_distance)
		{ return false; }
		return true;
	}
	comparing_dimension=-1;
	
	if(my_voxel_center[0] == other_voxel_center[0])
	{
		comparing_dimension = 1; comparing_dimension2 = 2;
	}
	else if(my_voxel_center[1] == other_voxel_center[1])
	{
		comparing_dimension=0; comparing_dimension2 = 2;
	}
	else if(my_voxel_center[2] == other_voxel_center[2])
	{
		comparing_dimension = 0; comparing_dimension2=1;
	}
	if(comparing_dimension != -1)
	{
		double line_coord1= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		double line_coord2= 0.5*(my_voxel_center[comparing_dimension2] + other_voxel_center[comparing_dimension2]);
		double distance_squared= std::pow( pCell->position[comparing_dimension] - line_coord1,2)+ std::pow( pCell->position[comparing_dimension2] - line_coord2,2);
		if(distance_squared > max_interactive_distance * max_interactive_distance)
		{ return false; }
		return true;
	}
	std::vector<double> corner_point= 0.5*(my_voxel_center+other_voxel_center);
	double distance_squared= (corner_point[0]-pCell->position[0])*(corner_point[0]-pCell->position[0])
		+(corner_point[1]-pCell->position[1])*(corner_point[1]-pCell->position[1]) 
		+(corner_point[2]-pCell->position[2]) * (corner_point[2]-pCell->position[2]);
	if(distance_squared > max_interactive_distance * max_interactive_distance)
	{ return false; }
	return true;
}

std::vector<Cell*>& Cell::cells_in_my_container( void )
{
	return get_container()->agent_grid[get_current_mechanics_voxel_index()];
}

void Cell::ingest_cell( Cell* pCell_to_eat )
{
	// absorb all the volume(s)

	// absorb fluid volume (all into the cytoplasm) 
	phenotype.volume.cytoplasmic_fluid += pCell_to_eat->phenotype.volume.fluid; 
	pCell_to_eat->phenotype.volume.cytoplasmic_fluid = 0.0; 
	
	// absorb nuclear and cyto solid volume (into the cytoplasm) 
	phenotype.volume.cytoplasmic_solid += pCell_to_eat->phenotype.volume.cytoplasmic_solid; 
	pCell_to_eat->phenotype.volume.cytoplasmic_solid = 0.0; 
	
	phenotype.volume.cytoplasmic_solid += pCell_to_eat->phenotype.volume.nuclear_solid; 
	pCell_to_eat->phenotype.volume.nuclear_solid = 0.0; 
	
	// consistency calculations 
	
	phenotype.volume.fluid = phenotype.volume.nuclear_fluid + 
		phenotype.volume.cytoplasmic_fluid; 
	pCell_to_eat->phenotype.volume.fluid = 0.0; 
	
	phenotype.volume.solid = phenotype.volume.cytoplasmic_solid + 
		phenotype.volume.nuclear_solid; 
	pCell_to_eat->phenotype.volume.solid = 0.0; 
	
	// no change to nuclear volume (initially) 
	pCell_to_eat->phenotype.volume.nuclear = 0.0; 
	pCell_to_eat->phenotype.volume.nuclear_fluid = 0.0; 
	
	phenotype.volume.cytoplasmic = phenotype.volume.cytoplasmic_solid + 
		phenotype.volume.cytoplasmic_fluid; 
	pCell_to_eat->phenotype.volume.cytoplasmic = 0.0; 
	
	phenotype.volume.total = phenotype.volume.nuclear + 
		phenotype.volume.cytoplasmic; 
	pCell_to_eat->phenotype.volume.total = 0.0; 

	phenotype.volume.fluid_fraction = phenotype.volume.fluid / 
		(  phenotype.volume.total ); 
	pCell_to_eat->phenotype.volume.fluid_fraction = 0.0; 

	phenotype.volume.cytoplasmic_to_nuclear_ratio = phenotype.volume.cytoplasmic_solid / 
		( phenotype.volume.nuclear_solid + 1e-16 );
		
	// update corresponding BioFVM parameters (self-consistency) 
	set_total_volume( phenotype.volume.total ); 
	pCell_to_eat->set_total_volume( 0.0 ); 
	
	// absorb the internalized substrates 
	
	// multiply by the fraction that is supposed to be ingested (for each substrate) 
	*(pCell_to_eat->internalized_substrates) *= 
		*(pCell_to_eat->fraction_transferred_when_ingested); // 
	
	*internalized_substrates += *(pCell_to_eat->internalized_substrates); 
	static int n_substrates = internalized_substrates->size(); 
	pCell_to_eat->internalized_substrates->assign( n_substrates , 0.0 ); 	
	
	// trigger removal from the simulation 
	// pCell_to_eat->die(); // I don't think this is safe if it's in an OpenMP loop 
	// flag it for removal 
	pCell_to_eat->flag_for_removal(); 
	// mark it as dead 
	pCell_to_eat->phenotype.death.dead = true; 
	// set secretion and uptake to zero 
	pCell_to_eat->phenotype.secretion.set_all_secretion_to_zero( );  
	pCell_to_eat->phenotype.secretion.set_all_uptake_to_zero( ); 

	
	// deactivate all custom function 
	pCell_to_eat->functions.custom_cell_rule = NULL; 
	pCell_to_eat->functions.update_phenotype = NULL; 
	pCell_to_eat->functions.contact_function = NULL; 
	
	// set it to zero mechanics 
	pCell_to_eat->functions.custom_cell_rule = NULL; 
	
	return; 
}

void Cell::lyse_cell( void )
{
	flag_for_removal(); 
	// mark it as dead 
	phenotype.death.dead = true; 
	// set secretion and uptake to zero 
	phenotype.secretion.set_all_secretion_to_zero( );  
	phenotype.secretion.set_all_uptake_to_zero( ); 
	
	// deactivate all custom function 
	functions.custom_cell_rule = NULL; 
	functions.update_phenotype = NULL; 
	functions.contact_function = NULL; 
	
	// set it to zero mechanics 
	functions.custom_cell_rule = NULL; 

	return; 
}

bool cell_definitions_by_name_constructed = false; 

void build_cell_definitions_maps( void )
{
//	cell_definitions_by_name.
//	cell_definitions_by_index

	for( int n=0; n < cell_definitions_by_index.size() ; n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		cell_definitions_by_name[ pCD->name ] = pCD; 
		cell_definitions_by_type[ pCD->type ] = pCD; 
	}

/*
	for( const auto& n : cell_definitions_by_name )
	{
		std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
	}	
	std::cout << std::endl << std::endl;
	for( const auto& n : cell_definitions_by_type )
	{
		std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
	}	
	std::cout << std::endl << std::endl;
*/
	cell_definitions_by_name_constructed = true; 
	
	return;
}

void display_ptr_as_bool( void (*ptr)(Cell*,Phenotype&,double), std::ostream& os )
{
	if( ptr )
	{ os << "true"; return; }
	os << "false"; 
	return;
}

void display_cell_definitions( std::ostream& os )
{
	for( int n=0; n < cell_definitions_by_index.size() ; n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		os << n << " :: type:" << pCD->type << " name: " << pCD->name << std::endl; 

		if( pCD->phenotype.cycle.pCycle_Model != NULL )
		{
			os << "\t cycle model: " << pCD->phenotype.cycle.model().name  
				<< " (code=" << pCD->phenotype.cycle.model().code << ")" << std::endl; 
			os << "\t death models: " << std::endl; 
		}
		else
		{ 	os << "\t cycle model not initialized" << std::endl; } 

		for( int k=0 ; k < pCD->phenotype.death.models.size(); k++ )
		{
			os << "\t\t" << k << " : " << pCD->phenotype.death.models[k]->name 
			<< " (code=" << pCD->phenotype.death.models[k]->code << ")" << std::endl; 
		}
		
		Cell_Functions* pCF = &(pCD->functions); 
		os << "\t key functions: " << std::endl; 
		os << "\t\t migration: "; display_ptr_as_bool( pCF->update_migration_bias , std::cout ); 
		os << std::endl; 
		os << "\t\t custom rule: "; display_ptr_as_bool( pCF->custom_cell_rule , std::cout ); 
		os << std::endl; 
		os << "\t\t phenotype rule: "; display_ptr_as_bool( pCF->update_phenotype , std::cout ); 
		os << std::endl; 
		
		Custom_Cell_Data* pCCD = &(pCD->custom_data); 
		os << "\tcustom data: " << std::endl; 
		for( int k=0; k < pCCD->variables.size(); k++)
		{
			os << "\t\t" << pCCD->variables[k].name << std::endl; 
		}
		os << "\tcustom vector data: " << std::endl; 
		for( int k=0; k < pCCD->vector_variables.size(); k++)
		{
			os << "\t\t" << pCCD->vector_variables[k].name << std::endl; 
		}
		os << "\t\t\tNOTE: custom vector data will eventually be merged with custom data" << std::endl; 
			
	}
	
	return; 
}

Cell_Definition* find_cell_definition( std::string search_string )
{
	// if the registry isn't built yet, then do it! 
	if( cell_definitions_by_name_constructed == false )
	{
		build_cell_definitions_maps(); 
	}
	
	Cell_Definition* output = NULL;
	if( cell_definitions_by_name.count( search_string ) > 0 )
	{ 
		output = cell_definitions_by_name.find( search_string )->second; 
	} 
	
	if( output == NULL )
	{
		std::cout << "Warning! Cell_Definition for " << search_string << " not found!" << std::endl; 
	}
	
	return output; 	
}

Cell_Definition* find_cell_definition( int search_type )
{
	// if the registry isn't built yet, then do it! 
	if( cell_definitions_by_name_constructed == false )
	{
		build_cell_definitions_maps(); 
	}
	
	Cell_Definition* output = NULL;
	if( cell_definitions_by_type.count( search_type ) > 0 )
	{ 
		output = cell_definitions_by_type.find( search_type )->second; 
	} 
	
	if( output == NULL )
	{
		std::cout << "Warning! Cell_Definition for " << search_type << " not found!" << std::endl; 
	}
	
	return output; 	
}




};

