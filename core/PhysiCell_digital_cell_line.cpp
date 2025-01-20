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

#include "PhysiCell_digital_cell_line.h"
#include "PhysiCell_constants.h"

#include <cmath>
#include <map>

using namespace BioFVM;

namespace PhysiCell
{

void set_cancer_cell_line( Cell_Line& DCL )
{
	DCL.name = "cancer";
	
	// create 6 phenotype: normoxic, physoxic, hypoxic, hypoxic_glycolytic, physoxic_glycolytic, normoxic_glycolytic 
	
	DCL.name = "MCF7";
	
	// create 6 phenotype: normoxic, physioxic, hypoxic, hypoxic_glycolytic, physioxic_glycolytic, normoxic_glycolytic 
	
	DCL.phenotype_names.resize(7); 
	DCL.phenotypes.resize(7); 
	DCL.microenvironment_samples.resize(7); 
	
	static double _pi_ = 3.141592653589793;
	
	int i = 0; 
	// set normoxic (21% O2) phenotype
	std::unordered_map<int,int> phase_hashmap;//=new std::unordered_map<int,int>;
	
	// sizing information 
	DCL.phenotype_names[i] = "viable";
	DCL.microenvironment_samples[i].variables.resize( 1 ) ; 
	DCL.microenvironment_samples[i].variables[0] = "oxygen"; 
	DCL.microenvironment_samples[i].densities.resize( 1 ) ; 
	DCL.microenvironment_samples[i].densities[0] = 7.2; 
	
	DCL.phenotypes[i].geometry.radius = 8.413;  
	DCL.phenotypes[i].geometry.nuclear_radius = 5.215;
	DCL.phenotypes[i].geometry.surface_area = 4.0*_pi_*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].geometry.polarization.resize( 3 , 0.0 );  
	DCL.phenotypes[i].geometry.polarized = false; 	
	
