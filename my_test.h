#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <vector> 
#include <unordered_map> 

namespace blah 
{

template <class T> 
class Parameter
{
 private:
	template <class Y>
	friend std::ostream& operator<<(std::ostream& os, const Parameter<Y>& param); 

 public: 
	std::string name; 
	std::string units; 
	T value; 
	
	Parameter();
	Parameter( std::string my_name ); 
	
	void operator=( T& rhs ); 
	void operator=( T rhs ); 
	void operator=( Parameter& p ); 
};

template <class T>
class Parameters
{
 private:
	std::unordered_map<std::string,int> name_to_index_map; 
	
	template <class Y>
	friend std::ostream& operator<<( std::ostream& os , const Parameters<Y>& params ); 

 public: 
	Parameters(); 
 
	std::vector< Parameter<T> > parameters; 
	
	void add_parameter( std::string my_name ); 
	void add_parameter( std::string my_name , T& my_value ); 
	void add_parameter( std::string my_name , T my_value ); 
	void add_parameter( std::string my_name , T& my_value , std::string my_units ); 
	void add_parameter( std::string my_name , T my_value , std::string my_units ); 
	
	void add_parameter( Parameter<T>& param );
	
	int find_index( std::string search_name ); 
	
	// these access the values 
	T& operator()( int i );
	T& operator()( std::string str ); 

	// these access the full, raw parameters 
	Parameter<T>& operator[]( int i );
	Parameter<T>& operator[]( std::string str ); 
	
	int size( void ) const; 
};
 



class User_Parameters
{
 private:
	friend std::ostream& operator<<( std::ostream& os , const User_Parameters up ); 
 
 public:
	Parameters<bool> bools; 
	Parameters<int> ints; 
	Parameters<double> doubles; 
	Parameters<std::string> strings; 
}; 

	
}

