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
/*	
	static bool setup_done = false; 
	if( !setup_done )
	{
		eosin_absorb[1] = 33.0;
		hematoxylin_absorb[1] = 51.0; 
		hematoxylin_absorb[2] = 20.0; 
		setup_done = true; 
	}
*/	

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

void SVG_plot( std::string filename , Microenvironment& M, double z_slice , double time, std::vector<std::string> (*cell_coloring_function)(Cell*) )
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

	// create a temporary cell 
	Cell* pCell; 
	pCell = create_cell(); 
	
	double cursor_x = padding + temp_cell_radius; 
	double cursor_y = padding + temp_cell_radius; 
	
	for( int k=0 ; k < number_of_cell_types ; k++ )
	{
		// switch to the cell type 
		pCell->convert_to_cell_definition( *(cell_definitions_by_index[k]) );
		
		// get the colors using the current coloring function 
		std::vector<std::string> colors = cell_coloring_function(pCell); 
		
		// place a big circle with cytoplasm colors 
		Write_SVG_circle(os,cursor_x, cursor_y , temp_cell_radius , 1.0 , colors[1] , colors[0] ); 
		// place a small circle with nuclear colors 
		Write_SVG_circle(os,cursor_x, cursor_y , 0.5*temp_cell_radius , 1.0 , colors[2] , colors[3] ); 
		
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
	
	// get rid of this temp cell at the earliest opportunity. 
	// make it harmless for now 
	pCell->assign_position( 0,0,0 ); 
	pCell->turn_off_reactions(0.0); 
	pCell->set_total_volume( 0.0 ); 
	pCell->start_death(0); 
 	pCell->phenotype.cycle.data.exit_rate(0) = 9e99; 
	
	Write_SVG_end( os ); 
	os.close(); 
}

};
