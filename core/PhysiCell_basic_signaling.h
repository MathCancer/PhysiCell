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
 
#include <vector>
#include <string>

#ifndef __PhysiCell_basic_signaling__
#define __PhysiCell_basic_signaling__

#include "./PhysiCell_constants.h" 
#include "./PhysiCell_phenotype.h" 
#include "./PhysiCell_Cell.h" 

namespace PhysiCell{
	
// std::vector<std::string> 

double Hill_response_function( double s, double half_max , double hill_power ); // done
// increases from 0 (at s_min) to 1 (at s_max)
double linear_response_function( double s, double s_min , double s_max ); // done 
// decreases from 1 (at s_min) to 0 (at s_max)
double decreasing_linear_response_function( double s, double s_min , double s_max ); // done 


/*
void setup_signal_response( void ); 

extern std::map<std::string,int> signal_to_int; 
extern std::map<std::string,int> response_to_int; 

extern std::map<int,std::string> int_to_signal; 
extern std::map<int,std::string> int_to_response; 
	
class Signal_Response
{
 private:
 	// find the microenvironment, get number of substrates 
	int number_of_substrates; 
	int number_of_cell_types; 
	int number_of_signals;
	int number_of_responses;

 public: 
	std::vector<double> scales; 
	std::vector< std::vector<double> > up_response_weights; 
	std::vector< std::vector<double> > down_response_weights; 
	
	std::vector<double> hill_powers; 
	std::vector<double> up_half_maxes; 
	std::vector<double> down_half_maxes; 
	
	std::vector<double> base_parameters; 
	std::vector<double> max_up_responses; 
	std::vector<double> max_down_responses; 
	
	Cell_Definition* pCD; 
	std::string cell_type; 

	Signal_Response(); 

	// setup 
	void sync_to_cell_definition( Cell_Definition& cd ); // done 

	void add_rule( std::string signal, std::string direction , std::string response, double max_response ); // done 
	void add_rule( std::string signal, std::string direction , std::string response ); // done 

	void set_response_parameters( int nResponse , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power ); // done
	void set_response_parameters( std::string response , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power ); // done 

	void set_response_parameters( int nResponse , double max_down_response , double max_up_response ); // done
	void set_response_parameters( std::string response , double max_down_response , double max_up_response  ); // done 

	void set_response_parameters_fold_changes( int nResponse , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power ); // done
	void set_response_parameters_fold_changes( std::string response , double max_down_response , double max_up_response, 
		double down_halfmax, double up_halfmax, double hill_power ); // done 

	void set_response_parameters_fold_changes( int nResponse , double max_down_response , double max_up_response ); // done
	void set_response_parameters_fold_changes( std::string response , double max_down_response , double max_up_response ); // done 

	// some helpers 

	double& secretion_rate( std::string& name ); 
	double& secretion_target( std::string& name ); 
	double& uptake_rate( std::string& name ); 
	double& export_rate( std::string& name ); 

	//
	void display( void ); // done 

	std::vector<double> construct_signals( Cell* pCell ); // done 
	std::vector<double> run_rules( std::vector<double>& signals ); // done 
	void transfer_updated_parameters( Cell* pCell , std::vector<double>& parameters ); // done 

	void apply_rules( Cell* pCell ); 
};
*/

/*
  need: a registry 
*/

/*
extern std::vector<Signal_Response*> SR_models; 
extern std::map<std::string, Signal_Response*> celldef_to_SR; 
extern std::map<int,Signal_Response*> celldef_int_to_SR; 

Signal_Response* find_SR_model( std::string cell_def_name ); // done 
Signal_Response* find_SR_model( int cell_def_int ); // done 
Signal_Response* find_SR_model( Cell* pCell ); // done 

void create_SR_model( Cell_Definition& cd ); // done 
void register_SR_model( Cell_Definition& cd, Signal_Response& sr ); // pointless? 

void signal_response_phenotype( Cell* pCell, Phenotype& phenotype, double dt ); // done-ish

void signal_response_custom( Cell* pCell, Phenotype& phenotype, double dt ); 
*/	

// signal increases/decreases parameter
// options: hill power
// options: half max

class Integrated_Signal
{
 private:
 public: 
	double base_activity; 
	double max_activity; 
	
	std::vector<double> promoters; 
	std::vector<double> promoter_weights; 
	double promoters_Hill;
	double promoters_half_max; 
	
	std::vector<double> inhibitors; 
	std::vector<double> inhibitor_weights; 
	double inhibitors_Hill;
	double inhibitors_half_max; 
	
	Integrated_Signal();
	void reset( void ); 
	
	void add_signal( char signal_type , double signal , double weight ); 
	void add_signal( char signal_type , double signal );

	double compute_signal( void );
};


}; 

#endif 