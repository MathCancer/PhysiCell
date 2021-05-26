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
     BooleanNetwork.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _BOOLEANNETWORK_H_
#define _BOOLEANNETWORK_H_

#include "maboss-config.h"

#if MAXNODES > 64
//#define USE_BOOST_BITSET
#define USE_BITSET
#else
#undef MAXNODES
#define MAXNODES 64
#endif

// To be defined only when comparing bitset with ulong implementation
//#define COMPARE_BITSET_AND_ULONG

#ifdef HAS_UNORDERED_MAP
#define USE_UNORDERED_MAP
#endif

#define MAP std::map

#include <map>

#include <string>
#include <vector>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <strings.h>
#include <string.h>
#ifdef USE_BOOST_BITSET
#include <boost/dynamic_bitset.hpp>
#elif defined(USE_BITSET)
#include <bitset>
#endif
#include "Function.h"

const std::string LOGICAL_AND_SYMBOL = " & ";
const std::string LOGICAL_OR_SYMBOL = " | ";
const std::string LOGICAL_NOT_SYMBOL = "!";
const std::string LOGICAL_XOR_SYMBOL = " ^ ";

class Expression;
class NotLogicalExpression;
class SymbolExpression;
class ConstantExpression;
class NetworkState;
class Network;
class Node;
class RandomGenerator;
class RunConfig;
class IStateGroup;

class LogicalExprGenContext {

  const Network* network;
  const Node* node;
  std::ostream& os;
  unsigned int level;

 public:
  LogicalExprGenContext(const Network* network, const Node* node, std::ostream& os) : network(network), node(node), os(os), level(0) { }

  const Network* getNetwork() const {return network;}
  std::ostream& getOStream() const {return os;}

  const Node* getNode() {return node;}

  unsigned int getLevel() const {return level;}
  void incrLevel() {level++;}
  void decrLevel() {level--;}

  class LevelManager {

    LogicalExprGenContext& genctx;
    unsigned int level;

  public:
    LevelManager(LogicalExprGenContext& genctx) : genctx(genctx) {
      level = genctx.getLevel();
      genctx.incrLevel();
    }

    unsigned int getLevel() const {return level;}

    ~LevelManager() {
      genctx.decrLevel();
    }
  };
};


typedef unsigned int NodeIndex;
typedef bool NodeState; // for now... could be a class
typedef unsigned int SymbolIndex;

#ifdef USE_BOOST_BITSET

typedef boost::dynamic_bitset<> NetworkState_Impl;

#elif defined(USE_BITSET)

#ifdef USE_UNORDERED_MAP
typedef std::bitset<MAXNODES> NetworkState_Impl;

namespace std {
  template <> struct HASH_STRUCT<bitset<MAXNODES> > : public std::unary_function<bitset<MAXNODES>, size_t>
  {
    size_t operator()(const bitset<MAXNODES>& val) const {
#ifdef COMPARE_BITSET_AND_ULONG
      return val.to_ulong();
#else
      static const bitset<MAXNODES> MASK(0xFFFFFFFFUL);
      return (val & MASK).to_ulong();
#endif
    }
  };

  template <> struct equal_to<bitset<MAXNODES> > : public binary_function<bitset<MAXNODES>, bitset<MAXNODES>, bool>
  {
    size_t operator()(const bitset<MAXNODES>& val1, const bitset<MAXNODES>& val2) const {
      return val1 == val2;
    }
  };

  // Added less operator, necessary for maps, sets. Code from https://stackoverflow.com/a/21245301/11713763
  template <> struct less<bitset<MAXNODES> > : public binary_function<bitset<MAXNODES>, bitset<MAXNODES>, bool>
  {
    size_t operator()(const bitset<MAXNODES>& val1, const bitset<MAXNODES>& val2) const {
    for (int i = MAXNODES-1; i >= 0; i--) {
        if (val1[i] ^ val2[i]) return val2[i];
    }
    return false;

    }
  };
}

#else

template <int N> class sbitset : public std::bitset<N> {

 public:
  sbitset() : std::bitset<N>() { }
  sbitset(const sbitset<N>& sbitset) : std::bitset<N>(sbitset) { }
  sbitset(const std::bitset<N>& bitset) : std::bitset<N>(bitset) { }

  int operator<(const sbitset<N>& bitset1) const {
#ifdef COMPARE_BITSET_AND_ULONG
    return this->to_ulong() < bitset1.to_ulong();
#else
    for (int nn = N-1; nn >= 0; --nn) {
      int delta = this->test(nn) - bitset1.test(nn);
      if (delta < 0) {
	return 1;
      }
      if (delta > 0) {
	return 0;
      }
    }
    return 0;
#endif
  }
};

typedef sbitset<MAXNODES> NetworkState_Impl;
#endif

#else
typedef unsigned long long NetworkState_Impl;
#endif

static const std::string ATTR_RATE_UP = "rate_up";
static const std::string ATTR_RATE_DOWN = "rate_down";
static const std::string ATTR_LOGIC = "logic";
static const std::string ATTR_DESCRIPTION = "description";
static const NodeIndex INVALID_NODE_INDEX = (NodeIndex)~0U;

