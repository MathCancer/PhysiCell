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
     RunConfig.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _RUNCONFIG_H_
#define _RUNCONFIG_H_

#include "RandomGenerator.h"
#include "BooleanNetwork.h"
class MaBEstEngine;
class FinalStateSimulationEngine;

class RunConfig {

  mutable RandomGeneratorFactory* randgen_factory;
  double time_tick;
  double max_time;
  unsigned int sample_count;
  bool discrete_time;
  bool use_physrandgen;
  bool use_glibcrandgen;
  bool use_mtrandgen;
  int seed_pseudorand;
  bool display_traj;
  unsigned int thread_count;
  unsigned int statdist_traj_count;
  double statdist_cluster_threshold;
  unsigned int statdist_similarity_cache_max_size;
  void dump_perform(Network* network, std::ostream& os, bool is_template) const;

 public:
  RunConfig();
  ~RunConfig();
  
  int parse(Network* network, const char* file = NULL);
  int parseExpression(Network* network, const char* expr);
  void setParameter(const std::string& param, double value);

  RandomGeneratorFactory* getRandomGeneratorFactory() const;
  double getTimeTick() const {return time_tick;}
  double getMaxTime() const {return max_time;}
  unsigned int getSampleCount() const {return sample_count;}
  bool isDiscreteTime() const {return discrete_time;}
  int getSeedPseudoRandom() const {return seed_pseudorand;}
  void setSeedPseudoRandom(int seed) { seed_pseudorand = seed;}
  void display(Network* network, time_t start_time, time_t end_time, MaBEstEngine& mabest, std::ostream& os) const;
  void display(Network* network, time_t start_time, time_t end_time, FinalStateSimulationEngine& engine, std::ostream& os) const;
  bool displayTrajectories() const {return display_traj;}
  unsigned int getThreadCount() const {return thread_count;}
  unsigned int getStatDistTrajCount() const {return statdist_traj_count <= sample_count ? statdist_traj_count : sample_count;}
  double getStatdistClusterThreshold() const {return statdist_cluster_threshold;}
  unsigned int getStatDistSimilarityCacheMaxSize() const {return statdist_similarity_cache_max_size;}

  void generateTemplate(Network* network, std::ostream& os) const;
  void dump(Network* network, std::ostream& os) const;
};

extern FILE* RCin;
extern int RCparse();
extern void RC_scan_expression(const char *);

extern void runconfig_setNetwork(Network* network);
extern void runconfig_setConfig(RunConfig* config);
extern void RC_set_file(const char* file);
extern void RC_set_expr(const char* expr);

#endif
