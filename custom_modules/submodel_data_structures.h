/*
 * submodel_data_structures.h
 *
 *  Created on: ?
 *      Author: Paul Macklin
 *  
 * 	Description:
 * 		Auxiliary class to manage submodels
 * 
 * 	Source: https://github.com/pc4covid19/COVID19/blob/master/PhysiCell/custom_modules/submodel_data_structures.cpp
*/


#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM;
using namespace PhysiCell;

#ifndef __submodel_data__
#define __submodel_data__

class Submodel_Information
{
 private:
 public:
	std::string name; 
	std::string version;
	void(*main_function)(Cell*,Phenotype&,double); 
	
	std::vector< std::string > microenvironment_variables; 
	std::vector< std::string > cell_variables; // custom data  
	
	Submodel_Information( void ); 
	
	void register_model( void ); 
	
	void display( std::ostream& os ); 
}; 

class Submodel_Registry
{
 private:
	std::vector<Submodel_Information*> submodels; 
 public:  
	void register_model( Submodel_Information& model ); 
	void display( std::ostream& os ); 
	
};

extern Submodel_Registry submodel_registry; 

#endif