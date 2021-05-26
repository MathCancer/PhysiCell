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
     StochasticSimulationEngine.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#include "StochasticSimulationEngine.h"
#include "Probe.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>

const std::string StochasticSimulationEngine::VERSION = "1.0.0";

NodeIndex StochasticSimulationEngine::getTargetNode(RandomGenerator *random_generator, const MAP<NodeIndex, double> &nodeTransitionRates, double total_rate) const
{
  double U_rand2 = random_generator->generate();
  double random_rate = U_rand2 * total_rate;
  MAP<NodeIndex, double>::const_iterator begin = nodeTransitionRates.begin();
  MAP<NodeIndex, double>::const_iterator end = nodeTransitionRates.end();
  NodeIndex node_idx = INVALID_NODE_INDEX;
  while (begin != end && random_rate > 0.)
  {
    node_idx = (*begin).first;
    double rate = (*begin).second;
    random_rate -= rate;
    ++begin;
  }

  assert(node_idx != INVALID_NODE_INDEX);
  assert(network->getNode(node_idx)->getIndex() == node_idx);
  return node_idx;
}

NetworkState StochasticSimulationEngine::run(NetworkState& initial_state, std::ostream *output_traj)
{
  const std::vector<Node *> &nodes = network->getNodes();
  std::vector<Node *>::const_iterator begin = nodes.begin();
  std::vector<Node *>::const_iterator end = nodes.end();
  NetworkState network_state;
    
  // if ( != NULL) {
    network_state = initial_state;
  // } else {
  //   network->initStates(network_state, random_generator);
  // }
  
  double tm = 0.;
  unsigned int step = 0;
  if (NULL != output_traj)
  {
    // (*output_traj) << "\nTrajectory #" << (nn+1) << '\n';
    (*output_traj) << " istate\t";
    network_state.displayOneLine(*output_traj, network);
    (*output_traj) << '\n';
  }
  while (tm < max_time)
  {
    double total_rate = 0.;
    MAP<NodeIndex, double> nodeTransitionRates;
    begin = nodes.begin();

    while (begin != end)
    {
      Node *node = *begin;
      NodeIndex node_idx = node->getIndex();
      if (node->getNodeState(network_state))
      {
        double r_down = node->getRateDown(network_state);
        if (r_down != 0.0)
        {
          total_rate += r_down;
          nodeTransitionRates[node_idx] = r_down;
        }
      }
      else
      {
        double r_up = node->getRateUp(network_state);
        if (r_up != 0.0)
        {
          total_rate += r_up;
          nodeTransitionRates[node_idx] = r_up;
        }
      }
      ++begin;
    }

    // double TH;
    if (total_rate == 0) {
      tm = max_time;
    } else {
      
      double transition_time ;
      if (discrete_time) {
        transition_time = time_tick;
      } else {
        double U_rand1 = random_generator->generate();
        transition_time = -log(U_rand1) / total_rate;
      }
      
      tm += transition_time;
    }

    if (NULL != output_traj)
    {
      (*output_traj) << std::setprecision(10) << tm << '\t';
      network_state.displayOneLine(*output_traj, network);
      (*output_traj) << std::endl;
    }

    if (tm >= max_time)
      break;

    NodeIndex node_idx = getTargetNode(random_generator, nodeTransitionRates, total_rate);
    network_state.flipState(network->getNode(node_idx));
    step++;
  }
  
  return network_state;
}
