#include "maboss_intracellular.h"

MaBoSSIntracellular::MaBoSSIntracellular() : Intracellular()
{
	intracellular_type = "maboss";
	initial_values.clear();
	mutations.clear();
	parameters.clear();
}

MaBoSSIntracellular::MaBoSSIntracellular(pugi::xml_node& node)
{
	intracellular_type = "maboss";
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
	initial_values = copy->initial_values;
	mutations = copy->mutations;
	parameters = copy->parameters;
	
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
		//maboss.set_state(copy->maboss.get_maboss_state());
		//std::cout << get_state();
	}	
}

void MaBoSSIntracellular::initialize_intracellular_from_pugixml(pugi::xml_node& node)
{
	pugi::xml_node node_bnd = node.child( "bnd_filename" );
	if ( node_bnd )
	{ bnd_filename = PhysiCell::xml_get_my_string_value (node_bnd); }
	
	pugi::xml_node node_cfg = node.child( "cfg_filename" );
	if ( node_cfg )
	{ cfg_filename = PhysiCell::xml_get_my_string_value (node_cfg); }
	
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
		<< std::endl;
}

void MaBoSSIntracellular::save(std::string filename, std::vector<PhysiCell::Cell*>& cells)
{
					
	std::ofstream state_file( filename );
	
	state_file << "ID,state" << std::endl;

	for( auto cell : cells )
		state_file << cell->ID << "," << static_cast<MaBoSSIntracellular*>(cell->phenotype.intracellular)->get_state() << std::endl;
		
	state_file.close();

}