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

#include "PhysiCell_standard_models.h" 
#include "PhysiCell_cell.h" 

namespace PhysiCell{
	
bool PhysiCell_standard_models_initialized = false; 
bool PhysiCell_standard_death_models_initialized = false; 
bool PhysiCell_standard_cycle_models_initialized = false; 
	
Cycle_Model Ki67_advanced, Ki67_basic, live, apoptosis, necrosis; 
Cycle_Model cycling_quiescent; 
Death_Parameters apoptosis_parameters, necrosis_parameters; 

// new cycle models:

Cycle_Model flow_cytometry_cycle_model, flow_cytometry_separated_cycle_model; 

void standard_Ki67_positive_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the cell wants to double its volume 
	phenotype.volume.target_solid_nuclear *= 2.0; 
	phenotype.volume.target_solid_cytoplasmic *= 2.0; 
	return; 
}

void standard_Ki67_negative_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	return; 
}

void standard_live_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the cell wants to double its volume 
	phenotype.volume.target_solid_nuclear *= 2.0; 
	phenotype.volume.target_solid_cytoplasmic *= 2.0; 
	
	return; 
}

void S_phase_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the cell wants to double its volume 
	phenotype.volume.target_solid_nuclear *= 2.0; 
	phenotype.volume.target_solid_cytoplasmic *= 2.0; 
	
	return; 
}

void standard_cycling_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the cell wants to double its volume 
	phenotype.volume.target_solid_nuclear *= 2.0; 
	phenotype.volume.target_solid_cytoplasmic *= 2.0; 	
	
	return; 
}

void standard_apoptosis_entry_function( Cell* pCell, Phenotype& phenotype, double dt ) 
{
	// the volume model wants to shrink the cell
	phenotype.volume.target_fluid_fraction = 0.0; 
	phenotype.volume.target_solid_cytoplasmic = 0.0;
	phenotype.volume.target_solid_nuclear = 0.0;
	
	phenotype.volume.target_cytoplasmic_to_nuclear_ratio = 0.0; 
	
	// change the rate parameters 
	
	phenotype.volume.cytoplasmic_biomass_change_rate = 
		phenotype.death.current_parameters().cytoplasmic_biomass_change_rate; 
	phenotype.volume.nuclear_biomass_change_rate = 
		phenotype.death.current_parameters().nuclear_biomass_change_rate; 
	phenotype.volume.fluid_change_rate = 
		phenotype.death.current_parameters().unlysed_fluid_change_rate; 
	
	phenotype.volume.calcification_rate = 
		phenotype.death.current_parameters().calcification_rate;
	
	phenotype.volume.relative_rupture_volume = 
		phenotype.death.current_parameters().relative_rupture_volume;  
	phenotype.volume.rupture_volume = 
		phenotype.volume.total * phenotype.volume.relative_rupture_volume; 
		
	return; 
}

void standard_necrosis_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the volume model wants to degrade the solids, but swell by osmosis 	
	phenotype.volume.target_fluid_fraction = 1.0; 
	phenotype.volume.target_solid_cytoplasmic = 0.0;
	phenotype.volume.target_solid_nuclear = 0.0;

	phenotype.volume.target_cytoplasmic_to_nuclear_ratio = 0.0; 
	
	// change the rate parameters 	
	phenotype.volume.cytoplasmic_biomass_change_rate = 
		phenotype.death.current_parameters().cytoplasmic_biomass_change_rate; 
	phenotype.volume.nuclear_biomass_change_rate = 
		phenotype.death.current_parameters().nuclear_biomass_change_rate; 
	phenotype.volume.fluid_change_rate = 
		phenotype.death.current_parameters().unlysed_fluid_change_rate; 
	
	phenotype.volume.calcification_rate = 
		phenotype.death.current_parameters().calcification_rate;
	
	// set the bursting volume 
	phenotype.volume.relative_rupture_volume = 
		phenotype.death.current_parameters().relative_rupture_volume;  
	phenotype.volume.rupture_volume = 
		phenotype.volume.total * phenotype.volume.relative_rupture_volume; 
		
	return; 
}

void standard_lysis_entry_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// the volume model wants to shrink the cell
	phenotype.volume.target_fluid_fraction = 0.0; 
	phenotype.volume.target_solid_cytoplasmic = 0.0;
	phenotype.volume.target_solid_nuclear = 0.0;
	
	// change the rate parameters 
	
	phenotype.volume.cytoplasmic_biomass_change_rate = 
		phenotype.death.current_parameters().cytoplasmic_biomass_change_rate; 
	phenotype.volume.nuclear_biomass_change_rate = 
		phenotype.death.current_parameters().nuclear_biomass_change_rate; 
	phenotype.volume.fluid_change_rate = 
		phenotype.death.current_parameters().lysed_fluid_change_rate; 
	
	phenotype.volume.calcification_rate = 
		phenotype.death.current_parameters().calcification_rate;
		
	// set the bursting volume 
	phenotype.volume.relative_rupture_volume = 9e99; 
	phenotype.volume.rupture_volume = 
		phenotype.volume.total * phenotype.volume.relative_rupture_volume; 
		
	return; 
}

