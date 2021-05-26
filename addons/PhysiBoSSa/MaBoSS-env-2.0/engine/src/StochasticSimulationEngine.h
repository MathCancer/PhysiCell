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
     StochasticSimulationEngine.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _STOCHASTICSIMULATIONENGINE_H_
#define _STOCHASTICSIMULATIONENGINE_H_

#include <string>
#include <map>
#include <vector>
#include <assert.h>

#include "BooleanNetwork.h"
#include "RandomGenerator.h"
#include "RunConfig.h"


class StochasticSimulationEngine {

  Network* network;
  RunConfig* runconfig;

  // Duration of the simulation
  double max_time;
  
  // Using discrete time
  bool discrete_time;
  
  // Time tick for discrete time
  double time_tick;
  

  NodeIndex getTargetNode(RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const;

public:
  static const std::string VERSION;
  RandomGenerator *random_generator;

  
  StochasticSimulationEngine(Network* network, RunConfig* runconfig, int seed): network(network), runconfig(runconfig), max_time(runconfig->getMaxTime()), discrete_time(runconfig->isDiscreteTime()), time_tick(runconfig->getTimeTick()) {
    random_generator = runconfig->getRandomGeneratorFactory()->generateRandomGenerator(seed);
  }
  ~StochasticSimulationEngine() { delete random_generator; }
  
  
  void setSeed(int _seed) { 
    random_generator->setSeed(_seed); 
  }
  void setMaxTime(double _max_time) { this->max_time = _max_time; }
  void setDiscreteTime(bool _discrete_time) { this->discrete_time = _discrete_time; }
  void setTimeTick(double _time_tick) { this->time_tick = _time_tick; }
  
  NetworkState run(NetworkState& initial_state, std::ostream* output_traj = NULL);
};

#endif
