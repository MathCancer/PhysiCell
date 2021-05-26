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
     EnsembleEngine.cc

   Authors:
     Vincent Noel <contact@vincent-noel.fr>
 
   Date:
     March 2019
*/

#include "EnsembleEngine.h"
#include "Probe.h"
#include "Utils.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#ifndef WINDOWS
  #include <dlfcn.h>
#else
  #include <windows.h>
#endif
#include <iostream>

const std::string EnsembleEngine::VERSION = "1.2.0";

EnsembleEngine::EnsembleEngine(std::vector<Network*> networks, RunConfig* runconfig, bool save_individual_result, bool _random_sampling) :
  MetaEngine(networks[0], runconfig), networks(networks), save_individual_result(save_individual_result), random_sampling(_random_sampling) {

  if (thread_count > 1 && !runconfig->getRandomGeneratorFactory()->isThreadSafe()) {
    std::cerr << "Warning: non reentrant random, may not work properly in multi-threaded mode\n";
  }
  
  const std::vector<Node*>& nodes = networks[0]->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();

  NetworkState internal_state;
  bool has_internal = false;
  refnode_count = 0;
  while (begin != end) {
    Node* node = *begin;
    if (node->isInternal()) {
      has_internal = true;
      internal_state.setNodeState(node, true);
    }
    if (node->isReference()) {
      reference_state.setNodeState(node, node->getReferenceState());
      refnode_count++;
    }
    ++begin;
  }

  merged_cumulator = NULL;
  cumulator_v.resize(thread_count);

  simulation_indices_v.resize(thread_count); // Per thread
  std::vector<unsigned int> simulations_per_model(networks.size(), 0);

  // Here we write a dict with the number of simulation by model
  unsigned int network_index;
  if (random_sampling){
    // Here we need the random generator to compute the list of simulations
    RandomGeneratorFactory* randgen_factory = runconfig->getRandomGeneratorFactory();
    int seed = runconfig->getSeedPseudoRandom();
    RandomGenerator* random_generator = randgen_factory->generateRandomGenerator(seed);
    
    for (unsigned int nn = 0; nn < sample_count; nn++) {
      // This will need sample_count random numbers... maybe there is another way ?
      network_index = (unsigned int) floor(random_generator->generate()*networks.size());
      simulations_per_model[network_index] += 1;
    }
    
    delete random_generator;

  } else{
    
    for (unsigned int nn = 0; nn < networks.size(); ++nn) {
      if (nn == 0) {
        simulations_per_model[nn] = floor(sample_count/networks.size()) + (sample_count % networks.size());
      } else {
        simulations_per_model[nn] = floor(sample_count/networks.size());
      }
    }
  }
  
  cumulator_models_v.resize(thread_count); // Per thread
  fixpoints_models_v.resize(thread_count);

  if (save_individual_result) {
    cumulators_thread_v.resize(networks.size());
    fixpoints_threads_v.resize(networks.size());
  }

  unsigned int count = sample_count / thread_count;
  unsigned int firstcount = count + sample_count - count * thread_count;
  unsigned int position = 0;
  unsigned int offset = 0;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    unsigned int t_count = (nn == 0 ? firstcount : count);
    Cumulator* cumulator = new Cumulator(runconfig, runconfig->getTimeTick(), runconfig->getMaxTime(), t_count);
    if (has_internal) {
#ifdef USE_BITSET
      NetworkState_Impl state2 = ~internal_state.getState();
      cumulator->setOutputMask(state2);
#else
      cumulator->setOutputMask(~internal_state.getState());
#endif
    }
    cumulator_v[nn] = cumulator;

    // Setting the size of the list of indices to the thread's sample count
    simulation_indices_v[nn].resize(t_count); 

    // Here we build the indice of simulation for this thread
    // We have two counters : 
    // - nnn, which is the counter of simulation indices
    // - j, k which are the index of the model, and the counter of repetition of this model
    unsigned int nnn = 0; // Simulation indice (< t_count, thus inside a thread)
    unsigned int j = position; // Model indice
    unsigned int k = offset; // Indice of simulation for a model

    // Aside from the simulation indice, we are building
    // a dict with the number of model simulations by model
    int m = 0;
    std::vector<unsigned int> count_by_models;
    
    while(nnn < t_count) {
      assert(j <= networks.size());
      // If we assigned all the simulation of the model j
      if (k == simulations_per_model[j])
      {
        if (k > 0) { // If we indeed assigned something

          // We add another element to the vector
          // count_by_models.resize(count_by_models.size()+1); 
     
          // If this is the first model assigned in this thread
          if (m == 0) {
            // Then we need to count those who were assigned in the previous thread
            // count_by_models[m] = k - offset;
            count_by_models.push_back(k - offset);
          } else {
            // Otherwise we did assigned them all in this thread
            // count_by_models[m] = k;
            count_by_models.push_back(k);
          }
          
          m++; // One model assigned ! 
        }
        
        j++; // One model completely assigned
        k = 0; // We reset the model simulation counter
      // }
      // Otherwise, we keep assigning them
      } else {
        simulation_indices_v[nn][nnn] = j;
        k++; // One model simulation assigned
        nnn++; // One thread simulation assigned
      }
    }

    // If we didn't finished assigning this model, 
    // then we need to put in the counts up to where we went
    if (k > 0) {
      // We add another element to the vector
      // count_by_models.resize(count_by_models.size()+1); 

      // // If this is the first model assigned in this thread
      if (m == 0) {
        // Then we need to substract those who were assigned in the previous thread
        // count_by_models[m] = k - offset;
        count_by_models.push_back(k - offset);
      } else {
        // Otherwise we did assigned them all in this thread
        // count_by_models[m] = k;
        count_by_models.push_back(k);
      }
    }

    // Here we update the position and offset for the next thread
    // If we just finished a model, the position will be set to the next model
    if (k == simulations_per_model[j]) {
      offset = 0;
      position = ++j;

    // Otherwise we keep with this model
    } else {
      offset = k;
      position = j;
    }

    // If we want to save the individual trajectories, then we 
    // initialize the cumulators, and we add them to the vector of 
    // cumulators by model
    unsigned int c = 0;
    if (save_individual_result) {
      cumulator_models_v[nn].resize(count_by_models.size());
      fixpoints_models_v[nn].resize(count_by_models.size());

      for (nnn = 0; nnn < count_by_models.size(); nnn++) {
        if (count_by_models[nnn] > 0) {
          Cumulator* t_cumulator = new Cumulator(
            runconfig, runconfig->getTimeTick(), runconfig->getMaxTime(), count_by_models[nnn]
          );
      
          if (has_internal) {

#ifdef USE_BITSET
          NetworkState_Impl state2 = ~internal_state.getState();
          t_cumulator->setOutputMask(state2);
#else
          t_cumulator->setOutputMask(~internal_state.getState());
#endif
          }
          cumulator_models_v[nn][nnn] = t_cumulator;
          cumulators_thread_v[simulation_indices_v[nn][c]].push_back(t_cumulator);
        

          STATE_MAP<NetworkState_Impl, unsigned int>* t_fixpoints_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
          fixpoints_models_v[nn][nnn] = t_fixpoints_map;
          fixpoints_threads_v[simulation_indices_v[nn][c]].push_back(t_fixpoints_map);
        }
        c += count_by_models[nnn];
      }
    }
  }
}