bool standard_necrosis_arrest_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	// remain in the non-lysed state / phase if volume has not exceeded the 
	// rupture volume 
	if( phenotype.volume.total < phenotype.volume.rupture_volume )
	{ return true; }
	
	return false; 
}

/* create standard models */ 

void create_ki67_models( void )
{
	// Ki67_basic: 
	
	Ki67_basic.code = PhysiCell_constants::basic_Ki67_cycle_model; 
	Ki67_basic.name = "Ki67 (basic)"; 
	
	Ki67_basic.data.time_units = "min"; 
	
	Ki67_basic.add_phase( PhysiCell_constants::Ki67_negative , "Ki67-" ); 
	Ki67_basic.add_phase( PhysiCell_constants::Ki67_positive , "Ki67+" ); 

	Ki67_basic.phases[1].division_at_phase_exit = true; 
	
	Ki67_basic.add_phase_link( 0 , 1 , NULL ); // - to +
	Ki67_basic.add_phase_link( 1 , 0 , NULL ); // + to - 
	
	Ki67_basic.transition_rate(0,1) = 1.0/(4.59*60.0); // MCF10A cells are ~4.59 hours in Ki67- state
	Ki67_basic.transition_rate(1,0) = 1.0/(15.5*60.0); // length of Ki67+ states in advanced model 
	Ki67_basic.phase_link(1,0).fixed_duration = true; 
	
	Ki67_basic.phases[0].entry_function = NULL; // standard_Ki67_negative_phase_entry_function;
	Ki67_basic.phases[1].entry_function = standard_Ki67_positive_phase_entry_function;
	
	// Ki67_advanced:
	
	Ki67_advanced.code = PhysiCell_constants::advanced_Ki67_cycle_model; 
	Ki67_advanced.name = "Ki67 (advanced)"; 

	Ki67_advanced.data.time_units = "min"; 
	
	Ki67_advanced.add_phase( PhysiCell_constants::Ki67_negative , "Ki67-" ); 
	Ki67_advanced.add_phase( PhysiCell_constants::Ki67_positive_premitotic , "Ki67+ (premitotic)" ); 
	Ki67_advanced.add_phase( PhysiCell_constants::Ki67_positive_postmitotic , "Ki67+ (postmitotic)" ); 

	Ki67_advanced.phases[1].division_at_phase_exit = true; 

	Ki67_advanced.add_phase_link( 0 , 1 , NULL ); // - to +
	Ki67_advanced.add_phase_link( 1 , 2 , NULL ); // + (pre-mitotic) to + (post-mitotic) 
	Ki67_advanced.add_phase_link( 2 , 0 , NULL ); // + to - 
	
	Ki67_advanced.phase_link(1,2).fixed_duration = true; 
	Ki67_advanced.phase_link(2,0).fixed_duration = true; 

	Ki67_advanced.transition_rate(0,1) = 1.0/(3.62*60.0); // MCF10A cells ~3.62 hours in Ki67- in this fitted model
	Ki67_advanced.transition_rate(1,2) = 1.0/(13.0*60.0); 
	Ki67_advanced.transition_rate(2,0) = 1.0/(2.5*60.0);

	Ki67_advanced.phases[0].entry_function = NULL; // standard_Ki67_negative_phase_entry_function;
	Ki67_advanced.phases[1].entry_function = standard_Ki67_positive_phase_entry_function;	
	
	return; 
}

void create_live_model( void )
{
	live.code = PhysiCell_constants::live_cells_cycle_model; 
	live.name = "Live"; 
	
	live.data.time_units = "min"; 
	
	live.add_phase( PhysiCell_constants::live , "Live" ); 
	
	live.phases[0].division_at_phase_exit = true; 
	
	live.add_phase_link( 0 , 0 , NULL ); 
	
	live.transition_rate(0,0) = 0.0432 / 60.0; // MCF10A have ~0.04 1/hr net birth rate
	
	live.phases[0].entry_function = standard_live_phase_entry_function;
		
	return; 
}

