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
# Copyright (c) 2015-2024, Paul Macklin and the PhysiCell Project             #
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

#include "./PhysiCell_phenotype.h"

#include "../BioFVM/BioFVM.h"
#include "./PhysiCell_constants.h"
#include "./PhysiCell_utilities.h"

using namespace BioFVM; 

namespace PhysiCell{
	
Phase::Phase()
{
	index = 0; 
	code = 0; 
	name = "unnamed"; 
	
	division_at_phase_exit = false; 
	removal_at_phase_exit = false; 
	
	entry_function = NULL; 
	return; 
}
	
Phase_Link::Phase_Link()
{
	start_phase_index = 0; 
	end_phase_index = 0; 
	
	fixed_duration = false; 
	
	arrest_function = NULL; 
	exit_function = NULL; 
	return; 
}

Cycle_Data::Cycle_Data()
{
	inverse_index_maps.resize(0); 
	
	pCycle_Model = NULL; 

	time_units = "min"; 
	
	transition_rates.resize( 0 );

	current_phase_index = 0; 
	elapsed_time_in_phase = 0.0; 
	return; 
}

void Cycle_Data::sync_to_cycle_model( void )
{
	// make sure the inverse map is the right size 
	int n = pCycle_Model->phases.size(); 
	inverse_index_maps.resize( n );
	
	// sync the inverse map to the cell cycle model by 
	// querying the phase_links 

	transition_rates.resize( n );
	
	// also make sure the transition_rates[] are the right size 
	
	for( int i=0 ; i < pCycle_Model->phase_links.size() ; i++ )
	{
		inverse_index_maps[i].clear(); 
		for( int j=0 ; j < pCycle_Model->phase_links[i].size() ; j++ )
		{
			inverse_index_maps[i][ pCycle_Model->phase_links[i][j].end_phase_index ] = j;
			transition_rates[i].resize( pCycle_Model->phase_links[i].size() ); 
		}
	}

	return; 
}

double& Cycle_Data::transition_rate( int start_phase_index , int end_phase_index )
{
	return transition_rates[ start_phase_index ][ inverse_index_maps[start_phase_index][end_phase_index] ]; 
}

double& Cycle_Data::exit_rate(int phase_index )
{
	return transition_rates[phase_index][0]; 
}
	
Cycle_Model::Cycle_Model()
{
	inverse_index_maps.resize( 0 );
	
	name = "unnamed";
	
	phases.resize(0); 
	phase_links.resize(0); 
	
	data.pCycle_Model = this; 
	
	code = PhysiCell_constants::custom_cycle_model; 
	default_phase_index = 0; 
	
	return; 
}	
	
int Cycle_Model::add_phase( int code, std::string name )
{
	int n = phases.size();
	
	// resize the data structures 
	phases.resize( n+1 ); 
	phase_links.resize( n+1 );
	phase_links[n].resize(0);
	
	inverse_index_maps.resize( n+1 );
	inverse_index_maps[n].clear(); 
	
	// update phase n
	phases[n].code = code; 
	phases[n].index = n; 
	
	phases[n].name.assign( name ); 
	
	// make sure the cycle_data is also correctly sized 
	
	data.sync_to_cycle_model();
	
	return n; 
}
	
int Cycle_Model::add_phase_link( int start_index, int end_index , 
	bool (*arrest_function)(Cell* pCell, Phenotype& phenotype, double dt) )
{
	// first, resize the phase links 
	int n = phase_links[start_index].size(); 
	phase_links[start_index].resize( n + 1 );
	
	// now, update the new phase links
	phase_links[start_index][n].start_phase_index  = start_index; 
	phase_links[start_index][n].end_phase_index = end_index; 
	phase_links[start_index][n].arrest_function = arrest_function; 
	
	// now, update the inverse index map 
	inverse_index_maps[start_index][end_index] = n; 
	
	// lastly, make sure the transition rates are the right size;
	
	data.sync_to_cycle_model(); 

	return n; 
}
	
int Cycle_Model::add_phase_link( int start_index, int end_index , double rate , 
	bool (*arrest_function)(Cell* pCell, Phenotype& phenotype, double dt) )
{
	int n = add_phase_link( start_index , end_index , arrest_function );
	data.transition_rate( start_index , end_index ) = rate; 
	return n;
}

int Cycle_Model::find_phase_index( int code )
{
	for( int i=0 ; i < phases.size() ; i++ )
	{
		if( phases[i].code == code )
		{ return i; }
	}
	return 0; 
}

int Cycle_Model::find_phase_index( std::string name )
{
	for( int i=0 ; i < phases.size() ; i++ )
	{
		if( phases[i].name == name )
		{ return i; }
	}
	return 0; 
}
	
std::ostream& Cycle_Model::display( std::ostream& os )
{
	os << "Cycle Model: " << name << " (PhysiCell code: " << code << ")" << std::endl; 
	os << "Phases and links: (* denotes phase with cell division)" << std::endl;
	for( int i=0; i < phases.size() ; i++ )
	{
		os << "Phase " << i << " (" << phases[i].name << ") "; 
		
		if( phases[i].division_at_phase_exit )
		{ os << "*"; }
		os << " links to: " << std::endl;
		for( int k=0 ; k < phase_links[i].size() ; k++ )
		{
			int j = phase_links[i][k].end_phase_index; 
			os << "\tPhase " << j << " (" << phases[j].name << ") with rate " << data.transition_rate(i,j) << " " << data.time_units << "^-1; " << std::endl; 
		}
		os << std::endl; 
	}
	
	return os; 
}
	
double& Cycle_Model::transition_rate( int start_index , int end_index )
{
	return data.transition_rate( start_index , end_index ); 
}

Phase_Link& Cycle_Model::phase_link( int start_index, int end_index )
{
	return phase_links[start_index][ inverse_index_maps[start_index][end_index] ]; 
}
	
void Cycle_Model::advance_model( Cell* pCell, Phenotype& phenotype, double dt )
{
	int i = phenotype.cycle.data.current_phase_index; 
	
	phenotype.cycle.data.elapsed_time_in_phase += dt; 

	// Evaluate each linked phase: 
	// advance to that phase IF probabiltiy is in the range, 
	// and if the arrest function (if any) is false 
	
	int j; 
	for( int k=0 ; k < phase_links[i].size() ; k++ )
	{
		j = phase_links[i][k].end_phase_index; 
		
		// check for arrest. If arrested, skip to the next transition
		bool transition_arrested = false; 
		if( phase_links[i][k].arrest_function )
		{
			transition_arrested = phase_links[i][k].arrest_function( pCell,phenotype,dt ); 
		}
		if( !transition_arrested )
		{
			// check to see if we should transition 
			bool continue_transition = false; 
			if( phase_links[i][k].fixed_duration )
			{
				if( phenotype.cycle.data.elapsed_time_in_phase > ((1.0/phenotype.cycle.data.transition_rates[i][k]) - 0.5 * dt) )
				{
					continue_transition = true; 
				}
			}
			else
			{
				double prob = phenotype.cycle.data.transition_rates[i][k]*dt; 
				if( UniformRandom() < prob )
				{
					continue_transition = true; 
				}
			}
			
			// if we should transition, check if we're not supposed to divide or die 
			
			if( continue_transition )
			{
				// if the phase transition has an exit function, execute it
				if( phase_links[i][k].exit_function )
				{
					phase_links[i][k].exit_function( pCell,phenotype,dt ); 
				}
				
				// check if division or removal are required 
				if( phases[i].division_at_phase_exit )
				{
					// pCell->flag_for_division();
					phenotype.flagged_for_division = true; 
				}
				if( phases[i].removal_at_phase_exit )
				{
					// pCell->flag_for_removal(); 
					phenotype.flagged_for_removal = true; 
					return; 
				}
				// move to the next phase, and reset the elapsed time 
				phenotype.cycle.data.current_phase_index = j; 
				phenotype.cycle.data.elapsed_time_in_phase = 0.0; 
				
				// if the new phase has an entry function, execute it 
				if( phases[j].entry_function )
				{
					phases[j].entry_function( pCell,phenotype,dt );  
				}
				
				return; 
			}
			
		}
		
	}
	
	return; 
}
	
Phase& Cycle_Data::current_phase( void )
{
	return pCycle_Model->phases[current_phase_index]; 
}

Death_Parameters::Death_Parameters()
{
	time_units = "min"; 
	
	// reference values: MCF-7 (1/min)
	unlysed_fluid_change_rate = 3.0/60.0; // apoptosis 
	lysed_fluid_change_rate = 0.05/60.0; // lysed necrotic cell 
	
	cytoplasmic_biomass_change_rate = 1.0/60.0; // apoptosis 
	nuclear_biomass_change_rate = 0.35/60.0; // apoptosis 
	
	calcification_rate = 0.0; // 0.0042 for necrotic cells 
	
	relative_rupture_volume = 2.0; 

	return; 
}
	
Death::Death()
{
	rates.resize( 0 ); 
	models.resize( 0 ); 
	parameters.resize( 0 ); 
	
	dead = false; 
	current_death_model_index = 0;
	
	return; 
}

int Death::add_death_model( double rate , Cycle_Model* pModel )
{
	rates.push_back( rate );
	models.push_back( pModel ); 
	
	parameters.resize( rates.size() ); 
	
	return rates.size() - 1; 
}

int Death::add_death_model( double rate, Cycle_Model* pModel, Death_Parameters& death_parameters)
{
	rates.push_back( rate );
	models.push_back( pModel ); 
	parameters.push_back( death_parameters ); 
	
	return rates.size() - 1; 
}

int Death::find_death_model_index( int code )
{
	for( int i=0 ; i < models.size() ; i++ )
	{
		if( models[i]->code == code )
		{ return i; }
	}
	return 0; 
}

int Death::find_death_model_index( std::string name )
{
	for( int i=0 ; i < models.size() ; i++ )
	{
		if( models[i]->name == name )
		{ return i; }
	}
	return 0; 
}
	
bool Death::check_for_death( double dt )
{
	// If the cell is already dead, exit. 
	if( dead == true )
	{
		return false;
	} 
	
	// If the cell is alive, evaluate all the 
	// death rates for each registered death type. 
	int i = 0; 
	while( !dead && i < rates.size() )
	{
		if( UniformRandom() < rates[i]*dt )
		{
			// update the Death data structure 
			dead = true; 
			current_death_model_index = i; 
			
			// and set the cycle model to this death model 
			
			return dead;
		}
		i++; 
	}
	
	return dead; 
}

void Death::trigger_death( int death_model_index )
{
	dead = true; 
	current_death_model_index = death_model_index; 
	
/*	
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
*/
	
	return; 
}

Cycle_Model& Death::current_model( void )
{
	return *models[current_death_model_index]; 
}

double& Death::apoptosis_rate(void)
{
	static int nApoptosis = find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	return rates[nApoptosis];
}

double& Death::necrosis_rate(void)
{
	static int nNecrosis = find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
	return rates[nNecrosis];
}


Cycle::Cycle()
{
	pCycle_Model = NULL; 
	return; 
}

void Cycle::advance_cycle( Cell* pCell, Phenotype& phenotype, double dt )
{
	pCycle_Model->advance_model( pCell, phenotype , dt ); 
	return; 
}

Cycle_Model& Cycle::model( void )
{
	return *pCycle_Model; 
}

Phase& Cycle::current_phase( void )
{
	return data.current_phase(); 
}

int& Cycle::current_phase_index( void )
{
	return data.current_phase_index; 
}

void Cycle::sync_to_cycle_model( Cycle_Model& cm )
{
	pCycle_Model = &cm; 
	data = cm.data; 
	return; 
}	

Death_Parameters& Death::current_parameters( void )
{
	return parameters[ current_death_model_index ]; 
}
	
Volume::Volume()
{
	// reference parameter values for MCF-7, in cubic microns 
	fluid_fraction = 0.75;  

	total = 2494; 
	fluid = fluid_fraction * total; 
	solid = total-fluid; 
	
	nuclear = 540.0;

	
	nuclear_fluid = fluid_fraction * nuclear; 
	nuclear_solid = nuclear - nuclear_fluid;

	cytoplasmic = total - nuclear;
	cytoplasmic_fluid = fluid_fraction*cytoplasmic; 
	cytoplasmic_solid = cytoplasmic - cytoplasmic_fluid; 
	
	// rates are in units of 1/min 
	cytoplasmic_biomass_change_rate = 0.27 / 60.0; 
	nuclear_biomass_change_rate = 0.33 / 60.0; 
	fluid_change_rate = 3.0 / 60.0;

	calcified_fraction = 0.0;
	
	calcification_rate = 0.0; 
	
	target_solid_cytoplasmic = cytoplasmic_solid;
	target_solid_nuclear = nuclear_solid;
	target_fluid_fraction = fluid_fraction;
	
	cytoplasmic_to_nuclear_ratio = cytoplasmic / ( 1e-16 + nuclear);
	target_cytoplasmic_to_nuclear_ratio = cytoplasmic_to_nuclear_ratio; 
	
	// the cell bursts at these volumes 
	relative_rupture_volume = 2.0; 
		// as fraction of volume at entry to the current phase
	rupture_volume = relative_rupture_volume * total; // in volume units 
	
	return; 
};


void Volume::multiply_by_ratio( double ratio )
{
	total *= ratio;
	solid *= ratio;
	fluid *= ratio;
	
	nuclear *= ratio;
	nuclear_fluid *= ratio;
	nuclear_solid *= ratio;
	
	cytoplasmic *= ratio;
	cytoplasmic_fluid *= ratio;
	cytoplasmic_solid *= ratio;
	
	rupture_volume *= ratio; 
	
	target_solid_nuclear *= ratio;
	target_solid_cytoplasmic *= ratio; 	
	
	return; 
}

void Volume::divide( void )
{
	multiply_by_ratio( 0.5 ); 
	return; 
}

Geometry::Geometry()
{
	// reference values for MCF-7, based on 
	// volume = 2494 cubic microns
	// nuclear volume = 540 cubic microns 
	radius = 8.412710547954228; 
	nuclear_radius = 5.051670902881889; 
	surface_area = 889.3685284131693; 
	
	polarity = 0.0; 
	return; 
}

void Geometry::update_radius( Cell* pCell, Phenotype& phenotype, double dt )
{
	static double four_thirds_pi =  4.188790204786391;
	radius = phenotype.volume.total; 
	radius /= four_thirds_pi; 
	radius = pow( radius , 0.333333333333333333333333333333333333333 ); 
	return; 
}

void Geometry::update_nuclear_radius( Cell* pCell, Phenotype& phenotype, double dt )
{
	static double four_thirds_pi = 4.188790204786391;
	nuclear_radius = phenotype.volume.nuclear; 
	nuclear_radius /= four_thirds_pi; 
	nuclear_radius = pow( nuclear_radius , 0.333333333333333333333333333333333333333 ); 
	return; 
}

void Geometry::update_surface_area( Cell* pCell, Phenotype& phenotype, double dt )
{
	// 4pi / (4pi/3)^(2/3)
	static double the_constant = 4.835975862049409; 
	surface_area = pow( phenotype.volume.total , 0.666666666666667 );
	surface_area /= the_constant; 
	
	return; 
}

void Geometry::update( Cell* pCell, Phenotype& phenotype, double dt )
{
	update_radius(pCell,phenotype,dt); 
	update_nuclear_radius(pCell,phenotype,dt);
	
	// surface area = 4*pi*r^2 = (4/3)*pi*r^3 / (r/3)	
	surface_area = phenotype.volume.total; 
	surface_area /= radius; 
	surface_area *= 3.0; 
	return; 
}
	
Mechanics::Mechanics()
{
	cell_cell_adhesion_strength = 0.4; 
	cell_BM_adhesion_strength = 4.0;
	
	cell_cell_repulsion_strength = 10.0; 
	cell_BM_repulsion_strength = 100.0; 

	cell_adhesion_affinities = {1}; 
	
	// this is a multiple of the cell (equivalent) radius
	relative_maximum_adhesion_distance = 1.25; 
	// maximum_adhesion_distance = 0.0; 

	/* for spring attachments */
	maximum_number_of_attachments = 12;
	attachment_elastic_constant = 0.01; 

	attachment_rate = 0; // 10.0 prior ot March 2023
	detachment_rate = 0; 

	/* to be deprecated */ 
	relative_maximum_attachment_distance = relative_maximum_adhesion_distance;
	relative_detachment_distance = relative_maximum_adhesion_distance;

	maximum_attachment_rate = 1.0; 
		
	return; 
}

void Mechanics::sync_to_cell_definitions()
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int number_of_cell_defs = cell_definition_indices_by_name.size(); 
	
