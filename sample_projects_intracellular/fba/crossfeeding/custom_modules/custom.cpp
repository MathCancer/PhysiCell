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
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
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

// libSBML and PhysiCell both use the name "Parameter"; parse SBML headers before
// PhysiCell_settings.h's template Parameter is visible (see sbmlfwd.h typedefs).
#include <sbml/SBMLTypes.h>

#include "custom.h"
#include <cmath>
#include "../addons/dFBA/src/dfba_intracellular.h"

namespace {

double get_dfba_infeasible_flag_for_cell(PhysiCell::Cell* pCell)
{
	if (!pCell || !pCell->phenotype.intracellular)
	{
		return 0.0;
	}
	auto* dfba = dynamic_cast<PhysiCelldFBA::dFBAIntracellular*>(pCell->phenotype.intracellular);
	if (!dfba)
	{
		return 0.0;
	}
	return dfba->flag_for_death ? 1.0 : 0.0;
}

void set_substrate_dirichlet_value_on_all_dirichlet_voxels(int substrate_index, double value)
{
	if (substrate_index < 0)
	{
		return;
	}
	for (unsigned int n = 0; n < microenvironment.mesh.voxels.size(); n++)
	{
		if (microenvironment.mesh.voxels[n].is_Dirichlet)
		{
			microenvironment.update_dirichlet_node(static_cast<int>(n), substrate_index, value);
		}
	}
}

bool glucose_refeed_pulse_active(double t_min, double first_pulse_min, double period_min, double pulse_duration_min)
{
	if (t_min + 1e-12 < first_pulse_min)
	{
		return false;
	}
	if (period_min <= 1e-12)
	{
		return (t_min < first_pulse_min + pulse_duration_min);
	}
	double phase = std::fmod(t_min - first_pulse_min, period_min);
	return (phase < pulse_duration_min + 1e-9);
}

void apply_legacy_nutrient_reintroduction_window(void)
{
	if (parameters.strings.find_index("reintroduced_nutrient") == -1 ||
		parameters.doubles.find_index("reintroduction_start_time") == -1 ||
		parameters.doubles.find_index("reintroduction_duration") == -1)
	{
		return;
	}
	int nutrient_index = microenvironment.find_density_index(parameters.strings("reintroduced_nutrient"));
	if (nutrient_index < 0)
	{
		return;
	}
	double t = PhysiCell_globals.current_time;
	double t0 = parameters.doubles("reintroduction_start_time");
	double dur = parameters.doubles("reintroduction_duration");
	bool in_window = (t >= t0 && t < t0 + dur);
	bool active = microenvironment.get_substrate_dirichlet_activation(nutrient_index);

	if (in_window && !active)
	{
		std::cout << parameters.strings("reintroduced_nutrient") << " Dirichlet activated at t=" << t << std::endl;
		microenvironment.set_substrate_dirichlet_activation(nutrient_index, true);
	}
	else if (t < t0 && active)
	{
		microenvironment.set_substrate_dirichlet_activation(nutrient_index, false);
	}
	else if (t >= t0 + dur && active)
	{
		std::cout << parameters.strings("reintroduced_nutrient") << " Dirichlet deactivated at t=" << t << std::endl;
		microenvironment.set_substrate_dirichlet_activation(nutrient_index, false);
	}
}

} // namespace


void create_cell_types(void)
{
	SeedRandom(parameters.ints("random_seed"));

	initialize_default_cell_definition();

	/*  This parses the cell definitions in the XML config file.  */
	initialize_cell_definitions_from_pugixml();

	//  This sets the pre and post intracellular update functions
	cell_defaults.functions.pre_update_intracellular =  NULL;
	cell_defaults.functions.post_update_intracellular = NULL;
	cell_defaults.functions.update_phenotype = NULL; 
	cell_defaults.functions.volume_update_function = NULL;

	build_cell_definitions_maps();
	
	setup_signal_behavior_dictionaries();

	Cell_Definition* c_beijerinckii = find_cell_definition( "c_beijerinckii");
	//  This sets the pre and post intracellular update functions for C. beijerinckii
	c_beijerinckii->functions.pre_update_intracellular =  NULL;
	c_beijerinckii->functions.post_update_intracellular = post_update_intracellular_c_beijerinckii;
	c_beijerinckii->functions.update_phenotype = NULL; 
	c_beijerinckii->functions.volume_update_function = NULL;

	Cell_Definition* m_barkeri = find_cell_definition( "m_barkeri");
	//  This sets the pre and post intracellular update functions for M. barkeri
	m_barkeri->functions.pre_update_intracellular =  NULL;
	m_barkeri->functions.post_update_intracellular = post_update_intracellular_m_barkeri;
	m_barkeri->functions.update_phenotype = NULL; 
	m_barkeri->functions.volume_update_function = NULL;

	display_cell_definitions(std::cout);

	return;
}




