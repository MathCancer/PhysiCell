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

#include "./PhysiCell_rules.h"

namespace PhysiCell{

#ifndef __PhysiCell_rules_cpp__
#define __PhysiCell_rules_cpp__
#endif 

Hypothesis_Rule::Hypothesis_Rule()
{
	signals_map.clear();  

	behavior = "none"; 
	base_value = 1.0; 
	max_value = 10.0; 
	min_value = 0.1; 

	signals.resize(0);  
	responses.resize(0); 
	half_maxes.resize(0); 
	hill_powers.resize(0);  
	applies_to_dead_cells.resize(0); 

	up_signals.resize(0);  
	up_half_maxes.resize(0);  
	up_hill_powers.resize(0);  
	up_applies_to_dead_cells.resize(0); 

	down_signals.resize(0);  
	down_half_maxes.resize(0);  
	down_hill_powers.resize(0);  
	down_applies_to_dead_cells.resize(0); 

	cell_type = "none"; 
	pCell_Definition = NULL; 

	return; 
}

std::string convert_bool_to_response( bool input )
{
	if( input )
	{ return "increases"; }
	return "decreases"; 
}

/*
double multivariate_Hill_response_function( std::vector<double> signals, std::vector<double> half_maxes , std::vector<double> hill_powers )
{
	double temp1 = 0.0; 
	double temp2 = 0.0; 
	double temp3 = 0.0; 
	// create the generalized (s^h), stored in temp1; 
	for( int j=0 ; j < signals.size(); j++ )
	{
		temp2 = signals[j];     // s
		temp2 /= half_maxes[j]; // s/s_half 
		temp3 = pow( temp2 , hill_powers[j] ); // (s/s_half)^h 
		temp1 += temp3; 
	}
	temp2 = temp1;   // numerator (S^h)
	temp1 += 1.0;    // denominator (1+S^h)
	temp2 /= temp1;  // numerator/denominator = S^h / (1+S^h)
	return temp2; 
}

double multivariate_linear_response_function( std::vector<double> signals, std::vector<double> min_thresholds , std::vector<double> max_thresholds ) 
{
    double output = 0.0; 

	for( int j=0 ; j < signals.size(); j++ )
	{ output += linear_response_function( signals[j] , min_thresholds[j], max_thresholds[j] ); }

    if( output > 1.0 )
    { return 1.0; }

	return output; 
}
*/

void Hypothesis_Rule::display( std::ostream& os )
{
	os << "For cell type " << cell_type << ": " << std::endl; 
	for( int j=0; j < signals.size(); j++ )
	{ os << signals[j] << " " << convert_bool_to_response( responses[j] ) << " " << behavior << std::endl; }

	return; 
}

void Hypothesis_Rule::reduced_display( std::ostream& os )
{
	for( int j=0; j < signals.size(); j++ )
	{ os << signals[j] << " " << convert_bool_to_response( responses[j] ) << " " << behavior << std::endl; }

	return; 
}

void Hypothesis_Rule::detailed_display( std::ostream& os )
{
	// os << "For cell type " << cell_type << ": " << std::endl; 
	os << behavior << " is modulated from " << min_value << " to " << max_value << " with a base value of " << base_value << std::endl; 
	os << "--------------------------------------------------------" << std::endl; 
	for( int j=0; j < signals.size(); j++ )
	{
		os << "\t" << signals[j] << " " << convert_bool_to_response( responses[j] ) << " " << behavior
			<< " with half-max " << half_maxes[j] << " and Hill power " << hill_powers[j] << "."; 
		if( applies_to_dead_cells[j] == true )
		{ os << " Rule applies to dead cells."; }
		std::cout << std::endl; 
	}
	return; 
}

void Hypothesis_Rule::English_detailed_display( std::ostream& os )
{
	for( int j=0 ; j < signals.size(); j++ )
	{
		os << signals[j] << " "; 
		if( responses[j] == true )
		{ os << "increases "; }
		else
		{ os << "decreases "; }
		os << behavior << " from " << base_value << " towards " ; 
		if( responses[j] == true )
		{ os << max_value; }
		else
		{ os << min_value; }
		os << " with a Hill response, with half-max " << half_maxes[j] ; 
		os << " and Hill power " << hill_powers[j] << ".";
		if( applies_to_dead_cells[j] == true )
		{ os << " Rule applies to dead cells."; }
		os << std::endl; 
	}
}

void Hypothesis_Rule::English_detailed_display_HTML( std::ostream& os )
{
	for( int j=0 ; j < signals.size(); j++ )
	{
		os << "<li>" << signals[j] << " "; 
		if( responses[j] == true )
		{ os << "increases "; }
		else
		{ os << "decreases "; }
		os << behavior << " from " << base_value << " towards " ; 
		if( responses[j] == true )
		{ os << max_value; }
		else
		{ os << min_value; }
		os << " with a Hill response, with half-max " << half_maxes[j] ; 
		os << " and Hill power " << hill_powers[j] << ".";
		if( applies_to_dead_cells[j] == true )
		{ os << " Rule applies to dead cells."; }
		os << "</li>" << std::endl; 
	}
}

void Hypothesis_Rule::English_display( std::ostream& os )
{
	for( int j=0 ; j < signals.size(); j++ )
	{
		os << signals[j] << " "; 
		if( responses[j] == true )
		{ os << "increases "; }
		else
		{ os << "decreases "; }
		os << behavior << std::endl; 
	}
}

void Hypothesis_Rule::English_display_HTML( std::ostream& os )
{
	for( int j=0 ; j < signals.size(); j++ )
	{
		os << "<li>" << signals[j] << " "; 
		if( responses[j] == true )
		{ os << "increases "; }
		else
		{ os << "decreases "; }
		os << behavior << "</li>" << std::endl; 
	}
}

void Hypothesis_Rule::add_signal( std::string signal , double half_max , double hill_power , std::string response )
{
    // check: is this a valid signal? (is it in the dictionary?)
    if( find_signal_index(signal) < 0 )
    {
        std::cout << "Warning! Attempted to add signal " << signal << " which is not in the dictionary." << std::endl; 
        std::cout << "Either fix your model or add the missing signal to the simulation." << std::endl; 

        exit(-1); 
    }

	// check to see if it's already there 
	int n = find_signal(signal); 

	// if so, then just warn and exit.  
	if( n > -1 )
	{
		std::cout << "Warning! Signal " << signal << " was already part of the rule. Ignoring input." << 
		std::endl; 

		return; 
	}

	// add the signal; 
	signals_map[signal] = signals_map.size(); 

	bool bResponse = false; // true if up-regulate, false if down
	if( response == "increase" || response == "increases" || response == "promotes" )
	{ bResponse = true; }

	signals.push_back( signal ); 
	half_maxes.push_back( half_max ); 
	hill_powers.push_back( hill_power );
	responses.push_back( bResponse ); 
	applies_to_dead_cells.push_back( false ); 

	// separate into up and down for our convenience 
	if( bResponse == true )
	{
		up_signals.push_back( signal ); 
		up_half_maxes.push_back( half_max ); 
		up_hill_powers.push_back( hill_power ); 
		up_applies_to_dead_cells.push_back( false ); 
	}
	else
	{
		down_signals.push_back( signal ); 
		down_half_maxes.push_back( half_max ); 
		down_hill_powers.push_back( hill_power ); 
		down_applies_to_dead_cells.push_back( false ); 
	}
	return; 
}

void Hypothesis_Rule::add_signal( std::string signal , std::string response )
{ return add_signal( signal, 0.5 , 3.0 , response ); }

double Hypothesis_Rule::evaluate( std::vector<double> signal_values , bool dead )
{
	// create signals 
	std::vector<double> up_signal(0,0); //  up_signals.size() , 0.0 ); 
	std::vector<double> down_signal(0,0); //  down_signals.size() , 0.0 ); 

	bool apply_rule = false; 

    // need to modify to evaluate if cell is live or if the rule is allowed for dead cells 

	for( int j=0; j < signal_values.size(); j++ )
	{ 
		if( applies_to_dead_cells[j] == true || dead == false )
		{
			if( responses[j] )
			{ up_signal.push_back( signal_values[j]);  }
			else
			{ down_signal.push_back( signal_values[j]);  }

			apply_rule = true; 
		}
		else
		{
			// new oin sep 7 , 2022
			if( responses[j] )
			{ up_signal.push_back( 0.0 );  }
			else
			{ down_signal.push_back( 0.0 );  }
		}
	}

	// March 27, 2023
	// if none of the rules apply, the return an absurdly low value 
	// to signal that the parameter value shoudl not be written 
	if( apply_rule == false )
	{ return -9e99; }

	// up-regulation part 
	double HU = multivariate_Hill_response_function(up_signal,up_half_maxes,up_hill_powers); 
	double U = base_value + (max_value-base_value)*HU; 

	// then the down-regulation part 
	double DU = multivariate_Hill_response_function(down_signal,down_half_maxes,down_hill_powers); 
	double output = U + (min_value-U)*DU; 

	return output; 
}

double Hypothesis_Rule::evaluate( std::vector<double> signal_values )
{ return Hypothesis_Rule::evaluate( signal_values , true ); } 

double Hypothesis_Rule::evaluate( Cell* pCell )
{
	// construct signal vector 
	std::vector<double> signal_values( signals.size() , 0.0 ); 
	for( int i=0; i < signals.size(); i++ )
	{ signal_values[i] = get_single_signal( pCell , signals[i] ); }

	// now, get live/dead value 
	bool dead = (bool) get_single_signal( pCell, "dead" ); 

	double out = evaluate( signal_values , dead ); 

	// new March 27, 2023 
	// if the rule was found to not apply, then just get the prior value 
	if( out < -9e90 )
	{ out = get_single_behavior( pCell , this->behavior ); }

	return out; 
}

void Hypothesis_Rule::apply( Cell* pCell )
{


	// evaluate the rule 
	double param = evaluate( pCell ); 

	// apply it ot the appropriate behavior 
	set_single_behavior( pCell , behavior , param ); 

	return; 
}

void Hypothesis_Rule::sync_to_cell_definition( Cell_Definition* pCD )
{
	if( pCD == NULL )
	{ return; }

	cell_type = pCD->name; 
	pCell_Definition = pCD; 

	// sync base behavior 
	base_value = get_single_base_behavior(pCD,behavior); 

	return; 
}

void Hypothesis_Rule::sync_to_cell_definition( std::string cell_name )
{ return sync_to_cell_definition( find_cell_definition(cell_name) ); }

int Hypothesis_Rule::find_signal( std::string name )
{
	auto search = signals_map.find(name);

	if( search == signals_map.end() )
	{ return -1; }

	return search->second; 
}

void Hypothesis_Rule::set_half_max( std::string name , double hm )
{	
	int n = find_signal( name ); 
	if( n < 0 )
	{ return; }

	half_maxes[n] = hm;

	if( responses[n] == true ) 
	{
		for( int m=0; m < up_signals.size(); m++ )
		{
			if( up_signals[m] == name )
			{ up_half_maxes[m] = hm; }
		}	
	}
	else
	{
		for( int m=0; m < down_signals.size(); m++ )
		{
			if( down_signals[m] == name )
			{ down_half_maxes[m] = hm; }
		}
	}
	return; 
}

void Hypothesis_Rule::set_hill_power( std::string name , double hp )
{
	int n = find_signal( name ); 
	if( n < 0 )
	{ return; }

	hill_powers[n] = hp;
	if( responses[n] == true ) 
	{   
		for( int m=0; m < up_signals.size(); m++ )
		{
			if( up_signals[m] == name )
			{ up_hill_powers[m] = hp; }
		}		
	}
	else
	{
		for( int m=0; m < down_signals.size(); m++ )
		{
			if( down_signals[m] == name )
			{ down_hill_powers[m] = hp; }
		}	
	}
	return; 
}

void Hypothesis_Rule::set_response( std::string name , std::string response )
{
	int n = find_signal( name ); 
	if( n < 0 )
	{ return; }

	bool bResponse = false; // true if up-regulate, false if down
	if( response == "increase" || response == "increases" || response == "promotes" )
	{ bResponse = true; }

	// this is already my response? if so exit 
	if( bResponse == responses[n] )
	{ return; }

	if( responses[n] == true ) 
	{   
		// need to switch from up to down 
			// find current index 
		int ci = -1; 
		for( int m=0; m < up_signals.size(); m++ )
		{
			if( up_signals[m] == name )
			{ ci = m; }
		}
			// swap last inot that position 
		up_half_maxes[ci] = up_half_maxes.back(); 
		up_hill_powers[ci] = up_hill_powers.back(); 
		up_signals[ci] = up_signals.back(); 
		up_applies_to_dead_cells[ci] = up_applies_to_dead_cells.back(); 

			// reduce size by one

		up_half_maxes.pop_back(); 
		up_hill_powers.pop_back(); 
		up_signals.pop_back(); 
		up_applies_to_dead_cells.pop_back(); 

			// move to the other side 

		down_half_maxes.push_back( half_maxes[n] ); 
		down_hill_powers.push_back( hill_powers[n]); 
		down_signals.push_back( signals[n] ); 
		down_applies_to_dead_cells.push_back( applies_to_dead_cells[n] ); 
	}
	else
	{
		// need to switch from down to up 
			// find current index 
		int ci = -1; 
		for( int m=0; m < down_signals.size(); m++ )
		{
			if( down_signals[m] == name )
			{ ci = m; }
		}
			// swap last inot that position 
		down_half_maxes[ci] = down_half_maxes.back(); 
		down_hill_powers[ci] = down_hill_powers.back(); 
		down_signals[ci] = down_signals.back(); 
		down_applies_to_dead_cells[ci] = up_applies_to_dead_cells.back(); 

			// reduce size by one

		down_half_maxes.pop_back(); 
		down_hill_powers.pop_back(); 
		down_signals.pop_back(); 
		down_applies_to_dead_cells.pop_back(); 

			// move to the other side 

		up_half_maxes.push_back( half_maxes[n] ); 
		up_hill_powers.push_back( hill_powers[n]); 
		up_signals.push_back( signals[n] ); 
		up_applies_to_dead_cells.push_back( applies_to_dead_cells[n] ); 
	}

	responses[n] = bResponse; 

	return; 
}

/* add this to the core library! */ 
/*
double get_single_base_behavior( Cell_Definition* pCD , std::string name )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	int index = find_behavior_index(name); 

	if( index < 0 )
	{
		std::cout << "Warning: attempted to get behavior " << name << " with unknown index " << index << std::endl	
				  << "         I'm ignoring it, but you should fix it!" << std::endl; 
		return 0.0; 
	}

	// substrate-related behaviors 

	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	if( index >= first_secretion_index && index < first_secretion_index + m )
	{ return pCD->phenotype.secretion.secretion_rates[index-first_secretion_index]; }

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	if( index >= first_secretion_target_index && index < first_secretion_target_index + m )
	{ return pCD->phenotype.secretion.saturation_densities[index-first_secretion_target_index]; }

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	if( index >= first_uptake_index && index < first_uptake_index + m )
	{ return pCD->phenotype.secretion.uptake_rates[index-first_uptake_index]; }

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	if( index >= first_export_index && index < first_export_index + m )
	{ return pCD->phenotype.secretion.net_export_rates[index-first_export_index]; }

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	int max_cycle_index = pCD->phenotype.cycle.model().phases.size(); 
	if( max_cycle_index > 6 )
	{
		max_cycle_index = 6; 
		std::cout << "Warning: Standardized behaviors only support exit rate from the first 6 phases of a cell cycle!" << std::endl 
		          << "         Ignoring any later phase exit rates." << std::endl; 
	}
	if( index >= first_cycle_index && index < first_cycle_index + 6 )
	{
		int ind = index - first_cycle_index; 
		if( ind < max_cycle_index )
		{ return pCD->phenotype.cycle.data.exit_rate( ind ); }
		return 0.0; 
	}

	static int apoptosis_index = pCD->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int necrosis_index = pCD->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 

	static int apop_param_index = find_behavior_index( "apoptosis"); 
	static int necr_param_index = find_behavior_index( "necrosis"); 

	// apoptosis 
	if( index == apop_param_index )
	{ return pCD->phenotype.death.rates[apoptosis_index]; }

	// necrosis 
	if( index == necr_param_index )
	{ return pCD->phenotype.death.rates[necrosis_index]; }

	// migration speed
	static int migr_spd_index = find_behavior_index( "migration speed"); 
	if( index == migr_spd_index )
	{ return pCD->phenotype.motility.migration_speed; }

	// migration bias 
	static int migr_bias_index = find_behavior_index( "migration bias"); 
	if( index == migr_bias_index )
	{ return pCD->phenotype.motility.migration_bias; }

	// migration persistence time
	static int migr_pt_index = find_behavior_index( "migration persistence time"); 
	if( index == migr_pt_index )
	{ return pCD->phenotype.motility.persistence_time; }

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	if( index >= first_chemotaxis_index && index < first_chemotaxis_index + m )
	{ return pCD->phenotype.motility.chemotactic_sensitivities[index-first_chemotaxis_index]; }

	// cell-cell adhesion 
	static int cca_index = find_behavior_index( "cell-cell adhesion" ); 
	if( index == cca_index )
	{ return pCD->phenotype.mechanics.cell_cell_adhesion_strength; }

	// cell-cell "springs"
	static int cca_spring_index = find_behavior_index( "cell-cell adhesion elastic constant" );  
	if( index == cca_spring_index )
	{ return pCD->phenotype.mechanics.attachment_elastic_constant; }

    // cell adhesion affinities 
	static int first_affinity_index = find_behavior_index("adhesive affinity to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_affinity_index && index < first_affinity_index + n )
	{ return pCD->phenotype.mechanics.cell_adhesion_affinities[index-first_affinity_index]; }

	// max relative maximum adhesion distance 
	static int max_adh_index = find_behavior_index("relative maximum adhesion distance" ); 
	if( index == max_adh_index )
	{ return pCD->phenotype.mechanics.relative_maximum_adhesion_distance; }

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index("cell-cell repulsion" ); 
	if( index == ccr_index )
	{ return pCD->phenotype.mechanics.cell_cell_repulsion_strength; }

	// cell-BM adhesion 
	static int cba_index = find_behavior_index("cell-BM adhesion" ); 
	if( index == cba_index )
	{ return pCD->phenotype.mechanics.cell_BM_adhesion_strength; }
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index("cell-BM repulsion" ); 
	if( index == cbr_index )
	{ return pCD->phenotype.mechanics.cell_BM_repulsion_strength; }

	// dead cell phagocytosis
	static int dead_phag_index = find_behavior_index("phagocytose dead cell" ); 
	if( index == dead_phag_index )
	{ return pCD->phenotype.cell_interactions.dead_phagocytosis_rate; }

    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	if( index >= first_phagocytosis_index && index < first_phagocytosis_index + n )
	{ return pCD->phenotype.cell_interactions.live_phagocytosis_rates[index-first_phagocytosis_index]; } 

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	if( index >= first_attack_index && index < first_attack_index + n )
	{ return pCD->phenotype.cell_interactions.attack_rates[index-first_attack_index]; } 

	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_fusion_index && index < first_fusion_index + n )
	{ return pCD->phenotype.cell_interactions.fusion_rates[index-first_fusion_index]; } 

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_transformation_index && index < first_transformation_index + n )
	{ return pCD->phenotype.cell_transformations.transformation_rates[index-first_transformation_index]; } 

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCD->custom_data.variables.size();  
	if( first_custom_ind >= 0 && index >= first_custom_ind && index < max_custom_ind )
	{ return pCD->custom_data.variables[index-first_custom_ind].value; }

	return -1; 
}
*/

Hypothesis_Ruleset::Hypothesis_Ruleset()
{
	cell_type = "none"; 
	pCell_Definition = NULL; 

	rules.resize(0); 
	rules_map.clear(); 

	return; 
}

void Hypothesis_Ruleset::display( std::ostream& os )
{
	os << "Behavioral rules for cell type " << cell_type << ":" << std::endl; 
	os << "===================================================" << std::endl; 
	for( int i=0; i < rules.size() ; i++ )
	{ rules[i].reduced_display(os); }
	os << std::endl; 
	return; 
}

void Hypothesis_Ruleset::detailed_display( std::ostream& os )
{
	os << "Behavioral rules for cell type " << cell_type << ":" << std::endl; 
	os << "===================================================" << std::endl; 
	for( int i=0; i < rules.size() ; i++ )
	{ rules[i].detailed_display(os); }
	os << std::endl; 
	return; 
}


void Hypothesis_Ruleset::sync_to_cell_definition( Cell_Definition* pCD )
{
	pCell_Definition = pCD; 
	cell_type = pCD->name; 

	for( int i=0; i < rules.size(); i++ )
	{ rules[i].sync_to_cell_definition(pCD); }

	return; 
}

Hypothesis_Rule* Hypothesis_Ruleset::add_behavior( std::string behavior , double min_behavior, double max_behavior )
{
    // check: is this a valid signal? (is it in the dictionary?)
    if( find_behavior_index(behavior) < 0 )
    {
        std::cout << "Warning! Attempted to add behavior " << behavior << " which is not in the dictionary." << std::endl; 
        std::cout << "Either fix your model or add the missing behavior to the simulation." << std::endl; 

        exit(-1); 
    }

	// first, check. Is there already a ruleset? 
	auto search = rules_map.find( behavior ); 

		// if not, add it 
	if( search == rules_map.end() )
	{
		Hypothesis_Rule hr; 

		hr.behavior = behavior; 

		hr.sync_to_cell_definition( pCell_Definition ); 

		hr.min_value = min_behavior; 
		hr.max_value = max_behavior; 

		rules.push_back( hr ); 
		Hypothesis_Rule* pHR = &(rules.back()); 
		rules_map[ behavior ] = pHR; 

		return pHR; 
	}

		// otherwise, edit it 
	Hypothesis_Rule* pHR = search->second; 

	/*
		// March 28 2023 fix  : let's not overwrite eixsting values
	pHR->min_value = min_behavior; 
	pHR->max_value = max_behavior; 
	*/

	return pHR; 
}

Hypothesis_Rule* Hypothesis_Ruleset::add_behavior( std::string behavior )
{ 
	double min_behavior = 0.1; 
	double max_behavior = 1.0; 
	return Hypothesis_Ruleset::add_behavior( behavior, min_behavior, max_behavior );
}

void Hypothesis_Ruleset::sync_to_cell_definition( std::string cell_name )
{ return sync_to_cell_definition( find_cell_definition(cell_name) ); }

Hypothesis_Rule* Hypothesis_Ruleset::find_behavior( std::string name )
{
    auto search = rules_map.find( name); 
	if( search == rules_map.end() )
	{
		// std::cout << "Warning! Ruleset does not contain " << name << std::endl; 
		// std::cout << "         Returning NULL." << std::endl; 
		return NULL; 
	}

	return search->second; 
}

Hypothesis_Rule& Hypothesis_Ruleset::operator[]( std::string name )
{
	Hypothesis_Rule* pHR = find_behavior(name);
	return *pHR; 
} 

void Hypothesis_Ruleset::apply( Cell* pCell )
{
	for( int n=0; n < rules.size() ; n++ )
	{ rules[n].apply( pCell );  }
	return; 
}

std::unordered_map< Cell_Definition* , Hypothesis_Ruleset > hypothesis_rulesets; 

void add_hypothesis_ruleset( Cell_Definition* pCD )
{
	auto search = hypothesis_rulesets.find( pCD );
	if( search == hypothesis_rulesets.end() )
	{
		Hypothesis_Ruleset HRS; 
		HRS.sync_to_cell_definition( pCD ); 
		hypothesis_rulesets[pCD] = HRS; 
	}
	return; 
}

void intialize_hypothesis_rulesets( void )
{
	hypothesis_rulesets.clear(); // empty(); 

	for( int n; n < cell_definitions_by_index.size() ; n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		add_hypothesis_ruleset(pCD); 
	}

	return; 
}

Hypothesis_Ruleset& access_ruleset( Cell_Definition* pCD )
{ return hypothesis_rulesets[pCD]; }

Hypothesis_Ruleset* find_ruleset( Cell_Definition* pCD )
{ return &(hypothesis_rulesets[pCD]); }

void display_hypothesis_rulesets( std::ostream& os )
{
	for( int n=0 ; n < cell_definitions_by_index.size() ; n++ )
	{ hypothesis_rulesets[ cell_definitions_by_index[n] ].display( os ); }

	return; 
}

void detailed_display_hypothesis_rulesets( std::ostream& os )
{
	for( int n=0 ; n < cell_definitions_by_index.size() ; n++ )
	{ hypothesis_rulesets[ cell_definitions_by_index[n] ].detailed_display( os ); }

	return; 
}

void add_rule( std::string cell_type, std::string signal, std::string behavior , std::string response )
{
	Cell_Definition* pCD = find_cell_definition(cell_type); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to add rule for " << cell_type  
            << ", but no cell definition found for this type." << std::endl; 
        exit(-1); 
    }

	Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
    if( !pHRS )
    {
        std::cout << "Warning: Attempted to add rule for " << cell_type  
            << ", but no hypothesis ruleset found for this type." << std::endl; 
        exit(-1); 
    }

