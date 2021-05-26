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
     Cumulator.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#include "BooleanNetwork.h"
#include "Cumulator.h"
#include "RunConfig.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <math.h>
#include <float.h>

double distance(const STATE_MAP<NetworkState_Impl, double>& proba_dist1, const STATE_MAP<NetworkState_Impl, double>& proba_dist2)
{
  return 0.;
}

void Cumulator::check() const
{
  // check that for each tick (except the last one), the sigma of each map == 1.
  for (int nn = 0; nn < max_tick_index; ++nn) {
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    double sum = 0.;
    while (iter.hasNext()) {
      TickValue tick_value;
      iter.next(tick_value);
      sum += tick_value.tm_slice;
    }
    sum /= time_tick*sample_count;
    assert(sum >= 1. - 0.0001 && sum <= 1. + 0.0001);
  }
}

void Cumulator::trajectoryEpilogue()
{
  assert(sample_num < sample_count);

  ProbaDist::Iterator curtraj_proba_dist_iter = curtraj_proba_dist.iterator();

  double proba_max_time = 0.;

  while (curtraj_proba_dist_iter.hasNext()) {
    double tm_slice;
    curtraj_proba_dist_iter.next(tm_slice);
    proba_max_time += tm_slice;
  }

  //std::cout << "Trajepilogue #" << (sample_num+1) << " " << proba_max_time << '\n';
  double proba = 0;
  curtraj_proba_dist_iter.rewind();

  ProbaDist& proba_dist = proba_dist_v[sample_num++];
  while (curtraj_proba_dist_iter.hasNext()) {
    NetworkState_Impl state;
    double tm_slice;
    curtraj_proba_dist_iter.next(state, tm_slice);
    //assert(proba_dist.find(state) == proba_dist.end());
    double new_tm_slice = tm_slice / proba_max_time;
    proba_dist.set(state, new_tm_slice);
    proba += new_tm_slice;
  }

  assert(proba >= 0.9999 && proba <= 1.0001);
}

void Cumulator::computeMaxTickIndex()
{
  /*
  unsigned int tmp_tick_index = tick_index + !tick_completed;
  if (max_tick_index > tmp_tick_index) {
    max_tick_index = tmp_tick_index;
  }
  */
  if (max_tick_index > tick_index) {
    max_tick_index = tick_index;
  }
}

void Cumulator::epilogue(Network* network, const NetworkState& reference_state)
{
  computeMaxTickIndex();

  //check();

  // compute H (Entropy), TH (Transition entropy) and HD (Hamming distance)
  H_v.resize(max_tick_index);
  TH_v.resize(max_tick_index);
#ifndef HD_BUG
  HD_v.resize(max_tick_index);
#endif

  maxcols = 0;
  double ratio = time_tick * sample_count;
  for (int nn = 0; nn < max_tick_index; ++nn) { // time tick
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    H_v[nn] = 0.;
    TH_v[nn] = 0.;
#ifndef HD_BUG
    MAP<unsigned int, double>& hd_m = HD_v[nn];
#endif
    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      double tm_slice = tick_value.tm_slice;
      double proba = tm_slice / ratio;      
      double TH = tick_value.TH / sample_count;
      H_v[nn] += -log2(proba) * proba;
#ifndef HD_BUG
      int hd = reference_state.hamming(network, state);
      if (hd_m.find(hd) == hd_m.end()) {
	hd_m[hd] = proba;
      } else {
	hd_m[hd] += proba;
      }
#endif
      TH_v[nn] += TH;
    }
    TH_v[nn] /= time_tick;
    if (mp.size() > maxcols) {
      maxcols = mp.size();
    }
  }

#ifdef HD_BUG
  HD_v.resize(max_tick_index);

  for (int nn = 0; nn < max_tick_index; ++nn) { // time tick
    const HDCumulMap& hd_mp = get_hd_map(nn);
    HDCumulMap::Iterator iter = hd_mp.iterator();
    MAP<unsigned int, double>& hd_m = HD_v[nn];
    while (iter.hasNext()) {
      NetworkState_Impl state;
      double tm_slice;
      iter.next(state, tm_slice);
      double proba = tm_slice / ratio;      
      int hd = reference_state.hamming(network, state);
      if (hd_m.find(hd) == hd_m.end()) {
	hd_m[hd] = proba;
      } else {
	hd_m[hd] += proba;
      }
    }
  }