NodeIndex EnsembleEngine::getTargetNode(Network* network, RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const
{
  double U_rand2 = random_generator->generate();
  double random_rate = U_rand2 * total_rate;
  MAP<NodeIndex, double>::const_iterator begin = nodeTransitionRates.begin();
  MAP<NodeIndex, double>::const_iterator end = nodeTransitionRates.end();
  NodeIndex node_idx = INVALID_NODE_INDEX;
  while (begin != end && random_rate > 0.) {
    node_idx = (*begin).first;
    double rate = (*begin).second;
    random_rate -= rate;
    ++begin;
  }

  assert(node_idx != INVALID_NODE_INDEX);
  assert(network->getNode(node_idx)->getIndex() == node_idx);
  return node_idx;
}

double EnsembleEngine::computeTH(const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const
{
  if (nodeTransitionRates.size() == 1) {
    return 0.;
  }

  MAP<NodeIndex, double>::const_iterator begin = nodeTransitionRates.begin();
  MAP<NodeIndex, double>::const_iterator end = nodeTransitionRates.end();
  double TH = 0.;
  double rate_internal = 0.;

  while (begin != end) {
    NodeIndex index = (*begin).first;
    double rate = (*begin).second;

    // Here we just check if the node is internal,
    // which should be the same for all model.
    // So we just take the first one, and we're good
    if (networks[0]->getNode(index)->isInternal()) {
      rate_internal += rate;
    }
    ++begin;
  }

  double total_rate_non_internal = total_rate - rate_internal;

  begin = nodeTransitionRates.begin();

  while (begin != end) {
    NodeIndex index = (*begin).first;
    double rate = (*begin).second;
    if (!networks[0]->getNode(index)->isInternal()) {
      double proba = rate / total_rate_non_internal;
      TH -= log2(proba) * proba;
    }
    ++begin;
  }

  return TH;
}

struct EnsembleArgWrapper {
  EnsembleEngine* mabest;
  unsigned int start_count_thread;
  unsigned int sample_count_thread;
  Cumulator* cumulator;

  std::vector<unsigned int> simulations_per_model;
  std::vector<Cumulator*> models_cumulators;
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > models_fixpoints;
  
  RandomGeneratorFactory* randgen_factory;
  int seed;
  STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map;
  std::ostream* output_traj;

  EnsembleArgWrapper(
    EnsembleEngine* mabest, unsigned int start_count_thread, unsigned int sample_count_thread, 
    Cumulator* cumulator, std::vector<unsigned int> simulations_per_model, 
    std::vector<Cumulator*> models_cumulators, std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > models_fixpoints,
    RandomGeneratorFactory* randgen_factory, int seed, 
    STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj) :

      mabest(mabest), start_count_thread(start_count_thread), sample_count_thread(sample_count_thread), 
      cumulator(cumulator), simulations_per_model(simulations_per_model), 
      models_cumulators(models_cumulators), models_fixpoints(models_fixpoints),
      randgen_factory(randgen_factory), seed(seed), 
      fixpoint_map(fixpoint_map), output_traj(output_traj) {

    }
};

void* EnsembleEngine::threadWrapper(void *arg)
{
  EnsembleArgWrapper* warg = (EnsembleArgWrapper*)arg;
  try {
    warg->mabest->runThread(
      warg->cumulator, warg->start_count_thread, warg->sample_count_thread, 
      warg->randgen_factory, warg->seed, warg->fixpoint_map, warg->output_traj, 
      warg->simulations_per_model, warg->models_cumulators, warg->models_fixpoints);
  } catch(const BNException& e) {
    std::cerr << e;
  }
  return NULL;
}

void EnsembleEngine::runThread(Cumulator* cumulator, unsigned int start_count_thread, unsigned int sample_count_thread, 
  RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map, std::ostream* output_traj, 
  std::vector<unsigned int> simulation_ind, std::vector<Cumulator*> t_models_cumulators, std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > t_models_fixpoints)
{
  unsigned int stable_cnt = 0;
  NetworkState network_state; 
  
  int model_ind = 0;
  RandomGenerator* random_generator = randgen_factory->generateRandomGenerator(seed);
  for (unsigned int nn = 0; nn < sample_count_thread; ++nn) {

    random_generator->setSeed(seed+start_count_thread+nn);
    unsigned int network_index = simulation_ind[nn];
    
    if (nn > 0 && network_index != simulation_ind[nn-1]) {
      model_ind++;
    }

    Network* network = networks[network_index];
    const std::vector<Node*>& nodes = network->getNodes();
    std::vector<Node*>::const_iterator begin = nodes.begin();
    std::vector<Node*>::const_iterator end = nodes.end();
  
    cumulator->rewind();
    if (save_individual_result){
      t_models_cumulators[model_ind]->rewind();
    }
    
    network->initStates(network_state, random_generator);
    double tm = 0.;
    unsigned int step = 0;
  //   if (NULL != output_traj) {
  //     (*output_traj) << "\nTrajectory #" << (nn+1) << '\n';
  //     (*output_traj) << " istate\t";
  //     network_state.displayOneLine(*output_traj, network);
  //     (*output_traj) << '\n';
  //   }
    while (tm < max_time) {
      double total_rate = 0.;
      MAP<NodeIndex, double> nodeTransitionRates;
      begin = nodes.begin();

      while (begin != end) {
        Node* node = *begin;
        NodeIndex node_idx = node->getIndex();
        if (node->getNodeState(network_state)) {
          double r_down = node->getRateDown(network_state);
          if (r_down != 0.0) {
            total_rate += r_down;
            nodeTransitionRates[node_idx] = r_down;
          }
        } else {
          double r_up = node->getRateUp(network_state);
          if (r_up != 0.0) {
            total_rate += r_up;
            nodeTransitionRates[node_idx] = r_up;
          }
        }
	      ++begin;
      }

      // EV: 2018-12-19 suppressed this block and integrated fixed point management below
      /*
      if (total_rate == 0.0) {
	std::cerr << "FP\n";
	// may have several fixpoint maps
	if (fixpoint_map->find(network_state.getState()) == fixpoint_map->end()) {
	  (*fixpoint_map)[network_state.getState()] = 1;
	} else {
	  (*fixpoint_map)[network_state.getState()]++;
	}
	cumulator->cumul(network_state, max_time, 0.);
	tm = max_time;
	stable_cnt++;
	break;
      }
      */

      double TH;
      if (total_rate == 0) {
        tm = max_time;
        TH = 0.;
        if (fixpoint_map->find(network_state.getState()) == fixpoint_map->end()) {
          (*fixpoint_map)[network_state.getState()] = 1;
        } else {
          (*fixpoint_map)[network_state.getState()]++;
        }

        if (save_individual_result) {
          STATE_MAP<NetworkState_Impl, unsigned int>* t_fixpoint_map = t_models_fixpoints[model_ind];
          if (t_fixpoint_map->find(network_state.getState()) == t_fixpoint_map->end()) {
            (*t_fixpoint_map)[network_state.getState()] = 1;
          } else {
            (*t_fixpoint_map)[network_state.getState()]++;
          }
        }

        stable_cnt++;
            } else {
        double transition_time ;
        if (discrete_time) {
          transition_time = time_tick;
        } else {
          double U_rand1 = random_generator->generate();
          transition_time = -log(U_rand1) / total_rate;
        }
        
        tm += transition_time;
        TH = computeTH(nodeTransitionRates, total_rate);
      }

  //     if (NULL != output_traj) {
	// (*output_traj) << std::setprecision(10) << tm << '\t';
	// network_state.displayOneLine(*output_traj, network);
	// (*output_traj) << '\t' << TH << '\n';
  //     }

      cumulator->cumul(network_state, tm, TH);
      if (save_individual_result){
        t_models_cumulators[model_ind]->cumul(network_state, tm, TH);
      }

      if (tm >= max_time) {
	      break;
      }

      NodeIndex node_idx = getTargetNode(network, random_generator, nodeTransitionRates, total_rate);
      network_state.flipState(network->getNode(node_idx));
      step++;
    }

    cumulator->trajectoryEpilogue();
    if (save_individual_result){
      t_models_cumulators[model_ind]->trajectoryEpilogue();
    }
  }
  delete random_generator;
}

void EnsembleEngine::run(std::ostream* output_traj)
{
  pthread_t* tid = new pthread_t[thread_count];
  RandomGeneratorFactory* randgen_factory = runconfig->getRandomGeneratorFactory();
  int seed = runconfig->getSeedPseudoRandom();
  unsigned int start_sample_count = 0;
  Probe probe;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
    fixpoint_map_v.push_back(fixpoint_map);
    EnsembleArgWrapper* warg = new EnsembleArgWrapper(this, start_sample_count, cumulator_v[nn]->getSampleCount(), cumulator_v[nn], simulation_indices_v[nn], cumulator_models_v[nn], fixpoints_models_v[nn], randgen_factory, seed, fixpoint_map, output_traj);
    pthread_create(&tid[nn], NULL, EnsembleEngine::threadWrapper, warg);
    arg_wrapper_v.push_back(warg);

    start_sample_count += cumulator_v[nn]->getSampleCount();
  }
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    pthread_join(tid[nn], NULL);
  }
  probe.stop();
  elapsed_core_runtime = probe.elapsed_msecs();
  user_core_runtime = probe.user_msecs();
  probe.start();
  epilogue();
  probe.stop();
  elapsed_epilogue_runtime = probe.elapsed_msecs();
  user_epilogue_runtime = probe.user_msecs();
  delete [] tid;
}  