	if( pHRS->find_behavior(behavior) )
	{
		if( (*pHRS)[behavior].behavior != behavior )
		{ (*pHRS)[behavior].behavior = behavior; std::cout << "wha?" << std::endl; }
	}

	pHRS->add_behavior(behavior); 

	(*pHRS)[behavior].add_signal(signal,response); 

	return; 
}

void add_rule( std::string cell_type, std::string signal, std::string behavior , std::string response , bool use_for_dead )
{
	Cell_Definition* pCD = find_cell_definition(cell_type); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to add rule for " << cell_type  
            << ", but no cell definition found for this type." << std::endl; 
        exit(-1); 
    }

	Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
    if( !pHRS )
    {
        std::cout << "Warning: Attempted to add rule for " << cell_type  
            << ", but no hypothesis ruleset found for this type." << std::endl; 
        exit(-1); 
    }

	if( pHRS->find_behavior(behavior) )
	{
		if( (*pHRS)[behavior].behavior != behavior )
		{ (*pHRS)[behavior].behavior = behavior; std::cout << "wha?" << std::endl; }
	}

	pHRS->add_behavior(behavior); 

	(*pHRS)[behavior].add_signal(signal,response); 

	// set dead flag

	int n = (*pHRS)[behavior].find_signal(signal); 
	(*pHRS)[behavior].applies_to_dead_cells[n] = use_for_dead; 

	return; 
}



