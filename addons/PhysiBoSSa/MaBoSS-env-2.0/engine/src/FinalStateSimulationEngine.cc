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
     FinalStateSimulationEngine.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#include "FinalStateSimulationEngine.h"
#include "Probe.h"
#include "Utils.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>

const std::string FinalStateSimulationEngine::VERSION = "1.0.0";

FinalStateSimulationEngine::FinalStateSimulationEngine(Network* network, RunConfig* runconfig) :
  network(network), runconfig(runconfig),
  time_tick(runconfig->getTimeTick()), 
  max_time(runconfig->getMaxTime()), 
  sample_count(runconfig->getSampleCount()), 
  discrete_time(runconfig->isDiscreteTime()), 
  thread_count(runconfig->getThreadCount())
  {

  if (thread_count > sample_count) {
    thread_count = sample_count;
  }

  if (thread_count > 1 && !runconfig->getRandomGeneratorFactory()->isThreadSafe()) {
    std::cerr << "Warning: non reentrant random, may not work properly in multi-threaded mode\n";
  }

  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();
  
  refnode_count = 0;
  while (begin != end) {
    Node* node = *begin;
    if (node->isInternal()) {

      has_internal = true;
      internal_state.setNodeState(node, true);
      // std::cout << "Node " << node->getLabel() << " is internal, internal state is ";
      // internal_state.displayOneLine(std::cout, network);
      // std::cout << std::endl;
    }
    if (node->isReference()) {
      reference_state.setNodeState(node, node->getReferenceState());
      refnode_count++;
    }
    ++begin;
  }

  sample_count_per_thread.resize(thread_count);
  unsigned int count = sample_count / thread_count;
  unsigned int firstcount = count + sample_count - count * thread_count;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
      sample_count_per_thread[nn] = (nn == 0 ? firstcount : count);
  }
}

NodeIndex FinalStateSimulationEngine::getTargetNode(RandomGenerator* random_generator, const MAP<NodeIndex, double>& nodeTransitionRates, double total_rate) const
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

struct FinalStateArgWrapper {
  FinalStateSimulationEngine* mabest;
  unsigned int start_count_thread;
  unsigned int sample_count_thread;
  
  RandomGeneratorFactory* randgen_factory;
  int seed;
  STATE_MAP<NetworkState_Impl, unsigned int>* final_state_map;
  std::ostream* output_traj;

  FinalStateArgWrapper(FinalStateSimulationEngine* mabest, unsigned int start_count_thread, unsigned int sample_count_thread, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* final_state_map, std::ostream* output_traj) :
    mabest(mabest), start_count_thread(start_count_thread), sample_count_thread(sample_count_thread), randgen_factory(randgen_factory), seed(seed), final_state_map(final_state_map), output_traj(output_traj) { }
};

void* FinalStateSimulationEngine::threadWrapper(void *arg)
{
  FinalStateArgWrapper* warg = (FinalStateArgWrapper*)arg;
  try {
    warg->mabest->runThread(warg->start_count_thread, warg->sample_count_thread, warg->randgen_factory, warg->seed, warg->final_state_map, warg->output_traj);
  } catch(const BNException& e) {
    std::cerr << e;
  }
  return NULL;
}

void FinalStateSimulationEngine::runThread(unsigned int start_count_thread, unsigned int sample_count_thread, RandomGeneratorFactory* randgen_factory, int seed, STATE_MAP<NetworkState_Impl, unsigned int>* final_state_map, std::ostream* output_traj)
{
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();
  NetworkState network_state; 

  RandomGenerator* random_generator = randgen_factory->generateRandomGenerator(seed);
  for (unsigned int nn = 0; nn < sample_count_thread; ++nn) {
    random_generator->setSeed(seed+start_count_thread+nn);

    network->initStates(network_state, random_generator);
    double tm = 0.;
    unsigned int step = 0;
    if (NULL != output_traj) {
      (*output_traj) << "\nTrajectory #" << (nn+1) << '\n';
      (*output_traj) << " istate\t";
      network_state.displayOneLine(*output_traj, network);
      (*output_traj) << '\n';
    }
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

      if (NULL != output_traj) {
        (*output_traj) << std::setprecision(10) << tm << '\t';
        network_state.displayOneLine(*output_traj, network);
      }

      if (tm >= max_time) {
	      break;
      }

      NodeIndex node_idx = getTargetNode(random_generator, nodeTransitionRates, total_rate);
      network_state.flipState(network->getNode(node_idx));
      step++;
    }

    NetworkState_Impl final_state = network_state.getState();
    if (has_internal) {

      final_state &= ~internal_state.getState();
    }

    if (final_state_map->find(final_state) == final_state_map->end()) {
      (*final_state_map)[final_state] = 1;
    } else {
      (*final_state_map)[final_state]++;
    }  
  }
  delete random_generator;
}

void FinalStateSimulationEngine::run(std::ostream* output_traj)
{
  pthread_t* tid = new pthread_t[thread_count];
  RandomGeneratorFactory* randgen_factory = runconfig->getRandomGeneratorFactory();
  int seed = runconfig->getSeedPseudoRandom();
  unsigned int start_sample_count = 0;
  Probe probe;
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    STATE_MAP<NetworkState_Impl, unsigned int>* final_states_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
    final_states_map_v.push_back(final_states_map);
    FinalStateArgWrapper* warg = new FinalStateArgWrapper(this, start_sample_count, sample_count_per_thread[nn], randgen_factory, seed, final_states_map, output_traj);
    pthread_create(&tid[nn], NULL, FinalStateSimulationEngine::threadWrapper, warg);
    arg_wrapper_v.push_back(warg);

    start_sample_count += sample_count_per_thread[nn];
  }
  for (unsigned int nn = 0; nn < thread_count; ++nn) {
    pthread_join(tid[nn], NULL);
  }
  epilogue();
  delete [] tid;
}  

