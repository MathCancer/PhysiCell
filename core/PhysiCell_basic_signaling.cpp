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
 
#include "./PhysiCell_basic_signaling.h"

using namespace BioFVM; 

namespace PhysiCell{

/*
std::map<std::string,int> signal_to_int; 
std::map<std::string,int> response_to_int; 

std::map<int,std::string> int_to_signal; 
std::map<int,std::string> int_to_response; 

std::vector<Signal_Response*> SR_models; 
std::map<std::string, Signal_Response*> celldef_to_SR; 
std::map<int,Signal_Response*> celldef_int_to_SR; 

void setup_signal_response( void )
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 
	extern std::unordered_map<int,int> cell_definition_indices_by_type; 
	extern std::unordered_map<int,Cell_Definition*> cell_definitions_by_type; 
	
	// set key parameters on number of signals, etc. 
	// make registry of signals 
	// make registry of responses 

	static bool setup_done = false; 
	if( setup_done == true )
	{ return; }
	setup_done = true; 

	int m = microenvironment.number_of_densities(); 
	int n = cell_definition_indices_by_name.size(); 

	signal_to_int.clear(); 
	int_to_signal.clear(); 

	// construct signals 

	// substrate densities 
	for( int i=0; i < m ; i++ )
	{
		std::string name = microenvironment.density_names[i]; 
		signal_to_int[ name ] = i;
		int_to_signal[i] = name; 
	}
	
	// mechanical pressure 
	int map_index = m; 
	signal_to_int[ "pressure"] = map_index; 
	int_to_signal[map_index] = "pressure"; 
	
	// contact with each cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "contact with " + pCD->name; 
		signal_to_int[temp] = map_index; 
		// synonym 
		std::string temp1 = "contact with cell type " + std::to_string( pCD->type ); 
		signal_to_int[temp1] = map_index; 
		int_to_signal[map_index] = temp; 
	}
	
	// contact with (any) live cell 
	map_index++; 
	signal_to_int["contact with live cells"] = map_index; 
	int_to_signal[map_index] = "contact with live cell"; 
	
	// contact with dead cell 
	map_index++; 
	signal_to_int["contact with dead cells"] = map_index; 
	int_to_signal[map_index] = "contact with dead cell"; 
	
	// contact iwth basement membrane 
	map_index++; 
	signal_to_int["contact with basement membrane"] = map_index; 
	int_to_signal[map_index] = "contact with basement membrane"; 
	// synonym
	signal_to_int["contact with BM"] = map_index; 
	
	// damage state 

	map_index++; 
	signal_to_int["damage"] = map_index; 
	int_to_signal[map_index] = "damage"; 
	
	// total attack time 
	map_index++; 
	signal_to_int["total attack time"] = map_index; 
	int_to_signal[map_index] = "total attack time"; 

	response_to_int.clear(); 	
	int_to_response.clear(); 

	// construct responses 
	std::string name;
	std::string map_name;

	for( int i=0; i < m ; i++ )
	{
		map_index = i; 
		name = microenvironment.density_names[i]; 
		map_name = name + " " + "secretion";

		// secretion rate 
		response_to_int[ map_name ] = map_index;
		int_to_response[map_index] = map_name; 

		// secretion target 
		map_index = m+i; 
		map_name = name + " " + "secretion_target"; 
		response_to_int[ map_name ] = map_index;
		int_to_response[map_index] = map_name; 

		// uptake rate 
		map_index = 2*m+i; 
		map_name = name + " " + "uptake"; 
		response_to_int[ map_name ] = map_index;
		int_to_response[map_index] = map_name; 

		// net export rate 
		map_index = 3*m+i; 
		map_name = name + " " + "export"; 
		response_to_int[ map_name ] = map_index;
		int_to_response[map_index] = map_name; 
	}
	
	map_index = 4*m; 
	map_name = "cycle entry";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 

	map_index++; 
	map_name = "apoptosis";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 

	map_index++; 
	map_name = "necrosis";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 

	map_index++; 
	map_name = "migration speed";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 

	map_index++; 
	map_name = "migration bias";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 
	
	// chemotactic sensitivities 
	for( int i=0; i < m ; i++ )
	{
		map_index++; 
		std::string name = "chemotactic response to " + microenvironment.density_names[i]; 
		response_to_int[ name ] = map_index;
		int_to_response[map_index] = name; 
		// synonym 
		name = "chemotactic sensitivity to " + microenvironment.density_names[i]; 
		response_to_int[ name ] = map_index;
	}
	
	// cell-cell adhesion 
	map_index++; 
	map_name = "cell-cell adhesion";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 
	
	// soon: cell-type specific adhesion 

	// cell-cell repulsion 
	map_index++; 
	map_name = "cell-cell repulsion";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 

	// cell-BM adhesion 
	map_index++; 
	map_name = "cell-BM adhesion";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 
	response_to_int["cell-membrane adhesion"] = map_index; 
	
	// cell-BM repulsion 
	map_index++; 
	map_name = "cell-BM repulsion";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 
	response_to_int["cell-membrane repulsion"] = map_index; 


	map_index++; 
	map_name = "phagocytosis of dead cell";
	response_to_int[ map_name ] = map_index;
	int_to_response[map_index] = map_name; 
	
	// phagocytosis of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "phagocytosis of " + pCD->name; 
		response_to_int[temp] = map_index; 
		int_to_response[map_index] = temp; 
	}

	// attack of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "attack " + pCD->name; 
		response_to_int[temp] = map_index; 
		int_to_response[map_index] = temp; 
		// synonym 
		temp = "attack cell type " + std::to_string(pCD->type); 
		response_to_int[temp] = map_index; 
	}

	// fusion 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "fuse to " + pCD->name; 
		response_to_int[temp] = map_index; 
		int_to_response[map_index] = temp; 
		// synonym 
		temp = "fuse to cell type " + std::to_string(pCD->type); 
		response_to_int[temp] = map_index; 
	}	
	
	// transformation 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "transform to " + pCD->name; 
		response_to_int[temp] = map_index; 
		int_to_response[map_index] = temp; 
		// synonym 
		temp = "transform to cell type " + std::to_string(pCD->type); 
		response_to_int[temp] = map_index; 
	}	
	

	std::cout << "Signals: " << std::endl 
			  << "=======" << std::endl; 
	for( int i=0; i < int_to_signal.size() ; i++ )
	{ std::cout << i << " : " << int_to_signal[i] << std::endl; }
	std::cout << std::endl << std::endl;  
	
	std::cout << "Signals (with synonyms): " << std::endl 
			  << "=======================" << std::endl; 
	for( auto it = signal_to_int.begin() ; it != signal_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	

	std::cout << "Responses: " << std::endl 
			  << "=========" << std::endl; 
	for( int i=0; i < int_to_response.size() ; i++ )
	{ std::cout << i << " : " << int_to_response[i] << std::endl; }
	std::cout << std::endl; 

	std::cout << "Responses (with synonyms): " << std::endl 
			  << "=========================" << std::endl; 
	for( auto it = response_to_int.begin() ; it != response_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	


	// now create empty SR models for each cell definition 

	for( int i=0 ; i < cell_definitions_by_index.size() ; i++ )
	{ create_SR_model( *cell_definitions_by_index[i] ); }

	return;
}
	
Signal_Response::Signal_Response()
{
	extern std::unordered_map<std::string,int> cell_definition_indices_by_name; 

	setup_signal_response(); 

	pCD = NULL; 
	cell_type = "none";
	SR_models.push_back( this ); 

	// find the microenvironment, get number of substrates 
	number_of_substrates = microenvironment.number_of_densities(); 
	number_of_cell_types = cell_definition_indices_by_name.size(); 
	
	number_of_signals = int_to_signal.size(); 
	number_of_responses = int_to_response.size(); 
	
	// allocate main vectors 

	scales.resize( number_of_signals , 1.0 );  
	std::vector<double> weight_template( number_of_signals , 0.0);

	// allocate space for weights

	up_response_weights.resize( number_of_responses , weight_template  ); 
	down_response_weights.resize( number_of_responses , weight_template  );  
	
	// allocate space for response parameters 

	hill_powers.resize( number_of_responses , 1.0 ); 
	up_half_maxes.resize( number_of_responses , 0.1 ); 
	down_half_maxes.resize( number_of_responses , 0.1 );
	
	base_parameters.resize( number_of_responses , 1.0 );
	max_up_responses.resize( number_of_responses , 10.0 );
	max_down_responses.resize( number_of_responses , 0.1 );
};

void Signal_Response::add_rule( std::string signal, std::string direction , std::string response, double max_response )
{
	std::cout << "adding rule: " << std::endl; 
	int nS = signal_to_int[signal];
	int nR = response_to_int[response]; 

	std::cout << signal << " [index " << nS << "] "	
	<< direction << "es " 
	<< response << " [index " << nR << "]" << std::endl; 

	// up or down 
	if( direction[0] == 'i' || direction[0] == 'p' )
	{
		// add a non-zero weight where it belongs 
		up_response_weights[nR][nS] = 1; 

		// add the value of the max response, 
		max_up_responses[nR] = max_response; 
	}
	else
	{
		// add a non-zero weight where it belongs 
		down_response_weights[nR][nS] = 1; 

		// add the value of the max response,   
		max_down_responses[nR] = max_response; 
	}

	return; 
}

void Signal_Response::add_rule( std::string signal, std::string direction , std::string response )
{
	std::cout << "adding rule: " << std::endl; 
	int nS = signal_to_int[signal];
	int nR = response_to_int[response]; 

	std::cout << signal << " [index " << nS << "] "	
	<< direction << "es " 
	<< response << " [index " << nR << "]" << std::endl; 

	// up or down 
	if( direction[0] == 'i' || direction[0] == 'p' )
	{
		// add a non-zero weight where it belongs 
		up_response_weights[nR][nS] = 1; 
	}
	else
	{
		// add a non-zero weight where it belongs 
		down_response_weights[nR][nS] = 1; 
	}

	return; 
}

std::vector<double> Signal_Response::construct_signals( Cell* pCell )
{
 // construct signal vector, including use of scales 
	std::vector<double> signals( number_of_signals , 0.0 );

	// extracellular signals 
	for( int i=0; i < number_of_substrates; i++ )
	{
		signals[i] = (pCell->nearest_density_vector())[i] / scales[i]; 
	}
	int ind = number_of_substrates; 

	// pressure 
	signals[ind] = pCell->state.simple_pressure / scales[ind]; 
	
	// physical contact with cells (of each type) 
		// clear them out 
	for( int i=0; i < number_of_cell_types; i++ )
	{ signals[ind+i] = 0.0; } 
		// increment signals 
	int dead_cells = 0; 
	int live_cells = 0; 
	for( int i=0; i < pCell->state.neighbors.size(); i++ )
	{
		Cell* pC = pCell->state.neighbors[i]; 
		if( pC->phenotype.death.dead == true )
		{ dead_cells++; } 
		else
		{ live_cells++; } 
		int nCT = cell_definition_indices_by_type[pC->type]; 
		
		signals[number_of_substrates+1+nCT] += 1; 
	}
	
	// physical contact with live cells 
	signals[number_of_substrates+number_of_cell_types+1] = live_cells; 
	
	// physical contact with dead cells 
	signals[number_of_substrates+number_of_cell_types+2] = dead_cells; 
	
	// physical contact with basement membrane (not implemented) 
	signals[number_of_substrates+number_of_cell_types+3] = 
		(int) pCell->state.contact_with_basement_membrane; 

	// damage
	signals[number_of_substrates+number_of_cell_types+4] = 
		pCell->state.damage; 
	
	// integrated total attack time 
	signals[number_of_substrates+number_of_cell_types+5] = 
		pCell->state.total_attack_time; 

	return signals; 
}


void Signal_Response::sync_to_cell_definition( Cell_Definition& cd )
{
	pCD = &cd; 
	cell_type = cd.name; 

	celldef_to_SR[cell_type] = this; 
	celldef_int_to_SR[pCD->type] = this; 
	
	// sample the base parameters 

	// substrate-related parameters 
	int m = number_of_substrates; 
	int n = number_of_cell_types; 
	
	for( int i=0; i < m ; i++ )
	{
		// secretion rate 
		base_parameters[i] = cd.phenotype.secretion.secretion_rates[i]; 
		set_response_parameters_fold_changes(i,10,10); 
		// secretion target 
		base_parameters[m+i] = cd.phenotype.secretion.saturation_densities[i]; 
		set_response_parameters_fold_changes(m+i,10,10); 
		// uptake rate 
		base_parameters[2*m+i] = cd.phenotype.secretion.uptake_rates[i]; 
		set_response_parameters_fold_changes(2*m+i,10,10); 
		// net export rate 
		base_parameters[3*m+i] = cd.phenotype.secretion.net_export_rates[i]; 
		set_response_parameters_fold_changes(3*m+i,10,10); 
	}
	int nR = 4*m; 
	// cycle entry 
	base_parameters[nR] = cd.phenotype.cycle.data.exit_rate(0); 
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// apoptotic death
	base_parameters[nR] = cd.phenotype.death.rates[0]; 
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// necrotic death
	base_parameters[nR] = cd.phenotype.death.rates[1]; 
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// migration speed
	base_parameters[nR] = cd.phenotype.motility.migration_speed;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// migration bias 
	base_parameters[nR] = cd.phenotype.motility.migration_bias;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	
	// chemotactic sensitivities 
	for( int i=0; i < m ; i++ )
	{
		base_parameters[nR] = cd.phenotype.motility.chemotactic_sensitivities[i]; 
		nR++; 
	}
	
	// cell-cell adhesion
	base_parameters[nR] = cd.phenotype.mechanics.cell_cell_adhesion_strength;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// cell-cell repulsion 
	base_parameters[nR] = cd.phenotype.mechanics.cell_cell_repulsion_strength;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	

	// cell-BM adhesion
	base_parameters[nR] = cd.phenotype.mechanics.cell_BM_adhesion_strength;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	// cell-BM repulsion 
	base_parameters[nR] = cd.phenotype.mechanics.cell_BM_repulsion_strength;
	set_response_parameters_fold_changes(nR,10,10); 
	nR++; 
	
	// phagocytosis of dead cell
	base_parameters[nR] = cd.phenotype.cell_interactions.dead_phagocytosis_rate; 
	nR++; 
	
	// phagocytosis of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		base_parameters[nR] = cd.phenotype.cell_interactions.live_phagocytosis_rates[i]; 
		nR++; 
	}

	// attack of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		base_parameters[nR] = cd.phenotype.cell_interactions.attack_rates[i]; 
		nR++; 
	}

	// fusion 
	for( int i=0; i < n ; i++ )
	{
		base_parameters[nR] = cd.phenotype.cell_interactions.fusion_rates[i]; 
		nR++; 
	}

	// transformation 
	for( int i=0; i < n ; i++ )
	{
		base_parameters[nR] = cd.phenotype.cell_transformations.transformation_rates[i]; 
		nR++; 
	}	

	return; 
}

std::vector<double> Signal_Response::run_rules( std::vector<double>& signals )
{
	std::vector<double> parameters( number_of_responses , 0.0 ); 
	for( int i=0; i < number_of_responses ; i++ )
	{
		// calculate U and D
		double U = 0.0; 
		double D = 0.0; 
		for( int j=0; j<number_of_signals; j++ )
		{
			U += ( signals[j]*up_response_weights[i][j] ); 
			D += ( signals[j]*down_response_weights[i][j] ); 
		}

		// get the right interp value (like an efect model)
		double E = 0.0; 
		double max_response = max_up_responses[i]; 
		if( U >= D )
		{ E = Hill_response_function(U-D,up_half_maxes[i],hill_powers[i]); }
		else
		{ 
			E = Hill_response_function(D-U,down_half_maxes[i],hill_powers[i]);
			max_response = max_down_responses[i];
		}

		// interpolate 
		parameters[i] = max_response; 
		parameters[i] -= base_parameters[i];
		parameters[i] *= E; 
		parameters[i] += base_parameters[i]; 
	}

//	std::cout << "signals: " << signals << std::endl
//	<< "parameters: " << parameters << std::endl; 

	return parameters;
}

void Signal_Response::set_response_parameters( int nResponse , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power )
{
	hill_powers[nResponse] = hill_power;

	max_down_responses[nResponse] = max_down_response;
	down_half_maxes[nResponse] = down_halfmax;

	max_up_responses[nResponse] = max_up_response; 
	up_half_maxes[nResponse] = up_halfmax; 

	return; 	
}

void Signal_Response::set_response_parameters( std::string response , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power )
{
	int nResponse = response_to_int[response]; 
	return set_response_parameters( nResponse, max_down_response,max_up_response, down_halfmax,up_halfmax, hill_power );
}


void Signal_Response::set_response_parameters( int nResponse , double max_down_response , double max_up_response )
{
	max_down_responses[nResponse] = max_down_response;
	max_up_responses[nResponse] = max_up_response; 
	return; 
}
void Signal_Response::set_response_parameters( std::string response , double max_down_response , double max_up_response  )
{
	int nResponse = response_to_int[response]; 
	return set_response_parameters( nResponse, max_down_response,max_up_response );
}


void Signal_Response::set_response_parameters_fold_changes( int nResponse , double max_down_response , double max_up_response, 
	double down_halfmax, double up_halfmax, double hill_power )
{
	double max = base_parameters[nResponse] * max_up_response; 
	double min = base_parameters[nResponse] / max_down_response; 
	return set_response_parameters( nResponse, min,max, down_halfmax,up_halfmax, hill_power );
}


void Signal_Response::set_response_parameters_fold_changes( std::string response , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power )
{
	int nResponse = response_to_int[response]; 
	return set_response_parameters_fold_changes( nResponse, max_down_response,max_up_response, down_halfmax,up_halfmax, hill_power );
}

void Signal_Response::set_response_parameters_fold_changes( int nResponse , double max_down_response , double max_up_response )
{
	double max = base_parameters[nResponse] * max_up_response; 
	double min = base_parameters[nResponse] / max_down_response; 
	return set_response_parameters( nResponse, min,max );
}


void Signal_Response::set_response_parameters_fold_changes( std::string response , double max_down_response , double max_up_response )
{
	int nResponse = response_to_int[response]; 
	return set_response_parameters_fold_changes( nResponse, max_down_response,max_up_response );
}





void Signal_Response::transfer_updated_parameters( Cell* pCell , std::vector<double>& parameters )
{
	Phenotype& p = pCell->phenotype; 
	
	// substrate-related parameters 
	static int m = number_of_substrates; 
	static int n = number_of_cell_types; 
	int nR = 0; 
	
	for( int i=0; i < m ; i++ )
	{
		// secretion rate 
		p.secretion.secretion_rates[i] = parameters[i]; 
		nR++; 
		// secretion target 
		p.secretion.saturation_densities[i] = parameters[m+i]; 
		set_response_parameters_fold_changes(n+i,10,10); 
		// uptake rate 
		p.secretion.uptake_rates[i] = parameters[2*m+i]; 
		// net export rate 
		p.secretion.net_export_rates[i] = parameters[3*m+i]; 
	}
	nR = 4*m; 
	// cycle entry 
	p.cycle.data.exit_rate(0) = parameters[nR];  
	nR++; 
	// apoptotic death
	p.death.rates[0] = parameters[nR]; 
	nR++; 
	// necrotic death
	p.death.rates[1] = parameters[nR]; 
	nR++; 
	// migration speed
	p.motility.migration_speed = parameters[nR]; 
	nR++; 
	// migration bias 
	p.motility.migration_bias = parameters[nR]; 
	nR++; 
	
	// chemotactic sensitivities 
	for( int i=0; i < m ; i++ )
	{
		p.motility.chemotactic_sensitivities[i] = parameters[nR];  
		nR++; 
	}
	
	// cell-cell adhesion
	p.mechanics.cell_cell_adhesion_strength = parameters[nR]; 
	nR++; 
	// cell-cell repulsion 
	p.mechanics.cell_cell_repulsion_strength = parameters[nR]; 
	nR++; 

	// cell-BM adhesion
	p.mechanics.cell_BM_adhesion_strength = parameters[nR]; 
	nR++; 
	// cell-BM repulsion 
	p.mechanics.cell_BM_repulsion_strength = parameters[nR]; 
	nR++; 
	
	// phagocytosis of dead cell
	p.cell_interactions.dead_phagocytosis_rate = parameters[nR]; 
	nR++; 
	
	// phagocytosis of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		p.cell_interactions.live_phagocytosis_rates[i] = parameters[nR]; 
		nR++; 
	}

	// attack of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		p.cell_interactions.attack_rates[i] = parameters[nR]; 
		nR++; 
	}

	// fusion 
	for( int i=0; i < n ; i++ )
	{
		p.cell_interactions.fusion_rates[i] = parameters[nR]; 
		nR++; 
	}

	// transformation 
	for( int i=0; i < n ; i++ )
	{
		p.cell_transformations.transformation_rates[i] = parameters[nR]; 
		nR++; 
	}	

	return; 
}


void cell_setup_signal_response( Cell* pCell )
{
	// we will overwrite teh cusotm rules with this function. 
	// might be stupid risky 


}

void signal_response_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// problem: how does cell "know" its SR model? 
	// soltuion a global registry of SR models, indexed by cell type 

	Signal_Response* pSR = find_SR_model(pCell);

	// construct singnals 

	std::vector<double> signals = pSR->construct_signals(pCell); 

	// run rules 

 	std::vector<double> parameters = pSR->run_rules(signals);  

	// write to phenotype parameters 

	pSR->transfer_updated_parameters(pCell,parameters);

	return; 
}

void signal_response_custom( Cell* pCell, Phenotype& phenotype, double dt )
{
	// construct singnals 

	// run rules 

	// write to phenotype parameters 


}

void Signal_Response::display( void )
{
	std::cout << "Model assigned to cells of type " << cell_type << " ::" << std::endl; 
	for( int nR = 0 ; nR < number_of_responses; nR++ )
	{
		// current rule for nR:  
		std::string response = int_to_response[nR]; 

		std::cout << std::endl << "current rule for the " << response << " response: " << std::endl 
				<< "================================================" << std::endl; 
		std::cout << "U = 0";
		for( int i = 0; i < up_response_weights[nR].size(); i++ )
		{
			if( up_response_weights[nR][i] > 1e-10 )
			{ std::cout << " + " << up_response_weights[nR][i] << "*" << int_to_signal[i] 
				<< "/" << scales[i]	<< " "; }
		}
		std::cout << std::endl; 
		std::cout << "D = 0";
		for( int i = 0; i < down_response_weights[nR].size(); i++ )
		{
			if( down_response_weights[nR][i] > 1e-10 )
			{ std::cout << " + " << down_response_weights[nR][i] << "*" << int_to_signal[i] 
			<< "/" << scales[i] << " "; }
		}
		std::cout << std::endl; 

		std::cout << "p0[nR]: " << base_parameters[nR] << std::endl; 
		std::cout << "up weights[nR]: " << up_response_weights[nR] << std::endl; 
		std::cout << "pU[nR]: " << max_up_responses[nR] << std::endl; 
		std::cout << "down weights[nR]: " << down_response_weights[nR] << std::endl; 
		std::cout << "pD[nR]: " << max_down_responses[nR] << std::endl; 

		std::cout << "math: p(U) = " << base_parameters[nR] 
			<< " + " << max_up_responses[nR]-base_parameters[nR] << " Hill(U-D)" 
			<< " + " << max_down_responses[nR]-base_parameters[nR] << " Hill(D-U)" << std::endl << std::endl; 
	}
	return; 
}

Signal_Response* find_SR_model( std::string cell_def_name )
{ return celldef_to_SR[cell_def_name]; }

Signal_Response* find_SR_model( int cell_def_int )
{ return celldef_int_to_SR[cell_def_int]; }

Signal_Response* find_SR_model( Cell* pCell )
{ return celldef_int_to_SR[pCell->type]; }

void create_SR_model( Cell_Definition& cd )
{
	// declare a new SR model 
	Signal_Response* pSR; 
	pSR = new Signal_Response;

	// sync it to the cell def 

	pSR->sync_to_cell_definition( cd ); 
	
	return; 
}

*/


Integrated_Signal::Integrated_Signal()
{
	base_activity = 0.0; 
	max_activity = 1.0; 
	
	promoters.clear(); 
	promoter_weights.clear(); 
	
	promoters_half_max = 0.1;
	promoters_Hill = 4; 
	
	inhibitors.clear(); 
	inhibitor_weights.clear(); 
	
	inhibitors_half_max = 0.1; 
	inhibitors_Hill = 4; 
	
	return; 
}

void Integrated_Signal::reset( void )
{
	promoters.clear(); 
	promoter_weights.clear(); 

	inhibitors.clear(); 
	inhibitor_weights.clear(); 
	return; 
}

double Integrated_Signal::compute_signal( void )
{
	double pr = 0.0; 
	double w = 0.0; 
	for( int k=0 ; k < promoters.size() ; k++ )
	{ pr += promoters[k]; w += promoter_weights[k]; } 
	w += 1e-16; 
	pr /= w; 
	
	double inhib = 0.0; 
	w = 0.0; 
	for( int k=0 ; k < inhibitors.size() ; k++ )
	{ inhib += inhibitors[k]; w += inhibitor_weights[k]; } 
	w += 1e-16; 
	inhib /= w; 
	
	double Pn = pow( pr , promoters_Hill ); 
	double Phalf = pow( promoters_half_max , promoters_Hill ); 

	double In = pow( inhib , inhibitors_Hill ); 
	double Ihalf = pow( inhibitors_half_max , inhibitors_Hill ); 
	
	double P = Pn / ( Pn + Phalf ); 
	double I = 1.0 / ( In + Ihalf ); 
	
	double output = max_activity; 
	output -= base_activity; //(max-base)
	output *= P; // (max-base)*P 
	output += base_activity; // base + (max-base)*P 
	output *= I; // (base + (max-base)*P)*I; 

	return output; 
};

void Integrated_Signal::add_signal( char signal_type , double signal , double weight )
{
	if( signal_type == 'P' || signal_type == 'p' )
	{
		promoters.push_back( signal ); 
		promoter_weights.push_back( weight ); 
		return; 
	}
	if( signal_type == 'I' || signal_type == 'i' )
	{
		inhibitors.push_back( signal ); 
		inhibitor_weights.push_back( weight ); 
		return; 
	}
	return; 
}

void Integrated_Signal::add_signal( char signal_type , double signal )
{ return add_signal( signal_type , signal , 1.0 ); }

double Hill_response_function( double s, double half_max , double hill_power )
{ 
	if( s <= 0.0 )
	{ return 0.0; } 
	double output = pow( s , hill_power ); // s^h 
	double temp1 = pow( half_max , hill_power ); // s_half^h 
	temp1 += output; // s^h + s_half^h 
	output /= temp1; // s^h / (s^h + s_half^h ); 
	return output;
}

double linear_response_function( double s, double s_min , double s_max )
{
	if( s <= s_min )
	{ return 0.0; } 
	if( s >= s_max )
	{ return 1.0; } 
	s -= s_min; // overwrite s with s - s_min 
	s_max -= s_min; // overwrite s_max with s_max - s_min 
	s /= s_max; // now we have (s-s_min)/(s_max-s_min
	return s; 
}

double decreasing_linear_response_function( double s, double s_min , double s_max )
{
	if( s <= s_min )
	{ return 1.0; } 
	if( s >= s_max )
	{ return 0.0; } 
	// (smax-s)/(smax-smin); 
	// = -(s-smax)/(smax-smin)
	s -= s_max; // replace s by s-s_max 
	s_max -= s_min; // replace s_max = s_max - s_min 
	s /= s_max; // this is (s-s_max)/(s_max-s_min)
	s *= -1; // this is (s_max-s)/(s_max-s_min)
	return s; 
}


};
