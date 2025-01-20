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
# Copyright (c) 2015-2025, Paul Macklin and the PhysiCell Project             #
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

#include "./PhysiCell_pathology.h"

namespace PhysiCell{

PhysiCell_SVG_options_struct PhysiCell_SVG_options;

// cyto_color, cyto_outline , nuclear_color, nuclear_outline
std::vector<std::string> simple_cell_coloring( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" ); 
	output[0] = "rgb(255,0,0)";
	output[2] = "rgb(0,0,255)";
	
	return output; 
}

// works for any Ki67-based cell cycle model 
std::vector<std::string> false_cell_coloring_Ki67( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
    
    // output[0] = cyto_color, output[1] = cyto_outline , output[2] = nuclear_color, output[3] = nuclear_outline

	// positive_premitotic - Green
	if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::Ki67_positive_premitotic || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::Ki67_positive )  
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,125,0)";
	}

    if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::Ki67_positive_postmitotic )  // postive_postmitotic - Magenta
    {
        output[0] = "rgb(255,0,255)";
        output[2] = "rgb(125,0,125)";
    }

    if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::Ki67_negative )  // Ki-67 negative/Quiescent - Blue
    {
        output[0] = "rgb(40,200,255)";
        output[2] = "rgb(20,100,255)";
    }

    if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )  // Apoptotic - Red
    {
        output[0] = "rgb(255,0,0)";
        output[2] = "rgb(125,0,0)";
    }
	
	// Necrotic - Brown
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
    }
    
    return output;
}

std::vector<std::string> false_cell_coloring_live_dead( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
    
	// output[0] = cyto_color, output[1] = cyto_outline , output[2] = nuclear_color, output[3] = nuclear_outline

	// live cell - Green
	if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::live )  
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,125,0)";
		return output; 
	}
	
	// if not, dead colors 
	
	if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )  // Apoptotic - Red
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
	}
	
	// Necrotic - Brown
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
	}	
	
	return output; 
}

// works for any Ki67-based cell cycle model 
std::vector<std::string> false_cell_coloring_cycling_quiescent( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
    
    // output[0] = cyto_color, output[1] = cyto_outline , output[2] = nuclear_color, output[3] = nuclear_outline

	// Cycling - Green
	if ( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::cycling )  
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,125,0)";
	}

	// Quiescent - Blue 
    if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::quiescent ) 
    {
        output[0] = "rgb(40,200,255)";
        output[2] = "rgb(20,100,255)";
    }

	// Apoptotic - Red
    if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )  
    {
        output[0] = "rgb(255,0,0)";
        output[2] = "rgb(125,0,0)";
    }
	
	// Necrotic - Brown
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
    }
    
    return output;
}

std::vector<std::string> false_cell_coloring_cytometry( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
	
	// First, check for death. Use standard dead colors and exit
	
	
	if (pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic )  // Apoptotic - Red
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
		return output; 
	}
	
	// Necrotic - Brown
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
		return output; 
	}		
	
	
	// Check if this coloring function even makes sense, and if so,
	
	if( pCell->phenotype.cycle.model().code != PhysiCell_constants::flow_cytometry_separated_cycle_model &&  
	    pCell->phenotype.cycle.model().code != PhysiCell_constants::flow_cytometry_cycle_model )
	{ return output; }
	
	// G0/G1 and G1 are blue 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::G0G1_phase || 
	    pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::G1_phase )
	{
		output[0] = "rgb(0,80,255)"; 
		output[2] = "rgb(0,40,255)"; 
		return output; 
	}

	// G0 is pale blue 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::G0_phase )
	{
		output[0] = "rgb(40,200,255)";
		output[2] = "rgb(20,100,255)";
		return output; 
	}
	
	// S is magenta  
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::S_phase )
	{
		output[0] = "rgb(255, 0, 255)";
		output[2] = "rgb(190,0,190)";
		return output; 
	}
	
	// G2 is yellow
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::G2_phase )
	{
		output[0] = "rgb(255, 255, 0)";
		output[2] = "rgb(190, 190, 0)";
		return output; 
	}
	
	// G2/M and M are green 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::G2M_phase || 
	    pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::M_phase )
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,190,0)";
		
		return output; 
	}
	
	return output; 
}

std::vector<double> transmission( std::vector<double>& incoming_light, std::vector<double>& absorb_color, double thickness , double stain )
{
	double param = thickness * stain / 255.0; 

	static std::vector<double> output( 3, 0.0 );
 
	for( int i=0; i < 3 ; i++ )
	{ output[i] = incoming_light[i] * exp( -param * absorb_color[i] ); }
 
	return output;
}

std::vector<std::string> hematoxylin_eosin_DAB_cell_coloring( Cell* pCell )
{
	static std::vector<std::string> out( 4, "rgb(255,255,255)" );
	// cyto_color, cyto_outline , nuclear_color, nuclear_outline

	// cytoplasm colors 
  
	double fluid_fraction = pCell->phenotype.volume.cytoplasmic_fluid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double solid_fraction = pCell->phenotype.volume.cytoplasmic_solid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double calc_fraction  = pCell->phenotype.volume.calcified_fraction; 
 
	static double thickness = 20;
 
	static std::vector<double> light( 3, 255.0 ); 
 
	static std::vector<double> eosin_absorb = {2.55,33.15,2.55}; // ( 3 , 3.0 ); // (3,33,3)
	static std::vector<double> hematoxylin_absorb = {45.90,51.00,20.40}; // ( 3, 45.0 ); // (45,51,20)
	static std::vector<double> DAB_absorb = {65.93,109.14,129.82}; 

	static std::vector<double> temp( 3, 0.0 );
 
	// eosin staining of cytoplasmic basics 
	temp = transmission( light, eosin_absorb , thickness , solid_fraction );
	// hematoxylin staining of cytoplasmic calcifications 
	temp = transmission( temp , hematoxylin_absorb ,thickness, calc_fraction );
	// DAB staining of cytoplasm (if any) 
	temp = transmission( temp , DAB_absorb ,thickness, pCell->custom_data["DAB_cytoplasm"]*solid_fraction );
 
	static char szTempString [128]; 
	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[0].assign( szTempString ); 
	out[1] = out[0]; 
 
	// nuclear colors 
 
	// fluid_fraction = pCell->phenotype.volume.nuclear_fluid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->phenotype.volume.nuclear_fluid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 
	solid_fraction = pCell->phenotype.volume.nuclear_solid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->State.nuclear_solid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 

	// hematoxylin staining 
	temp = transmission( light , hematoxylin_absorb , thickness , solid_fraction );
	// DAB staining of nucleus (if any) 
	temp = transmission( temp , DAB_absorb ,thickness, pCell->custom_data["DAB_nucleus"]*solid_fraction );

	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[2].assign( szTempString ); 
	out[3] = out[2]; 

	return out;

}

