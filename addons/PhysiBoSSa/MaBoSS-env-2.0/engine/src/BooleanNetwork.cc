/*
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2011-2020 Institut Curie, 26 rue d'Ulm, Paris, France       #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################

   Module:
     BooleanNetwork.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
     updated October 2014
*/

#include "BooleanNetwork.h"
#include "BooleanGrammar.h"
#include "RunConfig.h"
#include "Utils.h"
#include <iostream>

extern FILE* CTBNDLin;
extern void CTBNDL_scan_expression(const char *);
extern int CTBNDLparse();
extern void CTBNDLlex_destroy();
const bool backward_istate = getenv("MABOSS_BACKWARD_ISTATE") != NULL;

bool Node::override = false;
bool Node::augment = false;

Node::Node(const std::string& label, const std::string& description, NodeIndex index) : label(label), description(description), istate_set(false), is_internal(false), is_reference(false), referenceState(false), logicalInputExpr(NULL), rateUpExpr(NULL), rateDownExpr(NULL), index(index)
{
#if !defined(USE_BITSET) && !defined(USE_BOOST_BITSET)
  node_bit = NetworkState::nodeBit(index);
#endif
}

void Node::reset()
{
  description = "";
  istate_set = false;
  is_internal = false;
  is_reference = false;
  referenceState = false;
  delete logicalInputExpr;
  logicalInputExpr = NULL;
  delete rateUpExpr;
  rateUpExpr = NULL;
  delete rateDownExpr;
  rateDownExpr = NULL;
}

void Node::setLogicalInputExpression(const Expression* logicalInputExpr) {
  delete this->logicalInputExpr;
  this->logicalInputExpr = logicalInputExpr;
}

void Node::setRateUpExpression(const Expression* expr) {
  delete this->rateUpExpr;
  this->rateUpExpr = expr;
}

void Node::setRateDownExpression(const Expression* expr) {
  delete this->rateDownExpr;
  this->rateDownExpr = expr;
}

bool Node::isInputNode() const
{
  return getLogicalInputExpression() == NULL && getRateUpExpression() == NULL && getRateDownExpression() == NULL;
}

Network::Network() : last_index(0U)
{
  istate_group_list = new std::vector<IStateGroup*>();
  symbol_table = new SymbolTable();
}

int Network::parseExpression(const char* content, std::map<std::string, NodeIndex>* nodes_indexes){
  
  set_current_network(this);
  CTBNDL_scan_expression(content);

  try 
  {
    int r = CTBNDLparse();
    set_current_network(NULL);

    if (r) {
      CTBNDLlex_destroy();
      return 1;
    }
    compile(nodes_indexes);
    CTBNDLlex_destroy();
    return 0;
  }
  catch (const BNException& e) 
  {
    set_current_network(NULL);
    CTBNDLlex_destroy();

    throw;
  }
}


int Network::parse(const char* file, std::map<std::string, NodeIndex>* nodes_indexes)
{
  if (NULL != file) {
    CTBNDLin = fopen(file, "r");
    if (CTBNDLin == NULL) {
      throw BNException("network parsing: cannot open file:" + std::string(file) + " for reading");
    }
  }

  set_current_network(this);

  try{
    int r = CTBNDLparse();

    set_current_network(NULL);

    if (r) {
      if (NULL != file)
        fclose(CTBNDLin);
      CTBNDLlex_destroy();

      return 1;
    }
    compile(nodes_indexes);

    if (NULL != file)
      fclose(CTBNDLin);
    
    CTBNDLlex_destroy();

    return 0;
  }
  catch (const BNException& e) 
  {  
    if (NULL != file)
      fclose(CTBNDLin);
    
    set_current_network(NULL);
    CTBNDLlex_destroy();

    throw;
  }

}

void SymbolTable::display(std::ostream& os, bool check) const
{
  MAP<std::string, Symbol*>::const_iterator begin = symb_map.begin();
  MAP<std::string, Symbol*>::const_iterator end = symb_map.end();

  while (begin != end) {
    double value = getSymbolValue((*begin).second, check);
    os << (*begin).first << " = " << value << ";\n";
    ++begin;
  }
}