void setup_microenvironment(void)
{
	initialize_microenvironment();
	apply_initial_glucose_refeed_setup();
	return;
}

void setup_tissue(void)
{

	double Xmin = microenvironment.mesh.bounding_box[0]; 
	double Ymin = microenvironment.mesh.bounding_box[1]; 
	double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin; 
	
	// create some of each type of cell 
	
	Cell* pC;
	
	for( int k=0; k < cell_definitions_by_index.size() ; k++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[k]; 
		std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
		for( int n = 0 ; n < parameters.ints("number_of_cells") ; n++ )
		{
			std::vector<double> position = {0,0,0}; 
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			pC = create_cell( *pCD ); 
			pC->assign_position( position );
		}
	}
	// std::cout << std::endl; 
	
	// load cells from your CSV file
	load_cells_from_pugixml();

	// Desynchronize initial cell divisions by assigning each cell a random
	// starting volume drawn uniformly from [0.5, 1.0] × its reference_volume.
	// Without this, all cells of the same type start at identical volumes,
	// grow at the same rate and reach the division threshold simultaneously,
	// producing a community-wide "division wave" that causes synchronized
	// spikes in H2/CO2 fluxes every ~doubling time.
	for( auto pC : *all_cells )
	{
		if( pC->phenotype.intracellular == NULL ) { continue; }

		// Retrieve dFBA reference volume via the growth-model parameter
		double v_ref = pC->phenotype.intracellular->get_parameter_value("reference_volume");
		if( v_ref <= 0.0 )
		{
			// Fall back: use current volume as reference if the parameter
			// is not exposed (get_parameter_value returns 0 for unknown names)
			v_ref = pC->phenotype.volume.total;
		}

		// Random fraction in [0.5, 1.0] — cells span the full first half of
		// the cell cycle at t = 0, eliminating the synchronised first wave.
		double frac = 0.5 + 0.5 * UniformRandom();
		double new_vol = frac * v_ref;
		if( new_vol > 0.0 )
		{
			pC->set_total_volume( new_vol );
		}
	}

	return; 
}

