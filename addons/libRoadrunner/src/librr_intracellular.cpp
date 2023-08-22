#include "librr_intracellular.h"

#include <sstream>
#include <iostream>

RoadRunnerIntracellular::RoadRunnerIntracellular() : Intracellular()
{
	intracellular_type = "sbml";
    std::cout << "====== " << __FUNCTION__ << "() intracellular_type=" << intracellular_type << std::endl;
    std::cout << "====== " << __FUNCTION__ << "() sbml_filename = " <<  sbml_filename << std::endl;
	// initial_values.clear();
	// mutations.clear();
	parameters.clear();
}

// constructor using XML node
RoadRunnerIntracellular::RoadRunnerIntracellular(pugi::xml_node& node)
{
    // std::cout << "======rwh " << __FUNCTION__ << ": node.name() =" << node.name() << std::endl;
	intracellular_type = "roadrunner";
	initialize_intracellular_from_pugixml(node);
    // std::cout << "======rwh " << __FUNCTION__ << "(node) intracellular_type=" << intracellular_type << std::endl;
    // std::cout << "======rwh " << __FUNCTION__ << "(node) sbml_filename = " <<  sbml_filename << std::endl;
    // std::cout << "======rwh " << __FUNCTION__ << "(node) this=" <<  this << std::endl;
    // std::cout << "======rwh " << __FUNCTION__ << "(node) this->sbml_filename=" <<  this->sbml_filename << std::endl;
}

// Intracellular* RoadRunnerIntracellular::clone() // --> 'Intracellular' does not name a type
// {
// 	return static_cast<Intracellular*>(new RoadRunnerIntracellular(this));
// }

// rwh: review this
RoadRunnerIntracellular::RoadRunnerIntracellular(RoadRunnerIntracellular* copy) 
{
	intracellular_type = copy->intracellular_type;
	sbml_filename = copy->sbml_filename;
	// cfg_filename = copy->cfg_filename;
	// time_step = copy->time_step;
	// discrete_time = copy->discrete_time;
	// time_tick = copy->time_tick;
	// scaling = copy->scaling;
	// initial_values = copy->initial_values;
	// mutations = copy->mutations;
	parameters = copy->parameters;
	
}

// Parse the <intracellular> info in the .xml for (possibly) each <cell_definition ...>, e.g.
// <intracellular type="roadrunner">
// 	<sbml_filename>./config/Toy_SBML_Model_2.xml</sbml_filename>
// 	<time_step>1</time_step>
//     <species substrate="oxygen">Oxy</species>
//     <species substrate="glucose">Glc</species>
//     <species custom_data="energy">Energy</species>
void RoadRunnerIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_sbml = node.child( "sbml_filename" );
	if ( node_sbml )
	{ 
        sbml_filename = PhysiCell::xml_get_my_string_value (node_sbml); 
        std::cout << "\n------------- "  << __FUNCTION__ << ": sbml_filename = " << sbml_filename << std::endl;
    }
	
	pugi::xml_node node_species = node.child( "map" );
	while( node_species )
	{
        // ---------  substrate
        
		std::string substrate_name = node_species.attribute( "PC_substrate" ).value(); 
		if( substrate_name != "" )
		{
            //std::cout << "-----------" << node_species.attribute( "sbml_species" ).value() << std::endl; 
			std::string species_name = node_species.attribute( "sbml_species" ).value();
			substrate_species[substrate_name] = species_name;
            std::cout << "\n------------- "  << __FUNCTION__ << ": species_name= " << species_name << std::endl;
		}
        // ---------  custom_data
		std::string custom_data_name = node_species.attribute( "PC_custom_data" ).value(); 
		if( custom_data_name != "" )
		{
			std::string species_name = node_species.attribute( "sbml_species" ).value();
			custom_data_species[custom_data_name] = species_name;
            // std::cout << "\n------------- "  << __FUNCTION__ << ": species_name= " << species_name << std::endl;
		}
        
        
        // ---------  phenotype_data
        std::string phenotype_name = node_species.attribute( "PC_phenotype" ).value(); 
        
		if( phenotype_name != "" )
		{
			std::string species_name = node_species.attribute( "sbml_species" ).value();
			phenotype_species[phenotype_name] = species_name;
            // std::cout << "\n------------- "  << __FUNCTION__ << ": species_name= " << species_name << std::endl;
		}

		node_species = node_species.next_sibling( "map" ); 
	}
	
    std::cout << "  ------- substrate_species map:"  << std::endl;
    for(auto elm : substrate_species)
    {
        std::cout << "      "  << elm.first << " -> " << elm.second << std::endl;
    }
    std::cout << "  ------- custom_data_species map:"  << std::endl;
    for(auto elm : custom_data_species)
    {
        std::cout << "      "  << elm.first << " -> " << elm.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "  ------- phenotype_species map:"  << std::endl;
    for(auto elm : phenotype_species)
    {
        std::cout << "      "  << elm.first << " -> " << elm.second << std::endl;
    }
    std::cout << std::endl;

}


