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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>

#include "./core/PhysiCell.h"
#include "./modules/PhysiCell_standard_modules.h"

// put custom code modules here!

#include "./custom_modules/custom.h"

using namespace BioFVM;
using namespace PhysiCell;

int main(int argc, char *argv[])
{
	// load and parse settings file(s)

	bool XML_status = false;
	char copy_command[1024];
	if (argc > 1)
	{
		XML_status = load_PhysiCell_config_file(argv[1]);
		sprintf(copy_command, "cp %s %s", argv[1], PhysiCell_settings.folder.c_str());
	}
	else
	{
		XML_status = load_PhysiCell_config_file("./config/PhysiCell_settings.xml");
		sprintf(copy_command, "cp ./config/PhysiCell_settings.xml %s", PhysiCell_settings.folder.c_str());
	}
	if (!XML_status)
	{
		exit(-1);
	}

	// copy config file to output directry
	system(copy_command);

	// OpenMP setup
	omp_set_num_threads(PhysiCell_settings.omp_num_threads);

	// time setup
	std::string time_units = "min";

	/* Microenvironment setup */

	setup_microenvironment(); // modify this in the custom code

	/* PhysiCell setup */

	// set mechanics voxel size, and match the data structure to BioFVM
	double mechanics_voxel_size = 30;
	Cell_Container *cell_container = create_cell_container_for_microenvironment(microenvironment, mechanics_voxel_size);

	/* Users typically start modifying here. START USERMODS */

	/*
		for( int n=0; n < 100 ; n++ )
		{
			std::cout << "iteration: " << n << std::endl << "=================================" << std::endl;
			Hypothesis_Rule hr;
			hr.cell_type = cell_defaults.name;
			hr.pCell_Definition = &cell_defaults;
			hr.behavior = "migration speed";
			hr.add_signal( "oxygen" , 1 , 2, "increase" );
			hr.add_signal( "oxygen" , 3 , 4, "increase" );
			hr.add_signal( "oxygen" , 5, 6 , "decrease" );
			hr.add_signal( "pressure" , 5, 6 , "decrease" );
			hr.add_signal( "contact with tumor" , 5, 6 , "decrease" );
			hr.add_signal( "oxygen gradient" , 5, 6 , "increase" );
			hr.add_signal( "dead" , 5, 6 , "decrease" );
			hr.add_signal( "volume" , 5, 6 , "increase" );
			hr.add_signal( "intracellular crayons" , 5, 6 , "decrease" );

			hr.set_half_max( "oxygen" , 0.1 ); // done
			hr.set_hill_power( "shit" , 12  ); // done
			hr.set_response("oxygen" , "decreases" ); // done


			hr.set_half_max( "pressure" , 0.1 ); // done
			hr.set_hill_power( "oxygen" , 12  ); // done
			hr.set_response("volume" , "increases" ); // done

			hr.set_half_max( "shit" , 0.1 ); // done
			hr.set_hill_power( "intracellular crayons" , 12  ); // done
			hr.set_response("shit" , "increases" ); // done

			hr.detailed_display(std::cout);
			std::cout << std::endl;
		}
	*/

	/*
	hr.cell_type = "keratinocyte";
	hr.pCell_Definition = cell_defaults;



		std::string cell_type;
	Cell_Definition* pCell_Definition;

	std::string behavior;
	double base_value;
	double max_value;
	double min_value;

	std::vector< std::string > signals;
	std::vector<bool> responses;
	std::vector<double> half_maxes;
	std::vector<double> hill_powers;

	std::vector< std::string > up_signals;
	std::vector<double> up_half_maxes;
	std::vector<double> up_hill_powers;

	std::vector< std::string > down_signals;
	std::vector<double> down_half_maxes;
	std::vector<double> down_hill_powers;
*/

	create_cell_types();

	setup_tissue();

	/* Users typically stop modifying here. END USERMODS */

	// set MultiCellDS save options

	set_save_biofvm_mesh_as_matlab(true);
	set_save_biofvm_data_as_matlab(true);
	set_save_biofvm_cell_data(true);
	set_save_biofvm_cell_data_as_custom_matlab(true);

	// save a simulation snapshot

	char filename[1024];
	sprintf(filename, "%s/initial", PhysiCell_settings.folder.c_str());
	save_PhysiCell_to_MultiCellDS_v2(filename, microenvironment, PhysiCell_globals.current_time);

	// save a quick SVG cross section through z = 0, after setting its
	// length bar to 200 microns

	PhysiCell_SVG_options.length_bar = 200;

	// for simplicity, set a pathology coloring function

	std::vector<std::string> (*cell_coloring_function)(Cell *) = my_coloring_function;

	sprintf(filename, "%s/initial.svg", PhysiCell_settings.folder.c_str());
	SVG_plot(filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function);

	sprintf(filename, "%s/legend.svg", PhysiCell_settings.folder.c_str());
	create_plot_legend(filename, cell_coloring_function);

	display_citations();

	// set the performance timers

	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();

	std::ofstream report_file;
	if (PhysiCell_settings.enable_legacy_saves == true)
	{
		sprintf(filename, "%s/simulation_report.txt", PhysiCell_settings.folder.c_str());

		report_file.open(filename); // create the data log file
		report_file << "simulated time\tnum cells\tnum division\tnum death\twall time" << std::endl;
	}

	// main loop

	/* testing */
	/*
		Hypothesis_Rule hr;

		hr.behavior = "cycle entry";
		hr.sync_to_cell_definition( "default");

		hr.min_value = hr.base_value * 0.01;
		hr.max_value = hr.base_value * 10;

		hr.add_signal( "substrate" , 9 , 2 , "increase" );
		hr.add_signal( "contact with default" , 1 , 1 , "increase" );
		hr.add_signal( "pressure" , 0.5 , 2 , "decrease" );

		hr.display(std::cout);

		std::cout << hr.evaluate( {0,0,0}) << std::endl;
		std::cout << hr.evaluate( {9e9,9e9,9e9}) << std::endl;
		std::cout << hr.evaluate( {9e9,9e9,0}) << std::endl;
		std::cout << hr.evaluate( {9,0,0}) << std::endl;
		std::cout << hr.evaluate( {0,1,0}) << std::endl;
		std::cout << hr.evaluate( {0,0,0}) << std::endl;
		std::cout << hr.evaluate( {0,0,0.5}) << std::endl;
		std::cout << hr.evaluate( {18,0.08,0.05}) << std::endl;


		hr.detailed_display( std::cout );


		hr.set_response( "contact with default" , "increase");
		hr.set_hill_power( "contact with default" , 18 );
		hr.detailed_display( std::cout );

		hr.set_response( "contact with default" , "decrease");
		hr.set_half_max( "contact with default" , 5 );
		hr.detailed_display( std::cout );



		Cell* pCell = (*all_cells)[0];
		std::cout << "test evaluation: " << hr.evaluate(pCell) << std::endl;

		std::cout << "cell before the rule: " << hr.behavior << " : " << get_single_behavior( pCell , hr.behavior ) << std::endl;

		hr.apply(pCell);

		std::cout << "cell after the rule: " << hr.behavior << " : " << get_single_behavior( pCell , hr.behavior ) << std::endl;

		std::cout << std::endl << std::endl << std::endl << std::endl ;


		Hypothesis_Ruleset hrs;
		hrs.sync_to_cell_definition( "default");

		hrs.add_behavior( "cycle entry", 0 , 1e-3 );
		hrs["cycle entry"].add_signal( "substrate" , "increase" );
		hrs["cycle entry"].set_hill_power( "substrate", 4 );
		hrs["cycle entry"].set_half_max( "substrate", 21.5 );

		hrs["cycle entry"].add_signal( "contact with default" , 1 , 1 , "increase" );

		hrs["cycle entry"].add_signal( "pressure" , "decrease" );
		hrs["cycle entry"].set_half_max( "pressure" , 0.5 );
		hrs["cycle entry"].set_hill_power( "pressure" , 2);

		hrs.add_behavior( "apoptosis", 0 , 1e-1 );
		hrs["apoptosis"].add_signal( "damage" , 180 , 2 , "increase" );

		hrs.add_behavior( "necrosis", 0 , 1e-1 );
		hrs["necrosis"].add_signal( "substrate" , 2.5 , 2 , "decrease" );

		hrs.add_behavior( "migration speed" );

		hrs["migration speed"].add_signal( "contact with default" , 0.45 , 3 , "decrease" );
		// hrs["migration speed"].add_signal( "contact with default" , 0.45 , 3 , "decrease" );



		hrs.display( std::cout );

		*/

	//	intialize_hypothesis_rulesets();

	/*
		add_rule("tumor","oxygen","cycle entry","increase");
		set_hypothesis_parameters("tumor","oxygen","cycle entry",21.5,4);
		add_rule("tumor","pressure","cycle entry","decrease");
		set_hypothesis_parameters("tumor","pressure","cycle entry",0.25,3);
		set_behavior_parameters("tumor","cycle entry",0.0,0.0,0.0033);

		add_rule("tumor","oxygen","necrosis","decrease");

		set_hypothesis_parameters("tumor","oxygen","necrosis",3.75,8);
		set_behavior_parameters("tumor","necrosis",0.00,0.0028, 0);
	*/

	/*
		add_rule("tumor","damage","apoptosis","increase");
		add_rule("tumor","dead","debris export","increase");
	*/

	/*
		detailed_display_hypothesis_rulesets( std::cout );

		// std::vector<std::string> split_output;

		std::string csv_line = "tumor,cycle entry,0,1.7e-5,7e-4,oxygen,increases,21.5,4,0";
		// split_csv( csv_line , split_output , ',');
		// parse_csv_rule( split_output );
		parse_csv_rule( csv_line );

		csv_line = "tumor,cycle entry,0,1.7e-5,7e-4,pressure,decreases,0.25,3,0";
		// split_csv( csv_line , split_output , ',');
		// parse_csv_rule( split_output );
		parse_csv_rule( csv_line );

		csv_line = "tumor,debris secretion,0,0,1,dead,increases,0.1,10,1";
		// split_csv( csv_line , split_output , ',');
		// parse_csv_rule( split_output );
		parse_csv_rule( csv_line );
	*/

	/*
		parse_csv_rules( "./config/rules.csv") ;
		// parse_csv_rules( "./config/rules_tabs.txt") ;
		detailed_display_hypothesis_rulesets( std::cout );

		std::string my_str = "blah blah blah_blah";
		std::cout << my_str << " --> ";
		spaces_to_underscore( my_str );
		std::cout << my_str << std::endl;


		int nSearch = parameters.ints.find_index( "number_of_tumor" );
		std::cout << nSearch << std::endl;
		std::cout << parameters.ints[nSearch] << std::endl;


		std::cout << " *** *** *** done *** *** *** " << std::endl;

		exit(0);
	*/

	// std::cout << __FILE__ << " : " << __FUNCTION__ << " : " << __LINE__ << std::endl;
	// stream_annotated_English_rules(std::cout);
	// std::cout << __FILE__ << " : " << __FUNCTION__ << " : " << __LINE__ << std::endl;
	// export_rules_csv("test_rules.csv");

	try
	{
		while (PhysiCell_globals.current_time < PhysiCell_settings.max_time + 0.1 * diffusion_dt)
		{
			// save data if it's time.
			if (fabs(PhysiCell_globals.current_time - PhysiCell_globals.next_full_save_time) < 0.01 * diffusion_dt)
			{
				display_simulation_status(std::cout);
				if (PhysiCell_settings.enable_legacy_saves == true)
				{
					log_output(PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file);
				}

				if (PhysiCell_settings.enable_full_saves == true)
				{
					sprintf(filename, "%s/output%08u", PhysiCell_settings.folder.c_str(), PhysiCell_globals.full_output_index);

					save_PhysiCell_to_MultiCellDS_v2(filename, microenvironment, PhysiCell_globals.current_time);
				}

				PhysiCell_globals.full_output_index++;
				PhysiCell_globals.next_full_save_time += PhysiCell_settings.full_save_interval;
			}

			// save SVG plot if it's time
			if (fabs(PhysiCell_globals.current_time - PhysiCell_globals.next_SVG_save_time) < 0.01 * diffusion_dt)
			{
				if (PhysiCell_settings.enable_SVG_saves == true)
				{
					sprintf(filename, "%s/snapshot%08u.svg", PhysiCell_settings.folder.c_str(), PhysiCell_globals.SVG_output_index);
					SVG_plot(filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function);

					PhysiCell_globals.SVG_output_index++;
					PhysiCell_globals.next_SVG_save_time += PhysiCell_settings.SVG_save_interval;
				}
			}

			// update the microenvironment
			microenvironment.simulate_diffusion_decay(diffusion_dt);

			// run PhysiCell
			((Cell_Container *)microenvironment.agent_container)->update_all_cells(PhysiCell_globals.current_time);

			/*
			  Custom add-ons could potentially go here.
			*/
			turn_on_systemic_therapy("IFN-gamma", 0, 7200, 5);
			// turn_on_systemic_therapy("IL-4", 0, 7200, 5);
			// turn_on_injected_therapy("IL-4", {0, 0, 0}, 0, 7200, 5);

			PhysiCell_globals.current_time += diffusion_dt;
		}

		if (PhysiCell_settings.enable_legacy_saves == true)
		{
			log_output(PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file);
			report_file.close();
		}
	}
	catch (const std::exception &e)
	{						   // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}

	// save a final simulation snapshot

	sprintf(filename, "%s/final", PhysiCell_settings.folder.c_str());
	save_PhysiCell_to_MultiCellDS_v2(filename, microenvironment, PhysiCell_globals.current_time);

	sprintf(filename, "%s/final.svg", PhysiCell_settings.folder.c_str());
	SVG_plot(filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function);

	// timer

	std::cout << std::endl
			  << "Total simulation runtime: " << std::endl;
	BioFVM::display_stopwatch_value(std::cout, BioFVM::runtime_stopwatch_value());

	return 0;
}
