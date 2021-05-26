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
     RunConfig.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#include <ctime>
#include "RunConfig.h"
#include "BooleanNetwork.h"
#include "MaBEstEngine.h"
#include "FinalStateSimulationEngine.h"

extern void RClex_destroy();

RunConfig::RunConfig()
{
  time_tick = 0.5;
  max_time = 1000.;
  sample_count = 10000;
  discrete_time = false;
  //  use_physrandgen = true;
  use_physrandgen = false;
  use_glibcrandgen = false;
  use_mtrandgen = false;
  seed_pseudorand = 0;
  randgen_factory = NULL;
  display_traj = false;
  thread_count = 1;
  statdist_traj_count = 0;
  statdist_cluster_threshold = 1.0;
  statdist_similarity_cache_max_size = 20000;
}

RunConfig::~RunConfig()
{
  delete randgen_factory;
}

void RunConfig::setParameter(const std::string& param, double value)
{
  const char* str = param.c_str();
  if (!strcasecmp(str, "time_tick")) {
    time_tick = value;
  } else if (!strcasecmp(str, "max_time")) {
    max_time = value;
  } else if (!strcasecmp(str, "sample_count")) {
    sample_count = (int)value;
  } else if (!strcasecmp(str, "discrete_time")) {
    discrete_time = (bool)value;
  } else if (!strcasecmp(str, "use_physrandgen")) {
    use_physrandgen = (bool)value;
  } else if (!strcasecmp(str, "use_glibcrandgen")) {
    use_glibcrandgen = (bool)value;
  } else if (!strcasecmp(str, "use_mtrandgen")) {
    use_mtrandgen = (bool)value;
  } else if (!strcasecmp(str, "seed_pseudorandom")) {
    seed_pseudorand = (int)value;
  } else if (!strcasecmp(str, "display_traj")) {
    display_traj = (bool)value;
  } else if (!strcasecmp(str, "statdist_traj_count")) {
    statdist_traj_count = (unsigned int)value;
  } else if (!strcasecmp(str, "statdist_cluster_threshold")) {
    statdist_cluster_threshold = value;
  } else if (!strcasecmp(str, "thread_count")) {
    thread_count = (unsigned int)value;
  } else if (!strcasecmp(str, "statdist_similarity_cache_max_size")) {
    statdist_similarity_cache_max_size = (unsigned int)value;
  } else {
    throw BNException("configuration: invalid parameter " + param + ", valid parameters are: " +
		      "thread_count, time_tick, max_time, sample_count, discrete_time, statdist_traj_count, statdist_cluster_threshold, display_traj, statdist_similarity_cache_max_size, use_physrandgen or seed_pseudorandom");
  }
}

RandomGeneratorFactory* RunConfig::getRandomGeneratorFactory() const
{
  if (NULL == randgen_factory) {
    if (use_physrandgen) {
      randgen_factory = new RandomGeneratorFactory(RandomGeneratorFactory::PHYSICAL);
    } else if (use_mtrandgen) {
      randgen_factory = new RandomGeneratorFactory(RandomGeneratorFactory::MERSENNE_TWISTER);
    } else if (use_glibcrandgen) {
      randgen_factory = new RandomGeneratorFactory(RandomGeneratorFactory::GLIBC);
    } else {
      randgen_factory = new RandomGeneratorFactory(RandomGeneratorFactory::DEFAULT);
    }
  }
  return randgen_factory;
}

