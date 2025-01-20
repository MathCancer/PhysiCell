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
 
#include "./PhysiCell_signal_behavior.h"

using namespace BioFVM; 

namespace PhysiCell{

std::vector<double> signal_scales; 

std::map<std::string,int> signal_to_int; 
std::map<std::string,int> behavior_to_int; 

std::map<int,std::string> int_to_signal; 
std::map<int,std::string> int_to_behavior; 

void setup_signal_behavior_dictionaries( void )
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
		// synonym 
	signal_to_int["contact with live cells"] = map_index; 
	
	// contact with (any) dead cell 
	map_index++; 
	signal_to_int["contact with dead cell"] = map_index; 
	int_to_signal[map_index] = "contact with dead cell"; 
		// synonym 
	signal_to_int["contact with dead cells"] = map_index; 	

	// contact with apoptotic cell 
	map_index++; 
	signal_to_int["contact with apoptotic cell"] = map_index; 
	int_to_signal[map_index] = "contact with apoptotic cell"; 
		// synonym 
	signal_to_int["contact with apoptotic cells"] = map_index; 	
	
	// contact with necrotic cell 
	map_index++; 
	signal_to_int["contact with necrotic cell"] = map_index; 
	int_to_signal[map_index] = "contact with necrotic cell"; 
		// synonym 
	signal_to_int["contact with necrotic cells"] = map_index; 	
		
	// contact with other dead cell 
	map_index++; 
	signal_to_int["contact with other dead cell"] = map_index; 
	int_to_signal[map_index] = "contact with other dead cell"; 
		// synonym 
	signal_to_int["contact with other dead cells"] = map_index; 	

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

	map_index++; 
	signal_to_int["damage delivered"] = map_index; 
	int_to_signal[map_index] = "damage delivered"; 
	// synonym 
	signal_to_int["total damage delivered"] = map_index; 	

	// attacking yes/no?  
	map_index++; 
	signal_to_int["attacking"] = map_index; 
	int_to_signal[map_index] = "attacking"; 
	// synonym 
	signal_to_int["is attacking"] = map_index; 

	// live / dead state 
	map_index++; 
	signal_to_int["dead"] = map_index; 
	int_to_signal[map_index] = "dead"; 
	// synonym 
	signal_to_int["is dead"] = map_index; 

	// total attack time 
	map_index++; 
	signal_to_int["total attack time"] = map_index; 
	int_to_signal[map_index] = "total attack time"; 

	// current time
	map_index++; 
	signal_to_int["time"] = map_index; 
	int_to_signal[map_index] = "time"; 
	// synonyms 
	signal_to_int["current time"] = map_index; 
	signal_to_int["global time"] = map_index; 

	// custom signals
	for( int nc=0 ; nc < cell_defaults.custom_data.variables.size() ; nc++ )
	{
		map_index++; 
		std::string custom_signal_name = "custom:" ; 
		custom_signal_name += cell_defaults.custom_data.variables[nc].name; 
		signal_to_int[custom_signal_name] = map_index; 
		int_to_signal[map_index] = custom_signal_name; 

		// synonyms  
		custom_signal_name = "custom: " ; 
		custom_signal_name += cell_defaults.custom_data.variables[nc].name; 
		signal_to_int[custom_signal_name] = map_index; 

		custom_signal_name = "custom " ; 
		custom_signal_name += std::to_string(nc); 
		signal_to_int[custom_signal_name] = map_index; 
	}

	map_index++; 
	signal_to_int["apoptotic"] = map_index; 
	int_to_signal[map_index] = "apoptotic"; 
	// synonyms 
	signal_to_int["is_apoptotic"] = map_index; 

	map_index++; 
	signal_to_int["necrotic"] = map_index; 
	int_to_signal[map_index] = "necrotic"; 
	// synonyms 
	signal_to_int["is_necrotic"] = map_index; 

/*
	// immunogenicity to each cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "immunogenicity to " + pCD->name; 
		signal_to_int[temp] = map_index; 
		int_to_signal[map_index] = temp; 		
				// synonyms 
		std::string temp1 = "immunogenicity to cell type " + std::to_string( pCD->type ); 
		signal_to_int[temp1] = map_index; 
	}
*/



	/* add new signals above this line */

	behavior_to_int.clear(); 	
	int_to_behavior.clear(); 

	// construct behaviors 
	std::string name;
	std::string map_name;