class BNException {

  std::string msg;

public:
  BNException(const std::string& msg) : msg(msg) { }

  const std::string& getMessage() const {return msg;}
};

extern std::ostream& operator<<(std::ostream& os, const BNException& e);

class Node {
  static bool override;
  static bool augment;
  std::string label;
  std::string description;
  bool istate_set;
  bool is_internal;
  bool is_reference;
  NodeState referenceState;
  const Expression* logicalInputExpr;
  const Expression* rateUpExpr;
  const Expression* rateDownExpr;
  mutable NodeState istate;
  NodeIndex index;
  
  MAP<std::string, const Expression*> attr_expr_map;
  MAP<std::string, std::string> attr_str_map;
  Expression* rewriteLogicalExpression(Expression* rateUpExpr, Expression* rateDownExpr) const;
  NetworkState_Impl node_bit;

 public:
  Node(const std::string& label, const std::string& description, NodeIndex index);

  void setIndex(NodeIndex new_index) {
    index = new_index;
#if !defined(USE_BITSET) && !defined(USE_BOOST_BITSET)
    node_bit = 1ULL << new_index;
#endif
  }

  const std::string& getLabel() const {
    return label;
  }

  void setDescription(const std::string& description) {
    this->description = description;
  }

  const std::string& getDescription() const {
    return description;
  }

  void setLogicalInputExpression(const Expression* logicalInputExpr);

  void setRateUpExpression(const Expression* expr);

  void setRateDownExpression(const Expression* expr);

  const Expression* getLogicalInputExpression() const {
    return logicalInputExpr;
  }

  const Expression* getRateUpExpression() const {
    return rateUpExpr;
  }

  const Expression* getRateDownExpression() const {
    return rateDownExpr;
  }

  void setAttributeExpression(const std::string& attr_name, const Expression* expr) {
    if (attr_name == ATTR_RATE_UP) {
      setRateUpExpression(expr);
      return;
    }
    if (attr_name == ATTR_RATE_DOWN) {
      setRateDownExpression(expr);
      return;
    }
    if (attr_name == ATTR_LOGIC) {
      setLogicalInputExpression(expr);
      return;
    }
    attr_expr_map[attr_name] = expr;
  }

  void mutate(double value);

  NodeState getIState(const Network* network, RandomGenerator* randgen) const;

  void setIState(NodeState istate) {
    istate_set = true;
    this->istate = istate;
  }

  bool istateSetRandomly() const {
    return !istate_set;
  }

  bool isInternal() const {
    return is_internal;
  }

  void isInternal(bool is_internal) {
    this->is_internal = is_internal;
  }

  bool isReference() const {
    return is_reference;
  }

  void setReference(bool is_reference) {
    this->is_reference = is_reference;
  }

  NodeState getReferenceState() const {
    return referenceState;
  }

  void setReferenceState(NodeState referenceState) {
    this->is_reference = true;
    this->referenceState = referenceState;
  }

  const Expression* getAttributeExpression(const std::string& attr_name) const {
    MAP<std::string, const Expression*>::const_iterator iter = attr_expr_map.find(attr_name);
    if (iter == attr_expr_map.end()) {
      if (attr_name == ATTR_RATE_UP) {
	return getRateUpExpression();
      }
      if (attr_name == ATTR_RATE_DOWN) {
	return getRateDownExpression();
      }
      if (attr_name == ATTR_LOGIC) {
	return getLogicalInputExpression();
      }
      return NULL;
    }
    return (*iter).second;
  }

  void setAttributeString(const std::string& attr_name, const std::string& str) {
    if (attr_name == ATTR_DESCRIPTION) {
      setDescription(str);
      return;
    }

    attr_str_map[attr_name] = str;
  }

  std::string getAttributeString(const std::string& attr_name) const {
    MAP<std::string, std::string>::const_iterator iter = attr_str_map.find(attr_name);
    if (iter == attr_str_map.end()) {
      if (attr_name == ATTR_DESCRIPTION) {
	return getDescription();
      }
      return "";
    }
    return (*iter).second;
  }

  NodeIndex getIndex() const {return index;}

#if !defined(USE_BITSET) && !defined(USE_BOOST_BITSET)
  NetworkState_Impl getNodeBit() const {return node_bit;}
#endif

  const MAP<std::string, const Expression*>& getAttributeExpressionMap() const {
    return attr_expr_map;
  }

  const MAP<std::string, std::string>& getAttributeStringMap() const {
    return attr_str_map;
  }

  bool isInputNode() const; // true if node state does not depend on other node states

  double getRateUp(const NetworkState& network_state) const;
  double getRateDown(const NetworkState& network_state) const;
  NodeState getNodeState(const NetworkState& network_state) const;
  void setNodeState(NetworkState& network_state, NodeState state);

  // returns true if and only if there is a logical input expression that allows to compute state from input nodes
  bool computeNodeState(NetworkState& network_state, NodeState& node_state) const;

