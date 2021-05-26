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
    ProbaDist.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _PROBADIST_H_
#define _PROBADIST_H_

#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "BooleanNetwork.h"

#define CLUSTER_OPTIM

class ProbaDist {
  STATE_MAP<NetworkState_Impl, double> mp;

 public:
  size_t size() const {
    return mp.size();
  }

  void incr(NetworkState_Impl state, double tm_slice) {
    STATE_MAP<NetworkState_Impl, double>::iterator proba_iter = mp.find(state);
    if (proba_iter == mp.end()) {
      mp[state] = tm_slice;
    } else {
      (*proba_iter).second += tm_slice;
    }
  }

  void clear() {
    mp.clear();
  }

  void set(NetworkState_Impl state, double tm_slice) {
    mp[state] = tm_slice;
  }

  bool hasState(NetworkState_Impl state, double& tm_slice) const {
    STATE_MAP<NetworkState_Impl, double>::const_iterator iter = mp.find(state);
    if (iter != mp.end()) {
      tm_slice = (*iter).second;
      return true;
    }
    return false;
  }

  class Iterator {
    
    const ProbaDist& proba_dist_map;
    STATE_MAP<NetworkState_Impl, double>::const_iterator iter, end;

  public:
  Iterator(const ProbaDist& proba_dist_map) : proba_dist_map(proba_dist_map) {
      iter = proba_dist_map.mp.begin();
      end = proba_dist_map.mp.end();
    }
	
    void rewind() {
      iter = proba_dist_map.mp.begin();
    }

    bool hasNext() const {
      return iter != end;
    }

    void next(NetworkState_Impl& state, double& tm_slice) {
      state = (*iter).first;
      tm_slice = (*iter).second;
      ++iter;
    }

    void next(NetworkState_Impl& state) {
      state = (*iter).first;
      ++iter;
    }

    void next(double& tm_slice) {
      tm_slice = (*iter).second;
      ++iter;
    }
  };	

  void display(std::ostream& os, Network* network, bool hexfloat) const;

  Iterator iterator() {return Iterator(*this);}
  Iterator iterator() const {return Iterator(*this);}
};

class ProbaDistClusterFactory;

class ProbaDistCluster {

  MAP<unsigned int, ProbaDist> proba_dist_map;
  ProbaDistClusterFactory* factory;
  struct Proba {
    double proba;
    double probaSquare;
    Proba() : proba(0.), probaSquare(0.) { }
    Proba(double proba, double probaSquare) : proba(proba), probaSquare(probaSquare) { }
  };
  // state -> Proba
  STATE_MAP<NetworkState_Impl, Proba> stat_dist_map;

 public:
  ProbaDistCluster(ProbaDistClusterFactory* factory) : factory(factory) { }

  void add(unsigned int index, const ProbaDist& proba_dist);
  static double similarity(unsigned int nn1, const ProbaDist& proba_dist1, unsigned int nn2, const ProbaDist& proba_dist2, double** similarity_cache);

  size_t size() const {return proba_dist_map.size();}

  void complete(double threshold, unsigned int statdist_traj_count);
  void computeStationaryDistribution();

  void display(Network* network, std::ostream& os, bool display) const;
  void displayStationaryDistribution(Network* network, std::ostream& os, bool hexfloat) const;
};

class ProbaDistClusterFactory {

  std::vector<ProbaDistCluster*> proba_dist_cluster_v;
  MAP<unsigned int, bool> proba_dist_clusterized;
#ifdef CLUSTER_OPTIM
  MAP<unsigned int, bool> proba_dist_not_clusterized;
#endif
  const std::vector<ProbaDist> proba_dist_v;
  unsigned int statdist_traj_count;
  double** similarity_cache;

 public:
  ProbaDistClusterFactory(const std::vector<ProbaDist>& proba_dist_v, unsigned int statdist_traj_count) : proba_dist_v(proba_dist_v), statdist_traj_count(statdist_traj_count), similarity_cache(NULL) {
#ifdef CLUSTER_OPTIM
    for (unsigned int nn = 0; nn < statdist_traj_count; ++nn) {
      proba_dist_not_clusterized[nn] = true;
    }
    //std::cout << "SIZE: " << proba_dist_not_clusterized.size() << '\n';
#endif
  }

  ProbaDistCluster* newCluster() {
    ProbaDistCluster* proba_dist_cluster = new ProbaDistCluster(this);
    proba_dist_cluster_v.push_back(proba_dist_cluster);
    return proba_dist_cluster;
  }

  void setClusterized(unsigned int index) {
    proba_dist_clusterized[index] = true;
#ifdef CLUSTER_OPTIM
    MAP<unsigned int, bool>::iterator iter = proba_dist_not_clusterized.find(index);
    if (iter != proba_dist_not_clusterized.end()) {
      proba_dist_not_clusterized.erase(iter);
    }
#endif
  }

  double** getSimilarityCache() {
    return similarity_cache;
  }

#ifdef CLUSTER_OPTIM
  const MAP<unsigned int, bool>& getNotClusterizedMap() const {
    return proba_dist_not_clusterized;
  }
#endif

  bool isClusterized(unsigned int index) const {
    return proba_dist_clusterized.find(index) != proba_dist_clusterized.end();
  }

  const ProbaDist& getProbaDist(unsigned int index) const {
    //assert(index < proba_dist_v.size());
    return proba_dist_v[index];
  }

  void cacheSimilarities();

  void makeClusters(RunConfig* runconfig);
  void computeStationaryDistribution();
  void displayStationaryDistribution(Network* network, std::ostream& os, bool hexfloat) const;

  void display(Network* network, std::ostream& os, bool hexfloat) const;

  ~ProbaDistClusterFactory() {
    
    for (unsigned int nn1 = 0; nn1 < statdist_traj_count; ++nn1) {
      delete [] similarity_cache[nn1];
    }
    delete [] similarity_cache;
    for (auto& proba_dist_cluster: proba_dist_cluster_v) {
      delete proba_dist_cluster;
    }
  }
};

#endif