void set_hypothesis_parameters(std::string cell_type, std::string signal, std::string behavior , 
   double half_max, double hill_power )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " modulated by " << signal 
          << " in " << cell_type  
            << ", but no cell definition found for this type." << std::endl; 
        exit(-1); 
    }

	if( find_ruleset(pCD) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " modulated by " << signal 
          << " in " << cell_type  
            << ", but no behavior ruleset for this cell type." << std::endl; 
        exit(-1); 
    }

	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " modulated by " << signal 
          << " in " << cell_type  
            << ", but the cell type has no rules for this behavior." << std::endl; 
        exit(-1); 
        
     // std::cout << "Error. No " << behavior << " rules for " << cell_type << ". Ignoring."<< std::endl; return;
    }
		
	if( hypothesis_rulesets[pCD][behavior].find_signal(signal) < 0 )
	{ 
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " modulated by " << signal 
          << " in " << cell_type  
            << ", but the cell type's behavior does not vary with this signal." << std::endl; 
        exit(-1); 
    }

	hypothesis_rulesets[pCD][behavior].set_half_max(signal,half_max); 
	hypothesis_rulesets[pCD][behavior].set_hill_power(signal,hill_power); 
	
	return; 
}

void set_behavior_parameters( std::string cell_type, std::string behavior, 
   double min_value, double max_value )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but the cell definition is not found." << std::endl; 
        exit(-1);         
    }

	if( find_ruleset(pCD) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but there is no hypothesis ruleset for this cell type." << std::endl; 
        exit(-1);              
    }

	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but there is no rules for this behavior for this cell type." << std::endl; 
        exit(-1);              
    }
	
	hypothesis_rulesets[pCD][behavior].min_value = min_value; 
	hypothesis_rulesets[pCD][behavior].max_value = max_value; 
	
	return;
}

