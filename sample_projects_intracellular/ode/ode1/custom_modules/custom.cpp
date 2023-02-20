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

#include "custom.h"
#include "../BioFVM/BioFVM.h"  
using namespace BioFVM;

// declare cell definitions here 

std::vector<bool> nodes;

void create_cell_types( void )
{
	// set the random seed 
	SeedRandom( parameters.ints("random_seed") );  
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
    std::cout << "custom.cpp: before initialize_cell_definitions_from_pugixml() " << std::endl;
	initialize_cell_definitions_from_pugixml(); 
    std::cout << "custom.cpp: after initialize_cell_definitions_from_pugixml() " << std::endl;
	
	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/

	build_cell_definitions_maps(); 
    std::cout << "custom.cpp: after build_cell_definitions_maps() " << std::endl;

	display_cell_definitions( std::cout ); 
    std::cout << "custom.cpp: after display_cell_definitions() " << std::endl;
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	

    int oxygen_i = microenvironment.find_density_index( "oxygen" ); 
    int glucose_i = microenvironment.find_density_index( "glucose" ); 
    std::cout << "---------- setup_microenv\n";
    std::cout << "    oxygen_i = " << oxygen_i << std::endl;
    std::cout << "    glucose_i = " << glucose_i << std::endl;
    double oxy = 38.0;  // IC
    double oxy_del = 9.0;
    double glu = 32.0;  // IC
    double glu_del = 7.5;
    double x;
    // double xmin = -750.;
    double xmin = -400.;
    int nregions = 5;
    // nregions = 800;
    // oxy_del = oxy/nregions;
    // glu_del = glu/nregions;
	// double xdel = 1500./nregions;
    double xdel = -2*xmin/nregions;
    // 5 regions across x: [-750:-450:-150:150:450:750]
    std::cout << "setup_microenvironment: num voxels= " << microenvironment.number_of_voxels() << std::endl;
#undef substrate_regions
#ifdef substrate_regions
    for( int n=0; n < microenvironment.number_of_voxels(); n++ )
    {
        // x coordinate of the nth voxel's center
        x = microenvironment.mesh.voxels[n].center[0];
		for( int iregion=1; iregion <= nregions; iregion++ )
		{
			if (x < (xmin + iregion*xdel))
			{
				microenvironment(n)[oxygen_i] = oxy - (iregion-1)*oxy_del;
				microenvironment(n)[glucose_i] = glu - (iregion-1)*glu_del;
				break;
			}
			// oxy -= iregion-5;
			// glu -= iregion-2;
		}
    }
#endif
	
	return; 
}