STATE_MAP<NetworkState_Impl, unsigned int>* FinalStateSimulationEngine::mergeFinalStateMaps()
{
  if (1 == final_states_map_v.size()) {
    return new STATE_MAP<NetworkState_Impl, unsigned int>(*final_states_map_v[0]);
  }

  STATE_MAP<NetworkState_Impl, unsigned int>* final_states_map = new STATE_MAP<NetworkState_Impl, unsigned int>();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator begin = final_states_map_v.begin();
  std::vector<STATE_MAP<NetworkState_Impl, unsigned int>*>::iterator end = final_states_map_v.end();
  while (begin != end) {
    
    STATE_MAP<NetworkState_Impl, unsigned int>* fp_map = *begin;
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = fp_map->begin();
    STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = fp_map->end();
    while (b != e) {
      NetworkState_Impl state = (*b).first;
      if (final_states_map->find(state) == final_states_map->end()) {
	(*final_states_map)[state] = (*b).second;
      } else {
	(*final_states_map)[state] += (*b).second;
      }
      ++b;
    }
    ++begin;
  }
  return final_states_map;
}

void FinalStateSimulationEngine::epilogue()
{
  STATE_MAP<NetworkState_Impl, unsigned int>* merged_final_states_map = mergeFinalStateMaps();

  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator b = merged_final_states_map->begin();
  STATE_MAP<NetworkState_Impl, unsigned int>::const_iterator e = merged_final_states_map->end();

  while (b != e) {
    final_states[NetworkState((*b).first).getState()] = ((double) (*b).second)/sample_count;
    ++b;
  }
  delete merged_final_states_map;
}

FinalStateSimulationEngine::~FinalStateSimulationEngine()
{
  for (auto t_arg_wrapper: arg_wrapper_v)
    delete t_arg_wrapper;
}

void FinalStateSimulationEngine::displayFinal(std::ostream& output_final, bool hexfloat) const
{
  for (auto final_state: final_states) {
    if (hexfloat)
      output_final << std::setprecision(6) << fmthexdouble(final_state.second);
    
    else
      output_final << std::setprecision(6) << final_state.second << "\t";
    
    NetworkState(final_state.first).displayOneLine(output_final, network);
    output_final << "\n";
  }
}

const STATE_MAP<Node*, double> FinalStateSimulationEngine::getFinalNodes() const {

  STATE_MAP<Node *, double> node_dist;
  for (auto& node: network->getNodes())
  {
    if (!(node->isInternal()))
    {
      double dist = 0;
      for (auto final_state: final_states) {
        NetworkState state = final_state.first;
        dist += final_state.second * ((double) state.getNodeState(node));
      }

      node_dist[node] = dist;
    }
  }

  return node_dist;
}

#ifdef PYTHON_API

PyObject* FinalStateSimulationEngine::getNumpyLastStatesDists() const 
{
  
  npy_intp dims[2] = {(npy_intp) 1, (npy_intp) final_states.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 
  PyObject* list_states = PyList_New(final_states.size());

  int i=0;
  for(auto& final_state: final_states) {

    void* ptr = PyArray_GETPTR2(result, 0, i);
    PyArray_SETITEM(
      result, 
      (char*) ptr,
      PyFloat_FromDouble(final_state.second)
    );

    PyList_SetItem(
      list_states, i,
      PyUnicode_FromString(NetworkState(final_state.first).getName(network).c_str())
    );

    i++;
  }

  PyObject* timepoints = PyList_New(1);
  PyList_SetItem(
    timepoints, 0, 
    PyFloat_FromDouble(max_time)
  );

  return PyTuple_Pack(3, PyArray_Return(result), list_states, timepoints);
}

std::vector<Node*> FinalStateSimulationEngine::getNodes() const {
  std::vector<Node*> result_nodes;

  for (auto node: network->getNodes()) {
    if (!node->isInternal())
      result_nodes.push_back(node);
  }
  return result_nodes;
}

PyObject* FinalStateSimulationEngine::getNumpyLastNodesDists() const 
{
  std::vector<Node*> output_nodes = getNodes();

  npy_intp dims[2] = {(npy_intp) 1, (npy_intp) output_nodes.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 
  PyObject* list_nodes = PyList_New(output_nodes.size());
  
  int i=0;
  for (auto node: output_nodes) {

    for(auto& final_state: final_states) {
    
      if (NetworkState(final_state.first).getNodeState(node)){
        void* ptr_val = PyArray_GETPTR2(result, 0, i);

        PyArray_SETITEM(
          result, 
          (char*) ptr_val,
          PyFloat_FromDouble(
            PyFloat_AsDouble(PyArray_GETITEM(result, (char*) ptr_val))
            + final_state.second
          )
        );
      }
    }

    PyList_SetItem(list_nodes, i, PyUnicode_FromString(node->getLabel().c_str()));
    i++;
  }

  PyObject* timepoints = PyList_New(1);
  PyList_SetItem(
    timepoints, 0, 
    PyFloat_FromDouble(max_time)
  );

  return PyTuple_Pack(3, PyArray_Return(result), list_nodes, timepoints);
}


#endif