STATE_MAP<NetworkState_Impl, unsigned int>* MetaEngine::mergeFixpointMaps()
{
  if (1 == fixpoint_map_v.size()) {
    return new STATE_MAP<NetworkState_Impl, unsigned int>(*fixpoint_map_v[0]);
  }

  STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator begin = fixpoint_map_v.begin();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator end = fixpoint_map_v.end();
  while (begin != end) {
    STATE_MAP<NetworkState_Impl, unsigned int>* fp_map = *begin;
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = fp_map->begin();
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = fp_map->end();
    while (b != e) {
      NetworkState_Impl state = (*b).first;
      if (fixpoint_map->find(state) == fixpoint_map->end()) {
	(*fixpoint_map)[state] = (*b).second;
      } else {
	(*fixpoint_map)[state] += (*b).second;
      }
      ++b;
    }
    ++begin;
  }
  return fixpoint_map;
}

void EnsembleEngine::mergeEnsembleFixpointMaps()
{
  fixpoints_per_model.resize(networks.size(), NULL);

  for (unsigned int i=0; i < networks.size(); i++) {
    std::vector<STATE_MAP<NetworkState_Impl, unsigned int>* > model_fixpoints = fixpoints_threads_v[i];
    if (model_fixpoints.size() > 0) {
      if (1 == model_fixpoints.size()) {
        fixpoints_per_model[i] = new STATE_MAP<NetworkState_Impl, unsigned int>(*model_fixpoints[0]);
        delete model_fixpoints[0];
      } else {

        STATE_MAP<NetworkState_Impl, unsigned int>* fixpoint_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
        std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator begin = model_fixpoints.begin();
        std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator end = model_fixpoints.end();
        unsigned int iii = 0;
        while (begin != end) {
          STATE_MAP<NetworkState_Impl, unsigned int>* fp_map = *begin;
          STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = fp_map->begin();
          STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = fp_map->end();
          while (b != e) {
            NetworkState_Impl state = (*b).first;
            if (fixpoint_map->find(state) == fixpoint_map->end()) {
        (*fixpoint_map)[state] = (*b).second;
            } else {
        (*fixpoint_map)[state] += (*b).second;
            }
            ++b;
          }
          ++begin;
          ++iii;
        }
        fixpoints_per_model[i] = fixpoint_map;
        for (auto t_model_fixpoint: model_fixpoints) {
          delete t_model_fixpoint;
        }
      }
    }
  }
}