bool create_cytometry_cycle_models( void )
{
	// basic one first 
	flow_cytometry_cycle_model.code = PhysiCell_constants::flow_cytometry_cycle_model; 
	flow_cytometry_cycle_model.name = "Flow cytometry model (basic)"; 
	
	flow_cytometry_cycle_model.data.time_units = "min"; 
	
	flow_cytometry_cycle_model.add_phase( PhysiCell_constants::G0G1_phase , "G0/G1" ); 
	flow_cytometry_cycle_model.add_phase( PhysiCell_constants::S_phase , "S" ); 
	flow_cytometry_cycle_model.add_phase( PhysiCell_constants::G2M_phase , "G2/M" ); 

	flow_cytometry_cycle_model.phases[2].division_at_phase_exit = true; 
	
	flow_cytometry_cycle_model.add_phase_link( 0 , 1 , NULL ); // G0/G1 to S
	flow_cytometry_cycle_model.add_phase_link( 1 , 2 , NULL ); // S to G2/M
	flow_cytometry_cycle_model.add_phase_link( 2 , 0 , NULL ); // G2/M to G0/G1 

	// need reference values! 
	// https://www.ncbi.nlm.nih.gov/books/NBK9876/
	flow_cytometry_cycle_model.transition_rate(0,1) = 0.00324; // 5.15 hours in G0/G1 by fitting 
	flow_cytometry_cycle_model.transition_rate(1,2) = 0.00208; // 8 hours in S
	flow_cytometry_cycle_model.transition_rate(2,0) = 0.00333; // 5 hours in G2/M 
	
	
	flow_cytometry_cycle_model.phases[0].entry_function = NULL; //  ;
	flow_cytometry_cycle_model.phases[1].entry_function = S_phase_entry_function; // Double nuclear volume ;
	flow_cytometry_cycle_model.phases[2].entry_function = NULL;		

	// // expanded flow cytometry model 
	
	flow_cytometry_separated_cycle_model.code = PhysiCell_constants::flow_cytometry_separated_cycle_model; 
	flow_cytometry_separated_cycle_model.name = "Flow cytometry model (separated)"; 
	
	flow_cytometry_separated_cycle_model.data.time_units = "min"; 
	
	flow_cytometry_separated_cycle_model.add_phase( PhysiCell_constants::G0G1_phase , "G0/G1" ); 
	flow_cytometry_separated_cycle_model.add_phase( PhysiCell_constants::S_phase , "S" ); 
	flow_cytometry_separated_cycle_model.add_phase( PhysiCell_constants::G2_phase , "G2" ); 
	flow_cytometry_separated_cycle_model.add_phase( PhysiCell_constants::M_phase , "M" ); 
	
	
	flow_cytometry_separated_cycle_model.phases[3].division_at_phase_exit = true; 
	
	flow_cytometry_separated_cycle_model.add_phase_link( 0 , 1 , NULL ); // G0/G1 to S
	flow_cytometry_separated_cycle_model.add_phase_link( 1 , 2 , NULL ); // S to G2
	flow_cytometry_separated_cycle_model.add_phase_link( 2 , 3 , NULL ); // G2 to M 
	flow_cytometry_separated_cycle_model.add_phase_link( 3 , 0 , NULL ); // M to G0/G1 

	// need reference values! 
	flow_cytometry_separated_cycle_model.transition_rate(0,1) = 0.00335; // 4.98 hours in G0/G1
	flow_cytometry_separated_cycle_model.transition_rate(1,2) = 0.00208; // 8 hours in S  
	flow_cytometry_separated_cycle_model.transition_rate(2,3) = 0.00417; // 4 hours in G2 
	flow_cytometry_separated_cycle_model.transition_rate(3,0) = 0.0167; // 1 hour in M 
	
	flow_cytometry_separated_cycle_model.phases[0].entry_function = NULL; //  ;
	flow_cytometry_separated_cycle_model.phases[1].entry_function = S_phase_entry_function; // Double nuclear volume ;
	flow_cytometry_separated_cycle_model.phases[2].entry_function = NULL;		
	flow_cytometry_separated_cycle_model.phases[3].entry_function = NULL;		
	
	return true; 
}

void create_cycling_quiescent_model( void )
{
	// Ki67_basic: 
	
	cycling_quiescent.code = PhysiCell_constants::cycling_quiescent_model; 
	cycling_quiescent.name = "Cycling-Quiescent model"; 
	
	cycling_quiescent.data.time_units = "min"; 
	
	cycling_quiescent.add_phase( PhysiCell_constants::quiescent , "Quiescent" ); 
	cycling_quiescent.add_phase( PhysiCell_constants::cycling , "Cycling" ); 

	cycling_quiescent.phases[1].division_at_phase_exit = true; 
	
	cycling_quiescent.add_phase_link( 0 , 1 , NULL ); // Q to C
	cycling_quiescent.add_phase_link( 1 , 0 , NULL ); // C to Q 
	
	cycling_quiescent.transition_rate(0,1) = 1.0/(4.59*60.0); // MCF10A cells are ~4.59 hours in Ki67- state
	cycling_quiescent.transition_rate(1,0) = 1.0/(15.5*60.0); // length of Ki67+ states in advanced model 
	cycling_quiescent.phase_link(1,0).fixed_duration = true; 
	
	cycling_quiescent.phases[0].entry_function = NULL; 
	cycling_quiescent.phases[1].entry_function = standard_cycling_entry_function;
	
	return; 
}

bool create_standard_cell_cycle_models( void )
{
	if( PhysiCell_standard_cycle_models_initialized )
	{ return false; }

	create_ki67_models();
	create_live_model();
	
	create_cytometry_cycle_models(); 
	
	create_cycling_quiescent_model();

	PhysiCell_standard_cycle_models_initialized = true;
	if( PhysiCell_standard_death_models_initialized )
	{
		PhysiCell_standard_models_initialized = true; 
	}
	
	return true; 
}