void set_behavior_parameters( std::string cell_type, std::string behavior, 
   double min_value, double base_value , double max_value )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but the cell definition is not found." << std::endl; 
        exit(-1);         
    }

	if( find_ruleset(pCD) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but there is no hypothesis ruleset for this cell type." << std::endl; 
        exit(-1);              
    }


	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
	{
        std::cout << "Warning: Attempted to set parameters for " 
          << behavior << " in " << cell_type  
            << ", but there is no rules for this behavior for this cell type." << std::endl; 
        exit(-1);              
    }

	hypothesis_rulesets[pCD][behavior].min_value = min_value; 
	hypothesis_rulesets[pCD][behavior].max_value = max_value; 
	hypothesis_rulesets[pCD][behavior].base_value = base_value; 
	
	return;
}


void set_behavior_base_value( std::string cell_type, std::string behavior, double base_value )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but the cell definition is not found." << std::endl; 
        exit(-1);         
    }

	if( find_ruleset(pCD) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no hypothesis ruleset found for this cell type." << std::endl; 
        exit(-1);         
    }

	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no rule for this behavior found for this cell type." << std::endl; 
        exit(-1);         
    }

	hypothesis_rulesets[pCD][behavior].base_value = base_value; 

	return;
}

void set_behavior_min_value( std::string cell_type, std::string behavior, double min_value )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but the cell definition is not found." << std::endl; 
        exit(-1);         
    }

	if( find_ruleset(pCD) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no hypothesis ruleset found for this cell type." << std::endl; 
        exit(-1);         
    }

	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no rule for this behavior found for this cell type." << std::endl; 
        exit(-1);         
    }

	hypothesis_rulesets[pCD][behavior].min_value = min_value; 

	return;
}

void set_behavior_max_value( std::string cell_type, std::string behavior, double max_value )
{
	Cell_Definition* pCD = find_cell_definition( cell_type ); 
    if( !pCD )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but the cell definition is not found." << std::endl; 
        exit(-1);         
    }

	if( find_ruleset(pCD) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no hypothesis ruleset found for this cell type." << std::endl; 
        exit(-1);         
    }

	if( hypothesis_rulesets[pCD].find_behavior(behavior) == NULL )
    {
        std::cout << "Warning: Attempted to set base parameter for " 
          << behavior << " in " << cell_type  
            << ", but no rule for this behavior found for this cell type." << std::endl; 
        exit(-1);         
    }

	hypothesis_rulesets[pCD][behavior].max_value = max_value;
	
	return;
}



void apply_ruleset( Cell* pCell )
{
	Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 
	hypothesis_rulesets[pCD].apply( pCell );
	return; 
}

void rule_phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	apply_ruleset( pCell );

	// by default, rules only apply to live cells
	// 

/*
	// safety checks for dead cells 
	if( get_single_signal(pCell,"dead") > 0.11 )
	{
		// can't die twice
		set_single_behavior(pCell,"apoptosis",0.0);
		set_single_behavior(pCell,"necrosis",0.0);

		// can't cycle 
		set_single_behavior(pCell,"cycle entry",0.0);

		// can't crawl 
		set_single_behavior(pCell,"migration speed",0.0);
	}
*/

	return; 	
}

/* add these to core */ 
/*
std::vector<double> linear_response_to_Hill_parameters( double s0, double s1 )
{
	static double tol = 0.1; 
	static double param1 = (1-tol)/tol; 
	static double param2 = log(param1); 

	// half max, then hill power 
	double hm = 0.5* (s0+s1); 

	// hp so that H(s1) ~ (1-tol)
	double hp = round( param2 / log(s1/hm) ); 

	std::vector<double> output = { hm , hp }; 

	return output; 
}

std::vector<double> Hill_response_to_linear_parameters( double half_max , double Hill_power )
{
	static double tol = 0.1; 
	static double param1 = (1-tol)/tol; 
	double param2 = pow( param1 , 1.0/ Hill_power ); 

	// s1 such that H(s1) ~ (1-tol)
	double s1 = half_max * param2; 

	// s0 for symmetry
	double s0 = 2*half_max -s1; 
	if( s0 < 0 )
	{ s0 = 0.0; }

	std::vector<double> output = {s0,s1}; 

	return output; 
}
*/

