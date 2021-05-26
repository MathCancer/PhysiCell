
%{
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
     RunConfigGrammar.l

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
     updated to add extended initial conditions: October 2014
*/

#define _POSIX_SOURCE 1
#include "BooleanNetwork.h"
#include "RunConfig.h"

extern int yylex();

static void yyerror(const char *s);
static Network* network;
static RunConfig* config;

extern std::string yy_error_head();
%}

%union {
  Expression* expr;
  char* str;
  double d;
  long long l;
  std::vector<const Node*>* node_list;
  std::vector<Expression*>* expr_list;
  IStateGroup::ProbaIState* istate_expr;
  std::vector<IStateGroup::ProbaIState*>* istate_expr_list;
  ArgumentList* arg_list;

}

%type<expr> primary_expression 
%type<expr> postfix_expression 
%type<expr> unary_expression 
%type<expr> multiplicative_expression 
%type<expr> additive_expression 
%type<expr> relational_expression 
%type<expr> equality_expression
%type<expr> logical_and_expression
%type<expr> logical_or_expression
%type<expr> logical_xor_expression
%type<expr> conditional_expression
%type<expr> expression
%type<node_list> symbol_list
%type<node_list> symbol_istate_list
%type<expr_list> expression_list
%type<istate_expr> istate_expression
%type<istate_expr_list> istate_expression_list
%type<arg_list> argument_expression_list

%token<str> VARIABLE
%token<str> SYMBOL
%token<d> DOUBLE
%token<l> INTEGER

%token LOGAND LOGOR LOGXOR LOGNOT EQUAL NOT_EQUAL NODE GTEQ LTEQ

%%

translation_unit: decl
{
}
| translation_unit decl
{
}
;

decl: var_decl
| runconfig_decl
| node_attr_decl
| ';'
;

runconfig_decl: SYMBOL '=' expression ';'
{
  NetworkState network_state;
  double value = $3->eval(NULL, network_state);
  config->setParameter($1, value);
  free($1);
  delete $3;
}
;

node_attr_decl: SYMBOL '.' SYMBOL '=' expression ';'
{
  Node* node = network->getNode($1);
  NetworkState network_state;
  double value = $5->eval(NULL, network_state);
  if (!strcasecmp($3, "istate")) {
    if (backward_istate) {
      node->setIState((bool)value);
    } else {
      if (value < 0) {
	new IStateGroup(network, node);
      } else {
	new IStateGroup(network, node, $5);
      }
    }
  } else if (!strcasecmp($3, "is_internal")) {
    node->isInternal((bool)value);
  } else if (!strcasecmp($3, "refstate")) {
    if (value < 0) {
      node->setReference(false);
    } else {
      node->setReferenceState((NodeState)value);
    }
  } else {
    throw BNException(std::string(yy_error_head() + "invalid node attribute: ") + $3 + ", valid attributes are: istate or is_internal");
  }

  free($1);
  free($3);
  delete $5;
}
| symbol_istate_list '.' SYMBOL '=' istate_expression_list ';'
{
  if (strcasecmp($3, "istate")) {
    throw BNException(std::string(yy_error_head() + "invalid node group attribute: ") + $3 + ", valid attribute is istate");
  }
  std::string error_msg;
  new IStateGroup(network, $1, $5, error_msg);
  if (error_msg.length() > 0) {
    throw BNException(std::string(yy_error_head() + error_msg));
  }
  
  free($3);
}
;

symbol_istate_list: '[' symbol_list ']'
{
  $$ = $2;
}
;

symbol_list: SYMBOL
{
  $$ = new std::vector<const Node*>();
  $$->push_back(network->getNode($1));
  free($1);
}
| symbol_list ',' SYMBOL
{
  $$ = $1;
  $$->push_back(network->getNode($3));
}
;

istate_expression_list: istate_expression
{
  $$ = new std::vector<IStateGroup::ProbaIState*>();
  $$->push_back($1);
}
| istate_expression_list ',' istate_expression
{
  $$ = $1;
  $$->push_back($3);
}
;

istate_expression: primary_expression '[' expression_list ']'
{
  $$ = new IStateGroup::ProbaIState($1, $3);
  delete $1;
  for (std::vector<Expression*>::iterator it = $3->begin(); it != $3->end(); ++it) {
    delete *it;
  }
  delete $3;
}
;