void create_standard_apoptosis_model( void )
{
	// set default parameters for apoptosis
	apoptosis_parameters.time_units = "min"; 
	
	apoptosis_parameters.cytoplasmic_biomass_change_rate = 1.0 / 60.0; 
	apoptosis_parameters.nuclear_biomass_change_rate = 0.35 / 60.0; 
		
	apoptosis_parameters.unlysed_fluid_change_rate = 3.0 / 60.0; 
	apoptosis_parameters.lysed_fluid_change_rate = 0.0; 
	
	apoptosis_parameters.calcification_rate = 0.0; 

	apoptosis_parameters.relative_rupture_volume = 2.0; 
 
	// set up the apoptosis model 
	apoptosis.name = "Apoptosis"; 
	apoptosis.code = PhysiCell_constants::apoptosis_death_model; 

		// add the main phase for this model, make sure it 
		// triggers the appropriate entry function, and note that 
		// it should trigger cell removal at its end 
	apoptosis.add_phase( PhysiCell_constants::apoptotic , "Apoptotic" );
	apoptosis.phases[0].entry_function = standard_apoptosis_entry_function; 
	apoptosis.phases[0].removal_at_phase_exit = true;

		// add an empty junk debris phase for this model 
	apoptosis.add_phase( PhysiCell_constants::debris , "Debris" ); 

		// Add a link between these phases. Set the cell to be removed 
		// upon this transition. (So the "debris" phase should never be entered). 
	apoptosis.add_phase_link( 0, 1, NULL ); 
	apoptosis.transition_rate( 0, 1) = 1.0 / (8.6 * 60.0); 

		// Use the deterministic model, where this phase has fixed duration
	apoptosis.phase_link(0,1).fixed_duration = true; 
	
	return; 
}

void create_standard_necrosis_model( void )
{
	// set default parameters for necrosis
	necrosis_parameters.time_units = "min"; 
	
	necrosis_parameters.cytoplasmic_biomass_change_rate = 0.0032 / 60.0; 
	necrosis_parameters.nuclear_biomass_change_rate = 0.013 / 60.0; 
		
	necrosis_parameters.unlysed_fluid_change_rate = 0.67 / 60.0; 
	necrosis_parameters.lysed_fluid_change_rate = 0.050 / 60.0; 
	
	necrosis_parameters.calcification_rate = 0.0042 / 60.0; 

	necrosis_parameters.relative_rupture_volume = 2.0; 
 
	// set up the necrosis model 
	necrosis.name = "Necrosis"; 
	necrosis.code = PhysiCell_constants::necrosis_death_model; 

	necrosis.add_phase( PhysiCell_constants::necrotic_swelling , "Necrotic (swelling)" );
	necrosis.phases[0].entry_function = standard_necrosis_entry_function; 

	necrosis.add_phase( PhysiCell_constants::necrotic_lysed , "Necrotic (lysed)" );
	necrosis.phases[1].entry_function = standard_lysis_entry_function; 
	necrosis.phases[1].removal_at_phase_exit = true;
	
		// add an empty junk debris phase for this model 
	necrosis.add_phase( PhysiCell_constants::debris , "Debris" ); 


	necrosis.add_phase_link( 0, 1, standard_necrosis_arrest_function ); 
	necrosis.add_phase_link( 1, 2, NULL ); 

	necrosis.transition_rate( 0, 1 ) = 9e9; // set high so it's always evaluating against the "arrest" 
	necrosis.transition_rate( 1, 2 ) = 1.0 / (60.0 * 24.0 * 60.0 ); // 60 days max  

	// Deterministically remove the necrotic cell if it has been 60 days
	necrosis.phase_link(1,2).fixed_duration = true; 

	return; 
}	

bool create_standard_cell_death_models( void )
{
	if( PhysiCell_standard_death_models_initialized )
	{ return false; }
	
	create_standard_apoptosis_model();
	create_standard_necrosis_model();
	
	PhysiCell_standard_death_models_initialized = true; 
	if( PhysiCell_standard_cycle_models_initialized )
	{
		PhysiCell_standard_models_initialized = true; 
	}
	
	return true; 
}

bool create_standard_cycle_and_death_models( void )
{
	bool output = false; 
	if( create_standard_cell_cycle_models() )
	{ output = true; }
	if( create_standard_cell_death_models() )
	{ output = true; }
	
	return output; 
}