std::vector<std::string> hematoxylin_and_eosin_cell_coloring( Cell* pCell )
{
	static std::vector<std::string> out( 4, "rgb(255,255,255)" );
	// cyto_color, cyto_outline , nuclear_color, nuclear_outline

	// cytoplasm colors 
  
	double fluid_fraction = pCell->phenotype.volume.cytoplasmic_fluid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double solid_fraction = pCell->phenotype.volume.cytoplasmic_solid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double calc_fraction  = pCell->phenotype.volume.calcified_fraction; 
 
	static double thickness = 20;
 
	static std::vector<double> light( 3, 255.0 ); 
 
	static std::vector<double> eosin_absorb = {2.55,33.15,2.55}; // ( 3 , 3.0 ); // (3,33,3)
	static std::vector<double> hematoxylin_absorb = {45.90,51.00,20.40}; // ( 3, 45.0 ); // (45,51,20)

	static std::vector<double> temp( 3, 0.0 );
 
	temp = transmission( light, eosin_absorb , thickness , solid_fraction );
	temp = transmission( temp , hematoxylin_absorb ,thickness, calc_fraction );
 
	static char szTempString [128]; 
	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[0].assign( szTempString ); 
	out[1] = out[0]; 
 
	// nuclear colors 
 
	// fluid_fraction = pCell->phenotype.volume.nuclear_fluid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->phenotype.volume.nuclear_fluid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 
	solid_fraction = pCell->phenotype.volume.nuclear_solid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->State.nuclear_solid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 
 
	temp = transmission( light , hematoxylin_absorb , thickness , solid_fraction );

	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[2].assign( szTempString ); 
	out[3] = out[2]; 

	return out;
}

std::string formatted_minutes_to_DDHHMM( double minutes )
{
	static std::string output; 
	output.resize( 1024 ); 
	
	int nMinutes = rint(minutes); // round( minutes ); 
	// int nDays = (int) floor( (minutes+1e-6) / 1440.0 ); // minutes / 1440 
	int nDays = nMinutes / 1440; 
	nMinutes -= nDays*1440; 
	
	// int nHours = (int) floor( (nMinutes+1e-6) / 60.0 ); // nMinutes / 60;
	int nHours = nMinutes / 60; 
	double dMinutes = minutes - 60*( nDays*24 + nHours ); 
	if( dMinutes < 0 )
	{ dMinutes = 0.0; }
	sprintf( (char*) output.c_str(),"%d days, %d hours, and %2.2f minutes", nDays,nHours,dMinutes);
	
	return output ;
}

void SVG_plot(std::string filename, Microenvironment &M, double z_slice, double time, std::vector<std::string> (*cell_coloring_function)(Cell *), std::string (*substrate_coloring_function)(double, double, double), void(cell_counts_function)(char *))
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

	if(PhysiCell_settings.enable_substrate_plot == true && (*substrate_coloring_function) != NULL){

		double legend_padding = 200.0; // I have to add a margin on the left to visualize the bar plot and the values

		Write_SVG_start( os, plot_width + legend_padding, plot_height + top_margin );

		// draw the background
		Write_SVG_rect( os , 0 , 0 , plot_width + legend_padding, plot_height + top_margin , 0.002 * plot_height , "white", "white" );

	}
	else{

		Write_SVG_start( os, plot_width , plot_height + top_margin );

		// draw the background
		Write_SVG_rect( os , 0 , 0 , plot_width, plot_height + top_margin , 0.002 * plot_height , "white", "white" );

	}
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
	
	if (cell_counts_function != NULL){
		cell_counts_function(szString);
	} else {
		sprintf( szString , "%u agents" , total_cell_count ); 
	}
	
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

	// used for the legend
	double max_conc;
	double min_conc;
	// color in the background ECM
	if(PhysiCell_settings.enable_substrate_plot == true && (*substrate_coloring_function) != NULL)
	{
		double dz_stroma = M.mesh.dz;

		std::string sub = PhysiCell_settings.substrate_to_monitor;
		int sub_index = M.find_density_index(sub); // check the substrate does actually exist
		if(sub_index == -1){
			std::cout << "ERROR SAMPLING THE SUBSTRATE: COULD NOT FIND THE SUBSTRATE " << sub << std::endl; //if not print error message
		}
		else
		{
			if(PhysiCell_settings.limits_substrate_plot){
			 max_conc = PhysiCell_settings.max_concentration;
			 min_conc = PhysiCell_settings.min_concentration;
			}
			else{
			 max_conc = M.density_vector(5)[sub_index];
			 min_conc = M.density_vector(5)[sub_index];	 // so here I am sampling the concentration to set a min and a mx
			//look for the max and min concentration among all the substrates
			for (int n = 0; n < M.number_of_voxels(); n++)
			{
				double concentration = M.density_vector(n)[sub_index];
				if (concentration > max_conc)
					max_conc = concentration;
				if (concentration < min_conc)
					min_conc = concentration;
			}
			};

			//check that max conc is not zero otherwise it is a big problem!
			if(max_conc == 0){

				max_conc = 1.0;

			};

			for (int n = 0; n < M.number_of_voxels(); n++)
			{
				auto current_voxel = M.voxels(n);
				int z_center = current_voxel.center[2];
				double z_displ = z_center -  dz_stroma/2; 

				double z_compare = z_displ;

				if (default_microenvironment_options.simulate_2D == true){
					z_compare = z_center;
				};

				if (z_slice == z_compare){			//this is to make sure the substrate is sampled in the voxel visualized (so basically the slice)
					int x_center = current_voxel.center[0];
					int y_center = current_voxel.center[1];
					
					double x_displ = x_center -  dx_stroma/2;
					double y_displ = (y_center - dy_stroma) +  dy_stroma/2;

					double concentration = M.density_vector(n)[sub_index];

					std::string output = substrate_coloring_function(concentration, max_conc, min_conc );
					Write_SVG_rect( os , x_displ - X_lower , y_displ - Y_lower, dx_stroma, dy_stroma , 0 , "none", output );
				}

			}

		}
	}
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

		if( fabs( (pC->position)[2] - z_slice ) < pC->phenotype.geometry.radius )
		{
			os << "   <g id=\"cell" << pC->ID << "\" "
			   << "type=\"" << pC->type_name << "\" "; // new April 2022
			if( pC->phenotype.death.dead == true )
			{ os << "dead=\"true\" " ; } 
			else
			{ os << "dead=\"false\" " ; } 
			os << ">" << std::endl;

			pC->functions.plot_agent_SVG(os, pC, z_slice, cell_coloring_function, X_lower, Y_lower);

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

	if (PhysiCell_settings.enable_substrate_plot == true && (*substrate_coloring_function) != NULL) {

		// add legend for the substrate

		double conc_interval = (max_conc - min_conc) / 10; // setting the interval for the values in the legend.

		szString = new char [1024];
		double upper_left_x = plot_width + 25.0;
		double sub_rect_height = (plot_height - 25.0) / 10.0;
		for(int i = 0; i <= 9; i++){ //creating 10 rectangoles for the bar, each one with a different shade of color.

			double concentration_sample = min_conc + (conc_interval * (9-i)); // the color depends on the concentration, starting from the min concentration to the max (which was sampled before)

			std::string output = substrate_coloring_function(concentration_sample, max_conc, min_conc);

			double upper_left_y = sub_rect_height * i; // here I set the position of each rectangole

			Write_SVG_rect(os, upper_left_x, top_margin + upper_left_y, 25.0, sub_rect_height, 0.002 * plot_height , "none", output); //drawing each piece of the barplot

			if(i%2 != 0){ // of course I am not printing each value of the barplot, otherwise is too crowded, so just one each 2

				sprintf( szString , " %.2g", concentration_sample);
				Write_SVG_rect(os, upper_left_x + 25, top_margin + upper_left_y + sub_rect_height - (0.001 * plot_height), 3, 0.002 * plot_height, 0 , "rgb(0,0,0)", "rgb(0,0,0)");
				Write_SVG_text( os , szString, upper_left_x + 28, top_margin + upper_left_y + sub_rect_height, font_size ,
							   PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() ); // misterious values set with a trial and error approach due to OCD. But now the legend is coherent at pixel level
			}
		}

		sprintf( szString , "%.2g", max_conc);

		Write_SVG_rect(os, upper_left_x + 25, top_margin - (0.001 * plot_height), 3, 0.002 * plot_height, 0 , "rgb(0,0,0)", "rgb(0,0,0)");
		Write_SVG_text( os , szString, upper_left_x + 28, top_margin, font_size , 
			PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() ); // misterious values set with a trial and error approach due to OCD. But now the legend is coherent at pixel level

		delete [] szString;

		// add a label to the right of the colorbar defined by above Write_SVG_rect calls
		Write_SVG_text(os, PhysiCell_settings.substrate_to_monitor.c_str(), upper_left_x + 35, top_margin + plot_height / 2, font_size,
					   PhysiCell_SVG_options.font_color.c_str(), PhysiCell_SVG_options.font.c_str(), 90.0);
	}

	Write_SVG_rect(os, 25.0 + plot_width, top_margin, 25.0, plot_height - 25, 0.002 * plot_height , "black", "none"); // nice black contour around the legend

	// close the svg tag, close the file
	Write_SVG_end( os ); 
	os.close();

	return;
}