	if( cell_adhesion_affinities.size() != number_of_cell_defs )
	{ cell_adhesion_affinities.resize( number_of_cell_defs, 1.0); }
	return; 
}

double& Mechanics::cell_adhesion_affinity( std::string type_name )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	return cell_adhesion_affinities[n]; 
}

void Mechanics::set_fully_heterotypic( void )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int number_of_cell_defs = cell_definition_indices_by_name.size(); 	

	cell_adhesion_affinities.assign( number_of_cell_defs, 1.0);
	return; 
}

void Mechanics::set_fully_homotypic( Cell* pC )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int number_of_cell_defs = cell_definition_indices_by_name.size(); 	

	cell_adhesion_affinities.assign( number_of_cell_defs, 0.0);

	// now find my type and set to 1 
//	cell_adhesion_affinity( pC->type_name ) = 1.0; 

	return; 
}


// new on July 29, 2018
// change the ratio without changing the repulsion strength or equilibrium spacing 
void Mechanics::set_relative_maximum_adhesion_distance( double new_value )
{
	// get old equilibrium spacing, based on equilibriation of pairwise adhesive/repulsive forces at that distance. 
	
		// relative equilibrium spacing (relative to mean cell radius)
	double s_relative = 2.0; 
	
	double temp1 = cell_cell_adhesion_strength; 
	temp1 /= cell_cell_repulsion_strength;
	temp1 = sqrt( temp1 ); 
	
	double temp2 = 1.0; 
	temp2 -= temp1; //  1 - sqrt( alpha_CCA / alpha_CCR );
	
	
	s_relative *= temp2; // 2*( 1 - sqrt( alpha_CCA / alpha_CCR ) ); 
	
	temp1 /= relative_maximum_adhesion_distance; // sqrt( alpha_CCA / alpha_CCR)/f;
	temp2 = 1.0; 
	temp2 -= temp1; // 1 - sqrt( alpha_CCA / alpha_CCR )/f;

	s_relative /= temp2; // 2*( 1 - sqrt( alpha_CCA / alpha_CCR ) ) / ( 1-1/f) ; 
	
	// now, adjust the relative max adhesion distance 
	
	relative_maximum_adhesion_distance = new_value; 
	
	// adjust the adhesive coefficient to preserve the old equilibrium distance

	temp1 = s_relative; 
	temp1 /= 2.0; 
	
	temp2 = 1.0;
	temp2 -= temp1; // 1 - s_relative/2.0 
	
	temp1 /= relative_maximum_adhesion_distance; // s_relative/(2*relative_maximum_adhesion_distance); 
	temp1 *= -1.0; // -s_relative/(2*relative_maximum_adhesion_distance); 
	temp1 += 1.0; // 1.0 -s_relative/(2*relative_maximum_adhesion_distance); 
	
	temp2 /= temp1; 
	temp2 *= temp2; 
	
	cell_cell_adhesion_strength = cell_cell_repulsion_strength;
	cell_cell_adhesion_strength *= temp2; 

	return; 
}		
		