	DCL.phenotypes[i].volume.total = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].volume.fluid_fraction = 0.75;
	DCL.phenotypes[i].volume.fluid = DCL.phenotypes[i].volume.fluid_fraction * DCL.phenotypes[i].volume.total; 
	DCL.phenotypes[i].volume.solid = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.fluid;
	
	DCL.phenotypes[i].volume.nuclear = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius; 
	DCL.phenotypes[i].volume.nuclear_fluid = DCL.phenotypes[i].volume.fluid_fraction * DCL.phenotypes[i].volume.nuclear;
	DCL.phenotypes[i].volume.nuclear_solid = DCL.phenotypes[i].volume.nuclear -  DCL.phenotypes[i].volume.nuclear_fluid;
		
	DCL.phenotypes[i].volume.cytoplasmic = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.nuclear; 
	DCL.phenotypes[i].volume.cytoplasmic_fluid = DCL.phenotypes[i].volume.fluid_fraction * 	DCL.phenotypes[i].volume.cytoplasmic; 
	DCL.phenotypes[i].volume.cytoplasmic_solid = DCL.phenotypes[i].volume.cytoplasmic -  DCL.phenotypes[i].volume.cytoplasmic_fluid;  
	
	// the following values are set by Farzin for MCF7
	DCL.phenotypes[i].volume.target_fluid_fraction = 0.75; 
	DCL.phenotypes[i].volume.target_solid_nuclear = 148.5; 
	DCL.phenotypes[i].volume.target_solid_cytoplasmic = 474.9692; 
	DCL.phenotypes[i].volume.cytoplasmic_to_nuclear_fraction= 3.1984;
	
	
	// cell cycle information 
	DCL.phenotypes[i].cycle.cycle_model = PhysiCell_constants::live_apoptotic_cycle_model; 
	DCL.phenotypes[i].cycle.cycle_model_name = "live_apoptotic"; 
	DCL.phenotypes[i].cycle.phases.resize( 4 );
	DCL.phenotypes[i].cycle.phases[0].code = PhysiCell_constants::live; 
	DCL.phenotypes[i].cycle.phases[0].name = "live"; 
	DCL.phenotypes[i].cycle.phases[0].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[0].duration = 17.100027 * 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 1.0 / DCL.phenotypes[i].cycle.phases[0].duration; 
	DCL.phenotypes[i].cycle.phases[0].death_rate = 0.026133 / 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[0].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_N = 10.49*60.0; 
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_C = 15.83*60.0; 
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_F = 1*60.0; 
	phase_hashmap[(int)PhysiCell_constants::live]=0;
	
	
	DCL.phenotypes[i].cycle.phases[1].code = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[1].name = "apoptotic"; 
	DCL.phenotypes[i].cycle.phases[1].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[1].duration = 8.6 * 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[1].birth_rate = 0; 
	DCL.phenotypes[i].cycle.phases[1].death_rate = 0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[1].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[1].arrest_density = 9e99; // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_N = 9*60.0; 
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_C = 1*60.0; 
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_F = 1*60.0; 	
	phase_hashmap[(int)PhysiCell_constants::apoptotic]=1;

	DCL.phenotypes[i].cycle.phases[2] = DCL.phenotypes[i].cycle.phases[1]; 
	DCL.phenotypes[i].cycle.phases[2].code = PhysiCell_constants::necrotic_swelling; 
	DCL.phenotypes[i].cycle.phases[2].name = "necrotic swelling";
	DCL.phenotypes[i].cycle.phases[2].duration = 3.0 * 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[2].death_type = PhysiCell_constants::necrotic; 
	phase_hashmap[(int)PhysiCell_constants::necrotic_swelling]=2;
	
	DCL.phenotypes[i].cycle.phases[3] = DCL.phenotypes[i].cycle.phases[2]; 
	DCL.phenotypes[i].cycle.phases[3].code = PhysiCell_constants::necrotic_lysed; 
	DCL.phenotypes[i].cycle.phases[3].name = "necrotic lysed";
	DCL.phenotypes[i].cycle.phases[3].duration = 45.0 * 24.0 * 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[3].death_type = PhysiCell_constants::necrotic; 
	phase_hashmap[(int)PhysiCell_constants::necrotic_lysed]=3;
	
	DCL.phenotypes[i].set_phase_maps(new std::unordered_map<int,int>(phase_hashmap));
	
	i++; // i = 1 
		
	// set physoxic (55 mmHg) phenotype
	DCL.phenotype_names[i] = "physoxic";
	  // adjust microenvironment 
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 55.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 3.1441e-04; 
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / DCL.phenotypes[i].cycle.phases[0].birth_rate; // minutes (MDA-MB-231) 
	
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 2; 
	// set hypoxic (8 mmHg) phenotype
	DCL.phenotype_names[i] = "hypoxic";
	  // adjust microenvironment 
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 8.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 1.8864e-05; // 0.9 * DCL.phenotypes[0].cycle.phases[0].birth_rate ;
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / DCL.phenotypes[i].cycle.phases[0].birth_rate; // minutes (MDA-MB-231) 

	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);

	
	i++; // i = 3 
	// set necrotic (5 mmHg) phenotype 
	DCL.phenotype_names[i] = "necrotic";
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 5.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 0; // 0.9 * DCL.phenotypes[0].cycle.phases[0].birth_rate ;
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / ( DCL.phenotypes[i].cycle.phases[0].birth_rate + 1e-16); // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[0].death_rate = 1.0; // cells survive 1 hour in this condition on average in this model 
	DCL.phenotypes[i].cycle.phases[0].death_type = PhysiCell_constants::necrotic; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);

	
	i++; // i = 4 
	// set hypoxic glycolytic phenotype 
		  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	DCL.phenotype_names[i] = "hypoxic_glycolytic";
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 8.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 5 
	// set physoxic glycolytic phenotype 
	DCL.phenotype_names[i] = "physoxic_glycolytic";
	// adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 55.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 6 
	// set normoxic glycolytic phenotype 
	DCL.phenotype_names[i] = "normoxic_glycolytic";
	// adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 160.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
}	