void EnsembleEngine::epilogue()
{
  merged_cumulator = Cumulator::mergeCumulators(runconfig, cumulator_v);
  merged_cumulator->epilogue(networks[0], reference_state);

  if (save_individual_result) {

    cumulators_per_model.resize(networks.size(), NULL);

    for (unsigned int i=0; i < networks.size(); i++) {
      std::vector<Cumulator*> model_cumulator = cumulators_thread_v[i];
      if (model_cumulator.size() > 0) {
        
        if (model_cumulator.size() == 1) {
          cumulators_per_model[i] = model_cumulator[0];
          cumulators_per_model[i]->epilogue(networks[i], reference_state);
        } else {
          Cumulator* t_cumulator = Cumulator::mergeCumulators(runconfig, model_cumulator);
          t_cumulator->epilogue(networks[i], reference_state);
          cumulators_per_model[i] = t_cumulator;
          
          for (auto t_cumulator: model_cumulator) {
            delete t_cumulator;
          }
        }
      }
    }
  }

  STATE_MAP<NetworkState_Impl, unsigned int>* merged_fixpoint_map = mergeFixpointMaps();

  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = merged_fixpoint_map->begin();
  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = merged_fixpoint_map->end();

  while (b != e) {
    fixpoints[NetworkState((*b).first).getState()] = (*b).second;
    ++b;
  }
  delete merged_fixpoint_map;

  if (save_individual_result) {
    mergeEnsembleFixpointMaps();
  }

}