// Cell-type specific intracellular update function for C. beijerinckii
void post_update_intracellular_c_beijerinckii(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt) {
	// Keep a unified output schema across cell types (as declared in PhysiCell_settings_experiment_2.xml)
	// by always populating the same custom_data keys. For this type, MB_* keys are set to 0.
	pCell->custom_data["MB_h2_flux"] = 0.0;
	pCell->custom_data["MB_co2_flux"] = 0.0;
	pCell->custom_data["MB_acetate_flux"] = 0.0;
	pCell->custom_data["MB_growth_rate"] = 0.0;
	pCell->custom_data["MB_fba_infeasible"] = 0.0;
	pCell->custom_data["methane_flux"] = 0.0;
	
	// Debug: Print growth rate and biomass flux for early timepoints
	if (PhysiCell_globals.current_time < 2.0) {
		double biomass_flux = 0.0;
		try {
			if (pCell->phenotype.intracellular)
			{
				biomass_flux = pCell->phenotype.intracellular->get_flux_value("R_biomass");
			}
		} catch (const std::exception& e) {
			std::cout << "Error getting biomass flux for c_beijerinckii: " << e.what() << std::endl;
		}
		
		// std::cout << "Time: " << PhysiCell_globals.current_time 
				//   << ", C. beijerinckii ID: " << pCell->ID 
				//   << ", Growth rate: " << pCell->custom_data["growth_rate"] 
				//   << ", Biomass flux: " << biomass_flux << std::endl;
	}
	
	// Set metabolic fluxes specific to C. beijerinckii
	try {
		if (pCell->phenotype.intracellular)
		{
			// Transport model in exp2.xml (C. beijerinckii):
			// glucose: R_EX_glc_D_e, H2: R_EX_h2_e, CO2: R_EX_co2_e, acetate: R_EX_ac_e
			pCell->custom_data["glucose_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_glc_D_e");
			pCell->custom_data["CB_h2_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_h2_e");
			pCell->custom_data["CB_co2_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_co2_e");
			pCell->custom_data["CB_acetate_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_ac_e");
			pCell->custom_data["CB_growth_rate"] = pCell->phenotype.intracellular->get_growth_rate();
			pCell->custom_data["CB_fba_infeasible"] = get_dfba_infeasible_flag_for_cell(pCell);
		}
		else
		{
			pCell->custom_data["glucose_flux"] = 0.0;
			pCell->custom_data["CB_h2_flux"] = 0.0;
			pCell->custom_data["CB_co2_flux"] = 0.0;
			pCell->custom_data["CB_acetate_flux"] = 0.0;
			pCell->custom_data["CB_growth_rate"] = 0.0;
			pCell->custom_data["CB_fba_infeasible"] = 0.0;
		}
		
		// Debug fluxes for early timepoints (commented out for performance)
		// if (PhysiCell_globals.current_time < 5.0) {
		// 	std::cout << "C.beijerinckii ID " << pCell->ID 
		// 			  << ": Glucose=" << pCell->custom_data["glucose_flux"] 
		// 			  << ", H2=" << pCell->custom_data["CB_h2_flux"] << std::endl;
		// }
	} catch (const std::exception& e) {
		std::cout << "Error accessing flux values for c_beijerinckii: " << e.what() << std::endl;
	}
	
	return;
}

// Cell-type specific intracellular update function for M. barkeri
void post_update_intracellular_m_barkeri(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt) {
	// Keep unified output schema. For this type, CB_* keys are set to 0.
	pCell->custom_data["glucose_flux"] = 0.0;
	pCell->custom_data["CB_h2_flux"] = 0.0;
	pCell->custom_data["CB_co2_flux"] = 0.0;
	pCell->custom_data["CB_acetate_flux"] = 0.0;
	pCell->custom_data["CB_growth_rate"] = 0.0;
	pCell->custom_data["CB_fba_infeasible"] = 0.0;
	
	// Debug: Print growth rate and biomass flux for early timepoints
	if (PhysiCell_globals.current_time < 2.0) {
		double biomass_flux = 0.0;
		try {
			// SBML objective / biomass reaction for M. barkeri
			if (pCell->phenotype.intracellular)
			{
				biomass_flux = pCell->phenotype.intracellular->get_flux_value("R_Mb_biomass_65");
			}
		} catch (const std::exception& e) {
			std::cout << "Error getting biomass flux for m_barkeri: " << e.what() << std::endl;
		}
		
		// std::cout << "Time: " << PhysiCell_globals.current_time 
		// 		  << ", M. barkeri ID: " << pCell->ID 
		// 		  << ", Growth rate: " << pCell->custom_data["growth_rate"] 
		// 		  << ", Biomass flux: " << biomass_flux << std::endl;
	}
	
	// Set metabolic fluxes specific to M. barkeri
	try {
		if (pCell->phenotype.intracellular)
		{
			// Transport model in exp2.xml (M. barkeri):
			// CO2: R_EX_co2_e, H2: R_EX_h2_e, methane: R_EX_ch4_e, acetate: R_EX_ac_e
			pCell->custom_data["MB_h2_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_h2_e");
			pCell->custom_data["MB_co2_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_co2_e");
			pCell->custom_data["MB_acetate_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_ac_e");
			pCell->custom_data["methane_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_ch4_e");
			pCell->custom_data["MB_growth_rate"] = pCell->phenotype.intracellular->get_growth_rate();
			pCell->custom_data["MB_fba_infeasible"] = get_dfba_infeasible_flag_for_cell(pCell);
		}
		else
		{
			pCell->custom_data["MB_h2_flux"] = 0.0;
			pCell->custom_data["MB_co2_flux"] = 0.0;
			pCell->custom_data["MB_acetate_flux"] = 0.0;
			pCell->custom_data["methane_flux"] = 0.0;
			pCell->custom_data["MB_growth_rate"] = 0.0;
			pCell->custom_data["MB_fba_infeasible"] = 0.0;
		}
		
		// Debug fluxes for early timepoints (commented out for performance)
		// if (PhysiCell_globals.current_time < 10.0) {
		// 	std::cout << "M.barkeri ID " << pCell->ID 
		// 			  << ": H2=" << pCell->custom_data["MB_h2_flux"]
		// 			  << ", CO2=" << pCell->custom_data["MB_co2_flux"]
		// 			  << ", CH4=" << pCell->custom_data["methane_flux"] << std::endl;
		// }
	} catch (const std::exception& e) {
		std::cout << "Error accessing flux values for m_barkeri: " << e.what() << std::endl;
	}
	
	return;
}

