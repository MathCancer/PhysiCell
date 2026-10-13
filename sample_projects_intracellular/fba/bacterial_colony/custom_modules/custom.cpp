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
//#include "../addons/dFBA/src/dfba_intracellular.h"


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

	Cell_Definition* ecoli = find_cell_definition( "ecoli");
	//  This sets the pre and post intracellular update functions
	ecoli->functions.pre_update_intracellular =  NULL;
	ecoli->functions.post_update_intracellular = post_update_intracellular;
	ecoli->functions.update_phenotype = NULL; 
	ecoli->functions.volume_update_function = NULL;

	display_cell_definitions(std::cout);

	return;
}

void setup_microenvironment(void)
{
	initialize_microenvironment();
	return;
}

void setup_tissue(void)
{

	Cell_Definition* pCD = find_cell_definition( "ecoli");
	double cell_radius = pCD->phenotype.geometry.radius; 
	double cell_spacing = 0.95 * 2.0 * cell_radius; 
	
	double colony_radius = parameters.doubles("colony_radius"); // 250.0; 
	
	Cell* pCell = NULL; 
	
	std::vector<std::vector<double>> positions = create_cell_disc_positions(cell_radius, colony_radius); 
	std::cout << "creating " << positions.size() << " closely-packed colony cells ... " << std::endl; 
	
		
	for( int i=0; i < positions.size(); i++ )
	{
		pCell = create_cell(*pCD); // tumor cell 
		pCell->assign_position( positions[i] );
	}
	
	
	return; 
}

void post_update_intracellular(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt ){

	PhysiCelldFBA::dFBAIntracellular* dfba_model = static_cast<PhysiCelldFBA::dFBAIntracellular*>(phenotype.intracellular);

	std::vector<double>& density_vector = pCell->nearest_density_vector();
	for (const auto& exchange : dfba_model->substrate_exchanges) {
		const PhysiCelldFBA::ExchangeFluxData& ex = exchange.second;
		pCell->custom_data[ex.fba_flux_id] = dfba_model->get_flux_value(ex.fba_flux_id);
	}

	pCell->custom_data["growth_rate"] = dfba_model->get_growth_rate();

	return;
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
	// colors cells according to dfba flux values

	PhysiCelldFBA::dFBAIntracellular* dfba_model = static_cast<PhysiCelldFBA::dFBAIntracellular*>(pCell->phenotype.intracellular);

	float max_growth_rate = dfba_model->max_growth_rate;
	assert(max_growth_rate > 0.0);

	std::vector<std::string> output(4);
	
	output[0] = "rgb(255, 255, 255)"; 
	output[1] = "black"; // black border
	output[2] = "rgb(255, 255, 255)";
	output[3] = "black"; // black border



	if( pCell->phenotype.death.dead == true )
	{
		output[0] = "rgb(56, 38, 0)"; 
		output[2] = "rgb(56,38,0)";
		output[3] = "rgb(56,38,0)";
		return output;
	}
	if (pCell->phenotype.death.necrosis_rate() > 0.0)
	{
		output[0] = "rgb(222, 170, 0)"; 
		output[2] = "rgb(222, 170, 0)";
		output[3] = "rgb(222, 170, 0)";
		return output;
	}
	if (pCell->phenotype.death.apoptosis_rate() > 0.0)
	{
		output[0] = "rgb(255, 0, 0)";  
		output[2] = "rgb(255, 0, 0)";
		output[3] = "rgb(255, 0, 0)";
		return output;
	}

	if (dfba_model->get_growth_rate()  > 0.0)
	{
		double normalized_growth_rate = dfba_model->get_growth_rate() / max_growth_rate;	
		int red_blue = (int)(255.0 * (1.0 - normalized_growth_rate));
		int green = 255;

		std::string color = "rgb(" + std::to_string(red_blue) + ", " + 
								std::to_string(green) + ", " + 
								std::to_string(red_blue) + ")";

		output[0] = color;
		output[2] = color;
		output[3] = color;
		return output;
	}else{
		
		return output;
	}
	

}