void split_csv( std::string input , std::vector<std::string>& output , char delim )
{
	output.resize(0); 

	std::istringstream is(input);
	std::string part;
	while( getline(is, part, delim ) )
	{ output.push_back(part); }

	return; 
}

std::string csv_strings_to_English_v1( std::vector<std::string> strings , bool include_cell_header )
{
	std::string output = ""; 

	if( include_cell_header )
	{
		output += "In "; 
		output += strings[0]; 
		output += " cells:\n\t"; // In {cell type X} cells: 
	}

// malignant epithelial,oxygen,decreases,necrosis,2.80E-03,0,decreases,3.75,8,0

	output += strings[1] ; // {signal}
	output += " ";

	output += strings[2] ; // {increases/decreases}
	output += " ";

	output += strings[3] ; // {behavior}


	output += " from "; // {base}
	output += strings[4]; 

	output += " towards ";
	output += strings[5]; 

	output += " with a Hill response, with half-max "; 
	output += strings[6]; 

	output += " and Hill power "; 
	output += strings[7]; 

	output += "."; 
	bool use_when_dead = false; 
	char start_char = toupper( strings[8][0] ); 
	if( start_char == 'T' || start_char == '1' )
	{ output += " Rule applies to dead cells."; }
	
	return output; 
}

std::string csv_strings_to_English_v2( std::vector<std::string> strings , bool include_cell_header )
{
	std::string output = ""; 

	if( include_cell_header )
	{
		output += "In "; 
		output += strings[0]; 
		output += " cells:\n\t"; // In {cell type X} cells: 
	}

	output += strings[1] ; // {signal}
	output += " ";

	output += strings[2] ; // {increases/decreases}
	output += " ";

	output += strings[3] ; // {behavior}


//	output += " from "; // {base}
//	output += strings[4]; 

	output += " towards ";
	output += strings[4]; 

	output += " with a Hill response, with half-max "; 
	output += strings[5]; 

	output += " and Hill power "; 
	output += strings[6]; 

	output += "."; 
	bool use_when_dead = false; 
	char start_char = toupper( strings[7][0] ); 
	if( start_char == 'T' || start_char == '1' )
	{ output += " Rule applies to dead cells."; }
	
	return output; 
}


std::string csv_strings_to_English( std::vector<std::string> strings , bool include_cell_header )
{
	std::string output = ""; 

	if( include_cell_header )
	{
		output += "In "; 
		output += strings[0]; 
		output += " cells:\n\t"; // In {cell type X} cells: 
	}

	output += strings[5] ; // {signal}
	output += " ";

	output += strings[6] ; // {increases/decreases}
	output += " ";

	output += strings[1] ; // {behavior}


	output += " from "; // {base}
	output += strings[3]; 

	output += " towards ";
	if( strings[6][0] == 'i' || strings[6][0] == 'I' )
	{ output+= strings[4];  }
	else
	{ output+= strings[2];  }

	output += " with a Hill response, with half-max "; 
	output += strings[7]; 

	output += " and Hill power "; 
	output += strings[8]; 

	output += "."; 
	bool use_when_dead = false; 
	char start_char = toupper( strings[9][0] ); 
	if( start_char == 'T' || start_char == '1' )
	{ output += " Rule applies to dead cells."; }
	
	return output; 
}


std::string csv_strings_to_English_HTML( std::vector<std::string> strings , bool include_cell_header )
{
	std::string output = "<p>"; 

	if( include_cell_header )
	{
		output += "In "; 
		output += strings[0]; 
		output += " cells:<br>\n"; // In {cell type X} cells: 
	}

	output += "&nbsp;";
	output += strings[5] ; // {signal}
	output += " ";

	output += strings[6] ; // {increases/decreases}
	output += " ";

	output += strings[1] ; // {behavior}

	output += " from "; // {base}
	output += strings[3]; 

	output += " towards ";
	if( strings[6][0] == 'i' || strings[6][0] == 'I' )
	{ output+= strings[4];  }
	else
	{ output+= strings[2];  }

	output += " with a Hill response, with half-max "; 
	output += strings[7]; 

	output += " and Hill power "; 
	output += strings[8]; 

	output += "."; 
	bool use_when_dead = false; 
	char start_char = toupper( strings[9][0] ); 
	if( start_char == 'T' || start_char == '1' )
	{ output += " Rule applies to dead cells."; }
	output += "\n</p>\n"; 
	
	return output; 
}


/*
0              1          2        3           4         5       6          7         8           9 
 Cell type, behavior, min value, base value, max value, signal, direction, half-max, Hill power, dead
*/ 

void parse_csv_rule_v0( std::vector<std::string> input )
{
	// if it's wrong length, skip 
	bool skip = false; 
	if( input.size() != 10 )
	{ skip = true; }
	// if any empty strings, skip
	for( int n=0 ; n < input.size() ; n++ )
	{
		if( input[n].empty() == true )
		{ skip = true; }
	}
	if( skip == true )
	{
		std::cout << "Warning: Misformed rule (likely from an empty rules file). Skipping." << std::endl; 
		return; 
	}

	std::string temp = csv_strings_to_English( input , false ); 

	// string portions of the rule
	std::string cell_type = input[0]; 
	std::string behavior = input[1]; 
	std::string signal = input[5]; 
	std::string response = input[6]; 

	// numeric portions of the rule 
	double min_value  = std::atof( input[2].c_str() );
	double base_value = std::atof( input[3].c_str() );
	double max_value  = std::atof( input[4].c_str() ); 

	double half_max  = std::atof( input[7].c_str() );
	double hill_power = std::atof( input[8].c_str() );
	bool use_for_dead = (bool) std::atof( input[9].c_str() ); 

	std::cout << "Adding rule for " << cell_type << " cells:\n\t"; 
	std::cout << temp << std::endl; 

	// add_rule(cell_type,signal,behavior,response);  
	add_rule(cell_type,signal,behavior,response,use_for_dead);  

	set_hypothesis_parameters(cell_type,signal,behavior,half_max,hill_power);  

	// compare to base behavior value in cell def for discrepancies 
	Cell_Definition* pCD = find_cell_definition(cell_type); 
	double ref_base_value = get_single_base_behavior(pCD,behavior); 
	if( fabs(ref_base_value-base_value) > 1e-15 )
	{
		std::cout << "Error: Base value for " << behavior << " in cell type " << cell_type  << std::endl 
				  << "       has base value " << base_value << " in the rule, " // << std::endl 
		 		  << "but base value " << ref_base_value << " in the cell definition." << std::endl
				  << "       Fix this discrepancy to continue the model." << std::endl << std::endl; 
				  exit(-1); 
	}

	// get_single_base_behavior

	set_behavior_parameters(cell_type,behavior,min_value,base_value,max_value); 

/*
	// set "applies to dead"

	int n = access_ruleset(pCD)[behavior].find_signal(signal); 
	access_ruleset(pCD)[behavior].applies_to_dead_cells[n] = use_for_dead; 
*/

	return;  
}

void parse_csv_rule_v0( std::string input )
{
	std::vector<std::string> tokenized_string; 
	split_csv( input , tokenized_string , ','); 

	// Make sure it was truly comma-separated. 
	// If not, try tab.
	if( tokenized_string.size() != 10 )
	{ split_csv( input , tokenized_string , '\t');  }

	return parse_csv_rule_v0( tokenized_string ); 
}

