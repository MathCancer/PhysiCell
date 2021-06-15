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

#include "cancer_flux_balance.h"



// declare cell definitions here
// Cell_Definition metabolic_cell;

#include "../modules/PhysiCell_settings.h"

void create_cell_types( void )
{
	// set the random seed 
	SeedRandom( parameters.ints("random_seed") );  
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	initialize_default_cell_definition(); 

	cell_defaults.parameters.o2_proliferation_saturation = 38.0;  
	cell_defaults.parameters.o2_reference = 38.0; 
	
	cell_defaults.functions.update_phenotype = tumor_cell_phenotype_with_oncoprotein;  
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;	
	
 	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 
	
	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 
	
	return; 
}


void setup_microenvironment( void )
{
	// make sure ot override and go back to 2D 
	if( default_microenvironment_options.simulate_2D == true )
	{
		std::cout << "Warning: overriding 2D setting to return to 3D" << std::endl;
		default_microenvironment_options.simulate_2D = false;
	}
			
	initialize_microenvironment(); 	

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

void setup_tissue( void )
{
	double tumor_radius = parameters.doubles( "tumor_radius" ); // 250.0; 
	std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius, tumor_radius);
	std::cout << "creating " << positions.size() << " closely-packed tumor cells ... " << std::endl;

	Cell* pCell = NULL;
	for( int i=0; i < positions.size(); i++ )
	{
		pCell = create_cell(); // tumor cell
		pCell->assign_position( positions[i] );
	}
	
	return; 
}

/*
void setup_tissue( void )
{

	double box_radius = 100.;  // rf. config file
	double xmin = -box_radius;
	double ymin = -box_radius;
	double xmax = box_radius;
	double ymax = box_radius;
	double delta = 4.0;

	Cell* pC;

	pC = create_cell( cell_defaults );
	pC->assign_position( 0., 0.0, 0.0 );
	pC->physifba_model = PhysiFBA::PhysiFBA_default_model;

}

*/

