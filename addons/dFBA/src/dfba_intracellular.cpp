#include "dfba_intracellular.h"
#include <sstream>
#include <iostream>

dFBAIntracellular::dFBAIntracellular() : Intracellular()
{
    this->intracellular_type = "dfba";
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
    // model = copy->model;
    model.readSBMLModel(copy->sbml_filename.c_str());
    model.initLpModel();
    model.runFBA();
}


void dFBAIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{

    pugi::xml_node node_sbml = node.child( "sbml_filename" );

	if ( node_sbml )
	{ 
        this->sbml_filename = PhysiCell::xml_get_my_string_value (node_sbml);
        
    }
    else
    {
        std::cout << "Error: attempted get sbml_filename but not foun. " << std::endl;
        std::cout << "Please double-check your exchange nodes in the config file." << std::endl;
        std::cout << std::endl; 
        exit(-1); 
    }
	
    pugi::xml_node node_exchange = node.child( "exchange" );
    
	while( node_exchange )
	{
        exchange_data ex_struct;
        kinetic_parm Km;
        kinetic_parm Vmax;
        

		string density_name = node_exchange.attribute( "substrate" ).value(); 
        int density_index = microenvironment.find_density_index( density_name ); 
        std::cout << "Parsing " << density_name << std::endl;
        std::string actual_name = microenvironment.density_names[ density_index ]; 
			
        // error check 
        if( std::strcmp( density_name.c_str() , actual_name.c_str() ) != 0 )
        {
            std::cout << "Error: attempted to set secretion/uptake/export for \"" 
                << density_name << "\", which was not found in the microenvironment." << std::endl 
            << "       Please double-check your substrate name in the config file." << std::endl << std::endl; 
            exit(-1); 
        }
        
        pugi::xml_node node_fba_flux = node_exchange.child( "fba_flux" ); 
		if( node_fba_flux )
		{  
            ex_struct.fba_flux_id = PhysiCell::xml_get_my_string_value(node_fba_flux);
        }
        else {
            std::cout << "Error: attempted get fba_flux node for "; 
            std::cout << ex_struct.density_name << "\", but not found." << std::endl;
            std::cout << "Please double-check your exchange nodes in the config file." << std::endl;
            std::cout << std::endl; 
            exit(-1); 
        }

        pugi::xml_node node_Km = node_exchange.child( "Km" ); 
		if( node_Km )
		{
            Km.name = "Km";
            Km.untis = node_Km.attribute("units").value();
            Km.value = PhysiCell::xml_get_my_double_value(node_Km);
            
        }
        else {
            std::cout << "Error: attempted get Km node for "; 
            std::cout << density_name << "\", but not found." << std::endl;
            std::cout << "Please double-check your exchange nodes in the config file." << std::endl;
            std::cout << std::endl; 
            exit(-1); 
        }

        pugi::xml_node node_Vmax = node_exchange.child( "Vmax" ); 
		if( node_Vmax )
		{
            Vmax.name = "Vmax";
            Vmax.untis = node_Vmax.attribute("units").value();
            Vmax.value = PhysiCell::xml_get_my_double_value(node_Vmax);
            
        }
        else {
            std::cout << "Error: attempted get Vmax node for "; 
            std::cout << density_name << "\", but not found." << std::endl;
            std::cout << "Please double-check your exchange nodes in the config file." << std::endl;
            std::cout << std::endl; 
            exit(-1); 
        }
		
        ex_struct.density_name = density_name;
        ex_struct.density_index = density_index;
        ex_struct.Km = Km;
        ex_struct.Vmax = Vmax;

        this->substrate_exchanges[density_name] = ex_struct;
		node_exchange = node_exchange.next_sibling( "exchange" ); 
	}

    std::cout << "Loaing SBML model from: " << this->sbml_filename << std::endl;
    this->model.readSBMLModel(this->sbml_filename.c_str());
    this->model.initLpModel();
    this->model.runFBA();

}

void dFBAIntracellular::dFBAIntracellular::start()
{
    // return 0;
}

bool dFBAIntracellular::dFBAIntracellular::need_update()
{
    return PhysiCell::PhysiCell_globals.current_time >= this->next_model_run;
}

void dFBAIntracellular::update(PhysiCell::Cell* pCell, PhysiCell::Phenotype& phenotype, double dt)
{
   
    map<std::string, exchange_data>::iterator it;
    for(it = this->substrate_exchanges.begin(); it != this->substrate_exchanges.end(); it++)
    {
        std::string substrate_name = it->first;
        exchange_data ex_strut = it->second;

        double Vmax = ex_strut.Vmax.value;
        double Km = ex_strut.Km.value;
        
        // geting the amount of substrate
        double substrate_conc = pCell->nearest_density_vector()[ex_strut.density_index];
        // useing irreversible Michaelis Menten kinetics to estimate the flux bound
        double flux_bound = (Vmax * substrate_conc) / (Km + substrate_conc); // should be calculated from density
        // Change sign to use as lower bound of the exchange flux
        flux_bound *= -1;
        // Updateing the lower bound of the corresponding exchange flux

        std::cout << " - [" << substrate_name << "] = " << substrate_conc;
        std::cout << " ==> " << ex_strut.fba_flux_id << " = " << flux_bound << std::endl;

        this->model.setReactionLowerBound(ex_strut.fba_flux_id, flux_bound);
    }
    this->model.runFBA();
    // this->update_phenotype_parameters(phenotype);
    // return 0;
}

int dFBAIntracellular::update_phenotype_parameters(PhysiCell::Phenotype& phenotype)
{
    /*
        Required steps for the update

        1- rescale exchange fluxes from the dfba model
        2- update the net_export_rates using the rescaled exchanges
        3- remove the internalized substrates if needed
        4- update the cell volumne using the growth rate from FBA
        5- if no-growth check non-growth associated mantainanace to update apoptotic rate
    */

    map<std::string, exchange_data>::iterator it;
    for(it = this->substrate_exchanges.begin(); it != this->substrate_exchanges.end(); it++)
    {
        std::string substrate_name = it->first;
        exchange_data ex_strut = it->second;
        
        std::string fba_flux_id = ex_strut.fba_flux_id;
        FBA_reaction* exchange = this->model.getReaction(fba_flux_id);
        double flux = exchange->getFluxValue();
        
        // how to rescale FBA exchanges into net_export_rates
        float scaling = 1;
        flux *= scaling;
        phenotype.secretion.net_export_rates[ex_strut.density_index] = flux;
    }

    float delta_vol = 1;
    float growth_rate = 1; //
    phenotype.volume.total; // FBA predicts growth rate mu
    phenotype.volume.multiply_by_ratio(delta_vol);

    return 0;
}




void dFBAIntracellular::save_dFBA(std::string path, std::string index) 
{
	
}