void standard_volume_update_function( Cell* pCell, Phenotype& phenotype, double dt )
{
	phenotype.volume.fluid += dt * phenotype.volume.fluid_change_rate * 
		( phenotype.volume.target_fluid_fraction * phenotype.volume.total - phenotype.volume.fluid );
		
	// if the fluid volume is negative, set to zero
	if( phenotype.volume.fluid < 0.0 )
	{ phenotype.volume.fluid = 0.0; }
		
	phenotype.volume.nuclear_fluid = (phenotype.volume.nuclear / phenotype.volume.total) * 
		( phenotype.volume.fluid );
	phenotype.volume.cytoplasmic_fluid = phenotype.volume.fluid - phenotype.volume.nuclear_fluid; 

	phenotype.volume.nuclear_solid  += dt * phenotype.volume.nuclear_biomass_change_rate * 
		(phenotype.volume.target_solid_nuclear - phenotype.volume.nuclear_solid );    
	if( phenotype.volume.nuclear_solid < 0.0 )
	{ phenotype.volume.nuclear_solid = 0.0; }
	
	phenotype.volume.target_solid_cytoplasmic = phenotype.volume.target_cytoplasmic_to_nuclear_ratio * // phenotype.volume.cytoplasmic_to_nuclear_fraction * 
		phenotype.volume.target_solid_nuclear;

	phenotype.volume.cytoplasmic_solid += dt * phenotype.volume.cytoplasmic_biomass_change_rate * 
		( phenotype.volume.target_solid_cytoplasmic - phenotype.volume.cytoplasmic_solid );	
	if( phenotype.volume.cytoplasmic_solid < 0.0 )
	{ phenotype.volume.cytoplasmic_solid = 0.0; }
	
	phenotype.volume.solid = phenotype.volume.nuclear_solid + phenotype.volume.cytoplasmic_solid;
	
	phenotype.volume.nuclear = phenotype.volume.nuclear_solid + phenotype.volume.nuclear_fluid; 
	phenotype.volume.cytoplasmic = phenotype.volume.cytoplasmic_solid + phenotype.volume.cytoplasmic_fluid; 
	
	phenotype.volume.calcified_fraction = dt * phenotype.volume.calcification_rate 
		* (1- phenotype.volume.calcified_fraction);
   
	phenotype.volume.total = phenotype.volume.cytoplasmic + phenotype.volume.nuclear; 
	
	
	phenotype.volume.fluid_fraction = phenotype.volume.fluid / 
		( 1e-16 + phenotype.volume.total ); 
   
	phenotype.geometry.update( pCell,phenotype,dt );

	return; 
}

void basic_volume_model( Cell* pCell, Phenotype& phenotype, double dt )
{
	
	
	
	return; 
}

void standard_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt)
{
	if( pCell->functions.add_cell_basement_membrane_interactions )
	{
		pCell->functions.add_cell_basement_membrane_interactions(pCell, phenotype,dt);
	}
	
	pCell->state.simple_pressure = 0.0; 
	pCell->state.neighbors.clear(); // new 1.8.0
	
	//First check the neighbors in my current voxel
	std::vector<Cell*>::iterator neighbor;
	std::vector<Cell*>::iterator end = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].end();
	for(neighbor = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].begin(); neighbor != end; ++neighbor)
	{
		pCell->add_potentials(*neighbor);
	}
	std::vector<int>::iterator neighbor_voxel_index;
	std::vector<int>::iterator neighbor_voxel_index_end = 
		pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].end();

	for( neighbor_voxel_index = 
		pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].begin();
		neighbor_voxel_index != neighbor_voxel_index_end; 
		++neighbor_voxel_index )
	{
		if(!is_neighbor_voxel(pCell, pCell->get_container()->underlying_mesh.voxels[pCell->get_current_mechanics_voxel_index()].center, pCell->get_container()->underlying_mesh.voxels[*neighbor_voxel_index].center, *neighbor_voxel_index))
			continue;
		end = pCell->get_container()->agent_grid[*neighbor_voxel_index].end();
		for(neighbor = pCell->get_container()->agent_grid[*neighbor_voxel_index].begin();neighbor != end; ++neighbor)
		{
			pCell->add_potentials(*neighbor);
		}
	}

	pCell->update_motility_vector(dt); 
	pCell->velocity += phenotype.motility.motility_vector; 
	
	return; 
}

void standard_add_basement_membrane_interactions( Cell* pCell, Phenotype& phenotype, double dt )
{
	if( pCell->functions.calculate_distance_to_membrane == NULL )
	{ return; }
	
	double max_interactive_distance = phenotype.mechanics.relative_maximum_adhesion_distance * phenotype.geometry.radius;
	double distance = pCell->functions.calculate_distance_to_membrane(pCell,phenotype,dt); 
	//Note that the distance_to_membrane function must set displacement values (as a normal vector)
		
	double temp_a=0;
	// Adhesion to basement membrane
	if(distance< max_interactive_distance)
	{
		temp_a= (1- distance/max_interactive_distance);
		temp_a*=temp_a;
		temp_a*=-phenotype.mechanics.cell_BM_adhesion_strength;
	}
	// Repulsion from basement membrane
	double temp_r = 0;
	if(distance < phenotype.geometry.radius)
	{
		temp_r = (1- distance/phenotype.geometry.radius);
		temp_r *= temp_r;
		temp_r *= phenotype.mechanics.cell_BM_repulsion_strength;
	}
	temp_r += temp_a;
	if( fabs( temp_r ) < 1e-16 )
	{ return; }
	
	axpy( &( pCell->velocity ) , temp_r , pCell->displacement ); 
	return;	
}