void RoadRunnerIntracellular::start()
{
    // called when a new cell is created; creates the unique 'rrHandle'
    rrc::RRVectorPtr vptr;

    //std::cout << "\n------------ " << __FUNCTION__ << ": librr_intracellular.cpp: start() called\n";
    // this->enabled = true;

    //std::cout << "\n------------ " << __FUNCTION__ << ": doing: rrHandle = createRRInstance()\n";

    rrHandle = createRRInstance();

    //std::cout << "\n------------ " << __FUNCTION__ << ": rrHandle = " << rrHandle << std::endl;

    // if (!rrc::loadSBML (rrHandle, get_cell_definition("lung epithelium").sbml_filename.c_str())) 
    //std::cout << "     sbml_filename = " << sbml_filename << std::endl;

    // TODO: don't hard-code name
    if ( !rrc::loadSBML(rrHandle, (sbml_filename).c_str() ) )
    // std::cout << "     for now, hard-coding sbml_file = ./config/Toy_SBML_Model_1.xml" << std::endl;
    // if (!rrc::loadSBML(rrHandle, "./config/Toy_SBML_Model_1.xml") )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        // return -1;
        // 	printf ("Error message: %s\n", getLastError());
        exit (0);
    }

    // std::cout << "     rrHandle=" << rrHandle << std::endl;

    int r = rrc::getNumberOfReactions(rrHandle);
    int m = rrc::getNumberOfFloatingSpecies(rrHandle);
    int b = rrc::getNumberOfBoundarySpecies(rrHandle);
    int p = rrc::getNumberOfGlobalParameters(rrHandle);
    int c = rrc::getNumberOfCompartments(rrHandle);


    //std::cerr << "Number of reactions = " << r << std::endl;
    //std::cerr << "Number of floating species = " << m << std::endl;  // 4
    //std::cerr << "Number of boundary species = " << b << std::endl;  // 0
    //std::cerr << "Number of compartments = " << c << std::endl;  // 1

    //std::cerr << "Floating species names:\n";
    //std::cerr << "-----------------------\n";
    std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(rrHandle));
    //std::cerr <<  species_names_str <<"\n"<< std::endl;
    std::stringstream iss(species_names_str);
    std::string species_name;
    int idx = 0;
    while (iss >> species_name)
    {
        species_result_column_index[species_name] = idx;
        //std::cout << species_name << " -> " << idx << std::endl;
        idx++;
    }

    vptr = rrc::getFloatingSpeciesConcentrations(rrHandle);
    //std::cerr << vptr->Count << std::endl;
/*     for (int kdx=0; kdx<vptr->Count; kdx++)
    {
        std::cerr << kdx << ") " << vptr->Data[kdx] << std::endl;
    } */
    //std::cerr << "----------  end start() -------------\n";
    
    rrc::freeVector(vptr);
    // return 0;
}

bool RoadRunnerIntracellular::need_update()
{
    return PhysiCell::PhysiCell_globals.current_time >= this->next_librr_run;
}