void SymbolTable::checkSymbols() const
{
  MAP<std::string, Symbol*>::const_iterator begin = symb_map.begin();
  MAP<std::string, Symbol*>::const_iterator end = symb_map.end();

  std::string str;
  while (begin != end) {
    const Symbol* symbol = begin->second;;
    SymbolIndex idx = begin->second->getIndex();
    if (!symb_def[idx]) {
      str += std::string("\n") + "symbol " + symbol->getName() + " is not defined";
      //throw BNException("symbol " + symbol->getName() + " is not defined"); 
    }
    ++begin;
  }

  if (str.length() != 0) {
    throw BNException(str);
  }
}

void Network::compile(std::map<std::string, NodeIndex>* nodes_indexes)
{
  MAP<std::string, Node*>::iterator begin = node_map.begin();
  MAP<std::string, Node*>::iterator end = node_map.end();

#if 0
  // checks for cycles
  // actually, not really pertinent...
  while (begin != end) {
    Node* node = (*begin).second;
    if (!node->isInputNode()) {
      if (node->getLogicalInputExpr()->getRateUpExpression()->hasCycle(node)) {
	//std::cerr << "cycle detected for node " << node->getLabel() << '\n';
      }
    }
    ++begin;
  }
#endif

  // looks for input and non input nodes
  begin = node_map.begin();
  while (begin != end) {
    Node* node = (*begin).second;
    if (!isNodeDefined(node->getLabel())) {
      throw BNException("node " + node->getLabel() + " used but not defined"); 
    }
    ++begin;
  }

  begin = node_map.begin();
  nodes.resize(node_map.size());
  while (begin != end) {
    Node* node = (*begin).second;
    if (nodes_indexes != NULL) {
      node->setIndex((*nodes_indexes)[node->getLabel()]);
    }
    
    if (node->isInputNode()) {
      input_nodes.push_back(node);
    } else {
      non_input_nodes.push_back(node);
    }
    //nodes.push_back(node);
    nodes[node->getIndex()] = node;
    ++begin;
  }
}

Node* Network::defineNode(const std::string& label, const std::string& description)
{
  if (isNodeDefined(label)) {
    throw BNException("node " + label + " already defined");
  }
  Node* node = new Node(label, description, last_index++); // node factory
  setNodeAsDefined(label);
  node_map[label] = node;
  return node;
}

Node* Network::getNode(const std::string& label)
{
  if (node_map.find(label) == node_map.end()) {
    throw BNException("network: node " + label + " not defined");
  }
  return node_map[label];
}

void Network::initStates(NetworkState& initial_state, RandomGenerator * randgen)
{
  if (backward_istate) {
    std::vector<Node*>::const_iterator begin = nodes.begin();
    std::vector<Node*>::const_iterator end = nodes.end();
  
    while (begin != end) {
      Node* node = *begin;
      initial_state.setNodeState(node, node->getIState(this, randgen));
      ++begin;
    }
  } else {
    IStateGroup::initStates(this, initial_state, randgen);
  }
}

void Network::display(std::ostream& os) const
{
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  for (unsigned int nn = 0; begin != end; ++nn) {
    Node* node = *begin;
    if (0 != nn) {
      os << '\n';
    }
    node->display(os);
    ++begin;
  }
}

double Node::getRateUp(const NetworkState& network_state) const
{
  if (getRateUpExpression() == NULL) {
    if (NULL != getLogicalInputExpression()) {
      double d = getLogicalInputExpression()->eval(this, network_state);
      return (0.0 != d) ? 1.0 : 0.0;
    }
    return 0.0;
  }
  return getRateUpExpression()->eval(this, network_state);
}