void set_cancer_cell_line_MCF7( Cell_Line& DCL )
{
	DCL.name = "MCF7";
	
	// create 6 phenotype: normoxic, physioxic, hypoxic, hypoxic_glycolytic, physioxic_glycolytic, normoxic_glycolytic 
	
	DCL.phenotype_names.resize(7); 
	DCL.phenotypes.resize(7); 
	DCL.microenvironment_samples.resize(7); 
	
	static double _pi_ = 3.141592653589793;
	
	int i = 0; 
	// set normoxic (21% O2) phenotype
	std::unordered_map<int,int> phase_hashmap;//=new std::unordered_map<int,int>;
	
	// sizing information 
	DCL.phenotype_names[i] = "viable";
	DCL.microenvironment_samples[i].variables.resize( 1 ) ; 
	DCL.microenvironment_samples[i].variables[0] = "oxygen"; 
	DCL.microenvironment_samples[i].densities.resize( 1 ) ; 
	DCL.microenvironment_samples[i].densities[0] = 7.2; 
	
	DCL.phenotypes[i].geometry.radius = 8.413;  
	DCL.phenotypes[i].geometry.nuclear_radius = 5.052;
	DCL.phenotypes[i].geometry.surface_area = 4.0*_pi_*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].geometry.polarization.resize( 3 , 0.0 );  
	DCL.phenotypes[i].geometry.polarized = false; 	
	
	DCL.phenotypes[i].volume.total = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].volume.fluid_fraction = 0.75;
	DCL.phenotypes[i].volume.fluid = DCL.phenotypes[i].volume.fluid_fraction * DCL.phenotypes[i].volume.total; 
	DCL.phenotypes[i].volume.solid = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.fluid;
	
	DCL.phenotypes[i].volume.nuclear = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius; 
	DCL.phenotypes[i].volume.nuclear_fluid = DCL.phenotypes[i].volume.fluid_fraction * DCL.phenotypes[i].volume.nuclear;
	DCL.phenotypes[i].volume.nuclear_solid = DCL.phenotypes[i].volume.nuclear -  DCL.phenotypes[i].volume.nuclear_fluid;
		
	DCL.phenotypes[i].volume.cytoplasmic = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.nuclear; 
	DCL.phenotypes[i].volume.cytoplasmic_fluid = DCL.phenotypes[i].volume.fluid_fraction * 	DCL.phenotypes[i].volume.cytoplasmic; 
	DCL.phenotypes[i].volume.cytoplasmic_solid = DCL.phenotypes[i].volume.cytoplasmic -  DCL.phenotypes[i].volume.cytoplasmic_fluid;  
	
	// the following values are set by Farzin for MCF7
	DCL.phenotypes[i].volume.target_fluid_fraction = 0.75; 
	DCL.phenotypes[i].volume.target_solid_nuclear = 135; 
	DCL.phenotypes[i].volume.cytoplasmic_to_nuclear_fraction= 3.6;
	DCL.phenotypes[i].volume.target_solid_cytoplasmic = DCL.phenotypes[i].volume.cytoplasmic_to_nuclear_fraction* DCL.phenotypes[i].volume.target_solid_nuclear; 
	
	
	// cell cycle information 
	DCL.phenotypes[i].cycle.cycle_model = PhysiCell_constants::advanced_Ki67_cycle_model; 
	DCL.phenotypes[i].cycle.cycle_model_name = "advanced_Ki67"; 
	DCL.phenotypes[i].cycle.phases.resize( 7 );
	DCL.phenotypes[i].cycle.phases[0].code = PhysiCell_constants::Ki67_positive_premitotic; 
	DCL.phenotypes[i].cycle.phases[0].name = "Ki67_positive_premitotic"; 
	DCL.phenotypes[i].cycle.phases[0].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[0].duration = 13* 60.0; // 
	DCL.phenotypes[i].cycle.phases[0].death_rate = 0.00105  / 60; // arbitrarily set
	// DCL.phenotypes[i].cycle.phases[0].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_N = 9.1*60.0; 
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_C = 11.1*60.0; 
	DCL.phenotypes[i].cycle.phases[0].volume_change_timescale_F = 1*60.0; 
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[0].code]=0;
	
	DCL.phenotypes[i].cycle.phases[1].code = PhysiCell_constants::Ki67_positive_postmitotic; 
	DCL.phenotypes[i].cycle.phases[1].name = "Ki67_positive_postmitotic"; 
	DCL.phenotypes[i].cycle.phases[1].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[1].duration = 2.5 * 60.0; // 
	DCL.phenotypes[i].cycle.phases[1].birth_rate = 0; 
	DCL.phenotypes[i].cycle.phases[1].death_rate = 0.00105  / 60; // arbitrarily set
	// DCL.phenotypes[i].cycle.phases[1].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[1].arrest_density = 9e99; // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_N = 9.1*60.0; 
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_C = 11.1*60.0; 
	DCL.phenotypes[i].cycle.phases[1].volume_change_timescale_F = 1*60.0; 
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[1].code]=1;
	
	DCL.phenotypes[i].cycle.phases[2].code = PhysiCell_constants::Ki67_negative; 
	DCL.phenotypes[i].cycle.phases[2].name = "Ki67_negative"; 
	DCL.phenotypes[i].cycle.phases[2].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[2].duration = 74.35 * 60; // 
	DCL.phenotypes[i].cycle.phases[2].birth_rate = 0; 
	DCL.phenotypes[i].cycle.phases[2].death_rate = 0.00105  / 60; // arbitrarily set
	// DCL.phenotypes[i].cycle.phases[2].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[2].arrest_density = 9e99; // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[2].volume_change_timescale_N = 9.1*60.0; 
	DCL.phenotypes[i].cycle.phases[2].volume_change_timescale_C = 11.1*60.0; 
	DCL.phenotypes[i].cycle.phases[2].volume_change_timescale_F = 1*60.0; 
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[2].code]=2;
	
	
	DCL.phenotypes[i].cycle.phases[3].code = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[3].name = "apoptotic"; 
	DCL.phenotypes[i].cycle.phases[3].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[3].duration = 8.6 * 60.0; // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[3].birth_rate = 0; 
	DCL.phenotypes[i].cycle.phases[3].death_rate = 0; // minutes (MDA-MB-231) 
	// DCL.phenotypes[i].cycle.phases[3].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[3].arrest_density = 9e99; // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[3].volume_change_timescale_N = 8.6*60.0; 
	DCL.phenotypes[i].cycle.phases[3].volume_change_timescale_C = 3*60.0; 
	DCL.phenotypes[i].cycle.phases[3].volume_change_timescale_F = 1*60.0; 	
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[3].code]=3;

	// DCL.phenotypes[i].cycle.phases[4] = DCL.phenotypes[i].cycle.phases[1]; 
	DCL.phenotypes[i].cycle.phases[4].code = PhysiCell_constants::necrotic_swelling; 
	DCL.phenotypes[i].cycle.phases[4].name = "necrotic swelling";
	DCL.phenotypes[i].cycle.phases[4].duration = 3.0 * 60.0; // minutes (MDA-MB-231) 
	// DCL.phenotypes[i].cycle.phases[4].death_type = PhysiCell_constants::necrotic; 
	DCL.phenotypes[i].cycle.phases[4].volume_change_timescale_N = 230*60.0; 
	DCL.phenotypes[i].cycle.phases[4].volume_change_timescale_C = 936.2*60.0; 
	DCL.phenotypes[i].cycle.phases[4].volume_change_timescale_F = 4.47*60.0; 
	DCL.phenotypes[i].cycle.phases[4].calcification_rate = 0.0042/60;
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[4].code]=4;
	
	// DCL.phenotypes[i].cycle.phases[5] = DCL.phenotypes[i].cycle.phases[2]; 
	DCL.phenotypes[i].cycle.phases[5].code = PhysiCell_constants::necrotic_lysed; 
	DCL.phenotypes[i].cycle.phases[5].name = "necrotic lysed";
	DCL.phenotypes[i].cycle.phases[5].duration = 45.0 * 24.0 * 60.0; // minutes (MDA-MB-231) 
	// DCL.phenotypes[i].cycle.phases[5].death_type = PhysiCell_constants::necrotic; 
	DCL.phenotypes[i].cycle.phases[5].volume_change_timescale_N = 230*60.0; 
	DCL.phenotypes[i].cycle.phases[5].volume_change_timescale_C = 936.2*60.0; 
	DCL.phenotypes[i].cycle.phases[5].volume_change_timescale_F = 59*60.0; 
	DCL.phenotypes[i].cycle.phases[5].calcification_rate = 0.0042/60;
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[5].code]=5;
	
	DCL.phenotypes[i].cycle.phases[6].code = PhysiCell_constants::live; 
	DCL.phenotypes[i].cycle.phases[6].name = "live"; 
	DCL.phenotypes[i].cycle.phases[6].elapsed_time = 0.0;
	DCL.phenotypes[i].cycle.phases[6].duration = 17.1* 60.0; // 
	DCL.phenotypes[i].cycle.phases[6].birth_rate= 1.0 / DCL.phenotypes[i].cycle.phases[6].duration;
	DCL.phenotypes[i].cycle.phases[6].death_rate = 0.00105  / 60; // arbitrarily set
	// DCL.phenotypes[i].cycle.phases[0].death_type = PhysiCell_constants::apoptotic; 
	DCL.phenotypes[i].cycle.phases[6].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	DCL.phenotypes[i].cycle.phases[6].volume_change_timescale_N = 9.1*60.0; 
	DCL.phenotypes[i].cycle.phases[6].volume_change_timescale_C = 11.1*60.0; 
	DCL.phenotypes[i].cycle.phases[6].volume_change_timescale_F = 1*60.0; 
	phase_hashmap[(int)DCL.phenotypes[i].cycle.phases[6].code]=6;
		
	DCL.phenotypes[i].set_phase_maps(new std::unordered_map<int,int>(phase_hashmap));
	

	i++; // i = 1 
		
	// set physoxic (55 mmHg) phenotype
	DCL.phenotype_names[i] = "physioxic";
	  // adjust microenvironment 
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 55.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 3.1441e-04; // 0.9 * DCL.phenotypes[0].cycle.phases[0].birth_rate ;
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / DCL.phenotypes[i].cycle.phases[0].birth_rate; // minutes (MDA-MB-231) 
	
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 2; 
	// set hypoxic (8 mmHg) phenotype
	DCL.phenotype_names[i] = "hypoxic";
	  // adjust microenvironment 
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 8.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 1.8864e-05; // 0.9 * DCL.phenotypes[0].cycle.phases[0].birth_rate ;
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / DCL.phenotypes[i].cycle.phases[0].birth_rate; // minutes (MDA-MB-231) 

	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);

	
	i++; // i = 3 
	// set necrotic (5 mmHg) phenotype 
	DCL.phenotype_names[i] = "necrotic";
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 5.0; 
	  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	  // birth rate 
	DCL.phenotypes[i].cycle.phases[0].birth_rate = 0; // 0.9 * DCL.phenotypes[0].cycle.phases[0].birth_rate ;
	DCL.phenotypes[i].cycle.phases[0].duration = 1.0 / ( DCL.phenotypes[i].cycle.phases[0].birth_rate + 1e-16); // minutes (MDA-MB-231) 
	DCL.phenotypes[i].cycle.phases[0].death_rate = 1.0; // cells survive 1 hour in this condition on average in this model 
	DCL.phenotypes[i].cycle.phases[0].death_type = PhysiCell_constants::necrotic; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);

	
	i++; // i = 4 
	// set hypoxic glycolytic phenotype 
		  // adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	DCL.phenotype_names[i] = "hypoxic_glycolytic";
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 8.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 5 
	// set physoxic glycolytic phenotype 
	DCL.phenotype_names[i] = "physioxic_glycolytic";
	// adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 55.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
	
	i++; // i = 6 
	// set normoxic glycolytic phenotype 
	DCL.phenotype_names[i] = "normoxic_glycolytic";
	// adjust phenotype 
	DCL.phenotypes[i] = DCL.phenotypes[0];
	
	DCL.microenvironment_samples[i] = DCL.microenvironment_samples[0];
	DCL.microenvironment_samples[i].densities[0] = 160.0; 
	DCL.phenotypes[i].set_phase_maps(&phase_hashmap);
}	