void parse_csv_rules_v0( std::string filename )
{
	std::fstream fs( filename, std::ios::in );
	if( !fs )
	{
		std::cout << "Warning: Rules file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Processing rules in file " << filename << " ... " << std::endl; 

	while( fs.eof() == false )
	{
		std::string line; 	
		std::getline( fs , line, '\n'); 
		if( line.size() > 0 )
		{ parse_csv_rule_v0(line); }
	}

	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}

/* v1 work */

/*
 v0:::
0              1          2        3           4         5       6          7         8           9 
 Cell type, behavior, min value, base value, max value, signal, direction, half-max, Hill power, dead


 v1:::
 0          1       2          3         4           5                   6         7           8
 Cell type, signal, direction, behavior, base value, max response value, half-max, Hill power, applies to dead? 
*/ 

void parse_csv_rule_v1( std::vector<std::string> input )
{
	// if it's wrong length, skip 
	bool skip = false; 
	if( input.size() != 9 )
	{ skip = true; }
	// if any empty strings, skip
	for( int n=0 ; n < input.size() ; n++ )
	{
		if( input[n].empty() == true )
		{ skip = true; }
	}
	if( skip == true )
	{
		std::cout << "Warning: Misformed rule (likely from an empty rules file). Skipping." << std::endl; 

		for( int n=0 ; n < input.size(); n++ )
		{
			std::cout << n << " : " << input[n] << std::endl; 
		}

		return; 
	}

	std::string temp = csv_strings_to_English_v1( input , false ); // need a v1 version of this

	// string portions of the rule
	std::string cell_type = input[0]; 
	std::string signal = input[1]; 
	std::string response = input[2]; 
	std::string behavior = input[3]; 

	// numeric portions of the rule 
	// double min_value  = std::atof( input[2].c_str() );

	double base_value = std::atof( input[4].c_str() );
	double max_response = std::atof( input[5].c_str() ); 

	// hmm from here 
	// double max_value  = std::atof( input[4].c_str() ); 

	double half_max  = std::atof( input[6].c_str() );
	double hill_power = std::atof( input[7].c_str() );
	bool use_for_dead = (bool) std::atof( input[8].c_str() ); 

	std::cout << "Adding rule for " << cell_type << " cells:\n\t"; 
	std::cout << temp << std::endl; 

	// add_rule(cell_type,signal,behavior,response);  
	add_rule(cell_type,signal,behavior,response,use_for_dead);  
	set_hypothesis_parameters(cell_type,signal,behavior,half_max,hill_power);  

	// compare to base behavior value in cell def for discrepancies 
	Cell_Definition* pCD = find_cell_definition(cell_type); 
	double ref_base_value = get_single_base_behavior(pCD,behavior); 
	if( fabs(ref_base_value-base_value) > 1e-15 )
	{
		std::cout << "Error: Base value for " << behavior << " in cell type " << cell_type  << std::endl 
				  << "       has base value " << base_value << " in the rule, " // << std::endl 
		 		  << "but base value " << ref_base_value << " in the cell definition." << std::endl
				  << "       Fix this discrepancy to continue the model." << std::endl << std::endl; 
				  exit(-1); 
	}

	set_behavior_base_value(cell_type,behavior,base_value);

	if( response == "increases")
	{ set_behavior_max_value(cell_type,behavior,max_response); }
	else
	{ set_behavior_min_value(cell_type,behavior,max_response); }

	return;  
}

void parse_csv_rule_v1( std::string input )
{
	std::vector<std::string> tokenized_string; 
	split_csv( input , tokenized_string , ','); 

	// Make sure it was truly comma-separated. 
	// If not, try tab.
	if( tokenized_string.size() != 9 )
	{ split_csv( input , tokenized_string , '\t');  }

	// check for comment 
	if(tokenized_string[0][0] == '/' && tokenized_string[0][1] == '/' )
	{ std::cout << "Skipping commented rule (" << input << ")" << std::endl; return; }	

	return parse_csv_rule_v1( tokenized_string ); 
}

void parse_csv_rules_v1( std::string filename )
{
	std::fstream fs( filename, std::ios::in );
	if( !fs )
	{
		std::cout << "Warning: Rules file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Processing rules in file " << filename << " ... " << std::endl; 

	while( fs.eof() == false )
	{
		std::string line; 	
		std::getline( fs , line, '\n'); 
		if( line.size() > 0 )
		{ parse_csv_rule_v1(line); }
	}

	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}


/* end of v1 work */

/* v2 work */

/*
 v0:::
0              1          2        3           4         5       6          7         8           9 
 Cell type, behavior, min value, base value, max value, signal, direction, half-max, Hill power, dead


 v1:::
 0          1       2          3         4           5                   6         7           8
 Cell type, signal, direction, behavior, base value, max response value, half-max, Hill power, applies to dead? 

 v2:::
 0          1       2          3         4                   5         6           7           
 Cell type, signal, direction, behavior, max response value, half-max, Hill power, applies to dead?  
*/ 

void parse_csv_rule_v2( std::vector<std::string> input )
{
	// if it's wrong length, skip 
	bool skip = false; 
	if( input.size() != 8 )
	{ skip = true; }
	// if any empty strings, skip
	for( int n=0 ; n < input.size() ; n++ )
	{
		if( input[n].empty() == true )
		{ skip = true; }
	}
	if( skip == true )
	{
		std::cout << "Warning: Misformed rule (likely from an empty rules file). Skipping." << std::endl; 

		for( int n=0 ; n < input.size(); n++ )
		{
			std::cout << n << " : " << input[n] << std::endl; 
		}

		return; 
	}

	std::string temp = csv_strings_to_English_v2( input , false ); // need a v1 version of this

	// string portions of the rule
	std::string cell_type = input[0]; 
	std::string signal = input[1]; 
	std::string response = input[2]; 
	std::string behavior = input[3]; 

	// numeric portions of the rule 
	// double min_value  = std::atof( input[2].c_str() );

	// double base_value = std::atof( input[4].c_str() );
	double max_response = std::atof( input[4].c_str() ); 

	// hmm from here 
	// double max_value  = std::atof( input[4].c_str() ); 

	double half_max  = std::atof( input[5].c_str() );
	double hill_power = std::atof( input[6].c_str() );
	bool use_for_dead = (bool) std::atof( input[7].c_str() ); 

	std::cout << "Adding rule for " << cell_type << " cells:\n\t"; 
	std::cout << temp << std::endl; 

	// add_rule(cell_type,signal,behavior,response);  
	add_rule(cell_type,signal,behavior,response,use_for_dead);  
	set_hypothesis_parameters(cell_type,signal,behavior,half_max,hill_power);  

	// compare to base behavior value in cell def for discrepancies 
	Cell_Definition* pCD = find_cell_definition(cell_type); 
	double ref_base_value = get_single_base_behavior(pCD,behavior); 

	set_behavior_base_value(cell_type,behavior,ref_base_value);
	if( response == "increases")
	{ set_behavior_max_value(cell_type,behavior,max_response); }
	else
	{ set_behavior_min_value(cell_type,behavior,max_response); }

	return;  
}

void parse_csv_rule_v2( std::string input )
{
	std::vector<std::string> tokenized_string; 
	split_csv( input , tokenized_string , ','); 

	// Make sure it was truly comma-separated. 
	// If not, try tab.
	if( tokenized_string.size() != 8 )
	{ split_csv( input , tokenized_string , '\t');  }

	// check for comment 
	if(tokenized_string[0][0] == '/' && tokenized_string[0][1] == '/' )
	{ std::cout << "Skipping commented rule (" << input << ")" << std::endl; return; }	

	return parse_csv_rule_v2( tokenized_string ); 
}

void parse_csv_rules_v2( std::string filename )
{
	std::fstream fs( filename, std::ios::in );
	if( !fs )
	{
		std::cout << "Warning: Rules file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Processing rules in file " << filename << " ... " << std::endl; 

	while( fs.eof() == false )
	{
		std::string line; 	
		std::getline( fs , line, '\n'); 
		if( line.size() > 0 )
		{ parse_csv_rule_v2(line); }
	}

	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}


/* end of v2 work */

// needs fixing
void parse_rules_from_pugixml( void )
{
	pugi::xml_node node = physicell_config_root.child( "cell_rules" ); 
	if( !node )
	{ 
		std::cout << "Error: Could not find <cell_rules> section of XML config file." << std::endl 
				 <<  "       Cannot parse cell rules, so disabling." << std::endl; 

		PhysiCell_settings.rules_enabled = false; 
		return; 
	}

	// find the set of rulesets 
	node = node.child( "rulesets" ); 
	if( !node )
	{ 
		std::cout << "Error: Could not find <rulesets> in the <cell_rules> section of XML config file." << std::endl 
				 <<  "       Cannot parse cell rules, so disabling." << std::endl; 

		PhysiCell_settings.rules_enabled = false; 
		return; 
	}
	// find the first ruleset 
	node = node.child( "ruleset");
	if( !node )
	{ 
		std::cout << "Error: Could not find any <ruleset> in the <rulesets> section of XML config file." << std::endl 
				 <<  "       Cannot parse cell rules, so disabling." << std::endl; 

		PhysiCell_settings.rules_enabled = false; 
		return; 
	}

	while( node )
	{
		std::cout << node.name() << std::endl;
		if( node.attribute("enabled").as_bool() == true )
		{ 
			std::string folder = xml_get_string_value( node, "folder" ); 
			std::string filename = xml_get_string_value( node, "filename" ); 
			std::string input_filename = folder + "/" + filename; 

			std::cout << "\tProcessing ruleset in " << input_filename << " ... " << std::endl; 
			std::string format = node.attribute("format").as_string(); 
			std::string protocol = node.attribute("protocol").as_string(); 
			double version = node.attribute("version").as_double(); 

			bool done = false; 

			if( format == "CSV" || format == "csv" )  
			{ 			
				if( version < 1.0 )
				{
					std::cout << "\tFormat: CSV (prototype version)" << std::endl; 

					parse_csv_rules_v0( input_filename ); // parse all rules in a CSV file 

					PhysiCell_settings.rules_enabled = true; 

					done = true; 
				}
			
				if(version >= 1.0 - 1e-10 && version < 2.0 - 1e-10 && protocol == "CBHG" && done == false )
				{
					std::cout << "\tFormat: CSV (version " << version << ")" << std::endl; 

					parse_csv_rules_v1( input_filename ); // parse all rules in a CSV file 

					PhysiCell_settings.rules_enabled = true; 

					done = true; 
				}

				if(version >= 2.0 - 1e-10 && protocol == "CBHG" && done == false )
				{
					std::cout << "\tFormat: CSV (version " << version << ")" << std::endl; 

					parse_csv_rules_v2( input_filename ); // parse all rules in a CSV file 

					PhysiCell_settings.rules_enabled = true; 

					done = true; 
				}

			}  


			if( done == false )
			{ std::cout << "\tWarning: Ruleset had unknown format (" << format << "). Skipping!" << std::endl; }

		}
		else
		{ std::cout << "\tRuleset disabled ... " << std::endl; }
		node = node.next_sibling( "ruleset"); 		
	}
	return; 

	exit(0); 
	
	// enabled? 
	if( node.attribute("enabled").as_bool() == false )
	{ return; }

	// get filename 

	std::string folder = xml_get_string_value( node, "folder" ); 
	std::string filename = xml_get_string_value( node, "filename" ); 
	std::string input_filename = folder + "/" + filename; 

	std::string filetype = node.attribute("type").value() ; 

	// what kind? 
	if( filetype == "csv" || filetype == "CSV" )
	{
		std::cout << "Loading rules from CSV file " << input_filename << " ... " << std::endl; 
		// load_cells_csv( input_filename );
		parse_csv_rules_v0( input_filename ); 
		return; 
	}

	return; 
}

void parse_rules_from_parameters_v0( void )
{
	bool enabled = parameters.bools( "rules_enabled" ); 

	// enabled? 
	if( enabled == false )
	{ return; }

	// get filename 

	std::string folder = parameters.strings( "rules_folder" ); 
	std::string filename = parameters.strings( "rules_filename" ); 
	std::string input_filename = folder + "/" + filename; 

	std::string filetype = parameters.strings( "rules_type" );  ; 

	// what kind? 
	if( filetype == "csv" || filetype == "CSV" )
	{
		std::cout << "Loading rules from CSV file " << input_filename << " ... " << std::endl; 
		// load_cells_csv( input_filename );
		parse_csv_rules_v0( input_filename ); 
		return; 
	}

	return; 
}

void spaces_to_underscore( std::string& str )
{
	for( int n=0 ; n < str.size(); n++ )
	{ if( str[n] == ' ' ){ str[n] = '_'; } }
}

/*
submit this as bugfix in PhysiCell (PhysiCell_settings.cpp)

template <class T>
int Parameters<T>::find_index( std::string search_name )
{
	auto search = name_to_index_map.find( search_name );
	if( search != name_to_index_map.end() )
	{ return search->second; }
	return -1; 
	// return name_to_index_map[ search_name ]; 
}

*/

void stream_annotated_English_rules( std::ostream& os )
{
	os << "Cell Hypothesis Rules" << std::endl << std::endl; 
	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
		os << "In " << pHRS->cell_type << " cells:" << std::endl; 

		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{ pHRS->rules[k].English_display(os); }
		os << std::endl; 
	}
	return; 
} 

void stream_annotated_English_rules_HTML( std::ostream& os )
{
	os << "<html>\n<body><h1>Cell Hypothesis Rules</h1>" << std::endl; 
	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		os << "<p>"; 
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
		os << "In " << pHRS->cell_type << " cells:" << std::endl; 
		os << "<ul>" << std::endl; 
		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{ pHRS->rules[k].English_display_HTML(os); }
		os << "</ul>\n</p>" << std::endl; 
	}
	os << "</body>\n</html>" << std::endl; 
	return; 
	
} 

void save_annotated_English_rules( void )
{
	std::string filename = PhysiCell_settings.folder + "/rules.txt";
	std::ofstream of( filename , std::ios::out );
	stream_annotated_English_rules( of ); 
	of.close(); 
}

void stream_annotated_detailed_English_rules( std::ostream& os )
{
	os << "Cell Hypothesis Rules (detailed)" << std::endl << std::endl; 
	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
		os << "In " << pHRS->cell_type << " cells:" << std::endl; 
		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{ pHRS->rules[k].English_detailed_display(os); }
		os << std::endl; 
	}
	return; 
} 

