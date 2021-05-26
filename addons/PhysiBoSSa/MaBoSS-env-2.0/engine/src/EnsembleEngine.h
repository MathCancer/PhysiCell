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
     EnsembleEngine.h

   Authors:
     Vincent Noel <contact@vincent-noel.fr>
 
   Date:
     March 2019
*/

#ifndef _ENSEMBLEENGINE_H_
#define _ENSEMBLEENGINE_H_

#include <string>
#include <map>
#include <vector>
#include <assert.h>

#include "MetaEngine.h"
#include "BooleanNetwork.h"
#include "Cumulator.h"
#include "RandomGenerator.h"
#include "RunConfig.h"

struct EnsembleArgWrapper;

class EnsembleEngine : public MetaEngine {

  std::vector<Network*> networks;
  std::vector<Cumulator*> cumulators_per_model; // The final Cumulators for each model
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > fixpoints_per_model; // The final fixpoints for each model
  
  bool save_individual_result; // Do we want to save individual model simulation result
  bool random_sampling; // Randomly select the number of simulation per model

  std::vector<std::vector<unsigned int> > simulation_indices_v; // The list of indices of models to simulate for each thread
  std::vector<std::vector<Cumulator*> > cumulator_models_v; // The results for each model, by thread
  std::vector<std::vector<Cumulator*> > cumulators_thread_v; // The results for each model, by model
  std::vector<std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*> > fixpoints_models_v; // The fixpoints for each model, by thread
  std::vector<std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*> > fixpoints_threads_v; // The fixpoints for each model, by thread

  std::vector<EnsembleArgWrapper*> arg_wrapper_v;
  NodeIndex getTargetNode(Network* network, RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const;
  double computeTH(const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const;
  void epilogue();
  static void* threadWrapper(void *arg);
  void runThread(Cumulator* cumulator, unsigned int start_count_thread, unsigned int sample_count_thread, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj, std::vector<unsigned int> simulation_ind, std::vector<Cumulator*> t_models_cumulators, std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > t_models_fixpoints);
  void mergeEnsembleFixpointMaps();

public:
  static const std::string VERSION;

  EnsembleEngine(std::vector<Network*> network, RunConfig* runconfig, bool save_individual_result=false, bool random_sampling=false);

  void run(std::ostream* output_traj);

  void displayIndividual(unsigned int model_id, std::ostream& output_probtraj, std::ostream& output_statdist, std::ostream& output_fp, bool hexfloat = false) const;

  ~EnsembleEngine();
};

#endif
