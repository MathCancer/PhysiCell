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
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include "./custom.h"
#include "../BioFVM/BioFVM.h"  

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
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	cell_defaults.functions.contact_function = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 

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

	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 

	cell_defaults.functions.pre_update_intracellular = pre_update_intracellular;
	cell_defaults.functions.post_update_intracellular = post_update_intracellular;
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 

	Cell_Definition* pCD = find_cell_definition( "epithelial");

	pCD->functions.pre_update_intracellular = pre_update_intracellular;
	pCD->functions.post_update_intracellular = post_update_intracellular;
	pCD->functions.custom_cell_rule = custom_function; 
	pCD->functions.contact_function = contact_function; 
	pCD->functions.update_velocity = standard_update_cell_velocity; 

	pCD = find_cell_definition( "mesenchymal");
	pCD->functions.pre_update_intracellular = pre_update_intracellular;
	pCD->functions.post_update_intracellular = post_update_intracellular;
	pCD->functions.custom_cell_rule = custom_function; 
	pCD->functions.contact_function = contact_function;
	pCD->functions.update_velocity = standard_update_cell_velocity; 
	
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	display_cell_definitions( std::cout ); 
	
	return; 
}

void set_substrate_density(int density_index, double max, double min, double radius)
{
	std::cout << "SETTING SUBSTRATE --> " << density_index << std::endl;
	// Inject given concentration on the extremities only

	std::cout << microenvironment.number_of_voxels() << "\n";

	for (unsigned int n = 0; n < microenvironment.number_of_voxels(); n++)
	{
		auto current_voxel = microenvironment.voxels(n);
		double t_norm = norm(current_voxel.center);

		if ((radius - t_norm) <= 0)
			microenvironment.density_vector(n)[density_index] = current_value(min, max, uniform_random());
	}
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 

	double ECM_min = parameters.doubles("density_ECM_min");
	double ECM_max = parameters.doubles("density_ECM_max");
	double tgfbeta_max = parameters.doubles("density_tgfbeta_max");
	double tgfbeta_min = parameters.doubles("density_tgfbeta_min");

	if(ECM_max != ECM_min){
	int ecm_index = microenvironment.find_density_index("ecm");
	set_substrate_density(ecm_index, ECM_max, ECM_min);
	}
	if(tgfbeta_max != tgfbeta_min){
	int tgfbeta_index = microenvironment.find_density_index("TGFbeta");
	set_substrate_density(tgfbeta_index, tgfbeta_max, tgfbeta_min);
	}

	return; 
}

void setup_tissue( void )
{
	// place a cluster of tumor cells at the center 
	load_cells_from_pugixml(); 

	// removing substrate in cell voxel
	int ecm_index = BioFVM::microenvironment.find_density_index("ecm");
	int tgfbeta_index = BioFVM::microenvironment.find_density_index("tgfbeta");
	for( int i=0; i < (*all_cells).size(); i++ )
		{
			Cell* pC = (*all_cells)[i]; 

			int voxel_index = pC->get_current_voxel_index();
			microenvironment.density_vector(voxel_index)[ecm_index] = 0.0;
			microenvironment.density_vector(voxel_index)[tgfbeta_index] = 0.0;
		}
}


void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ 	

	pCell->custom_data["cell_contact"] = 0.0;
	for( int j=0; j < pCell->nearby_interacting_cells().size(); j++ )
    {
        Cell* pTest = pCell->nearby_interacting_cells()[j]; 
		contact_function(pCell, phenotype, pTest, pTest->phenotype, dt);
    }
	
	for( int j=0; j < pCell->state.spring_attachments.size(); j++ )
    {
        Cell* pTest = pCell->state.spring_attachments[j]; 
        contact_function(pCell, phenotype, pTest, pTest->phenotype, dt);
    }

	// ADDING ECM PHYSICAL INTERACTION AND ADHESION

	pCell->custom_data["ecm_contact"] = 0.0;
	pCell->custom_data["nucleus_deform"] = 0.0;
	//std::cout << pCell->custom_data["nucleus_deform"] << std::endl;

	int ecm_index = BioFVM::microenvironment.find_density_index("ecm");
	if ( ecm_index >= 0 ){
		add_ecm_interaction( pCell, ecm_index, pCell->get_current_voxel_index() );
		//add_TGFbeta_interaction(pCell, pCell->get_current_mechanics_voxel_index());
		std::vector<int>::iterator neighbor_voxel_index;
		std::vector<int>::iterator neighbor_voxel_index_end = 
		microenvironment.mesh.moore_connected_voxel_indices[pCell->get_current_voxel_index()].end();

		for( neighbor_voxel_index = 
			microenvironment.mesh.moore_connected_voxel_indices[pCell->get_current_voxel_index()].begin();
			neighbor_voxel_index != neighbor_voxel_index_end; 
			++neighbor_voxel_index )
		{
			add_ecm_interaction( pCell, ecm_index, *neighbor_voxel_index );
			
		}

		/* pCell->update_motility_vector(dt); 
		pCell->velocity += phenotype.motility.motility_vector; */
	}
	
	return; 	
} 