  std::string toString() const {
    std::ostringstream ostr;
    display(ostr);
    return ostr.str();
  }

  void display(std::ostream& os) const;
  void generateLogicalExpression(LogicalExprGenContext& gen) const;

  static void setOverride(bool override) {
    Node::override = override;
  }

  static bool isOverride() {return override;}

  static void setAugment(bool augment) {
    Node::augment = augment;
  }

  static bool isAugment() {return augment;}

  void reset();

  ~Node();
};

// symbol entry (i.e. variables)
class Symbol {
  std::string symb;
  SymbolIndex symb_idx;

public:
  Symbol(const std::string& symb, SymbolIndex symb_idx) : symb(symb), symb_idx(symb_idx) { }
  const std::string& getName() const {return symb;}
  SymbolIndex getIndex() const {return symb_idx;}
};

//The symbol table
class SymbolTable {
  SymbolIndex last_symb_idx;
  MAP<std::string, Symbol*> symb_map;
  std::vector<double> symb_value;
  std::vector<bool> symb_def;
  std::map<SymbolIndex, bool> symb_dont_set;
  
  std::vector<SymbolExpression *> symbolExpressions;

public:
  SymbolTable() : last_symb_idx(0) { }
  
  const Symbol* getSymbol(const std::string& symb) {
    if (symb_map.find(symb) == symb_map.end()) {
      return NULL;
    }
    return symb_map[symb];
  }

  const Symbol* getOrMakeSymbol(const std::string& symb) {
    if (symb_map.find(symb) == symb_map.end()) {
      symb_def.push_back(false);
      symb_value.push_back(0.0);
      symb_map[symb] = new Symbol(symb, last_symb_idx++);
      assert(symb_value.size() == last_symb_idx);
      assert(symb_def.size() == last_symb_idx);
    }
    return symb_map[symb];
  }

  double getSymbolValue(const Symbol* symbol, bool check = true) const {
    SymbolIndex idx = symbol->getIndex();
    if (!symb_def[idx]) {
      if (check) {
	throw BNException("symbol " + symbol->getName() + " is not defined"); 
      }
      return 0.;
   }
    return symb_value[idx];
  }

  size_t getSymbolCount() const {return symb_map.size();}

  void setSymbolValue(const Symbol* symbol, double value) {
    SymbolIndex idx = symbol->getIndex();
    if (symb_dont_set.find(idx) == symb_dont_set.end()) {
      symb_def[idx] = true;
      symb_value[idx] = value;
    }
  }

  void overrideSymbolValue(const Symbol* symbol, double value) {
    setSymbolValue(symbol, value);
    symb_dont_set[symbol->getIndex()] = true;
  }

  void display(std::ostream& os, bool check = true) const;
  void checkSymbols() const;

  std::vector<std::string> getSymbolsNames() const {
    std::vector<std::string> result;
    for (auto& symb : symb_map) {
      result.push_back(symb.first);
    }
    return result;
  }
  void reset();

  void addSymbolExpression(SymbolExpression * exp) {
    symbolExpressions.push_back(exp);
  }

  void unsetSymbolExpressions();

  ~SymbolTable() {
    for (auto& symbol : symb_map) {
      delete symbol.second;
    }
  }
};

// the boolean network (also used as a Node factory)
class Network {
  MAP<std::string, Node*> node_map;
  NodeIndex last_index;
  std::vector<Node*> input_nodes;
  std::vector<Node*> non_input_nodes;
  std::vector<Node*> nodes;

  MAP<std::string, bool> node_def_map;
  std::vector<IStateGroup*>* istate_group_list;
  SymbolTable* symbol_table;

public:

  Network();

  Network(const Network& network);
  Network& operator=(const Network& network);

  int parse(const char* file = NULL, std::map<std::string, NodeIndex>* nodes_indexes = NULL);
  int parseExpression(const char* content = NULL, std::map<std::string, NodeIndex>* nodes_indexes = NULL);

  std::vector<IStateGroup*>* getIStateGroup() {
    return istate_group_list;
  }

  void cloneIStateGroup(std::vector<IStateGroup*>* _istate_group_list);

  SymbolTable* getSymbolTable() { 
    return symbol_table;
  };
  Node* defineNode(const std::string& label, const std::string& description = "");

  Node* getNode(const std::string& label);

  Node* getNode(NodeIndex node_idx) {
    assert(node_idx < getNodeCount());
    return nodes[node_idx];
  }

  Node* getOrMakeNode(const std::string& label) {
    if (node_map.find(label) != node_map.end()) {
      return node_map[label];
    }
    if (node_map.size() >= MAXNODES) {
      std::ostringstream ostr;
      ostr << MAXNODES;
      throw BNException("maximum node count exceeded " + ostr.str());
    }
    Node* node = new Node(label, "", last_index++); // node factory
    node_map[label] = node;
    return node;
  }

  size_t getNodeCount() const {return node_map.size();}

  void compile(std::map<std::string, NodeIndex>* nodes_indexes = NULL);

  // vector of nodes which do not depend on other nodes
  const std::vector<Node*>& getInputNodes() const {return input_nodes;}