// solve the intracellular model
void RoadRunnerIntracellular::update()
{
    static double start_time = 0.0;
    static double end_time = 0.01;
    // static double end_time = 10.0;
    // static int num_vals = 1;
    // static int num_vals = 10;
    static int num_vals = 2;

    // result = rrc::simulateEx (pCell->phenotype.molecular.model_rr, 0, 10, 10);  // start time, end time, and number of points
    //std::cout << __FUNCTION__ << " ----- update(): this=" << this << std::endl;
    //std::cout << __FUNCTION__ << " ----- update(): rrHandle=" << this->rrHandle << std::endl;

    // if (this->result != 0)   // apparently not necessary (done in freeRRCData hopefully)
    rrc::freeRRCData (this->result);

    this->result = rrc::simulateEx (this->rrHandle, start_time, end_time, num_vals);  // start time, end time, and number of points


    // this->next_librr_run += this->rrHandle.get_time_to_update();
    // std::cout << "----- update(): result=" << result << std::endl;
    //std::cout << "----- update(): result->CSize=" << this->result->CSize << std::endl;
    //std::cout << "----- update(): result->RSize=" << this->result->RSize << std::endl;  // should be = num_vals
    // std::cout << "----- update(): result->ColumnHeaders[0]=" << result->ColumnHeaders[0] << std::endl;  // = "time"

    // debug - does it generate expected data?
    int index = 0;
    // Print out column headers... typically time and species.
    for (int col = 0; col < this->result->CSize; col++)
    {
        // std::cout << result->ColumnHeaders[index++];
        // std::cout << std::left << std::setw(15) << result->ColumnHeaders[index++];
        //std::cout << std::left << this->result->ColumnHeaders[index++];
        // if (col < result->CSize - 1)
        // {
        // 	// std::cout << "\t";
        // 	std::cout << "  ";
        // }
    }
    //std::cout << "\n";

    index = 0;
    // Print out the data
    for (int row = 0; row < this->result->RSize; row++)
    {
        for (int col = 0; col < this->result->CSize; col++)
        {
            // std::cout << result->Data[index++];
            //std::cout << std::left << std::setw(15) << this->result->Data[index++];
            // if (col < result->CSize -1)
            // {
            // 	// std::cout << "\t";
            // 	std::cout << "  ";
            // }
        }
       // std::cout << "\n";
    }
    // int idx = (result->RSize - 1) * result->CSize + 1;
    // std::cout << "Saving last energy value (cell custom var) = " << result->Data[idx] << std::endl;
    // pCell->custom_data[energy_cell_idx]  = result->Data[idx];

    // return 0;
}

double RoadRunnerIntracellular::get_parameter_value(std::string param_name)
{
    rrc::RRVectorPtr vptr;

    //std::cout << "-----------"  << __FUNCTION__ << "-----------" << std::endl;
    // std::cout << "    substrate_name = " << substrate_name << std::endl;
    //std::cout << "    param_name = " << param_name << std::endl;

    // TODO: optimize this eventually
    // std::map<std::string, int> species_result_column_index;
    // int num_columns = result->CSize;
    // int offset = (num_rows_result_table-1) * result->CSize - 1;
    // int offset = (num_rows_result_table-1) * result->CSize;
    // offset += (num_rows_result_table-1) * result->CSize - 1;

    // int offset = species_result_column_index[name];
    // std::string species_name = this->substrate_species[substrate_name];
    // std::cout << "    species_name = " << species_name << std::endl;

    vptr = rrc::getFloatingSpeciesConcentrations(this->rrHandle);
    //std::cerr << vptr->Count << std::endl;
    for (int kdx=0; kdx<vptr->Count; kdx++)
    {
        //std::cerr << kdx << ") " << vptr->Data[kdx] << std::endl;
    }

    int offset = species_result_column_index[param_name];
    //std::cout << "    result offset = "<< offset << std::endl;
    // double res = this->result->Data[offset];
    double res = vptr->Data[offset];
    //std::cout << "    res = " << res << std::endl;
    rrc::freeVector(vptr);
    return res;
}
	
// rwh: might consider doing a multi-[species_name, value] "set" method
void RoadRunnerIntracellular::set_parameter_value(std::string species_name, double value)
{
    rrc::RRVectorPtr vptr;

    vptr = rrc::getFloatingSpeciesConcentrations(this->rrHandle);
    int idx = species_result_column_index[species_name];
    vptr->Data[idx] = value;
	// rrc::setFloatingSpeciesConcentrations(pCell->phenotype.molecular.model_rr, vptr);
    rrc::setFloatingSpeciesConcentrations(this->rrHandle, vptr);
    rrc::freeVector(vptr);
    // return 0;
}

