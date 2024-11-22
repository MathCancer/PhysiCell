#include "./start_and_stop.h"
#include <algorithm>
#include <cassert>

using namespace std; 

// reset microenv
void reset_microenv(){
		
	string filename = "start_and_stop_saving_files/microenv_data.txt";

    // Open the file for input
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
	
    // Call the operator>> function to read data from the file into the Microenvironment instance
    input_file >> microenvironment;

    // Close the file
    input_file.close();
	return;
}

// Function to save cell data
void save_cell_microenv_data(Cell_Container* cell_container)
{
	
	// decide if restart from a previous point or from the beginning
	string filename_cells = "start_and_stop_saving_files/cell_data.txt";
	string filename_bool = "start_and_stop_saving_files/bool_data.txt";
	string filename_microenv = "start_and_stop_saving_files/microenv_data.txt";
	string cell_pos_filename = "start_and_stop_saving_files/initial.tsv";
	string global_param_filename = "start_and_stop_saving_files/global_param.txt";
	string random_counters_filename = "start_and_stop_saving_files/random_counters.txt";

	// Open the files in write mode
	ofstream file_cells(filename_cells);
	ofstream file_bool(filename_bool);
	ofstream file_microenv(filename_microenv);
	ofstream cell_pos_file(cell_pos_filename);
	ofstream global_param_file(global_param_filename);
	ofstream random_counters_file(random_counters_filename);

	// Check if the files are successfully opened
	if (!file_cells.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << filename_cells << endl;
		return;
	}
	if (!file_bool.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << filename_bool << endl;
		return;
	}
	if (!file_microenv.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << filename_microenv << endl;
		return;
	}
	if (!cell_pos_file.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << cell_pos_filename << endl;
		return;
	}
	if (!global_param_file.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << global_param_filename << endl;
		return;
	}
	if (!random_counters_file.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << random_counters_filename << endl;
		return;
	}
	
	// display microenvironment

	file_microenv << microenvironment;

	// add header to pos file
	cell_pos_file << "x	y	z" << endl;

	// Iterate through all cells
	for (int i = 0; i < (*all_cells).size(); i++)
	{
		// Access the current cell
		Cell *pCell = (*all_cells)[i];

		// Write the cell data to the file using the insertion operator
		file_cells << *pCell << endl;

		if (pCell->phenotype.intracellular){
			//save parameters maboss_intracellular
			pCell->phenotype.intracellular->save_current_parameters(file_cells);

			//save parameters maboss network
			pCell->phenotype.intracellular->save_current_parameters_maboss(file_cells);
		}

		// current_voxel_index
		file_cells << "current_voxel_index: " << pCell->get_current_voxel_index() << std::endl;

		// nearest_density_vector
		file_cells << "nearest_density_vector: " << pCell->nearest_density_vector() << std::endl;
		

		// empty line
		file_cells << std::endl;

		// Write the positions in the cell file
		cell_pos_file << pCell->position[0] << "	";
		cell_pos_file << pCell->position[1] << "	";
		cell_pos_file << pCell->position[2];

		
		if (pCell->phenotype.intracellular){
			// Write the output for Boolean network
			// iterate among the nodes and write the name of all the active nodes for each cell, a row = a cell.
			pCell->phenotype.intracellular->save_current_nodes(file_bool);
			
			

			// Write newline except for the last cell
			if (i != (*all_cells).size() - 1) {
				cell_pos_file << "\n";
				file_bool << "\n";
			}
		}
		
		
	}
	// Save global parameters and cell container
	global_param_file << "current_time: " << PhysiCell_globals.current_time << endl;
	global_param_file << "full_output_index: " <<PhysiCell_globals.full_output_index << endl;
	global_param_file << "intracellular_output_index: " <<PhysiCell_globals.intracellular_output_index << endl;
	global_param_file << "next_full_save_time: " <<PhysiCell_globals.next_full_save_time << endl;
	global_param_file << "next_intracellular_save_time: " <<PhysiCell_globals.next_intracellular_save_time << endl;
	global_param_file << "next_SVG_save_time: " <<PhysiCell_globals.next_SVG_save_time << endl;
	global_param_file << "SVG_output_index: " <<PhysiCell_globals.SVG_output_index << std::endl;
	global_param_file << "diffusion_dt: " << diffusion_dt << std::endl;
	global_param_file << "mechanics_dt: " << mechanics_dt << std::endl;
	global_param_file << "phenotype_dt: " << phenotype_dt << std::endl;
	if (((*all_cells)[0])->phenotype.intracellular){
		global_param_file << "intracellular_dt: " << intracellular_dt << std::endl;
	}
	cell_container->save_data(global_param_file);
	
	//save random counters
	save_counters(random_counters_file);

	//close all the files
	file_cells.close();
	file_bool.close();
	file_microenv.close();
	global_param_file.close();
	random_counters_file.close();

	return;
}
using namespace std;