void standard_domain_edge_avoidance_interactions( Cell* pCell, Phenotype& phenotype, double dt )
{
	if( pCell->functions.calculate_distance_to_membrane == NULL )
	{ pCell->functions.calculate_distance_to_membrane = distance_to_domain_edge; }
	phenotype.mechanics.cell_BM_repulsion_strength = 100;  
		
	double max_interactive_distance = phenotype.mechanics.relative_maximum_adhesion_distance * phenotype.geometry.radius;
	double distance = pCell->functions.calculate_distance_to_membrane(pCell,phenotype,dt); 
	//Note that the distance_to_membrane function must set displacement values (as a normal vector)
		
	// Repulsion from basement membrane
	double temp_r = 0;
	if(distance < phenotype.geometry.radius)
	{
		temp_r = (1- distance/phenotype.geometry.radius);
		temp_r *= temp_r;
		temp_r *= phenotype.mechanics.cell_BM_repulsion_strength;
	}
	if( fabs( temp_r ) < 1e-16 )
	{ return; }
	
	axpy( &( pCell->velocity ) , temp_r , pCell->displacement ); 
	return;
}

void empty_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; } 

void up_orientation( Cell* pCell, Phenotype& phenotype, double dt )
{  
	pCell->state.orientation[0] = 0.0; 
	pCell->state.orientation[1] = 0.0; 
	pCell->state.orientation[2] = 1.0;
	return; 
}

void initialize_default_cell_definition( void )
{
	// If the standard models have not yet been created, do so now. 
	create_standard_cycle_and_death_models();
		
	// set the microenvironment pointer 
	cell_defaults.pMicroenvironment = NULL;
	if( BioFVM::get_default_microenvironment() != NULL )
	{ cell_defaults.pMicroenvironment = BioFVM::get_default_microenvironment(); }
	
	// make sure phenotype.secretions are correctly sized 
	
	cell_defaults.phenotype.secretion.sync_to_current_microenvironment();
	
	// set up the default parameters 
		
	cell_defaults.type = 0; 
	cell_defaults.name = "breast epithelium"; 

	cell_defaults.parameters.pReference_live_phenotype = &(cell_defaults.phenotype); 
	
	// set up the default custom data 
		// the default Custom_Cell_Data constructor should take care of this
		
	// set up the default functions 
	cell_defaults.functions.cycle_model = Ki67_advanced; 
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_migration_bias = NULL; 
	
	cell_defaults.functions.update_phenotype = update_cell_and_death_parameters_O2_based; // NULL; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
	
	cell_defaults.functions.set_orientation = NULL;

	// add the standard death models to the default phenotype. 
	cell_defaults.phenotype.death.add_death_model( 0.00319/60.0 , &apoptosis , apoptosis_parameters );
		// MCF10A, to get a 2% apoptotic index 
	cell_defaults.phenotype.death.add_death_model( 0.0 , &necrosis , necrosis_parameters );
	
	// set up the default phenotype (to be consistent with the default functions)
	cell_defaults.phenotype.cycle.sync_to_cycle_model( cell_defaults.functions.cycle_model ); 
	
	// set molecular defaults 
	
	return; 	
}