RoadRunnerIntracellular* getRoadRunnerModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<RoadRunnerIntracellular*>(phenotype.intracellular);
}

void RoadRunnerIntracellular::save_libRR(std::string path, std::string index)
{
	std::string state_file_name = path + "/states_" + index + ".dat";
// 	std::string state_file_name = path + "/states_" + index + ".csv";
	std::ofstream state_file( state_file_name );
	state_file << "---------  dummy output from save_libRR  ---------" << std::endl;
	state_file << "ID,state" << std::endl;
	for( auto cell : *PhysiCell::all_cells )
		state_file << cell->ID << "," << cell->phenotype.intracellular->get_state() << std::endl;
	state_file.close();
}

std::string RoadRunnerIntracellular::get_state()
{
    return sbml_filename;
}


int RoadRunnerIntracellular::update_phenotype_parameters(PhysiCell::Phenotype& phenotype)
{
    for(auto elm : phenotype_species)
    {
        // motility params
        if (elm.first[0] == 'm')
        {
            if (elm.first == "mms")
            {
                phenotype.motility.migration_speed = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else if (elm.first == "mpt")
            {
                phenotype.motility.persistence_time = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else if (elm.first == "mmb")
            {
                phenotype.motility.migration_bias = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else
            {
            }
        }
        // death params
        else if (elm.first[0] == 'd')
        {
            if (elm.first == "da")
            {                
                phenotype.death.rates[0] = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else if (elm.first == "dn")
            {
                phenotype.death.rates[1] = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else
            {
            }
        }
        // secretion params
        else if (elm.first[0] == 's')
        {
            // parsing attribute and getting substrate name
            std::string s = elm.first;
            std::string delimiter = "_";

            size_t pos = 0;
            std::string token;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                s.erase(0, pos + delimiter.length());
            }
            int sub_index = microenvironment.find_density_index(s);

            //transport types
            //uptake rate
            if (elm.first.substr(0,3) == "sur")
            {
                //std::cout << sub_index << std::endl;
                //std::cout << "Before sur1 : " << phenotype.secretion.uptake_rates[sub_index] << std::endl;
                phenotype.secretion.uptake_rates[1] = phenotype.intracellular->get_parameter_value(elm.second);
                //std::cout << "After sur1 : " << phenotype.secretion.uptake_rates[sub_index] << std::endl;
            }
            //secretion rate
            else if (elm.first.substr(0,3) == "ssr")
            {
                phenotype.secretion.secretion_rates[sub_index] = phenotype.intracellular->get_parameter_value(elm.second);
            }
            //secretion density
            else if (elm.first.substr(0,3) == "ssd")
            {
                phenotype.secretion.saturation_densities[sub_index] = phenotype.intracellular->get_parameter_value(elm.second);
            }
            //net export rate
            else if (elm.first.substr(0,3) == "ser")
            {
                phenotype.secretion.net_export_rates[sub_index] = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else
            {
            }
        }
        
        // cycle params
        else if (elm.first[0] == 'c')
        {
            if (elm.first.substr(0,3) == "ctr")
            {
                // parsing attribute and getting substrate name
                std::string s = elm.first;
                std::string delimiter = "_";

                size_t pos = 0;
                std::string token;
                int counter = 0;
                int start_index;
                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    //std::cout << counter << " : "<< token << std::endl;
                    if (counter == 1)
                    {
                        start_index = atoi( token.c_str() );
                    }
                    s.erase(0, pos + delimiter.length());
                    counter += 1;
                }
                int end_index = atoi( s.c_str() );
                //std::cout << "START INDEX : " << start_index << std::endl;
                //std::cout << "END INDEX : " << end_index << std::endl;
                phenotype.cycle.data.transition_rate(start_index,end_index) = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else
            {
            }
        }
        
        // volume params
        else if (elm.first[0] == 'v')
        {
            if (elm.first == "vtsc")
            {
                phenotype.volume.target_solid_cytoplasmic = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else if (elm.first == "vtsn")
            {
                phenotype.volume.target_solid_nuclear = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else if (elm.first == "vff")
            {
                phenotype.volume.target_fluid_fraction = phenotype.intracellular->get_parameter_value(elm.second);
            }
            else
            {
            }
        }
        else
        {
        }
        
    }
    //std::cout << std::endl;
    return 0;
}


int RoadRunnerIntracellular::validate_PhysiCell_tokens(PhysiCell::Phenotype& phenotype)
{
    for(auto elm : phenotype_species)
    {
        //std::cout << "PhysiCell_token_validation" << std::endl;
        //std::cout << elm.first << " : " << elm.second << std::endl;
        
        // motility params
        if (elm.first[0] == 'm')
        {
            if (elm.first == "mms")
            {
            }
            else if (elm.first == "mpt")
            {
            }
            else if (elm.first == "mmb")
            {
            }
            else
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at motility parameters. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
        }
        // death params
        else if (elm.first[0] == 'd')
        {
            if (elm.first == "da")
            {                
            }
            else if (elm.first == "dn")
            {
            }
            else
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at death parameters. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
        }
        // secretion params
        else if (elm.first[0] == 's')
        {
            // parsing attribute and getting substrate name
            std::string s = elm.first;
            std::string delimiter = "_";
            size_t pos = 0;
            std::string token;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                s.erase(0, pos + delimiter.length());
            }
            int sub_index = microenvironment.find_density_index(s);
            //std::cout << "SUBSTRATE_INDEX = : " << sub_index << std::endl;
            if ( sub_index < 0 )
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no substrate named in the name of \"" << s << "\" at microenvironment. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
            
            if (elm.first.substr(0,3) == "sur")
            {
            }
            else if (elm.first.substr(0,3) == "ssr")
            {
            }
            else if (elm.first.substr(0,3) == "ssd")
            {
            }
            else if (elm.first.substr(0,3) == "ser")
            {
            }
            else
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at secretion parameters. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
        }
        else if (elm.first[0] == 'c')
        {
            if (elm.first.substr(0,3) == "ctr")
            {
                // getting num of phases
                int num_of_phases = (&(phenotype.cycle.model()))->phases.size();
                //std::cout << num_of_phases << std::endl;
                
                // getting start and end indices
                std::string s = elm.first;
                std::string delimiter = "_";
                size_t pos = 0;
                std::string token;
                int counter = 0;
                int start_index;
                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    if (counter == 1)
                    {
                        start_index = atoi( token.c_str() );
                    }
                    s.erase(0, pos + delimiter.length());
                    counter += 1;
                }
                int end_index = atoi( s.c_str() );
                
                // validating the indices
                if ( start_index > num_of_phases - 1)
                {
                    std::cout<< std::endl;
                    std::cout << "ERROR: Given transition start index is beyond cycle indices. Please double check it." << std::endl;
                    std::cout<< std::endl;
                    std::cout<< std::endl;
                    exit (-1);
                    return -1;
                }
                if ( end_index > num_of_phases - 1)
                {
                    std::cout<< std::endl;
                    std::cout << "ERROR: Given transition end index is beyond cycle indices. Please double check it." << std::endl;
                    std::cout<< std::endl;
                    std::cout<< std::endl;
                    exit (-1);
                    return -1;
                }
            }
            else
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at cycle parameters. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
        }
        
        else if (elm.first[0] == 'v')
        {
            if (elm.first == "vtsc")
            {
            }
            else if (elm.first == "vtsn")
            {
            }
            else if (elm.first == "vff")
            {
            }
            else
            {
                std::cout<< std::endl;
                std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at volume parameters. Please take a look token specifications." << std::endl;
                std::cout<< std::endl;
                std::cout<< std::endl;
                exit (-1);
                return -1;
            }
        }
        else
        {
            std::cout<< std::endl;
            std::cout << "ERROR: There is no specified token parameters in the name of \"" << elm.first << "\" at phenotypic parameters. Please take a look token specifications." << std::endl;
            std::cout<< std::endl;
            std::cout<< std::endl;
            exit (-1);
            return -1;
        }
        
    }
    std::cout << "---- Specified PhysiCell tokens at config file are validated. ----- " << std::endl;
    
    return 0;
}

int RoadRunnerIntracellular::validate_SBML_species()
{
    //std::cout << "---------VALIDATING_SBML_SPECIES START-------" << std::endl;
    
    // reading SBML
    rrHandle = createRRInstance();
    if ( !rrc::loadSBML(rrHandle, (sbml_filename).c_str() ) )
    {
        std::cerr << "------------->>>>>  Error while loading SBML file  <-------------\n\n";
        return -1;
    } 
    // getting Species Names
    std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(rrHandle));
    std::stringstream iss(species_names_str);
    std::string species_name;
    
    std::vector<std::string> all_species {};
    
    int idx = 0;
    while (iss >> species_name)
    {
        species_result_column_index[species_name] = idx;
        all_species.push_back(species_name);
        //std::cout << species_name << " -> " << idx << std::endl;
        idx++;
    }

    // Phenotype Species 
    for (auto elm : phenotype_species)
    {
        bool exist = 0;
       // std::cout << species_name.size() << std::endl;
        for (int i=0; i < all_species.size(); i++)
        {
            //std::cout << all_species[i] << std::endl;;
            //std::cout << "Comparing " << all_species[i] << " with " << elm.second << std::endl;
            if ( all_species[i] == elm.second )
            {
               //std::cout << "And they are the same..... " <<std::endl;
               exist = 1; 
            }
            idx++;  
        }
        if (!exist)
        {
            std::cout<< std::endl;
            std::cout << "ERROR: The specified SBML species in the name of \"" << elm.second << "\" at phenotypic species. Please take a look SBML species specifications." << std::endl;
            std::cout<< std::endl;
            std::cout<< std::endl;
            exit (-1);
            return -1;
        }
        //std::cout << "existence check : " << elm.second <<": " << exist << std::endl;
    }
    
    // Substrate Species
    for (auto elm : substrate_species)
    {
        bool exist = 0;
       // std::cout << species_name.size() << std::endl;
        for (int i=0; i < all_species.size(); i++)
        {
            //std::cout << all_species[i] << std::endl;;
            //std::cout << "Comparing " << all_species[i] << " with " << elm.second << std::endl;
            if ( all_species[i] == elm.second )
            {
               //std::cout << "And they are the same..... " <<std::endl;
               exist = 1; 
            }
            idx++;  
        }
        if (!exist)
        {
            std::cout<< std::endl;
            std::cout << "ERROR: The specified SBML species in the name of \"" << elm.second << "\" at substrate species. Please take a look SBML species specifications." << std::endl;
            std::cout<< std::endl;
            std::cout<< std::endl;
            exit (-1);
            return -1;
        }
        //std::cout << "existence check : " << elm.second <<": " << exist << std::endl;
    }    

    // custom data species
    for (auto elm : custom_data_species)
    {
        bool exist = 0;
       // std::cout << species_name.size() << std::endl;
        for (int i=0; i < all_species.size(); i++)
        {
            //std::cout << all_species[i] << std::endl;;
            //std::cout << "Comparing " << all_species[i] << " with " << elm.second << std::endl;
            if ( all_species[i] == elm.second )
            {
               //std::cout << "And they are the same..... " <<std::endl;
               exist = 1; 
            }
            idx++;  
        }
        if (!exist)
        {
            std::cout<< std::endl;
            std::cout << "ERROR: The specified SBML species in the name of \"" << elm.second << "\" at substrate species. Please take a look SBML species specifications." << std::endl;
            std::cout<< std::endl;
            std::cout<< std::endl;
            exit (-1);
            return -1;
        }
        //std::cout << "existence check : " << elm.second <<": " << exist << std::endl;
    }    
    
    
    //std::cout << "---------VALIDATING_SBML_SPECIES END-------" << std::endl;  
    
    
    std::cout << "---- Specified SBML species at config file are validated. ----- " << std::endl;
    return 0;
}

int RoadRunnerIntracellular::create_custom_data_for_SBML(PhysiCell::Phenotype& phenotype)
{
    //std::cout << "Test" << std::endl;
    
    return 0; 
}