	for( int i=0; i < m ; i++ )
	{
		map_index = i; 
		name = microenvironment.density_names[i]; 
		map_name = name + " " + "secretion";

		// secretion rate 
		behavior_to_int[ map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 

		// secretion target 
		map_index = m+i; 
		map_name = name + " " + "secretion target"; 
		behavior_to_int[ map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 
			// synonym 
			map_name = name + " " + "secretion saturation density"; 
			behavior_to_int[ map_name ] = map_index;

		// uptake rate 
		map_index = 2*m+i; 
		map_name = name + " " + "uptake"; 
		behavior_to_int[ map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 

		// net export rate 
		map_index = 3*m+i; 
		map_name = name + " " + "export"; 
		behavior_to_int[ map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 
	}
	
	map_index = 4*m; 
	map_name = "cycle entry";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

		// synonym 
		map_name = "exit from cycle phase 0"; 
		behavior_to_int[ map_name ] = map_index;

	// other cyle phases 
	for( int i=1; i < 6; i++ )
	{
		map_index++; 
		map_name = "exit from cycle phase " + std::to_string(i);
		behavior_to_int[ map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 
	}

	map_index++; 
	map_name = "apoptosis";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "necrosis";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "migration speed";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "migration bias";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 
	
	map_index++; 
	map_name = "migration persistence time";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	// chemotactic sensitivities 
	for( int i=0; i < m ; i++ )
	{
		map_index++; 
		std::string name = "chemotactic response to " + microenvironment.density_names[i]; 
		behavior_to_int[ name ] = map_index;
		int_to_behavior[map_index] = name; 
		// synonym 
		name = "chemotactic sensitivity to " + microenvironment.density_names[i]; 
		behavior_to_int[ name ] = map_index;
	}
	
	// cell-cell adhesion 
	map_index++; 
	map_name = "cell-cell adhesion";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "cell-cell adhesion elastic constant";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

    // cell adhesion affinities 
	// cell-type specific adhesion 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "adhesive affinity to " + pCD->name; 
		behavior_to_int[temp] = map_index; 
		int_to_behavior[map_index] = temp; 

		// synonym 
		temp = "adhesive affinity to cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 
	}

	// max adhesion distance 
	map_index++; 
	map_name = "relative maximum adhesion distance";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 	

	// cell-cell repulsion 
	map_index++; 
	map_name = "cell-cell repulsion";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	// cell-BM adhesion 
	map_index++; 
	map_name = "cell-BM adhesion";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 
	behavior_to_int["cell-membrane adhesion"] = map_index; 
	
	// cell-BM repulsion 
	map_index++; 
	map_name = "cell-BM repulsion";
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 
	behavior_to_int["cell-membrane repulsion"] = map_index; 

	map_index++; 
	map_name = "phagocytose apoptotic cell"; 
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

		// synonym "phagocytosis of apoptotic cell";
		behavior_to_int[ "phagocytosis of apoptotic cell" ] = map_index; 
		behavior_to_int[ "phagocytosis of apoptotic cells" ] = map_index; 

	map_index++; 
	map_name = "phagocytose necrotic cell"; 
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

		// synonym "phagocytosis of necrotic cell";
		behavior_to_int[ "phagocytosis of necrotic cell" ] = map_index; 
		behavior_to_int[ "phagocytosis of necrotic cells" ] = map_index; 

	map_index++; 
	map_name = "phagocytose other dead cell"; 
	behavior_to_int[ map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

		// synonym "phagocytosis of other dead cell";
		behavior_to_int[ "phagocytosis of other dead cell" ] = map_index; 
		behavior_to_int[ "phagocytosis of other dead cells" ] = map_index; 
	
	// phagocytosis of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "phagocytose " + pCD->name; 
		behavior_to_int[temp] = map_index; 
		int_to_behavior[map_index] = temp; 

		// synonym 
		temp = "phagocytose cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 

		// synonym 
		temp = "phagocytosis of " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 
	}

	// attack of each live cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "attack " + pCD->name; 
		behavior_to_int[temp] = map_index; 
		int_to_behavior[map_index] = temp; 
		// synonym 
		temp = "attack cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 
	}

	// fusion 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string temp =  "fuse to " + pCD->name; 
		behavior_to_int[temp] = map_index; 
		int_to_behavior[map_index] = temp; 
		// synonym 
		temp = "fuse to cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 
	}	
	
	// transformation / transition 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		// std::string temp =  "transform to " + pCD->name; 
		std::string temp =  "transition to " + pCD->name; 
		behavior_to_int[temp] = map_index; 
		int_to_behavior[map_index] = temp; 


		// synonym 
		temp = "transform to cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 

		temp = "transform to " + pCD->name; 
		behavior_to_int[temp] = map_index; 

		temp = "transition to cell type " + std::to_string(pCD->type); 
		behavior_to_int[temp] = map_index; 


	}	

	// asymmetic division
	for( int i=0; i < n ; i++ )
	{
		map_index++;
		Cell_Definition* pCD = cell_definitions_by_type[i];
		std::string temp =  "asymmetric division to " + pCD->name;
		behavior_to_int[temp] = map_index;
		int_to_behavior[map_index] = temp;
	}

	// custom behaviors
	for( int nc=0 ; nc < cell_defaults.custom_data.variables.size() ; nc++ )
	{
		map_index++; 
		std::string custom_behavior_name = "custom:" ; 
		custom_behavior_name += cell_defaults.custom_data.variables[nc].name; 
		behavior_to_int[custom_behavior_name] = map_index; 
		int_to_behavior[map_index] = custom_behavior_name; 

		// synonyms  
		custom_behavior_name = "custom: " ; 
		custom_behavior_name += cell_defaults.custom_data.variables[nc].name; 
		behavior_to_int[custom_behavior_name] = map_index; 

		custom_behavior_name = "custom " ; 
		custom_behavior_name += std::to_string(nc); 
		behavior_to_int[custom_behavior_name] = map_index; 
	}

	map_index++; 
	map_name = "is_movable";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 
	// synonyms
	behavior_to_int["movable"] = map_index; 
	behavior_to_int["is movable"] = map_index; 

	// immunogenicity to each cell type 
	for( int i=0; i < n ; i++ )
	{
		map_index++; 
		Cell_Definition* pCD = cell_definitions_by_type[i]; 
		std::string map_name =  "immunogenicity to " + pCD->name; 
		behavior_to_int[map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 

		// synonyms 
		std::string temp1 = "immunogenicity to cell type " + std::to_string( pCD->type ); 
		behavior_to_int[temp1] = map_index; 
	}

	map_index++; 
	map_name = "cell attachment rate";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "cell detachment rate";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "maximum number of cell attachments";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "attack damage rate";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "attack duration";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "damage rate";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	map_index++; 
	map_name = "damage repair rate";
	behavior_to_int[map_name ] = map_index;
	int_to_behavior[map_index] = map_name; 

	/* add new behaviors above this line */

    // resize scales; 
    signal_scales.resize( int_to_signal.size() , 1.0 ); 

    display_signal_dictionary(); 
    display_behavior_dictionary(); 
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

void display_signal_dictionary( std::ostream& os )
{
	os << "Signals: " << std::endl 
	   << "=======" << std::endl; 
	for( int i=0; i < int_to_signal.size() ; i++ )
	{ os << i << " : " << int_to_signal[i] << std::endl; }
	os << std::endl;  
    return; 
}

void display_signal_dictionary( void )
{ display_signal_dictionary( std::cout); std::cout << std::endl; }


void display_signal_dictionary_with_synonyms( void )
{ display_signal_dictionary_with_synonyms( std::cout ); }
/*
	std::cout << "Signals (with synonyms): " << std::endl 
			  << "=======================" << std::endl; 
	for( auto it = signal_to_int.begin() ; it != signal_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	
    return; 
*/

void display_signal_dictionary_with_synonyms( std::ostream& os )
{
	os << "Signals (with synonyms): " << std::endl 
	   << "=======================" << std::endl; 
	for( auto it = signal_to_int.begin() ; it != signal_to_int.end() ; it++ )
	{ os << it->second << " : " << it->first << std::endl; }
	os << std::endl << std::endl;  	
    return; 
}

void display_behavior_dictionary( std::ostream& os )
{
	os << "Behaviors: " << std::endl 
	   << "=========" << std::endl; 
	for( int i=0; i < int_to_behavior.size() ; i++ )
	{ os << i << " : " << int_to_behavior[i] << std::endl; }
	os << std::endl; 
    return; 
}

void display_behavior_dictionary( void )
{
	display_behavior_dictionary( std::cout );
	std::cout << std::endl; 
	return; 
}

void display_behavior_dictionary_with_synonyms( std::ostream& os )
{
	os << "Behaviors (with synonyms): " << std::endl 
  	   << "=========================" << std::endl; 
	for( auto it = behavior_to_int.begin() ; it != behavior_to_int.end() ; it++ )
	{ os << it->second << " : " << it->first << std::endl; }
	os << std::endl << std::endl;  	
    return; 
}

void display_behavior_dictionary_with_synonyms( void )
{ display_behavior_dictionary_with_synonyms( std::cout ); return; }
/*
	std::cout << "Behaviors (with synonyms): " << std::endl 
			  << "=========================" << std::endl; 
	for( auto it = behavior_to_int.begin() ; it != behavior_to_int.end() ; it++ )
	{ std::cout << it->second << " : " << it->first << std::endl; }
	std::cout << std::endl << std::endl;  	
    return; 
*/	

int find_signal_index( std::string signal_name )
{
	auto search = signal_to_int.find( signal_name );
	// safety first! 
	if( search != signal_to_int.end() )
    { return search->second; }   

	std::cout << "having trouble finding " << signal_name << std::endl; 

    return -1; 
}

std::vector<int> find_signal_indices( std::vector<std::string> signal_names )
{
	std::vector<int> output( signal_names.size() , 0 ); 
	for( int n=0; n < signal_names.size(); n++ )
	{ output[n] = find_signal_index(signal_names[n]); }
	return output; 
}

std::string signal_name( int i )
{
	if( i >= 0 && i < int_to_signal.size() )
	{ return int_to_signal[i]; }	
	return "not found"; 
}

int find_parameter_index( std::string response_name )
{
	auto search = behavior_to_int.find( response_name );
	if( search != behavior_to_int.end() )
    { return search->second; }   
    return -1; 
}

int find_behavior_index( std::string response_name )
{ return find_parameter_index(response_name); }

std::vector<int> find_behavior_indices( std::vector<std::string> behavior_names )
{
	std::vector<int> output( behavior_names.size() , 0 ); 
	for( int n=0; n < behavior_names.size(); n++ )
	{ output[n] = find_behavior_index(behavior_names[n]); }
	return output; 
}

// create a full signal vector 
std::vector<double> get_signals( Cell* pCell )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	// construct signals 
    std::vector<double> signals( int_to_signal.size() , 0.0 ); 

	// substrate densities 
    // copy efficiently; 
	static int start_substrate_ind = find_signal_index( microenvironment.density_names[0] ); 
    std::copy( pCell->nearest_density_vector().begin() , 
			   pCell->nearest_density_vector().end(), 
			   signals.begin()+start_substrate_ind ); 

    // internalized substrates 
	static int start_int_substrate_ind = find_signal_index( "intracellular " + microenvironment.density_names[0] ); 
    std::copy( pCell->phenotype.molecular.internalized_total_substrates.begin() , 
	           pCell->phenotype.molecular.internalized_total_substrates.end(), 
			   signals.begin()+start_int_substrate_ind);  
	for( int i=0; i < m ; i++ )
	{ signals[i+start_int_substrate_ind] /= pCell->phenotype.volume.total; }

    // substrate gradients 
	static int start_substrate_grad_ind = find_signal_index( microenvironment.density_names[0] + " gradient"); 
	for( int i=0; i < m ; i++ )
	{ signals[start_substrate_grad_ind+i] = norm( pCell->nearest_gradient(i) ); }    

	// mechanical pressure 
	static int pressure_ind = find_signal_index( "pressure"); 
	signals[pressure_ind] = pCell->state.simple_pressure;

	// cell volume 
	static int volume_ind = find_signal_index( "volume"); 
	signals[volume_ind] = pCell->phenotype.volume.total; 

	// physical contact with cells (of each type) 
		// increment signals 
	int dead_cells = 0; 
	int apop_cells = 0;
	int necro_cells = 0; 
	int other_dead_cells = 0; 
	int live_cells = 0; 
	static int contact_ind = find_signal_index( "contact with " + cell_definitions_by_type[0]->name ); 
	for( int i=0; i < pCell->state.neighbors.size(); i++ )
	{
		Cell* pC = pCell->state.neighbors[i]; 
		if( pC->phenotype.death.dead == true )
		{
			dead_cells++; 
			if(pC->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )
			{ apop_cells++; }

			if( pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
				pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
				pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
			{ necro_cells++; }	
		} 
		else
		{ live_cells++; } 
		int nCT = cell_definition_indices_by_type[pC->type]; 
		signals[contact_ind+nCT] += 1; 
	}
	other_dead_cells = dead_cells - apop_cells - necro_cells; 

	// physical contact with live cells 
	static int live_contact_ind = find_signal_index( "contact with live cell"); 
	signals[live_contact_ind] = live_cells; 
	
	// physical contact with dead cells 
	static int dead_contact_ind = find_signal_index( "contact with dead cell"); 
	signals[dead_contact_ind] = dead_cells; 

	// physical contact with apoptotic cells 
	static int apop_contact_ind = find_signal_index( "contact with apoptotic cell"); 
	signals[apop_contact_ind] = apop_cells; 

	// physical contact with necrotic cells 
	static int necro_contact_ind = find_signal_index( "contact with necrotic cell"); 
	signals[necro_contact_ind] = necro_cells; 

	// physical contact with other dead cells 
	static int other_dead_contact_ind = find_signal_index( "contact with other dead cell"); 
	signals[other_dead_contact_ind] = other_dead_cells; 


	// physical contact with basement membrane (not implemented) 
	static int BM_contact_ind = find_signal_index( "contact with basement membrane"); 
	signals[BM_contact_ind] = (int) pCell->state.contact_with_basement_membrane; 

	// damage
	static int damage_ind = find_signal_index( "damage"); 
	signals[damage_ind] = pCell->phenotype.cell_integrity.damage; 

	// damage delivered
	static int damage_deliv_ind = find_signal_index( "damage delivered"); 
	signals[damage_deliv_ind] = pCell->phenotype.cell_interactions.total_damage_delivered; 

	// attacking? 
	static int attacking_ind = find_signal_index( "attacking"); 
	signals[attacking_ind] = 0; 
	if( pCell->phenotype.cell_interactions.pAttackTarget )
	{ signals[attacking_ind] = 1; }

	// live / dead state 
	static int dead_ind = find_signal_index( "dead" ); 
	signals[dead_ind] = (double) pCell->phenotype.death.dead; 
	
	// integrated total attack time 
	static int tot_attack_ind = find_signal_index( "total attack time"); 
	signals[tot_attack_ind] = pCell->state.total_attack_time;     

	// time 
	static int time_ind = find_signal_index( "time" ); 
	signals[time_ind] = PhysiCell_globals.current_time; 

	// custom signals 
	static int first_custom_ind = find_signal_index( "custom 0" ); 
	if( first_custom_ind > -1 )
	{
		for( int nc=0 ; nc < pCell->custom_data.variables.size() ; nc++ )
		{ signals[first_custom_ind+nc] = pCell->custom_data.variables[nc].value; }
	}

	static int apoptotic_ind = find_signal_index( "apoptotic" ); 
	if(pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )
	{ signals[apoptotic_ind] = 1; }
	else
	{ signals[apoptotic_ind] = 0; }

	static int necrotic_ind = find_signal_index( "necrotic" ); 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{ signals[necrotic_ind] = 1; }
	else
	{ signals[necrotic_ind] = 0; }

/*
	// vector of immunogenicity signals 
	static int start_immunogenicity_ind = find_signal_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
    std::copy( pCell->phenotype.cell_interactions.immunogenicities.begin() , 
	           pCell->phenotype.cell_interactions.immunogenicities.end(), 
			   signals.begin()+start_immunogenicity_ind);  
*/

    // rescale 
    signals /= signal_scales; 
	
    return signals; 
}

// create a signal vector of only the cell contacts 
std::vector<double> get_cell_contact_signals( Cell* pCell )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	std::vector<double> output( n+2+3 , 0.0 ); 
	// process all neighbors 
	int dead_cells = 0; 
	int live_cells = 0; 
    int apop_cells = 0;
    int necro_cells = 0; 
    int other_dead_cells = 0; 

	for( int i=0; i < pCell->state.neighbors.size(); i++ )
	{
		Cell* pC = pCell->state.neighbors[i]; 
		if( pC->phenotype.death.dead == true )
		{
			dead_cells++; 
            if(pC->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )
            { apop_cells++; }

            if( pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
                pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
                pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
            { necro_cells++; }  	
		} 
		else
		{ live_cells++; } 
		int nCT = cell_definition_indices_by_type[pC->type]; 
		output[nCT] += 1; 
	}
    other_dead_cells = dead_cells - apop_cells - necro_cells; 

	output[n] = live_cells; 
	output[n+1] = dead_cells; 

	output[n+2] = apop_cells; 
	output[n+3] = necro_cells; 
	output[n+4] = other_dead_cells; 

	// rescale 
	std::string search_for = "contact with " + cell_definitions_by_type[0]->name; 
	static int scaling_start_index = find_signal_index( search_for ); 
	for( int i=0; i < n+2 ; i++ )
	{ output[i] /= signal_scales[scaling_start_index+i]; }

	return output; 
}

std::vector<double> get_selected_signals( Cell* pCell , std::vector<int> indices )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 	

	// contact signals start here 
	static int contact_start_index = find_signal_index( "contact with " + cell_definitions_by_type[0]->name ); 

	// typically more efficient to get these first 
	std::vector<double> contact_signals = get_cell_contact_signals( pCell ); 
	bool created_contact_signals = false; 

	std::vector<double> signals( indices.size() , 0.0 ); 
	for( int i=0; i < indices.size() ; i++ )
	{
		int ind = indices[i]; 
		if( ind >= contact_start_index && ind < contact_start_index+n+2)
		{ signals[i] = contact_signals[ind-contact_start_index]; }
		else
		{ signals[i] = get_single_signal( pCell , ind ); }
	}

	return signals; 
}

std::vector<double> get_selected_signals( Cell* pCell , std::vector<std::string> names )
{
	std::vector<int> signal_indices( names.size() , 0 ); 
	for( int i=0; i < names.size(); i++ )
	{ signal_indices[i] = find_signal_index( names[i]); }

	return get_selected_signals(pCell,signal_indices); 
}

double get_single_signal( Cell* pCell, int index )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	double out = 0.0; 
	if( index < 0 )
	{ 
		std::cout<< "Why would you ask for array[-1]? Why? WHY???? That's it, I quit." << std::endl; 
		return -9e9; 
	}

	// first m entries: extracellular concentration 
	static int start_substrate_ind = find_signal_index( microenvironment.density_names[0] ); 
	if( start_substrate_ind <= index && index < start_substrate_ind + m )
	{
		out = pCell->nearest_density_vector()[index-start_substrate_ind];
		out /= signal_scales[index]; 
		return out; 
	}

	// second m entries: intracellular concentration 
	static int start_int_substrate_ind = find_signal_index( "intracellular " + microenvironment.density_names[0] ); 
	if( start_int_substrate_ind <= index && index < start_int_substrate_ind + m )
	{
		out = pCell->phenotype.molecular.internalized_total_substrates[index-start_int_substrate_ind]; 
		out /= pCell->phenotype.volume.total;
		out /= signal_scales[index]; 
		return out; 
	}

	// next m entries: gradients 
	static int start_substrate_grad_ind = find_signal_index( microenvironment.density_names[0] + " gradient"); 
	if( start_substrate_grad_ind <= index && index < start_substrate_grad_ind + m )
	{
		out =  norm( pCell->nearest_gradient(index-start_substrate_grad_ind) ); 
		out /= signal_scales[index]; 
		return out; 
	}

	// mechanical pressure 
	static int pressure_ind = find_signal_index( "pressure" ); 
	if( index == pressure_ind )
	{
		out = pCell->state.simple_pressure;
		out /= signal_scales[index]; 
		return out; 
	}

	// cell volume 	
	static int volume_ind = find_signal_index( "volume"); 
	if( index == volume_ind )
	{
		out = pCell->phenotype.volume.total; 
		out /= signal_scales[index]; 
		return out; 
	}

	// physical contact with cells (of each type) 
	// individual contact signals are a bit costly 
	static int contact_ind = find_signal_index( "contact with " + cell_definitions_by_type[0]->name ); 
	if( contact_ind <= index && index < contact_ind + n+2 )
	{
		std::vector<int> counts( n , 0 ); 
		// process all neighbors 
		int dead_cells = 0; 
		int apop_cells = 0;
		int necro_cells = 0; 
		int other_dead_cells = 0; 
		int live_cells = 0; 
		for( int i=0; i < pCell->state.neighbors.size(); i++ )
		{
			Cell* pC = pCell->state.neighbors[i]; 
			if( pC->phenotype.death.dead == true )
			{
				dead_cells++;
				if(pC->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )
				{ apop_cells++; }
				if( pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
					pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
					pC->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
				{ necro_cells++; }  				
			} 
			else
			{ live_cells++; } 
			int nCT = cell_definition_indices_by_type[pC->type]; 
			counts[nCT] += 1; 
		}
		other_dead_cells = dead_cells - apop_cells - necro_cells; 		

		if( index < contact_ind + n )
		{
			out = counts[index-contact_ind]; 
			out /= signal_scales[index]; 
			return out; 
		}

		static int live_contact_ind = find_signal_index( "contact with live cell"); 
		if( index == live_contact_ind )
		{
			out = live_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		static int dead_contact_ind = find_signal_index( "contact with dead cell"); 
		if( index == dead_contact_ind )
		{
			out = dead_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		static int apop_contact_ind = find_signal_index( "contact with apoptotic cell"); 
		if( index == apop_contact_ind )
		{
			out = apop_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		static int necro_contact_ind = find_signal_index( "contact with necrotic cell"); 
		if( index == necro_contact_ind )
		{
			out = necro_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		static int other_dead_contact_ind = find_signal_index( "contact with other dead cell"); 
		if( index == other_dead_contact_ind )
		{
			out = other_dead_cells; 
			out /= signal_scales[index]; 
			return out; 
		}

		return out; 
	}

	// contact with BM 
	static int BM_contact_ind = find_signal_index( "contact with basement membrane"); 
	if( index == BM_contact_ind )
	{ 
		out = (double) pCell->state.contact_with_basement_membrane; 
		out /= signal_scales[index]; 
		return out; 
	} 

	// damage
	static int damage_ind = find_signal_index( "damage"); 
	if( index == damage_ind )
	{
		out = pCell->phenotype.cell_integrity.damage; 
		out /= signal_scales[index]; 
		return out; 
	} 

	// damage delivered
	static int damage_deliv_ind = find_signal_index( "damage delivered"); 
	if( index == damage_deliv_ind )
	{
		out = pCell->phenotype.cell_interactions.total_damage_delivered ; 
		out /= signal_scales[index]; 
		return out; 
	} 

	// live / dead state 
	static int dead_ind = find_signal_index( "dead" ); 
	if( index == dead_ind )
	{
		out = (double) pCell->phenotype.death.dead;  
		out /= signal_scales[index]; 
		return out; 
	} 

	// integrated total attack time 
	static int tot_attack_ind = find_signal_index( "total attack time"); 
	if( index == tot_attack_ind )
	{
		out = pCell->state.total_attack_time;     
		out /= signal_scales[index]; 
		return out; 
	} 

	// attacking? yes or no 
    static int attacking_ind = find_signal_index( "attacking"); 
	if( index == attacking_ind )
	{
		out = 0; 
		if( pCell->phenotype.cell_interactions.pAttackTarget ) 
		{ out = 1; }
		return out; 
	} 

	// time 
	static int time_ind = find_signal_index( "time" ); 
	if( index == time_ind )
	{
		out = PhysiCell_globals.current_time;      
		out /= signal_scales[index]; 
		return out; 
	} 

	// custom signals 
	static int first_custom_ind = find_signal_index( "custom 0" ); 
	static int max_custom_ind = first_custom_ind+pCell->custom_data.variables.size(); 
	if( first_custom_ind > -1 && index >= first_custom_ind && index < max_custom_ind )
	{
		out = pCell->custom_data.variables[ index-first_custom_ind ].value; 
		out /= signal_scales[index];
		return out; 
	}

	static int apoptotic_ind = find_signal_index( "apoptotic" ); 
	if( index == apoptotic_ind )
	{
		if(pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )
		{ return 1; }
		else
		{ return 0; }
	}

	static int necrotic_ind = find_signal_index( "necrotic" ); 
	if( index == necrotic_ind )
	{
		if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
		{ return 1; }
		else
		{ return 0; }
	}

/*
	static int start_immunogenicity_ind = find_signal_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
	static int max_immunogenicity_ind = start_immunogenicity_ind+n; 
	if( start_immunogenicity_ind > -1 && index >= start_immunogenicity_ind && index < max_immunogenicity_ind )
	{
		int j = index - start_immunogenicity_ind; 
		out = pCell->phenotype.cell_interactions.immunogenicities[j]; 
		out /= signal_scales[index];
		return out; 
	}
*/


	// unknown after here !

	std::cout << "Warning: Requested unknown signal number " << index << "!" << std::endl
		      << "         Returning 0.0, but you should fix this!" << std::endl << std::endl; 

	return 0.0; 
}

double get_single_signal( Cell* pCell, std::string name )
{ return get_single_signal( pCell, find_signal_index(name) ); }

// behaviors 

std::string behavior_name( int i )
{
	if( i >= 0 && i < int_to_behavior.size() )
	{ return int_to_behavior[i]; }
	return "not found"; 
}

std::vector<double> create_empty_behavior_vector()
{ 
	std::vector<double> parameters( int_to_behavior.size() , 0.0 ); 
	return parameters; 
}

void set_behaviors( Cell* pCell , std::vector<double> parameters )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	// substrate-related behaviors 
	
	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	std::copy(  parameters.begin()+first_secretion_index , 
				parameters.begin()+first_secretion_index + m , 
				pCell->phenotype.secretion.secretion_rates.begin() ); 

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	std::copy(  parameters.begin()+first_secretion_target_index , 
				parameters.begin()+first_secretion_target_index + m , 
				pCell->phenotype.secretion.saturation_densities.begin() ); 

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	std::copy(  parameters.begin()+first_uptake_index , 
				parameters.begin()+first_uptake_index + m , 
				pCell->phenotype.secretion.uptake_rates.begin() ); 

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	std::copy(  parameters.begin()+first_export_index , 
				parameters.begin()+first_export_index + m , 
				pCell->phenotype.secretion.net_export_rates.begin() ); 

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	int max_cycle_index = pCell->phenotype.cycle.model().phases.size(); 
	if( max_cycle_index > 6 )
	{
		max_cycle_index = 6; 
		std::cout << "Warning: Standardized behaviors only support exit rate from the first 6 phases of a cell cycle!" << std::endl 
		          << "         Ignoring any later phase exit rates." << std::endl; 
	}
	for( int i=0; i < max_cycle_index ; i++ )
	{ pCell->phenotype.cycle.data.exit_rate( i ) = parameters[first_cycle_index + i]; }

	static int apoptosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int necrosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 

	// apoptosis 
	static int apoptosis_param_index = find_behavior_index( "apoptosis"); 
	pCell->phenotype.death.rates[apoptosis_index] = parameters[apoptosis_param_index]; 

	// necrosis 
	static int necrosis_param_index = find_behavior_index( "necrosis"); 
	pCell->phenotype.death.rates[necrosis_index] = parameters[necrosis_param_index]; 

	// migration speed
	static int migration_speed_index = find_behavior_index("migration speed"); 
	pCell->phenotype.motility.migration_speed = parameters[migration_speed_index]; 

	// migration bias 
	static int migration_bias_index = find_behavior_index("migration bias"); 
	pCell->phenotype.motility.migration_bias = parameters[migration_bias_index]; 

	// migration persistence time
	static int migration_pt_index = find_behavior_index("migration persistence time"); 
	pCell->phenotype.motility.persistence_time = parameters[migration_pt_index]; 

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	std::copy(  parameters.begin()+first_chemotaxis_index , 
				parameters.begin()+first_chemotaxis_index + m , 
				pCell->phenotype.motility.chemotactic_sensitivities.begin() ); 	

	// cell-cell adhesion 
	static int cca_index = find_behavior_index("cell-cell adhesion"); 
	pCell->phenotype.mechanics.cell_cell_adhesion_strength = parameters[cca_index]; 

	// cell-cell "springs"
	static int cca_spring_index = find_behavior_index("cell-cell adhesion elastic constant"); 
	pCell->phenotype.mechanics.attachment_elastic_constant = parameters[cca_spring_index]; 

    // cell adhesion affinities 
	static int first_affinity_index = find_behavior_index( "adhesive affinity to " + cell_definitions_by_type[0]->name ); 
	std::copy(  parameters.begin()+first_affinity_index , 
				parameters.begin()+first_affinity_index + n , 
				pCell->phenotype.mechanics.cell_adhesion_affinities.begin() ); 	

	// max relative maximum adhesion distance 
	static int max_adhesion_distance_index = find_behavior_index("relative maximum adhesion distance"); 
	pCell->phenotype.mechanics.relative_maximum_adhesion_distance = parameters[max_adhesion_distance_index]; 

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index("cell-cell repulsion"); 
	pCell->phenotype.mechanics.cell_cell_repulsion_strength = parameters[ccr_index]; 

	// cell-BM adhesion 
	static int cba_index = find_behavior_index("cell-BM adhesion"); 
	pCell->phenotype.mechanics.cell_BM_adhesion_strength = parameters[cba_index]; 
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index("cell-BM repulsion"); 
	pCell->phenotype.mechanics.cell_BM_repulsion_strength = parameters[cbr_index]; 

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell"); 
	pCell->phenotype.cell_interactions.apoptotic_phagocytosis_rate = parameters[apop_phag_index]; 

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell"); 
	pCell->phenotype.cell_interactions.necrotic_phagocytosis_rate = parameters[necro_phag_index]; 

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell"); 
	pCell->phenotype.cell_interactions.other_dead_phagocytosis_rate = parameters[other_dead_phag_index]; 

    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	std::copy(  parameters.begin()+first_phagocytosis_index , 
				parameters.begin()+first_phagocytosis_index + n , 
				pCell->phenotype.cell_interactions.live_phagocytosis_rates.begin() ); 	

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	std::copy(  parameters.begin()+first_attack_index , 
				parameters.begin()+first_attack_index + n , 
				pCell->phenotype.cell_interactions.attack_rates.begin() ); 	
 
	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	std::copy(  parameters.begin()+first_fusion_index , 
				parameters.begin()+first_fusion_index + n , 
				pCell->phenotype.cell_interactions.fusion_rates.begin() ); 	

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	std::copy(  parameters.begin()+first_transformation_index , 
				parameters.begin()+first_transformation_index+n , 
				pCell->phenotype.cell_transformations.transformation_rates.begin() ); 	

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	std::copy(  parameters.begin()+first_asymmetric_division_index , 
				parameters.begin()+first_asymmetric_division_index+n , 
				pCell->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities.begin() );

	// custom behaviors
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	if( first_custom_ind >= 0 )
	{
		for( int nc=0 ; nc < pCell->custom_data.variables.size() ; nc++ )
		{ pCell->custom_data.variables[nc].value = parameters[first_custom_ind+nc]; }
	}

	// set cell to movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( parameters[movable_ind] > 0.5 )
	{ pCell->is_movable = true; }
	else
	{ pCell->is_movable = false; }

	// vector of immunogenicity signals 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
    std::copy( parameters.begin()+start_immunogenicity_ind , 
			   parameters.begin()+start_immunogenicity_ind+n , 
			   pCell->phenotype.cell_interactions.immunogenicities.begin() );  

	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	pCell->phenotype.mechanics.attachment_rate = parameters[attachment_rate_ind];

	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	pCell->phenotype.mechanics.detachment_rate = parameters[detachment_rate_ind];

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	pCell->phenotype.mechanics.maximum_number_of_attachments = (int) parameters[max_attachments_ind];

	// cell damage rate (for effector attack)
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	pCell->phenotype.cell_interactions.attack_damage_rate = parameters[attack_damage_rate_ind]; 

	// attack duration (for effector attack)
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	pCell->phenotype.cell_interactions.attack_duration = parameters[attack_duration_ind]; 

	// damage rate (non-effector)
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	pCell->phenotype.cell_integrity.damage_rate = parameters[damage_rate_ind]; 

	// damage repair rate (non-effector)
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	pCell->phenotype.cell_integrity.damage_repair_rate = parameters[damage_repair_rate_ind]; 

	return; 
}

void set_single_behavior( Cell* pCell, int index , double parameter )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	if( index < 0 )
	{ 
		std::cout << "Warning! Tried to set behavior of unknown index " << index << "!" << std::endl
				  << "         I'll ignore it, but you should fix it!" << std::endl; 
		return;
	}

	// substrate-related behaviors 
	
	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	if( index >= first_secretion_index && index < first_secretion_index + m )
	{ pCell->phenotype.secretion.secretion_rates[index-first_secretion_index] = parameter; return; }

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	if( index >= first_secretion_target_index && index < first_secretion_target_index + m )
	{ pCell->phenotype.secretion.saturation_densities[index-first_secretion_target_index] = parameter; return; }

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	if( index >= first_uptake_index && index < first_uptake_index + m )
	{ pCell->phenotype.secretion.uptake_rates[index-first_uptake_index] = parameter; return; }

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	if( index >= first_export_index && index < first_export_index + m )
	{ pCell->phenotype.secretion.net_export_rates[index-first_export_index] = parameter; return; }

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	if( index >= first_cycle_index && index < first_cycle_index+6 && !pCell->phenotype.death.dead )
	{
		int max_cycle_index = pCell->phenotype.cycle.model().phases.size(); 
		if( index < first_cycle_index + max_cycle_index )
		{ pCell->phenotype.cycle.data.exit_rate(index-first_cycle_index) = parameter; return; }
		std::cout << "Warning: Attempted to set a cycle exit rate outside the bounds of the cell's cycle model" << std::endl
       		      <<        "         Ignoring it, but you should fix this." << std::endl; 
		return; 
	}

	// death rates 

	// apoptosis
	static int apoptosis_model_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int apoptosis_parameter_index = find_behavior_index( "apoptosis"); 
	if( index == apoptosis_parameter_index )
	{ pCell->phenotype.death.rates[apoptosis_model_index] = parameter; return; }

	// necrosis 
	static int necrosis_model_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
	static int necrosis_parameter_index = find_behavior_index( "necrosis"); 
	if( index == necrosis_parameter_index )
	{ pCell->phenotype.death.rates[necrosis_model_index] = parameter; return; }

	// migration speed
	static int migration_speed_index = find_behavior_index( "migration speed"); 
	if( index == migration_speed_index )
	{ pCell->phenotype.motility.migration_speed = parameter; return; } 

	// migration bias 
	static int migration_bias_index = find_behavior_index( "migration bias"); 
	if( index == migration_bias_index )
	{ pCell->phenotype.motility.migration_bias = parameter; return; } 

	// migration persistence time
	static int persistence_time_index = find_behavior_index( "migration persistence time"); 
	if( index == persistence_time_index )
	{ pCell->phenotype.motility.persistence_time = parameter; return; } 

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	if( index >= first_chemotaxis_index && index < first_chemotaxis_index + m )
	{ pCell->phenotype.motility.chemotactic_sensitivities[index-first_chemotaxis_index] = parameter; return; } 

	// cell-cell adhesion 
	static int cca_index = find_behavior_index( "cell-cell adhesion"); 
	if( index == cca_index )
	{ pCell->phenotype.mechanics.cell_cell_adhesion_strength = parameter; return; } 

	// cell-cell "springs"
	static int elastic_index = find_behavior_index( "cell-cell adhesion elastic constant"); 
	if( index == elastic_index )
	{ pCell->phenotype.mechanics.attachment_elastic_constant = parameter; return; } 

    // cell adhesion affinities 
	static int first_affinity_index = find_behavior_index( "adhesive affinity to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_affinity_index && index < first_affinity_index + n )
	{ pCell->phenotype.mechanics.cell_adhesion_affinities[index-first_affinity_index] = parameter; return; } 
 
	// max relative maximum adhesion distance 
	static int max_adh_distance_index = find_behavior_index( "relative maximum adhesion distance" ); 
	if( index == max_adh_distance_index )
	{ pCell->phenotype.mechanics.relative_maximum_adhesion_distance = parameter; return; } 

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index( "cell-cell repulsion" ); 
	if( index == ccr_index )
	{ pCell->phenotype.mechanics.cell_cell_repulsion_strength = parameter; return; } 

	// cell-BM adhesion 
	static int cba_index = find_behavior_index( "cell-BM adhesion" ); 
	if( index == cba_index )
	{ pCell->phenotype.mechanics.cell_BM_adhesion_strength = parameter; return; } 
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index( "cell-BM repulsion" ); 
	if( index == cbr_index )
	{ pCell->phenotype.mechanics.cell_BM_repulsion_strength = parameter; return; } 

	// apoptotic cell phagocytosis
	static int apop_phago_index = find_behavior_index( "phagocytose apoptotic cell" ); 
	if( index == apop_phago_index )
	{ pCell->phenotype.cell_interactions.apoptotic_phagocytosis_rate = parameter; return; } 

	// necrotic cell phagocytosis
	static int necro_phago_index = find_behavior_index( "phagocytose necrotic cell" ); 
	if( index == necro_phago_index )
	{ pCell->phenotype.cell_interactions.necrotic_phagocytosis_rate = parameter; return; } 

	// other dead cell phagocytosis
	static int other_dead_phago_index = find_behavior_index( "phagocytose other dead cell" ); 
	if( index == other_dead_phago_index )
	{ pCell->phenotype.cell_interactions.other_dead_phagocytosis_rate = parameter; return; } 
 
    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	if( index >= first_phagocytosis_index && index < first_phagocytosis_index + n )
	{ pCell->phenotype.cell_interactions.live_phagocytosis_rates[index-first_phagocytosis_index] = parameter ; return; } 

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	if( index >= first_attack_index && index < first_attack_index + n )
	{ pCell->phenotype.cell_interactions.attack_rates[index-first_attack_index] = parameter; return; } 
 
	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_fusion_index && index < first_fusion_index + n )
	{ pCell->phenotype.cell_interactions.fusion_rates[index-first_fusion_index] = parameter; return; } 

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_transformation_index && index < first_transformation_index + n )
	{ pCell->phenotype.cell_transformations.transformation_rates[index-first_transformation_index] = parameter; return; } 

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	if( index >= first_asymmetric_division_index && index < first_asymmetric_division_index + n )
	{ pCell->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities[index-first_asymmetric_division_index] = parameter; return; }

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCell->custom_data.variables.size();  
	if( first_custom_ind >= 0 && index >= first_custom_ind && index < max_custom_ind )
	{ pCell->custom_data.variables[index-first_custom_ind].value = parameter; }

	// set cell to movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( index == movable_ind )
	{
		if( parameter > 0.5 )
		{ pCell->is_movable = true; }
		else
		{ pCell->is_movable = false; }
	}

    // immunogenicity to each cell type 
	static int first_immunogenicity_index = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_immunogenicity_index && index < first_immunogenicity_index + n )
	{ pCell->phenotype.cell_interactions.immunogenicities[index-first_immunogenicity_index] = parameter ; return; } 


	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	if( index == attachment_rate_ind )
	{ pCell->phenotype.mechanics.attachment_rate = parameter; }

	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	if( index == detachment_rate_ind )
	{ pCell->phenotype.mechanics.detachment_rate = parameter; }

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	if( index == max_attachments_ind )
	{ pCell->phenotype.mechanics.maximum_number_of_attachments = (int) parameter; }

	// cell damage rate (for effector attack)
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	if( index == attack_damage_rate_ind )
	{ pCell->phenotype.cell_interactions.attack_damage_rate = parameter; }

    // attack duration (for effector attack)
    static int attack_duration_ind = find_behavior_index( "attack duration"); 
	if( index == attack_duration_ind )
    { pCell->phenotype.cell_interactions.attack_duration = parameter; } 

    // damage rate (non-effector)
    static int damage_rate_ind = find_behavior_index( "damage rate"); 
	if( index == damage_rate_ind )
    { pCell->phenotype.cell_integrity.damage_rate = parameter; } 

    // damage repair rate (non-effector)
    static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	if( index == damage_repair_rate_ind )
    { pCell->phenotype.cell_integrity.damage_repair_rate = parameter; } 

	return; 
}

void set_single_behavior( Cell* pCell, std::string name , double parameter )
{
	int index = find_behavior_index( name ); 
	return set_single_behavior(pCell,index,parameter); 
}

std::vector<double> get_behaviors( Cell* pCell )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	std::vector<double> parameters( int_to_behavior.size() , 0.0 ); 

	// substrate-related behaviors 
	
	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	std::copy(  pCell->phenotype.secretion.secretion_rates.begin(), 
				pCell->phenotype.secretion.secretion_rates.end(), 
				parameters.begin()+first_secretion_index ); 

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	std::copy(  pCell->phenotype.secretion.saturation_densities.begin(), 
				pCell->phenotype.secretion.saturation_densities.end(), 
				parameters.begin()+first_secretion_target_index ); 

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	std::copy(  pCell->phenotype.secretion.uptake_rates.begin(), 
				pCell->phenotype.secretion.uptake_rates.end(), 
				parameters.begin()+first_uptake_index ); 

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	std::copy(  pCell->phenotype.secretion.net_export_rates.begin(), 
				pCell->phenotype.secretion.net_export_rates.end(), 
				parameters.begin()+first_export_index ); 

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	int max_cycle_index = pCell->phenotype.cycle.model().phases.size(); 
	if( max_cycle_index > 6 )
	{
		max_cycle_index = 6; 
		std::cout << "Warning: Standardized behaviors only support exit rate from the first 6 phases of a cell cycle!" << std::endl 
		          << "         Ignoring any later phase exit rates." << std::endl; 
	}
	for( int i=0; i < max_cycle_index ; i++ )
	{ parameters[first_cycle_index+i] = pCell->phenotype.cycle.data.exit_rate( i ); }

	static int apoptosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int necrosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 

	// apoptosis 
	static int apoptosis_param_index = find_behavior_index( "apoptosis"); 
	parameters[apoptosis_param_index] = pCell->phenotype.death.rates[apoptosis_index];

	// necrosis 
	static int necrosis_param_index = find_behavior_index( "necrosis"); 
	parameters[necrosis_param_index] = pCell->phenotype.death.rates[necrosis_index]; 

	// migration speed
	static int migration_speed_index = find_behavior_index("migration speed"); 
	parameters[migration_speed_index] = pCell->phenotype.motility.migration_speed; 

	// migration bias 
	static int migration_bias_index = find_behavior_index("migration bias"); 
	parameters[migration_bias_index] = pCell->phenotype.motility.migration_bias; 

	// migration persistence time
	static int migration_pt_index = find_behavior_index("migration persistence time"); 
	parameters[migration_pt_index] = pCell->phenotype.motility.persistence_time; 

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	std::copy(  pCell->phenotype.motility.chemotactic_sensitivities.begin() ,
				pCell->phenotype.motility.chemotactic_sensitivities.end() ,
			 	parameters.begin()+first_chemotaxis_index ); 

	// cell-cell adhesion 
	static int cca_index = find_behavior_index("cell-cell adhesion"); 
	parameters[cca_index] = pCell->phenotype.mechanics.cell_cell_adhesion_strength; 

	// cell-cell "springs"
	static int cca_spring_index = find_behavior_index("cell-cell adhesion elastic constant"); 
	parameters[cca_spring_index] = pCell->phenotype.mechanics.attachment_elastic_constant; 

    // cell adhesion affinities 
	static std::string search_for1 = "adhesive affinity to " + cell_definitions_by_type[0]->name ; 
	static int first_affinity_index = find_behavior_index( search_for1 ); 
	std::copy(  pCell->phenotype.mechanics.cell_adhesion_affinities.begin(), 
				pCell->phenotype.mechanics.cell_adhesion_affinities.end() ,
				parameters.begin()+first_affinity_index ); 

	// max relative maximum adhesion distance 
	static int max_adhesion_distance_index = find_behavior_index("relative maximum adhesion distance"); 
	parameters[max_adhesion_distance_index] = pCell->phenotype.mechanics.relative_maximum_adhesion_distance; 

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index("cell-cell repulsion"); 
	parameters[ccr_index] = pCell->phenotype.mechanics.cell_cell_repulsion_strength; 

	// cell-BM adhesion 
	static int cba_index = find_behavior_index("cell-BM adhesion"); 
	parameters[cba_index] = pCell->phenotype.mechanics.cell_BM_adhesion_strength; 
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index("cell-BM repulsion"); 
	parameters[cbr_index] = pCell->phenotype.mechanics.cell_BM_repulsion_strength; 

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell"); 
	parameters[apop_phag_index] = pCell->phenotype.cell_interactions.apoptotic_phagocytosis_rate; 

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell"); 
	parameters[necro_phag_index] = pCell->phenotype.cell_interactions.necrotic_phagocytosis_rate; 

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell"); 
	parameters[other_dead_phag_index] = pCell->phenotype.cell_interactions.other_dead_phagocytosis_rate; 

    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCell->phenotype.cell_interactions.live_phagocytosis_rates.begin(), 
				pCell->phenotype.cell_interactions.live_phagocytosis_rates.end(), 
				parameters.begin()+first_phagocytosis_index ); 	

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCell->phenotype.cell_interactions.attack_rates.begin(), 
				pCell->phenotype.cell_interactions.attack_rates.end(), 
				parameters.begin()+first_attack_index ); 	
 
	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCell->phenotype.cell_interactions.fusion_rates.begin(), 
				pCell->phenotype.cell_interactions.fusion_rates.end(), 
				parameters.begin()+first_fusion_index ); 	

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCell->phenotype.cell_transformations.transformation_rates.begin(), 
				pCell->phenotype.cell_transformations.transformation_rates.end(), 
				parameters.begin()+first_transformation_index ); 	

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	std::copy(  pCell->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities.begin(), 
				pCell->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities.end(), 
				parameters.begin()+first_asymmetric_division_index );

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCell->custom_data.variables.size();  
	if( first_custom_ind >= 0 )
	{
		for( int nc=0; nc < pCell->custom_data.variables.size(); nc++ )
		{ parameters[first_custom_ind+nc] = pCell->custom_data.variables[nc].value; }	 
	}

	// is the cell movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( pCell->is_movable == true )
	{ parameters[movable_ind] = 1; }
	else
	{ parameters[movable_ind] = 0; }

	// vector of immunogenicity behaviors 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
    std::copy( pCell->phenotype.cell_interactions.immunogenicities.begin(),
			   pCell->phenotype.cell_interactions.immunogenicities.end(), 
			   parameters.begin()+start_immunogenicity_ind );  

	// get cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	parameters[attachment_rate_ind] = pCell->phenotype.mechanics.attachment_rate; 

	// get cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	parameters[detachment_rate_ind] = pCell->phenotype.mechanics.detachment_rate; 

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	parameters[max_attachments_ind] = pCell->phenotype.mechanics.maximum_number_of_attachments; 

	// attack get damage rate 
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	parameters[attack_damage_rate_ind] = pCell->phenotype.cell_interactions.attack_damage_rate; 

	// get attack duration
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	parameters[attack_duration_ind] = pCell->phenotype.cell_interactions.attack_duration; 

	// get damage rate 
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	parameters[damage_rate_ind] = pCell->phenotype.cell_integrity.damage_rate; 
	
	// get damage repair rate 
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	parameters[damage_repair_rate_ind] = pCell->phenotype.cell_integrity.damage_repair_rate; 

	return parameters; 
}

