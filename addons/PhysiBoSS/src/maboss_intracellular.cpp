#include "maboss_intracellular.h"

MaBoSSIntracellular::MaBoSSIntracellular() : Intracellular()
{
	intracellular_type = "maboss";
	initial_values.clear();
	mutations.clear();
	parameters.clear();
	listOfInputs.clear();
	listOfOutputs.clear();
}

MaBoSSIntracellular::MaBoSSIntracellular(pugi::xml_node& node)
{
	intracellular_type = "maboss";
	initial_values.clear();
	mutations.clear();
	parameters.clear();
	listOfInputs.clear();
	listOfOutputs.clear();
	initialize_intracellular_from_pugixml(node);
}

MaBoSSIntracellular::MaBoSSIntracellular(MaBoSSIntracellular* copy) 
{
	intracellular_type = copy->intracellular_type;
	bnd_filename = copy->bnd_filename;
	cfg_filename = copy->cfg_filename;
	time_step = copy->time_step;
	discrete_time = copy->discrete_time;
	time_tick = copy->time_tick;
	scaling = copy->scaling;
	time_stochasticity = copy->time_stochasticity;
	inherit_state = copy->inherit_state;
	inherit_nodes = copy->inherit_nodes;
	initial_values = copy->initial_values;
	mutations = copy->mutations;
	parameters = copy->parameters;
	indicesOfInputs = copy->indicesOfInputs;
	indicesOfOutputs = copy->indicesOfOutputs;
	listOfInputs = copy->listOfInputs;
	listOfOutputs = copy->listOfOutputs;
	
	if (copy->maboss.has_init()) {
		maboss.init_maboss(copy->bnd_filename, copy->cfg_filename);
		maboss.mutate(mutations);
		maboss.set_initial_values(initial_values);
		maboss.set_parameters(parameters);
		maboss.set_update_time_step(copy->time_step);
		maboss.set_discrete_time(copy->discrete_time, copy->time_tick);
		maboss.set_scaling(copy->scaling);
		maboss.set_time_stochasticity(copy->time_stochasticity);
		maboss.restart_node_values();
		indicesOfInputs.clear();
		for (const auto& input: listOfInputs) {
			indicesOfInputs.push_back(PhysiCell::find_signal_index(input.second.physicell_name));
		}
		indicesOfOutputs.clear();
		for (const auto& output: listOfOutputs) {
			indicesOfOutputs.push_back(PhysiCell::find_behavior_index(output.second.physicell_name));
		}
	}	
}

void MaBoSSIntracellular::update_inputs(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt)
{
	std::vector<double> signals = PhysiCell::get_selected_signals(cell, indicesOfInputs);
	
	int i=0;
	for (auto& input: listOfInputs) 
	{
		if (input.second.isNode()) {
			maboss.set_node_value(
				input.first,
				input.second.updateNode(maboss.get_node_value(input.second.intracellular_name), signals[i])
			);
		} else if (input.second.isParameter()) {
			maboss.set_parameter_value(
				input.first,
				input.second.updateParameter(signals[i])
			);
		}
		i++;
	}
}

void MaBoSSIntracellular::update_outputs(PhysiCell::Cell* cell, PhysiCell::Phenotype& phenotype, double dt)
{
	std::vector<double> signals = std::vector<double>(listOfOutputs.size(), 0.0);
	
	int i=0;
	for (auto& output: listOfOutputs) 
	{
		signals[i] = output.second.update(maboss.get_node_value(output.second.intracellular_name));
		i++;
	}
	PhysiCell::set_selected_behaviors(cell, indicesOfOutputs, signals);
}


void MaBoSSIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_bnd = node.child( "bnd_filename" );
	if ( node_bnd )
	{ bnd_filename = PhysiCell::xml_get_my_string_value (node_bnd); }
	else 
	{ std::cerr << "Error : No BND model file defined !" << std::endl; exit(-1); }
	
	pugi::xml_node node_cfg = node.child( "cfg_filename" );
	if ( node_cfg )
	{ cfg_filename = PhysiCell::xml_get_my_string_value (node_cfg); }
	else 
	{ std::cerr << "Error : No CFG model file defined !" << std::endl; exit(-1); }
	
	// Setting all the rest to default values : Nothing should be kept from the existing intracellular object (NO INHERITANCE)
	time_step = 12;
	discrete_time = false;
	time_tick = 0.5;
	scaling = 1.0;
	time_stochasticity = 0.0;
	inherit_state = false;
	start_time = 0.0;
	initial_values.clear();
	mutations.clear();
	parameters.clear();
	inherit_nodes.clear();
	listOfInputs.clear();
	indicesOfInputs.clear();
	listOfOutputs.clear();
	indicesOfOutputs.clear();
	
	pugi::xml_node node_init_values = node.child( "initial_values" );
	if( node_init_values )
	{
		pugi::xml_node node_init_value = node_init_values.child( "initial_value" );
		while( node_init_value )
		{
			pugi::xml_attribute node_name = node_init_value.attribute( "node" );
			pugi::xml_attribute node_intracellular_name = node_init_value.attribute( "intracellular_name" ); 
			double node_value = PhysiCell::xml_get_my_double_value( node_init_value );
			
			if (node_intracellular_name) {
				initial_values[node_intracellular_name.value()] = node_value;
			} else if (node_name) {
				std::cout << "The attribute node in mutation is deprecated and will be removed in future versions. Please switch to intracellular_name !" << std::endl;
				initial_values[node_name.value()] = node_value;
			}
			node_init_value = node_init_value.next_sibling( "initial_value" ); 
		}
	}

	maboss.init_maboss(bnd_filename, cfg_filename);
	maboss.set_initial_values(initial_values);

	pugi::xml_node node_settings = node.child( "settings" );
	if ( node_settings ) {

		pugi::xml_node node_mutations = node_settings.child( "mutations" );
		if( node_mutations )
		{
			pugi::xml_node node_mutation = node_mutations.child( "mutation" );
			while( node_mutation )
			{
				pugi::xml_attribute node_name = node_mutation.attribute( "node" ); 
				pugi::xml_attribute node_intracellular_name = node_mutation.attribute( "intracellular_name" ); 
				
				double node_value = PhysiCell::xml_get_my_double_value( node_mutation );
					
				if (node_intracellular_name) {
					mutations[node_intracellular_name.value()] = node_value;
				} else if (node_name) {
					std::cout << "The attribute node in mutation is deprecated and will be removed in future versions. Please switch to intracellular_name !" << std::endl;
					mutations[node_name.value()] = node_value;
				}

				node_mutation = node_mutation.next_sibling( "mutation" ); 
			}
		}
		
		maboss.mutate(mutations);

		pugi::xml_node node_parameters = node_settings.child( "parameters" );
		if( node_parameters )
		{
			pugi::xml_node node_parameter = node_parameters.child( "parameter" );
			while( node_parameter )
			{
				pugi::xml_attribute param_name = node_parameter.attribute( "name" ); 
				pugi::xml_attribute param_intracellular_name = node_parameter.attribute( "intracellular_name" ); 
				double param_value = PhysiCell::xml_get_my_double_value( node_parameter );
				
				if (param_intracellular_name) {
					parameters[param_intracellular_name.value()] = param_value;
				} else if (param_name) {
					std::cout << "The attribute name in parameter is deprecated and will be removed in future versions. Please switch to intracellular_name !" << std::endl;
					parameters[param_name.value()] = param_value;
				}
				node_parameter = node_parameter.next_sibling( "parameter" ); 
			}
		}

		maboss.set_parameters(parameters);	

		pugi::xml_node node_timestep = node_settings.child( "time_step" ); 
		pugi::xml_node node_intracellular_dt = node_settings.child( "intracellular_dt" ); 
		if( node_intracellular_dt )
		{ 
			time_step = PhysiCell::xml_get_my_double_value( node_intracellular_dt );
			maboss.set_update_time_step(time_step);

		} else if ( node_timestep ) 
		{
			std::cout << "The setting timestep in parameter is deprecated and will be removed in future versions. Please switch to intracellular_name !" << std::endl;
			time_step = PhysiCell::xml_get_my_double_value( node_timestep );
			maboss.set_update_time_step(time_step);
		}
		
		pugi::xml_node node_discretetime = node_settings.child( "discrete_time" ); 
		pugi::xml_node node_timetick = node_settings.child( "time_tick" ); 

		if( node_discretetime && node_timetick )
		{ 
			discrete_time = PhysiCell::xml_get_my_bool_value( node_discretetime );		
			time_tick = PhysiCell::xml_get_my_double_value( node_timetick );
			maboss.set_discrete_time(discrete_time, time_tick);
		}

		pugi::xml_node node_scaling = node_settings.child( "scaling" ); 
		if( node_scaling )
		{ 
			scaling = PhysiCell::xml_get_my_double_value( node_scaling );
			maboss.set_scaling(scaling);
		}

		pugi::xml_node node_time_stochasticity = node_settings.child( "time_stochasticity" );
		if( node_time_stochasticity )
		{
			time_stochasticity = PhysiCell::xml_get_my_double_value( node_time_stochasticity );
			maboss.set_time_stochasticity(time_stochasticity);
		}

		pugi::xml_node node_inheritance = node_settings.child( "inheritance" );
		if( node_inheritance )
		{
			pugi::xml_attribute global_inheritance = node_inheritance.attribute( "global" ); 
			inherit_state = global_inheritance.as_bool();
			
			pugi::xml_node node_inherit_node = node_inheritance.child( "inherit_node" );
			while( node_inherit_node )
			{
				pugi::xml_attribute node_inherit_intracellular_name = node_inherit_node.attribute( "intracellular_name" ); 
				bool inherit_value = PhysiCell::xml_get_my_bool_value( node_inherit_node );
				inherit_nodes[node_inherit_intracellular_name.value()] = inherit_value;
				
				node_inherit_node = node_inheritance.next_sibling( "inherit_node" ); 
			}

		}

		pugi::xml_node node_start_time = node_settings.child( "start_time" );
		if( node_start_time )
		{
			start_time = PhysiCell::xml_get_my_double_value( node_start_time );
		}
	
		}
	
	// Old structure, with deprecation warnings

	pugi::xml_node node_timestep = node.child( "time_step" ); 
	pugi::xml_node node_intracellular_dt = node.child( "intracellular_dt" ); 
	if( node_intracellular_dt )
	{ 
		std::cout << "The intracellular_dt needs to be defined inside the settings tag. Please update your settings as this will become incompatible in future versions" << std::endl;
		time_step = PhysiCell::xml_get_my_double_value( node_intracellular_dt );
		maboss.set_update_time_step(time_step);

	} else if ( node_timestep ) 
	{
		std::cout << "The setting timestep in parameter is deprecated and will be removed in future versions. Please update your settings using intracellular_dt !" << std::endl;
		time_step = PhysiCell::xml_get_my_double_value( node_timestep );
		maboss.set_update_time_step(time_step);
	}
	
	pugi::xml_node node_discretetime = node.child( "discrete_time" ); 
	pugi::xml_node node_timetick = node.child( "time_tick" ); 

	if( node_discretetime && node_timetick )
	{ 
		std::cout << "The discrete_time needs to be defined inside the settings tag. Please update your settings as this will become incompatible in future versions" << std::endl;
		std::cout << "The time_tick needs to be defined inside the settings tag. Please update your settings as this will become incompatible in future versions" << std::endl;
		discrete_time = PhysiCell::xml_get_my_bool_value( node_discretetime );		
		time_tick = PhysiCell::xml_get_my_double_value( node_timetick );
		maboss.set_discrete_time(discrete_time, time_tick);
	}

	pugi::xml_node node_scaling = node.child( "scaling" ); 
	if( node_scaling )
	{ 
		std::cout << "The scaling needs to be defined inside the settings tag. Please update your settings as this will become incompatible in future versions" << std::endl;
		scaling = PhysiCell::xml_get_my_double_value( node_scaling );
		maboss.set_scaling(scaling);
	}

	pugi::xml_node node_time_stochasticity = node.child( "time_stochasticity" );
	if( node_time_stochasticity )
	{
		std::cout << "The time stochasticity needs to be defined inside the settings tag. Please update your settings as this will become incompatible in future versions" << std::endl;
		time_stochasticity = PhysiCell::xml_get_my_double_value( node_time_stochasticity );
		maboss.set_time_stochasticity(time_stochasticity);
	}


	pugi::xml_node node_mutations = node.child( "mutations" );
	if( node_mutations )
	{
		std::cout << "The mutant declaration is now defined the the settings tag. Please update your settings as this will be incompatible in future versions !" << std::endl;
		pugi::xml_node node_mutation = node_mutations.child( "mutation" );
		while( node_mutation )
		{
			pugi::xml_attribute node_name = node_mutation.attribute( "node" ); 
			
			double node_value = PhysiCell::xml_get_my_double_value( node_mutation );
				

			mutations[node_name.value()] = node_value;

			node_mutation = node_mutation.next_sibling( "mutation" ); 
		}
	}
	
	maboss.mutate(mutations);

	pugi::xml_node node_parameters = node.child( "parameters" );
	if( node_parameters )
	{
		std::cout << "The parameters declaration is now defined the the settings tag. Please update your settings as this will be incompatible in future versions !" << std::endl;
		pugi::xml_node node_parameter = node.child( "parameter" );
		while( node_parameter )
		{
			pugi::xml_attribute param_name = node_parameter.attribute( "name" ); 
			double param_value = PhysiCell::xml_get_my_double_value( node_parameter );
		
			parameters[param_name.value()] = param_value;
			node_parameter = node_parameter.next_sibling( "parameter" ); 
		}
	}

	maboss.set_parameters(parameters);	

	// Mappings

	pugi::xml_node node_mappings = node.child( "mapping" );
	if( node_mappings )
	{

		pugi::xml_node node_input = node_mappings.child("input");
		while (node_input) 
		{
			pugi::xml_node settings = node_input.child("settings");

			std::string intracellular_name = node_input.attribute( "intracellular_name" ).value();
			if (intracellular_name[0] == '$') {

				MaBoSSInput input = MaBoSSInput(
					node_input.attribute( "physicell_name" ).value(),
					intracellular_name,
					(settings && settings.child( "scaling" ) ? PhysiCell::xml_get_my_double_value( settings.child( "scaling" )) : 1.0),
					(settings && settings.child( "smoothing" ) ? PhysiCell::xml_get_my_int_value( settings.child( "smoothing" )) : 0)
				);

				// This construct is a trick to avoid making inputs and outputs constructible and assignable, or using c++17 insert_or_assign
				// Using the same construct below
				auto const res = listOfInputs.insert(std::pair<std::string, MaBoSSInput>(intracellular_name, input));
				if (!(res.second)) { res.first->second = input; }
				
			} else {
				
				MaBoSSInput input = MaBoSSInput(
					node_input.attribute( "physicell_name" ).value(),
					intracellular_name,
					PhysiCell::xml_get_my_string_value(settings.child("action")),
					PhysiCell::xml_get_my_double_value(settings.child("threshold")),
					(settings && settings.child( "inact_threshold" ) ? PhysiCell::xml_get_my_double_value( settings.child( "inact_threshold" )) : PhysiCell::xml_get_my_double_value(settings.child("threshold"))),
					(settings && settings.child( "smoothing" ) ? PhysiCell::xml_get_my_int_value( settings.child( "smoothing" )) : 0)
				);

				auto const res = listOfInputs.insert(std::pair<std::string, MaBoSSInput>(intracellular_name, input));
				if (!(res.second)) { res.first->second = input; }
			}

			node_input = node_input.next_sibling( "input" ); 
		}
		
		pugi::xml_node node_output = node_mappings.child("output");
		while (node_output) 
		{
			pugi::xml_node settings = node_output.child("settings");
			std::string physicell_name = node_output.attribute( "physicell_name" ).value();
			MaBoSSOutput output = MaBoSSOutput(
				physicell_name,
				node_output.attribute( "intracellular_name" ).value(),
				PhysiCell::xml_get_my_string_value(settings.child("action")),
				PhysiCell::xml_get_my_double_value(settings.child("value")),
				(settings && settings.child( "base_value" ) ? PhysiCell::xml_get_my_double_value( settings.child( "base_value" )) : PhysiCell::xml_get_my_double_value(settings.child("value"))),
				(settings && settings.child( "smoothing" ) ? PhysiCell::xml_get_my_int_value( settings.child( "smoothing" )) : 0),
				(settings && settings.child( "steepness" ) ? PhysiCell::xml_get_my_int_value( settings.child( "steepness" )) : 10)
			);

			auto const res = listOfOutputs.insert(std::pair<std::string, MaBoSSOutput>(physicell_name, output));
			if (!(res.second)) { res.first->second = output; }
			node_output = node_output.next_sibling( "output" ); 	
		}
	}
}