  // vector of the other nodes
  const std::vector<Node*>& getNonInputNodes() const {return non_input_nodes;}

  // vector of all nodes
  const std::vector<Node*>& getNodes() const {return nodes;}

  std::string toString() const {
    std::ostringstream ostr;
    display(ostr);
    return ostr.str();
  }
  
  void initStates(NetworkState& initial_state, RandomGenerator* randgen);

  void displayHeader(std::ostream& os) const;

  void display(std::ostream& os) const;

  void generateLogicalExpressions(std::ostream& os) const;

  bool isNodeDefined(const std::string& identifier) {
    return node_def_map.find(identifier) != node_def_map.end();
  }

  void setNodeAsDefined(const std::string& identifier) {
    node_def_map[identifier] = true;
  }

  void resetNodeDefinition() {
    node_def_map.clear();
  }

  ~Network();
};

// global state of the boolean network
class NetworkState {
  NetworkState_Impl state;

#if !defined(USE_BITSET) && !defined(USE_BOOST_BITSET)
  static NetworkState_Impl nodeBit(const Node* node) {
    return node->getNodeBit();
  }

public:
  static NetworkState_Impl nodeBit(NodeIndex node_index) {
    return (1ULL << node_index);
  }
#endif

public:
  NetworkState(const NetworkState_Impl& state) : state(state) { }

#ifdef USE_BITSET
  NetworkState() { }
#elif defined(USE_BOOST_BITSET)
  NetworkState() : state(MAXNODES) { }
#else
  NetworkState() : state(0ULL) { }
#endif

  NodeState getNodeState(const Node* node) const {
#if defined(USE_BITSET) || defined(USE_BOOST_BITSET)
    return state.test(node->getIndex());
#else
    return state & nodeBit(node);
#endif
  }

  void setNodeState(const Node* node, NodeState node_state) {
#if defined(USE_BITSET) || defined(USE_BOOST_BITSET)
    state.set(node->getIndex(), node_state);
#else
    if (node_state) {
      state |= nodeBit(node);
    } else {
      state &= ~nodeBit(node);
    }
#endif
  }

  void flipState(const Node* node) {
#if defined(USE_BITSET) || defined(USE_BOOST_BITSET)
    //state.set(node->getIndex(), !state.test(node->getIndex()));
    state.flip(node->getIndex());
#else
    state ^= nodeBit(node);
#endif
  }

  // returns true if and only if there is a logical input expression that allows to compute state from input nodes
  bool computeNodeState(const Node* node, NodeState& node_state);

  NetworkState_Impl getState() const {return state;}

  void display(std::ostream& os, Network* network) const;

  std::string getName(Network * network, const std::string& sep=" -- ") const;
 
  void displayOneLine(std::ostream& os, Network* network, const std::string& sep = " -- ") const;

#ifndef USE_UNORDERED_MAP
  bool operator<(const NetworkState& network_state) const {
    return state < network_state.state;
  }
#endif
  unsigned int hamming(Network* network, NetworkState_Impl state) const;

  static NodeState getState(Node* node, NetworkState_Impl state) {
#if defined(USE_BITSET) || defined(USE_BOOST_BITSET)
    return state.test(node->getIndex());
#else
    return state & nodeBit(node);
#endif
  }
};

// abstract base class used for expression evaluation
class Expression {

public:
  virtual double eval(const Node* this_node, const NetworkState& network_state) const = 0;
  virtual bool hasCycle(Node* node) const = 0;

  std::string toString() const {
    std::ostringstream ostr;
    display(ostr);
    return ostr.str();
  }

  virtual Expression* clone() const = 0; 

  virtual Expression* cloneAndShrink(bool& shrinked) const {
    return clone();
  }

  virtual void display(std::ostream& os) const = 0;
  virtual bool isConstantExpression() const {return false;}
  virtual bool isLogicalExpression() const {return false;}

  virtual void generateLogicalExpression(LogicalExprGenContext& genctx) const = 0;
  virtual bool generationWillAddParenthesis() const {return false;}

  bool evalIfConstant(double& value) const;
  bool evalIfConstant(bool& value) const;

  static Expression* cloneAndShrinkRecursive(Expression* expr);

  virtual const NotLogicalExpression* asNotLogicalExpression() const {return NULL;}

  virtual ~Expression() {
  }
};

class NodeExpression : public Expression {
  Node* node;

public:
  NodeExpression(Node* node) : node(node) { }

  Expression* clone() const {return new NodeExpression(node);}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return (double)node->getNodeState(network_state);
  }

  bool hasCycle(Node* node) const {
    return this->node == node;
  }

  void display(std::ostream& os) const {
    os << node->getLabel();
  }

  bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

  ~NodeExpression() {
    //delete node;
  }
};

// concrete classes used for expression evaluation
class BinaryExpression : public Expression {

protected:
  Expression* left;
  Expression* right;

public:
  BinaryExpression(Expression* left, Expression* right) : left(left), right(right) { }

  bool hasCycle(Node* node) const {
    return left->hasCycle(node) || right->hasCycle(node);
  }

