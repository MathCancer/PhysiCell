#include "maboss_network.h"
#include <fstream>

/* Default constructor */
void MaBoSSNetwork::init_maboss( std::string networkFile, std::string configFile)
{
	if (this->network != NULL) {
		delete this->network;
	}
	
	if (this->config != NULL) {
		delete this->config;
	}
	
	if (this->engine != NULL) {
		delete this->engine;
	}
	
	try{
		
		#pragma omp critical
		{
			std::ifstream f_bnd(networkFile.c_str());
			if (!f_bnd.good()) {
				std::cerr << "PhysiBoSS ERROR : Could not open the BND file " << networkFile.c_str() << std::endl;
				exit(1);
			}
			
			std::ifstream f_cfg(configFile.c_str());
			if (!f_cfg.good()) {
				std::cerr << "PhysiBoSS ERROR : Could not open the CFG file " << configFile.c_str() << std::endl;
				exit(1);
			}
			
			// Initialize MaBoSS Objects for a model
			this->network = new Network();
			this->network->parse(networkFile.c_str());

			this->config = new RunConfig();
			this->config->parse(this->network, configFile.c_str());
		}
		
		// Some models will have chosen to use the physical randon number generator 
		// This is a problem, as it will open /dev/urandom for each cell, and overload the number of file open
		// So for now we just don't use this, and choose by default mersen twister
		this->config->setParameter("use_physrandgen", false);
		this->config->setParameter("use_mtrandgen", true);
		
		IStateGroup::checkAndComplete(this->network);

		engine = new StochasticSimulationEngine(this->network, this->config, PhysiCell::UniformInt());
	
	} catch (BNException e) {
		std::cerr << "MaBoSS ERROR : " << e.getMessage() << std::endl;
		exit(1);
	}
	this->update_time_step = this->config->getMaxTime();
	
	// Building map of nodes for fast later access 
	for (auto node : this->network->getNodes()) {
		this->nodesByName[node->getLabel()] = node;
	}
	
	// Building map of parameters for fast later access
	for (auto parameter : this->network->getSymbolTable()->getSymbolsNames()) {
		if (parameter[0] == '$')
			this->parametersByName[parameter] = this->network->getSymbolTable()->getSymbol(parameter);
	}
	
	for (auto node : network->getNodes())
      if (!node->isInternal()) 
        output_mask.setNodeState(node, true);

}

void MaBoSSNetwork::mutate(std::map<std::string, double> mutations) 
{
	for (auto mutation : mutations) {
		if (nodesByName.find(mutation.first) != nodesByName.end())
			nodesByName[mutation.first]->mutate(mutation.second);
		else{
			std::cerr << "Mutation set for unknown node : can't find node " << mutation.first << std::endl;
			exit(1);
		}
	}
}

void MaBoSSNetwork::set_parameters(std::map<std::string, double> parameters) 
{	
	for (auto parameter: parameters) {
		set_parameter_value(parameter.first, parameter.second);
	}
}

double MaBoSSNetwork::get_parameter_value(std::string name) 
{
	return network->getSymbolTable()->getSymbolValue(parametersByName[name]);
}


void MaBoSSNetwork::set_parameter_value(std::string name, double value) 
{
	network->getSymbolTable()->setSymbolValue(parametersByName[name], value);
	network->getSymbolTable()->unsetSymbolExpressions();
}

/* Reset a vector of bools to the init state of the network */
void MaBoSSNetwork::restart_node_values()
{
	// NetworkState network_state;
	this->network->initStates(state, engine->random_generator);
	
	for (auto initial_value : initial_values) {
		if (nodesByName.find(initial_value.first) != nodesByName.end()) {
			state.setNodeState(nodesByName[initial_value.first], PhysiCell::UniformRandom() < initial_value.second);	
		} else {
			std::cerr << "Initial value set for unknown node : can't find node " << initial_value.first << std::endl;
			exit(1);
		}
	}
	this->set_time_to_update();
}

/* Run a MaBoSS simulation with the input values*/
void MaBoSSNetwork::run_simulation()
{	
	engine->setMaxTime(time_to_update/scaling);
	state = engine->run(state, NULL);
	this->set_time_to_update();

}

bool MaBoSSNetwork::has_node( std::string name ) {
	return nodesByName.find(name) != nodesByName.end();
}

void MaBoSSNetwork::set_node_value(std::string name, bool value) {
	if (has_node(name))
		state.setNodeState(nodesByName[name], value);
	else 
		std::cout << "Can't find node " << name  << "!!!!" << std::endl;
}

bool MaBoSSNetwork::get_node_value(std::string name) {
	if (has_node(name))
		return state.getNodeState(nodesByName[name]);
	else
		std::cout << "Can't find node " << name  << "!!!!" << std::endl;
		return true;
}

std::string MaBoSSNetwork::get_state() {
	return NetworkState(state.getState() & output_mask.getState()).getName(network);
}

/* Print current state of all the nodes of the network */
void MaBoSSNetwork::print_nodes()
{
	int i = 0;
	std::vector<Node*> nodes = this->network->getNodes();
	for ( auto node: nodes )
	{
		std::cout << node->getLabel() << "=" << state.getNodeState(node) << "; ";
		i++;
	}
	std::cout << std::endl;
}