// new on July 29, 2018
// set the cell-cell equilibrium spacing, accomplished by changing the 
// cell-cell adhesion strength, while leaving the cell-cell repulsion 
// strength and the maximum adhesion distance unchanged 
void Mechanics::set_relative_equilibrium_distance( double new_value )
{
	if( new_value > 2.0 )
	{
		std::cout << "**** Warning in function " << __FUNCTION__ << " in " << __FILE__ << " : " << std::endl 
			<< "\tAttempted to set equilibrium distance exceeding two cell radii." << std::endl
			<< "\tWe will cap the equilibrium distance at 2.0 cell radii." << std::endl 
			<< "****" << std::endl << std::endl; 
			
			new_value = 2.0; 
	}
 
	// adjust the adhesive coefficient to achieve the new (relative) equilibrium distance

	double temp1 = new_value; 
	temp1 /= 2.0; 
	
	double temp2 = 1.0;
	temp2 -= temp1; // 1 - s_relative/2.0 
	
	temp1 /= relative_maximum_adhesion_distance; // s_relative/(2*relative_maximum_adhesion_distance); 
	temp1 *= -1.0; // -s_relative/(2*relative_maximum_adhesion_distance); 
	temp1 += 1.0; // 1.0 -s_relative/(2*relative_maximum_adhesion_distance); 
	
	temp2 /= temp1; 
	temp2 *= temp2; 
	
	cell_cell_adhesion_strength = cell_cell_repulsion_strength;
	cell_cell_adhesion_strength *= temp2; 

	return; 
}

void Mechanics::set_absolute_equilibrium_distance( Phenotype& phenotype, double new_value )
{
	return set_relative_equilibrium_distance( new_value / phenotype.geometry.radius ); 
}

// void Mechanics::set_absolute_maximum_adhesion_distance( double new_value );
// void 
	
	
Motility::Motility()
{
	is_motile = false; 
	
	persistence_time = 1.0;
	migration_speed = 1.0;
	
	migration_bias_direction.resize( 3 , 0.0 ); 
	migration_bias = 0.0; 
		
	restrict_to_2D = false; 
	
	// update_migration_bias_direction = NULL; 
	
	motility_vector.resize( 3 , 0.0 ); 
	
	chemotaxis_index = 0; 
	chemotaxis_direction = 1; 
	
	sync_to_current_microenvironment(); 
	
	return; 
}

void Motility::sync_to_current_microenvironment( void )
{
	Microenvironment* pMicroenvironment = get_default_microenvironment(); 
	if( pMicroenvironment )
	{ sync_to_microenvironment( pMicroenvironment ); } 
	else
	{ chemotactic_sensitivities.resize( 1 , 0.0 ); }

	return; 
}

void Motility::sync_to_microenvironment( Microenvironment* pNew_Microenvironment )
{
	chemotactic_sensitivities.resize( pNew_Microenvironment->number_of_densities() , 0.0 ); 
	return; 
}

double& Motility::chemotactic_sensitivity( std::string name )
{
	int n = microenvironment.find_density_index(name); 
	return chemotactic_sensitivities[n]; 
}

Secretion::Secretion()
{
	pMicroenvironment = get_default_microenvironment(); 
	
	sync_to_current_microenvironment(); 
	return; 
}

void Secretion::sync_to_current_microenvironment( void )
{
	if( pMicroenvironment )
	{
		sync_to_microenvironment( pMicroenvironment ); 
	}
	else
	{
		secretion_rates.resize( 0 , 0.0 ); 
		uptake_rates.resize( 0 , 0.0 ); 
		saturation_densities.resize( 0 , 0.0 ); 
		net_export_rates.resize( 0, 0.0 ); 
	}
	return; 
}
	
void Secretion::sync_to_microenvironment( Microenvironment* pNew_Microenvironment )
{
	pMicroenvironment = pNew_Microenvironment;
	
	secretion_rates.resize( pMicroenvironment->number_of_densities() , 0.0 ); 
	uptake_rates.resize( pMicroenvironment->number_of_densities() , 0.0 ); 
	saturation_densities.resize( pMicroenvironment->number_of_densities() , 0.0 ); 
	net_export_rates.resize( pMicroenvironment->number_of_densities() , 0.0 ); 
	
	return; 
}

void Secretion::advance( Basic_Agent* pCell, Phenotype& phenotype , double dt )
{
	// if this phenotype is not associated with a cell, exit 
	if( pCell == NULL )
	{ return; }

	// if there is no microenvironment, attempt to sync. 
	if( pMicroenvironment == NULL )
	{
		// first, try the cell's microenvironment
		if( pCell->get_microenvironment() )
		{
			sync_to_microenvironment( pCell->get_microenvironment() ); 
		}
		// otherwise, try the default microenvironment
		else
		{
			sync_to_microenvironment( get_default_microenvironment() ); 
		}

		// if we've still failed, return. 
		if( pMicroenvironment == NULL ) 
		{
			return; 
		}
	}

	// make sure the associated cell has the correct rate vectors 
	if( pCell->secretion_rates != &secretion_rates )
	{
		delete pCell->secretion_rates; 
		delete pCell->uptake_rates; 
		delete pCell->saturation_densities; 
		delete pCell->net_export_rates; 
		
		pCell->secretion_rates = &secretion_rates; 
		pCell->uptake_rates = &uptake_rates; 
		pCell->saturation_densities = &saturation_densities; 
		pCell->net_export_rates = &net_export_rates; 
		
		pCell->set_total_volume( phenotype.volume.total ); 
		pCell->set_internal_uptake_constants( dt );
	}

	// now, call the BioFVM secretion/uptake function 
	
	pCell->simulate_secretion_and_uptake( pMicroenvironment , dt ); 
	
	return; 
}

void Secretion::set_all_secretion_to_zero( void )
{
	for( int i=0; i < secretion_rates.size(); i++ )
	{
		secretion_rates[i] = 0.0; 
		net_export_rates[i] = 0.0; 
	}
	return; 
}

void Secretion::set_all_uptake_to_zero( void )
{
	for( int i=0; i < uptake_rates.size(); i++ )
	{ uptake_rates[i] = 0.0; }
	return; 
}

void Secretion::scale_all_secretion_by_factor( double factor )
{
	for( int i=0; i < secretion_rates.size(); i++ )
	{
		secretion_rates[i] *= factor; 
		net_export_rates[i] *= factor; 
	}
	return; 
}

void Secretion::scale_all_uptake_by_factor( double factor )
{
	for( int i=0; i < uptake_rates.size(); i++ )
	{ uptake_rates[i] *= factor; }
	return; 
}

// ease of access
double& Secretion::secretion_rate( std::string name )
{
	int index = microenvironment.find_density_index(name); 
	return secretion_rates[index]; 
}

double& Secretion::uptake_rate( std::string name ) 
{
	int index = microenvironment.find_density_index(name); 
	return uptake_rates[index]; 
}

double& Secretion::saturation_density( std::string name ) 
{
	int index = microenvironment.find_density_index(name); 
	return saturation_densities[index]; 
}

double& Secretion::net_export_rate( std::string name )  
{
	int index = microenvironment.find_density_index(name); 
	return net_export_rates[index]; 
}

