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
# Copyright (c) 2015-2022, Paul Macklin and the PhysiCell Project             #
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


// load standard library
#include <stdbool.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <sys/stat.h>

// loade PhysiCell library
#include "./core/PhysiCell.h"
#include "./modules/PhysiCell_standard_modules.h"
#include "./custom_modules/custom.h"

// load namespace
using namespace BioFVM;
using namespace PhysiCell;


// main function
int main( int argc, char* argv[] )
	{

	////////////////////////
	// EPISODE LOOP BEGIN //
	////////////////////////

	for ( int i_episode = 0; i_episode < 4; i_episode++ )
	{
		///////////
		// start //
		///////////

		std::cout << "\nrun episode: " << i_episode << " !" << std::endl;

		// variables
		char filename[1024];
		std::ofstream report_file;
		std::vector<std::string> ( *cell_coloring_function )( Cell* ) = my_coloring_function;  // set a pathology coloring function
		std::string ( *substrate_coloring_function )( double, double, double ) = paint_by_density_percentage;

		// generate output folder name
		std::string s_episode = std::to_string( i_episode );
		std::string folder = "output/episode" + s_episode.insert( 0, 8 - s_episode.length(), '0' );

		// handle settings file
		const char* settingxml = "config/PhysiCell_settings.xml";
		if ( argc > 1 ) { settingxml = argv[1]; };

		// reset global variables
		std::cout << "(re)set global variables ..." << std::endl;
		PhysiCell_globals = PhysiCell_Globals();

		// time setup
		std::string time_units = "min";

		// densities and cell types can only be defined in the first episode
		// and have to be reloaded in all following episodes!
		if ( i_episode == 0 )
		{
			// load xml file
			std::cout << "load setting xml " << settingxml << " ..." << std::endl;
			bool XML_status = false;
			XML_status = load_PhysiCell_config_file( settingxml );
			PhysiCell_settings.folder = folder;
			create_output_directory( PhysiCell_settings.folder );

			// OpenMP setup
			omp_set_num_threads( PhysiCell_settings.omp_num_threads );

			// setup microenviroment and mechanics voxel size and match the data structure to BioFVM
			std::cout << "set densities ..." << std::endl;
			setup_microenvironment();  // modify this in the custom code
			double mechanics_voxel_size = 30;
			Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );

			// load cell type definition and setup tisse
			std::cout << "load cell type definition and setup tissue ..." << std::endl;
			create_cell_types();  // modify this in the custom code
			setup_tissue();  // modify this in the custom code

			// set MultiCellDS save options
			set_save_biofvm_mesh_as_matlab( true );
			set_save_biofvm_data_as_matlab( true );
			set_save_biofvm_cell_data( true );
			set_save_biofvm_cell_data_as_custom_matlab( true );

		}
		else
		{
			// load xml file
			std::cout << "load setting xml " << settingxml << " ..." << std::endl;
			bool XML_status = false;
			XML_status = read_PhysiCell_config_file( settingxml );
			if ( XML_status ) { PhysiCell_settings.read_from_pugixml(); }
			if ( !XML_status ) { exit( -1 ); }
			PhysiCell_settings.folder = folder;
			create_output_directory( PhysiCell_settings.folder );

			// OpenMP setup
			omp_set_num_threads( PhysiCell_settings.omp_num_threads );

			// reset cells
			std::cout << "reset cells ..." << std::endl;
			for ( Cell* pCell: (*all_cells) )
			{
				pCell->die();
			}
			BioFVM::reset_max_basic_agent_ID();

			// reset mesh0
			std::cout << "reset mesh0 ..." << std::endl;
			BioFVM::reset_BioFVM_substrates_initialized_in_dom();

			// reset microenvironment and mechanics voxel size and match the data structure to BioFVM
			std::cout << "reset densities ..." << std::endl;
			set_microenvironment_initial_condition();
			microenvironment.display_information( std::cout );
			double mechanics_voxel_size = 30;
			Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );

			// reset tissue
			std::cout << "reset tissue ..." << std::endl;
			display_cell_definitions( std::cout );
			setup_tissue();  // modify this in the custom code

			// MultiCellDS save options
			// have only to be set once per runtime
		}

		// copy config file to output directory
		char copy_command [1024];
		sprintf( copy_command, "cp %s %s", settingxml, PhysiCell_settings.folder.c_str() );
		system( copy_command );

		// save initial data simulation snapshot
		sprintf( filename, "%s/initial", PhysiCell_settings.folder.c_str() );
		save_PhysiCell_to_MultiCellDS_v2( filename, microenvironment, PhysiCell_globals.current_time );

		// save data simulation snapshot output00000000
		if ( PhysiCell_settings.enable_full_saves == true )
		{
			sprintf( filename, "%s/output%08u", PhysiCell_settings.folder.c_str(), PhysiCell_globals.full_output_index );
			save_PhysiCell_to_MultiCellDS_v2( filename, microenvironment, PhysiCell_globals.current_time );
		}

		// save initial svg cross section through z = 0 and legend
		PhysiCell_SVG_options.length_bar = 200;  // set cross section length bar to 200 microns

		sprintf( filename, "%s/legend.svg", PhysiCell_settings.folder.c_str() );
		create_plot_legend( filename, cell_coloring_function );

		sprintf( filename, "%s/initial.svg", PhysiCell_settings.folder.c_str() );
		SVG_plot( filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function, substrate_coloring_function );

		// save svg cross section snapshot00000000
		if ( PhysiCell_settings.enable_SVG_saves == true )
		{
			sprintf( filename, "%s/snapshot%08u.svg", PhysiCell_settings.folder.c_str(), PhysiCell_globals.SVG_output_index );
			SVG_plot( filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function, substrate_coloring_function );
		}

		// save legacy simulation report
		if ( PhysiCell_settings.enable_legacy_saves == true )
		{
			sprintf( filename, "%s/simulation_report.txt", PhysiCell_settings.folder.c_str() );
			report_file.open( filename );  // create the data log file
			report_file << "simulated time\tnum cells\tnum division\tnum death\twall time" << std::endl;
			log_output( PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file );  // output00000000
		}

		// standard output
		display_citations();
		display_simulation_status( std::cout );  // output00000000

		// set the performance timers
		BioFVM::RUNTIME_TIC();
		BioFVM::TIC();


		//////////
		// step //
		//////////

		// main loop
		try
		{
			// set time variables
			double custom_dt = 60; // min
			double custom_countdown = custom_dt;
			double phenotype_countdown = phenotype_dt;
			double mechanics_countdown = mechanics_dt;
			double mcds_countdown = PhysiCell_settings.full_save_interval;
			double svg_countdown = PhysiCell_settings.SVG_save_interval;

			// run diffusion time step paced main loop
			bool step = true;
			while ( step )
			{

				// max time reached?
				if ( PhysiCell_globals.current_time > PhysiCell_settings.max_time )
				{
					step = false;
				}

				// on custom time step
				if ( custom_countdown < 0.5 * diffusion_dt )
				{
					custom_countdown += custom_dt;

					// Put custom time scale code here!
					//std::cout << "processing custom time step block ... " << std::endl;
					// Custom add ons could potentially go here.
				}

				// on phenotype time step
				if ( phenotype_countdown < 0.5 * diffusion_dt )
				{
					phenotype_countdown += phenotype_dt;

					// Put phenotype time scale code here!
					//std::cout << "processing phenotype time step observation block ... " << std::endl;
				}

				// on mechanics time step
				if ( mechanics_countdown < 0.5 * diffusion_dt )
				{
					mechanics_countdown += mechanics_dt;

					// Put mechanics time scale code here!
					//std::cout << "processing mechanic time step observation block ... " << std::endl;
				}

				// on diffusion time step
				// Put diffusion time scale code here!
				//std::cout << "processing diffusion time step observation block ... " << std::endl << std::endl;

				// run microenvironment
				microenvironment.simulate_diffusion_decay( diffusion_dt );

				// run PhysiCell
				( (Cell_Container *)microenvironment.agent_container )->update_all_cells ( PhysiCell_globals.current_time );

				// update time
				custom_countdown -= diffusion_dt;
				phenotype_countdown -= diffusion_dt;
				mechanics_countdown -= diffusion_dt;
				mcds_countdown -= diffusion_dt;
				svg_countdown -= diffusion_dt;
				PhysiCell_globals.current_time += diffusion_dt;

				// save data if it's time.
				if ( mcds_countdown < 0.5 * diffusion_dt )
				{
					mcds_countdown += PhysiCell_settings.full_save_interval;
					PhysiCell_globals.full_output_index++;

					display_simulation_status( std::cout );

					// save data simulation snapshot
					if ( PhysiCell_settings.enable_full_saves == true )
					{
						sprintf( filename, "%s/output%08u", PhysiCell_settings.folder.c_str(), PhysiCell_globals.full_output_index );
						save_PhysiCell_to_MultiCellDS_v2( filename, microenvironment, PhysiCell_globals.current_time );
					}

					// save legacy simulation report
					if ( PhysiCell_settings.enable_legacy_saves == true )
					{
						log_output( PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file );
					}
				}

				// save svg plot if it's time
				if ( ( PhysiCell_settings.enable_SVG_saves == true ) and ( svg_countdown < 0.5 * diffusion_dt ) )
				{
					svg_countdown += PhysiCell_settings.SVG_save_interval;
					PhysiCell_globals.SVG_output_index++;

					// save final svg cross section
					sprintf( filename, "%s/snapshot%08u.svg", PhysiCell_settings.folder.c_str(), PhysiCell_globals.SVG_output_index );
					SVG_plot( filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function, substrate_coloring_function );
				}
			}

		}
		catch ( const std::exception& e )
		{  // reference to the base of a polymorphic object
			std::cout << e.what();  // information from length_error printed
		}

		//////////
		// stop //
		//////////

		// save final data simulation snapshot
		sprintf( filename, "%s/final", PhysiCell_settings.folder.c_str() );
		save_PhysiCell_to_MultiCellDS_v2( filename, microenvironment, PhysiCell_globals.current_time );

		// save final svg cross section
		sprintf( filename, "%s/final.svg", PhysiCell_settings.folder.c_str() );
		SVG_plot( filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function, substrate_coloring_function );

		// timer
		std::cout << std::endl << "Total simulation runtime: " << std::endl;
		BioFVM::display_stopwatch_value( std::cout, BioFVM::runtime_stopwatch_value() );
		std::cout << std::endl;

		// save legacy simulation report
		if ( PhysiCell_settings.enable_legacy_saves == true )
		{
			log_output( PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file );
			report_file.close();
		}


	//////////////////////
	// EPISODE LOOP END //
	//////////////////////

	}

	// going home
	return 0;
}