  virtual bool isConstantExpression() const {
    return left->isConstantExpression() && right->isConstantExpression();
  }

  virtual ~BinaryExpression() {
    delete left;
    delete right;
  }
};

class MulExpression : public BinaryExpression {

public:
  MulExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new MulExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) * right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " * ";
    right->display(os);
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class DivExpression : public BinaryExpression {

public:
  DivExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new DivExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) / right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " / ";
    right->display(os);
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class AddExpression : public BinaryExpression {

public:
  AddExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new AddExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) + right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " + ";
    right->display(os);
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class SubExpression : public BinaryExpression {

public:
  SubExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new SubExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) - right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " - ";
    right->display(os);
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class EqualExpression : public BinaryExpression {

public:
  EqualExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new EqualExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) == right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " == ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class NotEqualExpression : public BinaryExpression {

public:
  NotEqualExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new NotEqualExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) != right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " != ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class LetterExpression : public BinaryExpression {

public:
  LetterExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new LetterExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) < right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " < ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class LetterOrEqualExpression : public BinaryExpression {

public:
  LetterOrEqualExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new LetterOrEqualExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) <= right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " <= ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class GreaterExpression : public BinaryExpression {

public:
  GreaterExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new GreaterExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) > right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " > ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class GreaterOrEqualExpression : public BinaryExpression {

public:
  GreaterOrEqualExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new GreaterOrEqualExpression(left->clone(), right->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return left->eval(this_node, network_state) >= right->eval(this_node, network_state);
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " >= ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class CondExpression : public Expression {

  Expression* cond_expr;
  Expression* true_expr;
  Expression* false_expr;

public:
  CondExpression(Expression* cond_expr, Expression* true_expr, Expression* false_expr) : cond_expr(cond_expr), true_expr(true_expr), false_expr(false_expr) { }

  Expression* clone() const {return new CondExpression(cond_expr->clone(), true_expr->clone(), false_expr->clone());}

  Expression* cloneAndShrink(bool& shrinked) const;
  //  Expression* cloneAndShrink(bool& shrinked) const {return new CondExpression(cond_expr->cloneAndShrink(shrinked), true_expr->cloneAndShrink(shrinked), false_expr->cloneAndShrink(shrinked));}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    if (0. != cond_expr->eval(this_node, network_state)) {
      return true_expr->eval(this_node, network_state);
    }
    return false_expr->eval(this_node, network_state);
  }

  bool hasCycle(Node* node) const {
    return cond_expr->hasCycle(node) || true_expr->hasCycle(node) || false_expr->hasCycle(node);
  }

  void display(std::ostream& os) const {
    cond_expr->display(os);
    os <<  " ? ";
    true_expr->display(os);
    os <<  " : ";
    false_expr->display(os);
  }

  bool isConstantExpression() const {
    return cond_expr->isConstantExpression() && true_expr->isConstantExpression() && false_expr->isConstantExpression();
  }

  bool isLogicalExpression() const {
    return true_expr->isLogicalExpression() && false_expr->isLogicalExpression();
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

  virtual ~CondExpression() {
    delete cond_expr;
    delete true_expr;
    delete false_expr;
  }
};

class ConstantExpression : public Expression {

  double value;

public:
  ConstantExpression(double value) : value(value) { }

  Expression* clone() const {return new ConstantExpression(value);}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return value;
  }

  bool hasCycle(Node* node) const {
    return false;
  }

  void display(std::ostream& os) const {
    os << value;
  }

  bool isConstantExpression() const {return true;}

  bool isLogicalExpression() const {return value == 0 || value == 1;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

};

class SymbolExpression : public Expression {

  SymbolTable* symbol_table;
  const Symbol* symbol;
  mutable bool value_set;
  mutable double value;

public:
  SymbolExpression(SymbolTable* symbol_table, const Symbol* symbol) : symbol_table(symbol_table), symbol(symbol), value_set(false) { 
    symbol_table->addSymbolExpression(this);
  }

  Expression* clone() const {return new SymbolExpression(symbol_table, symbol);}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    if (!value_set) {
      value = symbol_table->getSymbolValue(symbol);
      value_set = true;
    }
    return value;
  }

  bool hasCycle(Node* node) const {
    return false;
  }

  void display(std::ostream& os) const {
    os << symbol->getName();
  }

  bool isConstantExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

  void unset() { value_set = false; }
};

class AliasExpression : public Expression {
  std::string identifier;

  const Expression* getAliasExpression(const Node* this_node) const {
    if (NULL != this_node) {
      return this_node->getAttributeExpression(identifier);
    }
    return NULL;
  }

  mutable const Expression* alias_expr;

public:
  AliasExpression(const std::string& identifier) : identifier(identifier), alias_expr(NULL) { }