Molecular::Molecular()
{
	pMicroenvironment = get_default_microenvironment(); 
	sync_to_current_microenvironment(); 

	return; 
}

void Molecular::sync_to_current_microenvironment( void )
{
	if( pMicroenvironment )
	{
		sync_to_microenvironment( pMicroenvironment ); 
	}
	else
	{
		internalized_total_substrates.resize( 0 , 0.0 ); 
		fraction_released_at_death.resize( 0 , 0.0 ); 
		fraction_transferred_when_ingested.resize( 0, 0.0 ); 
	}
	return; 
}
	
void Molecular::sync_to_microenvironment( Microenvironment* pNew_Microenvironment )
{
	pMicroenvironment = pNew_Microenvironment;
	
	int number_of_densities = pMicroenvironment->number_of_densities() ; 

	internalized_total_substrates.resize( number_of_densities , 0.0 ); 
	fraction_released_at_death.resize( number_of_densities , 0.0 ); 
	fraction_transferred_when_ingested.resize( number_of_densities , 0.0 ); 
	
	return; 
}

void Molecular::sync_to_cell( Basic_Agent* pCell )
{
	delete pCell->internalized_substrates;
	pCell->internalized_substrates = &internalized_total_substrates;
	
	delete pCell->fraction_released_at_death;
	pCell->fraction_released_at_death = &fraction_released_at_death; 
	
	delete pCell->fraction_transferred_when_ingested; 
	pCell->fraction_transferred_when_ingested = &fraction_transferred_when_ingested; 

	return; 
}

// ease of access 
double&  Molecular::internalized_total_substrate( std::string name )
{
	int index = microenvironment.find_density_index(name); 
	return internalized_total_substrates[index]; 
}

/*
void Molecular::advance( Basic_Agent* pCell, Phenotype& phenotype , double dt )
{
	// if this phenotype is not associated with a cell, exit 
	if( pCell == NULL )
	{ return; }

	// if there is no microenvironment, attempt to sync. 
	if( pMicroenvironment == NULL )
	{
		// first, try the cell's microenvironment
		if( pCell->get_microenvironment() )
		{
			sync_to_microenvironment( pCell->get_microenvironment() ); 
		}
		// otherwise, try the default microenvironment
		else
		{
			sync_to_microenvironment( get_default_microenvironment() ); 
		}

		// if we've still failed, return. 
		if( pMicroenvironment == NULL ) 
		{
			return; 
		}
	}

	// make sure the associated cell has the correct rate vectors 
	if( pCell->internalized_substrates != &internalized_substrates )
	{
		// copy the data over 
		internalized_substrates = *(pCell->internalized_substrates);
		// remove the BioFVM copy 
		delete pCell->internalized_substrates; 
		// point BioFVM to this one  
		pCell->internalized_substrates = &internalized_substrates; 
	}

	// now, call the functions 
//	if( pCell->functions.internal_substrate_function )
//	{ pCell->functions.internal_substrate_function( pCell,phenotype,dt);  }
//	if( pCell->functions.molecular_model_function )
//	{ pCell->functions.molecular_model_function( pCell,phenotype,dt);  }


	return; 
}
*/


Cell_Functions::Cell_Functions()
{
	instantiate_cell = NULL;
	
	volume_update_function = NULL; 
	update_migration_bias = NULL; 
	
	update_phenotype = NULL; 
	custom_cell_rule = NULL; 
	
	pre_update_intracellular = NULL;
	post_update_intracellular = NULL;

	update_velocity = NULL; 
	add_cell_basement_membrane_interactions = NULL; 
	calculate_distance_to_membrane = NULL; 
	
	set_orientation = NULL; 
	
	contact_function = NULL; 

/*	
	internal_substrate_function = NULL; 
	molecular_model_function = NULL; 
*/
	return; 
}

void Phenotype::sync_to_functions( Cell_Functions& functions )
{
	cycle.sync_to_cycle_model( functions.cycle_model );  
	
	return; 
}

Phenotype::Phenotype()
{
	flagged_for_division = false;
	flagged_for_removal = false; 
	
	// sync the molecular stuff here automatically? 
	intracellular = NULL;
	
	return; 
}

Phenotype::Phenotype(const Phenotype &p) {
	intracellular = NULL;
	*this = p;
}

Phenotype::~Phenotype() 
{
	if (intracellular != NULL)
		delete intracellular;
}

Phenotype& Phenotype::operator=(const Phenotype &p ) { 
		
	flagged_for_division = p.flagged_for_division;
	flagged_for_removal = p.flagged_for_removal;
	
	cycle = p.cycle;
	death = p.death;
	volume = p.volume;
	geometry = p.geometry;
	mechanics = p.mechanics;
	motility = p.motility;
	secretion = p.secretion;
	
	molecular = p.molecular;

	cell_integrity = p.cell_integrity; 
	
	delete intracellular;
	
	if (p.intracellular != NULL)
	{ intracellular = p.intracellular->clone(); }
	else
	{ intracellular = NULL; }
	
	cell_interactions = p.cell_interactions; 
	cell_transformations = p.cell_transformations; 
	
	return *this;
}
/*
class Bools
{
	public:
		std::vector<bool> values; 
		std::unordered_map<std::string,int> name_map; 
		std::string& name( int i ); 
		std::vector<std::string> units; 
		void resize( int n ); 
		int add( std::string name , std::string units , bool value ); 
		
		bool& operator[]( int i ); 
		bool& operator[]( std::string name ); 
		
		Bools(); 
}
*/

Bools::Bools()
{
	values.resize( 0 , true ); 
	name_map.clear(); 
	return; 
}

int Bools::size( void )
{ return values.size(); } 


void Phenotype::sync_to_microenvironment( Microenvironment* pMicroenvironment )
{
	secretion.sync_to_microenvironment( pMicroenvironment ); 
	molecular.sync_to_microenvironment( pMicroenvironment ); 

	return; 
}

Cell_Interactions::Cell_Interactions()
{
	// dead_phagocytosis_rate = 0.0; 

	apoptotic_phagocytosis_rate = 0.0; 
	necrotic_phagocytosis_rate = 0.0; 
	other_dead_phagocytosis_rate = 0.0; 

	live_phagocytosis_rates = {0.0}; 

	attack_damage_rate = 1.0; 
	attack_rates = {0.0}; 
	immunogenicities = {1}; 

	pAttackTarget = NULL; 
	total_damage_delivered = 0.0; 

	attack_duration = 30.0; // a typical attack duration for a T cell using perforin/granzyme is ~30 minutes

	fusion_rates = {0.0}; 
	
	return; 
}

void Cell_Interactions::sync_to_cell_definitions()
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int number_of_cell_defs = cell_definition_indices_by_name.size(); 
	
	if( live_phagocytosis_rates.size() != number_of_cell_defs )
	{
		live_phagocytosis_rates.resize( number_of_cell_defs, 0.0); 
		attack_rates.resize( number_of_cell_defs, 0.0); 
		fusion_rates.resize( number_of_cell_defs, 0.0); 
		immunogenicities.resize( number_of_cell_defs , 1.0 ); 
	}
	
	return; 
}

// ease of access 
double& Cell_Interactions::live_phagocytosis_rate( std::string type_name )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	// std::cout << type_name << " " << n << std::endl; 
	return live_phagocytosis_rates[n]; 
}

double& Cell_Interactions::attack_rate( std::string type_name ) 
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	return attack_rates[n]; 
}

double& Cell_Interactions::fusion_rate( std::string type_name )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	return fusion_rates[n]; 
}

double& Cell_Interactions::immunogenicity( std::string type_name )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	return immunogenicities[n]; 
}

Cell_Transformations::Cell_Transformations()
{
	transformation_rates = {0.0}; 
	
	return; 
}

void Cell_Transformations::sync_to_cell_definitions()
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 	
	int number_of_cell_defs = cell_definition_indices_by_name.size();  
	
	if( transformation_rates.size() != number_of_cell_defs )
	{ transformation_rates.resize( number_of_cell_defs, 0.0); }
	
	return; 
}

// ease of access 
double& Cell_Transformations::transformation_rate( std::string type_name )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	int n = cell_definition_indices_by_name[type_name]; 
	return transformation_rates[n]; 
}

// beta functionality in 1.10.3 
Cell_Integrity::Cell_Integrity()
{
 	damage = 0;  
	damage_rate = 0.0; 
	damage_repair_rate = 0.0; 

/*
	lipid_damage = 0.0; 
	lipid_damage_rate = 0.0; 
	lipid_damage_repair_rate = 0.0; 

	// DNA damage 
	DNA_damage = 0.0; 
	DNA_damage_rate = 0.0; 
	DNA_damage_repair_rate = 0.0; 
*/

	return; 
}

