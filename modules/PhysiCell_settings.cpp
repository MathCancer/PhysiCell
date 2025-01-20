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
 
#include <sys/stat.h>
#include "./PhysiCell_settings.h"

using namespace BioFVM; 

namespace PhysiCell{
	
PhysiCell_Settings PhysiCell_settings; 
User_Parameters parameters; 

bool physicell_config_dom_initialized = false; 
pugi::xml_document physicell_config_doc; 	
pugi::xml_node physicell_config_root; 
	
bool read_PhysiCell_config_file( std::string filename )
{
	physicell_config_dom_initialized = false; 

	std::cout << "Using config file " << filename << " ... " << std::endl ; 
	pugi::xml_parse_result result = physicell_config_doc.load_file( filename.c_str() );
	
	if( result.status != pugi::xml_parse_status::status_ok )
	{
		std::cout << "Error loading " << filename << "!" << std::endl; 
		return false;
	}
	
	physicell_config_root = physicell_config_doc.child("PhysiCell_settings");
	physicell_config_dom_initialized = true; 
	return true;
}

bool load_PhysiCell_config_file( std::string filename )
{
	if (!read_PhysiCell_config_file( filename ))
	{ return false; }

	PhysiCell_settings.read_from_pugixml(); 
	
	// now read the microenvironment (optional) 
	
	if( !setup_microenvironment_from_XML( physicell_config_root ) )
	{
		std::cout << std::endl
			<< "Warning: microenvironment_setup not found in " << filename << std::endl
			<< "         Either manually setup microenvironment in setup_microenvironment() (custom.cpp)" << std::endl
			<< "         or consult documentation to add microenvironment_setup to your configuration file." << std::endl << std::endl;
	}
	
	// now read user parameters
	parameters.read_from_pugixml( physicell_config_root );

	create_output_directory( PhysiCell_settings.folder );

	return true; 	
}

PhysiCell_Settings::PhysiCell_Settings()
{
	// units 
	time_units = "min"; 
	space_units = "micron"; 
	
	// save options
	folder = "."; 
	max_time = 60*24*45;   

	full_save_interval = 60;  
	enable_full_saves = true; 
	enable_legacy_saves = false; 
	
	SVG_save_interval = 60; 
	enable_SVG_saves = true; 
	enable_substrate_plot = false;
	substrate_to_monitor = "oxygen"; 
	limits_substrate_plot = false;
	min_concentration = -1.0;
	max_concentration = -1.0;
	svg_substrate_colormap = "YlOrRd";

	intracellular_save_interval = 60;  
	enable_intracellular_saves = false; 
	
	// parallel options 
	
	omp_num_threads = 4; 

	rules_enabled = false; 
	rules_protocol = "Cell Behavior Hypothesis Grammar (CBHG)"; 
	rules_protocol_version = "1.0"; 
	 
	return; 
}
 	
void PhysiCell_Settings::read_from_pugixml( void )
{
	pugi::xml_node node; 
	
	// overall options 
	
	node = xml_find_node( physicell_config_root , "overall" );

	max_time = xml_get_double_value( node , "max_time" );
	time_units = xml_get_string_value( node, "time_units" ) ;
	space_units = xml_get_string_value( node, "space_units" ) ;
	
	// check to see if dt is specified in overall options
	// if so, set from XML 
	
	pugi::xml_node search_result;  
	search_result = xml_find_node( node , "dt_diffusion" ); 
	if( search_result )
	{ diffusion_dt = xml_get_my_double_value( search_result ); }

	search_result = xml_find_node( node , "dt_mechanics" ); 
	if( search_result )
	{ mechanics_dt = xml_get_my_double_value( search_result ); }

	search_result = xml_find_node( node , "dt_phenotype" ); 
	if( search_result )
	{ phenotype_dt = xml_get_my_double_value( search_result ); }

	search_result = xml_find_node( node , "dt_intracellular" ); 
	if( search_result )
	{ intracellular_dt = xml_get_my_double_value( search_result ); }
	
	node = node.parent(); 
	
	// save options 
	
	node = xml_find_node( physicell_config_root , "save" ); 
	
	folder = xml_get_string_value( node, "folder" ) ;
	
	node = xml_find_node( node , "full_data" ); 
	full_save_interval = xml_get_double_value( node , "interval" );
	enable_full_saves = xml_get_bool_value( node , "enable" ); 
	node = node.parent(); 
	
	node = xml_find_node( node , "SVG" ); 
	SVG_save_interval = xml_get_double_value( node , "interval" );
	enable_SVG_saves = xml_get_bool_value( node , "enable" ); 

	pugi::xml_node node_plot_substrate; 
	node_plot_substrate = xml_find_node( node , "plot_substrate" );
	enable_substrate_plot = node_plot_substrate.attribute("enabled").as_bool();
	limits_substrate_plot = node_plot_substrate.attribute("limits").as_bool();

	if(enable_substrate_plot){
		substrate_to_monitor = xml_get_string_value(node_plot_substrate, "substrate");
		if (limits_substrate_plot) {
			min_concentration = xml_get_double_value(node_plot_substrate, "min_conc");
			max_concentration = xml_get_double_value(node_plot_substrate, "max_conc");
		}
		pugi::xml_node colormap_node = xml_find_node( node_plot_substrate, "colormap");
		if (colormap_node)
		{
			svg_substrate_colormap = xml_get_my_string_value(colormap_node);
		}
	}

	node = node.parent(); 

	node = xml_find_node( node , "intracellular_data" ); 
	intracellular_save_interval = xml_get_double_value( node , "interval" );
	enable_intracellular_saves = xml_get_bool_value( node , "enable" ); 
	node = node.parent(); 
	
	node = xml_find_node( node , "legacy_data" ); 
	enable_legacy_saves = xml_get_bool_value( node , "enable" );
	node = node.parent(); 

	// parallel options 

	node = xml_find_node( physicell_config_root , "parallel" ); 		
	omp_num_threads = xml_get_int_value( node, "omp_num_threads" ); 
	
	node = node.parent(); 
	
	// legacy and other options 
	
	pugi::xml_node node_options; 
	
	node_options = xml_find_node( physicell_config_root , "options" );
	if (node_options)
	{
		bool settings;

		// look for legacy_random_points_on_sphere_in_divide
		settings = xml_get_bool_value(node_options, "legacy_random_points_on_sphere_in_divide");
		if (settings)
		{
			std::cout << "setting legacy unif" << std::endl;
			extern std::vector<double> (*cell_division_orientation)(void);
			cell_division_orientation = LegacyRandomOnUnitSphere;
		}

		settings = xml_get_bool_value(node_options, "disable_automated_spring_adhesions");
		if (settings)
		{
			std::cout << "Disabling automated spring adhesions and detachments!" << std::endl;
			PhysiCell_settings.disable_automated_spring_adhesions = true;
		}

		pugi::xml_node random_seed_node = xml_find_node(node_options, "random_seed");
		std::string random_seed = ""; // default is system clock, even if this element is not present
		if (random_seed_node)
		{ random_seed = xml_get_my_string_value(random_seed_node); }

		if (random_seed == "" || random_seed == "random" || random_seed == "system_clock")
		{
			std::cout << "Using system clock for random seed" << std::endl;
			SeedRandom();
		}
		else
		{
			unsigned int seed;
			try
			{ seed = std::stoul(random_seed); }
			catch(const std::exception& e)
			{
				std::cout << "ERROR: " << random_seed << " is not a valid random seed. It must be an integer. Fix this within <options>." << std::endl;
				exit(-1);
			}
			SeedRandom(seed);
		}

		// other options can go here, eventually
	}

	// domain options 
	
	node = xml_find_node( physicell_config_root , "domain" );

	double xmin = xml_get_double_value( node , "x_min" );
	double xmax = xml_get_double_value( node , "x_max" );
	double ymin = xml_get_double_value( node , "y_min" );
	double ymax = xml_get_double_value( node , "y_max" );
	double zmin = xml_get_double_value( node , "z_min" );
	double zmax = xml_get_double_value( node , "z_max" );
	double dx = xml_get_double_value( node, "dx" ); 
	double dy = xml_get_double_value( node, "dy" ); 
	double dz = xml_get_double_value( node, "dz" ); 
	
	default_microenvironment_options.simulate_2D = xml_get_bool_value( node, "use_2D" ); 

	if( default_microenvironment_options.simulate_2D == true )
	{
		zmin = -0.5 * dz; 
		zmax = 0.5 * dz; 
	}			
	default_microenvironment_options.X_range = {xmin, xmax}; 
	default_microenvironment_options.Y_range = {ymin, ymax}; 
	default_microenvironment_options.Z_range = {zmin, zmax}; 

	default_microenvironment_options.dx = dx; 
	default_microenvironment_options.dy = dy; 
	default_microenvironment_options.dz = dz; 		

	node = node.parent(); 

	// random seed options 
	
	return; 
}

bool create_directories(const std::string &path)
{
    size_t pos = 0;
    std::string currentPath;

	// Check for Unix-like absolute path or Windows absolute path with drive letter
	if (path[0] == '\\' || path[0] == '/')
	{
		pos = 1; // Unix-like or Windows absolute path starting with backslash or forward slash
	}
	else if (path.length() > 2 && isalpha(path[0]) && path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
	{
		pos = 3; // Windows absolute path with drive letter
	}

	while ((pos = path.find_first_of("/\\", pos)) != std::string::npos) {
        currentPath = path.substr(0, pos++);
        if (!create_directory(currentPath)) {
            return false;
        }
    }
    return create_directory(path);
}

bool create_directory(const std::string &path)
{
#if defined(_WIN32)
	bool success = mkdir(path.c_str()) == 0;
#else
	bool success = mkdir(path.c_str(), 0755) == 0;
#endif
	return success || errno == EEXIST;
}

void create_output_directory(const std::string& path)
{
	if (!create_directories(path))
	{
		std::cout << "ERROR: Could not create output directory " << path << " ! Quitting." << std::endl;
		exit(-1);
	}
}

void create_output_directory(void)
{
	create_output_directory(PhysiCell_settings.folder);
}

PhysiCell_Globals PhysiCell_globals; 

/* parameters functions */ 

template <class T>
Parameter<T>::Parameter()
{
	name = "unnamed"; 
	units= "none"; 
/*	
	T* pT; 
	pT = new T; 
	value = *pT; 
*/
	value = (T) 0;
//	value = 1-1; 
	return; 
}

template <> Parameter<std::string>::Parameter()
{
	name = "unnamed"; 
	units= "none"; 
	value = "none";
	return; 
}

template <class T>
Parameter<T>::Parameter( std::string my_name )
{
	name = my_name; 
	units = "dimensionless"; 
/*
	T* pT; 
	pT = new T; 
	value = *pT; 
*/
	value = (T) 0; 
	return; 
}

template <> Parameter<std::string>::Parameter( std::string my_name )
{
	name = my_name;  
	units= "none"; 
	value = "none";
	return; 
}

template <class T>
void Parameter<T>::operator=( T& rhs )
{
	value = rhs; 
	return; 
} 

template <class T>
void Parameter<T>::operator=( T rhs )
{
	value = rhs; 
	return; 
} 

template <class T>
void Parameter<T>::operator=( Parameter& p )
{
	name = p.name; 
	units = p.units; 
	value = p.value; 
	return; 
} 

template <class T>
std::ostream& operator<<(std::ostream& os, const Parameter<T>& param)
{
	os << param.name << ": " << param.value << " [" << param.units << "]"; 
	return os; 
}

template <class T>
int Parameters<T>::size( void ) const
{ return parameters.size(); } 

template <class T>
T& Parameters<T>::operator()( int i )
{
	return parameters[i].value; 
}

template <class T>
T& Parameters<T>::operator()( std::string str )
{
	if (name_to_index_map.find(str) == name_to_index_map.end())
	{
		std::cerr << "ERROR : Unknown parameter " << str << " ! Quitting." << std::endl;
		exit(-1);
	}
	return parameters[ name_to_index_map[str] ].value; 
}

template <class T>
Parameter<T>& Parameters<T>::operator[]( int i )
{
	return parameters[i]; 
}

template <class T>
Parameter<T>& Parameters<T>::operator[]( std::string str )
{
	if (name_to_index_map.find(str) == name_to_index_map.end())
	{
		std::cerr << "ERROR : Unknown parameter " << str << " ! Quitting." << std::endl;
		exit(-1);
	}
	return parameters[ name_to_index_map[str] ]; 
}


template <class T>
int Parameters<T>::find_index( std::string search_name )
{
	auto out = name_to_index_map.find( search_name ); 
	if( out != name_to_index_map.end() )
	{ return out->second; }
	return -1; 
	// return name_to_index_map[ search_name ]; 
}


template <class T>
std::ostream& operator<<( std::ostream& os , const Parameters<T>& params )
{
	for( int i=0; i < params.parameters.size() ; i++ )
	{
		os <<  params.parameters[i] << std::endl; 
	}
	return os; 
}

template <class T>
Parameters<T>::Parameters()
{
	parameters.resize( 0 ); 
	name_to_index_map.clear(); 
	return; 
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name )
{
	// this function is not currently (2024-06-03) called in the code, so these defaults largely do not matter; very unlikely others are directly calling this function, let alone this implementation
	T my_value = T(); // for {int, double, bool, string} this will be {0, 0.0, false, ""} (this would technically change the behavior for strings since it is hardcoded above to default to "none", but nobody should rely on the default value of a string being "none")
	return add_parameter( my_name , my_value );
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name , T my_value )
{
	// this function is not currently (2024-06-03) called in the code, so these defaults largely do not matter; very unlikely others are directly calling this function, let alone this implementation
	std::string my_units = "dimensionless"; // technically this would change the behavior for strings since it is hardcoded above to default to "none", but nobody should be using units on strings; also, if the xml does not have units, then "dimensionless" is used even for strings
	return add_parameter( my_name , my_value , my_units );
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name , T my_value , std::string my_units )
{
	assert_not_exists(my_name);

	Parameter<T>* pNew; 
	pNew = new Parameter<T> ;
	pNew->name = my_name ; 
	pNew->value = my_value; 
	pNew->units = my_units; 
	
	int n = parameters.size(); 
	
	parameters.push_back( *pNew ); 
	
	name_to_index_map[ my_name ] = n; 
	return; 
}

template <class T>
void Parameters<T>::add_parameter( Parameter<T> param )
{
	assert_not_exists(param.name);

	int n = parameters.size(); 
	parameters.push_back( param); 
	name_to_index_map[ param.name ] = n; 
	return; 
}

template <class T>
void Parameters<T>::assert_not_exists( std::string search_name )
{
	if( find_index( search_name ) == -1 )
	{ return; }

	std::cout << "ERROR: Parameter " << search_name << " already exists. Make sure all parameters (of a given type) have unique names." << std::endl;
	exit(-1);
}

std::ostream& operator<<( std::ostream& os , const User_Parameters up )
{
	os << "Bool parameters:: " << std::endl << up.bools << std::endl; 
	os << "Int parameters:: " << std::endl << up.ints << std::endl; 
	os << "Double parameters:: " << std::endl << up.doubles << std::endl; 
	os << "String parameters:: " << std::endl << up.strings << std::endl; 
	return os; 
}

void User_Parameters::read_from_pugixml( pugi::xml_node parent_node )
{
	pugi::xml_node node = xml_find_node( parent_node , "user_parameters" ); 
	
	pugi::xml_node node1 = node.first_child(); 
	int i = 0; 
	while( node1 )
	{
		std::string name = xml_get_my_name( node1 );
		std::string units = node1.attribute( "units" ).value(); 
		if( units == "" )
		{ units = "dimensionless"; } 
		
		std::string type = node1.attribute( "type" ).value();

		if (type == "bool")
		{
			bool value = xml_get_my_bool_value(node1);
			bools.add_parameter(name, value, units);
		}
		else if (type == "int")
		{
			int value = xml_get_my_int_value(node1);
			ints.add_parameter(name, value, units);
		}
		else if (type == "double")
		{
			double value = xml_get_my_double_value(node1);
			doubles.add_parameter(name, value, units);
		}
		else if (type == "string")
		{
			std::string value = xml_get_my_string_value(node1);
			strings.add_parameter(name, value, units);
		}
		else // default if no type specified
		{
			double value = xml_get_my_double_value(node1);
			doubles.add_parameter(name, value, units);
		}

		node1 = node1.next_sibling(); 
		i++; 
	}
	
	std::cout << "User parameters in XML config file: " << std::endl; 
	std::cout << *this << std::endl; 
	
	return; 
}

// need this so that the template gets filled and compiled prior to linking 
template class Parameter<bool>;
template class Parameter<int>;
template class Parameter<double>;
template class Parameter<std::string>;
 
template class Parameters<bool>;
template class Parameters<int>;
template class Parameters<double>;
template class Parameters<std::string>;


template std::ostream& operator<<(std::ostream& os, const Parameter<bool>& param);
template std::ostream& operator<<(std::ostream& os, const Parameter<int>& param);
template std::ostream& operator<<(std::ostream& os, const Parameter<double>& param);
template std::ostream& operator<<(std::ostream& os, const Parameter<std::string>& param);

bool setup_microenvironment_from_XML( pugi::xml_node root_node )
{
	pugi::xml_node node; 

	// First, look for the correct XML node. 
	// If it isn't there, return false. 
	
	node = xml_find_node( root_node , "microenvironment_setup" );
	if( !node )
	{ return false; }

	// now that we're using the XML to specify the microenvironment, don't 
	// use old defaults 
	
	// Don't let BioFVM use oxygen as the default 
	
	default_microenvironment_options.use_oxygen_as_first_field = false; 

	std::vector<double> initial_condition_vector = {}; 
	std::vector<double> Dirichlet_condition_vector = {}; 
	std::vector<bool> Dirichlet_activation_vector = {}; 

	std::vector<bool> Dirichlet_all = {}; 
	std::vector<bool> Dirichlet_xmin = {}; 
	std::vector<bool> Dirichlet_xmax = {}; 
	std::vector<bool> Dirichlet_ymin = {}; 
	std::vector<bool> Dirichlet_ymax = {}; 
	std::vector<bool> Dirichlet_zmin = {}; 
	std::vector<bool> Dirichlet_zmax = {}; 

	std::vector<double> Dirichlet_xmin_values = {}; 
	std::vector<double> Dirichlet_xmax_values = {}; 
	std::vector<double> Dirichlet_ymin_values = {}; 
	std::vector<double> Dirichlet_ymax_values = {}; 
	std::vector<double> Dirichlet_zmin_values = {}; 
	std::vector<double> Dirichlet_zmax_values = {}; 
	std::vector<double> Dirichlet_interior_values = {}; 


	// next, add all the substrates to the microenvironment
	// build the initial conditions and Dirichlet conditions as we go 

	// find the first substrate 
	pugi::xml_node node1 = node.child( "variable" ); // xml_find_node( node , "variable" ); 
	node = node1; 
	int i = 0; 
	
	bool activated_Dirichlet_boundary_detected = false; 
	
	while( node )
	{
		// get the name and units 
		std::string name = node.attribute( "name" ).value(); 
		std::string units = node.attribute( "units" ).value(); 
		
		// add the substrate 
		if( i == 0 )
		{ microenvironment.set_density( 0, name, units ); }
		else
		{ microenvironment.add_density( name, units ); }
		
		// get the diffusion and decay parameters 
		node1 = xml_find_node( node, "physical_parameter_set" ); 
		
		microenvironment.diffusion_coefficients[i] = 
			xml_get_double_value( node1, "diffusion_coefficient" ); 
		microenvironment.decay_rates[i] = 
			xml_get_double_value( node1, "decay_rate" ); 
			
		// now, get the initial value  
		node1 = xml_find_node( node, "initial_condition" ); 
		initial_condition_vector.push_back( xml_get_my_double_value(node1) );
		
		// now, get the Dirichlet value
		node1 = xml_find_node( node, "Dirichlet_boundary_condition" ); 
		Dirichlet_condition_vector.push_back( xml_get_my_double_value(node1) );

		// now, decide whether or not to enable it 
		Dirichlet_activation_vector.push_back( node1.attribute("enabled").as_bool() );

		Dirichlet_all.push_back( Dirichlet_activation_vector[i] ); 
		if( Dirichlet_activation_vector[i] )
		{ activated_Dirichlet_boundary_detected = true; }
		
		// default interior activation will mirror the boundary 
		
		Dirichlet_xmin.push_back( Dirichlet_activation_vector[i] ); 
		Dirichlet_xmax.push_back( Dirichlet_activation_vector[i] ); 
		Dirichlet_ymin.push_back( Dirichlet_activation_vector[i] ); 
		Dirichlet_ymax.push_back( Dirichlet_activation_vector[i] ); 
		Dirichlet_zmin.push_back( Dirichlet_activation_vector[i] ); 
		Dirichlet_zmax.push_back( Dirichlet_activation_vector[i] ); 
		
		Dirichlet_xmin_values.push_back( Dirichlet_condition_vector[i] ); 
		Dirichlet_xmax_values.push_back( Dirichlet_condition_vector[i] ); 
		Dirichlet_ymin_values.push_back( Dirichlet_condition_vector[i] ); 
		Dirichlet_ymax_values.push_back( Dirichlet_condition_vector[i] ); 
		Dirichlet_zmin_values.push_back( Dirichlet_condition_vector[i] ); 
		Dirichlet_zmax_values.push_back( Dirichlet_condition_vector[i] ); 
		
		// now figure out finer-grained controls 
		
		node1 = node.child( "Dirichlet_options" );
		if( node1 )
		{
			// xmin, xmax, ymin, ymax, zmin, zmax, interior 
			pugi::xml_node node2 = node1.child("boundary_value"); 
			
			while( node2 )
			{
				// which boundary? 
				std::string boundary_ID = node2.attribute("ID").value(); 
				
				// xmin 
				if( std::strstr( boundary_ID.c_str() , "xmin" ) )
				{
					// on or off 
					Dirichlet_xmin[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }
					
					// which value 
					{ Dirichlet_xmin_values[i] = xml_get_my_double_value( node2 ); }
				}
				
				// xmax 
				if( std::strstr( boundary_ID.c_str() , "xmax" ) )
				{
					// on or off 
					Dirichlet_xmax[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }
				
					// which value 
					{ Dirichlet_xmax_values[i] = xml_get_my_double_value( node2 ); }
				}
				
				// ymin 
				if( std::strstr( boundary_ID.c_str() , "ymin" ) )
				{
					// on or off 
					Dirichlet_ymin[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }
				
					// which value 
					{ Dirichlet_ymin_values[i] = xml_get_my_double_value( node2 ); }
				}
				
				// ymax 
				if( std::strstr( boundary_ID.c_str() , "ymax" ) )
				{
					// on or off 
					Dirichlet_ymax[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }					
					
					// which value 
					{ Dirichlet_ymax_values[i] = xml_get_my_double_value( node2 ); }
				}				
								
				// zmin 
				if( std::strstr( boundary_ID.c_str() , "zmin" ) )
				{
					// on or off 
					Dirichlet_zmin[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }
				
					// which value 
					{ Dirichlet_zmin_values[i] = xml_get_my_double_value( node2 ); }
				}
				
				// zmax 
				if( std::strstr( boundary_ID.c_str() , "zmax" ) )
				{
					// on or off 
					Dirichlet_zmax[i] = node2.attribute("enabled").as_bool();
					// if there is at least one off bondary here, "all" is false for this substrate 
					if( node2.attribute("enabled").as_bool() == false )
					{ Dirichlet_all[i] = false; }
				
					// which value 
					{ Dirichlet_zmax_values[i] = xml_get_my_double_value( node2 ); }
				}
				
				node2 = node2.next_sibling("boundary_value"); 
			}
		}
		
		// now, figure out if individual boundaries are set 
/*		
		if( node1.attribute("boundaries") )
		{
			std::string option_string = node1.attribute("boundaries").value(); 
			Dirichlet_all.push_back(false); 

			if( strstr( option_string.c_str() , "xmin" ) )
			{ Dirichlet_xmin.push_back( true ); }
			else
			{ Dirichlet_xmin.push_back( false ); }
		
			if( strstr( option_string.c_str() , "xmax" ) )
			{ Dirichlet_xmax.push_back( true ); }
			else
			{ Dirichlet_xmax.push_back( false ); }
		
			if( strstr( option_string.c_str() , "ymin" ) )
			{ Dirichlet_ymin.push_back( true ); }
			else
			{ Dirichlet_ymin.push_back( false ); }
		
			if( strstr( option_string.c_str() , "ymax" ) )
			{ Dirichlet_ymax.push_back( true ); }
			else
			{ Dirichlet_ymax.push_back( false ); }
		
			if( strstr( option_string.c_str() , "zmin" ) )
			{ Dirichlet_zmin.push_back( true ); }
			else
			{ Dirichlet_zmin.push_back( false ); }

			if( strstr( option_string.c_str() , "zmax" ) )
			{ Dirichlet_zmax.push_back( true ); }
			else
			{ Dirichlet_zmax.push_back( false ); }
		}
		else
		{	
			Dirichlet_all.push_back(true); 
		}
*/		
		
		// move on to the next variable (if any!)
		node = node.next_sibling( "variable" ); 
		i++; 
	}

	// now that all the variables and boundary / initial conditions are defined, 
	// make sure that BioFVM knows about them 

	default_microenvironment_options.Dirichlet_condition_vector = Dirichlet_condition_vector;  
	default_microenvironment_options.Dirichlet_activation_vector = Dirichlet_activation_vector;
	default_microenvironment_options.initial_condition_vector = initial_condition_vector; 

	default_microenvironment_options.Dirichlet_all = Dirichlet_all; 
	
	default_microenvironment_options.Dirichlet_xmin = Dirichlet_xmin; 
	default_microenvironment_options.Dirichlet_xmax = Dirichlet_xmax; 
	default_microenvironment_options.Dirichlet_ymin = Dirichlet_ymin; 
	default_microenvironment_options.Dirichlet_ymax = Dirichlet_ymax; 
	default_microenvironment_options.Dirichlet_zmin = Dirichlet_zmin; 
	default_microenvironment_options.Dirichlet_zmax = Dirichlet_zmax; 
	
	default_microenvironment_options.Dirichlet_xmin_values = Dirichlet_xmin_values; 
	default_microenvironment_options.Dirichlet_xmax_values = Dirichlet_xmax_values; 
	default_microenvironment_options.Dirichlet_ymin_values = Dirichlet_ymin_values; 
	default_microenvironment_options.Dirichlet_ymax_values = Dirichlet_ymax_values; 
	default_microenvironment_options.Dirichlet_zmin_values = Dirichlet_zmin_values; 
	default_microenvironment_options.Dirichlet_zmax_values = Dirichlet_zmax_values; 

	// because outer boundary Dirichlet conditions are defined in the XML, 
	// make sure we don't accidentally disable them 
	
	default_microenvironment_options.outer_Dirichlet_conditions = false;
	
	// if *any* of the substrates have outer Dirichlet conditions enables, 
	// then set teh outer_Dirichlet_conditions = true; 
	
	if( activated_Dirichlet_boundary_detected ) 
	{
		default_microenvironment_options.outer_Dirichlet_conditions = true;
	}
	
	std::cout << activated_Dirichlet_boundary_detected << std::endl; 
	std::cout << "dc? " << default_microenvironment_options.outer_Dirichlet_conditions << std::endl; 
	
	// now, get the options 
	node = xml_find_node( root_node , "microenvironment_setup" );
	node = xml_find_node( node , "options" ); 
	
	// calculate gradients? 
	default_microenvironment_options.calculate_gradients = xml_get_bool_value( node, "calculate_gradients" ); 
	
	// track internalized substrates in each agent? 
	default_microenvironment_options.track_internalized_substrates_in_each_agent 
		= xml_get_bool_value( node, "track_internalized_substrates_in_each_agent" );

	node = xml_find_node(node, "initial_condition");
	if (node)
	{
		default_microenvironment_options.initial_condition_from_file_enabled = node.attribute("enabled").as_bool();
		if (default_microenvironment_options.initial_condition_from_file_enabled)
		{
			default_microenvironment_options.initial_condition_file_type = node.attribute("type").as_string();
			default_microenvironment_options.initial_condition_file = xml_get_string_value(node, "filename");

			copy_file_to_output(default_microenvironment_options.initial_condition_file);
		}
	}

	// not yet supported : read initial conditions 
	/*
	// read in initial conditions from an external file 
			<!-- not yet supported --> 
			<initial_condition type="matlab" enabled="false">
				<filename>./config/initial.mat</filename>
			</initial_condition>
	*/
	
	// not yet supported : read Dirichlet nodes (including boundary)
	/*
	// Read in Dirichlet nodes from an external file.
	// Note that if they are defined this way, then 
	// set 	default_microenvironment_options.outer_Dirichlet_conditions = false;
	// so that the microenvironment initialization in BioFVM does not 
	// also add Dirichlet nodes at the outer boundary

			<!-- not yet supported --> 
			<dirichlet_nodes type="matlab" enabled="false">
				<filename>./config/dirichlet.mat</filename>
			</dirichlet_nodes>
	*/	
	
	return true;  
}

bool setup_microenvironment_from_XML( void )
{ return setup_microenvironment_from_XML( physicell_config_root ); }

}; 