void standard_agent_SVG(std::ofstream& os, PhysiCell::Cell* pC, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower) {

	double r = pC->phenotype.geometry.radius ; 
	double rn = pC->phenotype.geometry.nuclear_radius ; 
	double z = fabs( (pC->position)[2] - z_slice) ; 

	std::vector<std::string> Colors = cell_coloring_function( pC ); 
	
	// figure out how much of the cell intersects with z = 0
	double plot_radius = sqrt( r*r - z*z );

	// then normal cell, plot sphere if it intersects z = 0;
	Write_SVG_circle( os, (pC->position)[0]-X_lower, (pC->position)[1]-Y_lower,
						plot_radius , 0.5, Colors[1], Colors[0] );
	// plot the nucleus if it, too intersects z = 0;
	if( fabs(z) < rn && PhysiCell_SVG_options.plot_nuclei == true )
	{
		plot_radius = sqrt( rn*rn - z*z );
		Write_SVG_circle( os, (pC->position)[0]-X_lower, (pC->position)[1]-Y_lower,
							plot_radius, 0.5, Colors[3],Colors[2]);
	}
}

void setup_svg_substrate_colormap(std::vector<std::string> &colormap)
{
	std::string map_name = PhysiCell_settings.svg_substrate_colormap;
	int name_length = map_name.size();
	bool is_reversed = false;
	if (map_name.substr(name_length-2,2)=="_r")
	{
		map_name = map_name.substr(0,name_length-2);
		is_reversed = true;
	}
	if (map_name=="original")
	{
		for(unsigned int i = 0; i<256; i++)
		{
			char color[128];
			sprintf(color, "rgb(%u,234,197)", 255 - i);
			colormap.push_back(color);
		}
	}
	else if (map_name=="YlOrRd")
	{
		colormap.assign({"rgb(255,255,204)",
						 "rgb(255,250,193)",
						 "rgb(255,246,181)",
						 "rgb(255,241,170)",
						 "rgb(255,236,159)",
						 "rgb(255,231,148)",
						 "rgb(254,226,137)",
						 "rgb(254,221,126)",
						 "rgb(254,214,115)",
						 "rgb(254,204,104)",
						 "rgb(254,194,94)",
						 "rgb(254,184,83)",
						 "rgb(254,174,74)",
						 "rgb(254,165,70)",
						 "rgb(253,155,66)",
						 "rgb(253,146,62)",
						 "rgb(253,133,58)",
						 "rgb(253,117,53)",
						 "rgb(252,100,48)",
						 "rgb(252,84,44)",
						 "rgb(248,70,40)",
						 "rgb(242,56,36)",
						 "rgb(235,43,33)",
						 "rgb(229,29,29)",
						 "rgb(220,21,30)",
						 "rgb(210,14,33)",
						 "rgb(200,8,35)",
						 "rgb(190,1,38)",
						 "rgb(175,0,38)",
						 "rgb(159,0,38)",
						 "rgb(144,0,38)",
						 "rgb(128,0,38)"});
	}
	else if (map_name=="viridis")
	{
		colormap.assign({"rgb(68,1,84)",
						 "rgb(71,13,96)",
						 "rgb(72,24,106)",
						 "rgb(72,35,116)",
						 "rgb(71,46,124)",
						 "rgb(69,56,130)",
						 "rgb(66,65,134)",
						 "rgb(62,74,137)",
						 "rgb(58,84,140)",
						 "rgb(54,93,141)",
						 "rgb(50,101,142)",
						 "rgb(46,109,142)",
						 "rgb(43,117,142)",
						 "rgb(40,125,142)",
						 "rgb(37,132,142)",
						 "rgb(34,140,141)",
						 "rgb(31,148,140)",
						 "rgb(30,156,137)",
						 "rgb(32,163,134)",
						 "rgb(37,171,130)",
						 "rgb(46,179,124)",
						 "rgb(58,186,118)",
						 "rgb(72,193,110)",
						 "rgb(88,199,101)",
						 "rgb(108,205,90)",
						 "rgb(127,211,78)",
						 "rgb(147,215,65)",
						 "rgb(168,219,52)",
						 "rgb(192,223,37)",
						 "rgb(213,226,26)",
						 "rgb(234,229,26)",
						 "rgb(253,231,37)"});
	}
	else if (map_name=="turbo")
	{
		colormap.assign({"rgb(48,18,59)",
						 "rgb(50,21,67)",
						 "rgb(51,24,74)",
						 "rgb(52,27,81)",
						 "rgb(53,30,88)",
						 "rgb(54,33,95)",
						 "rgb(55,36,102)",
						 "rgb(56,39,109)",
						 "rgb(57,42,115)",
						 "rgb(58,45,121)",
						 "rgb(59,47,128)",
						 "rgb(60,50,134)",
						 "rgb(61,53,139)",
						 "rgb(62,56,145)",
						 "rgb(63,59,151)",
						 "rgb(63,62,156)",
						 "rgb(64,64,162)",
						 "rgb(65,67,167)",
						 "rgb(65,70,172)",
						 "rgb(66,73,177)",
						 "rgb(66,75,181)",
						 "rgb(67,78,186)",
						 "rgb(68,81,191)",
						 "rgb(68,84,195)",
						 "rgb(68,86,199)",
						 "rgb(69,89,203)",
						 "rgb(69,92,207)",
						 "rgb(69,94,211)",
						 "rgb(70,97,214)",
						 "rgb(70,100,218)",
						 "rgb(70,102,221)",
						 "rgb(70,105,224)",
						 "rgb(70,107,227)",
						 "rgb(71,110,230)",
						 "rgb(71,113,233)",
						 "rgb(71,115,235)",
						 "rgb(71,118,238)",
						 "rgb(71,120,240)",
						 "rgb(71,123,242)",
						 "rgb(70,125,244)",
						 "rgb(70,128,246)",
						 "rgb(70,130,248)",
						 "rgb(70,133,250)",
						 "rgb(70,135,251)",
						 "rgb(69,138,252)",
						 "rgb(69,140,253)",
						 "rgb(68,143,254)",
						 "rgb(67,145,254)",
						 "rgb(66,148,255)",
						 "rgb(65,150,255)",
						 "rgb(64,153,255)",
						 "rgb(62,155,254)",
						 "rgb(61,158,254)",
						 "rgb(59,160,253)",
						 "rgb(58,163,252)",
						 "rgb(56,165,251)",
						 "rgb(55,168,250)",
						 "rgb(53,171,248)",
						 "rgb(51,173,247)",
						 "rgb(49,175,245)",
						 "rgb(47,178,244)",
						 "rgb(46,180,242)",
						 "rgb(44,183,240)",
						 "rgb(42,185,238)",
						 "rgb(40,188,235)",
						 "rgb(39,190,233)",
						 "rgb(37,192,231)",
						 "rgb(35,195,228)",
						 "rgb(34,197,226)",
						 "rgb(32,199,223)",
						 "rgb(31,201,221)",
						 "rgb(30,203,218)",
						 "rgb(28,205,216)",
						 "rgb(27,208,213)",
						 "rgb(26,210,210)",
						 "rgb(26,212,208)",
						 "rgb(25,213,205)",
						 "rgb(24,215,202)",
						 "rgb(24,217,200)",
						 "rgb(24,219,197)",
						 "rgb(24,221,194)",
						 "rgb(24,222,192)",
						 "rgb(24,224,189)",
						 "rgb(25,226,187)",
						 "rgb(25,227,185)",
						 "rgb(26,228,182)",
						 "rgb(28,230,180)",
						 "rgb(29,231,178)",
						 "rgb(31,233,175)",
						 "rgb(32,234,172)",
						 "rgb(34,235,170)",
						 "rgb(37,236,167)",
						 "rgb(39,238,164)",
						 "rgb(42,239,161)",
						 "rgb(44,240,158)",
						 "rgb(47,241,155)",
						 "rgb(50,242,152)",
						 "rgb(53,243,148)",
						 "rgb(56,244,145)",
						 "rgb(60,245,142)",
						 "rgb(63,246,138)",
						 "rgb(67,247,135)",
						 "rgb(70,248,132)",
						 "rgb(74,248,128)",
						 "rgb(78,249,125)",
						 "rgb(82,250,122)",
						 "rgb(85,250,118)",
						 "rgb(89,251,115)",
						 "rgb(93,252,111)",
						 "rgb(97,252,108)",
						 "rgb(101,253,105)",
						 "rgb(105,253,102)",
						 "rgb(109,254,98)",
						 "rgb(113,254,95)",
						 "rgb(117,254,92)",
						 "rgb(121,254,89)",
						 "rgb(125,255,86)",
						 "rgb(128,255,83)",
						 "rgb(132,255,81)",
						 "rgb(136,255,78)",
						 "rgb(139,255,75)",
						 "rgb(143,255,73)",
						 "rgb(146,255,71)",
						 "rgb(150,254,68)",
						 "rgb(153,254,66)",
						 "rgb(156,254,64)",
						 "rgb(159,253,63)",
						 "rgb(161,253,61)",
						 "rgb(164,252,60)",
						 "rgb(167,252,58)",
						 "rgb(169,251,57)",
						 "rgb(172,251,56)",
						 "rgb(175,250,55)",
						 "rgb(177,249,54)",
						 "rgb(180,248,54)",
						 "rgb(183,247,53)",
						 "rgb(185,246,53)",
						 "rgb(188,245,52)",
						 "rgb(190,244,52)",
						 "rgb(193,243,52)",
						 "rgb(195,241,52)",
						 "rgb(198,240,52)",
						 "rgb(200,239,52)",
						 "rgb(203,237,52)",
						 "rgb(205,236,52)",
						 "rgb(208,234,52)",
						 "rgb(210,233,53)",
						 "rgb(212,231,53)",
						 "rgb(215,229,53)",
						 "rgb(217,228,54)",
						 "rgb(219,226,54)",
						 "rgb(221,224,55)",
						 "rgb(223,223,55)",
						 "rgb(225,221,55)",
						 "rgb(227,219,56)",
						 "rgb(229,217,56)",
						 "rgb(231,215,57)",
						 "rgb(233,213,57)",
						 "rgb(235,211,57)",
						 "rgb(236,209,58)",
						 "rgb(238,207,58)",
						 "rgb(239,205,58)",
						 "rgb(241,203,58)",
						 "rgb(242,201,58)",
						 "rgb(244,199,58)",
						 "rgb(245,197,58)",
						 "rgb(246,195,58)",
						 "rgb(247,193,58)",
						 "rgb(248,190,57)",
						 "rgb(249,188,57)",
						 "rgb(250,186,57)",
						 "rgb(251,184,56)",
						 "rgb(251,182,55)",
						 "rgb(252,179,54)",
						 "rgb(252,177,54)",
						 "rgb(253,174,53)",
						 "rgb(253,172,52)",
						 "rgb(254,169,51)",
						 "rgb(254,167,50)",
						 "rgb(254,164,49)",
						 "rgb(254,161,48)",
						 "rgb(254,158,47)",
						 "rgb(254,155,45)",
						 "rgb(254,153,44)",
						 "rgb(254,150,43)",
						 "rgb(254,147,42)",
						 "rgb(254,144,41)",
						 "rgb(253,141,39)",
						 "rgb(253,138,38)",
						 "rgb(252,135,37)",
						 "rgb(252,132,35)",
						 "rgb(251,129,34)",
						 "rgb(251,126,33)",
						 "rgb(250,123,31)",
						 "rgb(249,120,30)",
						 "rgb(249,117,29)",
						 "rgb(248,114,28)",
						 "rgb(247,111,26)",
						 "rgb(246,108,25)",
						 "rgb(245,105,24)",
						 "rgb(244,102,23)",
						 "rgb(243,99,21)",
						 "rgb(242,96,20)",
						 "rgb(241,93,19)",
						 "rgb(240,91,18)",
						 "rgb(239,88,17)",
						 "rgb(237,85,16)",
						 "rgb(236,83,15)",
						 "rgb(235,80,14)",
						 "rgb(234,78,13)",
						 "rgb(232,75,12)",
						 "rgb(231,73,12)",
						 "rgb(229,71,11)",
						 "rgb(228,69,10)",
						 "rgb(226,67,10)",
						 "rgb(225,65,9)",
						 "rgb(223,63,8)",
						 "rgb(221,61,8)",
						 "rgb(220,59,7)",
						 "rgb(218,57,7)",
						 "rgb(216,55,6)",
						 "rgb(214,53,6)",
						 "rgb(212,51,5)",
						 "rgb(210,49,5)",
						 "rgb(208,47,5)",
						 "rgb(206,45,4)",
						 "rgb(204,43,4)",
						 "rgb(202,42,4)",
						 "rgb(200,40,3)",
						 "rgb(197,38,3)",
						 "rgb(195,37,3)",
						 "rgb(193,35,2)",
						 "rgb(190,33,2)",
						 "rgb(188,32,2)",
						 "rgb(185,30,2)",
						 "rgb(183,29,2)",
						 "rgb(180,27,1)",
						 "rgb(178,26,1)",
						 "rgb(175,24,1)",
						 "rgb(172,23,1)",
						 "rgb(169,22,1)",
						 "rgb(167,20,1)",
						 "rgb(164,19,1)",
						 "rgb(161,18,1)",
						 "rgb(158,16,1)",
						 "rgb(155,15,1)",
						 "rgb(152,14,1)",
						 "rgb(149,13,1)",
						 "rgb(146,11,1)",
						 "rgb(142,10,1)",
						 "rgb(139,9,2)",
						 "rgb(136,8,2)",
						 "rgb(133,7,2)",
						 "rgb(129,6,2)",
						 "rgb(126,5,2)",
						 "rgb(122,4,3)"});
	}
	else if (map_name == "jet")
	{
		colormap.assign({"rgb(0,0,131)",
						 "rgb(0,0,135)",
						 "rgb(0,0,139)",
						 "rgb(0,0,143)",
						 "rgb(0,0,147)",
						 "rgb(0,0,151)",
						 "rgb(0,0,155)",
						 "rgb(0,0,159)",
						 "rgb(0,0,163)",
						 "rgb(0,0,167)",
						 "rgb(0,0,171)",
						 "rgb(0,0,175)",
						 "rgb(0,0,179)",
						 "rgb(0,0,183)",
						 "rgb(0,0,187)",
						 "rgb(0,0,191)",
						 "rgb(0,0,195)",
						 "rgb(0,0,199)",
						 "rgb(0,0,203)",
						 "rgb(0,0,207)",
						 "rgb(0,0,211)",
						 "rgb(0,0,215)",
						 "rgb(0,0,219)",
						 "rgb(0,0,223)",
						 "rgb(0,0,227)",
						 "rgb(0,0,231)",
						 "rgb(0,0,235)",
						 "rgb(0,0,239)",
						 "rgb(0,0,243)",
						 "rgb(0,0,247)",
						 "rgb(0,0,251)",
						 "rgb(0,0,255)",
						 "rgb(0,4,255)",
						 "rgb(0,8,255)",
						 "rgb(0,12,255)",
						 "rgb(0,16,255)",
						 "rgb(0,20,255)",
						 "rgb(0,24,255)",
						 "rgb(0,28,255)",
						 "rgb(0,32,255)",
						 "rgb(0,36,255)",
						 "rgb(0,40,255)",
						 "rgb(0,44,255)",
						 "rgb(0,48,255)",
						 "rgb(0,52,255)",
						 "rgb(0,56,255)",
						 "rgb(0,60,255)",
						 "rgb(0,64,255)",
						 "rgb(0,68,255)",
						 "rgb(0,72,255)",
						 "rgb(0,76,255)",
						 "rgb(0,80,255)",
						 "rgb(0,84,255)",
						 "rgb(0,88,255)",
						 "rgb(0,92,255)",
						 "rgb(0,96,255)",
						 "rgb(0,100,255)",
						 "rgb(0,104,255)",
						 "rgb(0,108,255)",
						 "rgb(0,112,255)",
						 "rgb(0,116,255)",
						 "rgb(0,120,255)",
						 "rgb(0,124,255)",
						 "rgb(0,128,255)",
						 "rgb(0,131,255)",
						 "rgb(0,135,255)",
						 "rgb(0,139,255)",
						 "rgb(0,143,255)",
						 "rgb(0,147,255)",
						 "rgb(0,151,255)",
						 "rgb(0,155,255)",
						 "rgb(0,159,255)",
						 "rgb(0,163,255)",
						 "rgb(0,167,255)",
						 "rgb(0,171,255)",
						 "rgb(0,175,255)",
						 "rgb(0,179,255)",
						 "rgb(0,183,255)",
						 "rgb(0,187,255)",
						 "rgb(0,191,255)",
						 "rgb(0,195,255)",
						 "rgb(0,199,255)",
						 "rgb(0,203,255)",
						 "rgb(0,207,255)",
						 "rgb(0,211,255)",
						 "rgb(0,215,255)",
						 "rgb(0,219,255)",
						 "rgb(0,223,255)",
						 "rgb(0,227,255)",
						 "rgb(0,231,255)",
						 "rgb(0,235,255)",
						 "rgb(0,239,255)",
						 "rgb(0,243,255)",
						 "rgb(0,247,255)",
						 "rgb(0,251,255)",
						 "rgb(0,255,255)",
						 "rgb(4,255,251)",
						 "rgb(8,255,247)",
						 "rgb(12,255,243)",
						 "rgb(16,255,239)",
						 "rgb(20,255,235)",
						 "rgb(24,255,231)",
						 "rgb(28,255,227)",
						 "rgb(32,255,223)",
						 "rgb(36,255,219)",
						 "rgb(40,255,215)",
						 "rgb(44,255,211)",
						 "rgb(48,255,207)",
						 "rgb(52,255,203)",
						 "rgb(56,255,199)",
						 "rgb(60,255,195)",
						 "rgb(64,255,191)",
						 "rgb(68,255,187)",
						 "rgb(72,255,183)",
						 "rgb(76,255,179)",
						 "rgb(80,255,175)",
						 "rgb(84,255,171)",
						 "rgb(88,255,167)",
						 "rgb(92,255,163)",
						 "rgb(96,255,159)",
						 "rgb(100,255,155)",
						 "rgb(104,255,151)",
						 "rgb(108,255,147)",
						 "rgb(112,255,143)",
						 "rgb(116,255,139)",
						 "rgb(120,255,135)",
						 "rgb(124,255,131)",
						 "rgb(128,255,128)",
						 "rgb(131,255,124)",
						 "rgb(135,255,120)",
						 "rgb(139,255,116)",
						 "rgb(143,255,112)",
						 "rgb(147,255,108)",
						 "rgb(151,255,104)",
						 "rgb(155,255,100)",
						 "rgb(159,255,96)",
						 "rgb(163,255,92)",
						 "rgb(167,255,88)",
						 "rgb(171,255,84)",
						 "rgb(175,255,80)",
						 "rgb(179,255,76)",
						 "rgb(183,255,72)",
						 "rgb(187,255,68)",
						 "rgb(191,255,64)",
						 "rgb(195,255,60)",
						 "rgb(199,255,56)",
						 "rgb(203,255,52)",
						 "rgb(207,255,48)",
						 "rgb(211,255,44)",
						 "rgb(215,255,40)",
						 "rgb(219,255,36)",
						 "rgb(223,255,32)",
						 "rgb(227,255,28)",
						 "rgb(231,255,24)",
						 "rgb(235,255,20)",
						 "rgb(239,255,16)",
						 "rgb(243,255,12)",
						 "rgb(247,255,8)",
						 "rgb(251,255,4)",
						 "rgb(255,255,0)",
						 "rgb(255,251,0)",
						 "rgb(255,247,0)",
						 "rgb(255,243,0)",
						 "rgb(255,239,0)",
						 "rgb(255,235,0)",
						 "rgb(255,231,0)",
						 "rgb(255,227,0)",
						 "rgb(255,223,0)",
						 "rgb(255,219,0)",
						 "rgb(255,215,0)",
						 "rgb(255,211,0)",
						 "rgb(255,207,0)",
						 "rgb(255,203,0)",
						 "rgb(255,199,0)",
						 "rgb(255,195,0)",
						 "rgb(255,191,0)",
						 "rgb(255,187,0)",
						 "rgb(255,183,0)",
						 "rgb(255,179,0)",
						 "rgb(255,175,0)",
						 "rgb(255,171,0)",
						 "rgb(255,167,0)",
						 "rgb(255,163,0)",
						 "rgb(255,159,0)",
						 "rgb(255,155,0)",
						 "rgb(255,151,0)",
						 "rgb(255,147,0)",
						 "rgb(255,143,0)",
						 "rgb(255,139,0)",
						 "rgb(255,135,0)",
						 "rgb(255,131,0)",
						 "rgb(255,128,0)",
						 "rgb(255,124,0)",
						 "rgb(255,120,0)",
						 "rgb(255,116,0)",
						 "rgb(255,112,0)",
						 "rgb(255,108,0)",
						 "rgb(255,104,0)",
						 "rgb(255,100,0)",
						 "rgb(255,96,0)",
						 "rgb(255,92,0)",
						 "rgb(255,88,0)",
						 "rgb(255,84,0)",
						 "rgb(255,80,0)",
						 "rgb(255,76,0)",
						 "rgb(255,72,0)",
						 "rgb(255,68,0)",
						 "rgb(255,64,0)",
						 "rgb(255,60,0)",
						 "rgb(255,56,0)",
						 "rgb(255,52,0)",
						 "rgb(255,48,0)",
						 "rgb(255,44,0)",
						 "rgb(255,40,0)",
						 "rgb(255,36,0)",
						 "rgb(255,32,0)",
						 "rgb(255,28,0)",
						 "rgb(255,24,0)",
						 "rgb(255,20,0)",
						 "rgb(255,16,0)",
						 "rgb(255,12,0)",
						 "rgb(255,8,0)",
						 "rgb(255,4,0)",
						 "rgb(255,0,0)",
						 "rgb(251,0,0)",
						 "rgb(247,0,0)",
						 "rgb(243,0,0)",
						 "rgb(239,0,0)",
						 "rgb(235,0,0)",
						 "rgb(231,0,0)",
						 "rgb(227,0,0)",
						 "rgb(223,0,0)",
						 "rgb(219,0,0)",
						 "rgb(215,0,0)",
						 "rgb(211,0,0)",
						 "rgb(207,0,0)",
						 "rgb(203,0,0)",
						 "rgb(199,0,0)",
						 "rgb(195,0,0)",
						 "rgb(191,0,0)",
						 "rgb(187,0,0)",
						 "rgb(183,0,0)",
						 "rgb(179,0,0)",
						 "rgb(175,0,0)",
						 "rgb(171,0,0)",
						 "rgb(167,0,0)",
						 "rgb(163,0,0)",
						 "rgb(159,0,0)",
						 "rgb(155,0,0)",
						 "rgb(151,0,0)",
						 "rgb(147,0,0)",
						 "rgb(143,0,0)",
						 "rgb(139,0,0)",
						 "rgb(135,0,0)",
						 "rgb(131,0,0)",
						 "rgb(128,0,0)"});
	}
	else if (map_name == "plasma")
	{
		colormap.assign({"rgb(13,8,135)",
						 "rgb(16,7,136)",
						 "rgb(19,7,137)",
						 "rgb(22,7,138)",
						 "rgb(25,6,140)",
						 "rgb(27,6,141)",
						 "rgb(29,6,142)",
						 "rgb(32,6,143)",
						 "rgb(34,6,144)",
						 "rgb(36,6,145)",
						 "rgb(38,5,145)",
						 "rgb(40,5,146)",
						 "rgb(42,5,147)",
						 "rgb(44,5,148)",
						 "rgb(46,5,149)",
						 "rgb(47,5,150)",
						 "rgb(49,5,151)",
						 "rgb(51,5,151)",
						 "rgb(53,4,152)",
						 "rgb(55,4,153)",
						 "rgb(56,4,154)",
						 "rgb(58,4,154)",
						 "rgb(60,4,155)",
						 "rgb(62,4,156)",
						 "rgb(63,4,156)",
						 "rgb(65,4,157)",
						 "rgb(67,3,158)",
						 "rgb(68,3,158)",
						 "rgb(70,3,159)",
						 "rgb(72,3,159)",
						 "rgb(73,3,160)",
						 "rgb(75,3,161)",
						 "rgb(76,2,161)",
						 "rgb(78,2,162)",
						 "rgb(80,2,162)",
						 "rgb(81,2,163)",
						 "rgb(83,2,163)",
						 "rgb(85,2,164)",
						 "rgb(86,1,164)",
						 "rgb(88,1,164)",
						 "rgb(89,1,165)",
						 "rgb(91,1,165)",
						 "rgb(92,1,166)",
						 "rgb(94,1,166)",
						 "rgb(96,1,166)",
						 "rgb(97,0,167)",
						 "rgb(99,0,167)",
						 "rgb(100,0,167)",
						 "rgb(102,0,167)",
						 "rgb(103,0,168)",
						 "rgb(105,0,168)",
						 "rgb(106,0,168)",
						 "rgb(108,0,168)",
						 "rgb(110,0,168)",
						 "rgb(111,0,168)",
						 "rgb(113,0,168)",
						 "rgb(114,1,168)",
						 "rgb(116,1,168)",
						 "rgb(117,1,168)",
						 "rgb(119,1,168)",
						 "rgb(120,1,168)",
						 "rgb(122,2,168)",
						 "rgb(123,2,168)",
						 "rgb(125,3,168)",
						 "rgb(126,3,168)",
						 "rgb(128,4,168)",
						 "rgb(129,4,167)",
						 "rgb(131,5,167)",
						 "rgb(132,5,167)",
						 "rgb(134,6,166)",
						 "rgb(135,7,166)",
						 "rgb(136,8,166)",
						 "rgb(138,9,165)",
						 "rgb(139,10,165)",
						 "rgb(141,11,165)",
						 "rgb(142,12,164)",
						 "rgb(143,13,164)",
						 "rgb(145,14,163)",
						 "rgb(146,15,163)",
						 "rgb(148,16,162)",
						 "rgb(149,17,161)",
						 "rgb(150,19,161)",
						 "rgb(152,20,160)",
						 "rgb(153,21,159)",
						 "rgb(154,22,159)",
						 "rgb(156,23,158)",
						 "rgb(157,24,157)",
						 "rgb(158,25,157)",
						 "rgb(160,26,156)",
						 "rgb(161,27,155)",
						 "rgb(162,29,154)",
						 "rgb(163,30,154)",
						 "rgb(165,31,153)",
						 "rgb(166,32,152)",
						 "rgb(167,33,151)",
						 "rgb(168,34,150)",
						 "rgb(170,35,149)",
						 "rgb(171,36,148)",
						 "rgb(172,38,148)",
						 "rgb(173,39,147)",
						 "rgb(174,40,146)",
						 "rgb(176,41,145)",
						 "rgb(177,42,144)",
						 "rgb(178,43,143)",
						 "rgb(179,44,142)",
						 "rgb(180,46,141)",
						 "rgb(181,47,140)",
						 "rgb(182,48,139)",
						 "rgb(183,49,138)",
						 "rgb(184,50,137)",
						 "rgb(186,51,136)",
						 "rgb(187,52,136)",
						 "rgb(188,53,135)",
						 "rgb(189,55,134)",
						 "rgb(190,56,133)",
						 "rgb(191,57,132)",
						 "rgb(192,58,131)",
						 "rgb(193,59,130)",
						 "rgb(194,60,129)",
						 "rgb(195,61,128)",
						 "rgb(196,62,127)",
						 "rgb(197,64,126)",
						 "rgb(198,65,125)",
						 "rgb(199,66,124)",
						 "rgb(200,67,123)",
						 "rgb(201,68,122)",
						 "rgb(202,69,122)",
						 "rgb(203,70,121)",
						 "rgb(204,71,120)",
						 "rgb(204,73,119)",
						 "rgb(205,74,118)",
						 "rgb(206,75,117)",
						 "rgb(207,76,116)",
						 "rgb(208,77,115)",
						 "rgb(209,78,114)",
						 "rgb(210,79,113)",
						 "rgb(211,81,113)",
						 "rgb(212,82,112)",
						 "rgb(213,83,111)",
						 "rgb(213,84,110)",
						 "rgb(214,85,109)",
						 "rgb(215,86,108)",
						 "rgb(216,87,107)",
						 "rgb(217,88,106)",
						 "rgb(218,90,106)",
						 "rgb(218,91,105)",
						 "rgb(219,92,104)",
						 "rgb(220,93,103)",
						 "rgb(221,94,102)",
						 "rgb(222,95,101)",
						 "rgb(222,97,100)",
						 "rgb(223,98,99)",
						 "rgb(224,99,99)",
						 "rgb(225,100,98)",
						 "rgb(226,101,97)",
						 "rgb(226,102,96)",
						 "rgb(227,104,95)",
						 "rgb(228,105,94)",
						 "rgb(229,106,93)",
						 "rgb(229,107,93)",
						 "rgb(230,108,92)",
						 "rgb(231,110,91)",
						 "rgb(231,111,90)",
						 "rgb(232,112,89)",
						 "rgb(233,113,88)",
						 "rgb(233,114,87)",
						 "rgb(234,116,87)",
						 "rgb(235,117,86)",
						 "rgb(235,118,85)",
						 "rgb(236,119,84)",
						 "rgb(237,121,83)",
						 "rgb(237,122,82)",
						 "rgb(238,123,81)",
						 "rgb(239,124,81)",
						 "rgb(239,126,80)",
						 "rgb(240,127,79)",
						 "rgb(240,128,78)",
						 "rgb(241,129,77)",
						 "rgb(241,131,76)",
						 "rgb(242,132,75)",
						 "rgb(243,133,75)",
						 "rgb(243,135,74)",
						 "rgb(244,136,73)",
						 "rgb(244,137,72)",
						 "rgb(245,139,71)",
						 "rgb(245,140,70)",
						 "rgb(246,141,69)",
						 "rgb(246,143,68)",
						 "rgb(247,144,68)",
						 "rgb(247,145,67)",
						 "rgb(247,147,66)",
						 "rgb(248,148,65)",
						 "rgb(248,149,64)",
						 "rgb(249,151,63)",
						 "rgb(249,152,62)",
						 "rgb(249,154,62)",
						 "rgb(250,155,61)",
						 "rgb(250,156,60)",
						 "rgb(250,158,59)",
						 "rgb(251,159,58)",
						 "rgb(251,161,57)",
						 "rgb(251,162,56)",
						 "rgb(252,163,56)",
						 "rgb(252,165,55)",
						 "rgb(252,166,54)",
						 "rgb(252,168,53)",
						 "rgb(252,169,52)",
						 "rgb(253,171,51)",
						 "rgb(253,172,51)",
						 "rgb(253,174,50)",
						 "rgb(253,175,49)",
						 "rgb(253,177,48)",
						 "rgb(253,178,47)",
						 "rgb(253,180,47)",
						 "rgb(253,181,46)",
						 "rgb(254,183,45)",
						 "rgb(254,184,44)",
						 "rgb(254,186,44)",
						 "rgb(254,187,43)",
						 "rgb(254,189,42)",
						 "rgb(254,190,42)",
						 "rgb(254,192,41)",
						 "rgb(253,194,41)",
						 "rgb(253,195,40)",
						 "rgb(253,197,39)",
						 "rgb(253,198,39)",
						 "rgb(253,200,39)",
						 "rgb(253,202,38)",
						 "rgb(253,203,38)",
						 "rgb(252,205,37)",
						 "rgb(252,206,37)",
						 "rgb(252,208,37)",
						 "rgb(252,210,37)",
						 "rgb(251,211,36)",
						 "rgb(251,213,36)",
						 "rgb(251,215,36)",
						 "rgb(250,216,36)",
						 "rgb(250,218,36)",
						 "rgb(249,220,36)",
						 "rgb(249,221,37)",
						 "rgb(248,223,37)",
						 "rgb(248,225,37)",
						 "rgb(247,226,37)",
						 "rgb(247,228,37)",
						 "rgb(246,230,38)",
						 "rgb(246,232,38)",
						 "rgb(245,233,38)",
						 "rgb(245,235,39)",
						 "rgb(244,237,39)",
						 "rgb(243,238,39)",
						 "rgb(243,240,39)",
						 "rgb(242,242,39)",
						 "rgb(241,244,38)",
						 "rgb(241,245,37)",
						 "rgb(240,247,36)",
						 "rgb(240,249,33)"});
	}
	else 
	{
		std::cout << "WARNING : The colormap " << map_name << " has not been specified for the SVG substrate plot." << std::endl
				  << "\tUsing YlOrRd." << std::endl
				  << std::endl;
		PhysiCell_settings.svg_substrate_colormap = "YlOrRd";
		setup_svg_substrate_colormap(colormap);
	}
	
	if (is_reversed)
	{
		for (unsigned int i = 0; i<colormap.size(); i++)
		{
			int I1 = i;
			int I2 = colormap.size()-1-i;
			if (I2<I1) { break; }
			std::string temp = colormap[I2];
			colormap[I2] = colormap[I1];
			colormap[I1] = temp;
		}
	}
}

