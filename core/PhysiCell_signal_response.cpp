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
# Copyright (c) 2015-2022, Paul Macklin and the PhysiCell Project             #
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
 
#include "./PhysiCell_signal_response.h"

using namespace BioFVM; 

namespace PhysiCell{

std::vector<double> signal_scales; 

std::map<std::string,int> signal_to_int; 
std::map<std::string,int> response_to_int; 

std::map<int,std::string> int_to_signal; 
std::map<int,std::string> int_to_response; 

void setup_signal_response_dictionaries( void )
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

    // internalized substrates 
    int map_index = m; 
    for( int i=0; i < m ; i++ )
	{
		std::string name = "intracellular " + microenvironment.density_names[i]; 
		signal_to_int[ name ] = m+i;
		int_to_signal[m+i] = name; 

		// synonym 
		name = "internalized " + microenvironment.density_names[i]; 
		signal_to_int[ name ] = m+i; 
	}

    // substrate gradients 
    map_index = 2*m; 
	for( int i=0; i < m ; i++ )
	{
		std::string name = microenvironment.density_names[i] + " gradient"; 
		signal_to_int[ name ] = map_index;
		int_to_signal[map_index] = name; 

        // synonym
        name = "grad(" + microenvironment.density_names[i] +")"; 
		signal_to_int[ name ] = map_index;

        // synonym 
        name = "gradient of " + microenvironment.density_names[i]; 
		signal_to_int[ name ] = map_index;

        map_index++; 
	}    
	
	// mechanical pressure 
	// int map_index = m; 
	signal_to_int[ "pressure"] = map_index; 
	int_to_signal[map_index] = "pressure"; 

	// total volume 

	map_index++; 
	signal_to_int[ "volume"] = map_index; 
	int_to_signal[map_index] = "volume"; 

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
	signal_to_int["contact with live cell"] = map_index; 
	int_to_signal[map_index] = "contact with live cell"; 
	
	// contact with dead cell 
	map_index++; 
	signal_to_int["contact with dead cell"] = map_index; 
	int_to_signal[map_index] = "contact with dead cell"; 
	
	// contact with basement membrane 
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
	
    // cell adhesion affinities 
	// cell-type specific adhesion 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "adhesive affinity to " + pCD->name; 
		response_to_int[temp] = map_index; 
		int_to_response[map_index] = temp; 

		// synonym 
		temp = "adhesive affinity to cell type " + std::to_string(pCD->type); 
		response_to_int[temp] = map_index; 
	}

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

		// synonym 
		temp = "phagocytose cell type " + std::to_string(pCD->type); 
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

    // resize scales; 
    signal_scales.resize( int_to_signal.size() , 1.0 ); 

    display_signal_dictionary(); 
    display_response_dictionary(); 
/*
	// now create empty SR models for each cell definition 

	for( int i=0 ; i < cell_definitions_by_index.size() ; i++ )
	{ create_SR_model( *cell_definitions_by_index[i] ); }
*/    

	return;
}

double& signal_scale( std::string signal_name )
{ return signal_scales[ find_signal_index(signal_name) ]; }

double& signal_scale( int signal_index  )
{ return signal_scales[signal_index]; }


void display_signal_dictionary( void )
{
	std::cout << "Signals: " << std::endl 
			  << "=======" << std::endl; 
	for( int i=0; i < int_to_signal.size() ; i++ )
	{ std::cout << i << " : " << int_to_signal[i] << std::endl; }
	std::cout << std::endl << std::endl;  
	
    return; 
}

