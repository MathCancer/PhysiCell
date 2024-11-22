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
# Copyright (c) 2015-2024, Paul Macklin and the PhysiCell Project             #
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

#include "./PhysiCell_custom.h" 
#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cstring>

namespace PhysiCell
{
	
Variable::Variable()
{
	name = "unnamed"; 
	units = "dimensionless"; 
	value = 0.0; 
	conserved_quantity = false; 
	return; 
}

std::ostream& operator<<(std::ostream& os, const Variable& v)
{
	if (v.units.empty()){
		os << v.name << ": " << v.value << " no_dim";
	}
	else{
		os << v.name << ": " << v.value << " " << v.units;
	}
	 
	return os; 
}


Vector_Variable::Vector_Variable()
{
	name = "unnamed"; 
	units = "dimensionless"; 
	value.resize(3, 0.0 );
	conserved_quantity = false; 
	return; 
}

std::ostream& operator<<(std::ostream& os, const Vector_Variable& v)
{
 	os << v.name << ": ";
	if( v.value.size() == 0 )
	{ os << "[empty]"; return os; }
/*
	if( v.value.size() == 1 )
	{ os << v.value[0] << " (" << v.units << ")"; return os;  }
*/
	for( int i=0; i < v.value.size()-1 ; i++ )
	{ os << v.value[i] << ","; }
	os << v.value[v.value.size()-1] << " (" << v.units << ")"; 
	return os; 
}

	
Custom_Cell_Data::Custom_Cell_Data()
{
//	std::cout << __FUNCTION__ << "(default)" << std::endl; 
	variables.resize(0); 
	vector_variables.resize(0); 
	
	name_to_index_map.clear(); 
//	vector_name_to_index_map.clear();
	
	return;
}

Custom_Cell_Data::Custom_Cell_Data( const Custom_Cell_Data& ccd )
{
//	std::cout << __FUNCTION__ << "(copy)" << std::endl; 
	variables = ccd.variables; 
	vector_variables = ccd.vector_variables; 
	
	name_to_index_map= ccd.name_to_index_map; 
	
	return; 
}

int Custom_Cell_Data::add_variable( Variable& v )
{
	int n = variables.size(); 
	variables.push_back( v ); 
	name_to_index_map[ v.name ] = n; 
	return n; 
}

int Custom_Cell_Data::add_variable( std::string name , std::string units , double value )
{
	int n = variables.size(); 
	variables.resize( n+1 ); 
	variables[n].name = name; 
	variables[n].units = units; 
	variables[n].value = value; 
	name_to_index_map[ name ] = n; 
	return n; 
}

int Custom_Cell_Data::add_variable( std::string name , double value )
{
	int n = variables.size(); 
	variables.resize( n+1 ); 
	variables[n].name = name; 
	variables[n].units = "dimensionless"; 
	variables[n].value = value; 
	name_to_index_map[ name ] = n; 
	return n; 
}

int Custom_Cell_Data::add_vector_variable( Vector_Variable& v )
{
	int n = vector_variables.size(); 
	vector_variables.push_back( v ); 
//	vector_name_to_index_map[ v.name ] = n; 
	return n; 
}

int Custom_Cell_Data::add_vector_variable( std::string name , std::string units , std::vector<double>& value )
{
	int n = vector_variables.size(); 
	vector_variables.resize( n+1 ); 
	vector_variables[n].name = name; 
	vector_variables[n].units = units; 
	vector_variables[n].value = value; 
//	vector_name_to_index_map[ name ] = n; 
	return n; 
}

int Custom_Cell_Data::add_vector_variable( std::string name , std::vector<double>& value )
{
	int n = vector_variables.size(); 
	vector_variables.resize( n+1 ); 
	vector_variables[n].name = name; 
	vector_variables[n].units = "dimensionless"; 
	vector_variables[n].value = value; 
//	vector_name_to_index_map[ name ] = n; 
	return n; 
}

int Custom_Cell_Data::find_variable_index( std::string name )
{
	// this should return -1 if not found, not zero 
	auto out = name_to_index_map.find( name ); 
	if( out != name_to_index_map.end() )
	{ return out->second; }
	return -1; 
}

/*
int Custom_Cell_Data::find_vector_variable_index( std::string name )
{
	return vector_name_to_index_map[ name ]; 
}
*/

int Custom_Cell_Data::find_vector_variable_index( std::string name )
{
	int n = 0; 
	while( n < vector_variables.size() )
	{
		if( std::strcmp( vector_variables[n].name.c_str() , name.c_str() ) == 0 )
		{ return n; } 
		n++; 
	}
	
	return -1; 
}


double& Custom_Cell_Data::operator[](int i)
{
	return variables[i].value; 
}

double& Custom_Cell_Data::operator[]( std::string name )
{
	return variables[ name_to_index_map[name] ].value; 
}

//std::ostream& operator<<(std::ostream& os, const Custom_Cell_Data& ccd)
//{
//	os << "Custom data (scalar): " << std::endl; 
//	for( int i=0 ; i < ccd.variables.size() ; i++ )
//	{
//		os << i << ": " << ccd.variables[i] << std::endl; 
//	}
//
//	os << "Custom data (vector): " << std::endl; 
//	for( int i=0 ; i < ccd.vector_variables.size() ; i++ )
//	{
//		os << i << ": " << ccd.vector_variables[i] << std::endl; 
//	}
//	
//	return os;
//}

//Operators for variable class only is because os was already defined
std::istream& operator>>(std::istream& is, PhysiCell::Variable& v)
{
    // Read name, value, and units directly from the line
    is >> v.name >> v.value >> v.units;
    //std::cout << "ayooo " << v.name << std::endl;
    // Check if the input was successful
    if (is.fail()) 
    {
        std::cerr << v.name << " Error: Unable to read variable data." << std::endl;
    }

    return is;
}


//Operators for Vector_variable class only is because os was already defined
std::istream& operator>>(std::istream& is, PhysiCell::Vector_Variable& v)
{
    // Read name
    is >> v.name;

    // Read values until a non-numeric character is encountered
    double value;
    while (is >> value)
    {
        v.value.push_back(value);
    }

    // Clear the failbit if it was set due to encountering a non-numeric character
    is.clear();

    // Read units
    is >> v.units;

    return is;
}


//Operators for custom data class
std::ostream& operator<<(std::ostream& os, const Custom_Cell_Data& ccd)
{
    for (int i = 0; i < ccd.variables.size(); i++)
    {

        os << ccd.variables[i] << std::endl;
    }

    for (int i = 0; i < ccd.vector_variables.size(); i++)
    {
        os << ccd.vector_variables[i] << std::endl;
    }

	os << "End of custom data" << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, PhysiCell::Custom_Cell_Data& ccd)
{
    // Read scalar data until a line starts with a space

	std::string line;
    for (;;)
    {
        // Read the whole line
        std::getline(is, line);


        // Check if the first character is a space or the line is empty
        if (line.find("End of custom data") != std::string::npos)
        {
            break;
        }

        // Use a stringstream to extract values from the line
        std::istringstream line_stream(line);
        PhysiCell::Variable v;
        line_stream >> v;


        // Add the variable to ccd in this version it is saving the : in name. i have to remove it
		std::string correct_name = v.name.substr(0, v.name.size() - 1);

        int index = ccd.find_variable_index(correct_name);
		ccd[index] = v.value;
    }

    // Read vector data until a line starts with a space
    //std::cout << "Enter Custom data (vector): " << std::endl;
	/*
    for (;;)
    {
        // Read the whole line
        std::string line;
        std::getline(is, line);

        // Check if the first character is a space or the line is empty
        if (line.empty() || std::isspace(line[0]))
        {
            // Break the loop if the first character is a space
            break;
        }

        // Use a stringstream to extract values from the line
        std::istringstream line_stream(line);
        PhysiCell::Vector_Variable vv;
        line_stream >> vv;

        // Add the vector variable to ccd
        ccd.add_vector_variable(vv);
    }
*/
    return is;
}


};