std::string paint_by_density_percentage(double concentration, double max_conc, double min_conc)
{
	static std::vector< std::string > colormap;
	static int n_color_bins;
	static bool is_colormap_setup = false;

	if (!is_colormap_setup)
	{
		setup_svg_substrate_colormap(colormap);
		n_color_bins = colormap.size();
		is_colormap_setup = true;
	}
	std::string output;
	int ind = (int)round(((concentration - min_conc) / (max_conc - min_conc)) * n_color_bins);
	if (ind >= n_color_bins)
	{
		ind = n_color_bins - 1;
	}
	else if (ind < 0)
	{
		ind = 0;
	}
	// std::cout << "colormap[ind] = " << colormap[ind] << std::endl;
	return colormap[ind];
}

std::vector<std::string> paint_by_number_cell_coloring( Cell* pCell )
{
	static std::vector< std::string > colors(0); 
	static bool setup_done = false; 
	if( setup_done == false )
	{
		colors.push_back( "grey" ); // default color will be grey 

		colors.push_back( "red" );
		colors.push_back( "yellow" ); 	
		colors.push_back( "green" ); 	
		colors.push_back( "blue" ); 
		
		colors.push_back( "magenta" ); 	
		colors.push_back( "orange" ); 	
		colors.push_back( "lime" ); 	
		colors.push_back( "cyan" );
		
		colors.push_back( "hotpink" ); 	
		colors.push_back( "peachpuff" ); 	
		colors.push_back( "darkseagreen" ); 	
		colors.push_back( "lightskyblue" );

		setup_done = true; 
	}
	
	// start all black 
	
	std::vector<std::string> output = { "black", "black", "black", "black" }; 
	
	// paint by number -- by cell type 
	
	std::string interior_color = "white"; 
	if( pCell->type < 13 )
	{ interior_color = colors[ pCell->type ]; }
	
	output[0] = interior_color; // set cytoplasm color 
	
	/*
	if( pCell->phenotype.death.dead == false ) // if live, color nucleus same color 
	{
		output[2] = interior_color; 
		output[3] = interior_color; 
	}
	else
	{
		// apoptotic cells will retain a black nucleus 
		// if necrotic, color the nucleus brown 
		if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
			pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
		{
			output[2] = "rgb(139,69,19)";
			output[3] = "rgb(139,69,19)";
		}



	}
	*/

	// new March 2023 (for better compatibility with studio)

	// if dead, use live color for the outline
	// if( pCell->phenotype.death.dead == true )
	// { output[1] = interior_color; } 

	// necrotic cells are brown 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::necrotic )
	{ interior_color = "saddlebrown"; }
	// apoptotic cells are white 
	if( pCell->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic ) 
	{ interior_color = "black"; }

	output[0] = interior_color; // set cytoplasm color 
	output[2] = interior_color; // set cytoplasm color 
	output[3] = interior_color; // set cytoplasm color 

	output[1] = "black"; 
	
	return output; 
}	