double get_single_behavior( Cell* pCell , int index )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	if( index < 0 )
	{
		std::cout << "Warning: attempted to get behavior with unknown index " << index << std::endl	
				  << "         I'm ignoring it, but you should fix it!" << std::endl; 
		return 0.0; 
	}

	// substrate-related behaviors 

	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	if( index >= first_secretion_index && index < first_secretion_index + m )
	{ return pCell->phenotype.secretion.secretion_rates[index-first_secretion_index]; }

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	if( index >= first_secretion_target_index && index < first_secretion_target_index + m )
	{ return pCell->phenotype.secretion.saturation_densities[index-first_secretion_target_index]; }

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	if( index >= first_uptake_index && index < first_uptake_index + m )
	{ return pCell->phenotype.secretion.uptake_rates[index-first_uptake_index]; }

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	if( index >= first_export_index && index < first_export_index + m )
	{ return pCell->phenotype.secretion.net_export_rates[index-first_export_index]; }

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	int max_cycle_index = pCell->phenotype.cycle.model().phases.size(); 
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
		{ return pCell->phenotype.cycle.data.exit_rate( ind ); }
		return 0.0; 
	}

	static int apoptosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int necrosis_index = pCell->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 

	static int apop_param_index = find_behavior_index( "apoptosis"); 
	static int necr_param_index = find_behavior_index( "necrosis"); 

	// apoptosis 
	if( index == apop_param_index )
	{ return pCell->phenotype.death.rates[apoptosis_index]; }

	// necrosis 
	if( index == necr_param_index )
	{ return pCell->phenotype.death.rates[necrosis_index]; }

	// migration speed
	static int migr_spd_index = find_behavior_index( "migration speed"); 
	if( index == migr_spd_index )
	{ return pCell->phenotype.motility.migration_speed; }

	// migration bias 
	static int migr_bias_index = find_behavior_index( "migration bias"); 
	if( index == migr_bias_index )
	{ return pCell->phenotype.motility.migration_bias; }

	// migration persistence time
	static int migr_pt_index = find_behavior_index( "migration persistence time"); 
	if( index == migr_pt_index )
	{ return pCell->phenotype.motility.persistence_time; }

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	if( index >= first_chemotaxis_index && index < first_chemotaxis_index + m )
	{ return pCell->phenotype.motility.chemotactic_sensitivities[index-first_chemotaxis_index]; }

	// cell-cell adhesion 
	static int cca_index = find_behavior_index( "cell-cell adhesion" ); 
	if( index == cca_index )
	{ return pCell->phenotype.mechanics.cell_cell_adhesion_strength; }

	// cell-cell "springs"
	static int cca_spring_index = find_behavior_index( "cell-cell adhesion elastic constant" );  
	if( index == cca_spring_index )
	{ return pCell->phenotype.mechanics.attachment_elastic_constant; }

    // cell adhesion affinities 
	static int first_affinity_index = find_behavior_index("adhesive affinity to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_affinity_index && index < first_affinity_index + n )
	{ return pCell->phenotype.mechanics.cell_adhesion_affinities[index-first_affinity_index]; }

	// max relative maximum adhesion distance 
	static int max_adh_index = find_behavior_index("relative maximum adhesion distance" ); 
	if( index == max_adh_index )
	{ return pCell->phenotype.mechanics.relative_maximum_adhesion_distance; }

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index("cell-cell repulsion" ); 
	if( index == ccr_index )
	{ return pCell->phenotype.mechanics.cell_cell_repulsion_strength; }

	// cell-BM adhesion 
	static int cba_index = find_behavior_index("cell-BM adhesion" ); 
	if( index == cba_index )
	{ return pCell->phenotype.mechanics.cell_BM_adhesion_strength; }
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index("cell-BM repulsion" ); 
	if( index == cbr_index )
	{ return pCell->phenotype.mechanics.cell_BM_repulsion_strength; }

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell" ); 
	if( index == apop_phag_index )
	{ return pCell->phenotype.cell_interactions.apoptotic_phagocytosis_rate; }

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell" ); 
	if( index == necro_phag_index )
	{ return pCell->phenotype.cell_interactions.necrotic_phagocytosis_rate; }

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell" ); 
	if( index == other_dead_phag_index )
	{ return pCell->phenotype.cell_interactions.other_dead_phagocytosis_rate; }

    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	if( index >= first_phagocytosis_index && index < first_phagocytosis_index+n )
	{ return pCell->phenotype.cell_interactions.live_phagocytosis_rates[index-first_phagocytosis_index]; } 

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	if( index >= first_attack_index && index < first_attack_index+n )
	{ return pCell->phenotype.cell_interactions.attack_rates[index-first_attack_index]; } 

	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_fusion_index && index < first_fusion_index+n )
	{ return pCell->phenotype.cell_interactions.fusion_rates[index-first_fusion_index]; } 

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	if( index >= first_transformation_index && index < first_transformation_index+n )
	{ return pCell->phenotype.cell_transformations.transformation_rates[index-first_transformation_index]; } 

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	if( index >= first_asymmetric_division_index && index < first_asymmetric_division_index+n )
	{ return pCell->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities[index-first_asymmetric_division_index]; }

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCell->custom_data.variables.size();  
	if( first_custom_ind >= 0 && index >= first_custom_ind && index < max_custom_ind )
	{ return pCell->custom_data.variables[index-first_custom_ind].value; }

	// is the cell movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( index == movable_ind )
	{
		if( pCell->is_movable == true )
		{ return 1.0; }
		else
		{ return 0.0; }
	}

	// vector of immunogenicity behaviors 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
	static int max_immunogenicity_ind = start_immunogenicity_ind + n; 
	if( start_immunogenicity_ind > -1 && index >= start_immunogenicity_ind && index < max_immunogenicity_ind )
	{ return pCell->phenotype.cell_interactions.immunogenicities[index-start_immunogenicity_ind]; }


	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	if( index == attachment_rate_ind )
	return pCell->phenotype.mechanics.attachment_rate; 
	
	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	if( index == detachment_rate_ind )
	{ return pCell->phenotype.mechanics.detachment_rate; }

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	if( index == max_attachments_ind )
	{ return pCell->phenotype.mechanics.maximum_number_of_attachments; }

	// get attack damage rate 
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	if( index == attack_damage_rate_ind )
	{ return pCell->phenotype.cell_interactions.attack_damage_rate; }

	// get attack duration 
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	if( index == attack_duration_ind )
	{ return pCell->phenotype.cell_interactions.attack_duration; }

	// get damage rate 
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	if( index == damage_rate_ind )
	{ return pCell->phenotype.cell_integrity.damage_rate; }

	// get damage repair rate 
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	if( index == damage_repair_rate_ind )
	{ return pCell->phenotype.cell_integrity.damage_repair_rate; }

	return -1; 
}

