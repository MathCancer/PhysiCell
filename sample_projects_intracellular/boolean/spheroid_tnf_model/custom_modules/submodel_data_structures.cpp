/*
 * submodel_data_structures.cpp
 *
 *  Created on: ?
 *      Author: Paul Macklin
 *  
 * 	Description:
 * 		Auxiliary class to manage submodels
 * 
 * 	Source: https://github.com/pc4covid19/COVID19/blob/master/PhysiCell/custom_modules/submodel_data_structures.cpp
*/

#include "./submodel_data_structures.h" 

using namespace PhysiCell; 

Submodel_Registry submodel_registry; 

Submodel_Information::Submodel_Information( void )
{
	name = "none";
	version = "-1"; 
	main_function = NULL; 
	
	microenvironment_variables.resize(0); 
	cell_variables.resize(0); // custom data  

	return; 
}

void Submodel_Information::register_model( void )
{
	// make sure the cell defaults "know" about each custom variable 
	// Make sure it's there, or add if it's not. 
	
	for( int n = 0 ; n < cell_variables.size() ; n++ )
	{
		// let's do this a bit manually (and inefficiently), but safely. 
		bool found_it = false; 
		for( unsigned int i=0; i < cell_defaults.custom_data.variables.size() ; i++ )
		{
			if( cell_defaults.custom_data.variables[i].name == cell_variables[n] )
			{ found_it = true; }
		}
		if( found_it == false )
		{
			cell_defaults.custom_data.add_variable( cell_variables[n] , "none", 0.0 ); 
		}
		
	}
	
	// add the model to the registry of submodels 
	
	submodel_registry.register_model( *this ); 
}

void Submodel_Information::display( std::ostream& os )
{
	os << "Submodel: " << name << " (Version " << version << ")" << std::endl ;
	
	os << "\tcell variables: " << std::endl;
	for( int n = 0 ; n < cell_variables.size(); n++ )
	{
		os << "\t\t" << cell_variables[n] << std::endl; 
	}
	
	os << "\tfunction: " ; 
	if( main_function )
	{ os << (long long int) main_function; }
	else
	{ os << "NULL"; }
	os << std::endl; 
	
	return; 
}

void Submodel_Registry::register_model( Submodel_Information& model )
{
	#pragma omp critical 
	{
		// already registered? 
		bool found = false; 
		for( int n = 0; n < submodels.size() ; n++ )
		{
			if( submodels[n] == &model )
			{ found = true; } 
		} 
		
		if( found == false )
		{
			submodels.push_back( &model ); 
//			add_software_citation(); 
// void add_software_citation( std::string name , std::string version, std::string DOI , std::string URL )
		}
	}
	
	return;
}

void Submodel_Registry::display( std::ostream& os )
{
	os << "The following submodels are registered: " << std::endl; 
	os << "=======================================" << std::endl; 
	for( int n = 0 ; n < submodels.size(); n++ )
	{
		submodels[n]->display( os ); 
	}
	os << std::endl; 
	
	return;
}

	