  Expression* clone() const {return new AliasExpression(identifier);}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    if (NULL == alias_expr) {
      alias_expr = getAliasExpression(this_node);
    }
    if (NULL != alias_expr) {
      return alias_expr->eval(this_node, network_state);
    }
    if (NULL != this_node) {
      throw BNException("invalid use of alias attribute @" + identifier + " in node " + this_node->getLabel());
    }
    throw BNException("invalid use of alias attribute @" + identifier + " in unknown node");
  }

  bool hasCycle(Node* node) const {
    return false;
  }

  void display(std::ostream& os) const {
    os << '@' << identifier;
  }

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class OrLogicalExpression : public BinaryExpression {

public:
  OrLogicalExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }
  Expression* cloneAndShrink(bool& shrinked) const;

  Expression* clone() const {return new OrLogicalExpression(left->clone(), right->clone());}

  bool generationWillAddParenthesis() const {return true;}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return (double)((bool)left->eval(this_node, network_state) || (bool)right->eval(this_node, network_state));
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " OR ";
    right->display(os);
  }

  virtual bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class AndLogicalExpression : public BinaryExpression {

public:
  AndLogicalExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new AndLogicalExpression(left->clone(), right->clone());}
  Expression* cloneAndShrink(bool& shrinked) const;

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return (double)((bool)left->eval(this_node, network_state) && (bool)right->eval(this_node, network_state));
  }

  bool generationWillAddParenthesis() const {return true;}

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " AND ";
    right->display(os);
  }

  bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class XorLogicalExpression : public BinaryExpression {

public:
  XorLogicalExpression(Expression* left, Expression* right) : BinaryExpression(left, right) { }

  Expression* clone() const {return new XorLogicalExpression(left->clone(), right->clone());}

  Expression* cloneAndShrink(bool& shrinked) const;

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return (double)((bool)left->eval(this_node, network_state) ^ (bool)right->eval(this_node, network_state));
  }

  void display(std::ostream& os) const {
    left->display(os);
    os <<  " XOR ";
    right->display(os);
  }

  bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;
};

class NotLogicalExpression : public Expression {
  Expression* expr;

public:
  NotLogicalExpression(Expression* expr) : expr(expr) { }

  Expression* clone() const {return new NotLogicalExpression(expr->clone());}
  Expression* cloneAndShrink(bool& shrinked) const;

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return (double)(!((bool)expr->eval(this_node, network_state)));
  }

  bool hasCycle(Node* node) const {
    return expr->hasCycle(node);
  }

  const NotLogicalExpression* asNotLogicalExpression() const {return this;}

  void display(std::ostream& os) const {
    os <<  "NOT ";
    expr->display(os);
  }

  bool isLogicalExpression() const {return true;}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

  ~NotLogicalExpression() {
    delete expr;
  }
};

class ParenthesisExpression : public Expression {
  Expression* expr;

public:
  ParenthesisExpression(Expression* expr) : expr(expr) { }

  Expression* clone() const {return new ParenthesisExpression(expr->clone());}

  Expression* cloneAndShrink(bool& shrinked) const {
    return new ParenthesisExpression(expr->cloneAndShrink(shrinked));
  }

  bool generationWillAddParenthesis() const {return true;}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    return expr->eval(this_node, network_state);
  }

  const NotLogicalExpression* asNotLogicalExpression() const {return expr->asNotLogicalExpression();}

  bool hasCycle(Node* node) const {
    return expr->hasCycle(node);
  }

  void display(std::ostream& os) const {
    os <<  '(';
    expr->display(os);
    os <<  ')';
  }

  bool isConstantExpression() const {return expr->isConstantExpression();}
  bool isLogicalExpression() const {return expr->isLogicalExpression();}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const;

  virtual ~ParenthesisExpression() {
    delete expr;
  }
};

class ArgumentList {
  std::vector<Expression*> expr_v;

public:
  ArgumentList() { }

  void push_back(Expression* expr) {expr_v.push_back(expr);}

  ArgumentList* clone() const {
    ArgumentList* arg_list_cloned = new ArgumentList();
    for (std::vector<Expression*>::const_iterator iter = expr_v.begin(); iter != expr_v.end(); ++iter) {
      arg_list_cloned->push_back(*iter);
    }
    return arg_list_cloned;
  }

  bool hasCycle(Node* node) const {
    for (std::vector<Expression*>::const_iterator iter = expr_v.begin(); iter != expr_v.end(); ++iter) {
      if ((*iter)->hasCycle(node)) {
	return true;
      }
    }
    return false;
  }

  bool isConstantExpression() const {
    for (std::vector<Expression*>::const_iterator iter = expr_v.begin(); iter != expr_v.end(); ++iter) {
      if (!(*iter)->isConstantExpression()) {
	return false;
      }
    }
    return true;
  }

  void display(std::ostream& os) const {
    unsigned int nn = 0;
    for (std::vector<Expression*>::const_iterator iter = expr_v.begin(); iter != expr_v.end(); ++iter) {
      os << (nn > 0 ? ", " : "");
      (*iter)->display(os);
      nn++;
    }
  }

  const std::vector<Expression*>& getExpressionList() const { return expr_v; }
  size_t getExpressionListCount() { return expr_v.size(); }