#endif
}
void Cumulator::displayProbTrajCSV(Network* network, unsigned int refnode_count, std::ostream& os_probtraj, bool hexfloat) const
{
 std::vector<Node*>::const_iterator begin_network;

  os_probtraj << "Time\tTH" << (COMPUTE_ERRORS ? "\tErrorTH" : "") << "\tH";

  for (unsigned int jj = 0; jj <= refnode_count; ++jj) {
    os_probtraj << "\tHD=" << jj;
  }

  for (unsigned int nn = 0; nn < maxcols; ++nn) {
    os_probtraj << "\tState\tProba" << (COMPUTE_ERRORS ? "\tErrorProba" : "");
  }

  os_probtraj << '\n';

  //os_probtraj << std::fixed;
  //os_probtraj << std::setprecision(6);
  double time_tick2 = time_tick * time_tick;
  double ratio = time_tick*sample_count;
  for (int nn = 0; nn < max_tick_index; ++nn) {
    os_probtraj << std::setprecision(4) << std::fixed << (nn*time_tick);
#ifdef HAS_STD_HEXFLOAT
    if (hexfloat) {
      os_probtraj << std::hexfloat;
    }
#endif
    // TH
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();
    if (hexfloat) {
      os_probtraj << '\t' << fmthexdouble(TH_v[nn]);
    } else {
      os_probtraj << '\t' << TH_v[nn];
    }

    // ErrorTH
    //assert((size_t)nn < TH_square_v.size());
    if (COMPUTE_ERRORS) {
      double TH_square = TH_square_v[nn];
      double TH = TH_v[nn]; // == TH
      double variance_TH = (TH_square / ((sample_count-1) * time_tick2)) - (TH*TH*sample_count/(sample_count-1));
      double err_TH;
      double variance_TH_sample_count = variance_TH/sample_count;
      //assert(variance_TH > 0.0);
      if (variance_TH_sample_count >= 0.0) {
	err_TH = sqrt(variance_TH_sample_count);
      } else {
	err_TH = 0.;
      }
      if (hexfloat) {
	os_probtraj << '\t' << fmthexdouble(err_TH);
      } else {
	os_probtraj << '\t' << err_TH;
      }
    }

    // H
    if (hexfloat) {
      os_probtraj << '\t' << fmthexdouble(H_v[nn]);
    } else {
      os_probtraj << '\t' << H_v[nn];
    }

    std::string zero_hexfloat = fmthexdouble(0.0);
    // HD
    const MAP<unsigned int, double>& hd_m = HD_v[nn];
    for (unsigned int hd = 0; hd <= refnode_count; ++hd) { 
      os_probtraj << '\t';
      MAP<unsigned int, double>::const_iterator hd_m_iter = hd_m.find(hd);
      if (hd_m_iter != hd_m.end()) {
	if (hexfloat) {
	  os_probtraj << fmthexdouble((*hd_m_iter).second);
	} else {
	  os_probtraj << (*hd_m_iter).second;
	}
      } else {
	if (hexfloat) {
	  os_probtraj << zero_hexfloat;
	} else {
	  os_probtraj << "0.0";
	}
      }
    }

    // Proba, ErrorProba
    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      double proba = tick_value.tm_slice / ratio;      
      //double TH = tick_value.TH / sample_count;
      os_probtraj << '\t';
      NetworkState network_state(state);
      network_state.displayOneLine(os_probtraj, network);
      if (hexfloat) {
	os_probtraj << '\t' << std::setprecision(6) << fmthexdouble(proba);
      } else {
	os_probtraj << '\t' << std::setprecision(6) << proba;
      }
      if (COMPUTE_ERRORS) {
	double tm_slice_square = tick_value.tm_slice_square;
	double variance_proba = (tm_slice_square / ((sample_count-1) * time_tick2)) - (proba*proba*sample_count/(sample_count-1));
	double err_proba;
	double variance_proba_sample_count = variance_proba/sample_count;
	if (variance_proba_sample_count >= DBL_MIN) {
	  err_proba = sqrt(variance_proba_sample_count);
	} else {
	  err_proba = 0.;
	}
	if (hexfloat) {
	  os_probtraj << '\t' << fmthexdouble(err_proba);
	} else {
	  os_probtraj << '\t' << err_proba;
	}
      }
    }
    os_probtraj << '\n';
  }
}
void Cumulator::displayStatDistCSV(Network* network, unsigned int refnode_count, std::ostream& os_statdist, bool hexfloat) const
{
  // should not be in cumulator, but somehwere in ProbaDist*

  // Probability distribution
  unsigned int statdist_traj_count = runconfig->getStatDistTrajCount();
  if (statdist_traj_count == 0) {
    return;
  }

#ifdef HAS_STD_HEXFLOAT
  if (hexfloat) {
    os_statdist << std::hexfloat;
  }
#endif

  unsigned int max_size = 0;
  unsigned int cnt = 0;
  unsigned int proba_dist_size = proba_dist_v.size();
  for (unsigned int nn = 0; nn < proba_dist_size; ++nn) {
    const ProbaDist& proba_dist = proba_dist_v[nn];
    if (proba_dist.size() > max_size) {
      max_size = proba_dist.size();
    }
    cnt++;
    if (cnt > statdist_traj_count) {
      break;
    }
  }

  //unsigned int proba_dist_traj_count = cnt;

  os_statdist << "Trajectory";
  for (unsigned int nn = 0; nn < max_size; ++nn) {
    os_statdist << "\tState\tProba";
  }

  os_statdist << '\n';
  cnt = 0;
  for (unsigned int nn = 0; nn < proba_dist_size; ++nn) {
    const ProbaDist& proba_dist = proba_dist_v[nn];
    os_statdist << "#" << (cnt+1);
    cnt++;

    proba_dist.display(os_statdist, network, hexfloat);
    if (cnt >= statdist_traj_count) {
      break;
    }
  }

#if 0
  // compute similarity between all proba_dist (< statdist_traj_count), just for testing
  for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) {
    const ProbaDist& proba_dist1 = proba_dist_v[nn1];
    for (unsigned int nn2 = nn1+1; nn2 < statdist_traj_count; ++nn2) {
      const ProbaDist& proba_dist2 = proba_dist_v[nn2];
      similarity(network, proba_dist1, proba_dist2);
    }
  }
