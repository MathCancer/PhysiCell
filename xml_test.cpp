#include "./BioFVM/pugixml.hpp" 
#include "./beta/PhysiCell_pugixml.h" 

// using namespace BioFVM;
using namespace PhysiCell;

// g++ -o test xml_test.cpp ./BioFVM/pugixml.cpp ./beta/PhysiCell_pugixml.cpp

int main( int argc, char* argv[] )
{
	
	
	load_PhysiCell_config_file( "./config/PhysiCell_settings.xml" ); 
	
	pugi::xml_node node = xml_find_node( physicell_config_root , "domain" ); 
	if( node )
	{
		node = xml_find_node( node , "xmin" ); 
	}
	else
	{
		std::cout<< "no!" << std::endl;
	}	
	
	if( node )
	{
		std::cout << node.text().get() << std::endl; 
	}
	else
	{
		std::cout<< "no!" << std::endl;
	}
	
	node = xml_find_node( physicell_config_root ,  "domain" ); 
	std::cout << xml_get_string_value( node, "xmin" ) << std::endl ; 
	
	std::string str = xml_get_string_value( node, "xmax" ); 
	std::cout << str << std::endl; 
	
	std::cout << xml_get_double_value( node, "zmin" ) << std::endl ; 	
	
	std::cout << xml_get_int_value( node, "dz" ) << std::endl ; 	
	
	return 0; 
}