double Node::getRateDown(const NetworkState& network_state) const
{
  if (getRateDownExpression() == NULL) {
    if (NULL != getLogicalInputExpression()) {
      double d = getLogicalInputExpression()->eval(this, network_state);
      return (0.0 != d) ? 0.0 : 1.0;
    }
    return 0.0;
  }
  return getRateDownExpression()->eval(this, network_state);
}

void Node::mutate(double value) 
{
    logicalInputExpr = new ConstantExpression(value);
    rateUpExpr = NULL;
    rateDownExpr = NULL;
}

NodeState Node::getNodeState(const NetworkState& network_state) const
{
  return network_state.getNodeState(this);
}

void Node::setNodeState(NetworkState& network_state, NodeState state)
{
  network_state.setNodeState(this, state);
}

bool Node::computeNodeState(NetworkState& network_state, NodeState& node_state) const
{
  return network_state.computeNodeState(this, node_state);
}

NodeState Node::getIState(const Network* network, RandomGenerator* rangen) const
{
  if (!istate_set) {
#if 1
    double rand = rangen->generate();
    istate = rand > 0.5; // >= 0.5 ?
#else
    istate = (rangen->generateUInt32() % 2) == 0;
#endif
  }
  return istate;
}

void Node::display(std::ostream& os) const
{
  os << "node " << label << " {\n";
  if (description.length() > 0) {
    os << "  description = \"" << description << "\";\n";
  }
  if (NULL != logicalInputExpr) {
    os << "  logic = ";
    logicalInputExpr->display(os);
    os << ";\n";
  }
  if (NULL != rateUpExpr) {
    os << "  rate_up = ";
    rateUpExpr->display(os);
    os << ";\n";
  }
  if (NULL != rateDownExpr) {
    os << "  rate_down = ";
    rateDownExpr->display(os);
    os << ";\n";
  }

  if (attr_expr_map.size() || attr_str_map.size()) {
    os << "\n  // extra attributes\n";
    MAP<std::string, const Expression*>::const_iterator attr_expr_begin = attr_expr_map.begin();
    MAP<std::string, const Expression*>::const_iterator attr_expr_end = attr_expr_map.end();
    while (attr_expr_begin != attr_expr_end) {
      os << "  " << (*attr_expr_begin).first << " = ";
      (*attr_expr_begin).second->display(os);
      os << ";\n";
      ++attr_expr_begin;
    }

    MAP<std::string, std::string>::const_iterator attr_str_begin = attr_str_map.begin();
    MAP<std::string, std::string>::const_iterator attr_str_end = attr_str_map.end();
    while (attr_str_begin != attr_str_end) {
      os << "  " << (*attr_str_begin).first << " = \"" << (*attr_str_begin).second << "\";\n";
      ++attr_str_begin;
    }
  }
  os << "}\n";
}

bool NetworkState::computeNodeState(const Node* node, NodeState& node_state)
{
  const Expression* expr = node->getLogicalInputExpression();
  if (NULL != expr) {
    double d = expr->eval(node, *this);
    node_state = d != 0.;
    setNodeState(node, node_state);
    return true;
  }
  return false;
}

#define HAMMING_METHOD2

unsigned int NetworkState::hamming(Network* network, NetworkState_Impl state2) const
{
  unsigned int hd = 0;
#ifdef HAMMING_METHOD1
  // faster way
  unsigned long s = (state ^ (state2 & state));
  unsigned int node_count = network->getNodes().size();
  for (unsigned int nn = 0; nn < node_count; ++nn) {
    if ((1ULL << nn) & s) {
      hd++;
    }
  }
  return hd;
#endif

#ifdef HAMMING_METHOD2
  NetworkState network_state2(state2);
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  while (begin != end) {
    Node* node = *begin;
    if (node->isReference()) {
      NodeState node_state1 = getNodeState(node);
      NodeState node_state2 = network_state2.getNodeState(node);
      hd += 1 - (node_state1 == node_state2);
    }
    ++begin;
  }

  return hd;
#endif
}