void EnsembleEngine::displayIndividual(unsigned int model_id, std::ostream& output_probtraj, std::ostream& output_statdist, std::ostream& output_fp, bool hexfloat) const {

  if (cumulators_per_model[model_id] != NULL){
    cumulators_per_model[model_id]->displayCSV(networks[0], refnode_count, output_probtraj, output_statdist, hexfloat);
  }

  if (fixpoints_per_model[model_id] != NULL){
    output_fp << "Fixed Points (" << fixpoints_per_model[model_id]->size() << ")\n";
    if (0 < fixpoints_per_model[model_id]->size()) {

#ifdef HAS_STD_HEXFLOAT
      if (hexfloat) {
        output_fp << std::hexfloat;
      }
#endif

      STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator begin = fixpoints_per_model[model_id]->begin();
      STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator end = fixpoints_per_model[model_id]->end();
      
      output_fp << "FP\tProba\tState\t";
      networks[model_id]->displayHeader(output_fp);
      for (unsigned int nn = 0; begin != end; ++nn) {
        const NetworkState& network_state = (*begin).first;
        output_fp << "#" << (nn+1) << "\t";
        if (hexfloat) {
          output_fp << fmthexdouble((double)(*begin).second / sample_count) <<  "\t";
        } else {
          output_fp << ((double)(*begin).second / sample_count) <<  "\t";
        }
        network_state.displayOneLine(output_fp, networks[model_id]);
        output_fp << '\t';
        network_state.display(output_fp, networks[model_id]);
        ++begin;
      }
    }
  }
}

EnsembleEngine::~EnsembleEngine()
{
  for (auto t_cumulator: cumulator_v)
    delete t_cumulator;

  for (auto t_fixpoint_map: fixpoint_map_v)
    delete t_fixpoint_map;
  
  for (auto t_arg_wrapper: arg_wrapper_v)
    delete t_arg_wrapper;
  
  delete merged_cumulator;

  for (auto t_cumulator: cumulators_per_model) 
    delete t_cumulator;

  for (auto t_fixpoint: fixpoints_per_model)
    delete t_fixpoint;
}