void reset_cell(double last_cell_cycle_time)
{

	// Specify the path to the input files
	std::string input_file_cells_path = "start_and_stop_saving_files/cell_data.txt";
	std::string input_file_bool_path = "start_and_stop_saving_files/bool_data.txt";

	// Open the files in input mode using std::ifstream
	std::ifstream input_file_cells(input_file_cells_path);
	std::ifstream input_file_bool(input_file_bool_path);



	// Check if the files was opened successfully
	if (!input_file_cells.is_open())
	{
		std::cerr << "Error opening the file: " << input_file_cells_path << std::endl;
		return;
	}
	if (!input_file_bool.is_open())
	{
		std::cerr << "Error opening the file: " << input_file_bool_path << std::endl;
		return;
	}

	// Iterate through all cells
	for (int i = 0; i < (*all_cells).size(); i++)
	{
	  // Use the extraction operator >> to read data from the file line into the cell

		// Access the current cell
		Cell *pCell = (*all_cells)[i];
		//read header
		string header;
		getline(input_file_cells, header);
		// ID:
		getline(input_file_cells, header);
		pCell->ID  = read_number_in_line_int(header);
		// index:
		getline(input_file_cells, header);
		pCell->index = read_number_in_line_int(header);
		// type:
		getline(input_file_cells, header);
		pCell->type = read_number_in_line_int(header);
		
		// is_active:
		std::getline(input_file_cells, header);
		pCell->is_active = read_number_in_line_bool(header);
		// type_name
		getline(input_file_cells >> ws, pCell->type_name);
		// is_out_of_domain
		std::getline(input_file_cells, header);
		pCell->is_out_of_domain = read_number_in_line_bool(header);
		
		// is_movable
		std::getline(input_file_cells, header);
		pCell->is_movable = read_number_in_line_bool(header);
		
		// state
        input_file_cells >> pCell->state;
		
		
		//parameters
		input_file_cells >> pCell->parameters;

		

		string line;
		
		// flagged_for_division:
		std::getline(input_file_cells, line);
		pCell->phenotype.flagged_for_division = read_number_in_line_bool(line);
		

		// flagged_for_removal:
		std::getline(input_file_cells, line);
		pCell->phenotype.flagged_for_removal = read_number_in_line_bool(line);
		

		//death
		input_file_cells >> pCell->phenotype.death;

		if(pCell->phenotype.death.dead == true){
			pCell->phenotype.cycle.sync_to_cycle_model(pCell->phenotype.death.current_model());
		}
		//cycle
		input_file_cells >> pCell->phenotype.cycle;
		if( pCell->phenotype.cycle.current_phase().entry_function )
		{
			pCell->phenotype.cycle.current_phase().entry_function( pCell, pCell->phenotype,  PhysiCell_globals.current_time - last_cell_cycle_time); 
		}

		//volume
		input_file_cells >> pCell->phenotype.volume;
		pCell->set_total_volume(pCell->phenotype.volume.total);
		//geometry
        input_file_cells >> pCell->phenotype.geometry;
		//mechanics
        input_file_cells >> pCell->phenotype.mechanics;
		//motility
        input_file_cells >> pCell->phenotype.motility;

		//secretion
        input_file_cells >> pCell->phenotype.secretion;
		
		//molecular
        input_file_cells >> pCell->phenotype.molecular;

		//Cell_Interactions
		input_file_cells >> pCell->phenotype.cell_interactions;

		//Cell_Transformations
		input_file_cells >> pCell->phenotype.cell_transformations;

		//custom_data 
		input_file_cells >> pCell->custom_data;

		getline(input_file_cells, header);
		getline(input_file_cells, header);

		//intracellular
		if (pCell->phenotype.intracellular){
			pCell->phenotype.intracellular->read_current_parameter(input_file_cells);

			pCell->phenotype.intracellular->read_current_parameter_maboss(input_file_cells);

			std::string current_bnd_filename;
			std::string current_cfg_filename;

			// load and parse settings file(s)
			pugi::xml_document physicell_xml;
			pugi::xml_node node;
			pugi::xml_parse_result result = physicell_xml.load_file( "./config/PhysiCell_settings.xml"  );

			if( result.status != pugi::xml_parse_status::status_ok )
			{
				std::cout << "Error loading " << "./config/PhysiCell_settings.xml" << "!" << std::endl; 
			}

			// navigate to the desired value
			pugi::xml_node physicell_settings = physicell_xml.child("PhysiCell_settings");
			pugi::xml_node cell_defs = physicell_settings.child("cell_definitions");

			for (pugi::xml_node cell_def = cell_defs.child("cell_definition"); cell_def; cell_def = cell_def.next_sibling("cell_definition"))
			{
				if (std::string(cell_def.attribute("name").value()) == pCell->type_name)
				{
					pugi::xml_node phenotype = cell_def.child("phenotype");

					pugi::xml_node intracellular = phenotype.child("intracellular");

					if(intracellular && std::string(intracellular.attribute("type").value()) == "maboss")
					{
						pugi::xml_node bnd_filename = intracellular.child("bnd_filename");
						pugi::xml_node cfg_filename = intracellular.child("cfg_filename");

						current_bnd_filename = bnd_filename.text().get();
						current_cfg_filename = cfg_filename.text().get();
						
					}
				}
			}
			// reinit maboss
			pCell->phenotype.intracellular->reinit_maboss(current_bnd_filename, current_cfg_filename);
		}

		// current_voxel_index
		std::getline(input_file_cells, header);

		// nearest_density_vector
		std::getline(input_file_cells, header);

		// empty line
		std::getline(input_file_cells, header);
		
		if (pCell->phenotype.intracellular){
			// Boolean network reinitialization
			string dummy;
			getline(input_file_bool, dummy); 

			// Use a stringstream to extract tokens from the line
			istringstream iss(dummy);

			// Loop to extract node names and values from the line
			string token;
			unsigned int n_line = 0;
			while (getline(iss, token, ' ')) { // Read the string until the ';' character
				// Find the position of the '=' character
				size_t pos = token.find('=');
				if (pos != string::npos) { // If the '=' character is found
					// Extract the node name and its value from the substring
					string node_name = token.substr(0, pos);
					string node_value_str = token.substr(pos + 1);
					bool node_value = (node_value_str == "1");
					
					pCell->phenotype.intracellular->set_boolean_variable_value(node_name, node_value);
					n_line++;
					
				}
				
				
			}
			// Check if the number of nodes is the same

			assert(n_line == pCell->phenotype.intracellular->get_number_of_nodes());
		}
		
	}
		input_file_cells.close();
		input_file_bool.close();
		return;

    }

	void reset_global_parameters(Cell_Container* cell_container)
	{
	string global_param_filename = "start_and_stop_saving_files/global_param.txt";

	ifstream global_param_file(global_param_filename);

	if (!global_param_file.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << global_param_filename << endl;
		return;
	}

	// current_time:
	string line_current_time;
	getline(global_param_file, line_current_time);
	PhysiCell_globals.current_time = read_number_in_line(line_current_time);

	// full_output_index:
	string line_full_output_index;
	getline(global_param_file, line_full_output_index);
	PhysiCell_globals.full_output_index = read_number_in_line_int(line_full_output_index);

	// intracellular_output_index:
	string line_intracellular_output_index;
	getline(global_param_file, line_intracellular_output_index);
	PhysiCell_globals.intracellular_output_index = read_number_in_line_int(line_intracellular_output_index);

	// next_full_save_time:
	string line_next_full_save_time;
	getline(global_param_file, line_next_full_save_time);
	PhysiCell_globals.next_full_save_time = read_number_in_line(line_next_full_save_time);

	// next_intracellular_save_time:
	string line_next_intracellular_save_time;
	getline(global_param_file, line_next_intracellular_save_time);
	PhysiCell_globals.next_intracellular_save_time = read_number_in_line(line_next_intracellular_save_time);

	// next_SVG_save_time:
	string line_next_SVG_save_time;
	getline(global_param_file, line_next_SVG_save_time);
	PhysiCell_globals.next_SVG_save_time = read_number_in_line(line_next_SVG_save_time);
    
	// SVG_output_index:
	string line_SVG_output_index;
	getline(global_param_file, line_SVG_output_index);
	PhysiCell_globals.SVG_output_index = read_number_in_line_int(line_SVG_output_index);

	// diffusion_dt:
	string line_diffusion_dt;
	getline(global_param_file, line_diffusion_dt);
	diffusion_dt = read_number_in_line(line_diffusion_dt);

	// diffusion_dt:
	string line_mechanics_dt;
	getline(global_param_file, line_mechanics_dt);
	mechanics_dt = read_number_in_line(line_mechanics_dt);

	// phenotype_dt:
	string line_phenotype_dt;
	getline(global_param_file, line_phenotype_dt);
	phenotype_dt = read_number_in_line(line_phenotype_dt);

	// intracellular_dt:
	if (((*all_cells)[0])->phenotype.intracellular){
		string line_intracellular_dt;
		getline(global_param_file, line_intracellular_dt);
		intracellular_dt = read_number_in_line(line_intracellular_dt);
	}

	//reset cell_container
	cell_container->reset_data(global_param_file);

	// Close the file after finishing reading
    global_param_file.close();

	return;
	}

	void reset_randomness()
	{

	string random_counters_filename = "start_and_stop_saving_files/random_counters.txt";

	ifstream random_counters_file(random_counters_filename);

	if (!random_counters_file.is_open())
	{
		// Handle the case where the file cannot be opened
		cerr << "Error opening file " << random_counters_filename << endl;
		return;
	}

			// counter_double_random:
	string line_counter_double_random;
	getline(random_counters_file, line_counter_double_random);
	set_counter_double_random(read_number_in_line(line_counter_double_random));

	// counter_int_random:
	string line_counter_int_random;
	getline(random_counters_file, line_counter_int_random);
	set_counter_int_random(read_number_in_line(line_counter_int_random));

	// counter_normal_random:
	string line_counter_normal_random;
	getline(random_counters_file, line_counter_normal_random);
	set_counter_normal_random(read_number_in_line(line_counter_normal_random));

	random_counters_file.close();

	}