  ~ArgumentList() {
    for (std::vector<Expression*>::iterator iter = expr_v.begin(); iter != expr_v.end(); ++iter) {
      delete *iter;
    }
  }
};

class FuncCallExpression : public Expression {
  std::string funname;
  ArgumentList* arg_list;
  Function* function;
  bool is_const = false;
  double value;

public:
  FuncCallExpression(const std::string& funname, ArgumentList* arg_list) : funname(funname), arg_list(arg_list), function(NULL), value(0.) {
    function = Function::getFunction(funname);

    if (function == NULL) {
      throw BNException("unknown function " + funname);
    }
    function->check(arg_list);
    
    // This part is evaluating the formula, but if there is a parameter its value
    // has not been parsed yet. So this will fail. 
    // is_const = function->isDeterministic() && isConstantExpression();
    // if (is_const) {
    //   NetworkState network_state;
    //   value = function->eval(NULL, network_state, arg_list);
    // }
 }

  Expression* clone() const {return new FuncCallExpression(funname, arg_list->clone());}

  double eval(const Node* this_node, const NetworkState& network_state) const {
    if (is_const) {
      return value;
    }
    return function->eval(this_node, network_state, arg_list);
  }

  void display(std::ostream& os) const {
    os <<  funname << '(';
    arg_list->display(os);
    os <<  ')';
  }

  bool hasCycle(Node* node) const {
    return arg_list->hasCycle(node);
  }

  bool isConstantExpression() const {return arg_list->isConstantExpression();}

  void generateLogicalExpression(LogicalExprGenContext& genctx) const {
    // for now
  }

  virtual ~FuncCallExpression() {
    delete arg_list;
  }
};

class IStateGroup {

public:
  struct ProbaIState {
    double proba_value;
    std::vector<double>* state_value_list;

    ProbaIState(Expression* proba_expr, std::vector<Expression*>* state_expr_list) {
      NetworkState network_state;
      proba_value = proba_expr->eval(NULL, network_state);
      std::vector<Expression*>::iterator begin = state_expr_list->begin();
      std::vector<Expression*>::iterator end = state_expr_list->end();
      state_value_list = new std::vector<double>();
      while (begin != end) {
	NetworkState network_state;
	state_value_list->push_back((*begin)->eval(NULL, network_state));
	++begin;
      }
    }
  
    // only one node
    ProbaIState(double proba_value, Expression* state_expr) {
      this->proba_value = proba_value;
      state_value_list = new std::vector<double>();
      NetworkState network_state;
      state_value_list->push_back(state_expr->eval(NULL, network_state));
    }

    ProbaIState(double proba_value, double istate_value) {
      this->proba_value = proba_value;
      state_value_list = new std::vector<double>();
      state_value_list->push_back(istate_value);
    }
    
    ProbaIState(ProbaIState* obj) {
      this->proba_value = obj->getProbaValue();
      this->state_value_list = new std::vector<double>(*(obj->getStateValueList()));
    }

    ~ProbaIState() {
      delete state_value_list;
    }
    double getProbaValue() {return proba_value;}
    std::vector<double>* getStateValueList() {return state_value_list;}
    void normalizeProbaValue(double proba_sum) {proba_value /= proba_sum;}
  };
  
  IStateGroup(Network* network, std::vector<const Node*>* nodes, std::vector<ProbaIState*>* proba_istates, std::string& error_msg) : nodes(nodes), proba_istates(proba_istates) {
    is_random = false;
    size_t node_size = nodes->size();
    std::vector<IStateGroup::ProbaIState*>::iterator begin = proba_istates->begin();
    std::vector<IStateGroup::ProbaIState*>::iterator end = proba_istates->end();
    while (begin != end) {
      if ((*begin)->getStateValueList()->size() != node_size) {
	std::ostringstream ostr;
	ostr << "size inconsistency in istate expression: got " <<  (*begin)->getStateValueList()->size() << " states, has " << node_size << " nodes";
	error_msg = ostr.str();
	return;
      }
      ++begin;
    }
    epilogue(network);
 }

  IStateGroup(Network * network, const Node* node) {
    is_random = true;
    nodes = new std::vector<const Node*>();
    nodes->push_back(node);
    proba_istates = new std::vector<ProbaIState*>();
    proba_istates->push_back(new ProbaIState(0.5, 0.));
    proba_istates->push_back(new ProbaIState(0.5, 1.));
    epilogue(network);
  }

  IStateGroup(Network * network, const Node* node, Expression* expr) {
    is_random = false;
    nodes = new std::vector<const Node*>();
    nodes->push_back(node);
    proba_istates = new std::vector<ProbaIState*>();
    proba_istates->push_back(new ProbaIState(1., expr));
    epilogue(network);
  }
  
  IStateGroup(IStateGroup* obj, Network* network) {
    this->is_random = obj->isRandom();
    this->nodes = new std::vector<const Node*>();
    for (const auto node: *(obj->getNodes())) {
      this->nodes->push_back(node);
    }
    this->proba_istates = new std::vector<ProbaIState*>();
    for(auto proba_istate: *(obj->getProbaIStates())) {
      this->proba_istates->push_back(new ProbaIState(proba_istate));
    }
    epilogue(network);
  }
  
