/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.1) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.1) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the PhysiCell Project           #
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

#include "PhysiCell_cell.h"
#include "PhysiCell_cell_container.h"
#include "PhysiCell_standard_models.h"
#include "PhysiCell_constants.h"
#include <random>
#include <unordered_map>


using namespace PhysiCell;

void do_nothing( Cell* pCell, double dt)
{
}

bool check_necrosis( Cell* pCell, double dt)
{
	double probability_necrosis = dt*pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].necrosis_rate; 
	if( (pCell->parameters.necrosis_type== PhysiCell_constants::stochastic_necrosis && uniform_random() < probability_necrosis)
		|| 
		(pCell->parameters.necrosis_type== PhysiCell_constants::deterministic_necrosis && 
		(pCell->nearest_density_vector())[PhysiCell_constants::oxygen_index]< pCell->parameters.o2_necrotic  ) ) 
	{
		pCell->phenotype.set_current_phase(PhysiCell_constants::necrotic_swelling); 
		//pCell->phenotype.phase_model_initialized = false; 
		pCell->advance_cell_current_phase = death_necrosis_swelling_model; 
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0; 	
		return true;
	}
	return false;
}

bool check_apoptosis( Cell* pCell, double dt, bool stoc_A)
{
	double probability_apoptosis = dt*pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].death_rate; 
	if( uniform_random() < probability_apoptosis )
	{
		// set the current phase to apoptotic 
		pCell->phenotype.set_current_phase(PhysiCell_constants::apoptotic); 
		//pCell->phenotype.phase_model_initialized = false; 
		pCell->advance_cell_current_phase = stoc_A ? death_apoptosis_model_stochastic: death_apoptosis_model_deterministic; 
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0; 
		return true; 
	}	
	return false;
}

void ki67_basic_cycle_model( Cell* pCell, double dt, bool stoc_K, bool stoc_Q, bool stoc_A)
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(pCell, dt);
	if( is_necrotic )
	{	return; }
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(pCell, dt, stoc_A);
	if( is_apoptotic )
	{ return; }
	
	// K phase
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::Ki67_positive)
	{
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 
			pCell->phenotype.volume.target_solid_nuclear= pCell->phenotype.volume.target_solid_nuclear*2.0;
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
			
		}
		
		double probability_K_Q = dt / pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration;		
		// advance to Q phase? 
		if( (!stoc_K && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration-0.001 ) ||  (stoc_K && uniform_random() < probability_K_Q ) )
		{
			// flag the cell for division 
			pCell->get_container()->flag_cell_for_division( pCell ); 
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;			
			//pCell->phenotype.phase_model_initialized = false; 
			return; 
		}
	}
	
	// Q phase 
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::Ki67_negative )
	{
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 
			pCell->phenotype.volume.target_solid_nuclear= pCell->phenotype.volume.target_solid_nuclear/2.0;
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
		}
		
		// advance to K phase? 
		double probability_Q_K = (dt / pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration); 
		if( (!stoc_Q && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration-0.001 ) ||  (stoc_Q && uniform_random() < probability_Q_K ) )
		{
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_premitotic); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;
			//pCell->phenotype.phase_model_initialized = false; 
			return; 
		}
	}

	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt;  
}

void ki67_basic_cycle_model_deterministic( Cell* pCell, double dt)
{
	ki67_basic_cycle_model( pCell, dt, false, false, false);
}

void ki67_basic_cycle_model_stochastic( Cell* pCell, double dt)
{
	ki67_basic_cycle_model( pCell, dt, true, true, true);
}

void ki67_basic_cycle_model_legacy( Cell* pCell, double dt)
{
	ki67_basic_cycle_model( pCell, dt, false, true, false);
}

void ki67_advanced_cycle_model( Cell* pCell, double dt, bool stoc_K1, bool stoc_K2, bool stoc_Q, bool stoc_A)
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(pCell, dt);
	if (is_necrotic)
		return;
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(pCell, dt, stoc_A);
	if (is_apoptotic)
		return;
	
	// K1 phase
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::Ki67_positive_premitotic )
	{
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 
			pCell->phenotype.volume.target_solid_nuclear= pCell->phenotype.volume.target_solid_nuclear*2.0;
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
			
		}
		
		double probability_K1_K2 = dt /pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration;		
		// advance to K2 phase? 
		if( (!stoc_K1 && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration-0.001 ) ||  (stoc_K1 && uniform_random() < probability_K1_K2 ) )
		{
			// flag the cell for division 
			pCell->get_container()->flag_cell_for_division( pCell ); 
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_postmitotic); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;			
			//pCell->phenotype.phase_model_initialized = false; 
			return; 
		}
	}
	
	// K2 phase
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::Ki67_positive_postmitotic )
	{
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 			
			pCell->phenotype.volume.target_solid_nuclear= pCell->phenotype.volume.target_solid_nuclear/2.0;
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
		}
		
		double probability_K2_Q = dt /pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration;
			
		// advance to Q phase? 
		if( (!stoc_K2 && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration -0.001 ) || (stoc_K2 && uniform_random() < probability_K2_Q))
		{
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_negative); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;
			//pCell->phenotype.phase_model_initialized = false; 
			return; 
		}
	}	
	
	// Q phase 
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::Ki67_negative )
	{
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
		}
		
		// advance to K1 phase? 
		double probability_Q_K1 = (dt / pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration); 
		
		if( (!stoc_Q && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration -0.001 ) || (stoc_Q && uniform_random() < probability_Q_K1))
		{
			pCell->phenotype.set_current_phase(PhysiCell_constants::Ki67_positive_premitotic); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;
			//pCell->phenotype.phase_model_initialized = false; 
			return; 
		}
	}

	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time	+= dt; 
	return; 
}

