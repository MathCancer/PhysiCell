/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based simulator for multicellular systemssimulator, 	#
#	 J. Comput. Biol., 2016 (submitted). 									# 
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 0.5.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, and P Macklin, PhysiCell: an open       #
#    source physics-based multicellular simulator, J. Comput. Biol., 2016   # 
#   (submitted).                                                            #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "./BioFVM/BioFVM.h"
#include "./core/PhysiCell.h" 

#include "./modules/PhysiCell_SVG.h"
#include "./modules/PhysiCell_pathology.h"

#include <omp.h>

using namespace BioFVM; 
using namespace PhysiCell; 

int main( int argc, char* argv[] )
{
	RUNTIME_TIC();
	omp_set_num_threads( 8 );
	
	seed_random();
	
	std::cout << "Starting program ... " << std::endl;

	// now, biofvm setup
	
	set_program_metadata(); 
	set_save_biofvm_cell_data( true ); 	
	set_save_biofvm_cell_data_as_custom_matlab( true );	
	
	// display summary information 	
	M.display_information( std::cout ); 	
	
  // PhysiCell SVG options 
	PhysiCell_SVG_options.length_bar = 1000; 
	PhysiCell_SVG_options.font_color = "black";
	PhysiCell_SVG_options.font = "Arial";
	PhysiCell_SVG_options.plot_nuclei = false; 
	
	// save the initial profile 
	
	// M.write_to_matlab( "initial.mat" ); // barebones 
	SVG_plot( "initial.svg", M , 0.0, 0 , liver_coloring_function ) ; // hematoxylin_and_eosin_cell_coloring ); // simple_cell_coloring );
	save_BioFVM_to_MultiCellDS_xml_pugi( "initial" , M , 0.0 ); // MultiCellDS digital snapshot
	
	double t     = 0.0; 
	double t_max = 480; // 1 * 24 * 60;
	double dt    = 0.01; 
	
	double output_interval  = 60;  // how often you save data 
	double next_output_time = t;     // next time you save data 
	
	double cell_cycle_dt = 6; 
	double mechanics_dt = 0.1; 

	TIC();
	while( t < t_max )
	{
		// std::cout << t << std::endl; 
		
		// if it's time, save the simulation 
		if( fabs( t - next_output_time ) < dt/2.0 )
		{
			std::cout << "simulation time: " << t << " " << M.time_units << " (" << t_max << " " << M.time_units << " max)" << std::endl; 
			TOC();
			display_stopwatch_value( std::cout , stopwatch_value() ); 
			std::cout << std::endl; 
			
			char* filename; 
			filename = new char [1024];
			
			// sprintf( filename, "output_%6f.mat" , next_output_time ); 
			// M.write_to_matlab( filename ); 

			sprintf( filename, "output_%6f" , next_output_time ); 
			save_BioFVM_to_MultiCellDS_xml_pugi( filename , M , 0.0 ); // MultiCellDS digital snapshot
			// sprintf( filename , "%s.svg" , filename );
			SVG_plot( filename , M , 0.0, next_output_time , liver_coloring_function ) ;
			char szCommand [1024]; 
			sprintf( szCommand , "cp %s final.svg" , filename );
			system( szCommand );
			
			delete [] filename; 
			next_output_time += output_interval; 
			
			TIC();
		}
//		M.simulate_bulk_sources_and_sinks( dt );
		M.simulate_diffusion_decay( dt );
		M.simulate_cell_sources_and_sinks( dt ); 
		((Cell_Container *)M.agent_container)->update_all_cells(t, cell_cycle_dt, mechanics_dt);	// crashes ??
		
		t += dt;
	}
	
	// M.write_to_matlab( "final.mat"); // barebones 
	SVG_plot( "final.svg", M , 0.0, t_max , liver_coloring_function ) ; // hematoxylin_and_eosin_cell_coloring ); // simple_cell_coloring );
	save_BioFVM_to_MultiCellDS_xml_pugi( "final" , M , t_max ); // MultiCellDS digital snapshot

	RUNTIME_TOC();
	std::cout << std::endl << "Program wall time: "; 
	std::cout << runtime_stopwatch_value() << std::endl; 
	display_stopwatch_value( std::cout , runtime_stopwatch_value() ); 
	std::cout << std::endl << "Done!" << std::endl; 
	
	return 0; 
}