void Cell_Integrity::advance_damage( double dt )
{
	double temp1;
	double temp2; 
	static double tol = 1e-8; 

	// general damage 
	if( damage_rate > tol || damage_repair_rate > tol )
	{
		temp1 = dt; 
		temp2 = dt; 
		temp1 *= damage_rate;  
		temp2 *= damage_repair_rate; 
		temp2 += 1; 

		damage += temp1; 
		damage /= temp2; 
	}
/*
	// lipid damage 
	if( lipid_damage_rate > tol || lipid_damage_repair_rate > tol )
	{
		temp1 = dt;
		temp2 = dt;
		temp1 *= lipid_damage_rate;  
		temp2 *= lipid_damage_repair_rate; 
		temp2 += 1; 
	
		lipid_damage += temp1; 
		lipid_damage /= temp2; 
	}

	// DNA damage 
	if( DNA_damage_rate > tol || DNA_damage_repair_rate > tol )
	{
		temp1 = dt;
		temp2 = dt;
		temp1 *= DNA_damage_rate;  
		temp2 *= DNA_damage_repair_rate; 
		temp2 += 1; 

		DNA_damage += temp1; 
		DNA_damage /= temp2; 
	}
*/	

//	std::cout << "damage: " << damage << std::endl; 

	return; 
}

// streaming operator for phase
std::ostream& operator<<(std::ostream& os, const Phase& phase) {
        os << "index: " << phase.index << std::endl;
		os << "code: " << phase.code << std::endl;
		os << "name: " << phase.name << std::endl;
		os << "division_at_phase_exit: " << (phase.division_at_phase_exit ? "true" : "false") << std::endl;
		os << "removal_at_phase_exit: " << (phase.removal_at_phase_exit ? "true" : "false") << std::endl;
		os << "entry_function: " << phase.entry_function << std::endl;
        
        return os;
}

std::istream& operator>>(std::istream& is, Phase& phase){

	std::string dummy;

    //index
	std::getline(is, dummy);
	phase.index = read_number_in_line_int(dummy);

	//code
	std::getline(is, dummy);
	phase.code = read_number_in_line_int(dummy);

	// name
	std::getline(is, dummy);
	std::istringstream stream(dummy);
	std::string key, name;
	stream >> key;
	std::getline(stream, name);  
	name.erase(0, name.find_first_not_of(" "));
	phase.name = name;

	// division_at_phase_exit
    std::getline(is, dummy);
	phase.division_at_phase_exit = read_number_in_line_bool(dummy);

	// removal_at_phase_exit
    std::getline(is, dummy);
	phase.removal_at_phase_exit = read_number_in_line_bool(dummy);

	// entry_function
    std::getline(is, dummy);

	//empty_row
	std::getline(is, dummy);

	return is;
}

// streaming operator for phase link
std::ostream& operator<<(std::ostream& os, const Phase_Link& phase_link) {
        os << "start_phase_index: " << phase_link.start_phase_index << std::endl;
		os << "end_phase_index: " << phase_link.end_phase_index << std::endl;
		os << "fixed_duration: " << (phase_link.fixed_duration ? "true" : "false") << std::endl;
		os << "arrest_function: " << (phase_link.arrest_function ? "true" : "false") << std::endl;
		//not boolean need fixing
    	os << "exit_function: " << phase_link.exit_function << std::endl;

        return os;
}

std::istream& operator>>(std::istream& is, Phase_Link& phase_link){

	std::string dummy;


    //start_phase_index
	std::getline(is, dummy);
	phase_link.start_phase_index = read_number_in_line_int(dummy);

	//end_phase_index
	std::getline(is, dummy);
	phase_link.end_phase_index = read_number_in_line_int(dummy);

	// fixed_duration
    std::getline(is, dummy);
	phase_link.fixed_duration = read_number_in_line_bool(dummy);

	// arrest_function
    std::getline(is, dummy);

	// exit_function
    std::getline(is, dummy);

	//empty_row
	std::getline(is, dummy);

	return is;
}

// Streaming operator for cycle model
void Cycle_Model::save_data(std::ostream& os )
{

	//name
	os << "name: " << name << std::endl;

	//code
	os << "code: " << code << std::endl;

	//phases
	os << "phases:" << std::endl;
    for (size_t i = 0; i < phases.size(); ++i) {
        os << phases[i] << std::endl;;
    }
    os << std::endl;

	//phase_links
	os << "phase_links:" << std::endl;
    for (const auto& phase_links_v : phase_links) {
		for( const auto& phase_links_current : phase_links_v){
        os << phase_links_current << std::endl;
    }}
    os << std::endl;

	//default_phase_index
	os << "default_phase_index: " << default_phase_index << std::endl;

}

void Cycle_Model::reset_data(std::istream& is)
{

	std::string dummy;

	// Skip "Cycle" and read subsequent data
    std::getline(is, dummy);


	// name
	std::getline(is, dummy);
	std::istringstream stream(dummy);
	std::string key, name_new;
	stream >> key >> name_new;
	name_new = name;

	//code
	std::getline(is, dummy);
	code = read_number_in_line_int(dummy);


	//phases
	std::getline(is, dummy);

	for (size_t i = 0; i < phases.size(); ++i) {
        is >> phases[i];
    }std::getline(is, dummy);

	//phase_links
	std::getline(is, dummy);

	for (auto& phase_links_v : phase_links) {
		for( auto& phase_links_current : phase_links_v){
        is >> phase_links_current;
    }}std::getline(is, dummy);

	//default_phase_index
	std::getline(is, dummy);
	default_phase_index = read_number_in_line_int(dummy);


}
// Streaming operator for cycle data
std::ostream& operator<<(std::ostream& os, const Cycle_Data& cycleData) {
        os << "Time_Units: " << cycleData.time_units << std::endl;
        os << "Current_Phase_Index: " << cycleData.current_phase_index << std::endl;
        os << "Elapsed_Time_in_Phase: " << cycleData.elapsed_time_in_phase << std::endl;

        os << "Transition_Rates:" << std::endl;
		int num_phases = cycleData.transition_rates.size();
		os << "num_Phases: " << num_phases << std::endl;
        for (std::size_t i = 0; i < cycleData.transition_rates.size(); ++i) {
            for (std::size_t j = 0; j < cycleData.transition_rates[i].size(); ++j) {
                os << "i" << i << "j" << j << ": " << cycleData.transition_rates[i][j] << std::endl;
            }
        }
		os << "End of cycle data" << std::endl;
        return os;
}

std::istream& operator>>(std::istream& is, Cycle_Data& cycleData) {
    std::string dummy;

    // Read Time Units
	std::getline(is, dummy);
	std::istringstream stream(dummy);
	std::string key, time_units;
	stream >> key >> time_units;
	cycleData.time_units = time_units;
		
    // current_phase_index
    std::getline(is, dummy);
	cycleData.current_phase_index = read_number_in_line_int(dummy);

    // elapsed_time_in_phase
	std::getline(is, dummy);
	cycleData.elapsed_time_in_phase = read_number_in_line(dummy);

	// Read "Transition Rates:"
    std::getline(is, dummy);

	//Read num phases
	std::getline(is, dummy);
	double num_phases;
	num_phases = read_number_in_line(dummy);

	// no need to reupdate since now the dimension should be good.

	// Read the transition rates until "End of cycle data" is encountered
	while (std::getline(is, dummy)) {
		// Check if the line contains "End of cycle data"
		if (dummy.find("End of cycle data") != std::string::npos) {
			break;
		}
		//read the phase
		int i,j;

		size_t pos_i = dummy.find('i');
    	size_t pos_j = dummy.find('j');

		std::string value_i = dummy.substr(pos_i + 1, 1);
    	std::string value_j = dummy.substr(pos_j + 1, 1);

		i = std::stoi(value_i);
    	j = std::stoi(value_j);

		std::istringstream stream_case1(dummy);
		std::string key_case1;
		stream_case1 >> key_case1;
		double p1;
		while (stream_case1 >> p1) {
			cycleData.transition_rates[i][j] = p1;
	}
	}
	
	// discard empty rows;

	std::getline(is, dummy);
	std::getline(is, dummy);

    return is;
}

// Stream operator for Cycle
std::ostream& operator<<(std::ostream& os, const Cycle& cycle) {
	//for now no pointers in the saved model
	cycle.pCycle_Model->save_data(os);
    os << cycle.data << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Cycle& cycle) {
    // read string "Cycle Data:"
	cycle.pCycle_Model->reset_data(is);
    is >> cycle.data;

    return is;
}