double get_single_behavior( Cell* pCell , std::string name )
{ return get_single_behavior(pCell,find_behavior_index(name) ); }

std::vector<double> get_behaviors( Cell* pCell , std::vector<int> indices )
{
	std::vector<double> parameters( indices.size() , 0.0 ); 
	for( int n=0; n < indices.size(); n++ )
	{ parameters[n] = get_single_behavior(pCell,indices[n]); }
	return parameters; 
}

std::vector<double> get_behaviors( Cell* pCell , std::vector<std::string> names )
{
	std::vector<double> parameters( names.size() , 0.0 ); 
	for( int n=0; n < names.size(); n++ )
	{ parameters[n] = get_single_behavior(pCell,names[n]); }
	return parameters; 
}

void set_selected_behaviors( Cell* pCell , std::vector<int> indices , std::vector<double> parameters )
{
	for( int i=0 ; i < indices.size() ; i++ )
	{ set_single_behavior(pCell,indices[i],parameters[i]); }
	return; 
}

void set_selected_behaviors( Cell* pCell , std::vector<std::string> names , std::vector<double> parameters )
{
	for( int i=0 ; i < names.size() ; i++ )
	{ set_single_behavior(pCell,find_behavior_index(names[i]),parameters[i]); }
	return; 
}

std::vector<double> get_base_behaviors( Cell* pCell )
{
	Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 

	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	std::vector<double> parameters( int_to_behavior.size() , 0.0 ); 

	// substrate-related behaviors 
	
	// first m entries are secretion 
	static int first_secretion_index = find_behavior_index( microenvironment.density_names[0] + " secretion" ); // 0; 
	std::copy(  pCD->phenotype.secretion.secretion_rates.begin(), 
				pCD->phenotype.secretion.secretion_rates.end(), 
				parameters.begin()+first_secretion_index ); 

	// next m entries are secretion targets
	static int first_secretion_target_index = find_behavior_index( microenvironment.density_names[0] + " secretion target" ); // m; 
	std::copy(  pCD->phenotype.secretion.saturation_densities.begin(), 
				pCD->phenotype.secretion.saturation_densities.end(), 
				parameters.begin()+first_secretion_target_index ); 

	// next m entries are uptake rates
	static int first_uptake_index = find_behavior_index( microenvironment.density_names[0] + " uptake" );  // 2*m; 
	std::copy(  pCD->phenotype.secretion.uptake_rates.begin(), 
				pCD->phenotype.secretion.uptake_rates.end(), 
				parameters.begin()+first_uptake_index ); 

	// next m entries are net export rates 
	static int first_export_index = find_behavior_index( microenvironment.density_names[0] + " export" ); //  3*m; 
	std::copy(  pCD->phenotype.secretion.net_export_rates.begin(), 
				pCD->phenotype.secretion.net_export_rates.end(), 
				parameters.begin()+first_export_index ); 

	// cycle entry (exit from phase 0) and exit from up to 5 more phases 
	static int first_cycle_index = find_behavior_index("exit from cycle phase 0" ); //  4*m; 
	int max_cycle_index = pCD->phenotype.cycle.model().phases.size(); 
	if( max_cycle_index > 6 )
	{
		max_cycle_index = 6; 
		std::cout << "Warning: Standardized behaviors only support exit rate from the first 6 phases of a cell cycle!" << std::endl 
		          << "         Ignoring any later phase exit rates." << std::endl; 
	}
	for( int i=0; i < max_cycle_index ; i++ )
	{  parameters[first_cycle_index + i] = pCD->phenotype.cycle.data.exit_rate( i ); }

	static int apoptosis_index = pCD->phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	static int necrosis_index = pCD->phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 

	// apoptosis 
	static int apoptosis_param_index = find_behavior_index( "apoptosis"); 
	parameters[apoptosis_param_index] = pCD->phenotype.death.rates[apoptosis_index];

	// necrosis 
	static int necrosis_param_index = find_behavior_index( "necrosis"); 
	parameters[necrosis_param_index] = pCD->phenotype.death.rates[necrosis_index]; 

	// migration speed
	static int migration_speed_index = find_behavior_index("migration speed"); 
	parameters[migration_speed_index] = pCD->phenotype.motility.migration_speed; 

	// migration bias 
	static int migration_bias_index = find_behavior_index("migration bias"); 
	parameters[migration_bias_index] = pCD->phenotype.motility.migration_bias; 

	// migration persistence time
	static int migration_pt_index = find_behavior_index("migration persistence time"); 
	parameters[migration_pt_index] = pCD->phenotype.motility.persistence_time; 

	// chemotactic sensitivities 
	static int first_chemotaxis_index = find_behavior_index( "chemotactic response to " + microenvironment.density_names[0] ); 
	std::copy(  pCD->phenotype.motility.chemotactic_sensitivities.begin() ,
				pCD->phenotype.motility.chemotactic_sensitivities.end() ,
			 	parameters.begin()+first_chemotaxis_index ); 

	// cell-cell adhesion 
	static int cca_index = find_behavior_index("cell-cell adhesion"); 
	parameters[cca_index] = pCD->phenotype.mechanics.cell_cell_adhesion_strength; 

	// cell-cell "springs"
	static int cca_spring_index = find_behavior_index("cell-cell adhesion elastic constant"); 
	parameters[cca_spring_index] = pCD->phenotype.mechanics.attachment_elastic_constant; 

    // cell adhesion affinities 
	static std::string search_for1 = "adhesive affinity to " + cell_definitions_by_type[0]->name ; 
	static int first_affinity_index = find_behavior_index( search_for1 ); 
	std::copy(  pCD->phenotype.mechanics.cell_adhesion_affinities.begin(), 
				pCD->phenotype.mechanics.cell_adhesion_affinities.end() ,
				parameters.begin()+first_affinity_index ); 

	// max relative maximum adhesion distance 
	static int max_adhesion_distance_index = find_behavior_index("relative maximum adhesion distance"); 
	parameters[max_adhesion_distance_index] = pCD->phenotype.mechanics.relative_maximum_adhesion_distance; 

	// cell-cell repulsion 
	static int ccr_index = find_behavior_index("cell-cell repulsion"); 
	parameters[ccr_index] = pCD->phenotype.mechanics.cell_cell_repulsion_strength; 

	// cell-BM adhesion 
	static int cba_index = find_behavior_index("cell-BM adhesion"); 
	parameters[cba_index] = pCD->phenotype.mechanics.cell_BM_adhesion_strength; 
	
	// cell-BM repulsion 
	static int cbr_index = find_behavior_index("cell-BM repulsion"); 
	parameters[cbr_index] = pCD->phenotype.mechanics.cell_BM_repulsion_strength; 

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell"); 
	parameters[apop_phag_index] = pCD->phenotype.cell_interactions.apoptotic_phagocytosis_rate; 

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell"); 
	parameters[necro_phag_index] = pCD->phenotype.cell_interactions.necrotic_phagocytosis_rate; 

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell"); 
	parameters[other_dead_phag_index] = pCD->phenotype.cell_interactions.other_dead_phagocytosis_rate; 

    // phagocytosis of each live cell type 
	static int first_phagocytosis_index = find_behavior_index( "phagocytose " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCD->phenotype.cell_interactions.live_phagocytosis_rates.begin(), 
				pCD->phenotype.cell_interactions.live_phagocytosis_rates.end(), 
				parameters.begin()+first_phagocytosis_index ); 	

	// attack of each live cell type 
	static int first_attack_index = find_behavior_index( "attack " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCD->phenotype.cell_interactions.attack_rates.begin(), 
				pCD->phenotype.cell_interactions.attack_rates.end(), 
				parameters.begin()+first_attack_index ); 	
 
	// fusion 
	static int first_fusion_index = find_behavior_index( "fuse to " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCD->phenotype.cell_interactions.fusion_rates.begin(), 
				pCD->phenotype.cell_interactions.fusion_rates.end(), 
				parameters.begin()+first_fusion_index ); 	

 	// transformation 
	static int first_transformation_index = find_behavior_index( "transform to " + cell_definitions_by_type[0]->name ); 
	std::copy(  pCD->phenotype.cell_transformations.transformation_rates.begin(), 
				pCD->phenotype.cell_transformations.transformation_rates.end(), 
				parameters.begin()+first_transformation_index ); 	

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	std::copy(  pCD->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities.begin(), 
				pCD->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities.end(), 
				parameters.begin()+first_asymmetric_division_index );

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCell->custom_data.variables.size();  
	if( first_custom_ind >= 0 )
	{
		for( int nc=0; nc < pCell->custom_data.variables.size(); nc++ )
		{ parameters[first_custom_ind+nc] = pCD->custom_data.variables[nc].value; }	 
	}

	// is the cell movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( pCD->is_movable == true )
	{ parameters[movable_ind] = 1; }
	else
	{ parameters[movable_ind] = 0; }

	// vector of immunogenicity behaviors 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
    std::copy( pCD->phenotype.cell_interactions.immunogenicities.begin(),
			   pCD->phenotype.cell_interactions.immunogenicities.end(), 
			   parameters.begin()+start_immunogenicity_ind );  


	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	parameters[attachment_rate_ind] = pCD->phenotype.mechanics.attachment_rate; 

	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	parameters[detachment_rate_ind] = pCD->phenotype.mechanics.detachment_rate; 

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	parameters[max_attachments_ind] = pCD->phenotype.mechanics.maximum_number_of_attachments; 

	// cell damage rate (effector attack)
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	parameters[attack_damage_rate_ind] = pCD->phenotype.cell_interactions.attack_damage_rate; 

	// attack duration
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	parameters[attack_duration_ind] = pCD->phenotype.cell_interactions.attack_duration; 

	//  damage rate (non-attack)
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	parameters[damage_rate_ind] = pCD->phenotype.cell_integrity.damage_rate; 

	//  damage repair rate 
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	parameters[damage_repair_rate_ind] = pCD->phenotype.cell_integrity.damage_repair_rate; 

	return parameters; 
}