void Full_Phenotype::set_current_phase(int phase_code)
{
	current_phase_index=get_phase_index(phase_code);
	phase_model_initialized=false;
}

int Full_Phenotype::get_phase_index(int phase_code)
{
	return (*phase_indices_map)[phase_code];
}

void Full_Phenotype::set_phase_maps(std::unordered_map<int,int> * phase_map)
{
	phase_indices_map=phase_map;
}

void Full_Phenotype::update_volume_change_rates()
{
	double divider= -log(.05);	
	volume.cytoplasmic_biomass_change_rate= divider/ cycle.phases[current_phase_index].volume_change_timescale_C;	
	volume.nuclear_biomass_change_rate= divider/cycle.phases[current_phase_index].volume_change_timescale_N;	
	volume.fluid_change_rate= divider/cycle.phases[current_phase_index].volume_change_timescale_F;
// std::cout<<	cycle.phases[current_phase_index].volume_change_timescale_F<<", "<<divider<<std::endl;
}

void Full_Phenotype::update_radius()
{
	geometry.radius = pow( 0.238732414638*volume.total , 0.33333333333333333333333333333333 ); 
	geometry.nuclear_radius = pow( 0.238732414638*volume.nuclear , 0.33333333333333333333333333333333 );   
	geometry.surface_area = 4* PhysiCell_constants::pi * geometry.radius * geometry.radius;
}