void stream_annotated_detailed_English_rules_HTML( std::ostream& os )
{
	os << "<html>\n<body><h1>Cell Hypothesis Rules (detailed)</h1>" << std::endl; 
	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		os << "<p>"; 
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 
		os << "In " << pHRS->cell_type << " cells:" << std::endl; 
		os << "<ul>" << std::endl; 
		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{ pHRS->rules[k].English_detailed_display_HTML(os); }
		os << "</ul>\n</p>" << std::endl; 
	}
	os << "</body>\n</html>" << std::endl; 
	return; 
} 

void save_annotated_detailed_English_rules( void )
{
	std::string filename = PhysiCell_settings.folder + "/detailed_rules.txt";
	std::ofstream of( filename , std::ios::out );
	stream_annotated_detailed_English_rules( of ); 
	of.close(); 
}

void save_annotated_detailed_English_rules_HTML( void )
{
	std::string filename = PhysiCell_settings.folder + "/detailed_rules.html";
	std::ofstream of( filename , std::ios::out );
	stream_annotated_detailed_English_rules_HTML( of ); 
	of.close(); 
}

void save_annotated_English_rules_HTML( void )
{
	std::string filename = PhysiCell_settings.folder + "/rules.html";
	std::ofstream of( filename , std::ios::out );
	stream_annotated_English_rules_HTML( of ); 
	of.close(); 
}

// v0 version 

