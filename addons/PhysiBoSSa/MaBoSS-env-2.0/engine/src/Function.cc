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
     Function.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     July 2018
*/

#include "BooleanNetwork.h"
#include <iostream>

std::map<std::string, Function*>* Function::func_map;

Function::Function(const std::string& funname, unsigned int min_args, unsigned int max_args) : funname(funname), min_args(min_args), max_args(max_args == ~0U ? min_args : max_args)
{
  if (NULL == func_map) {
    func_map = new std::map<std::string, Function*>();
  }
  (*func_map)[funname] = this;
}

void Function::check(ArgumentList* arg_list)
{
  size_t count = arg_list->getExpressionListCount();
  if (count < min_args) {
    throw BNException("not enough parameters in " + funname + " call");
  }
  if (count > max_args) {
    throw BNException("too many parameters in " + funname + " call");
  }
}

Function* Function::getFunction(const std::string& funname)
{
  if (func_map == NULL) {return NULL;}
  std::map<std::string, Function*>::iterator iter = func_map->find(funname);
  if (iter == func_map->end()) {
    return NULL;
  }
  return iter->second;
}

void Function::displayFunctionDescriptions(std::ostream& os)
{
  if (func_map == NULL) {return;}
  for (std::map<std::string, Function*>::iterator iter = func_map->begin(); iter != func_map->end(); ++iter) {
    os << "  " << iter->second->getDescription() << "\n\n";
  }
}