#endif

  // should not be called from here, but from MaBestEngine
  ProbaDistClusterFactory* clusterFactory = new ProbaDistClusterFactory(proba_dist_v, statdist_traj_count);
  clusterFactory->makeClusters(runconfig);
  clusterFactory->display(network, os_statdist, hexfloat);
  clusterFactory->computeStationaryDistribution();
  clusterFactory->displayStationaryDistribution(network, os_statdist, hexfloat);
  delete clusterFactory;
}

void Cumulator::displayCSV(Network* network, unsigned int refnode_count, std::ostream& os_probtraj, std::ostream& os_statdist, bool hexfloat) const
{
  displayProbTrajCSV(network, refnode_count, os_probtraj, hexfloat);
  displayStatDistCSV(network, refnode_count, os_statdist, hexfloat);
}

void Cumulator::displayAsymptoticCSV(Network *network, unsigned int refnode_count, std::ostream &os_asymptprob, bool hexfloat, bool proba) const
{

  double ratio;
  if (proba)
  {
    ratio = time_tick * sample_count;
  }
  else
  {
    ratio = time_tick;
  }

  // Choosing the last tick
  int nn = max_tick_index - 1;

#ifdef HAS_STD_HEXFLOAT
  if (hexfloat)
  {
    os_asymptprob << std::hexfloat;
  }
#endif
  // TH
  const CumulMap &mp = get_map(nn);
  CumulMap::Iterator iter = mp.iterator();


  while (iter.hasNext())
  {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);

    double proba = tick_value.tm_slice / ratio;
    if (proba)
    {
      if (hexfloat)
      {
        os_asymptprob << std::setprecision(6) << fmthexdouble(proba);
      }
      else
      {
        os_asymptprob << std::setprecision(6) << proba;
      }
    }
    else
    {
      int t_proba = static_cast<int>(round(proba));
      os_asymptprob << std::fixed << t_proba;
    }

    os_asymptprob << '\t';
    NetworkState network_state(state);
    network_state.displayOneLine(os_asymptprob, network);

    os_asymptprob << '\n';

  }
}