void RunConfig::display(Network* network, time_t start_time, time_t end_time, MaBEstEngine& mabest, std::ostream& os) const
{
  const char sepfmt[] = "-----------------------------------------------%s-----------------------------------------------\n";
  char bufstr[1024];

  os << '\n';
  sprintf(bufstr, sepfmt, "--- Run ---");
  os << bufstr;

  os << "MaBoSS version: " << MaBEstEngine::VERSION << " [networks up to " << MAXNODES << " nodes]\n";
  os << "\nRun start time: " << ctime(&start_time);
  os << "Run end time: " << ctime(&end_time);

  os << "\nCore user runtime: " << (mabest.getUserCoreRunTime()/1000.) << " secs using " << thread_count << " thread" << (thread_count > 1 ? "s" : "") << "\n";
  os << "Core elapsed runtime: " << (mabest.getElapsedCoreRunTime()/1000.) << " secs using " << thread_count << " thread" << (thread_count > 1 ? "s" : "") << "\n\n";

  os << "Epilogue user runtime: " << (mabest.getUserEpilogueRunTime()/1000.) << " secs using 1 thread\n";
  os << "Epilogue elapsed runtime: " << (mabest.getElapsedEpilogueRunTime()/1000.) << " secs using 1 thread\n\n";

  os << "StatDist user runtime: " << (mabest.getUserStatDistRunTime()/1000.) << " secs using 1 thread\n";
  os << "StatDist elapsed runtime: " << (mabest.getElapsedStatDistRunTime()/1000.) << " secs using 1 thread\n\n";
  os << "Time Tick: " << getTimeTick() << '\n';
  os << "Max Time: " <<getMaxTime() << '\n';
  os << "Sample Count: " << getSampleCount() << '\n';
  os << "StatDist Trajectory Count: " << getStatDistTrajCount() << '\n';
  os << "StatDist Similarity Cache Maximum Size: " << getStatDistSimilarityCacheMaxSize() << '\n';
  os << "Discrete Time: " << (isDiscreteTime() ? "TRUE" : "FALSE") << '\n';
  os << "Random Generator: " << getRandomGeneratorFactory()->getName() << '\n';
  if (getRandomGeneratorFactory()->isPseudoRandom()) {
    os << "Seed Pseudo Random: " << getSeedPseudoRandom() << '\n';
  }
  os << "Generated Number Count: " << RandomGenerator::getGeneratedNumberCount() << "\n\n";

  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';

  sprintf(bufstr, sepfmt, "- Network -");
  os << bufstr;
  network->display(os);
  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';

  sprintf(bufstr, sepfmt, " Variables ");
  os << bufstr;
  network->getSymbolTable()->display(os);
  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';
}

void RunConfig::display(Network* network, time_t start_time, time_t end_time, FinalStateSimulationEngine& mabest, std::ostream& os) const
{
  const char sepfmt[] = "-----------------------------------------------%s-----------------------------------------------\n";
  char bufstr[1024];

  os << '\n';
  sprintf(bufstr, sepfmt, "--- Run ---");
  os << bufstr;

  os << "MaBoSS version: " << FinalStateSimulationEngine::VERSION << " [networks up to " << MAXNODES << " nodes]\n";
  os << "\nRun start time: " << ctime(&start_time);
  os << "Run end time: " << ctime(&end_time);

  os << "Time Tick: " << getTimeTick() << '\n';
  os << "Max Time: " <<getMaxTime() << '\n';
  os << "Sample Count: " << getSampleCount() << '\n';
  os << "StatDist Trajectory Count: " << getStatDistTrajCount() << '\n';
  os << "StatDist Similarity Cache Maximum Size: " << getStatDistSimilarityCacheMaxSize() << '\n';
  os << "Discrete Time: " << (isDiscreteTime() ? "TRUE" : "FALSE") << '\n';
  os << "Random Generator: " << getRandomGeneratorFactory()->getName() << '\n';
  if (getRandomGeneratorFactory()->isPseudoRandom()) {
    os << "Seed Pseudo Random: " << getSeedPseudoRandom() << '\n';
  }
  os << "Generated Number Count: " << RandomGenerator::getGeneratedNumberCount() << "\n\n";

  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';

  sprintf(bufstr, sepfmt, "- Network -");
  os << bufstr;
  network->display(os);
  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';

  sprintf(bufstr, sepfmt, " Variables ");
  os << bufstr;
  network->getSymbolTable()->display(os);
  sprintf(bufstr, sepfmt, "-----------");
  os << bufstr << '\n';
}

