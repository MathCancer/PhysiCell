#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <vector> 
#include <unordered_map> 

#include "my_test.h" 
namespace blah
{

template <class T>
Parameter<T>::Parameter()
{
	name = "unnamed"; 
	units= "dimensionless"; 
/*	
	T* pT; 
	pT = new T; 
	value = *pT; 
*/
	value = 1-1; 
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
	value = 1-1; 
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
	return parameters[ name_to_index_map[str] ]; 
}


template <class T>
int Parameters<T>::find_index( std::string search_name )
{
	return name_to_index_map[ search_name ]; 
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
	Parameter<T>* pNew; 
	pNew = new Parameter<T> ;
	pNew->name = my_name ; 
	
	int n = parameters.size(); 
	
	parameters.push_back( *pNew ); 
	
	name_to_index_map[ my_name ] = n; 
	return; 
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name , T& my_value )
{
	Parameter<T>* pNew; 
	pNew = new Parameter<T> ;
	pNew->name = my_name ; 
	pNew->value = my_value; 
	
	int n = parameters.size(); 
	
	parameters.push_back( *pNew ); 
	
	name_to_index_map[ my_name ] = n; 
	return; 
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name , T my_value )
{
	Parameter<T>* pNew; 
	pNew = new Parameter<T> ;
	pNew->name = my_name ; 
	pNew->value = my_value; 
	
	int n = parameters.size(); 
	
	parameters.push_back( *pNew ); 
	
	name_to_index_map[ my_name ] = n; 
	return; 
}

template <class T>
void Parameters<T>::add_parameter( std::string my_name , T& my_value , std::string my_units )
{
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
void Parameters<T>::add_parameter( std::string my_name , T my_value , std::string my_units )
{
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
void Parameters<T>::add_parameter( Parameter<T>& param )
{
	int n = parameters.size(); 
	parameters.push_back( param); 
	name_to_index_map[ param.name ] = n; 
	return; 
}

std::ostream& operator<<( std::ostream& os , const User_Parameters up )
{
	os << "Bool parameters:: " << std::endl << up.bools << std::endl; 
	os << "Int parameters:: " << std::endl << up.ints << std::endl; 
	os << "Double parameters:: " << std::endl << up.doubles << std::endl; 
	os << "String parameters:: " << std::endl << up.strings << std::endl; 
	return os; 
}

}

using namespace blah; 

int main( int argc, char* argv[] )
{
	std::cout << std::endl << std::endl << "-----===== TEST =====-----" << std::endl << std::endl; 
	
	std::cout << "Create user parameters, and display: " << std::endl; 
	User_Parameters parameters; 
	std::cout << parameters << std::endl << std::endl; 
	
	std::cout << "Add bool, int, double, and string params and display: " << std::endl; 
	parameters.bools.add_parameter( "my_bool" ); 
	parameters.ints.add_parameter( "my_int" ); 
	parameters.doubles.add_parameter( "my_double" ); 
	parameters.strings.add_parameter( "my_string" ); 
	std::cout << parameters << std::endl << std::endl; 
	
	std::cout << "Find indices of those parameters, display parameters by index: " << std::endl; 
	int i_b = parameters.bools.find_index( "my_bool" ); 
	int i_i = parameters.ints.find_index( "my_int" ); 
	int i_d = parameters.doubles.find_index( "my_double" ); 
	int i_s = parameters.strings.find_index( "my_string" ); 
 
	std::cout << parameters.bools(i_b) << " " 
		<< parameters.ints(i_i) << " " 
		<< parameters.doubles(i_d) << " " 
		<< parameters.strings(i_s) << std::endl << std::endl; 
	
	std::cout << "Change units of parameters (by index) and display : " << std::endl; 
	parameters.bools[i_b].units = "uB"; 
	parameters.ints[i_i].units = "uI"; 
	parameters.doubles[i_d].units = "uD"; 
	parameters.strings[i_s].units = "uS"; 
	std::cout << parameters << std::endl << std::endl; 

	std::cout << "Change values of parameters (by string name) and display : " << std::endl; 
	parameters.bools( "my_bool" ) = true; 
	parameters.ints( "my_int" ) = 42; 
	parameters.doubles( "my_double" ) = 42.05; 
	parameters.strings( "my_string" ) = "stringy" ; 
	std::cout << parameters << std::endl << std::endl; 
	
	std::cout << "Add one more parameter of each type and display : " << std::endl; 
	parameters.bools.add_parameter( "yuck" , false, "dimensionless" ); 
	parameters.ints.add_parameter( "data_size" , 32, "bytes" ); 
	parameters.doubles.add_parameter( "crayons" , 38.4, "megacrayolas" ); 
	parameters.strings.add_parameter( "color" , "rgb(38,183,94)", "rgb" ); 
	std::cout << parameters << std::endl << std::endl; 
	
	std::cout << "For each parameter type, iterate through by index and display : " << std::endl; 
	std::cout << "boolean parameters: " << std::endl; 
	for( int i=0; i < parameters.bools.size() ; i++ )
	{
		std::cout << parameters.bools[i] << std::endl; 
	}
	std::cout << std::endl; 
	
	std::cout << "integer parameters: " << std::endl; 
	for( int i=0; i < parameters.ints.size() ; i++ )
	{
		std::cout << parameters.ints[i] << std::endl; 
	}
	std::cout << std::endl; 
	
	std::cout << "double parameters: " << std::endl; 
	for( int i=0; i < parameters.doubles.size() ; i++ )
	{
		std::cout << parameters.doubles[i] << std::endl; 
	}
	std::cout << std::endl; 
	
	std::cout << "string parameters: " << std::endl; 
	for( int i=0; i < parameters.strings.size() ; i++ )
	{
		std::cout << parameters.strings[i] << std::endl; 
	}	
	std::cout << std::endl; 

	std::cout << "A bit more practice in different access methods on a new parameters : " << std::endl; 
	parameters.bools.add_parameter( "cat", false , "meh" ); 
	parameters.bools( "cat" ) = true; 
	parameters.bools(0) = false; 
	parameters.bools[0].value = false; 
	parameters.bools["cat"].units = "truthiness"; 
	
	Parameter<double> dog; 
	dog.name = "dog"; 
	parameters.doubles.add_parameter( dog ); 
	
	Parameter<int> emu ("emu" ); 
	parameters.ints.add_parameter( emu ); 
	emu.value = 32; 
	parameters.ints["emu"] = emu; 
	
	Parameter<std::string> meh( "ostentatiousness" ); 
	parameters.strings.add_parameter( meh ); 
	parameters.strings("ostentatiousness" ) = "plenty" ; 
	int index = parameters.strings.find_index( "ostentatiousness" ); 
	parameters.strings[index].units = "megafonzies"; 
	
	std::cout << parameters << std::endl << std::endl; 


		
	std::cout << "done!" << std::endl; 
	return 0; 
}
