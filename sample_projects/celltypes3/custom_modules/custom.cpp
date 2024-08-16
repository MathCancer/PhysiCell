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

#include "./custom.h"

void create_cell_types( void )
{
	// set the random seed 
	if (parameters.ints.find_index("random_seed") != -1)
	{
		SeedRandom(parameters.ints("random_seed"));
	}
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	initialize_default_cell_definition(); 
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 
	
	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 

	get_cell_definition("A").functions.update_phenotype = A_phenotype; 
	get_cell_definition("B").functions.update_phenotype = B_phenotype; 
	get_cell_definition("C").functions.update_phenotype = C_phenotype; 
		
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 

	/*
	   This intializes cell signal and response dictionaries 
	*/

	setup_signal_behavior_dictionaries(); 	

	/*
       Cell rule definitions 
	*/

	setup_cell_rules(); 



	display_cell_definitions( std::cout ); 
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
	double Xmin = microenvironment.mesh.bounding_box[0]; 
	double Ymin = microenvironment.mesh.bounding_box[1]; 
	double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	double max_radius = parameters.doubles("max_distance_from_origin");
	if( Xmax > max_radius )
	{ Xmax = max_radius; }
	if( Xmin < -max_radius )
	{ Xmin = -max_radius; }
	
	if( Ymax > max_radius )
	{ Ymax = max_radius; }
	if( Ymin < -max_radius )
	{ Ymin = -max_radius; }

	if( Zmax > max_radius )
	{ Zmax = max_radius; }
	if( Zmin < -max_radius )
	{ Zmin = -max_radius; }
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}

	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin; 

//	Xmin += 0.25*Xrange; 
//	Xrange *= 0.5;

//	Ymin += 0.25*Yrange; 
//	Yrange *= 0.5;

//	Zmin += 0.25*Zrange; 
//	Zrange *= 0.5; 
	// create some of each type of cell 
	
	Cell* pC;
	
	// place A
	
	for( int n = 0 ; n < parameters.ints("number_of_A") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		
		double r = max_radius + 1; 
		while( r > max_radius )
		{
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			r = norm( position ); 
		}
		
		pC = create_cell( get_cell_definition("A") ); 
		pC->assign_position( position );
		for( int k=0 ; k < pC->phenotype.death.rates.size() ; k++ )
		{ pC->phenotype.death.rates[k] = 0.0; }
	}
	
	// place B
	
	for( int n = 0 ; n < parameters.ints("number_of_B") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 
		
		double r = max_radius + 1; 
		while( r > max_radius )
		{
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			r = norm( position ); 
		}
		
		pC = create_cell( get_cell_definition("B") ); 
		pC->assign_position( position );
		for( int k=0 ; k < pC->phenotype.death.rates.size() ; k++ )
		{ pC->phenotype.death.rates[k] = 0.0; }
	}

	// place C
	
	for( int n = 0 ; n < parameters.ints("number_of_C") ; n++ )
	{
		std::vector<double> position = {0,0,0}; 

		double r = max_radius + 1; 
		while( r > max_radius )
		{
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			r = norm( position ); 
		}
		
		pC = create_cell( get_cell_definition("C") ); 
		pC->assign_position( position );
		for( int k=0 ; k < pC->phenotype.death.rates.size() ; k++ )
		{ pC->phenotype.death.rates[k] = 0.0; }
	}

	return; 
}

std::vector<std::string> regular_colors( Cell* pCell )
{
	static int A_type = get_cell_definition( "A" ).type; 
	static int B_type = get_cell_definition( "B" ).type; 
	static int C_type = get_cell_definition( "C" ).type; 
	
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = {"black" , "black" , "black" , "black"} ;

	// color live C 
		
	if( pCell->type == A_type )
	{
		 output[0] = parameters.strings("A_color");  
		 output[2] = parameters.strings("A_color");  
	}
	
	// color live B

	if( pCell->type == B_type )
	{
		 output[0] = parameters.strings("B_color");  
		 output[2] = parameters.strings("B_color");  
	}
	
	// color live C

	if( pCell->type == C_type )
	{
		 output[0] = parameters.strings("C_color");  
		 output[2] = parameters.strings("C_color");  
	}
	
	if( pCell->phenotype.death.dead == true )
	{
		// Necrotic - Brown
		if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
		{
			output[2] = "chocolate";
		}
		else
		{
			output[2] = "black"; 
		}
	}

	return output; 
}