void update_cell_and_death_parameters_O2_based( Cell* pCell, Phenotype& phenotype, double dt )
{
	// supported cycle models:
		// advanced_Ki67_cycle_model= 0;
		// basic_Ki67_cycle_model=1
		// live_cells_cycle_model = 5; 
	
	if( phenotype.death.dead == true )
	{ return; }
	
	// set up shortcuts to find the Q and K(1) phases (assuming Ki67 basic or advanced model)
	static bool indices_initiated = false; 
	static int start_phase_index; // Q_phase_index; 
	static int end_phase_index; // K_phase_index;
	static int necrosis_index; 
	
	static int oxygen_substrate_index = pCell->get_microenvironment()->find_density_index( "oxygen" ); 
	
	if( indices_initiated == false )
	{
		// Ki67 models
		
		if( phenotype.cycle.model().code == PhysiCell_constants::advanced_Ki67_cycle_model || 
			phenotype.cycle.model().code == PhysiCell_constants::basic_Ki67_cycle_model )
		{
			start_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::Ki67_negative );
			necrosis_index = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
			
			if( phenotype.cycle.model().code == PhysiCell_constants::basic_Ki67_cycle_model )
			{
				end_phase_index = 
					phenotype.cycle.model().find_phase_index( PhysiCell_constants::Ki67_positive );
				indices_initiated = true; 
			}
			if( phenotype.cycle.model().code == PhysiCell_constants::advanced_Ki67_cycle_model )
			{
				end_phase_index = 
					phenotype.cycle.model().find_phase_index( PhysiCell_constants::Ki67_positive_premitotic );
				indices_initiated = true; 
			}
		}
		
		// live model 
			
		if( phenotype.cycle.model().code == PhysiCell_constants::live_cells_cycle_model )
		{
			start_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::live );
			necrosis_index = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
			end_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::live );
			indices_initiated = true; 
		}
		
		// cytometry models 
		
		if( phenotype.cycle.model().code == PhysiCell_constants::flow_cytometry_cycle_model || 
			phenotype.cycle.model().code == PhysiCell_constants::flow_cytometry_separated_cycle_model )
		{
			start_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::G0G1_phase );
			necrosis_index = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
			end_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::S_phase );
			indices_initiated = true; 
		}	

		if( phenotype.cycle.model().code == PhysiCell_constants::cycling_quiescent_model )
		{
			start_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::quiescent );
			necrosis_index = phenotype.death.find_death_model_index( PhysiCell_constants::necrosis_death_model ); 
			end_phase_index = phenotype.cycle.model().find_phase_index( PhysiCell_constants::cycling );
			indices_initiated = true; 
		}
		
	}
	
	// don't continue if we never "figured out" the current cycle model. 
	if( indices_initiated == false )
	{
		return; 
	}

	// sample the microenvironment to get the pO2 value 
	
	double pO2 = (pCell->nearest_density_vector())[oxygen_substrate_index]; // PhysiCell_constants::oxygen_index]; 
	int n = pCell->phenotype.cycle.current_phase_index(); 
	
	// this multiplier is for linear interpolation of the oxygen value 
	double multiplier = 1.0;
	if( pO2 < pCell->parameters.o2_proliferation_saturation )
	{
		multiplier = ( pO2 - pCell->parameters.o2_proliferation_threshold ) 
			/ ( pCell->parameters.o2_proliferation_saturation - pCell->parameters.o2_proliferation_threshold );
	}
	if( pO2 < pCell->parameters.o2_proliferation_threshold )
	{ 
		multiplier = 0.0; 
	}
	
	// now, update the appropriate cycle transition rate 
	
	phenotype.cycle.data.transition_rate(start_phase_index,end_phase_index) = multiplier * 
		pCell->parameters.pReference_live_phenotype->cycle.data.transition_rate(start_phase_index,end_phase_index);
	
	// Update necrosis rate
	multiplier = 0.0;
	if( pO2 < pCell->parameters.o2_necrosis_threshold )
	{
		multiplier = ( pCell->parameters.o2_necrosis_threshold - pO2 ) 
			/ ( pCell->parameters.o2_necrosis_threshold - pCell->parameters.o2_necrosis_max );
	}
	if( pO2 < pCell->parameters.o2_necrosis_max )
	{ 
		multiplier = 1.0; 
	}	
	
	// now, update the necrosis rate 
	
	pCell->phenotype.death.rates[necrosis_index] = multiplier * pCell->parameters.max_necrosis_rate; 
	
	// check for deterministic necrosis 
	
	if( pCell->parameters.necrosis_type == PhysiCell_constants::deterministic_necrosis && multiplier > 1e-16 )
	{ pCell->phenotype.death.rates[necrosis_index] = 9e99; } 
	
	return; 
}

void chemotaxis_function( Cell* pCell, Phenotype& phenotype , double dt )
{
	// bias direction is gradient for the indicated substrate 
	phenotype.motility.migration_bias_direction = pCell->nearest_gradient(phenotype.motility.chemotaxis_index);
	// move up or down gradient based on this direction 
	phenotype.motility.migration_bias_direction *= phenotype.motility.chemotaxis_direction; 

	// normalize 
	normalize( &( phenotype.motility.migration_bias_direction ) );
	
	return;
}

void standard_elastic_contact_function( Cell* pC1, Phenotype& p1, Cell* pC2, Phenotype& p2 , double dt )
{
	if( pC1->position.size() != 3 || pC2->position.size() != 3 )
	{
		#pragma omp critical
		{
			std::cout << "what?! " << std::endl
			<< pC1 << " : " << pC1->type << " " << pC1->type_name << " " << pC1->position << std::endl 
			<< pC2 << " : " << pC2->type << " " << pC2->type_name << " " << pC2->position << std::endl ;
		}
		return; 
	}
	
	std::vector<double> displacement = pC2->position;
	displacement -= pC1->position; 
	// std::cout << "vel: " << pC1->velocity << " disp: " << displacement << " e: " << p1.mechanics.attachment_elastic_constant << " vel new: "; 
	axpy( &(pC1->velocity) , p1.mechanics.attachment_elastic_constant , displacement ); 
	// std::cout << pC1->velocity << std::endl << std::endl; 
	return; 
}

void evaluate_interactions( Cell* pCell, Phenotype& phenotype, double dt )
{
	if( pCell->functions.contact_function == NULL )
	{ return; }
	
	for( int n = 0; n < pCell->state.attached_cells.size() ; n++ )
	{
		pCell->functions.contact_function( pCell, phenotype , 
			pCell->state.attached_cells[n] , pCell->state.attached_cells[n]->phenotype , dt ); 
	}
	
	return; 
}