MaBoSSIntracellular* getMaBoSSModel(PhysiCell::Phenotype& phenotype) {
	return static_cast<MaBoSSIntracellular*>(phenotype.intracellular);
}

void MaBoSSIntracellular::display(std::ostream& os)
{
	os 	<< "\tintracellular model using maboss" << std::endl
		<< "\t\t model bnd : " << bnd_filename << std::endl
		<< "\t\t model cfg : " << cfg_filename << std::endl
		<< "\t\t dt = " << time_step << std::endl
		<< "\t\t " << initial_values.size() << " initial values override" << std::endl;
	for (auto& initial_value : initial_values)
		os << "\t\t\t" << initial_value.first << " = " << initial_value.second << std::endl;
	
	os	<< "\t\t " << parameters.size() << " parameters override" << std::endl;
	for (auto& parameter : parameters)
		os << "\t\t\t" << parameter.first << " = " << parameter.second << std::endl;
	
	os	<< "\t\t " << mutations.size() << " mutations override" << std::endl;
	for (auto& mutation : mutations)
		os << "\t\t\t" << mutation.first << " = " << mutation.second << std::endl;

	os 	<< "\t\t scaling = " << scaling << std::endl
		<< "\t\t time_stochasticity = " << time_stochasticity << std::endl
		<< "\t\t start_time = " << start_time << std::endl;

	os	<< "\t\t " << listOfInputs.size() << " input mapping defined" << std::endl;
	for (const auto& input : listOfInputs)
		os 	<< "\t\t\t" << input.second.physicell_name << " = " << input.first
			<< "(" << input.second.threshold << ", " << input.second.inact_threshold << ", " << input.second.smoothing << ")"
			<< std::endl;

	os	<< "\t\t " << listOfOutputs.size() << " output mapping defined" << std::endl;
	for (const auto& output : listOfOutputs)
		os 	<< "\t\t\t" << output.first << " = " << output.second.intracellular_name 
			<< "(" << output.second.value << ", " << output.second.base_value 
			<< ", " << output.second.smoothing << ", " << output.second.steepness << ")"
			<< std::endl;

	os 	<< "\t\t global inheritance = " << inherit_state << std::endl;
	os	<< "\t\t " << inherit_nodes.size() << " node-specific inheritance defined" << std::endl;
	for (const auto& node_inheritance : inherit_nodes)
		os 	<< "\t\t\t" << node_inheritance.first << " = " << node_inheritance.second
			<< std::endl;

	std::cout << std::endl;
}

void MaBoSSIntracellular::save(std::string filename, std::vector<PhysiCell::Cell*>& cells)
{
	std::ofstream state_file( filename );
	
	state_file << "ID,state" << std::endl;

	for( auto cell : cells )
		if (cell->phenotype.intracellular != NULL)
			state_file << cell->ID << "," << static_cast<MaBoSSIntracellular*>(cell->phenotype.intracellular)->get_state() << std::endl;
	
	state_file.close();
}