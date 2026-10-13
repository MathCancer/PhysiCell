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

// Include dFBA header before custom.h to avoid libSBML/PhysiCell 'Parameter' name collision
#ifdef ADDON_PHYSIDFBA
#include "../addons/dFBA/src/dfba_intracellular.h"
#endif
#include "custom.h"
#include <unordered_set>


// Prefer per-cell custom_data trait; fall back to global user_parameter if unset/non-positive.
static double cell_trait_or_param(Cell* pCell, const char* custom_key, const char* param_key)
{
    double val = pCell->custom_data[custom_key];
    if (val > 0.0)
        return val;
    return parameters.doubles(param_key);
}

void custom_atp_optimization(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt)
{
    auto* dfba =
        static_cast<PhysiCelldFBA::dFBAIntracellular*>(pCell->phenotype.intracellular);

    // basal_atp_flux should match the SBML R_ATPM lower bound (8.39 for E. coli core).
    // Only ATP *above* this floor is "free" for motility; below it the FBA problem
    // becomes infeasible and the cell dies anyway.
    static double basal_atp_flux = parameters.doubles("basal_atp_flux");
    // mmol ATP / gDW / h
    static double phi_atp_hill = parameters.doubles("phi_atp_hill");
    // Hill exponent shaping ATP fraction to speed (1=linear, n>1=sigmoidal)

    // Per-cell heterogeneity (sampled via initial_parameter_distributions on seekers).
    double vmax = cell_trait_or_param(pCell, "ecoli_vmax", "ecoli_vmax");
    // micron / min
    double motility_cost_at_vmax =
        cell_trait_or_param(pCell, "motility_cost_at_vmax", "motility_cost_at_vmax");
    // mmol ATP / gDW / h — surplus above basal needed to reach vmax

    dFBASolution solution = dfba->optimize_for_objective("R_ATPM", 1.0);

    if (solution.status == "optimal")
    {
        double atp_flux = solution.getObjectiveValue();
        // mmol ATP / gDW / h

        double motility_atp_flux = std::max(0.0, atp_flux - basal_atp_flux);
        // mmol ATP / gDW / h — surplus above basal maintenance

        double speed_fraction = 0.0;
        if (motility_cost_at_vmax > 0.0)
            speed_fraction = motility_atp_flux / motility_cost_at_vmax;
        speed_fraction = std::max(0.0, std::min(1.0, speed_fraction));

        // Hill shaping: spreads the ATP→speed response across the gradient
        if (phi_atp_hill != 1.0)
            speed_fraction = std::pow(speed_fraction, phi_atp_hill);

        pCell->custom_data["atp_flux"]              = atp_flux;
        pCell->custom_data["motility_atp_flux"]     = motility_atp_flux;
        pCell->custom_data["motility_speed_fraction"] = speed_fraction;

        phenotype.motility.migration_speed = vmax * speed_fraction;

        dfba->current_growth_rate = dfba->sbml_model.getReaction("R_Biomass_Ecoli_core")->getFluxValue();
        dfba->flag_for_death = false;
    }
    else if (solution.status == "unknown")
    {
        std::cout << "ERROR: Unknown status for ATP optimization!" << std::endl;
        exit(1);
    }
    else
    {
        pCell->custom_data["atp_flux"]               = 0.0;
        pCell->custom_data["motility_atp_flux"]      = 0.0;
        pCell->custom_data["motility_speed_fraction"] = 0.0;

        phenotype.motility.migration_speed = 0.0;

        dfba->flag_for_death = true;
        dfba->current_growth_rate = 0.0;
    }
}

void create_cell_types(void)
{
	SeedRandom(parameters.ints("random_seed"));

	initialize_default_cell_definition();

	/*  This parses the cell definitions in the XML config file.  */
	initialize_cell_definitions_from_pugixml();

	//  This sets the pre and post intracellular update functions
	cell_defaults.functions.pre_update_intracellular =  NULL;
	cell_defaults.functions.post_update_intracellular = post_update_intracellular;
	cell_defaults.functions.update_phenotype = NULL; 
	cell_defaults.functions.volume_update_function = NULL;

	build_cell_definitions_maps();
	
	setup_signal_behavior_dictionaries();

	// Legacy single-type motility configs still use name "ecoli".
	Cell_Definition* ecoli = find_cell_definition("ecoli");
	if (ecoli)
	{
		ecoli->functions.pre_update_intracellular = NULL;
		ecoli->functions.post_update_intracellular = post_update_intracellular;
		ecoli->functions.custom_optimization = NULL; // set by metabolic_bound_migration_rule
		ecoli->functions.update_phenotype = NULL;
		ecoli->functions.volume_update_function = NULL;
		ecoli->functions.custom_cell_rule = metabolic_bound_migration_rule;
	}

	// Growers: biomass only — no motility phenotypic switch.
	Cell_Definition* grower = find_cell_definition("ecoli_grower");
	if (grower)
	{
		grower->functions.pre_update_intracellular = NULL;
		grower->functions.post_update_intracellular = post_update_intracellular;
		grower->functions.custom_optimization = NULL;
		grower->functions.update_phenotype = NULL;
		grower->functions.volume_update_function = NULL;
		grower->functions.custom_cell_rule = NULL;
	}

	// Seekers: hysteretic biomass ↔ ATP/motility switch.
	Cell_Definition* seeker = find_cell_definition("ecoli_seeker");
	if (seeker)
	{
		seeker->functions.pre_update_intracellular = NULL;
		seeker->functions.post_update_intracellular = post_update_intracellular;
		seeker->functions.custom_optimization = NULL; // set by metabolic_bound_migration_rule
		seeker->functions.update_phenotype = NULL;
		seeker->functions.volume_update_function = NULL;
		seeker->functions.custom_cell_rule = metabolic_bound_migration_rule;
	}

	display_cell_definitions(std::cout);

	return;
}




