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


void update_intracellular()
{
    static int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" );
    static int glucose_substrate_index = microenvironment.find_density_index( "glucose" ); 
    static int lactate_substrate_index = microenvironment.find_density_index( "lactate");

    int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
    
    // std::cout << "----- update_intracellular at t= " << PhysiCell_globals.current_time << std::endl;
    
    #pragma omp parallel for 
    for( int i=0; i < (*all_cells).size(); i++ )
    {
        // static int i_Oxy_i = pCell->custom_data.find_variable_index( "intra_oxy" );
        // static int i_Glu_i = pCell->custom_data.find_variable_index( "intra_glu" );
        // static int i_Lac_i = pCell->custom_data.find_variable_index( "intra_lac" );
        // static int energy_vi = pCell->custom_data.find_variable_index( "intra_energy" );
        
        // if( ((*all_cells)[i]->phenotype.intracellular != NULL ) && ((*all_cells)[i]->is_out_of_domain == false ) )
        if( (*all_cells)[i]->is_out_of_domain == false  )
        {
            // std::cout << "Intracellular Oxygen : " <<(*all_cells)[i]->phenotype.molecular.internalized_total_substrates[oxygen_substrate_index] << std::endl;
            // std::cout << "Intracellular Glucose : " <<(*all_cells)[i]->phenotype.molecular.internalized_total_substrates[glucose_substrate_index] << std::endl;
            // std::cout << "Intracellular Lactate : " <<(*all_cells)[i]->phenotype.molecular.internalized_total_substrates[lactate_substrate_index] << std::endl;
            
            int vi = microenvironment.nearest_voxel_index((*all_cells)[i]->position);
            double oxy_val = microenvironment(vi)[oxygen_substrate_index];
            double glu_val = microenvironment(vi)[glucose_substrate_index];
            //double lac_val = microenvironment(vi)[lactate_substrate_index];
            
            //std::cout << "main.cpp:  oxy_val (from substrate)= " << oxy_val << std::endl; 
            (*all_cells)[i]->phenotype.intracellular->set_parameter_value("Oxygen",oxy_val);
            (*all_cells)[i]->phenotype.intracellular->set_parameter_value("Glucose",glu_val);
            
            //std::cout << "SBML Oxygen : " <<(*all_cells)[i]->phenotype.intracellular->get_parameter_value("Oxygen") << std::endl;
            (*all_cells)[i]->phenotype.intracellular->update();
            (*all_cells)[i]->phenotype.intracellular->update_phenotype_parameters((*all_cells)[i]->phenotype);
            // std::cout << "SBML Oxygen : " <<(*all_cells)[i]->phenotype.intracellular->get_parameter_value("Oxygen") << std::endl;
            // std::cout << "SBML Glucose : " <<(*all_cells)[i]->phenotype.intracellular->get_parameter_value("Glucose") << std::endl;
            // std::cout << "SBML Lactate : " <<(*all_cells)[i]->phenotype.intracellular->get_parameter_value("Lactate") << std::endl;

            //std::cout << "SBML migration_speed : " <<(*all_cells)[i]->phenotype.intracellular->get_parameter_value("Oxygen") << std::endl;
            
            // if ( (*all_cells)[i]->phenotype.intracellular->get_parameter_value("Energy") < 50.0 )
            // {
            //     (*all_cells)[i]->phenotype.motility.migration_speed = 100.0;
            // }
            // else
            // {
            //     (*all_cells)[i]->phenotype.motility.migration_speed = 0.0;
            // }
            // (*all_cells)[i]->phenotype.intracellular->update_phenotype_parameters((*all_cells)[i]->phenotype);
            
        }
    }
    // std::exit(-1);
}