// Generic post_update_intracellular function that dispatches to cell-type specific functions
void post_update_intracellular(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt) {
	// Dispatch to the appropriate cell-type specific function
	if (pCell->type_name == "c_beijerinckii") {
		post_update_intracellular_c_beijerinckii(pCell, phenotype, dt);
	} else if (pCell->type_name == "m_barkeri") {
		post_update_intracellular_m_barkeri(pCell, phenotype, dt);
	} else {
		// Default behavior for unknown cell types
		std::cout << "Warning: Unknown cell type '" << pCell->type_name 
				  << "' in post_update_intracellular" << std::endl;
		pCell->custom_data["growth_rate"] = pCell->phenotype.intracellular->get_growth_rate();
	}
	
	return;
}


void apply_initial_glucose_refeed_setup(void)
{
	if (parameters.bools.find_index("glucose_refeed_enabled") == -1 ||
		!parameters.bools("glucose_refeed_enabled"))
	{
		return;
	}
	std::string sub_name = "glucose";
	if (parameters.strings.find_index("glucose_refeed_substrate") != -1 &&
		!parameters.strings("glucose_refeed_substrate").empty())
	{
		sub_name = parameters.strings("glucose_refeed_substrate");
	}
	int gi = microenvironment.find_density_index(sub_name);
	if (gi < 0)
	{
		std::cout << "Warning: apply_initial_glucose_refeed_setup: substrate \"" << sub_name << "\" not found." << std::endl;
		return;
	}
	if (parameters.bools.find_index("glucose_refeed_start_with_dirichlet_off") != -1 &&
		parameters.bools("glucose_refeed_start_with_dirichlet_off"))
	{
		microenvironment.set_substrate_dirichlet_activation(gi, false);
		std::cout << "Glucose refeed: initial Dirichlet activation OFF for \"" << sub_name << "\" (index " << gi << ")." << std::endl;
	}
}