// File-scope statics required because bulk_supply_*_function are raw function
// pointers and cannot bind capturing lambdas.
static int                     _src_glucose_idx  = -1;
static std::vector<int>        _src_voxels;            // all active source voxel indices
static std::unordered_set<int> _src_voxel_set;         // fast membership test for lambdas
static double                  _src_glucose_conc = 0.0;
static double                  _src_glucose_supply_rate = 0.0;

// Register four glucose secretion nodes at fixed positions.
// Target concentration (mM) and supply rate (1/min) are set separately so
// patches can be held near target even when surrounded by uptake.
// Call this from main.cpp AFTER setup_microenvironment().
void setup_secretion_nodes( void )
{
	_src_glucose_idx = microenvironment.find_density_index( "glucose" );

	// Source centre positions
	std::vector<std::vector<double>> centers = {
		{-90.0,  90.0, 0.0},
		{-90.0, -90.0, 0.0},
		{ 90.0,  90.0, 0.0},
		{ 90.0, -90.0, 0.0}
	};

	// Neighbourhood radius: 1 = single voxel, 2 = Moore (3x3), 3 = 5x5, ...
	int radius = parameters.ints("glucose_source_radius");
	double dx  = microenvironment.mesh.dx;
	double dy  = microenvironment.mesh.dy;

	_src_voxels.clear();
	_src_voxel_set.clear();

	for( auto& c : centers )
	{
		for( int di = -(radius - 1); di <= (radius - 1); di++ )
		{
			for( int dj = -(radius - 1); dj <= (radius - 1); dj++ )
			{
				std::vector<double> p = { c[0] + di * dx, c[1] + dj * dy, c[2] };
				int idx = microenvironment.nearest_voxel_index( p );
				if( idx >= 0 && _src_voxel_set.find(idx) == _src_voxel_set.end() )
				{
					_src_voxel_set.insert( idx );
					_src_voxels.push_back( idx );
				}
			}
		}
	}

	_src_glucose_conc = parameters.doubles("glucose_source_concentration");
	if (parameters.doubles.find_index("glucose_source_supply_rate") != -1)
		_src_glucose_supply_rate = parameters.doubles("glucose_source_supply_rate");
	else
		_src_glucose_supply_rate = _src_glucose_conc;

	std::cout << "[setup_secretion_nodes] glucose substrate index : " << _src_glucose_idx << std::endl;
	std::cout << "[setup_secretion_nodes] source glucose conc     : " << _src_glucose_conc << " mM" << std::endl;
	std::cout << "[setup_secretion_nodes] source supply rate      : " << _src_glucose_supply_rate << " 1/min" << std::endl;
	std::cout << "[setup_secretion_nodes] source radius (layers)  : " << radius << std::endl;
	std::cout << "[setup_secretion_nodes] total source voxels     : " << _src_voxels.size() << std::endl;
	for( int k = 0; k < (int)_src_voxels.size(); k++ )
	{
		double cx = microenvironment.voxels(_src_voxels[k]).center[0];
		double cy = microenvironment.voxels(_src_voxels[k]).center[1];
		std::cout << "[setup_secretion_nodes]   voxel " << k
		          << ": index=" << _src_voxels[k]
		          << "  center=(" << cx << ", " << cy << ")" << std::endl;
	}

	microenvironment.bulk_supply_rate_function = [](
		Microenvironment*, int n, std::vector<double>* dest )
	{
		(*dest)[_src_glucose_idx] =
			(_src_voxel_set.count(n) > 0) ? _src_glucose_supply_rate : 0.0;
	};

	microenvironment.bulk_supply_target_densities_function = [](
		Microenvironment*, int n, std::vector<double>* dest )
	{
		(*dest)[_src_glucose_idx] =
			(_src_voxel_set.count(n) > 0) ? _src_glucose_conc : 0.0;
	};
}

void setup_microenvironment(void)
{
	initialize_microenvironment();
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
	std::cout << std::endl; 
	
	// load cells from your CSV file
	load_cells_from_pugixml();

	// Sample per-cell traits from <initial_parameter_distributions> (seekers).
	set_parameters_from_distributions();
	
	return; 
}