const std::map<double, STATE_MAP<NetworkState_Impl, double> > Cumulator::getStateDists() const
{
  std::map<double, STATE_MAP<NetworkState_Impl, double> > result;

  double ratio = time_tick*sample_count;
  for (int nn = 0; nn < max_tick_index; ++nn) {

    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();

    STATE_MAP<NetworkState_Impl, double> t_result;

    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);

      double proba = tick_value.tm_slice / ratio;      
      t_result[state] = proba;
    }

    result[((double) nn)*time_tick] = t_result;
  } 
  return result;
}


#ifdef PYTHON_API

std::set<NetworkState_Impl> Cumulator::getStates() const
{
  std::set<NetworkState_Impl> result_states;

  for (int nn=0; nn < getMaxTickIndex(); nn++) {
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();

    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      result_states.insert(state);
    }
  }

  return result_states;
}

std::vector<NetworkState_Impl> Cumulator::getLastStates() const
{
  std::vector<NetworkState_Impl> result_states;

    const CumulMap& mp = get_map(getMaxTickIndex()-1);
    CumulMap::Iterator iter = mp.iterator();

    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      result_states.push_back(state);
    }

  return result_states;
}


PyObject* Cumulator::getNumpyStatesDists(Network* network) const 
{
  std::set<NetworkState_Impl> result_states = getStates();
  
  npy_intp dims[2] = {(npy_intp) getMaxTickIndex(), (npy_intp) result_states.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 

  std::vector<NetworkState_Impl> list_states(result_states.begin(), result_states.end());
  std::map<NetworkState_Impl, unsigned int> pos_states;
  for(unsigned int i=0; i < list_states.size(); i++) {
    pos_states[list_states[i]] = i;
  }

  double ratio = time_tick*sample_count;

  for (int nn=0; nn < getMaxTickIndex(); nn++) {
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();

    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      
      void* ptr = PyArray_GETPTR2(result, nn, pos_states[state]);
      PyArray_SETITEM(
        result, 
        (char*) ptr,
        PyFloat_FromDouble(tick_value.tm_slice / ratio)
      );
    }
  }
  PyObject* pylist_state = PyList_New(list_states.size());
  for (unsigned int i=0; i < list_states.size(); i++) {
    PyList_SetItem(
      pylist_state, i, 
      PyUnicode_FromString(NetworkState(list_states[i]).getName(network).c_str())
    );
  }

  PyObject* timepoints = PyList_New(getMaxTickIndex());
  for (int i=0; i < getMaxTickIndex(); i++) {
    PyList_SetItem(timepoints, i, PyFloat_FromDouble(((double) i) * time_tick));
  }

  return PyTuple_Pack(3, PyArray_Return(result), pylist_state, timepoints);
}


PyObject* Cumulator::getNumpyLastStatesDists(Network* network) const 
{
  std::vector<NetworkState_Impl> result_last_states = getLastStates();
  
  npy_intp dims[2] = {(npy_intp) 1, (npy_intp) result_last_states.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 

  std::map<NetworkState_Impl, unsigned int> pos_states;
  for(unsigned int i=0; i < result_last_states.size(); i++) {
    pos_states[result_last_states[i]] = i;
  }

  double ratio = time_tick*sample_count;

  const CumulMap& mp = get_map(getMaxTickIndex()-1);
  CumulMap::Iterator iter = mp.iterator();

  while (iter.hasNext()) {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);
    
    void* ptr = PyArray_GETPTR2(result, 0, pos_states[state]);
    PyArray_SETITEM(
      result, 
      (char*) ptr,
      PyFloat_FromDouble(tick_value.tm_slice / ratio)
    );
  }

  PyObject* pylist_state = PyList_New(result_last_states.size());
  for (unsigned int i=0; i < result_last_states.size(); i++) {
    PyList_SetItem(
      pylist_state, i, 
      PyUnicode_FromString(NetworkState(result_last_states[i]).getName(network).c_str())
    );
  }

  PyObject* timepoints = PyList_New(1);
  PyList_SetItem(
    timepoints, 0, 
    PyFloat_FromDouble(
      (
        (double) (getMaxTickIndex()-1)
      )*time_tick
    )
  );

  return PyTuple_Pack(3, PyArray_Return(result), pylist_state, timepoints);
}