void Network::displayHeader(std::ostream& os) const
{
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  for (int nn = 0; begin != end; ++nn) {
    Node* node = *begin;
    os << (nn > 0 ? "\t" : "") << node->getLabel();
    ++begin;
  }
  os << '\n';
}


void NetworkState::display(std::ostream& os, Network* network) const
{
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  for (int nn = 0; begin != end; ++nn) {
    Node* node = *begin;
    os << (nn > 0 ? "\t" : "") << getNodeState(node);
    ++begin;
  }
  os << '\n';
}

std::string NetworkState::getName(Network* network, const std::string& sep) const {
   #if defined(USE_BITSET) || defined(USE_BOOST_BITSET)
  if (state.none()) {
    return "<nil>";
  }
#else
  if (!state) {
    return "<nil>";
  }
#endif

  std::string result = "";
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  bool displayed = false;
  while (begin != end) {
    Node* node = *begin;
    if (getNodeState(node)) {
      if (displayed) {
	    result += sep;
      } else {
	displayed = true;
      }
      result += node->getLabel();
    }
    ++begin;
  }
  return result;
  }


void NetworkState::displayOneLine(std::ostream& os, Network* network, const std::string& sep) const
{
  os << getName(network, sep);
}

std::ostream& operator<<(std::ostream& os, const BNException& e)
{
  os << "BooleanNetwork exception: " << e.getMessage() << '\n';
  return os;
}

void IStateGroup::checkAndComplete(Network* network)
{
  std::map<std::string, bool> node_label_map;
  std::vector<IStateGroup*>::iterator begin = network->getIStateGroup()->begin();
  std::vector<IStateGroup*>::iterator end = network->getIStateGroup()->end();
  
  
  while (begin != end) {
    IStateGroup* istate_group = *begin;
    std::vector<const Node*>* nodes = istate_group->getNodes();

    std::vector<const Node*>::iterator bb = nodes->begin();
    std::vector<const Node*>::iterator ee = nodes->end();
    while (bb != ee) {
      const Node* node = *bb;
      if (node_label_map.find(node->getLabel()) != node_label_map.end()) {
	throw BNException("duplicate node istate declaration: " + node->getLabel());
      }
      node_label_map[node->getLabel()] = true;
      ++bb;
    }

    ++begin;
  }

  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator bb = nodes.begin();
  std::vector<Node*>::const_iterator ee = nodes.end();
  while (bb != ee) {
    const Node* node = *bb;
    if (node_label_map.find(node->getLabel()) == node_label_map.end()) {
      new IStateGroup(network, node);
    }
    ++bb;
  }

  // now complete missing nodes
}

void IStateGroup::initStates(Network* network, NetworkState& initial_state, RandomGenerator* randgen)
{
  std::vector<IStateGroup*>::iterator istate_group_iter = network->getIStateGroup()->begin();
  std::vector<IStateGroup*>::iterator istate_group_end = network->getIStateGroup()->end();
  
  while (istate_group_iter != istate_group_end) {
    IStateGroup* istate_group = *istate_group_iter;
    std::vector<const Node*>* nodes = istate_group->getNodes();
    //size_t node_size = nodes->size();
    std::vector<ProbaIState*>* proba_istates = istate_group->getProbaIStates();

    if (proba_istates->size() == 1) {
      ProbaIState* proba_istate = (*proba_istates)[0];
      //double proba_value = proba_istate->getProbaValue();
      std::vector<double>* state_value_list = proba_istate->getStateValueList();
      std::vector<double>::const_iterator state_value_iter = state_value_list->begin();
      std::vector<double>::const_iterator state_value_end = state_value_list->end();
      size_t nn = 0;
      while (state_value_iter != state_value_end) {
	const Node* node = (*nodes)[nn++];
	initial_state.setNodeState(node, (*state_value_iter) != 0. ? true : false);
	++state_value_iter;
      }
    } else {
      // double rand = network->getRandomGenerator()->generate();
      double rand = randgen->generate();
      assert(rand >= 0. && rand <= 1.);
      std::vector<ProbaIState*>::iterator proba_istate_iter = proba_istates->begin();
      std::vector<ProbaIState*>::iterator proba_istate_end = proba_istates->end();
      size_t proba_istate_size = proba_istates->size();
      double proba_sum = 0;
      for (size_t jj = 0; proba_istate_iter != proba_istate_end; ++jj) {
	ProbaIState* proba_istate = *proba_istate_iter;
	proba_sum += proba_istate->getProbaValue();
	//std::cerr << "rand: " << rand << " " << proba_sum << '\n';
	if (rand < proba_sum || jj == proba_istate_size - 1) {
	  std::vector<double>* state_value_list = proba_istate->getStateValueList();
	  std::vector<double>::iterator state_value_iter = state_value_list->begin();
	  std::vector<double>::iterator state_value_end = state_value_list->end();
	  size_t nn = 0;
	  //std::cerr << "state #" << jj << " choosen\n";
	  while (state_value_iter != state_value_end) {
	    const Node* node = (*nodes)[nn++];
	    initial_state.setNodeState(node, (*state_value_iter) != 0. ? true : false);
	    ++state_value_iter;
	  }
	  break;
	}
	++proba_istate_iter;
      }
    }
    ++istate_group_iter;
  }
}