void display_signal_dictionary_with_synonyms( void )
{
	std::cout << "Signals (with synonyms): " << std::endl 
			  << "=======================" << std::endl; 
	for( auto it = signal_to_int.begin() ; it != signal_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	
    return; 
}

void display_response_dictionary( void )
{
	std::cout << "Responses: " << std::endl 
			  << "=========" << std::endl; 
	for( int i=0; i < int_to_response.size() ; i++ )
	{ std::cout << i << " : " << int_to_response[i] << std::endl; }
	std::cout << std::endl; 

    return; 
}

void display_response_dictionary_with_synonyms( void )
{
	std::cout << "Responses (with synonyms): " << std::endl 
			  << "=========================" << std::endl; 
	for( auto it = response_to_int.begin() ; it != response_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	
    return; 
}


int find_signal_index( std::string signal_name )
{
	auto search = signal_to_int.find( signal_name );
	// safety first! 
	if( search != signal_to_int.end() )
    { return search->second; }   
    return -1; 
}

std::string signal_name( int i )
{
	if( i >= 0 && i < int_to_signal.size() )
	{ return int_to_signal[i]; }
	
	return "not found"; 
}

int find_parameter_index( std::string response_name )
{
	auto search = response_to_int.find( response_name );
	// safety first! 
	if( search != response_to_int.end() )
    { return search->second; }   
    return -1; 
}

int find_behavior_index( std::string response_name )
{ return find_parameter_index(response_name); }

// create a full signal vector 
std::vector<double> construct_signals( Cell* pCell )
{
     // hard-coded 

	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	// construct signals 
    std::vector<double> signals( int_to_signal.size() , 0.0 ); 

	// substrate densities 
    // copy efficiently; 
    std::copy( pCell->nearest_density_vector().begin() , pCell->nearest_density_vector().end(), signals.begin() ); 
	// for( int i=0; i < m ; i++ )
	// { signals[i] /= signal_scales[i]; }


    // internalized substrates 
    int ind = m; 
    std::copy( pCell->phenotype.molecular.internalized_total_substrates.begin() , pCell->phenotype.molecular.internalized_total_substrates.end(), signals.begin()+m ); 
	for( int i=0; i < m ; i++ )
	{ signals[i+m] /= pCell->phenotype.volume.total; }

    // substrate gradients 
    ind = 2*m; // int ind = m; 
	for( int i=0; i < m ; i++ )
	{
        signals[ind] = norm( pCell->nearest_gradient(i) ); 
        // signals[ind] /= signal_scales[ind]; 
        ind++; 
	}    

	// mechanical pressure 
	signals[ind] = pCell->state.simple_pressure;
    // signals[ind] / signal_scales[ind]; 
    ind++; 

	// cell volume 
	signals[ind] = pCell->phenotype.volume.total; 
    // signals[ind] / signal_scales[ind]; 
    ind++; 


	// physical contact with cells (of each type) 
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
		signals[ind+nCT] += 1; 
	}
    // rescale 
    // for( int nCT=0; nCT < n ; nCT++ )
    // { signals[ind+nCT] /= signal_scales[ind+nCT]; }

    ind += n; 
	// physical contact with live cells 
	signals[ind] = live_cells; 
    // signals[ind] /= signal_scales[ind]; 
    ind++; 
	
	// physical contact with dead cells 
	signals[ind] = dead_cells; 
    // signals[ind] /= signal_scales[ind]; 
    ind++; 

	// physical contact with basement membrane (not implemented) 
	signals[ind] = (int) pCell->state.contact_with_basement_membrane; 
    ind++; 

	// damage
	signals[ind] = pCell->state.damage; 
    ind++; 
	
	// integrated total attack time 
	signals[ind] = pCell->state.total_attack_time;     
    ind++; 

    // rescale 
    signals /= signal_scales; 
	
    return signals; 
}

// create a signal vector of only the cell contacts 
std::vector<double> construct_cell_contact_signals( Cell* pCell )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	std::vector<double> output( n+2 , 0.0 ); 
	// process all neighbors 
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
		output[nCT] += 1; 
	}
	output[n] = live_cells; 
	output[n+1] = dead_cells; 

	// rescale 
	std::string search_for = "contact with " + cell_definitions_by_type[0]->name; 
	static int scaling_start_index = find_signal_index( search_for ); 
	for( int i=0; i < n+2 ; i++ )
	{ output[i] /= signal_scales[scaling_start_index+i]; }

	return output; 
}