void setup_tissue( void )
{
    int retval;

    std::cout << "\n------- start " << __FUNCTION__ << "  -------------" << std::endl;
    // place a cluster of tumor cells at the center 
	
	double cell_radius = cell_defaults.phenotype.geometry.radius; 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	
	Cell* pCell = NULL; 

#define simpletest
#ifdef simpletest
	// pCell = create_cell();   // defaults to "default" cell type
	// pCell->assign_position( 0. , 0. , 0.0 );
    // std::cout << "\n------- " << __FUNCTION__ << ": pheno intra = " << pCell->phenotype.intracellular << std::endl;
    // std::cout << "------------   pheno intra type = " << pCell->phenotype.intracellular->type << std::endl;
    // // std::cout << "------------   pheno intra sbml_filename = " << (SBMLIntracellular*)(pCell->phenotype.intracellular)->sbml_filename << std::endl;
    // std::cout << "------------   pheno intra get_state = " << pCell->phenotype.intracellular->get_state() << std::endl;

	// pCell = create_cell(); 
    pCell = create_cell(get_cell_definition("default")); 
    std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;
	// pCell = create_cell(cell_defaults); 
    pCell->phenotype.intracellular->start();
	pCell->assign_position( -50, -30 , 0.0 );
    std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;

    // std::cout << __FUNCTION__ << "------------   pheno intra type = " << pCell->phenotype.intracellular->intracellular_type << std::endl;
    // std::cout << __FUNCTION__ << "------------   pheno intra sbml_filename = " << pCell->phenotype.intracellular->sbml_filename << std::endl;
    // std::cout << "------------   pheno intra sbml_filename = " << (SBMLIntracellular*)(pCell->phenotype.intracellular)->sbml_filename << std::endl;
    // std::string str_retval = pCell->phenotype.intracellular->get_state();
    // std::cout << __FUNCTION__ << "------------   pheno intra get_state() retval = " << str_retval << std::endl;

    pCell = create_cell(); 
    // std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;
    // retval = pCell->phenotype.intracellular->start();

    // Override (nullify) the intracellular model so that it doesn't exist.
    pCell->phenotype.intracellular = NULL;
    std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;
	pCell->assign_position( 0, 0 , 0.0 );

    // retval = pCell->phenotype.intracellular->start();

    // std::cout << __FUNCTION__ << "------------   pheno intra start() retval = " << retval << std::endl;
    // std::cout << __FUNCTION__ << "------------   pheno intra start() = " << (pCell->phenotype.intracellular)->start() << std::endl;

    // std::cout << "------------   pheno intra update() = " <<  pCell->phenotype.intracellular->update() << std::endl;


    // pCell = create_cell(get_cell_definition("default")); 
    pCell = create_cell(); 
    std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;
    pCell->phenotype.intracellular->start();
    std::cout << __FUNCTION__ << ": cell ID= " << pCell->ID <<": ------------   pheno intra = " << pCell->phenotype.intracellular << std::endl;
	pCell->assign_position( 50, 50 , 0.0 );

#else
	
	double tumor_radius = parameters.doubles("tumor_radius"); // 200.0; 
	double x = 0.0; 
	double x_outer = tumor_radius; 
	double y = 0.0; 
	
	int n = 0; 
	// while( y < tumor_radius )  // to just create cells at y=0
	cell_spacing *= 4;   // spread them out
	{
		x = 0.0; 
		if( n % 2 == 1 )
		{ x = 0.5*cell_spacing; }
		x_outer = sqrt( tumor_radius*tumor_radius - y*y ); 
		
		while( x < x_outer )
		{
			pCell = create_cell(); // tumor cell 
			pCell->assign_position( x , y , 0.0 );
			
			// if( fabs( y ) > 0.01 )
			// {
			// 	pCell = create_cell(); // tumor cell 
			// 	pCell->assign_position( x , -y , 0.0 );				
			// }
			
			if( fabs( x ) > 0.01 )
			{ 
				pCell = create_cell(); // tumor cell 
				pCell->assign_position( -x , y , 0.0 );

                // SBMLIntracellular* mysbml =  getSBMLModel(pCell->phenotype);

                // std::cout << "-- create_cell: sbml file= "<< pCell->phenotype.intracellular->sbml_filename << std::endl;

                // rrc::RRHandle rrHandle = createRRInstance();
                // pC->phenotype.molecular.model_rr = rrHandle;  // assign the intracellular model to each cell
                // int r = rrc::getNumberOfReactions(rrHandle);
				
				// if( fabs( y ) > 0.01 )
				// {
				// 	pCell = create_cell(); // tumor cell 
				// 	pCell->assign_position( -x , -y , 0.0 );
				// }
			}
			x += cell_spacing; 
			
		}
		
		y += cell_spacing * sqrt(3.0)/2.0; 
		n++; 
	}
#endif
	
    std::cout << "------- end " << __FUNCTION__ << "  -------------\n" << std::endl;
	return; 
}

// This may need to be done more frequently than dt_phenotype; may need to put in main.cpp
void tumor_cell_phenotype_with_signaling( Cell* pCell, Phenotype& phenotype, double dt )
{
	
    std::cout << "--------------------  tumor_cell_phenotype_with_signaling called ------------------\n";
	if (pCell->phenotype.intracellular->need_update() )
	{	
		// if (PhysiCell::PhysiCell_globals.current_time >= 100.0)
		// 	pCell->type == get_cell_definition("last_one").type
		// 	&& PhysiCell::PhysiCell_globals.current_time >= 100.0 
		// 	&& pCell->phenotype.intracellular->get_parameter_value("$time_scale") == 0.0
		// )
		// 	pCell->phenotype.intracellular->set_parameter_value("$time_scale", 0.1);

		// set_input_nodes(pCell);

		pCell->phenotype.intracellular->update();
		
		// from_nodes_to_cell(pCell, phenotype, dt);
		// color_node(pCell);
	}	
}


// void set_input_nodes(Cell* pCell) {}

// void from_nodes_to_cell(Cell* pCell, Phenotype& phenotype, double dt) {}


std::vector<std::string> my_coloring_function( Cell* pCell )
{
	std::vector< std::string > output( 4 , "rgb(0,0,0)" );
	
	// if ( !pCell->phenotype.intracellular->get_boolean_node_value( parameters.strings("node_to_visualize") ) )
	if (true)
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
		
	}
	else{
		output[0] = "rgb(0, 255,0)";
		output[2] = "rgb(0, 125,0)";
	}
	
	return output;
}

// void color_node(Cell* pCell){
// 	std::string node_name = parameters.strings("node_to_visualize");
// 	pCell->custom_data[node_name] = pCell->phenotype.intracellular->get_boolean_node_value(node_name);
// }