double get_single_base_behavior( Cell* pCell , int index )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 	

	if( index < 0 )
	{
		std::cout << "Warning: attempted to get behavior with unknown index " << index << std::endl	
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

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell" ); 
	if( index == apop_phag_index )
	{ return pCD->phenotype.cell_interactions.apoptotic_phagocytosis_rate; }

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell" ); 
	if( index == necro_phag_index )
	{ return pCD->phenotype.cell_interactions.necrotic_phagocytosis_rate; }

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell" ); 
	if( index == other_dead_phag_index )
	{ return pCD->phenotype.cell_interactions.other_dead_phagocytosis_rate; }

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

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	if( index >= first_asymmetric_division_index && index < first_asymmetric_division_index + n )
	{ return pCD->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities[index-first_asymmetric_division_index]; }

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCell->custom_data.variables.size();  
	if( first_custom_ind >= 0 && index >= first_custom_ind && index < max_custom_ind )
	{ return pCD->custom_data.variables[index-first_custom_ind].value; }

	// is the cell movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( index == movable_ind )
	{
		if( pCD->is_movable == true )
		{ return 1.0; }
		else
		{ return 0.0; }
	}

	// vector of immunogenicity behaviors 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
	static int max_immunogenicity_ind = start_immunogenicity_ind + n; 
	if( start_immunogenicity_ind > -1 && index >= start_immunogenicity_ind && index < max_immunogenicity_ind )
	{ return pCD->phenotype.cell_interactions.immunogenicities[index-start_immunogenicity_ind]; }

	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	if( index == attachment_rate_ind )
	{ return pCD->phenotype.mechanics.attachment_rate; }

	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	if( index == detachment_rate_ind )
	{ return pCD->phenotype.mechanics.detachment_rate; }

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	if( index == max_attachments_ind )
	{ return pCD->phenotype.mechanics.maximum_number_of_attachments; }

	// cell attack damage rate (effector attack)
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	if( index == attack_damage_rate_ind )
	{ return pCD->phenotype.cell_interactions.attack_damage_rate; }

	// cell attack duration
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	if( index == attack_duration_ind )
	{ return pCD->phenotype.cell_interactions.attack_duration; }

	// cell damage rate (non-effector)
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	if( index == damage_rate_ind )
	{ return pCD->phenotype.cell_integrity.damage_rate; }

	// cell damage repair rate (non-effector)
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	if( index == damage_repair_rate_ind )
	{ return pCD->phenotype.cell_integrity.damage_repair_rate; }

	return -1; 
}