int RunConfig::parse(Network* network, const char* file)
{
  runconfig_setNetwork(network);
  runconfig_setConfig(this);
  if (NULL != file) {
    RCin = fopen(file, "r");
    if (RCin == NULL) {
      throw BNException("variable parsing: cannot open file:" + std::string(file) + " for reading");
    }
  }
  RC_set_file(file);

  try
  {
    int res = RCparse();
    runconfig_setNetwork(NULL);
    runconfig_setConfig(NULL);

    if (NULL != file)
      fclose(RCin);
    RClex_destroy();

    return res;
  }
  catch(const BNException& e)
  {
    runconfig_setNetwork(NULL);
    runconfig_setConfig(NULL);

    if (NULL != file)
      fclose(RCin);
    RClex_destroy();
    
    throw;
  } 
}

int RunConfig::parseExpression(Network* network, const char* expr)
{
  runconfig_setNetwork(network);
  runconfig_setConfig(this);
  RC_scan_expression(expr);

  try
  {
    int res = RCparse();
    runconfig_setNetwork(NULL);
    runconfig_setConfig(NULL);
    
    RClex_destroy();
    
    return res;
  }
  catch(const BNException& e)
  {
    runconfig_setNetwork(NULL);
    runconfig_setConfig(NULL);
    RClex_destroy();
    
    throw;
  }
}

void RunConfig::generateTemplate(Network* network, std::ostream& os) const
{
  dump_perform(network, os, true);
}

void RunConfig::dump(Network* network, std::ostream& os) const
{
  dump_perform(network, os, false);
}

void RunConfig::dump_perform(Network* network, std::ostream& os, bool is_template) const
{
  time_t now = time(NULL);

  os << "//\n";
  os << "// MaBoSS " << MaBEstEngine::VERSION << " configuration " << (is_template ? "template " : "") << "generated at " << ctime(&now);
  os << "//\n\n";

  if (is_template) {
    os << "// global configuration variables\n";
  }
  os << "time_tick = " << time_tick << ";\n";
  os << "max_time = " << max_time << ";\n";
  os << "sample_count = " << sample_count << ";\n";
  os << "discrete_time = " << discrete_time << ";\n";
  os << "use_physrandgen = " << use_physrandgen << ";\n";
  os << "seed_pseudorandom = " << seed_pseudorand << ";\n";
  os << "display_traj = " << display_traj << ";\n";
  os << "statdist_traj_count = " << statdist_traj_count << ";\n";
  os << "statdist_cluster_threshold = " << statdist_cluster_threshold << ";\n";
  os << "thread_count = " << thread_count << ";\n";
  os << "statdist_similarity_cache_max_size = " << statdist_similarity_cache_max_size << ";\n";

  os << '\n';
  if (network->getSymbolTable()->getSymbolCount() != 0) {
    if (is_template) {
      os << "// variables to be set in the configuration file or by using the --config-vars option\n";
    }
    network->getSymbolTable()->display(os, false);
    os << '\n';
  }

  if (is_template) {
    os << "// set is_internal attribute value to 1 if node is an internal node\n";
  }
  const std::vector<Node*>& nodes = network->getNodes();
  std::vector<Node*>::const_iterator begin = nodes.begin();
  std::vector<Node*>::const_iterator end = nodes.end();
  while (begin != end) {
    Node* node = *begin;
    os << node->getLabel() << ".is_internal = " << node->isInternal() << ";\n";
    ++begin;
  }

  os << '\n';
  if (is_template) {
    os << "// if node is a reference node, set refstate attribute value to 0 or 1 according to its reference state\n";
    os << "// if node is not a reference node, skip its refstate declaration or set value to -1\n";
  }
  begin = nodes.begin();
  while (begin != end) {
    Node* node = *begin;
    if (node->isReference()) {
      os << node->getLabel() << ".refstate = " << node->getReferenceState() << ";\n";
    } else {
      os << node->getLabel() << ".refstate = -1;\n";
    }
    ++begin;
  }

  os << '\n';
  if (is_template) {
    os << "// if NODE initial state is:\n";
    os << "// - equals to 1: NODE.istate = 1;\n";
    os << "// - equals to 0: NODE.istate = 0;\n";
    os << "// - random: NODE.istate = -1; OR [NODE].istate = 0.5 [0], 0.5 [1]; OR skip NODE.istate declaration\n";
    os << "// - weighted random: [NODE].istate = P0 [0], P1 [1]; where P0 and P1 are arithmetic expressions\n";
  }
  IStateGroup::display(network, os);
}
