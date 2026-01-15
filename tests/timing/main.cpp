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

#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include "PhysiCell_standard_models.h" 
#include "PhysiCell_cell.h" 

//using namespace PhysiCell;   // bad practice

static PhysiCell::Cell_Definition mycell; 

int ncells = 1000000;

int time_custom_vars1()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    std::cout << "--------------  " << __FUNCTION__ << " -------------- " << std::endl;
    double mechanics_voxel_size = 30; 
    PhysiCell::Cell_Container* cell_container = PhysiCell::create_cell_container_for_microenvironment( microenvironment, mechanics_voxel_size );

    PhysiCell::cell_defaults.custom_data.add_variable( "myvar0" , "dimensionless", 42.0); 
    PhysiCell::cell_defaults.custom_data.add_variable( "myvar1" , "dimensionless", 42.1); 
    PhysiCell::cell_defaults.custom_data.add_variable( "myvar2" , "dimensionless", 42.2); 

    mycell = PhysiCell::cell_defaults;
    // std::cout << "-- retrieving: " << std::endl;
    // name = "myvar1";    // reminder that retrieving an undefined var value will return the 0th defined.
    // std::cout << name << " = " << mycell.custom_data[name] << std::endl;

    PhysiCell::Cell* pCell = NULL; 
    double x,y;
    double sum = 0;
    std::cout << "ncells = " << ncells << std::endl;
    auto start = std::chrono::steady_clock::now();

    for (int idx=0; idx<ncells; idx++)
    {
        // std::cout << "idx=" << idx << std::endl;
        pCell = PhysiCell::create_cell(); 
        x = 1000*dis(gen);  // we don't need to create x,y for this test, but do
        y = 1000*dis(gen);
        pCell->assign_position( x, y, 0.0 );
        // std::cout << x << y << std::endl;
        // std::cout << pCell->custom_data["myvar0"] << std::endl;
        // std::cout << pCell->custom_data["myvar1"] << std::endl;
        // std::cout << pCell->custom_data["myvar2"] << std::endl;
        sum += (pCell->custom_data["myvar0"] + pCell->custom_data["myvar1"]) / pCell->custom_data["myvar2"] ; // = ~2
        // std::cout << pCell->custom_data["notfound"] << std::endl;
    }
    std::cout << "sum = " << sum << std::endl;  // ~ 2*ncells
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds : " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    // This will throw an error if we check for a valid var name in the "[]" operator:
    // Invalid custom variable: notfound
    std::cout << "pCell->custom_data['notfound'] = " << pCell->custom_data["notfound"] << std::endl;

    return 1;
}

int main()
{
    std::cout << ">>>>>>>>>  Timing tests" << std::endl;
    time_custom_vars1();

    return 1;
}
