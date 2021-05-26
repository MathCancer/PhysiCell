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
     Function.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
   July 2018
*/

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <vector>
#include <map>
#include <math.h>

class ArgumentList;
class Expression;
class Node;
class NetworkState;
class Function;

class Function {
  std::string funname;
  unsigned int min_args;
  unsigned int max_args;
  static std::map<std::string, Function*>* func_map;

protected:
  Function(const std::string& funname, unsigned int min_args, unsigned int max_args = ~0U);

public:
  const std::string& getFunName() const {return funname;}
  unsigned int getMinArgs() const {return min_args;}
  unsigned int getMaxArgs() const {return max_args;}

  static Function* getFunction(const std::string& funname);

  virtual bool isDeterministic() const {return true;}

  virtual std::string getDescription() const = 0;

  void check(ArgumentList* arg_list);

  virtual double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) = 0;

  static void displayFunctionDescriptions(std::ostream& os);
  static std::map<std::string, Function*>* getFuncMap() {return func_map;}
  static void setFuncMap(std::map<std::string, Function*>* _func_map) {func_map = _func_map;}
  static void destroyFuncMap() { 
     for (std::map<std::string, Function*>::iterator it = func_map->begin(); it != func_map->end(); ++it) {
        delete it->second;
      }
     delete func_map; 
    }
    virtual ~Function() {}
};

#endif