void IStateGroup::display(Network* network, std::ostream& os)
{
  std::vector<IStateGroup*>::iterator begin = network->getIStateGroup()->begin();
  std::vector<IStateGroup*>::iterator end = network->getIStateGroup()->end();
  
  while (begin != end) {
    IStateGroup* istate_group = *begin;
    std::vector<const Node*>* nodes = istate_group->getNodes();
    std::vector<ProbaIState*>* proba_istates = istate_group->getProbaIStates();

    std::vector<const Node*>::iterator bb = nodes->begin();
    std::vector<const Node*>::iterator ee = nodes->end();
    if (nodes->size() == 1 && proba_istates->size() == 1) {
      std::vector<double>* state_value_list = (*proba_istates)[0]->getStateValueList();
      os << (*bb)->getLabel() << ".istate = " << ((*state_value_list)[0] != 0. ? "1" : "0") << ";\n";
      ++begin;
      continue;
    }
    os << '[';
    for (unsigned int nn = 0; bb != ee; ++nn) {
      const Node* node = *bb;
      os << (nn > 0 ? ", " : "") << node->getLabel();
      ++bb;
    }
    os << "].istate = ";

    std::vector<ProbaIState*>::iterator proba_istate_iter = proba_istates->begin();
    std::vector<ProbaIState*>::iterator proba_istate_end = proba_istates->end();
    for (size_t jj = 0; proba_istate_iter != proba_istate_end; ++jj) {
      ProbaIState* proba_istate = *proba_istate_iter;
      os << (jj > 0 ? ", " : "") << proba_istate->getProbaValue() << " [";
      std::vector<double>* state_value_list = proba_istate->getStateValueList();
      std::vector<double>::iterator state_value_iter = state_value_list->begin();
      std::vector<double>::iterator state_value_end = state_value_list->end();
      for (size_t ii = 0; state_value_iter != state_value_end; ++ii) {
	os << (ii > 0 ? ", " : "") << *state_value_iter;
	++state_value_iter;
      }
      os << "]";
      ++proba_istate_iter;
    }

    os << ";";
    /*
    if (istate_group->isRandom()) {
      os << " // <=> " << (*nodes->begin())->getLabel() << ".istate = -1";
    }
    */
    os << '\n';
    ++begin;
  }
}

void IStateGroup::reset(Network * network) {
  network->getIStateGroup()->clear();
}

Node::~Node()
{
  delete logicalInputExpr;
  delete rateUpExpr;
  delete rateDownExpr;

  MAP<std::string, const Expression*>::const_iterator attr_expr_begin = attr_expr_map.begin();
  MAP<std::string, const Expression*>::const_iterator attr_expr_end = attr_expr_map.end();
  while (attr_expr_begin != attr_expr_end) {
    delete (*attr_expr_begin).second;
    attr_expr_begin++;
  }
}