std::vector<std::string> pseudo_fluorescence( Cell* pCell )
{
	static int A_type = get_cell_definition( "A" ).type; 
	static int B_type = get_cell_definition( "B" ).type; 
	static int C_type = get_cell_definition( "C" ).type; 
	
	static int nA = microenvironment.find_density_index( "signal A" ); 
	static int nB = microenvironment.find_density_index( "signal B" ); 
	static int nC = microenvironment.find_density_index( "signal C" ); 
	
	static Cell_Definition* pCD_A  = find_cell_definition("A");
	static Cell_Definition* pCD_B  = find_cell_definition("B");
	static Cell_Definition* pCD_C  = find_cell_definition("C");
	
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = {"black" , "black" , "black" , "black"} ;
	
	char color [32]; 
	double max_fluorescence = 1; // 

	double value = 0.0; 

	// color live A
		
	if( pCell->type == A_type )
	{
		value = pCell->phenotype.secretion.secretion_rates[nA] 
			/ ( 0.001 + pCD_A->phenotype.secretion.secretion_rates[nA] ) ;
			
		value *= (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence;  
		if( pCell->phenotype.death.dead == true )
		{ value = (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence; }
		sprintf( color, "rgba(255,0,255,%f)", value ); 		
	}
	
	// color live B

	if( pCell->type == B_type )
	{
		value = pCell->phenotype.secretion.secretion_rates[nB] 
			/ ( 0.001 + pCD_B->phenotype.secretion.secretion_rates[nB] ); 
		value *= (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence;  
		if( pCell->phenotype.death.dead == true )
		{ value = (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence; }
		sprintf( color, "rgba(0,255,0,%f)", value ); 		
	}
	
	// color live C

	if( pCell->type == C_type )
	{
		value = pCell->phenotype.secretion.secretion_rates[nC] 
			/ ( 0.001 + pCD_C->phenotype.secretion.secretion_rates[nC] ); 
		value *= (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence;  
		if( pCell->phenotype.death.dead == true )
		{ value = (1.0-pCell->phenotype.volume.fluid_fraction) * max_fluorescence; }
		sprintf( color, "rgba(0,255,255,%f)", value ); 		
	}

	// Necrotic - black
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{ sprintf(color,"rgba(0,0,0,%f)" , value ); } 

	output = { color, "none", color , "none" }; 
	return output; 
}

up_down_signal::up_down_signal()
{
    up = 0.0;
	down = 0.0; 
	
	base_parameter = 0.0;
	max_parameter = 1.0; 

    no_promoters = true; 
    no_inhibitors = true; 
	return; 
} 

void up_down_signal::add_effect( double factor, char factor_type )
{
	// neutral signal 
	if( factor_type == 'N' || factor_type == 'n' )
	{ return; }

	// promoter signal 
	if( factor_type == 'P' || factor_type == 'p' )
	{
		// up = sum of all (scaled) promoter signals 
		up += factor; 
		no_promoters = false; 
		return; 
	}

	// inhibitor signal 
	if( factor_type == 'I' || factor_type == 'i' )
	{
		down += factor;
		no_inhibitors = false; 
		return; 
	}

	return; 
}

void up_down_signal::add_effect( double factor, std::string factor_type )
{
	this->add_effect( factor, factor_type[0] ); 
	return; 
}

double up_down_signal::compute_effect_hill( void )
{
	static double hill_power = parameters.doubles( "hill_power" ); 
	static double half_max = parameters.doubles( "half_max" ); 
	static double denom_constant = pow( half_max, hill_power );
	
	double temp = pow( up , hill_power ); 
	double UP = temp / ( denom_constant + temp ); 
	if( no_promoters )
	{ UP = 0.0; }
	
	temp = pow( down , hill_power ); 
	double DOWN = denom_constant / ( denom_constant + temp ); 
	if( no_inhibitors )
	{ DOWN = 1.0; }

	// return UP * DOWN; 
	return (base_parameter + (max_parameter-base_parameter)*UP) * DOWN; 
}

double up_down_signal::compute_effect( void )
{ return this->compute_effect_hill(); }

void up_down_signal::reset( void )
{ 
	up = 0.0;
	down = 0.0; 
	no_promoters = true; 
	no_inhibitors = true; 
	
	base_parameter = 0.0;
	max_parameter = 1.0; 
	
	return; 
}

void up_down_signal::display( void )
{
	std::cout << "up    : " << up <<   " (no promoters : " << (int) no_promoters << ")" << std::endl;
	std::cout << "down  : " << down << " (no inhibiters: " << (int) no_inhibitors << ")" << std::endl;

	std::cout << "effect: " << compute_effect() << std::endl; 

	std::cout << std::endl; 
	return; 
}

void A_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// housekeeping 
	static Cell_Definition* pCD  = find_cell_definition("A");
	static int nApoptosis = pCD->phenotype.death.find_death_model_index( "Apoptosis"); 
	static int nNecrosis  = pCD->phenotype.death.find_death_model_index( "Necrosis"); 

	if( phenotype.death.dead == true )
	{

		phenotype.secretion.set_all_secretion_to_zero(); 
		phenotype.secretion.set_all_uptake_to_zero(); 
		phenotype.motility.is_motile = false; 

		pCell->functions.update_phenotype = NULL; 
		return; 
	}
	
	// sample A, B, C, resource, and pressure 
	static int nA = microenvironment.find_density_index( "signal A" ); 
	static int nB = microenvironment.find_density_index( "signal B" ); 
	static int nC = microenvironment.find_density_index( "signal C" ); 
	static int nR = microenvironment.find_density_index( "resource" ); 

	double A = pCell->nearest_density_vector()[nA];
	double B = pCell->nearest_density_vector()[nB];
	double C = pCell->nearest_density_vector()[nC];
	double R = pCell->nearest_density_vector()[nR];
	double p = pCell->state.simple_pressure; 

	// necrotic death rate 
	static double base_necrosis_rate = pCD->phenotype.death.rates[nNecrosis];
	static double necrosis_threshold = parameters.doubles("A_necrosis_threshold");
	phenotype.death.rates[nNecrosis] = 0.0;
	
	if( R < necrosis_threshold )
	{
		phenotype.death.rates[nNecrosis] = base_necrosis_rate; 
		phenotype.death.rates[nNecrosis] *= (1.0 - R / necrosis_threshold);
	}

	// cycle rate 
	double param0 = parameters.doubles("A_base_cycle") * R; 

	up_down_signal sig; 
	sig.base_parameter = param0; 
	sig.max_parameter = parameters.doubles("A_max_cycle");

	// A 
	sig.add_effect( A , parameters.strings("A_cycle_A") );
	// B
	sig.add_effect( B , parameters.strings("A_cycle_B") );
	// C 
	sig.add_effect( C , parameters.strings("A_cycle_C") );
	
	phenotype.cycle.data.transition_rate(0,0) = sig.compute_effect(); 
	if( p > parameters.doubles( "A_cycle_pressure_threshold") )
	{ phenotype.cycle.data.transition_rate(0,0) = 0.0; }

	// apoptotic rate 
	
	static double base_death_rate = parameters.doubles("A_base_death"); 
	static double max_death_rate = parameters.doubles("A_max_death");
	sig.reset();
	sig.base_parameter = base_death_rate;
	sig.max_parameter = max_death_rate; 

	// A 
	sig.add_effect( A , parameters.strings("A_death_A") );
	// B
	sig.add_effect( B , parameters.strings("A_death_B") );
	// C 
	sig.add_effect( C , parameters.strings("A_death_C") );	
	// R 
	sig.add_effect( C , parameters.strings("A_death_R") );	
	
	phenotype.death.rates[nApoptosis] = sig.compute_effect(); 
	if( p > parameters.doubles("A_apoptosis_pressure_threshold") )
	{
		phenotype.death.rates[nApoptosis] = 10; 
	}

	// speed 
	static double base_speed = parameters.doubles("A_base_speed"); 
	static double max_speed = parameters.doubles("A_max_speed"); 
	sig.reset(); 
	sig.base_parameter = base_speed;
	sig.max_parameter = max_speed;

	// A 
	sig.add_effect( A , parameters.strings("A_speed_A") );
	// B
	sig.add_effect( B , parameters.strings("A_speed_B") );
	// C 
	sig.add_effect( C , parameters.strings("A_speed_C") );	
	// R 
	sig.add_effect( C , parameters.strings("A_speed_R") );	

	phenotype.motility.migration_speed = sig.compute_effect();

	// secretion 
	static double base_secretion = parameters.doubles("A_base_secretion");
	static double max_secretion = parameters.doubles("A_max_secretion"); 
	sig.reset(); 
	sig.base_parameter = base_secretion; 
	sig.max_parameter = max_secretion; 
	// A 
	sig.add_effect( A , parameters.strings("A_signal_A") );
	// B
	sig.add_effect( B , parameters.strings("A_signal_B") );
	// C 
	sig.add_effect( C , parameters.strings("A_signal_C") );	
	// R 
	sig.add_effect( R , parameters.strings("A_signal_R") );	

	phenotype.secretion.secretion_rates[nA] = sig.compute_effect();

	return; 
}

void B_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// housekeeping 
	static Cell_Definition* pCD  = find_cell_definition("B");
	static int nApoptosis = pCD->phenotype.death.find_death_model_index( "Apoptosis"); 
	static int nNecrosis  = pCD->phenotype.death.find_death_model_index( "Necrosis"); 

	if( phenotype.death.dead == true )
	{

		phenotype.secretion.set_all_secretion_to_zero(); 
		phenotype.secretion.set_all_uptake_to_zero(); 
		phenotype.motility.is_motile = false; 

		pCell->functions.update_phenotype = NULL; 
		return; 
	}
	
	// sample A, B, C, resource, and pressure 
	static int nA = microenvironment.find_density_index( "signal A" ); 
	static int nB = microenvironment.find_density_index( "signal B" ); 
	static int nC = microenvironment.find_density_index( "signal C" ); 
	static int nR = microenvironment.find_density_index( "resource" ); 

	double A = pCell->nearest_density_vector()[nA];
	double B = pCell->nearest_density_vector()[nB];
	double C = pCell->nearest_density_vector()[nC];
	double R = pCell->nearest_density_vector()[nR];
	double p = pCell->state.simple_pressure; 

	// necrotic death rate 
	static double base_necrosis_rate = pCD->phenotype.death.rates[nNecrosis];
	static double necrosis_threshold = parameters.doubles("B_necrosis_threshold");
	phenotype.death.rates[nNecrosis] = 0.0;
	
	if( R < necrosis_threshold )
	{
		phenotype.death.rates[nNecrosis] = base_necrosis_rate; 
		phenotype.death.rates[nNecrosis] *= (1.0 - R / necrosis_threshold);
	}

	// cycle rate 
	double param0 = parameters.doubles("B_base_cycle") * R; 

	up_down_signal sig; 
	sig.base_parameter = param0; 
	sig.max_parameter = parameters.doubles("B_max_cycle");

	// A 
	sig.add_effect( A , parameters.strings("B_cycle_A") );
	// B
	sig.add_effect( B , parameters.strings("B_cycle_B") );
	// C 
	sig.add_effect( C , parameters.strings("B_cycle_C") );
	
	phenotype.cycle.data.transition_rate(0,0) = sig.compute_effect(); 
	if( p > parameters.doubles( "B_cycle_pressure_threshold") )
	{ phenotype.cycle.data.transition_rate(0,0) = 0.0; }
	
	// apoptotic rate 
	static double base_death_rate = parameters.doubles("B_base_death"); 
	static double max_death_rate = parameters.doubles("B_max_death");
	sig.reset();
	sig.base_parameter = base_death_rate;
	sig.max_parameter = max_death_rate; 

	// A 
	sig.add_effect( A , parameters.strings("B_death_A") );
	// B
	sig.add_effect( B , parameters.strings("B_death_B") );
	// C 
	sig.add_effect( C , parameters.strings("B_death_C") );	
	// R 
	sig.add_effect( C , parameters.strings("B_death_R") );	
	
	phenotype.death.rates[nApoptosis] = sig.compute_effect(); 
	if( p > parameters.doubles("A_apoptosis_pressure_threshold") )
	{
		phenotype.death.rates[nApoptosis] = 10; 
	}

	// speed 
	static double base_speed = parameters.doubles("B_base_speed"); 
	static double max_speed = parameters.doubles("B_max_speed"); 
	sig.reset(); 
	sig.base_parameter = base_speed;
	sig.max_parameter = max_speed;
	// A 
	sig.add_effect( A , parameters.strings("B_speed_A") );
	// B
	sig.add_effect( B , parameters.strings("B_speed_B") );
	// C 
	sig.add_effect( C , parameters.strings("B_speed_C") );	
	// R 
	sig.add_effect( C , parameters.strings("B_speed_R") );	

	phenotype.motility.migration_speed = sig.compute_effect();

	// secretion 

	static double base_secretion = parameters.doubles("B_base_secretion");
	static double max_secretion = parameters.doubles("B_max_secretion"); 
	sig.reset(); 
	sig.base_parameter = base_secretion; 
	sig.max_parameter = max_secretion; 
	
	// A 
	sig.add_effect( A , parameters.strings("B_signal_A") );
	// B
	sig.add_effect( B , parameters.strings("B_signal_B") );
	// C 
	sig.add_effect( C , parameters.strings("B_signal_C") );	
	// R 
	sig.add_effect( R , parameters.strings("B_signal_R") );	

	phenotype.secretion.secretion_rates[nB] = sig.compute_effect();

	return; 
}

void C_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// housekeeping 
	static Cell_Definition* pCD  = find_cell_definition("C");
	static int nApoptosis = pCD->phenotype.death.find_death_model_index( "Apoptosis"); 
	static int nNecrosis  = pCD->phenotype.death.find_death_model_index( "Necrosis"); 

	if( phenotype.death.dead == true )
	{

		phenotype.secretion.set_all_secretion_to_zero(); 
		phenotype.secretion.set_all_uptake_to_zero(); 
		phenotype.motility.is_motile = false; 

		pCell->functions.update_phenotype = NULL; 
		return; 
	}
	
	// sample A, B, C, resource, and pressure 
	static int nA = microenvironment.find_density_index( "signal A" ); 
	static int nB = microenvironment.find_density_index( "signal B" ); 
	static int nC = microenvironment.find_density_index( "signal C" ); 
	static int nR = microenvironment.find_density_index( "resource" ); 

	double A = pCell->nearest_density_vector()[nA];
	double B = pCell->nearest_density_vector()[nB];
	double C = pCell->nearest_density_vector()[nC];
	double R = pCell->nearest_density_vector()[nR];
	double p = pCell->state.simple_pressure; 

	// necrotic death rate 
	static double base_necrosis_rate = pCD->phenotype.death.rates[nNecrosis];
	static double necrosis_threshold = parameters.doubles("A_necrosis_threshold");
	phenotype.death.rates[nNecrosis] = 0.0;
	
	if( R < necrosis_threshold )
	{
		phenotype.death.rates[nNecrosis] = base_necrosis_rate; 
		phenotype.death.rates[nNecrosis] *= (1.0 - R / necrosis_threshold);
	}
 
	// cycle rate 
	double param0 = parameters.doubles("C_base_cycle") * R; 

	up_down_signal sig; 
	sig.base_parameter = param0; 
	sig.max_parameter = parameters.doubles("C_max_cycle");

	// A 
	sig.add_effect( A , parameters.strings("C_cycle_A") );
	// B
	sig.add_effect( B , parameters.strings("C_cycle_B") );
	// C 
	sig.add_effect( C , parameters.strings("C_cycle_C") );
	
	phenotype.cycle.data.transition_rate(0,0) = sig.compute_effect(); 
	if( p > parameters.doubles( "C_cycle_pressure_threshold") )
	{ phenotype.cycle.data.transition_rate(0,0) = 0.0; }
	
	// apoptotic rate 

	static double base_death_rate = parameters.doubles("C_base_death"); 
	static double max_death_rate = parameters.doubles("C_max_death");
	sig.reset();
	sig.base_parameter = base_death_rate;
	sig.max_parameter = max_death_rate; 

	// A 
	sig.add_effect( A , parameters.strings("C_death_A") );
	// B
	sig.add_effect( B , parameters.strings("C_death_B") );
	// C 
	sig.add_effect( C , parameters.strings("C_death_C") );	
	// R 
	sig.add_effect( C , parameters.strings("C_death_R") );	
	
	phenotype.death.rates[nApoptosis] = sig.compute_effect(); 
	if( p > parameters.doubles("C_apoptosis_pressure_threshold") )
	{
		phenotype.death.rates[nApoptosis] = 10; 
	}

	// speed 
	static double base_speed = parameters.doubles("C_base_speed"); 
	static double max_speed = parameters.doubles("C_max_speed");
	sig.reset(); 
	sig.base_parameter = base_speed;
	sig.max_parameter = max_speed;

	// A 
	sig.add_effect( A , parameters.strings("C_speed_A") );
	// B
	sig.add_effect( B , parameters.strings("C_speed_B") );
	// C 
	sig.add_effect( C , parameters.strings("C_speed_C") );	
	// R 
	sig.add_effect( C , parameters.strings("C_speed_R") );	

	phenotype.motility.migration_speed = sig.compute_effect();

	// secretion 

	static double base_secretion = parameters.doubles("C_base_secretion");
	static double max_secretion = parameters.doubles("C_max_secretion"); 
	sig.reset(); 
	sig.base_parameter = base_secretion; 
	sig.max_parameter = max_secretion; 
	
	// A 
	sig.add_effect( A , parameters.strings("C_signal_A") );
	// B
	sig.add_effect( B , parameters.strings("C_signal_B") );
	// C 
	sig.add_effect( C , parameters.strings("C_signal_C") );	
	// R 
	sig.add_effect( R , parameters.strings("C_signal_R") );	

	phenotype.secretion.secretion_rates[nC] = sig.compute_effect();

	return; 
}


void SVG_plot_dark( std::string filename , Microenvironment& M, double z_slice , double time, std::vector<std::string> (*cell_coloring_function)(Cell*) )
{
	double X_lower = M.mesh.bounding_box[0];
	double X_upper = M.mesh.bounding_box[3];
 
	double Y_lower = M.mesh.bounding_box[1]; 
	double Y_upper = M.mesh.bounding_box[4]; 

	double plot_width = X_upper - X_lower; 
	double plot_height = Y_upper - Y_lower; 

	double font_size = 0.025 * plot_height; // PhysiCell_SVG_options.font_size; 
	double top_margin = font_size*(.2+1+.2+.9+.5 ); 

	// open the file, write a basic "header"
	std::ofstream os( filename , std::ios::out );
	if( os.fail() )
	{ 
		std::cout << std::endl << "Error: Failed to open " << filename << " for SVG writing." << std::endl << std::endl; 

		std::cout << std::endl << "Error: We're not writing data like we expect. " << std::endl
		<< "Check to make sure your save directory exists. " << std::endl << std::endl
		<< "I'm going to exit with a crash code of -1 now until " << std::endl 
		<< "you fix your directory. Sorry!" << std::endl << std::endl; 
		exit(-1); 
	} 
	
	Write_SVG_start( os, plot_width , plot_height + top_margin );

	// draw the background 
	Write_SVG_rect( os , 0 , 0 , plot_width, plot_height + top_margin , 0.002 * plot_height , "white", "white" );

	// write the simulation time to the top of the plot
 
	char* szString; 
	szString = new char [1024]; 
 
	int total_cell_count = all_cells->size(); 
 
	double temp_time = time; 

	std::string time_label = formatted_minutes_to_DDHHMM( temp_time ); 
 
	sprintf( szString , "Current time: %s, z = %3.2f %s", time_label.c_str(), 
		z_slice , PhysiCell_SVG_options.simulation_space_units.c_str() ); 
	Write_SVG_text( os, szString, font_size*0.5,  font_size*(.2+1), 
		font_size, PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() );
	sprintf( szString , "%u agents" , total_cell_count ); 
	Write_SVG_text( os, szString, font_size*0.5,  font_size*(.2+1+.2+.9), 
		0.95*font_size, PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() );
	
	delete [] szString; 


	// add an outer "g" for coordinate transforms 
	
	os << " <g id=\"tissue\" " << std::endl 
	   << "    transform=\"translate(0," << plot_height+top_margin << ") scale(1,-1)\">" << std::endl; 
	   
	// prepare to do mesh-based plot (later)
	
	double dx_stroma = M.mesh.dx; 
	double dy_stroma = M.mesh.dy; 
	
	os << "  <g id=\"ECM\">" << std::endl; 
  
	int ratio = 1; 
	double voxel_size = dx_stroma / (double) ratio ; 
  
	double half_voxel_size = voxel_size / 2.0; 
	double normalizer = 78.539816339744831 / (voxel_size*voxel_size*voxel_size); 
	
	// color the dark background 
	Write_SVG_rect( os , 0 , 0 , plot_width, plot_height , 0 , "none", "black" );

 
 // color in the background ECM
/* 
 if( ECM.TellRows() > 0 )
 {
  // find the k corresponding to z_slice
  
  
  
  Vector position; 
  *position(2) = z_slice; 
  

  // 25*pi* 5 microns^2 * length (in source) / voxelsize^3
  
  for( int j=0; j < ratio*ECM.TellCols() ; j++ )
  {
   // *position(1) = *Y_environment(j); 
   *position(1) = *Y_environment(0) - dy_stroma/2.0 + j*voxel_size + half_voxel_size; 
   
   for( int i=0; i < ratio*ECM.TellRows() ; i++ )
   {
    // *position(0) = *X_environment(i); 
    *position(0) = *X_environment(0) - dx_stroma/2.0 + i*voxel_size + half_voxel_size; 
	
    double E = evaluate_Matrix3( ECM, X_environment , Y_environment, Z_environment , position );	
	double BV = normalizer * evaluate_Matrix3( OxygenSourceHD, X_environment , Y_environment, Z_environment , position );
	if( isnan( BV ) )
	{ BV = 0.0; }

	vector<string> Colors;
	Colors = hematoxylin_and_eosin_stroma_coloring( E , BV );
	Write_SVG_rect( os , *position(0)-half_voxel_size-X_lower , *position(1)-half_voxel_size+top_margin-Y_lower, 
	voxel_size , voxel_size , 1 , Colors[0], Colors[0] );
   
   }
  }
 
 }
*/
	os << "  </g>" << std::endl; 
 
	// Now draw vessels

	/*
	 std::vector<std::string> VesselColors = hematoxylin_and_eosin_stroma_coloring( 0,1 );

	 os << " <g id=\"BloodVessels\">" << endl; 
	 extern vector<BloodVesselSegment*> BloodVesselSegments; 
	 Vector Offset; 
	 *Offset(0) = X_lower; 
	 *Offset(1) = Y_lower-top_margin;
	*/
 

 
	// plot intersecting cells 
	os << "  <g id=\"cells\">" << std::endl; 
	for( int i=0 ; i < total_cell_count ; i++ )
	{
		Cell* pC = (*all_cells)[i]; // global_cell_list[i]; 
  
		static std::vector<std::string> Colors; 
		if( fabs( (pC->position)[2] - z_slice ) < pC->phenotype.geometry.radius )
		{
			double r = pC->phenotype.geometry.radius ; 
			double rn = pC->phenotype.geometry.nuclear_radius ; 
			double z = fabs( (pC->position)[2] - z_slice) ; 
   
			Colors = cell_coloring_function( pC ); 

			os << "   <g id=\"cell" << pC->ID << "\">" << std::endl; 
  
			// figure out how much of the cell intersects with z = 0 
   
			double plot_radius = sqrt( r*r - z*z ); 

			Write_SVG_circle( os, (pC->position)[0]-X_lower, (pC->position)[1]-Y_lower, 
				plot_radius , 0.5, Colors[1], Colors[0] ); 

			// plot the nucleus if it, too intersects z = 0;
			if( fabs(z) < rn && PhysiCell_SVG_options.plot_nuclei == true )
			{   
				plot_radius = sqrt( rn*rn - z*z ); 
			 	Write_SVG_circle( os, (pC->position)[0]-X_lower, (pC->position)[1]-Y_lower, 
					plot_radius, 0.5, Colors[3],Colors[2]); 
			}					  
			os << "   </g>" << std::endl;
		}
	}
	os << "  </g>" << std::endl; 
	
	// plot intersecting BM points
	/* 
	 for( int i=0 ; i < BasementMembraneNodes.size() ; i++ )
	 {
		// vector<string> Colors = false_cell_coloring( pC ); 
		BasementMembraneNode* pBMN = BasementMembraneNodes[i]; 
		double thickness =0.1; 
		
		if( fabs( *(pBMN->Position)(2) - z_slice ) < thickness/2.0 ) 
		{
		 string bm_color ( "rgb(0,0,0)" );
		 double r = thickness/2.0; 
		 double z = fabs( *(pBMN->Position)(2) - z_slice) ; 

		 os << " <g id=\"BMN" << pBMN->ID << "\">" << std::endl; 
		 Write_SVG_circle( os,*(pBMN->Position)(0)-X_lower, *(pBMN->Position)(1)+top_margin-Y_lower, 10*thickness/2.0 , 0.5 , bm_color , bm_color ); 
		 os << " </g>" << std::endl;
		}
		// pC = pC->pNextCell;
	 }
	*/ 
	
	// end of the <g ID="tissue">
	os << " </g>" << std::endl; 
 
	// draw a scale bar
 
	double bar_margin = 0.025 * plot_height; 
	double bar_height = 0.01 * plot_height; 
	double bar_width = PhysiCell_SVG_options.length_bar; 
	double bar_stroke_width = 0.001 * plot_height; 
	
	std::string bar_units = PhysiCell_SVG_options.simulation_space_units; 
	// convert from micron to mm
	double temp = bar_width;  

	if( temp > 999 && std::strstr( bar_units.c_str() , PhysiCell_SVG_options.mu.c_str() )   )
	{
		temp /= 1000;
		bar_units = "mm";
	}
	// convert from mm to cm 
	if( temp > 9 && std::strcmp( bar_units.c_str() , "mm" ) == 0 )
	{
		temp /= 10; 
		bar_units = "cm";
	}
	
	szString = new char [1024];
	sprintf( szString , "%u %s" , (int) round( temp ) , bar_units.c_str() );
 
	Write_SVG_rect( os , plot_width - bar_margin - bar_width  , plot_height + top_margin - bar_margin - bar_height , 
		bar_width , bar_height , 0.002 * plot_height , "rgb(255,255,255)", "rgb(0,0,0)" );
	Write_SVG_text( os, szString , plot_width - bar_margin - bar_width + 0.25*font_size , 
		plot_height + top_margin - bar_margin - bar_height - 0.25*font_size , 
		font_size , PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() ); 
	
	delete [] szString; 

	// plot runtime 
	szString = new char [1024]; 
	RUNTIME_TOC(); 
	std::string formatted_stopwatch_value = format_stopwatch_value( runtime_stopwatch_value() );
	Write_SVG_text( os, formatted_stopwatch_value.c_str() , bar_margin , top_margin + plot_height - bar_margin , 0.75 * font_size , 
		PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() );
	delete [] szString; 

	// draw a box around the plot window
	Write_SVG_rect( os , 0 , top_margin, plot_width, plot_height , 0.002 * plot_height , "rgb(0,0,0)", "none" );
	
	// close the svg tag, close the file
	Write_SVG_end( os ); 
	os.close();
 
	return; 
}