void ki67_advanced_cycle_model_deterministic( Cell* pCell, double dt)
{
	ki67_advanced_cycle_model(pCell, dt, false, false, false, false);
}

void ki67_advanced_cycle_model_legacy( Cell* pCell, double dt)
{
	ki67_advanced_cycle_model(pCell, dt, false, false, true, false);
}

void ki67_advanced_cycle_model_stochastic( Cell* pCell, double dt)
{
	ki67_advanced_cycle_model(pCell, dt, true, true, true, true);
}

void live_apoptotic_cycle_model( Cell* pCell , double dt )
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(pCell, dt);
	if (is_necrotic)
		return;
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(pCell, dt, false);
	if (is_apoptotic)
		return;
	
	// live phase 
	if( pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].code == PhysiCell_constants::live )
	{
		// parameters set? 
		if( pCell->phenotype.phase_model_initialized == false )
		{
			// set phase-specific parameters 
			pCell->phenotype.update_volume_change_rates();
			pCell->phenotype.phase_model_initialized = true; 
		}		
		
		// divide? 
		double probability_division = dt / pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration; 
		if( uniform_random() < probability_division )
		{
			// flag the cell for division ; 
			pCell->get_container()->flag_cell_for_division( pCell ); 
			pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0; 
			return; 
		}
	}
	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt; 
}

void total_cells_cycle_model( Cell* pCell, double dt )
{	
	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt; 
}

void death_apoptosis_model( Cell* pCell, double dt, bool stochastic)
{
	if( pCell->phenotype.phase_model_initialized == false )
	{
		// set phase-specific parameters 
		pCell->phenotype.volume.target_fluid_fraction=0.0; 
		pCell->phenotype.volume.target_solid_nuclear=0;
		pCell->phenotype.update_volume_change_rates();
		pCell->phenotype.phase_model_initialized = true; 
	}

	//extern Cell_Container* cell_container; 
	if( pCell->phenotype.phase_model_initialized == false )
	{	
		pCell->phenotype.phase_model_initialized = true; 
	}
	
	// std::cout<<pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time<<", "<<pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration<<std::endl;
	// if too small, flag for deletion 	
	//if( pCell->phenotype.volume.total < PhysiCell_constants::cell_removal_threshold_volume )
		
	double probability_removal = dt/pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration ; 
		
	if((!stochastic && pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time >= pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].duration - 0.001 ) || (stochastic && uniform_random() < probability_removal))
	{
		pCell->get_container()->flag_cell_for_removal( pCell ); 
		return; 
	}
	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt; 
	return;
}

void death_apoptosis_model_stochastic( Cell* pCell, double dt)
{
	death_apoptosis_model(pCell, dt, true);
}

void death_apoptosis_model_deterministic( Cell* pCell, double dt)
{
	death_apoptosis_model(pCell, dt, false);
}

void death_necrosis_swelling_model( Cell* pCell, double dt )
{
	if( pCell->phenotype.phase_model_initialized == false )
	{
		// set phase-specific parameters 
		pCell->turn_off_reactions(dt);  // See the source code to make sure it does what you want it to do
		pCell->phenotype.volume.target_fluid_fraction=1.0; 
		pCell->phenotype.volume.target_solid_nuclear=0;
		pCell->phenotype.update_volume_change_rates();
		
		pCell->phenotype.volume.rupture_threshold = pCell->phenotype.volume.total *2.0;
		pCell->phenotype.phase_model_initialized = true; 
	}
	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt; 
	if( pCell->phenotype.volume.total > pCell->phenotype.volume.rupture_threshold )
	{
		// std::cout<<"lysed"<<std::endl;
		pCell->phenotype.set_current_phase(PhysiCell_constants::necrotic_lysed); 
		pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time = 0.0;	
		pCell->advance_cell_current_phase = death_necrosis_lysed_model; 			
		pCell->phenotype.volume.target_fluid_fraction=0; 
		pCell->phenotype.volume.target_solid_nuclear=0;
		//pCell->phenotype.phase_model_initialized = false; 
		return; 
	}		
}

void death_necrosis_lysed_model( Cell* pCell, double dt )
{
	if( pCell->phenotype.phase_model_initialized == false )
	{
		// set phase-specific parameters 
		pCell->phenotype.update_volume_change_rates();
		pCell->phenotype.phase_model_initialized = true; 
		pCell->phenotype.volume.target_fluid_fraction=0; 
	}
	pCell->phenotype.cycle.phases[pCell->phenotype.current_phase_index].elapsed_time += dt; 
	if( pCell->phenotype.volume.total < PhysiCell_constants::cell_removal_threshold_volume )
		pCell->get_container()->flag_cell_for_removal( pCell ); 
}

void up_orientation( Cell* pCell, double dt )
{
	pCell->orientation[0] = 0.0; 
	pCell->orientation[1] = 0.0;
	pCell->orientation[2] = 1.0; 
	
	pCell->polarity = 1.0; 

	return; 
}
