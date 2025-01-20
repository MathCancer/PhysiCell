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

#ifndef __PhysiCell_digital_cell_line_h__
#define __PhysiCell_digital_cell_line_h__

#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <ostream>
#include <istream> 
#include <unordered_map>

#include "../BioFVM/pugixml.hpp"
#include "../BioFVM/BioFVM_basic_agent.h"

namespace PhysiCell{

class Microenvironment_Sample
{
public:
	std::vector<std::string> variables; 
	std::vector<double> densities; 
};

class Geometry{
public:
	double radius; 
	double nuclear_radius; 
	double surface_area; 
	std::vector<double> polarization; 
	bool polarized; 
};

class Volume{
public:
	double total;
	double solid;
	double fluid;
	double fluid_fraction; 
	
	double nuclear;
	double nuclear_fluid;
	double nuclear_solid; 

	double cytoplasmic;
	double cytoplasmic_fluid; 
	double cytoplasmic_solid; 
	
	double cytoplasmic_biomass_change_rate; 
	double nuclear_biomass_change_rate; 
	double fluid_change_rate;

	
	double target_solid_cytoplasmic;
	double target_solid_nuclear;
	double target_fluid_fraction;
	
	double cytoplasmic_to_nuclear_fraction;
	
	double rupture_threshold;
	void divide( void );
	void multiply_by_ratio(double);
	double calcified_fraction;
};

class Phase{
public:
	int code; 
	std::string name; 
	
	double elapsed_time; 
	double duration; 	
	double birth_rate; 
	double death_rate; 
	double necrosis_rate=0;
	double calcification_rate; 
	int death_type; 
	double arrest_density; 
	double volume_change_timescale_N;
	double volume_change_timescale_C;
	double volume_change_timescale_F;
};

class Cycle{
public: 
	int cycle_model; 
	std::string cycle_model_name; 
	
	std::vector<Phase> phases; 
};

/* Commented out on 5/2/2016
class Death{
public: 
	int death_model; 
	double duration; 
	
	double cytoplasmic_biomass_loss_timescale;
	double nuclear_biomass_loss_timescale;
	double fluid_loss_timescale; 
	double adhesion_loss_timescale; 
	
	double calcification_rate; 
};*/

class Motility{
public:
	std::vector<double> gradient_correlations; 
	double mean_speed; 
};

class Uptake_Rates{
public:
	std::vector<std::string> variables; 
	std::vector<double> rates; 
}; 

class Secretion_Rates{
public:
	std::vector<std::string> variables; 
	std::vector<double> rates; 
};

class Secretion_Saturation_Densities{
public: 
	std::vector<std::string> variables; 
	std::vector<double> densities; 
}; 

class Full_Phenotype{
private: 
	std::unordered_map<int, int> * phase_indices_map;
public:
	// std::vector<Phase> phase; 
	int current_phase_index; 
	bool phase_model_initialized; //should this be here?
	// Phase current_phase;
	Cycle cycle; 
	
	Geometry geometry; 
	Volume volume; 	
	// Death apoptosis; 
	// Death necrosis; 
	Motility motility; 
	Uptake_Rates uptake_rates; 
	Secretion_Rates secretion_rates;
	Secretion_Saturation_Densities saturation_densities; 
	void set_current_phase(int phase_code); // set current_phase based on a given phase_code
	int get_phase_index(int phase_code); //return the phase_index of a given phase in cycle.phases
	void update_volume_change_rates();
	void update_radius();
	void set_phase_maps(std::unordered_map<int,int> * phase_map);
	std::unordered_map<int,int> * get_phase_maps(void);
	
	int get_current_phase_code(void); 
};

class Cell_Line{
public:
	std::string name; 
	std::vector<std::string> phenotype_names; 
	std::vector<Microenvironment_Sample> microenvironment_samples; 
	std::vector<Full_Phenotype> phenotypes; 
	
	void display_information( std::ostream& os ); 
};

// create some standard generic digital cell lines 
void set_cancer_cell_line( Cell_Line& DCL );
void set_cancer_cell_line_MCF7( Cell_Line& DCL );
void set_endothelial_cell_line( Cell_Line& DCL );  
	
}; 

#endif
