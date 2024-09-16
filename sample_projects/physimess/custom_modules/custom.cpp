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
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 

	cell_defaults.functions.instantiate_cell = instantiate_physimess_cell;	
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = physimess_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	cell_defaults.functions.contact_function = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
    
    cell_defaults.functions.plot_agent_SVG = fibre_agent_SVG;
	cell_defaults.functions.plot_agent_legend = fibre_agent_legend;
	
	
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
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.contact_function = contact_function; 
    
	for (auto* pCD: *getFibreCellDefinitions()){
		pCD->functions.instantiate_cell = instantiate_physimess_fibre;
		pCD->functions.plot_agent_SVG = fibre_agent_SVG;
		pCD->functions.plot_agent_legend = fibre_agent_legend;
	
	}

	for (auto* pCD: cell_definitions_by_index){
		if (!isFibre(pCD) && pCD->custom_data.find_variable_index("fibre_custom_degradation") > 0){	
			if (pCD->custom_data["fibre_custom_degradation"] > 0.5)
				pCD->functions.instantiate_cell = instantiate_physimess_cell_custom_degrade;	
		}
	}
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
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
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin;

    load_cells_from_pugixml();

    // new fibre related parameters and bools
    bool isFibreFromFile = false;
    
    for( int i=0; i < (*all_cells).size(); i++ ){

        if (isFibre((*all_cells)[i]))
        {
            /* fibre positions are given by csv
               assign fibre orientation and test whether out of bounds */
            isFibreFromFile = true;
			static_cast<PhysiMeSS_Fibre*>((*all_cells)[i])->assign_fibre_orientation();
			
        } 
    }

    /* agents have not been added from the file but do want them
       create some of each agent type */

    if(!isFibreFromFile){
        Cell* pC;
        std::vector<double> position = {0, 0, 0};

        for( int k=0; k < cell_definitions_by_index.size() ; k++ ) {

            Cell_Definition *pCD = cell_definitions_by_index[k];
            // std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl;
            
            if (!isFibre(pCD))
            {
                for (int n = 0; n < parameters.ints("number_of_cells"); n++) {

                    position[0] = Xmin + UniformRandom() * Xrange;
                    position[1] = Ymin + UniformRandom() * Yrange;
                    position[2] = Zmin + UniformRandom() * Zrange;

                    pC = create_cell(*pCD);
                                        
                    pC->assign_position(position);
                }
            } 
            
            else 
            {
                for ( int nf = 0 ; nf < parameters.ints("number_of_fibres") ; nf++ ) {

                    position[0] = Xmin + UniformRandom() * Xrange;
                    position[1] = Ymin + UniformRandom() * Yrange;
                    position[2] = Zmin + UniformRandom() * Zrange;

                    pC = create_cell(*pCD);

                    static_cast<PhysiMeSS_Fibre*>(pC)->assign_fibre_orientation();
                    static_cast<PhysiMeSS_Fibre*>(pC)->check_out_of_bounds(position);

                    pC->assign_position(position);
                }
            }
        }
    }

    remove_physimess_out_of_bounds_fibres();
    
    // std::cout << std::endl;
}

std::vector<std::string> paint_by_cell_pressure( Cell* pCell ){

	std::vector< std::string > output( 0);
	int color = (int) round( ((pCell->state.simple_pressure) / 10) * 255 );
	if(color > 255){
		color = 255;
	}
	char szTempString [128];
	sprintf( szTempString , "rgb(%u,0,%u)", color, 255 - color);
	output.push_back( std::string("black") );
	output.push_back( szTempString );
	output.push_back( szTempString );
	output.push_back( szTempString );
	return output;
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{ 
	if (parameters.bools("color_cells_by_pressure")){
		return paint_by_cell_pressure(pCell); 
	} else {
		return paint_by_number_cell_coloring(pCell);
	}
}
std::string my_coloring_function_for_substrate( double concentration, double max_conc, double min_conc )
{ return paint_by_density_percentage( concentration,  max_conc,  min_conc); }

void my_cellcount_function(char* string){
	int nb_fibres = 0;
	for (Cell* cell : *all_cells) {
		if (isFibre(cell))
			nb_fibres++;
	}

	sprintf( string , "%lu cells, %u fibres" , all_cells->size()-nb_fibres, nb_fibres ); 
}

void custom_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ return; }

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 

Cell* instantiate_physimess_cell() { return new PhysiMeSS_Cell; }
Cell* instantiate_physimess_fibre() { return new PhysiMeSS_Fibre; }
Cell* instantiate_physimess_cell_custom_degrade() { return new PhysiMeSS_Cell_Custom_Degrade; }


void PhysiMeSS_Cell_Custom_Degrade::degrade_fibre(PhysiMeSS_Fibre* pFibre)
{
	// Here this version of the degrade function takes cell pressure into account in the degradation rate
    double distance = 0.0;
    pFibre->nearest_point_on_fibre(position, displacement);
    for (int index = 0; index < 3; index++) {
        distance += displacement[index] * displacement[index];
    }
    distance = std::max(sqrt(distance), 0.00001);
    
    
        // Fibre degradation by cell - switched on by flag fibre_degradation
        double stuck_threshold = this->custom_data["fibre_stuck_time"];
        double pressure_threshold = this->custom_data["fibre_pressure_threshold"];
        if (this->custom_data["fibre_degradation"] > 0.5 && (stuck_counter >= stuck_threshold
                                                        || state.simple_pressure > pressure_threshold)) {
            // if (stuck_counter >= stuck_threshold){
            //     std::cout << "Cell " << ID << " is stuck at time " << PhysiCell::PhysiCell_globals.current_time
            //                 << " near fibre " << pFibre->ID  << std::endl;;
            // }
            // if (state.simple_pressure > pressure_threshold){
            //     std::cout << "Cell " << ID << " is under pressure of " << state.simple_pressure << " at "
            //                 << PhysiCell::PhysiCell_globals.current_time << " near fibre " << pFibre->ID  << std::endl;;
            // }
            displacement *= -1.0/distance;
            double dotproduct = dot_product(displacement, phenotype.motility.motility_vector);
            if (dotproduct >= 0) {
                double rand_degradation = PhysiCell::UniformRandom();
                double prob_degradation = this->custom_data["fibre_degradation_rate"];
                if (state.simple_pressure > pressure_threshold){
                    prob_degradation *= state.simple_pressure;
                }
                if (rand_degradation <= prob_degradation) {
                    //std::cout << " --------> fibre " << (*other_agent).ID << " is flagged for degradation " << std::endl;
                    // (*other_agent).parameters.degradation_flag = true;
                    pFibre->flag_for_removal();
                    // std::cout << "Degrading fibre agent " << pFibre->ID << " using flag for removal !!" << std::endl;
                    stuck_counter = 0;
                }
            }
        }
    // }
}