int main( int argc, char* argv[] )
{
	// load and parse settings file(s)
	
	bool XML_status = false; 
	if( argc > 1 )
	{ XML_status = load_PhysiCell_config_file( argv[1] ); }
	else
	{ XML_status = load_PhysiCell_config_file( "./config/PhysiCell_settings.xml" ); }
	if( !XML_status )
	{ exit(-1); }
	
	// OpenMP setup
	omp_set_num_threads(PhysiCell_settings.omp_num_threads);
	
	// time setup 
	std::string time_units = "min"; 

	/* Microenvironment setup */ 
	
	setup_microenvironment(); // modify this in the custom code 
	
	/* PhysiCell setup */ 
 	
	// set mechanics voxel size, and match the data structure to BioFVM
	double mechanics_voxel_size = 30; 
	Cell_Container* cell_container = create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );
	
	/* Users typically start modifying here. START USERMODS */ 
	
	create_cell_types();
	
	setup_tissue();

	/* Users typically stop modifying here. END USERMODS */ 
	
	// set MultiCellDS save options 

	set_save_biofvm_mesh_as_matlab( true ); 
	set_save_biofvm_data_as_matlab( true ); 
	set_save_biofvm_cell_data( true ); 
	set_save_biofvm_cell_data_as_custom_matlab( true );
	
	// save a simulation snapshot 
	
	char filename[1024];
	sprintf( filename , "%s/initial" , PhysiCell_settings.folder.c_str() ); 
	save_PhysiCell_to_MultiCellDS_xml_pugi( filename , microenvironment , PhysiCell_globals.current_time ); 
	
	// save a quick SVG cross section through z = 0, after setting its 
	// length bar to 200 microns 

	PhysiCell_SVG_options.length_bar = 200; 

	// for simplicity, set a pathology coloring function 
	
	std::vector<std::string> (*cell_coloring_function)(Cell*) = my_coloring_function; 
	
	sprintf( filename , "%s/initial.svg" , PhysiCell_settings.folder.c_str() ); 
	SVG_plot( filename , microenvironment, 0.0 , PhysiCell_globals.current_time, cell_coloring_function );
	
	display_citations(); 
	
	// set the performance timers 

	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	
	std::ofstream report_file;
	if( PhysiCell_settings.enable_legacy_saves == true )
	{	
		sprintf( filename , "%s/simulation_report.txt" , PhysiCell_settings.folder.c_str() ); 
		
		report_file.open(filename); 	// create the data log file 
		report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time"<<std::endl;
	}
	
	// main loop 
	
    double last_intracellular_time  = 0.0; 
    static double intracellular_dt_tolerance = 0.001 * intracellular_dt; 
    double next_intracellular_update = intracellular_dt; 
    bool intracellular_initialized = false;
    PhysiCell_globals.next_intracellular_save_time = PhysiCell_settings.intracellular_save_interval;

	try 
	{		
		while( PhysiCell_globals.current_time < PhysiCell_settings.max_time + 0.1*diffusion_dt )
		{
			// save data if it's time. 
			if( fabs( PhysiCell_globals.current_time - PhysiCell_globals.next_full_save_time ) < 0.01 * diffusion_dt )
			{
				display_simulation_status( std::cout ); 
				if( PhysiCell_settings.enable_legacy_saves == true )
				{	
					log_output( PhysiCell_globals.current_time , PhysiCell_globals.full_output_index, microenvironment, report_file);
				}
				
				if( PhysiCell_settings.enable_full_saves == true )
				{	
					sprintf( filename , "%s/output%08u" , PhysiCell_settings.folder.c_str(),  PhysiCell_globals.full_output_index ); 
					
					save_PhysiCell_to_MultiCellDS_xml_pugi( filename , microenvironment , PhysiCell_globals.current_time ); 
				}
				
				PhysiCell_globals.full_output_index++; 
				PhysiCell_globals.next_full_save_time += PhysiCell_settings.full_save_interval;
			}
			
			// save SVG plot if it's time
			if( fabs( PhysiCell_globals.current_time - PhysiCell_globals.next_SVG_save_time  ) < 0.01 * diffusion_dt )
			{
				if( PhysiCell_settings.enable_SVG_saves == true )
				{	
					sprintf( filename , "%s/snapshot%08u.svg" , PhysiCell_settings.folder.c_str() , PhysiCell_globals.SVG_output_index ); 
					SVG_plot( filename , microenvironment, 0.0 , PhysiCell_globals.current_time, cell_coloring_function );
					
					PhysiCell_globals.SVG_output_index++; 
					PhysiCell_globals.next_SVG_save_time  += PhysiCell_settings.SVG_save_interval;
				}
			}


			// update the microenvironment
			microenvironment.simulate_diffusion_decay( diffusion_dt );
            
            
/*             double dt_intracellular = 1.0;
            //std::cout << "Current_Time : " << PhysiCell_globals.current_time << " -   FMOD : " << fmod(PhysiCell_globals.current_time,1.0) << std::endl;
            if ( fabs((fmod (PhysiCell_globals.current_time, dt_intracellular) - 1 )) < 0.001 || (fmod (PhysiCell_globals.current_time, dt_intracellular) < 0.00001) ) 
            {
                std::cout << "DIVIDED" <<std::endl;
            } */
			
			// run PhysiCell 
			((Cell_Container *)microenvironment.agent_container)->update_all_cells( PhysiCell_globals.current_time );
			
			//   Custom add-ons could potentially go here. 
            double time_since_last_intracellular = PhysiCell_globals.current_time - last_intracellular_time;
            if( PhysiCell_globals.current_time >= next_intracellular_update )
            {
			    update_intracellular();

                next_intracellular_update += intracellular_dt; 

                // save intracellular data if enabled and it's time
                if( PhysiCell_settings.enable_intracellular_saves == true )
                {
                    // if( fabs( PhysiCell_globals.current_time - PhysiCell_globals.next_intracellular_save_time  ) < 0.01 * intracellular_dt )
                    if( PhysiCell_globals.current_time >= PhysiCell_globals.next_intracellular_save_time ) 
                    {
                        // std::cout << "-- saving intracellular file at t= " << PhysiCell_globals.current_time << std::endl;

                        // Save output for intracellular data
                        sprintf( filename , "%s/intra%08u.dat" , PhysiCell_settings.folder.c_str() , PhysiCell_globals.intracellular_output_index ); 

                        std::ofstream ofile( filename , std::ios::out );
                        if( ofile.fail() )
                        { 
                            std::cout << std::endl << "Error: Failed to open " << filename << " for SVG writing." << std::endl << std::endl; 
                            std::cout << std::endl << "Error: We're not writing data like we expect. " << std::endl
                            << "Check to make sure your save directory exists. " << std::endl << std::endl;
                            exit(-1); 
                        } 
                        ofile << PhysiCell_globals.current_time << ", " << 42.3 << std::endl;   // dummy output
                        ofile.close();

                        PhysiCell_globals.intracellular_output_index++; 
                        PhysiCell_globals.next_intracellular_save_time += PhysiCell_settings.intracellular_save_interval;
                    }
                }
            }


			PhysiCell_globals.current_time += diffusion_dt;
		}
		
		if( PhysiCell_settings.enable_legacy_saves == true )
		{			
			log_output(PhysiCell_globals.current_time, PhysiCell_globals.full_output_index, microenvironment, report_file);
			report_file.close();
		}
	}
	catch( const std::exception& e )
	{ // reference to the base of a polymorphic object
		std::cout << e.what(); // information from length_error printed
	}
	
	// save a final simulation snapshot 
	
	sprintf( filename , "%s/final" , PhysiCell_settings.folder.c_str() ); 
	save_PhysiCell_to_MultiCellDS_xml_pugi( filename , microenvironment , PhysiCell_globals.current_time ); 
	
	sprintf( filename , "%s/final.svg" , PhysiCell_settings.folder.c_str() ); 
	SVG_plot( filename , microenvironment, 0.0 , PhysiCell_globals.current_time, cell_coloring_function );
	
	// timer 
	
	std::cout << std::endl << "Total simulation runtime: " << std::endl; 
	BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() ); 

	return 0; 
}