double get_single_base_behavior( Cell_Definition* pCD , int index )
{
	static int m = microenvironment.number_of_densities(); 
	static int n = cell_definition_indices_by_name.size(); 

	// Cell_Definition* pCD = find_cell_definition( pCell->type_name ); 	

	if( index < 0 )
	{
		std::cout << "Warning: attempted to get behavior with unknown index " << index << std::endl	
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

	// apoptotic cell phagocytosis
	static int apop_phag_index = find_behavior_index("phagocytose apoptotic cell" ); 
	if( index == apop_phag_index )
	{ return pCD->phenotype.cell_interactions.apoptotic_phagocytosis_rate; }

	// necrotic cell phagocytosis
	static int necro_phag_index = find_behavior_index("phagocytose necrotic cell" ); 
	if( index == necro_phag_index )
	{ return pCD->phenotype.cell_interactions.necrotic_phagocytosis_rate; }

	// other dead cell phagocytosis
	static int other_dead_phag_index = find_behavior_index("phagocytose other dead cell" ); 
	if( index == other_dead_phag_index )
	{ return pCD->phenotype.cell_interactions.other_dead_phagocytosis_rate; }

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

	// asymmetric division
	static int first_asymmetric_division_index = find_behavior_index( "asymmetric division to " + cell_definitions_by_type[0]->name );
	if( index >= first_asymmetric_division_index && index < first_asymmetric_division_index + n )
	{ return pCD->phenotype.cycle.asymmetric_division.asymmetric_division_probabilities[index-first_asymmetric_division_index]; }

	// custom behavior
	static int first_custom_ind = find_behavior_index( "custom 0"); 
	static int max_custom_ind = first_custom_ind + pCD->custom_data.variables.size();  
	if( first_custom_ind >= 0 && index >= first_custom_ind && index < max_custom_ind )
	{ return pCD->custom_data.variables[index-first_custom_ind].value; }

	// is the cell movable / not movable 
	static int movable_ind = find_behavior_index( "is_movable"); 
	if( index == movable_ind )
	{
		if( pCD->is_movable == true )
		{ return 1.0; }
		else
		{ return 0.0; }
	}

	// vector of immunogenicity behaviors 
	static int start_immunogenicity_ind = find_behavior_index( "immunogenicity to " + cell_definitions_by_type[0]->name ); 
	static int max_immunogenicity_ind = start_immunogenicity_ind + n; 
	if( start_immunogenicity_ind > -1 && index >= start_immunogenicity_ind && index < max_immunogenicity_ind )
	{ return pCD->phenotype.cell_interactions.immunogenicities[index-start_immunogenicity_ind]; }

	// set cell attachment rate  
	static int attachment_rate_ind = find_behavior_index( "cell attachment rate"); 
	if( index == attachment_rate_ind )
	{ return pCD->phenotype.mechanics.attachment_rate; }

	// set cell detachment rate  
	static int detachment_rate_ind = find_behavior_index( "cell detachment rate"); 
	if( index == detachment_rate_ind )
	{ return pCD->phenotype.mechanics.detachment_rate; }

	// maximum number of cell attachments 
	static int max_attachments_ind = find_behavior_index( "maximum number of cell attachments"); 
	if( index == max_attachments_ind )
	{ return pCD->phenotype.mechanics.maximum_number_of_attachments; }

	// cell attack damage rate (effector attack)
	static int attack_damage_rate_ind = find_behavior_index( "attack damage rate"); 
	if( index == attack_damage_rate_ind )
	{ return pCD->phenotype.cell_interactions.attack_damage_rate; }

	// cell attack duration (effector attack)
	static int attack_duration_ind = find_behavior_index( "attack duration"); 
	if( index == attack_duration_ind )
	{ return pCD->phenotype.cell_interactions.attack_duration; }

	// damage rate (non-effector)
	static int damage_rate_ind = find_behavior_index( "damage rate"); 
	if( index == damage_rate_ind )
	{ return pCD->phenotype.cell_integrity.damage_rate; }

	// damage repair rate (non-effector)
	static int damage_repair_rate_ind = find_behavior_index( "damage repair rate"); 
	if( index == damage_repair_rate_ind )
	{ return pCD->phenotype.cell_integrity.damage_repair_rate; }

	return -1; 
}

double get_single_base_behavior( Cell* pCell , std::string name )
{ return get_single_base_behavior(pCell,find_behavior_index(name)); }

double get_single_base_behavior( Cell_Definition* pCD , std::string name )
{ return get_single_base_behavior(pCD,find_behavior_index(name)); }

std::vector<double> get_base_behaviors( Cell* pCell , std::vector<int> indices )
{
	std::vector<double> parameters( indices.size() , 0.0 ); 
	for( int n=0 ; n < indices.size(); n++ )
	{ parameters[n] = get_single_base_behavior(pCell,indices[n]); }
	return parameters; 
}

std::vector<double> get_base_behaviors( Cell* pCell , std::vector<std::string> names )
{
	std::vector<double> parameters( names.size() , 0.0 ); 
	for( int n=0 ; n < names.size(); n++ )
	{ parameters[n] = get_single_base_behavior(pCell,names[n]); }
	return parameters; 
}

};