void apply_glucose_dirichlet_refeed_schedule(void)
{
	double t = PhysiCell_globals.current_time;

	if (parameters.bools.find_index("glucose_refeed_enabled") != -1 && parameters.bools("glucose_refeed_enabled"))
	{
		std::string sub_name = "glucose";
		if (parameters.strings.find_index("glucose_refeed_substrate") != -1 &&
			!parameters.strings("glucose_refeed_substrate").empty())
		{
			sub_name = parameters.strings("glucose_refeed_substrate");
		}
		int gi = microenvironment.find_density_index(sub_name);
		if (gi < 0)
		{
			return;
		}

		double first_pulse = 0.0;
		if (parameters.doubles.find_index("glucose_refeed_first_pulse_time_min") != -1)
		{
			first_pulse = parameters.doubles("glucose_refeed_first_pulse_time_min");
		}
		double pulse_dur = 120.0;
		if (parameters.doubles.find_index("glucose_refeed_pulse_duration_min") != -1)
		{
			pulse_dur = parameters.doubles("glucose_refeed_pulse_duration_min");
		}
		double period = 0.0;
		if (parameters.doubles.find_index("glucose_refeed_period_min") != -1)
		{
			period = parameters.doubles("glucose_refeed_period_min");
		}
		double pulse_mM = 1.0;
		if (parameters.doubles.find_index("glucose_refeed_pulse_dirichlet_mM") != -1)
		{
			pulse_mM = parameters.doubles("glucose_refeed_pulse_dirichlet_mM");
		}

		bool want_on = glucose_refeed_pulse_active(t, first_pulse, period, pulse_dur);
		bool on_now = microenvironment.get_substrate_dirichlet_activation(gi);

		if (want_on && !on_now)
		{
			set_substrate_dirichlet_value_on_all_dirichlet_voxels(gi, pulse_mM);
			microenvironment.set_substrate_dirichlet_activation(gi, true);
			std::cout << sub_name << " refeed: Dirichlet ON at t=" << t << " min, boundary target " << pulse_mM << " mM" << std::endl;
		}
		else if (!want_on && on_now)
		{
			microenvironment.set_substrate_dirichlet_activation(gi, false);
			std::cout << sub_name << " refeed: Dirichlet OFF at t=" << t << " min" << std::endl;
		}
		return;
	}

	if (parameters.bools.find_index("nutrient_reintroduction") != -1)
	{
		if (!parameters.bools("nutrient_reintroduction") &&
			parameters.strings.find_index("reintroduced_nutrient") != -1)
		{
			int ni = microenvironment.find_density_index(parameters.strings("reintroduced_nutrient"));
			if (ni >= 0 && microenvironment.get_substrate_dirichlet_activation(ni))
			{
				microenvironment.set_substrate_dirichlet_activation(ni, false);
			}
			return;
		}
		if (parameters.bools("nutrient_reintroduction"))
		{
			apply_legacy_nutrient_reintroduction_window();
		}
	}
}

std::vector<std::vector<double>> create_cell_sphere_positions(double cell_radius, double sphere_radius)
{
	std::vector<std::vector<double>> cells;
	int xc=0,yc=0,zc=0;
	double x_spacing= cell_radius*sqrt(3);
	double y_spacing= cell_radius*2;
	double z_spacing= cell_radius*sqrt(3);
	
	std::vector<double> tempPoint(3,0.0);
	// std::vector<double> cylinder_center(3,0.0);
	
	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
	{
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
		{
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0]=x + (zc%2) * 0.5 * cell_radius;
				tempPoint[1]=y + (xc%2) * cell_radius;
				tempPoint[2]=z;
				
				if(sqrt(norm_squared(tempPoint))< sphere_radius)
				{ cells.push_back(tempPoint); }
			}
			
		}
	}
	return cells;
	
}


std::vector<std::vector<double>> create_cell_disc_positions(double cell_radius, double disc_radius)
{	 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	double x = 0.0; 
	double y = 0.0; 
	double x_outer = 0.0;

	std::vector<std::vector<double>> positions;
	std::vector<double> tempPoint(3,0.0);
	
	int n = 0; 
	while( y < disc_radius )
	{
		x = 0.0; 
		if( n % 2 == 1 )
		{ x = 0.5 * cell_spacing; }
		x_outer = sqrt( disc_radius*disc_radius - y*y ); 
		
		while( x < x_outer )
		{
			tempPoint[0]= x; tempPoint[1]= y;	tempPoint[2]= 0.0;
			positions.push_back(tempPoint);			
			if( fabs( y ) > 0.01 )
			{
				tempPoint[0]= x; tempPoint[1]= -y;	tempPoint[2]= 0.0;
				positions.push_back(tempPoint);
			}
			if( fabs( x ) > 0.01 )
			{ 
				tempPoint[0]= -x; tempPoint[1]= y;	tempPoint[2]= 0.0;
				positions.push_back(tempPoint);
				if( fabs( y ) > 0.01 )
				{
					tempPoint[0]= -x; tempPoint[1]= -y;	tempPoint[2]= 0.0;
					positions.push_back(tempPoint);
				}
			}
			x += cell_spacing; 
		}		
		y += cell_spacing * sqrt(3.0)/2.0; 
		n++; 
	}
	return positions;
}


std::vector<std::string> my_coloring_function( Cell* pCell )
{

	std::vector<std::string> output(4, "red");
        
	double flux_value =  0.0;
	if( abs(flux_value) >= 0.0 )
	{
		output[0] = "blue";
		output[2] = "blue";
		return output;
	}
	else
	{
		output[0] = "green";
		output[2] = "green";
	}

	return output;
}