std::vector<Node*> Cumulator::getNodes(Network* network) const {
  std::vector<Node*> result_nodes;

  for (auto node: network->getNodes()) {
    if (!node->isInternal())
      result_nodes.push_back(node);
  }
  return result_nodes;
}

PyObject* Cumulator::getNumpyNodesDists(Network* network) const 
{
  std::vector<Node*> output_nodes = getNodes(network);
  
  npy_intp dims[2] = {(npy_intp) getMaxTickIndex(), (npy_intp) output_nodes.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 

  std::map<Node*, unsigned int> pos_nodes;
  for(unsigned int i=0; i < output_nodes.size(); i++) {
    pos_nodes[output_nodes[i]] = i;
  }

  double ratio = time_tick*sample_count;

  for (int nn=0; nn < getMaxTickIndex(); nn++) {
    const CumulMap& mp = get_map(nn);
    CumulMap::Iterator iter = mp.iterator();

    while (iter.hasNext()) {
      NetworkState_Impl state;
      TickValue tick_value;
      iter.next(state, tick_value);
      
      for (auto node: output_nodes) {
        
        if (((NetworkState) state).getNodeState(node)){
          void* ptr_val = PyArray_GETPTR2(result, nn, pos_nodes[node]);

          PyArray_SETITEM(
            result, 
            (char*) ptr_val,
            PyFloat_FromDouble(
              PyFloat_AsDouble(PyArray_GETITEM(result, (char*) ptr_val))
              + (tick_value.tm_slice / ratio)
            )
          );
        }
      }
    }
  }
  PyObject* pylist_nodes = PyList_New(output_nodes.size());
  for (unsigned int i=0; i < output_nodes.size(); i++) {
    PyList_SetItem(
      pylist_nodes, i, 
      PyUnicode_FromString(output_nodes[i]->getLabel().c_str())
    );
  }

  PyObject* timepoints = PyList_New(getMaxTickIndex());
  for (int i=0; i < getMaxTickIndex(); i++) {
    PyList_SetItem(timepoints, i, PyFloat_FromDouble(((double) i) * time_tick));
  }

  return PyTuple_Pack(3, PyArray_Return(result), pylist_nodes, timepoints);
}


PyObject* Cumulator::getNumpyLastNodesDists(Network* network) const 
{
  std::vector<Node*> output_nodes = getNodes(network);
  
  npy_intp dims[2] = {(npy_intp) 1, (npy_intp) output_nodes.size()};
  PyArrayObject* result = (PyArrayObject *) PyArray_ZEROS(2,dims,NPY_DOUBLE, 0); 

  std::map<Node*, unsigned int> pos_nodes;
  for(unsigned int i=0; i < output_nodes.size(); i++) {
    pos_nodes[output_nodes[i]] = i;
  }

  double ratio = time_tick*sample_count;

  const CumulMap& mp = get_map(getMaxTickIndex()-1);
  CumulMap::Iterator iter = mp.iterator();

  while (iter.hasNext()) {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);
    
    for (auto node: output_nodes) {
      
      if (((NetworkState) state).getNodeState(node)){
        void* ptr_val = PyArray_GETPTR2(result, 0, pos_nodes[node]);

        PyArray_SETITEM(
          result, 
          (char*) ptr_val,
          PyFloat_FromDouble(
            PyFloat_AsDouble(PyArray_GETITEM(result, (char*) ptr_val))
            + (tick_value.tm_slice / ratio)
          )
        );
      }
    }
  }
  PyObject* pylist_nodes = PyList_New(output_nodes.size());
  for (unsigned int i=0; i < output_nodes.size(); i++) {
    PyList_SetItem(
      pylist_nodes, i, 
      PyUnicode_FromString(output_nodes[i]->getLabel().c_str())
    );
  }
  PyObject* timepoints = PyList_New(1);
  PyList_SetItem(
    timepoints, 0, 
    PyFloat_FromDouble(
      (
        (double) (getMaxTickIndex()-1)
      )*time_tick
    )
  );
  
  return PyTuple_Pack(3, PyArray_Return(result), pylist_nodes, timepoints);
}