void Volume::divide()
{
	multiply_by_ratio(0.5);
}

void Volume:: multiply_by_ratio(double ratio)
{
	// std::cout<<ratio<<std::endl;
	total*=ratio;
	solid*=ratio;
	fluid*=ratio;
	nuclear*=ratio;
	nuclear_fluid*=ratio;
	nuclear_solid*=ratio;
	cytoplasmic*=ratio;
	cytoplasmic_fluid*=ratio;
	cytoplasmic_solid*=ratio;
}

std::unordered_map<int,int> * Full_Phenotype::get_phase_maps()
{
	return phase_indices_map;
}

void Cell_Line::display_information( std::ostream& os )
{
	os << "Cell line: " << name << std::endl; 
	for( int i=0; i < phenotype_names.size() ; i++ )
	{
		os << "phenotype " << i << ": " << phenotype_names[i] << std::endl; 
	}
	return;
} 

void set_endothelial_cell_line( Cell_Line& DCL )
{
// high suppressor 

// low VEGF 
	
// high VEGF, low suppressor

// high VEGF, high suppressor

// low VEGF
	// needs completion
	
	DCL.name = "endothelial";
		
	DCL.phenotype_names.resize(1); 
	DCL.phenotypes.resize(1); 
	DCL.microenvironment_samples.resize(1); 
	
	static double _pi_ = 3.141592653589793;
	
	int i = 0; 
	// set normoxic (21% O2) phenotype
	std::unordered_map<int,int> phase_hashmap;//=new std::unordered_map<int,int>;
	
	// sizing information 
	DCL.phenotype_names[i] = "normal";
	DCL.microenvironment_samples[i].variables.resize( 1 ) ; 
	DCL.microenvironment_samples[i].variables[0] = "oxygen"; 
	DCL.microenvironment_samples[i].densities.resize( 1 ) ; 
	DCL.microenvironment_samples[i].densities[0] = 160.0; 
	
	
	DCL.phenotypes[i].geometry.radius = 10.0; 
	DCL.phenotypes[i].geometry.nuclear_radius = 8.0; 
	DCL.phenotypes[i].geometry.surface_area = 4.0*_pi_*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].geometry.polarization.resize( 3 , 0.0 );  
	DCL.phenotypes[i].geometry.polarized = false; 	
	
	DCL.phenotypes[i].volume.total = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius*DCL.phenotypes[i].geometry.radius; 
	DCL.phenotypes[i].volume.fluid_fraction = 0.7; 
	DCL.phenotypes[i].volume.fluid = DCL.phenotypes[i].volume.fluid_fraction * DCL.phenotypes[i].volume.total; 
	DCL.phenotypes[i].volume.solid = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.fluid;
	
	DCL.phenotypes[i].volume.nuclear = 4.0*_pi_/3.0*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius*DCL.phenotypes[i].geometry.nuclear_radius; 
	DCL.phenotypes[i].volume.nuclear_fluid = DCL.phenotypes[i].volume.fluid_fraction * 	DCL.phenotypes[i].volume.nuclear;
	DCL.phenotypes[i].volume.nuclear_solid = DCL.phenotypes[i].volume.nuclear -  DCL.phenotypes[i].volume.nuclear_fluid;
		
	DCL.phenotypes[i].volume.cytoplasmic = DCL.phenotypes[i].volume.total - DCL.phenotypes[i].volume.nuclear; 
	DCL.phenotypes[i].volume.cytoplasmic_fluid = DCL.phenotypes[i].volume.fluid_fraction * 	DCL.phenotypes[i].volume.cytoplasmic; 
	DCL.phenotypes[i].volume.cytoplasmic_solid = DCL.phenotypes[i].volume.cytoplasmic -  DCL.phenotypes[i].volume.cytoplasmic_fluid;  

}

int Full_Phenotype::get_current_phase_code(void)
{
	return cycle.phases[current_phase_index].code; 
}

};