// stream operator for death parameter class
std::ostream& operator<<(std::ostream& os, const Death_Parameters& DParameters) {
    os << "Death_Parameters:" << std::endl;
    os << "Time_Units: " << DParameters.time_units << std::endl;
    os << "Unlysed_Fluid_Change_Rate: " << DParameters.unlysed_fluid_change_rate << std::endl;
    os << "Lysed_Fluid_Change_Rate: " << DParameters.lysed_fluid_change_rate << std::endl;
    os << "Cytoplasmic_Biomass_Change_Rate: " << DParameters.cytoplasmic_biomass_change_rate << std::endl;
    os << "Nuclear_Biomass_Change_Rate: " << DParameters.nuclear_biomass_change_rate << std::endl;
    os << "Calcification_Rate: " << DParameters.calcification_rate << std::endl;
    os << "Relative_Rupture_Volume: " << DParameters.relative_rupture_volume << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Death_Parameters& DParameters) {
	std::string dummy;

	// skip the header death parameters and parameters:
	std::getline(is, dummy);
	std::getline(is, dummy);

    // time_units
    std::getline(is, dummy);
	std::istringstream stream(dummy);
	std::string key, time_units;
	stream >> key >> time_units;
	DParameters.time_units = time_units;

	//unlysed_fluid_change_rate
    std::getline(is, dummy);
	DParameters.unlysed_fluid_change_rate = read_number_in_line(dummy);

	// lysed_fluid_change_rate
    std::getline(is, dummy);
	DParameters.lysed_fluid_change_rate = read_number_in_line(dummy);

	// cytoplasmic_biomass_change_rate
    std::getline(is, dummy);
	DParameters.cytoplasmic_biomass_change_rate = read_number_in_line(dummy);

	// nuclear_biomass_change_rate
    std::getline(is, dummy);
	DParameters.nuclear_biomass_change_rate = read_number_in_line(dummy);

	//calcification_rate
    std::getline(is, dummy);
	DParameters.calcification_rate = read_number_in_line(dummy);

	// relative_rupture_volume
    std::getline(is, dummy);
	DParameters.relative_rupture_volume = read_number_in_line(dummy);
	

	//skip empty row
	std::getline(is, dummy);

    return is;
}