void post_update_intracellular(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt )
{
#ifdef ADDON_PHYSIDFBA
	// XML death_model may be enabled (core accumulates death_rate_increase while
	// flag_for_death). Cap the resulting rates so phenotype_dt checks stay
	// stochastic (UniformRandom) instead of mass-killing every starved cell at once.
	auto* dfba = static_cast<PhysiCelldFBA::dFBAIntracellular*>(pCell->phenotype.intracellular);
	if (dfba != nullptr && phenotype.death.dead == false)
	{
		static double metabolic_death_rate_cap = 0.05; // 1/min
		static bool death_rate_loaded = false;
		if (!death_rate_loaded)
		{
			if (parameters.doubles.find_index("metabolic_death_rate") >= 0)
				metabolic_death_rate_cap = parameters.doubles("metabolic_death_rate");
			death_rate_loaded = true;
		}

		auto cap_rate = [&](int idx)
		{
			if (idx >= 0 && idx < (int)phenotype.death.rates.size()
				&& phenotype.death.rates[idx] > metabolic_death_rate_cap)
			{
				phenotype.death.rates[idx] = metabolic_death_rate_cap;
			}
		};
		cap_rate(phenotype.death.find_death_model_index(
			PhysiCell_constants::apoptosis_death_model));
		cap_rate(phenotype.death.find_death_model_index(
			PhysiCell_constants::necrosis_death_model));

		if (dfba->flag_for_death)
		{
			for (int i = 0; i < (int)phenotype.secretion.net_export_rates.size(); i++)
				phenotype.secretion.net_export_rates[i] = 0.0;

			pCell->custom_data["growth_rate"] = 0.0;
			pCell->custom_data["oxygen_flux"] = 0.0;
			pCell->custom_data["glucose_flux"] = 0.0;
			pCell->custom_data["acetate_flux"] = 0.0;
			pCell->custom_data["co2_flux"] = 0.0;
			return;
		}
	}
#endif

	pCell->custom_data["growth_rate"] = pCell->phenotype.intracellular->get_growth_rate();
	pCell->custom_data["oxygen_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_o2_e");
	pCell->custom_data["glucose_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_glc__D_e");
	pCell->custom_data["acetate_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_ac_e");
	pCell->custom_data["co2_flux"] = pCell->phenotype.intracellular->get_flux_value("R_EX_co2_e");
	return;
}


void inject_density(int density_index, double concentration)
{
	// Inject given concentration on the extremities only
	#pragma omp parallel for
	for (int n = 0; n < microenvironment.number_of_voxels(); n++)
	{
		auto current_voxel = microenvironment.voxels(n);
		microenvironment.density_vector(n)[density_index] = concentration;
	}
}

void metabolic_bound_migration_rule(Cell* pCell, Phenotype& phenotype, double dt)
{
    static int glucose_index = microenvironment.find_density_index("glucose");
    double glucose_conc = pCell->nearest_density_vector()[glucose_index];

    // Per-cell thresholds (heterogeneity via custom:* distributions); global fallback.
    double glucose_threshold_low =
        cell_trait_or_param(pCell, "glucose_threshold_low", "glucose_threshold_low");
    double glucose_threshold_high =
        cell_trait_or_param(pCell, "glucose_threshold_high", "glucose_threshold_high");
    if (glucose_threshold_high < glucose_threshold_low)
        std::swap(glucose_threshold_high, glucose_threshold_low);

    double mode = pCell->custom_data["metabolic_mode"];
    // 0 = biomass mode, 1 = ATP mode

    // Hysteretic mode switch only.  Speed is set inside custom_atp_optimization
    // at every intracellular step (intracellular_dt), so there is no stale read here.
    if (mode < 0.5 && glucose_conc < glucose_threshold_low)
    {
        pCell->custom_data["metabolic_mode"] = 1.0;
        pCell->functions.custom_optimization = custom_atp_optimization;
    }
    else if (mode > 0.5 && glucose_conc > glucose_threshold_high)
    {
        pCell->custom_data["metabolic_mode"] = 0.0;
        pCell->functions.custom_optimization = NULL;

        // Back to growth mode: clear ATP-motility state and stop moving.
        pCell->custom_data["atp_flux"]               = 0.0;
        pCell->custom_data["motility_atp_flux"]      = 0.0;
        pCell->custom_data["motility_speed_fraction"] = 0.0;
        phenotype.motility.migration_speed = 0.0;

        // Do not std::cout here: this rule runs under OpenMP and concurrent
        // iostream use garbles logs and can destabilize large runs.
    }

    return;
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

    std::string fba_flux_id = "R_EX_o2_e";

	if( pCell->phenotype.intracellular == nullptr )
	{
		return output;
	}

	double flux_value =  pCell->phenotype.intracellular->get_flux_value(fba_flux_id);

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