  ~IStateGroup() {
    delete nodes;
    for (std::vector<IStateGroup::ProbaIState*>::iterator it = proba_istates->begin(); it != proba_istates->end(); ++it) {
      delete *it;
    }
    delete proba_istates;
  }

  std::vector<const Node*>* getNodes() {return nodes;}
  std::vector<ProbaIState*>* getProbaIStates() {return proba_istates;}
  double getProbaSum() const {return proba_sum;}

  bool isRandom() const {return is_random;}

  bool hasNode(Node * node) {
    for(std::vector<const Node*>::iterator it = nodes->begin(); it != nodes->end(); it++) {
      if (node == *it)
        return true;
    }
    return false;
  }

  static void checkAndComplete(Network* network);
  static void initStates(Network* network, NetworkState& initial_state, RandomGenerator * randgen);
  static void display(Network* network, std::ostream& os);
  static void reset(Network* network);
  

    static void setNodeProba(Network * network, Node * node, double value) {

    std::vector<IStateGroup*>::iterator begin = network->getIStateGroup()->begin();
    std::vector<IStateGroup*>::iterator end = network->getIStateGroup()->end();

    while (begin != end) {
      IStateGroup* istate_group = *begin;

      if (istate_group->hasNode(node)) {

        std::vector<IStateGroup::ProbaIState*>* proba_istates = istate_group->getProbaIStates();
        std::vector<const Node*>* group_nodes = istate_group->getNodes();

        // Simple case, if the node is alone
        if (group_nodes->size() == 1) {

          // Do we also need to free memory for all ProbaIStaste ? 
          for (size_t i=0; i < proba_istates->size(); i++) {
            delete (*proba_istates)[i];
          }
          proba_istates->clear();

          // Again, simple case, we just need one ProbaIState
          if (value == 0.0 || value == 1.0) {            
            proba_istates->push_back(new ProbaIState(1.0, value));

          } else {
            proba_istates->push_back(new ProbaIState(1.0-value, 0.0));
            proba_istates->push_back(new ProbaIState(value, 1.0));
          }
        }
        // Otherwise, the simplest might be to remove it and create a new IStateGroup with this node alone
        else {

          //First, we erase it from nodes, and get its index
          size_t i;
          for(i = 0; i < group_nodes->size(); i++) {
              if (group_nodes->at(i) == node) {
                group_nodes->erase(group_nodes->begin() + (std::ptrdiff_t) i);
                break;
              }
          }

          // Then we erase it from the state_value_list of each proba_istates
          for(std::vector<IStateGroup::ProbaIState*>::iterator it = proba_istates->begin(); it != proba_istates->end(); it++) {
            (*it)->state_value_list->erase((*it)->state_value_list->begin() + (std::ptrdiff_t) i);
          }

          // Finally, we add a proba_istate with the desired value
          std::vector<const Node*>* new_nodes = new std::vector<const Node*>();

          new_nodes->push_back(node);

          std::vector<IStateGroup::ProbaIState*>* new_proba_istates = new std::vector<IStateGroup::ProbaIState*>();

          if (value == 0.0 || value == 1.0) {            
            new_proba_istates->push_back(new ProbaIState(1.0, value));

          } else {
            new_proba_istates->push_back(new ProbaIState(1.0-value, 0.0));
            new_proba_istates->push_back(new ProbaIState(value, 1.0));
          }

          std::string message = "";

          new IStateGroup(network, new_nodes, new_proba_istates, message);
        }
      }
      ++begin;
    }
  }

static void setInitialState(Network * network, NetworkState * state) {

  const std::vector<Node *> nodes = network->getNodes();

  std::vector<Node *>::const_iterator it = nodes.begin();

  while(it != nodes.end()) 
  {
    // std::cout << "Setting " << (*it)->getLabel() << " to " << (*it)->getNodeState(*state) << std::endl;
    setNodeProba(network, *it, (*it)->getNodeState(*state));

    it++;
  }

}
private:
  std::vector<const Node*>* nodes;
  std::vector<ProbaIState*>* proba_istates;
  double proba_sum;
  bool is_random;

  void computeProbaSum() {
    std::vector<ProbaIState*>::iterator bb = proba_istates->begin();
    std::vector<ProbaIState*>::iterator ee = proba_istates->end();
    proba_sum = 0;
    while (bb != ee) {
      proba_sum += (*bb)->getProbaValue();
      ++bb;
    }
    bb = proba_istates->begin();
    while (bb != ee) {
      (*bb)->normalizeProbaValue(proba_sum);
      ++bb;
    }
  }

  void epilogue(Network* network) {
    computeProbaSum();
    network->getIStateGroup()->push_back(this);
  }
};

extern const bool backward_istate;

extern bool dont_shrink_logical_expressions;
extern int setConfigVariables(Network* network, const std::string& prog, const std::string& runvar);
extern int setConfigVariables(Network* network, const std::string& prog, std::vector<std::string>& runvar_v);

#endif