//Stream operator for death class
std::ostream& operator<<(std::ostream& os, const Death& death) {
    os << "Death_Information:" << std::endl;
    os << "Current_Death_Model_Index: " << death.current_death_model_index << std::endl;
    os << "Dead: " << (death.dead ? "true" : "false") << std::endl;

    os << "Death_Models: " << death.models.size() << std::endl;
    for (size_t i = 0; i < death.models.size(); ++i) {
        os << "Model " << i << ":" << std::endl;
        os << "Rate: " << death.rates[i] << std::endl;
        os << "Parameters:\n" << death.parameters[i] << std::endl;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Death& death) {
    std::string dummy;
    // Read the header "Death:" and "Death Information:"
    std::getline(is, dummy);
	std::getline(is, dummy);

    // current_death_model_index
    std::getline(is, dummy);
	death.current_death_model_index = read_number_in_line_int(dummy);

    // Dead status
    std::getline(is, dummy);
	death.dead = read_number_in_line_bool(dummy);

	// num_Death_Models
	std::getline(is, dummy);
	double num_Death_Models;
	num_Death_Models = read_number_in_line(dummy);

	for (int i = 0; i < num_Death_Models; ++i) {
		// skip the header Model: i
		std::getline(is, dummy);

		// rate
		std::getline(is, dummy);
		death.rates[i] = read_number_in_line(dummy);

		// Parameters
		is >> death.parameters[i];
	}

	// skip empty rows
	std::getline(is, dummy);
    return is;
}

// Stream operator for volume class
std::ostream& operator<<(std::ostream& os, const Volume& volume) {
    os << "Total: " << volume.total << std::endl;
    os << "Solid: " << volume.solid << std::endl;
    os << "Fluid: " << volume.fluid << std::endl;
    os << "Fluid_Fraction: " << volume.fluid_fraction << std::endl;
    os << "Nuclear: " << volume.nuclear << std::endl;
    os << "Nuclear_Fluid: " << volume.nuclear_fluid << std::endl;
    os << "Nuclear_Solid: " << volume.nuclear_solid << std::endl;
    os << "Cytoplasmic: " << volume.cytoplasmic << std::endl;
    os << "Cytoplasmic_Fluid: " << volume.cytoplasmic_fluid << std::endl;
    os << "Cytoplasmic_Solid: " << volume.cytoplasmic_solid << std::endl;
    os << "Calcified_Fraction: " << volume.calcified_fraction << std::endl;
    os << "Cytoplasmic_to_Nuclear_Ratio: " << volume.cytoplasmic_to_nuclear_ratio << std::endl;
    os << "Rupture_Volume: " << volume.rupture_volume << std::endl;
    os << "Cytoplasmic_Biomass_Change_Rate: " << volume.cytoplasmic_biomass_change_rate << std::endl;
    os << "Nuclear_Biomass_Change_Rate: " << volume.nuclear_biomass_change_rate << std::endl;
    os << "Fluid_Change_Rate: " << volume.fluid_change_rate << std::endl;
    os << "Calcification_Rate: " << volume.calcification_rate << std::endl;
    os << "Target_Solid_Cytoplasmic: " << volume.target_solid_cytoplasmic << std::endl;
    os << "Target_Solid_Nuclear: " << volume.target_solid_nuclear << std::endl;
    os << "Target_Fluid_Fraction: " << volume.target_fluid_fraction << std::endl;
    os << "Target_Cytoplasmic_to_Nuclear_Ratio: " << volume.target_cytoplasmic_to_nuclear_ratio << std::endl;
    os << "Relative_Rupture_Volume: " << volume.relative_rupture_volume << std::endl;

    return os;
}
std::istream& operator>>(std::istream& is, Volume& volume) {
	std::string dummy;

	//skip first 2 lines line
	std::getline(is, dummy);

	//total
	std::getline(is, dummy);
	volume.total = read_number_in_line(dummy);
	
	//solid
	std::getline(is, dummy);
	volume.solid = read_number_in_line(dummy); 

	//fluid
    std::getline(is, dummy);
	volume.fluid = read_number_in_line(dummy);

	//fluid_fraction
    std::getline(is, dummy);
	volume.fluid_fraction = read_number_in_line(dummy);

	//nuclear
    std::getline(is, dummy);
	volume.nuclear = read_number_in_line(dummy);

	//nuclear_fluid
    std::getline(is, dummy);
	volume.nuclear_fluid = read_number_in_line(dummy);

	//nuclear_solid
    std::getline(is, dummy);
	volume.nuclear_solid = read_number_in_line(dummy);

	//cytoplasmic
    std::getline(is, dummy);
	volume.cytoplasmic = read_number_in_line(dummy);

	//cytoplasmic_fluid
    std::getline(is, dummy);
	volume.cytoplasmic_fluid = read_number_in_line(dummy);

	//cytoplasmic_solid
    std::getline(is, dummy);
	volume.cytoplasmic_solid = read_number_in_line(dummy);

	//calcified_fraction
    std::getline(is, dummy);
	volume.calcified_fraction = read_number_in_line(dummy);

    //cytoplasmic_to_nuclear_ratio
    std::getline(is, dummy);
	volume.cytoplasmic_to_nuclear_ratio = read_number_in_line(dummy);

	//rupture_volume
    std::getline(is, dummy);
	volume.rupture_volume = read_number_in_line(dummy);

	//cytoplasmic_biomass_change_rate
    std::getline(is, dummy);
	volume.cytoplasmic_biomass_change_rate = read_number_in_line(dummy);

	//nuclear_biomass_change_rate
    std::getline(is, dummy);
	volume.nuclear_biomass_change_rate = read_number_in_line(dummy);

	//fluid_change_rate
    std::getline(is, dummy);
	volume.fluid_change_rate = read_number_in_line(dummy);

	//calcification_rate
    std::getline(is, dummy);
	volume.calcification_rate = read_number_in_line(dummy);

	//target_solid_cytoplasmic
    std::getline(is, dummy);
	volume.target_solid_cytoplasmic = read_number_in_line(dummy);

	//target_solid_nuclear
    std::getline(is, dummy);
	volume.target_solid_nuclear = read_number_in_line(dummy);

	//target_fluid_fraction
    std::getline(is, dummy);
	volume.target_fluid_fraction = read_number_in_line(dummy);

	//target_cytoplasmic_to_nuclear_ratio
    std::getline(is, dummy);
	volume.target_cytoplasmic_to_nuclear_ratio = read_number_in_line(dummy);

	//relative_rupture_volume
    std::getline(is, dummy);
	volume.relative_rupture_volume = read_number_in_line(dummy);

	//skip empty lines
	std::getline(is, dummy);
    return is;
}

// Stream operator for geometry class
std::ostream& operator<<(std::ostream& os, const Geometry& geometry) {
    os << "Radius: " << geometry.radius << std::endl;
    os << "Nuclear_radius: " << geometry.nuclear_radius << std::endl;
    os << "Surface_area: " << geometry.surface_area << std::endl;
    os << "Polarity: " << geometry.polarity << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Geometry& geometry) {
	std::string dummy;

	//skip first line
	std::getline(is, dummy);

	//radius
	std::getline(is, dummy);
	geometry.radius = read_number_in_line(dummy);

	//nuclear_radius
    std::getline(is, dummy);
	geometry.nuclear_radius = read_number_in_line(dummy);

	//surface_area
    std::getline(is, dummy);
	geometry.surface_area = read_number_in_line(dummy);

	//polarity
    std::getline(is, dummy);
	geometry.polarity = read_number_in_line(dummy);


	//skip last rows
	std::getline(is, dummy);
    return is;
}

// Stream operator for Mechanics class
std::ostream& operator<<(std::ostream& os, const Mechanics& mechanics) {
    os << "Cell_cell_adhesion_strength: " << mechanics.cell_cell_adhesion_strength << std::endl;
    os << "Cell_BM_adhesion_strength: " << mechanics.cell_BM_adhesion_strength << std::endl;
    os << "Cell_cell_repulsion_strength: " << mechanics.cell_cell_repulsion_strength << std::endl;
    os << "Cell_BM_repulsion_strength: " << mechanics.cell_BM_repulsion_strength << std::endl;
	os << "cell_adhesion_affinities: ";
    for (const auto& affinity : mechanics.cell_adhesion_affinities) {
        os << affinity << " ";
    }
    os << std::endl;
	os << "Relative_maximum_adhesion_distance " << mechanics.relative_maximum_adhesion_distance << std::endl;
	os << "maximum_number_of_attachments " << mechanics.maximum_number_of_attachments << std::endl;
	os << "attachment_elastic_constant " << mechanics.attachment_elastic_constant << std::endl;
	os << "attachment_rate " << mechanics.attachment_rate << std::endl;
	os << "detachment_rate " << mechanics.detachment_rate << std::endl;
	os << "relative_maximum_attachment_distance " << mechanics.relative_maximum_attachment_distance << std::endl;
	os << "relative_detachment_distance " << mechanics.relative_detachment_distance << std::endl;
	os << "maximum_attachment_rate " << mechanics.maximum_attachment_rate << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Mechanics& mechanics) {
	std::string dummy;

	//skip first line
	std::getline(is, dummy);

	//cell_cell_adhesion_strength
	std::getline(is, dummy);
	mechanics.cell_cell_adhesion_strength = read_number_in_line(dummy);

	//cell_BM_adhesion_strength
    std::getline(is, dummy);
	mechanics.cell_BM_adhesion_strength = read_number_in_line(dummy);

	//cell_cell_repulsion_strength
    std::getline(is, dummy);
	mechanics.cell_cell_repulsion_strength = read_number_in_line(dummy);

	//cell_BM_repulsion_strength
    std::getline(is, dummy);
	mechanics.cell_BM_repulsion_strength = read_number_in_line(dummy);

	// cell_adhesion_affinities
	std::getline(is, dummy);
	std::istringstream stream_cell_adhesion_affinities(dummy);
	std::string key_cell_adhesion_affinities;
	stream_cell_adhesion_affinities >> key_cell_adhesion_affinities;
	double number;
	std::vector<double> cell_adhesion_affinities;
	while(stream_cell_adhesion_affinities >> number) {
		cell_adhesion_affinities.push_back(number);
	}
	mechanics.cell_adhesion_affinities.clear();
	mechanics.cell_adhesion_affinities = cell_adhesion_affinities;

	//relative_maximum_adhesion_distance
	std::getline(is, dummy);
	mechanics.relative_maximum_adhesion_distance = read_number_in_line(dummy);

	//maximum_number_of_attachments
	std::getline(is, dummy);
	mechanics.maximum_number_of_attachments = read_number_in_line_int(dummy);

	//attachment_elastic_constant
	std::getline(is, dummy);
	mechanics.attachment_elastic_constant = read_number_in_line(dummy);

	//attachment_rate
	std::getline(is, dummy);
	mechanics.attachment_rate = read_number_in_line(dummy);

	//detachment_rate
	std::getline(is, dummy);
	mechanics.detachment_rate = read_number_in_line(dummy);

	//relative_maximum_attachment_distance
	std::getline(is, dummy);
	mechanics.relative_maximum_attachment_distance = read_number_in_line(dummy);
	//relative_detachment_distance
	std::getline(is, dummy);
	mechanics.relative_detachment_distance = read_number_in_line(dummy);

	//maximum_attachment_rate
	std::getline(is, dummy);
	mechanics.maximum_attachment_rate = read_number_in_line(dummy);

	//skip empty rows
	std::getline(is, dummy);
    return is;
}

// stream operators for Motility class
std::ostream& operator<<(std::ostream& os, const Motility& motility) {
    os << "Motility_Information:" << std::endl;
    os << "Is_Motile: " << (motility.is_motile ? "true" : "false") << std::endl;
    os << "Persistence_Time: " << motility.persistence_time << std::endl;
    os << "Migration_Speed: " << motility.migration_speed << std::endl;

    os << "Migration_Bias_Direction: ";
    for (const auto& direction : motility.migration_bias_direction) {
        os << direction << " ";
    }
    os << std::endl;

    os << "Migration_Bias: " << motility.migration_bias << std::endl;
    os << "Restrict_to_2D: " << (motility.restrict_to_2D ? "true" : "false") << std::endl;

    os << "Motility_Vector: ";
    for (const auto& component : motility.motility_vector) {
        os << component << " ";
    }
    os << std::endl;

    os << "Chemotaxis_Index: " << motility.chemotaxis_index << std::endl;
    os << "Chemotaxis_Direction: " << motility.chemotaxis_direction << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Motility& motilityparams) {
	std::string dummy;

	// read headers
	std::getline(is, dummy);
	std::getline(is, dummy);

	//is motile
	std::getline(is, dummy);
	motilityparams.is_motile = read_number_in_line_bool(dummy);

	//persistence_time
	std::getline(is, dummy);
	motilityparams.persistence_time = read_number_in_line(dummy);

	//migration_speed
	std::getline(is, dummy);
	motilityparams.migration_speed = read_number_in_line(dummy);

	// migration_bias_direction
	std::getline(is, dummy);
	std::istringstream stream_migration_bias_direction(dummy);
	std::string key_migration_bias_direction;
	stream_migration_bias_direction >> key_migration_bias_direction;
	double number;
	std::vector<double> migration_bias_direction;
	while(stream_migration_bias_direction >> number) {
		migration_bias_direction.push_back(number);
	}
	motilityparams.migration_bias_direction.clear();
	motilityparams.migration_bias_direction = migration_bias_direction;

	//migration_bias
	std::getline(is, dummy);
	motilityparams.migration_bias = read_number_in_line(dummy);

	//restrict_to_2D
	std::getline(is, dummy);
	motilityparams.restrict_to_2D = read_number_in_line_bool(dummy);

	// motility_vector
	std::getline(is, dummy);
	std::istringstream stream_motility_vector(dummy);
	std::string key_motility_vector;
	stream_motility_vector >> key_motility_vector;
	double number_2;
	std::vector<double> motility_vector;
	while(stream_motility_vector >> number_2) {
		motility_vector.push_back(number_2);
	}
	motilityparams.motility_vector.clear();
	motilityparams.motility_vector = motility_vector;

	//chemotaxis_index
	std::getline(is, dummy);
	motilityparams.chemotaxis_index = read_number_in_line_int(dummy);

	//chemotaxis_direction
	std::getline(is, dummy);
	motilityparams.chemotaxis_direction = read_number_in_line_int(dummy);

	//skip empty rows
	std::getline(is, dummy);

    return is;
}

// Streaming operators for Secretion class
std::ostream& operator<<(std::ostream& os, const Secretion& secretionparams) {
    os << "Secretion_Information:" << std::endl;
    os << "Number_of_Secretions: " << secretionparams.secretion_rates.size() << std::endl;

    for (size_t i = 0; i < secretionparams.secretion_rates.size(); ++i) {
        os << "Secretion " << i << ":" << std::endl;
        os << "Secretion_Rate: " << secretionparams.secretion_rates[i] << std::endl;
        os << "Uptake_Rate: " << secretionparams.uptake_rates[i] << std::endl;
        os << "Saturation_Density: " << secretionparams.saturation_densities[i] << std::endl;
        os << "Net_Export_Rate: " << secretionparams.net_export_rates[i] << "\n" << std::endl;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Secretion& secretionparams) {
	std::string dummy;

	// read headers
	std::getline(is, dummy);
	std::getline(is, dummy);

	//number_of_secretions
	std::getline(is, dummy);
	double number_of_secretions;
	number_of_secretions = read_number_in_line(dummy);

	for (size_t i = 0; i < number_of_secretions; ++i) {

		//skip header
		std::getline(is, dummy);

		//secretion_rate
		std::getline(is, dummy);
		secretionparams.secretion_rates[i] = read_number_in_line(dummy);

		//uptake_rate
		std::getline(is, dummy);
		secretionparams.uptake_rates[i] = read_number_in_line(dummy);

		//saturation_density
		std::getline(is, dummy);
		secretionparams.saturation_densities[i] = read_number_in_line(dummy);

		//net_export_rate
		std::getline(is, dummy);
		secretionparams.net_export_rates[i] = read_number_in_line(dummy);

		//skip empty rows
		std::getline(is, dummy);

	}

	//skip last rows
	std::getline(is, dummy);

    return is;
}

// Streaming operators for class Molecular
std::ostream& operator<<(std::ostream& os, const Molecular& molecular) {
    os << "Molecular_Information:" << std::endl;
	os << "number_of_substrates: " << molecular.internalized_total_substrates.size() << std::endl;

	for (size_t i = 0; i < molecular.internalized_total_substrates.size(); ++i){

		os << "Substrate: " << i << std::endl;
		os << "Internalized_Total_Substrates: " << molecular.internalized_total_substrates[i] << std::endl;
		os << "Fraction_Released_at_Death: " << molecular.fraction_released_at_death[i] << std::endl;
		os << "Fraction_Transferred_When_Ingested: " << molecular.fraction_transferred_when_ingested[i] << "\n" << std::endl;

	}

    return os;
}

std::istream& operator>>(std::istream& is, Molecular& molecular) {
	std::string dummy;

	// read headers
	std::getline(is, dummy);
	std::getline(is, dummy);

	//number_of_secretions
	std::getline(is, dummy);
	double number_of_substrates;
	number_of_substrates = read_number_in_line(dummy);

	for (size_t i = 0; i < number_of_substrates; ++i) {

		//skip header
		std::getline(is, dummy);

		//Internalized_Total_Substrates
		std::getline(is, dummy);
		molecular.internalized_total_substrates[i] = read_number_in_line(dummy);

		//Fraction_Released_at_Death
		std::getline(is, dummy);
		molecular.fraction_released_at_death[i] = read_number_in_line(dummy);

		//Fraction_Transferred_When_Ingested
		std::getline(is, dummy);
		molecular.fraction_released_at_death[i] = read_number_in_line(dummy);

		//skip empty rows
		std::getline(is, dummy);

	}

	//skip last rows
	std::getline(is, dummy);
	std::getline(is, dummy);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Cell_Interactions& cell_interactions) {
    os << "Cell_Interactions_Information:" << std::endl;
    os << "Dead_Phagocytosis_Rate: " << cell_interactions.other_dead_phagocytosis_rate << std::endl;
    os << "Live_Phagocytosis_Rates: ";
    for (size_t i = 0; i < cell_interactions.live_phagocytosis_rates.size(); ++i) {
        os << cell_interactions.live_phagocytosis_rates[i] << " ";
    }
    os << std::endl;
    os << "Attack_Rates: ";
    for (size_t i = 0; i < cell_interactions.attack_rates.size(); ++i) {
        os << cell_interactions.attack_rates[i] << " ";
    }
    os << std::endl;
    os << "Immunogenicities: ";
    for (size_t i = 0; i < cell_interactions.immunogenicities.size(); ++i) {
        os << cell_interactions.immunogenicities[i] << " ";
    }
    os << std::endl;

    os << "Damage_Rate: " << cell_interactions.attack_damage_rate << std::endl;
    os << "Fusion_Rates: ";
    for (size_t i = 0; i < cell_interactions.fusion_rates.size(); ++i) {
        os << cell_interactions.fusion_rates[i] << " ";
    }
    os << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Cell_Interactions& cell_interactions){
	std::string dummy;
	
	// read headers
	std::getline(is, dummy);

	//other_dead_phagocytosis_rate
	std::getline(is, dummy);
	cell_interactions.other_dead_phagocytosis_rate = read_number_in_line(dummy);

	// Live_Phagocytosis_Rates
	std::getline(is, dummy);
	std::istringstream stream_Live_Phagocytosis_Rates(dummy);
	std::string key_Live_Phagocytosis_Rates;
	stream_Live_Phagocytosis_Rates >> key_Live_Phagocytosis_Rates;
	double number1;
	std::vector<double> Live_Phagocytosis_Rates;
	while(stream_Live_Phagocytosis_Rates >> number1) {
		Live_Phagocytosis_Rates.push_back(number1);
	}
	cell_interactions.live_phagocytosis_rates.clear();
	cell_interactions.live_phagocytosis_rates = Live_Phagocytosis_Rates;

	// Attack_Rates
	std::getline(is, dummy);
	std::istringstream stream_Attack_Rates(dummy);
	std::string key_Attack_Rates;
	stream_Attack_Rates >> key_Attack_Rates;
	double number2;
	std::vector<double> Attack_Rates;
	while(stream_Attack_Rates >> number2) {
		Attack_Rates.push_back(number2);
	}
	cell_interactions.attack_rates.clear();
	cell_interactions.attack_rates = Attack_Rates;

	// Immunogenicities
	std::getline(is, dummy);
	std::istringstream stream_Immunogenicities(dummy);
	std::string key_Immunogenicities;
	stream_Immunogenicities >> key_Immunogenicities;
	double number3;
	std::vector<double> Immunogenicities;
	while(stream_Immunogenicities >> number3) {
		Immunogenicities.push_back(number3);
	}
	cell_interactions.immunogenicities.clear();
	cell_interactions.immunogenicities = Immunogenicities;

	//Damage_Rate
	std::getline(is, dummy);
	cell_interactions.attack_damage_rate = read_number_in_line(dummy);

	// Fusion_Rates
	std::getline(is, dummy);
	std::istringstream stream_Fusion_Rates(dummy);
	std::string key_Fusion_Rates;
	stream_Fusion_Rates >> key_Fusion_Rates;
	double number4;
	std::vector<double> Fusion_Rates;
	while(stream_Fusion_Rates >> number4) {
		Fusion_Rates.push_back(number4);
	}
	cell_interactions.fusion_rates.clear();
	cell_interactions.fusion_rates = Fusion_Rates;

	std::getline(is, dummy);


	return is;
}

std::ostream& operator<<(std::ostream& os, const Cell_Transformations& cell_transformations) {
    os << "Cell_Transformations_Information:" << std::endl;
    os << "Transformation_Rates: ";
    for (size_t i = 0; i < cell_transformations.transformation_rates.size(); ++i) {
        os << cell_transformations.transformation_rates[i] << " ";
    }
    os << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, Cell_Transformations& cell_transformations) {
	std::string dummy;
	
	// read headers
	std::getline(is, dummy);
	std::getline(is, dummy);

	// Transformation_Rates
	std::getline(is, dummy);
	std::istringstream stream_Transformation_Rates(dummy);
	std::string key_Transformation_Rates;
	stream_Transformation_Rates >> key_Transformation_Rates;
	double number;
	std::vector<double> Transformation_Rates;
	while(stream_Transformation_Rates >> number) {
		Transformation_Rates.push_back(number);
	}
	cell_transformations.transformation_rates.clear();
	cell_transformations.transformation_rates = Transformation_Rates;

	std::getline(is, dummy);
	std::getline(is, dummy);
	return is;

}

// Stream operator for Phenotyope class
std::ostream& operator<<(std::ostream& os, const Phenotype& phenotype) {
		os << "Flagged_for_division: " << (phenotype.flagged_for_division ? "true" : "false") << std::endl;
		os << "Flagged_for_removal: " << (phenotype.flagged_for_removal ? "true" : "false") << std::endl;
		os << "Death:\n" << phenotype.death << std::endl;
        os << "Cycle:\n" << phenotype.cycle << std::endl;
        os << "Volume:\n" << phenotype.volume << std::endl;
        os << "Geometry:\n" << phenotype.geometry << std::endl;
        os << "Mechanics:\n" << phenotype.mechanics << std::endl;
        os << "Motility:\n" << phenotype.motility << std::endl;
        os << "Secretion:\n" << phenotype.secretion << std::endl;
        os << "Molecular:\n" << phenotype.molecular << std::endl;
		os << "Cell_Interactions:\n" << phenotype.cell_interactions << std::endl;
		os << "Cell_Transformations:\n" << phenotype.cell_transformations << std::endl;
        return os;
    }

};
double read_number_in_line(const std::string& line){
    std::istringstream stream(line);
    std::string key;
    stream >> key;
    std::string numberStr;
    if(stream >> numberStr) {
        try {
            long double value = std::stod(numberStr);
			
            return value;
        } catch (std::invalid_argument& e) {
            throw std::runtime_error("Failed to parse value from line");
        } catch (std::out_of_range& e) {
            throw std::runtime_error("Value out of range for type");
        }
    }
    throw std::runtime_error("Failed to parse value from line");
};

int read_number_in_line_int(const std::string& line){
    std::istringstream stream(line);
    std::string key;
    stream >> key;
    std::string numberStr;
    if(stream >> numberStr) {
        try {
            int value = std::stoi(numberStr);
            return value;
        } catch (std::invalid_argument& e) {
            throw std::runtime_error("Failed to parse value from line");
        } catch (std::out_of_range& e) {
            throw std::runtime_error("Value out of range for type");
        }
    }
    throw std::runtime_error("Failed to parse value from line");
};

bool read_number_in_line_bool(const std::string& line){
	std::istringstream stream(line);
	std::string key, value;
	stream >> key >> value;
	
	
	if(value == "true"){
		return true;
	}
	else if(value == "false"){
		return false;
	}
	else{
		throw std::runtime_error("Failed to parse value from line");
	}
};