#endif

const STATE_MAP<NetworkState_Impl, double> Cumulator::getNthStateDist(int nn) const
{
  double ratio = time_tick*sample_count;

  const CumulMap& mp = get_map(nn);
  CumulMap::Iterator iter = mp.iterator();

  STATE_MAP<NetworkState_Impl, double> result;

  while (iter.hasNext()) {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);

    double proba = tick_value.tm_slice / ratio;      
    result[state] = proba;
  }
 
  return result;
}
 
const STATE_MAP<NetworkState_Impl, double> Cumulator::getAsymptoticStateDist() const 
{ return getNthStateDist(getMaxTickIndex()-1); }

const double Cumulator::getFinalTime() const {
  return time_tick*(getMaxTickIndex()-1);
}
void Cumulator::add(unsigned int where, const CumulMap& add_cumul_map)
{
  CumulMap& to_cumul_map = get_map(where);

  CumulMap::Iterator iter = add_cumul_map.iterator();
  while (iter.hasNext()) {
    NetworkState_Impl state;
    TickValue tick_value;
    iter.next(state, tick_value);
    to_cumul_map.add(state, tick_value);
  }
}

#ifdef HD_BUG
void Cumulator::add(unsigned int where, const HDCumulMap& add_hd_cumul_map)
{
  HDCumulMap& to_hd_cumul_map = get_hd_map(where);

  HDCumulMap::Iterator iter = add_hd_cumul_map.iterator();
  while (iter.hasNext()) {
    NetworkState_Impl state;
    double tm_slice;
    iter.next(state, tm_slice);
    to_hd_cumul_map.add(state, tm_slice);
  }
}
#endif

Cumulator* Cumulator::mergeCumulators(RunConfig* runconfig, std::vector<Cumulator*>& cumulator_v)
{
  size_t size = cumulator_v.size();
  if (1 == size) {
    Cumulator* cumulator = cumulator_v[0];
    return new Cumulator(*cumulator);
  }

  unsigned int t_cumulator_size = 0;
  for (auto& cum: cumulator_v) {
    t_cumulator_size += cum->sample_count;
  }

  Cumulator* ret_cumul = new Cumulator(runconfig, runconfig->getTimeTick(), runconfig->getMaxTime(), t_cumulator_size);
  size_t min_cumul_size = ~0ULL;
  size_t min_tick_index_size = ~0ULL;
  std::vector<Cumulator*>::iterator begin = cumulator_v.begin();
  std::vector<Cumulator*>::iterator end = cumulator_v.end();
  while (begin != end) {
    Cumulator* cumulator = *begin;
    cumulator->computeMaxTickIndex();
    if (cumulator->cumul_map_v.size() < min_cumul_size) {
      min_cumul_size = cumulator->cumul_map_v.size();
    }
    if ((size_t)cumulator->max_tick_index < min_tick_index_size) {
      min_tick_index_size = cumulator->max_tick_index;
    }
    ++begin;
  }

  ret_cumul->cumul_map_v.resize(min_cumul_size);
#ifdef HD_BUG
  ret_cumul->hd_cumul_map_v.resize(min_cumul_size);
#endif
  ret_cumul->max_tick_index = ret_cumul->tick_index = min_tick_index_size;

  begin = cumulator_v.begin();
  unsigned int rr = 0;
  for (unsigned int jj = 0; begin != end; ++jj) {
    Cumulator* cumulator = *begin;
    for (unsigned int nn = 0; nn < min_cumul_size; ++nn) {
      ret_cumul->add(nn, cumulator->cumul_map_v[nn]);
#ifdef HD_BUG
      ret_cumul->add(nn, cumulator->hd_cumul_map_v[nn]);
#endif
      ret_cumul->TH_square_v[nn] += cumulator->TH_square_v[nn];
    }
    unsigned int proba_dist_size = cumulator->proba_dist_v.size();
    for (unsigned int ii = 0; ii < proba_dist_size; ++ii) {
      assert(ret_cumul->proba_dist_v.size() > rr);
      ret_cumul->proba_dist_v[rr++] = cumulator->proba_dist_v[ii];
    }
    ++begin;
  }
  return ret_cumul;
}
