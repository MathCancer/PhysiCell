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

#include "../core/PhysiCell.h"
#include "./PhysiCell_various_outputs.h"

#include "./PhysiCell_settings.h"

namespace PhysiCell{

int writePov(std::vector<Cell*> all_cells, double timepoint, double scale)
{
	static int TUMOR_TYPE=0; 
	static int VESSEL_TYPE=1; 
	
	std::string filename; 
	filename.resize( 1024 ); 
//	sprintf( (char*) filename.c_str() , "output//cells_%i.pov" , (int)round(timepoint) ); 
	sprintf( (char*) filename.c_str() , "%s/cells_%i.pov" , PhysiCell_settings.folder.c_str() ,  (int)round(timepoint) ); 
	std::ofstream povFile (filename.c_str(), std::ofstream::out);
	povFile<<"#include \"colors.inc\" \n";
	povFile<<"#include \"header.inc\" \n";
	
	for(int i=0;i<all_cells.size();i++)
	{
		std::string _nameCore;

		if( all_cells[i]->phenotype.cycle.pCycle_Model )
		{
			int code= all_cells[i]->phenotype.cycle.current_phase().code;
			if (code ==PhysiCell_constants::Ki67_positive_premitotic || code==PhysiCell_constants::Ki67_positive_postmitotic || code==PhysiCell_constants::Ki67_positive || code==PhysiCell_constants::Ki67_negative || code==PhysiCell_constants::live)
				_nameCore="LIVE";
			else if (code==PhysiCell_constants::apoptotic)
				_nameCore="APOP";
			else if (code==PhysiCell_constants::necrotic_swelling || code==PhysiCell_constants::necrotic_lysed || code==PhysiCell_constants::necrotic)
				_nameCore="NEC";
			else if (code==PhysiCell_constants::debris)
				_nameCore="DEBR";
			else
				_nameCore="MISC";
		}
		else if(all_cells[i]->type==TUMOR_TYPE)
			_nameCore="LIVE";
		else if(all_cells[i]->type==VESSEL_TYPE)
			_nameCore="ENDO";
		else
			_nameCore="MISC";
		std::string center= "<" + std::to_string(all_cells[i]->position[0]/scale) + "," + std::to_string(all_cells[i]->position[1]/scale) +","+ std::to_string(all_cells[i]->position[2]/scale) +">";
		std::string core = "sphere {\n\t" + center + "\n\t " + std::to_string( all_cells[i]->phenotype.geometry.radius/scale) + "\n\t FinishMacro ( " + center +","+ _nameCore+ "Finish,"+ _nameCore + "*1)\n}\n";
		povFile<< core;		
	}
	
	povFile<<"#include \"footer.inc\" \n";
	povFile.close();
	return 0;
}

int writeCellReport(std::vector<Cell*> all_cells, double timepoint)
{
	std::string filename; 
	filename.resize( 1024 ); 
//	sprintf( (char*) filename.c_str() , "output//cells_%i.txt" , (int)round(timepoint) ); 
	sprintf( (char*) filename.c_str() , "%s/cells_%i.txt" , PhysiCell_settings.folder.c_str() , (int)round(timepoint) ); 
	std::ofstream povFile (filename.c_str(), std::ofstream::out);
	povFile<<"\tID\tx\ty\tz\tradius\tvolume_total\tvolume_nuclear_fluid\tvolume_nuclear_solid\tvolume_cytoplasmic_fluid\tvolume_cytoplasmic_solid\tvolume_calcified_fraction\tphenotype\telapsed_time\n";
	int phenotype_code;
	for(int i=0;i<all_cells.size();i++)
	{
		phenotype_code = all_cells[i]->phenotype.cycle.current_phase().code;
		// phenotype_code = phases.size()>0?all_cells[i]->phenotype.cycle.phases[all_cells[i]->phenotype.current_phase_index].code:-1;
		povFile<<i<<"\t"<<all_cells[i]->ID<<"\t"<<all_cells[i]->position[0]<<"\t" << all_cells[i]->position[1] <<"\t"<< all_cells[i]->position[2]<<"\t";
		povFile<<all_cells[i]->phenotype.geometry.radius<<"\t"<<all_cells[i]->phenotype.volume.total<<"\t"<<all_cells[i]->phenotype.volume.nuclear_fluid
		<<"\t"<<all_cells[i]->phenotype.volume.nuclear_solid<<"\t"<<all_cells[i]->phenotype.volume.cytoplasmic_fluid<<"\t"<<
		all_cells[i]->phenotype.volume.cytoplasmic_solid<<"\t"<<all_cells[i]->phenotype.volume.calcified_fraction<<"\t"<<phenotype_code<< 
		// "\t"<< all_cells[i]->phenotype.cycle.phases[all_cells[i]->phenotype.current_phase_index].elapsed_time <<std::endl;		
		"\t"<< all_cells[i]->phenotype.cycle.data.elapsed_time_in_phase <<std::endl;		
	}
	povFile.close();
	return 0;
}

void display_simulation_status( std::ostream& os )
{
	os << "current simulated time: " << PhysiCell_globals.current_time << " " << 
		PhysiCell_settings.time_units << " (max: " << 
		PhysiCell_settings.max_time << " " << 
		PhysiCell_settings.time_units << ")" << std::endl; 
		
	os << "total agents: " << all_cells->size() << std::endl; 
	
	os << "interval wall time: ";
	BioFVM::TOC();
	BioFVM::display_stopwatch_value( os , BioFVM::stopwatch_value() ); 
	os << std::endl; 
	BioFVM::TIC(); 
	
	os << "total wall time: "; 
	BioFVM::RUNTIME_TOC();
	BioFVM::display_stopwatch_value( os , BioFVM::runtime_stopwatch_value() ); 
	os << std::endl << std::endl; 
	
	return;
}

void log_output(double t, int output_index, Microenvironment microenvironment, std::ofstream& report_file)
{
	double scale=1000;
	int num_new_cells= 0;
	int num_deaths=0;
//	std::cout << "current simulated time: " << t   << " minutes " << std::endl; 
//	std::cout << "interval wall time: ";
//	BioFVM::TOC();
//	BioFVM::display_stopwatch_value( std::cout , BioFVM::stopwatch_value() ); 
//	std::cout << std::endl; 
//	std::cout << "total wall time: "; 
//	BioFVM::RUNTIME_TOC();
//	BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() ); 
//	std::cout << std::endl;
	
	std::cout << "time: "<<t<<std::endl;
	num_new_cells=t==0?all_basic_agents.size():((Cell_Container *)microenvironment.agent_container)->num_divisions_in_current_step;
	num_deaths=((Cell_Container *)microenvironment.agent_container)->num_deaths_in_current_step;
	std::cout<<"total number of agents (newly born, deaths): " << (*all_cells).size()<<"("<<num_new_cells<<", "<<num_deaths<<")" << std::endl; 
	report_file<<t<<"\t"<<(*all_cells).size()<<"\t"<<num_new_cells<<"\t"<<num_deaths<<"\t"<<BioFVM::stopwatch_value()<< std::endl; 
//	BioFVM::TIC();
	
	((Cell_Container *)microenvironment.agent_container)->num_divisions_in_current_step=0;
	((Cell_Container *)microenvironment.agent_container)->num_deaths_in_current_step=0;
	writePov(*all_cells, t, scale);
	writeCellReport(*all_cells, t);
	std::string filename; 
	filename.resize( 1024 , '\0' ); 
	sprintf( (char*) filename.c_str() , "output%08d.mat" , output_index ); 
	filename.resize( strlen( filename.c_str() ) ); 
	// std::cout << "\tWriting to file " << filename << " ... " << std::endl; 
	// microenvironment.write_to_matlab( filename ); 
	
	return;
}

	
};