expression_list: primary_expression
{
  $$ = new std::vector<Expression*>();
  $$->push_back($1);
}
| expression_list ',' primary_expression
{
  $$ = $1;
  $$->push_back($3);
}
;

var_decl: VARIABLE '=' expression ';'
{
  const Symbol* symbol = network->getSymbolTable()->getOrMakeSymbol($1);
  free($1);
  NetworkState dummy_state;
  network->getSymbolTable()->setSymbolValue(symbol, $3->eval(NULL, dummy_state));
  delete $3;
}
;

primary_expression: INTEGER
{
  $$ = new ConstantExpression($1);
}
| DOUBLE
{
  $$ = new ConstantExpression($1);
}
| VARIABLE
{
  $$ = new SymbolExpression(network->getSymbolTable(), network->getSymbolTable()->getOrMakeSymbol($1));
  free($1);
}
| '(' expression ')'
{
  $$ = new ParenthesisExpression($2);
}
;

argument_expression_list: expression
{
  $$ = new ArgumentList();
  $$->push_back($1);
}
| argument_expression_list ',' expression
{
  $$ = $1;
  $$->push_back($3);
}
;


postfix_expression: primary_expression
{
  $$ = $1;
}
| SYMBOL '(' argument_expression_list ')'
{
  $$ = new FuncCallExpression($1, $3);
  free($1);
}
| SYMBOL '(' ')'
{
  $$ = new FuncCallExpression($1, NULL);
  free($1);
}
;


unary_expression: postfix_expression
{
  $$ = $1;
}
| '+' unary_expression
{
  $$ = $2;
}
| '-' unary_expression
{
  $$ = new SubExpression(new ConstantExpression(0.0), $2);
}
| '!' unary_expression
{
  $$ = new NotLogicalExpression($2);
}
| LOGNOT unary_expression
{
  $$ = new NotLogicalExpression($2);
}
;

multiplicative_expression: unary_expression
{
  $$ = $1;
}
| multiplicative_expression '*' unary_expression
{
  $$ = new MulExpression($1, $3);
}
| multiplicative_expression '/' unary_expression
{
  $$ = new DivExpression($1, $3);
}
;

additive_expression: multiplicative_expression
{
  $$ = $1;
}
| additive_expression '+' multiplicative_expression
{
  $$ = new AddExpression($1, $3);
}
| additive_expression '-' multiplicative_expression
{
  $$ = new SubExpression($1, $3);
}
;

relational_expression: additive_expression
{
  $$ = $1;
}
| relational_expression '<' additive_expression
{
  $$ = new LetterExpression($1, $3);
}
| relational_expression '>' additive_expression
{
  $$ = new GreaterExpression($1, $3);
}
| relational_expression LTEQ additive_expression
{
  $$ = new LetterOrEqualExpression($1, $3);
}
| relational_expression GTEQ additive_expression
{
  $$ = new GreaterOrEqualExpression($1, $3);
}
;

equality_expression: relational_expression
{
  $$ = $1;
}
| equality_expression EQUAL relational_expression
{
  $$ = new EqualExpression($1, $3);
}
| equality_expression NOT_EQUAL relational_expression
{
  $$ = new NotEqualExpression($1, $3);
}
;

logical_and_expression: equality_expression
{
  $$ = $1;
}
| logical_and_expression LOGAND equality_expression
{
  $$ = new AndLogicalExpression($1, $3);
}
;

logical_or_expression: logical_and_expression
{
  $$ = $1;
}
| logical_or_expression LOGOR logical_and_expression
{
  $$ = new OrLogicalExpression($1, $3);
}
;

logical_xor_expression: logical_or_expression
{
  $$ = $1;
}
| logical_xor_expression LOGXOR logical_or_expression
{
  $$ = new XorLogicalExpression($1, $3);
}
| logical_xor_expression '^' logical_or_expression
{
  $$ = new XorLogicalExpression($1, $3);
}
;

conditional_expression: logical_xor_expression
{
  $$ = $1;
}
| logical_xor_expression '?' expression ':' conditional_expression
{
  $$ = new CondExpression($1, $3, $5);
}
;

expression: conditional_expression
{
  $$ = $1;
}
;

%%

void runconfig_setNetwork(Network* _network)
{
  network = _network;
}

void runconfig_setConfig(RunConfig* _config)
{
  config = _config;
}

#include "lex.RC.cc"

void yy_scan_expression(const char* str)
{
    yy_switch_to_buffer(yy_scan_string(str));
}