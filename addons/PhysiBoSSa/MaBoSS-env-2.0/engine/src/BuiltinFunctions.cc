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
     BuiltinFunctions.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     July 2018
*/

#include "BooleanNetwork.h"

//
// User function definition
//

class LogFunction : public Function {

public:
  LogFunction() : Function("log", 1, 2) { }

  double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) {
    const std::vector<Expression*>& expr_v = arg_list->getExpressionList();
    std::vector<Expression*>::const_iterator iter = expr_v.begin();
    double val = (*iter)->eval(this_node, network_state);
    if (expr_v.size() == 1) {
      return log(val);
    }

    iter++;
    double base = (*iter)->eval(this_node, network_state);
    return log(val) / log(base);
  }

  std::string getDescription() const {
    return "double log(double VALUE[, double BASE=e])\n  computes the value of the natural logarithm of VALUE; uses BASE if set";
  }
};

class ExpFunction : public Function {

public:
  ExpFunction() : Function("exp", 1, 2) { }

  double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) {
    const std::vector<Expression*>& expr_v = arg_list->getExpressionList();
    std::vector<Expression*>::const_iterator iter = expr_v.begin();
    double val = (*iter)->eval(this_node, network_state);
    if (expr_v.size() == 1) {
      return exp(val);
    }

    iter++;
    double base = (*iter)->eval(this_node, network_state);
    return exp(val * log(base));
  }

  std::string getDescription() const {
    return "double exp(double VALUE[, double BASE=e])\n  computes the base-e exponential of VALUE; uses BASE if set";
  }
};

void builtin_functions_init()
{
  static int init = false;
  if (!init) {
    new LogFunction();
    new ExpFunction();
    init = true;
  }
}

