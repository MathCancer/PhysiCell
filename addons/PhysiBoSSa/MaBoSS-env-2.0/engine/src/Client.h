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
     Client.h
     
     Authors:
     Eric Viara <viara@sysra.com>
     
     Date:
     May 2018
*/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <vector>

#include "RPC.h"

class ServerData;

class ClientData {
  std::string protocol_version;
  unsigned long long flags;
  std::string command;

  std::string network;
  std::vector<std::string> config_v;
  std::vector<std::string> config_expr_v;
  std::string config_vars;

public:
  void setProtocolVersion(const std::string& protocol_version) {
    this->protocol_version = protocol_version;
  }

  void setFlags(unsigned long long flags) {
    this->flags = flags;
  }

  void setCommand(const std::string& command) {
    this->command = command;
  }

  void setNetwork(const std::string& network) {
    this->network = network;
  }

  void addConfig(const std::string& config) {
    config_v.push_back(config);
  }

  void addConfigExpr(const std::string& config_expr) {
    config_expr_v.push_back(config_expr);
  }

  void setConfigVars(const std::string& config_vars) {
    this->config_vars = config_vars;
  }

  const std::string& getProtocolVersion() const {
    return protocol_version;
  }

  const unsigned long long getFlags() const {
    return flags;
  }

  const std::string& getCommand() const {
    return command;
  }

  const std::string& getNetwork() const {
    return network;
  }

  const std::vector<std::string>& getConfigs() const {
    return config_v;
  }

  const std::string& getConfigVars() const {
    return config_vars;
  }

  const std::vector<std::string>& getConfigExprs() const {
    return config_expr_v;
  }
};

class Client : public rpc_Client {
  std::string host;
  std::string port;
  bool verbose;

public:
  Client(const std::string& host, const std::string& port, bool verbose = false) : rpc_Client(host, port), verbose(verbose) { }

  void send(const ClientData& client_data, ServerData& server_data);
};

#endif