// This function is never called because I am not using "dynamic attachment" but I am using the "dynamic SPRING adhesion"
void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ 

	std::vector<double> displacement = pOther->position;
	displacement -= pMe->position;
	double distance = norm( displacement ); 
			
	double max_distance = pMe->phenotype.geometry.radius + 
				pOther->phenotype.geometry.radius; 
	max_distance *=  pMe->phenotype.mechanics.relative_maximum_adhesion_distance;  //parameters.doubles("max_interaction_factor"); 

			//std::cout << max_distance << " - " << distance << "\n";

	double interaction_distance = max_distance - distance;

	if (interaction_distance > 0){

		double perc_distance = distance / pMe->phenotype.geometry.radius ;
		pMe->custom_data["cell_contact"] += perc_distance;
			}
	else {
		detach_cells_as_spring(pMe, pOther);
	}

	return; 
} 

void pre_update_intracellular(Cell* pCell, Phenotype& phenotype, double dt){
	return;
}

void post_update_intracellular(Cell* pCell, Phenotype& phenotype, double dt){
	return;
}

/* Calculate repulsion/adhesion between agent and ecm according to its local density */
void add_ecm_interaction(Cell* pC, int index_ecm, int index_voxel )
{
	// Check if there is ECM material in given voxel
	//double dens2 = get_microenvironment()->density_vector(index_voxel)[index_ecm];
	double dens = pC->get_microenvironment()->nearest_density_vector(index_voxel)[index_ecm];
	double ecmrad = sqrt(3.0) * pC->get_microenvironment()->mesh.dx * 0.5;
	// if voxel is "full", density is 1
	dens = std::min( dens, 1.0 ); 
	if ( dens > EPSILON )
	{
		// Distance between agent center and ECM voxel center
		pC->displacement = pC->position - microenvironment.mesh.voxels[index_voxel].center;
		double distance = norm(pC->displacement);
		// Make sure that the distance is not zero
		distance = std::max(distance, EPSILON);
		
		double dd = pC->phenotype.geometry.radius + ecmrad;  
		double dnuc = pC->phenotype.geometry.nuclear_radius + ecmrad;  

		double tmp_r = 0;
		// Cell overlap with ECM node, add a repulsion term
		if ( distance < dd )
		{
			// repulsion stronger if nucleii overlap, see Macklin et al. 2012, 2.3.1
			if ( distance < dnuc )
			{
				double M = 1.0;
				double c = 1.0 - dnuc/dd;
				c *= c;
				c -= M;
				tmp_r = c*distance/dnuc + M;
				pC->custom_data["nucleus_deform"] += (dnuc-distance);
			}
			else
			{
				tmp_r = ( 1 - distance / dd );
				tmp_r *= tmp_r;
			}
			tmp_r *= dens * PhysiCell::parameters.doubles("cell_ecm_repulsion");
		}

		// Cell adherence to ECM through integrins
		double max_interactive_distance = (PhysiCell::parameters.doubles("max_interaction_factor")*pC->phenotype.geometry.radius) + ecmrad;
		if ( distance < max_interactive_distance ) 
		{	
			double temp_a = 1 - distance/max_interactive_distance; 
			temp_a *= temp_a; 
			/* \todo change dens with a maximal density ratio ? */

			pC->custom_data["ecm_contact"] += dens * (max_interactive_distance-distance);
			// temp_a *= dens * ( static_cast<Cell*>(this) )->integrinStrength();

			double temp_integrins = get_integrin_strength( pC->custom_data["pintegrin"] );

			temp_a *= dens * temp_integrins;
			
			tmp_r -= temp_a;
		}
		
		/////////////////////////////////////////////////////////////////
		if(tmp_r==0)
			return;
		tmp_r/=distance;

		axpy( &pC->velocity , tmp_r , pC->displacement ); 
	}

}

void set_substrate_density(int density_index, double max, double min)
{
	std::cout << "SETTING SUBSTRATE \n";

	std::cout << microenvironment.number_of_voxels() << "\n";
	#pragma omp parallel for
	for (int n = 0; n < microenvironment.number_of_voxels(); n++)
	{
		auto current_voxel = microenvironment.voxels(n);
		microenvironment.density_vector(n)[density_index] = current_value(min, max, uniform_random());
	}
}

	// FUNCTIONS TO PLOT CELLS

std::string my_coloring_function_for_stroma( double concentration, double max_conc, double min_conc )
{
	 return paint_by_density_percentage( concentration,  max_conc,  min_conc); 

}