void export_rules_csv_v0( std::string filename )
{
	std::fstream fs( filename, std::ios::out );
	if( !fs )
	{
		std::cout << "Warning: Rules export file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Exporting rules to file " << filename << " (v0 format) ... " << std::endl; 

	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 

		std::string cell_type = pHRS->cell_type; 

		std::cout << cell_type << " :: "; 
		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{
			std::string behavior = pHRS->rules[k].behavior; 
			std::cout << behavior << " : "; 

			double min_value = pHRS->rules[k].min_value; 
			double max_value = pHRS->rules[k].max_value; 
			double base_value = pHRS->rules[k].base_value; 
			for( int i=0; i < pHRS->rules[k].signals.size() ; i++ )
			{
				std::string signal = pHRS->rules[k].signals[i]; 
				std::cout << signal << " "; 
				std::string response; 
				if( pHRS->rules[k].responses[i] == true )
				{ response = "increases"; }
				else
				{ response = "decreases"; }
				double half_max = pHRS->rules[k].half_maxes[i];
				double hill_power = pHRS->rules[k].hill_powers[i];
				bool use_for_dead = false; 

				// output the rule 
				fs << cell_type << "," << behavior << "," 
					<< min_value << "," << base_value << "," << max_value << ","
					<< signal << "," << response << "," 
					<< half_max << "," << hill_power << "," 
					<< use_for_dead << std::endl; 
			}
			std::cout << std::endl; 
		}
	}

/*
0              1          2        3           4          5       6          7         8           9 
 Cell type, behavior, min value, base value, max value,   signal, direction, half-max, Hill power, dead
*/ 
	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}

// v1 

void export_rules_csv_v1( std::string filename )
{
	std::fstream fs( filename, std::ios::out );
	if( !fs )
	{
		std::cout << "Warning: Rules export file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Exporting rules to file " << filename << " (v1 format) ... " << std::endl; 

	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 

		std::string cell_type = pHRS->cell_type; 

		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{
			std::string behavior = pHRS->rules[k].behavior; 

			double min_value = pHRS->rules[k].min_value; 
			double max_value = pHRS->rules[k].max_value; 
			double base_value = pHRS->rules[k].base_value; 
			for( int i=0; i < pHRS->rules[k].signals.size() ; i++ )
			{
				std::string signal = pHRS->rules[k].signals[i]; 
				std::string response; 
				double max_response = -9e99; 
				if( pHRS->rules[k].responses[i] == true )
				{ response = "increases"; max_response = max_value; }
				else
				{ response = "decreases"; max_response = min_value; }
				double half_max = pHRS->rules[k].half_maxes[i];
				double hill_power = pHRS->rules[k].hill_powers[i];
				bool use_for_dead = false; 

				// output the rule 
				fs << cell_type << "," << signal << "," << response << "," << behavior << "," // 0,1,2,3
					<< base_value << "," << max_response << "," << half_max << "," << hill_power << "," // 4,5, 6,7, 
					<< use_for_dead << std::endl; // 8 
			}
		}
	}

/*
 Cell type, signal, direcxtion, behavior, base, max_response, half-max, hill , dead 
*/ 
	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}

void export_rules_csv_v2( std::string filename )
{
	std::fstream fs( filename, std::ios::out );
	if( !fs )
	{
		std::cout << "Warning: Rules export file " << filename << " failed to open." << std::endl; 
		return; 
	}

	std::cout << "Exporting rules to file " << filename << " (v2 format) ... " << std::endl; 

	for( int n=0; n < cell_definitions_by_index.size(); n++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[n]; 
		Hypothesis_Ruleset* pHRS = find_ruleset( pCD ); 

		std::string cell_type = pHRS->cell_type; 

		for( int k=0 ; k < pHRS->rules.size(); k++ ) 
		{
			std::string behavior = pHRS->rules[k].behavior; 

			double min_value = pHRS->rules[k].min_value; 
			double max_value = pHRS->rules[k].max_value; 
			double base_value = pHRS->rules[k].base_value; 
			for( int i=0; i < pHRS->rules[k].signals.size() ; i++ )
			{
				std::string signal = pHRS->rules[k].signals[i]; 
				std::string response; 
				double max_response = -9e99; 
				if( pHRS->rules[k].responses[i] == true )
				{ response = "increases"; max_response = max_value; }
				else
				{ response = "decreases"; max_response = min_value; }
				double half_max = pHRS->rules[k].half_maxes[i];
				double hill_power = pHRS->rules[k].hill_powers[i];
				bool use_for_dead = false; 

				// output the rule 
				fs << cell_type << "," << signal << "," << response << "," << behavior << "," // 0,1,2,3
					// << base_value << "," 
					<< max_response << "," << half_max << "," << hill_power << "," // 4,5, 6,7, 
					<< use_for_dead << std::endl; // 8 
			}
		}
	}

/*
 Cell type, signal, direcxtion, behavior, base, max_response, half-max, hill , dead 
*/ 
	fs.close(); 

	std::cout << "Done!" << std::endl << std::endl; 

	return; 
}


std::vector<double> UniformInUnitDisc( void )
{
	static double two_pi = 6.283185307179586; 
	double theta = UniformRandom(); // U(0,1)
	theta *= two_pi; // U(0,2*pi)
	double r = sqrt( UniformRandom() );  // sqrt( U(0,1) )
	return { r*cos(theta), r*sin(theta), 0.0 }; 
}

std::vector<double> UniformInUnitSphere( void )
{
	// reference: https://doi.org/10.1063/1.168311, adapting equation 13

	static double two_pi = 6.283185307179586; 

    double T = UniformRandom(); 
	double sqrt_T = sqrt(T); 
	double sqrt_one_minus_T = 1.0;
	sqrt_one_minus_T -= T; 
	sqrt_one_minus_T = sqrt( sqrt_one_minus_T ); 

	double param1 = pow( UniformRandom() , 0.33333333333333333333333333333333333333 );  //  xi^(1/3), 
    double param2 = param1; // xi^(1/3)
	param2 *= 2.0; // 2 * xi^(1/3)
	param2 *= sqrt_T; // 2 * xi(1) * T^(1/2)
	param2 *= sqrt_one_minus_T; //  2 * xi(1) * T^(1/2) * (1-T)^(1/2)
	
    double theta = UniformRandom(); // U(0,1)
	theta *= two_pi; // U(0,2*pi)
	
	return { param2*sin(theta) , param2*cos(theta), param1*(1-2*T) }; 
}

std::vector<double> UniformInAnnulus( double r1, double r2 )
{
	static double two_pi = 6.283185307179586; 

    double theta = UniformRandom(); 
	theta *= two_pi; 
	double r1_2 = r1*r1; 
	double r2_2 = r2*r2; 

    double r = sqrt( r1_2 + (r2_2-r1_2) * UniformRandom() ); 
    double x = r*cos(theta); 
    double y = r*sin(theta); 
    return {x,y,0.0}; 
}

std::vector<double> UniformInShell( double r1, double r2 )
{
	static double two_pi = 6.283185307179586; 

    double T = UniformRandom(); 
	double sqrt_T = sqrt(T); 
	double sqrt_one_minus_T = 1.0;
	sqrt_one_minus_T -= T; 
	sqrt_one_minus_T = sqrt( sqrt_one_minus_T ); 

	double param1 = pow( UniformRandom() , 0.33333333333333333333333333333333333333 );  //  xi^(1/3), 
	// param1 *= (r2-r1); 
	// param1 += r1; 
    double param2 = param1; // xi^(1/3)
	param2 *= 2.0; // 2 * xi^(1/3)
	param2 *= sqrt_T; // 2 * xi(1) * T^(1/2)
	param2 *= sqrt_one_minus_T; //  2 * xi(1) * T^(1/2) * (1-T)^(1/2)
	
    double theta = UniformRandom(); // U(0,1)
	theta *= two_pi; // U(0,2*pi)
	
	return { param2*sin(theta) , param2*cos(theta), param1*(1-2*T) }; 
}

void setup_cell_rules( void )
{
	// setup 
	intialize_hypothesis_rulesets(); 

	// load rules 
	parse_rules_from_pugixml(); 

	// display rules to screen
	display_hypothesis_rulesets( std::cout );

	// save annotations 
	save_annotated_detailed_English_rules(); 
	save_annotated_detailed_English_rules_HTML(); 
	save_annotated_English_rules(); 
	save_annotated_English_rules_HTML(); 

	// save dictionaries 
	std::string dictionary_file = "./" + PhysiCell_settings.folder + "/dictionaries.txt";
	std::ofstream dict_of( dictionary_file , std::ios::out ); 

	display_signal_dictionary( dict_of ); // done 
	display_behavior_dictionary( dict_of ); // done 
	dict_of.close(); 

	// save rules (v1)
	std::string rules_file = PhysiCell_settings.folder + "/cell_rules.csv"; 
	export_rules_csv_v1( rules_file ); 


	return; 
}


};