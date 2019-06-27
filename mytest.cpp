#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <string> 

#include <map> 
#include <unordered_map>

std::unordered_map< std::string , int > name_to_int; 

int find_variable_index( std::string find_me );
int safe_find_variable_index( std::string find_me );

int main( int argc, char* argv[] )
{
	name_to_int[ "two" ] = 2; 
	name_to_int[ "one" ] = 1; 
	name_to_int[ "zero" ] = 0; 
	
	std::unordered_map<std::string,int>::iterator it = name_to_int.begin(); 
	while( it != name_to_int.end() )
	{
		std::cout << it->first << " : " << it->second << std::endl; 
		
		it++; 
	}
	
	std::cout << safe_find_variable_index( "Two" ) << std::endl; 
	std::cout << safe_find_variable_index( "two" ) << std::endl; 
	std::cout << safe_find_variable_index( "zero" ) << std::endl; 	
	
	
	std::cout << "done" << std::endl; 
	
	return 0; 
}

int find_variable_index( std::string find_me )
{
	return name_to_int[ find_me ]; 	
}

int safe_find_variable_index( std::string find_me )
{
	if( name_to_int.count( find_me ) < 1 )
	{
		std::cout << "Error: Attempted to access element [" << find_me << "], which was not found." << std::endl;
		std::cout << "       The known elements are: " << std::endl; 
		std::unordered_map<std::string,int>::iterator it = name_to_int.begin(); 
		while( it != name_to_int.end() )
		{
			std::cout << "          " << it->first << " -> " << it->second << std::endl; 
			it++; 
		}
		std::cout << "I'm going to crash out so you can find your bug." << std::endl << std::endl; 
		exit( -3 ); 
	}
	return name_to_int[ find_me ]; 	
}