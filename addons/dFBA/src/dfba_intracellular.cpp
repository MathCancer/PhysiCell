#include "dfba_intracellular.h"

#include <sstream>
#include <iostream>


dFBAIntracellular::dFBAIntracellular() : Intracellular()
{
    this->intracellular_type = "dfba"
}

dFBAIntracellular::dFBAIntracellular(pugi::xml_node& node)
{
    intracellular_type = "dfba";
	this->initialize_intracellular_from_pugixml(node);   
}

dFBAIntracellular::dFBAIntracellular(dFBAIntracellular* copy) 
{
    intracellular_type = copy->intracellular_type;
	sbml_filename = copy->sbml_filename;
	parameters = copy->parameters;
}


int dFBAIntracellular::update_phenotype_parameters(PhysiCell::Phenotype& phenotype) 
{
    
}


void dFBAIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
    pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        this->sbml_filename = PhysiCell::xml_get_my_string_value (node_sbml);
        this->model.readSBMLModel(this->sbml_filename.c_str());
    }
	
	pugi::xml_node node_species = node.child( "map" );
	while( node_species )
	{
		std::string substrate_name = node_species.attribute( "PC_substrate" ).value(); 
		if( substrate_name != "" )
		{
			std::string boundary_reaction = node_species.attribute( "boundary_reaction" ).value();
			substrate_fluxes[substrate_name] = boundary_reaction;
		}
		node_species = node_species.next_sibling( "map" ); 
	}
}

int dFBAIntracellular::dFBAIntracellular::start()
{

}

bool dFBAIntracellular::dFBAIntracellular::need_update()
{
    return PhysiCell::PhysiCell_globals.current_time >= this->need_update;
}

int dFBAIntracellular::dFBAIntracellular::update()
{
    map<std::string, std::string>::iterator it;
    for(it = this->substrate_fluxes.begin(); it != this->substrate_fluxes.end(); it++)
    {
        std::string pc_density = it->first;
        std::string fba_reaction = it->second;

        int density_index = microenvironment.find_density_index(pc_density);

        // double density_value = pCell->nearest_density_vector()[density_index];
        double flux_bound = 0; // should be calculated from density

        this->model.setReactionLowerBound(fba_reaction, flux_bound)
        pCell->fba_model.runFBA();
    }
    

}

int dFBAIntracellular:: update_phenotype_parameters(PhysiCell::Phenotype& phenotype)
{

}

int dFBAIntracellular::validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype)
{

}

int dFBAIntracellular::validate_SBML_species()
{

}

double dFBAIntracellular::get_parameter_value(std::string name)
{

}

int dFBAIntracellular::set_parameter_value(std::string name, double value)
{

}


std::string dFBAIntracellular::get_state(){

}


void dFBAIntracellular::save_dFBA(std::string path, std::string index) 
{
	
}