void create_plot_legend( std::string filename , std::vector<std::string> (*cell_coloring_function)(Cell*) )
{
	int number_of_cell_types = cell_definitions_by_index.size(); 
	
	double temp_cell_radius = 25; 
	double temp_cell_volume = 4.1887902047863909846168578443727 * pow( temp_cell_radius , 3.0 ); 

	double relative_padding = 0.15; 
	double padding = relative_padding * 2.0 * temp_cell_radius; 

	double row_height = 2.0 * temp_cell_radius + 2*padding; 
	
	double font_size = 0.85 * 2.0 * temp_cell_radius; 
	double row_width  = 2.0 * temp_cell_radius + 2*padding + ( 32 * font_size ) + 2 * padding; 
	
	double total_height = number_of_cell_types * row_height; 
	double total_width  = row_width; 
	
	
	std::ofstream os( filename , std::ios::out );
	Write_SVG_start( os , total_width ,total_height ); 
	
	double cursor_x = padding + temp_cell_radius; 
	double cursor_y = padding + temp_cell_radius; 
	
	for( int k=0 ; k < number_of_cell_types ; k++ )
	{
		Cell_Definition* cell_def = cell_definitions_by_index[k];
		cell_def->functions.plot_agent_legend(os, cell_def, cursor_x, cursor_y, cell_coloring_function, temp_cell_radius);
				
		// place the label
		cursor_x += temp_cell_radius + 2*padding; 
		cursor_y += 0.3*font_size; 
		
		Write_SVG_text( os , cell_definitions_by_index[k]->name.c_str() , cursor_x , cursor_y, font_size , 
			PhysiCell_SVG_options.font_color.c_str() , PhysiCell_SVG_options.font.c_str() );
		
		// move the cursor down to the next row 
		
		cursor_y -= 0.3*font_size; 
		cursor_y += ( 2.0 * padding + 2.0*temp_cell_radius ); 
		cursor_x = padding + temp_cell_radius;
	}
	
	Write_SVG_end( os ); 
	os.close(); 
}

void standard_agent_legend(std::ofstream& os, Cell_Definition* cell_definition, double& cursor_x, double& cursor_y, std::vector<std::string> (*cell_coloring_function)(Cell*), double temp_cell_radius) {
	
	// switch to the cell type 
	Cell C; 
	C.convert_to_cell_definition( *cell_definition );
	
	// get the colors using the current coloring function 
	std::vector<std::string> colors = cell_coloring_function(&C); 	

	// place a big circle with cytoplasm colors 
	Write_SVG_circle(os,cursor_x, cursor_y , temp_cell_radius , 1.0 , colors[1] , colors[0] ); 
	// place a small circle with nuclear colors 
	Write_SVG_circle(os,cursor_x, cursor_y , 0.5*temp_cell_radius , 1.0 , colors[3] , colors[2] ); 

}

};