void update_cell(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt ){

  phenotype.secretion.set_all_secretion_to_zero();
  phenotype.secretion.set_all_uptake_to_zero();

  std::string oxygen_name = "oxygen";
  std::string glucose_name = "glucose";
  std::string lactate_name = "lactate";
  
  std::string oxygen_flux_id = PhysiFBA::exchange_flux_density_map[oxygen_name];
  std::string glucose_flux_id = PhysiFBA::exchange_flux_density_map[glucose_name];
  std::string lactate_flux_id = PhysiFBA::exchange_flux_density_map[lactate_name];

  static int oxygen_idx = microenvironment.find_density_index( oxygen_name );
  static int glucose_idx = microenvironment.find_density_index( glucose_name );
  static int lactate_idx = microenvironment.find_density_index( lactate_name );

  double oxygen_density = pCell->nearest_density_vector()[oxygen_idx];
  double glucose_density = pCell->nearest_density_vector()[glucose_idx]; // dived by voxel size?
  double lactate_density = pCell->nearest_density_vector()[lactate_idx]; // dived by voxel size?
  
  
  double O2_Km = parameters.doubles("oxygen_Km");
  double O2_Vmax = parameters.doubles("oxygen_Vmax");
  double oxygen_flux_bound = -1 * ( O2_Vmax * oxygen_density) / (oxygen_density + O2_Km);
  pCell->physifba_model.setReactionLowerBound(oxygen_flux_id, oxygen_flux_bound);

  double glc_Km = parameters.doubles("glucose_Km");
  double glc_Vmax = parameters.doubles("glucose_Vmax");
  double glucose_flux_bound = -1 * ( glc_Vmax * glucose_density) / (glucose_density + glc_Km);
  pCell->physifba_model.setReactionLowerBound(glucose_flux_id, glucose_flux_bound);

  double lac_Km = parameters.doubles("lactate_Km");
  double lac_Vmax = parameters.doubles("lactate_Vmax");
  double lactate_flux_bound = -1 * ( lac_Vmax * lactate_density) / (lactate_density + lac_Km);
  pCell->physifba_model.setReactionLowerBound(lactate_flux_id, lactate_flux_bound);


  std::cout << "Running FBA for cell: " << pCell->ID << std::endl;
  pCell->physifba_model.runFBA();
    
  PhysiFBA_reaction* reaction = pCell->physifba_model.getReaction(oxygen_flux_id);
  double oxygen_flux = reaction->getFluxValue();
  std::cout << "Oxygen flux: " << oxygen_flux << std::endl;

  reaction = pCell->physifba_model.getReaction(glucose_flux_id);
  double glucose_flux = reaction->getFluxValue();
  std::cout << "glucose flux: " << glucose_flux << std::endl;

  reaction = pCell->physifba_model.getReaction(lactate_flux_id);
  double lactate_flux = reaction->getFluxValue();
  std::cout << "lactate flux: " << lactate_flux << std::endl;

  
  if ( oxygen_flux < 0)
  	phenotype.secretion.uptake_rates[oxygen_idx] = abs(oxygen_flux / oxygen_density);

  if ( glucose_flux < 0)
  	phenotype.secretion.uptake_rates[glucose_idx] = abs(glucose_flux / glucose_density);
	  

  if ( lactate_flux < 0 )
  	phenotype.secretion.uptake_rates[lactate_idx] = abs(lactate_flux / lactate_density);
	  
  else if ( lactate_flux > 0 )
    phenotype.secretion.secretion_rates[lactate_idx] = abs(lactate_flux / lactate_density);
  
/*
  phenotype.volume.fluid += dt * phenotype.volume.fluid_change_rate *
  	( phenotype.volume.target_fluid_fraction * phenotype.volume.total - phenotype.volume.fluid );

  if( phenotype.volume.fluid < 0.0 )
  { phenotype.volume.fluid = 0.0; }

  phenotype.volume.cytoplasmic_fluid = phenotype.volume.fluid;

  phenotype.volume.cytoplasmic_solid += dt * phenotype.volume.cytoplasmic_biomass_change_rate *
    (phenotype.volume.target_solid_cytoplasmic - phenotype.volume.cytoplasmic_solid );

  if( phenotype.volume.cytoplasmic_solid < 0.0 )
  { phenotype.volume.cytoplasmic_solid = 0.0; }

  phenotype.volume.solid = phenotype.volume.cytoplasmic_solid;

  phenotype.volume.cytoplasmic = phenotype.volume.cytoplasmic_solid + phenotype.volume.cytoplasmic_fluid;
  phenotype.volume.total = phenotype.volume.cytoplasmic_solid + phenotype.volume.cytoplasmic_fluid;

  // Tell physicell to update the cell radius to the new volume
  phenotype.geometry.update(pCell, phenotype, dt);
*/

}

void setup_default_metabolic_model( void )
{

  // This codes is to use iSIM model
  std::string sbml_fileame = parameters.strings("sbml_model");
  PhysiFBA::PhysiFBA_default_model.readSBMLModel(sbml_fileame.c_str());
  PhysiFBA::PhysiFBA_default_model.initLpModel();

  PhysiFBA::exchange_flux_density_map["glucose"] = "R_E1";
  PhysiFBA::exchange_flux_density_map["lactate"] = "R_E2";
  PhysiFBA::exchange_flux_density_map["oxygen"] = "R_E3";
  PhysiFBA::exchange_flux_density_map["energy"] = "R_R4";

}

void anuclear_volume_model (Cell* pCell, Phenotype& phenotype, double dt)
{

    return;
}

void metabolic_cell_phenotype( Cell* pCell, Phenotype& phenotype, double dt )
{
	// if cell is dead, don't bother with future phenotype changes.
	if( phenotype.death.dead == true )
	{
		pCell->functions.update_phenotype = NULL;
		return;
	}

	// update the transition rate according to growth rate?
	static int cycle_start_index = live.find_phase_index( PhysiCell_constants::live );
	static int cycle_end_index = live.find_phase_index( PhysiCell_constants::live );

	//static int oncoprotein_i = pCell->custom_data.find_variable_index( "oncoprotein" );
	//phenotype.cycle.data.transition_rate( cycle_start_index ,cycle_end_index ) *= pCell->custom_data[oncoprotein_i] ;
	return;
}



std::vector<std::string> my_coloring_function( Cell* pCell )
{
	// start with flow cytometry coloring

	std::vector<std::string> output = false_cell_coloring_cytometry(pCell);
	output[0] = "red";
	output[1] = "red";
	output[2] = "red";

	if( pCell->phenotype.death.dead == false && pCell->type == 1 )
	{
		 output[0] = "black";
		 output[2] = "black";
	}

	return output;
}