Network::Network(const Network& network)
{
  *this = network;
}

Network& Network::operator=(const Network& network)
{
  node_map = network.node_map;
  last_index = network.last_index;
  input_nodes = network.input_nodes;
  non_input_nodes = network.non_input_nodes;
  nodes = network.nodes;
  symbol_table = network.symbol_table;
  return *this;
}

Network::~Network()
{
  delete symbol_table;
  
  for (std::vector<IStateGroup*>::iterator iter = istate_group_list->begin(); iter != istate_group_list->end(); ++iter) {
    delete *iter;
  }
  delete istate_group_list;
  
  for (MAP<std::string, Node*>::iterator iter = node_map.begin(); iter != node_map.end(); ++iter) {
    delete (*iter).second;
  }
  
}

void Network::cloneIStateGroup(std::vector<IStateGroup*>* _istate_group_list) 
{
  for (auto istate_group: *_istate_group_list) 
  {
    new IStateGroup(istate_group, this);
  }
}

void SymbolTable::reset()
{
  symb_map.clear();
  symb_value.clear();
  symb_def.clear();
  symb_dont_set.clear();
  last_symb_idx = 0;
}

int setConfigVariables(Network* network, const std::string& prog, std::vector<std::string>& runvar_v)
{
  SymbolTable* symtab = network->getSymbolTable();
  std::vector<std::string>::const_iterator begin = runvar_v.begin();
  std::vector<std::string>::const_iterator end = runvar_v.end();

  while (begin != end) {
    const std::string& var_values = *begin;
    size_t o_var_value_pos = 0;
    for (;;) {
      if (o_var_value_pos == std::string::npos) {
	break;
      }
      size_t var_value_pos = var_values.find(',', o_var_value_pos);
      std::string var_value = var_value_pos == std::string::npos ? var_values.substr(o_var_value_pos) : var_values.substr(o_var_value_pos, var_value_pos-o_var_value_pos);
      o_var_value_pos = var_value_pos + (var_value_pos == std::string::npos ? 0 : 1);
      size_t pos = var_value.find('=');
      if (pos == std::string::npos) {
	std::cerr << '\n' << prog << ": invalid var format [" << var_value << "] VAR=BOOL_OR_DOUBLE expected\n";
	return 1;
      }
      std::string ovar = var_value.substr(0, pos);
      std::string var = ovar[0] != '$' ? "$" + ovar : ovar;
      const Symbol* symbol = symtab->getOrMakeSymbol(var);
      std::string value = var_value.substr(pos+1);
      if (!strcasecmp(value.c_str(), "true")) {
	symtab->overrideSymbolValue(symbol, 1);
      } else if (!strcasecmp(value.c_str(), "false")) {
	symtab->overrideSymbolValue(symbol, 0);
      } else {
	double dval;
	int r = sscanf(value.c_str(), "%lf", &dval);
	if (r != 1) {
	  std::cerr << '\n' << prog << ": invalid value format [" << var_value << "] " << ovar << "=BOOL_OR_DOUBLE expected\n";
	  return 1;
	}
	symtab->overrideSymbolValue(symbol, dval);
      }
    }
    ++begin;
  }
  return 0;
}

int setConfigVariables(Network* network, const std::string& prog, const std::string& runvar)
{
  std::vector<std::string> runvar_v;
  runvar_v.push_back(runvar);
  return setConfigVariables(network, prog, runvar_v);
}

void SymbolTable::unsetSymbolExpressions() {
  std::vector<SymbolExpression *>::const_iterator begin = symbolExpressions.begin();
  std::vector<SymbolExpression *>::const_iterator end = symbolExpressions.end();

  while(begin != end) {

    SymbolExpression * exp = *begin;
    exp->unset();
    begin++;
  }
}