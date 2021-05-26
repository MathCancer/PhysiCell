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
     DataStreamer.h
     
     Authors:
     Eric Viara <viara@sysra.com>
     
     Date:
     May 2018
*/

#ifndef _DATASTREAMER_H_
#define _DATASTREAMER_H_

#include <string>
#include <vector>
#include "Client.h"
#include "Server.h"

class DataStreamer {
public:
  static const std::string MABOSS_MAGIC;
  static const std::string PROTOCOL_VERSION;
  static const std::string PROTOCOL_VERSION_NUMBER;
  static const std::string FLAGS;
  static const unsigned long long HEXFLOAT_FLAG;
  static const unsigned long long OVERRIDE_FLAG;
  static const unsigned long long AUGMENT_FLAG;
  static const std::string COMMAND;
  static const std::string RUN_COMMAND;
  static const std::string CHECK_COMMAND;
  static const std::string PARSE_COMMAND;
  static const std::string RETURN;
  static const std::string STATUS;
  static const std::string ERROR_MESSAGE;
  static const std::string NETWORK;
  static const std::string CONFIGURATION;
  static const std::string CONFIGURATION_EXPRESSIONS;
  static const std::string CONFIGURATION_VARIABLES;
  static const std::string STATIONARY_DISTRIBUTION;
  static const std::string TRAJECTORY_PROBABILITY;
  static const std::string TRAJECTORIES;
  static const std::string FIXED_POINTS;
  static const std::string RUN_LOG;
  static std::string error(int status, const std::string& errmsg);

private:
  class HeaderItem {
    std::string directive;
    size_t from;
    size_t to;
    std::string value;

  public:
    HeaderItem(const std::string& directive, size_t from, size_t to) : directive(directive), from(from), to(to) { }
    HeaderItem(const std::string& directive, const std::string& value) : directive(directive), from(0), to(0), value(value) { }
    const std::string& getDirective() const {return directive;}
    size_t getFrom() const {return from;}
    size_t getTo() const {return to;}
    const std::string& getValue() const {return value;}
  };

  static int parse_header_items(const std::string &header, std::vector<HeaderItem>& header_item_v, std::string& err_data);

public:
  static void buildStreamData(std::string& data, const ClientData& client_data);
  static void buildStreamData(std::string &data, const ServerData& server_data);
  static int parseStreamData(ClientData& client_data, const std::string& data, std::string& err_data);
  static int parseStreamData(ServerData& server_data, const std::string& data);
};

#endif