double distance_to_domain_edge(Cell* pCell, Phenotype& phenotype, double dummy)
{
	static double tolerance = 1e-7;
	static double one_over_sqrt_2 = 0.70710678118;
	static double one_over_sqrt_3 = 0.57735026919; 
	
		
	double min_distance = 9e99; 
	int nearest_boundary = -1; 
	
	// check against xL and xU
	double temp_distance = pCell->position[0] - microenvironment.mesh.bounding_box[0]; 
	if( temp_distance < min_distance )
	{
		min_distance = temp_distance; 
		nearest_boundary = 0; 
	}
	temp_distance = microenvironment.mesh.bounding_box[3] - pCell->position[0]; 
	if( temp_distance < min_distance )
	{
		min_distance = temp_distance; 
		nearest_boundary = 1; 
	}
	
	// check against yL and yU
	temp_distance = pCell->position[1] - microenvironment.mesh.bounding_box[1]; 
	if( temp_distance < min_distance )
	{
		min_distance = temp_distance; 
		nearest_boundary = 2; 
	}
	temp_distance = microenvironment.mesh.bounding_box[4] - pCell->position[1]; 
	if( temp_distance < min_distance )
	{
		min_distance = temp_distance; 
		nearest_boundary = 3; 
	}	
	
	if( default_microenvironment_options.simulate_2D == false )
	{
		// if in 3D, check against zL and zU
		temp_distance = pCell->position[2] - microenvironment.mesh.bounding_box[2]; 
		if( temp_distance < min_distance )
		{
			min_distance = temp_distance; 
			nearest_boundary = 4; 
		}
		temp_distance = microenvironment.mesh.bounding_box[5] - pCell->position[2]; 
		if( temp_distance < min_distance )
		{
			min_distance = temp_distance; 
			nearest_boundary = 5; 
		}			
		
		// check for 3D exceptions 
		
		// lines 
		if( fabs( (pCell->position[0]) - (pCell->position[1]) ) < tolerance && 
			fabs( (pCell->position[1]) - (pCell->position[2]) ) < tolerance && 
			fabs( (pCell->position[0]) - (pCell->position[2]) ) < tolerance )
		{
			if( pCell->position[0] > 0 )
			{
				if( pCell->position[0] > 0 && pCell->position[1] > 0 )
				{ pCell->displacement = { -one_over_sqrt_3 , -one_over_sqrt_3 , -one_over_sqrt_3 }; }
				if( pCell->position[0] < 0 && pCell->position[1] > 0 )
				{ pCell->displacement = { one_over_sqrt_3 , -one_over_sqrt_3 , -one_over_sqrt_3 }; }
				
				if( pCell->position[0] > 0 && pCell->position[1] < 0 )
				{ pCell->displacement = { -one_over_sqrt_3 , one_over_sqrt_3 , -one_over_sqrt_3 }; }
				if( pCell->position[0] < 0 && pCell->position[1] < 0 )
				{ pCell->displacement = { one_over_sqrt_3 , one_over_sqrt_3 , -one_over_sqrt_3 }; }
			} 
			else
			{
				if( pCell->position[0] > 0 && pCell->position[1] > 0 )
				{ pCell->displacement = { -one_over_sqrt_3 , -one_over_sqrt_3 , one_over_sqrt_3 }; }
				if( pCell->position[0] < 0 && pCell->position[1] > 0 )
				{ pCell->displacement = { one_over_sqrt_3 , -one_over_sqrt_3 , one_over_sqrt_3 }; }
				
				if( pCell->position[0] > 0 && pCell->position[1] < 0 )
				{ pCell->displacement = { -one_over_sqrt_3 , one_over_sqrt_3 , one_over_sqrt_3 }; }
				if( pCell->position[0] < 0 && pCell->position[1] < 0 )
				{ pCell->displacement = { one_over_sqrt_3 , one_over_sqrt_3 , one_over_sqrt_3 }; }				
			}
			return min_distance; 
		}
		
		// planes - let's not worry for today 
		
	}
	else
	{
		// check for 2D  exceptions 
		
		if( fabs( (pCell->position[0]) - (pCell->position[1]) ) < tolerance )
		{
			if( pCell->position[0] > 0 && pCell->position[1] > 0 )
			{ pCell->displacement = { -one_over_sqrt_2 , -one_over_sqrt_2 , 0 }; }
			if( pCell->position[0] < 0 && pCell->position[1] > 0 )
			{ pCell->displacement = { one_over_sqrt_2 , -one_over_sqrt_2 , 0 }; }
			
			if( pCell->position[0] > 0 && pCell->position[1] < 0 )
			{ pCell->displacement = { -one_over_sqrt_2 , one_over_sqrt_2 , 0 }; }
			if( pCell->position[0] < 0 && pCell->position[1] < 0 )
			{ pCell->displacement = { one_over_sqrt_2 , one_over_sqrt_2 , 0 }; }
			return min_distance; 
		}
	}
	
	// no exceptions 
	switch(nearest_boundary)
	{
		case 0:
			pCell->displacement = {1,0,0}; 
			return min_distance; 
		case 1:
			pCell->displacement = {-1,0,0}; 
			return min_distance;
		case 2:
			pCell->displacement = {0,1,0}; 
			return min_distance; 
		case 3: 
			pCell->displacement = {0,-1,0}; 
			return min_distance; 
		case 4: 
			pCell->displacement = {0,0,1}; 
			return min_distance; 
		case 5: 
			pCell->displacement = {0,0,-1}; 
			return min_distance; 
		default:
			pCell->displacement = {0,0,0};
			return 9e99; 
	}
	
	pCell->displacement = {0,0,0};
	return 9e99; 
}	
	
};