std::vector<double> construct_selected_signals( Cell* pCell , std::vector<int> signal_indices )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 	

	// contact signals start here 
	static int contact_start_index = find_signal_index( "contact with " + cell_definitions_by_type[0]->name ); 

	// typically more efficient to get these first 
	std::vector<double> contact_signals = construct_cell_contact_signals( pCell ); 
	bool created_contact_signals = false; 

	std::vector<double> signals( signal_indices.size() , 0.0 ); 
	for( int i=0; i < signal_indices.size() ; i++ )
	{
		int ind = signal_indices[i]; 
		if( ind >= contact_start_index && ind < contact_start_index+n+2)
		{ signals[i] = contact_signals[ind-contact_start_index]; }
		else
		{ signals[i] = single_signal( pCell , ind ); }
	}

	return signals; 
}

std::vector<double> construct_selected_signals( Cell* pCell , std::vector<std::string> signal_names )
{
	std::vector<int> signal_indices( signal_names.size() , 0 ); 
	for( int i=0; i < signal_names.size(); i++ )
	{ signal_indices[i] = find_signal_index( signal_names[i]); }

	return construct_selected_signals(pCell,signal_indices); 
}



double single_signal( Cell* pCell, int index )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	double out = 0.0; 

	if( index < 0 )
	{ 
		std::cout<< "Why would you ask for array[-1]? Why? WHY???? That's it, I quit." << std::endl; 
		exit(-1832443); 
		return -9e9; 
	}

	int ind = index; 
	// first m entries: extracellular concentration 
	if( 0 <= ind && ind < m )
	{
		out = pCell->nearest_density_vector()[ind];
		out /= signal_scales[index]; 
		return out; 
	}

	// second m entries: intracellular concentration 
	ind -= m; 
	if( ind < m )
	{
		out = pCell->phenotype.molecular.internalized_total_substrates[ind]; 
		out /= pCell->phenotype.volume.total;
		out /= signal_scales[index]; 
		return out; 
	}

	// next m entries: gradients 
	ind -= m; 
	if( ind < m )
	{
		out =  norm( pCell->nearest_gradient(ind) ); 
		out /= signal_scales[index]; 
		return out; 
	}

	ind -= m; 
	// mechanical pressure 
	if( ind == 0 )
	{
		out = pCell->state.simple_pressure;
		out /= signal_scales[index]; 
		return out; 
	}

	ind -= 1; 
	// cell volume 	
	if( ind == 0 )
	{
		out = pCell->phenotype.volume.total; 
		out /= signal_scales[index]; 
		return out; 
	}

	ind -=1; 
	// physical contact with cells (of each type) 
	// individual contact signals are a bit costly 
	if( ind < n+2 )
	{
		std::vector<int> counts( n , 0 ); 
		// process all neighbors 
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
			counts[nCT] += 1; 
		}

		if( ind < n )
		{
			out = counts[ind]; 
			out /= signal_scales[index]; 
			return out; 
		}
		if( ind == n )
		{
			out = live_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		// ind == n+1 
		out = dead_cells; 
		out /= signal_scales[index]; 
		return out; 
	}

	// contact with BM 
	ind -= (n+2); 
	if( ind == 0 )
	{ 
		out = (double) pCell->state.contact_with_basement_membrane; 
		out /= signal_scales[index]; 
		return out; 
	} 

	// damage
	ind -= 1; 
	if( ind == 0 )
	{
		out = pCell->state.damage; 
		out /= signal_scales[index]; 
		return out; 
	} 

	// integrated total attack time 
	ind -= 1; 
	if( ind == 0 )
	{
		out = pCell->state.total_attack_time;     
		out /= signal_scales[index]; 
		return out; 
	} 

	// unknown after here !

	std::cout << "Warning: Requested unknown signal number " << index << "!" << std::endl
		      << "         Returning 0.0, but you should fix this!" << std::endl << std::endl; 

	return 0.0; 
}

double single_signal( Cell* pCell, std::string name )
{ return single_signal( pCell, find_signal_index(name) ); }




};

