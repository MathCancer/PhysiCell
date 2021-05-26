#include "maboss_network.h"

/* Default constructor */
void MaBoSSNetwork::init_maboss( std::string networkFile, std::string configFile)
{
	if (this->network != NULL) {
		delete this->network;
	}
	
	if (this->config != NULL) {
		delete this->config;
	}
	
	// Initialize MaBoSS Objects for a model
	this->network = new Network();
	this->network->parse(networkFile.c_str());

	this->config = new RunConfig();
	this->config->parse(this->network, configFile.c_str());

	IStateGroup::checkAndComplete(this->network);

	engine = new StochasticSimulationEngine(this->network, this->config, PhysiCell::UniformInt());

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
		nodesByName[mutation.first]->mutate(mutation.second);
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
		state.setNodeState(nodesByName[initial_value.first], PhysiCell::UniformRandom() < initial_value.second);
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
	state.setNodeState(nodesByName[name], value);
}

bool MaBoSSNetwork::get_node_value(std::string name) {
	return state.getNodeState(nodesByName[name]);
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