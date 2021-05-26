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
     DataStreamer.cc

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "DataStreamer.h"
#include "Utils.h"

const std::string DataStreamer::PROTOCOL_VERSION_NUMBER = "1.0";

const std::string DataStreamer::MABOSS_MAGIC = "MaBoSS-2.0";
const std::string DataStreamer::PROTOCOL_VERSION = "Protocol-Version:";
const std::string DataStreamer::FLAGS = "Flags:";
const unsigned long long DataStreamer::HEXFLOAT_FLAG = 0x1ULL;
const unsigned long long DataStreamer::OVERRIDE_FLAG = 0x2ULL;
const unsigned long long DataStreamer::AUGMENT_FLAG = 0x4ULL;
const std::string DataStreamer::COMMAND = "Command:";
const std::string DataStreamer::RUN_COMMAND = "run";
const std::string DataStreamer::CHECK_COMMAND = "check";
const std::string DataStreamer::PARSE_COMMAND = "parse";
const std::string DataStreamer::NETWORK = "Network:";
const std::string DataStreamer::CONFIGURATION = "Configuration:";
const std::string DataStreamer::CONFIGURATION_EXPRESSIONS = "Configuration-Expressions:";
const std::string DataStreamer::CONFIGURATION_VARIABLES = "Configuration-Variables:";
const std::string DataStreamer::RETURN = "RETURN";
const std::string DataStreamer::STATUS = "Status:";
const std::string DataStreamer::ERROR_MESSAGE = "Error-Message:";
const std::string DataStreamer::STATIONARY_DISTRIBUTION = "Stationary-Distribution:";
const std::string DataStreamer::TRAJECTORY_PROBABILITY = "Trajectory-Probability:";
const std::string DataStreamer::TRAJECTORIES = "Trajectories:";
const std::string DataStreamer::FIXED_POINTS = "Fixed-Points:";
const std::string DataStreamer::RUN_LOG = "Run-Log:";

static size_t add_header(std::ostringstream& o_header, const std::string& directive, size_t o_offset, size_t offset)
{
  if (o_offset != offset) {
    o_header << directive << o_offset << "-" << (offset-1) << '\n';
  }
  return offset;
}

void DataStreamer::buildStreamData(std::string& data, const ClientData& client_data)
{
  std::ostringstream o_header;
  std::ostringstream o_data;
  size_t offset = 0;
  size_t o_offset = offset;

  o_header << MABOSS_MAGIC << "\n";
  o_header << PROTOCOL_VERSION << PROTOCOL_VERSION_NUMBER << "\n";
  o_header << FLAGS << client_data.getFlags() << "\n";
  o_header << COMMAND << client_data.getCommand() << "\n";

  const std::vector<std::string>& config_v = client_data.getConfigs();
  for (std::vector<std::string>::const_iterator iter_config = config_v.begin(); iter_config != config_v.end(); ++iter_config) {
    const std::string& config = *iter_config;
    o_data << config;
    offset += config.length();
  }

  o_offset = add_header(o_header, CONFIGURATION, o_offset, offset);

  const std::vector<std::string>& config_expr_v = client_data.getConfigExprs();
  for (std::vector<std::string>::const_iterator iter_config_expr = config_expr_v.begin(); iter_config_expr != config_expr_v.end(); ++iter_config_expr) {
    const std::string& config_expr = *iter_config_expr;
    o_data << config_expr << ';';
    offset += config_expr.length() + 1;
  }

  o_offset = add_header(o_header, CONFIGURATION_EXPRESSIONS, o_offset, offset);

  const std::string& config_vars = client_data.getConfigVars();
  o_data << config_vars;
  offset += config_vars.length();
  o_offset = add_header(o_header, CONFIGURATION_VARIABLES, o_offset, offset);

  const std::string& network = client_data.getNetwork();
  o_data << network;
  offset += network.length();

  o_offset = add_header(o_header, NETWORK, o_offset, offset);

  data = o_header.str() + "\n" + o_data.str();
}

void DataStreamer::buildStreamData(std::string &data, const ServerData& server_data)
{
  std::ostringstream o_header;
  std::ostringstream o_data;
  size_t offset = 0;
  size_t o_offset = offset;

  o_header << RETURN << " " << MABOSS_MAGIC << "\n";
  o_header << STATUS << server_data.getStatus() << "\n";
  if (server_data.getStatus() != 0) {
    o_header << ERROR_MESSAGE << server_data.getErrorMessageRaw() << "\n";
    data = o_header.str() + "\n\n";
    return;
  }

  const std::string& traj = server_data.getTraj();
  if (traj.length() > 0) {
    o_data << traj;
    offset += traj.length();
    o_offset = add_header(o_header, TRAJECTORIES, o_offset, offset);
  }

  const std::string& stat_dist = server_data.getStatDist();
  if (stat_dist.length() > 0) {
    o_data << stat_dist;
    offset += stat_dist.length();
    o_offset = add_header(o_header, STATIONARY_DISTRIBUTION, o_offset, offset);
  }

  const std::string& prob_traj = server_data.getProbTraj();
  if (prob_traj.length() > 0) {
    o_data << prob_traj;
    offset += prob_traj.length();
    o_offset = add_header(o_header, TRAJECTORY_PROBABILITY, o_offset, offset);
  }

  const std::string& fp = server_data.getFP();
  if (fp.length() > 0) {
    o_data << fp;
    offset += fp.length();
    o_offset = add_header(o_header, FIXED_POINTS, o_offset, offset);
  }

  const std::string& run_log = server_data.getRunLog();
  if (run_log.length() > 0) {
    o_data << run_log;
    offset += run_log.length();
    o_offset = add_header(o_header, RUN_LOG, o_offset, offset);
  }

  data = o_header.str() + "\n" + o_data.str();
}

std::string DataStreamer::error(int status, const std::string& errmsg)
{
  std::ostringstream o_str;
  o_str << RETURN << " " << MABOSS_MAGIC << "\n" << STATUS << status << "\n" << ERROR_MESSAGE << errmsg << "\n\n";
  return o_str.str();
}

int DataStreamer::parse_header_items(const std::string &header, std::vector<HeaderItem>& header_item_v, std::string& err_data)
{
  size_t opos = 0;
  size_t pos = 0;

  for (;;) {
    pos = header.find(':', opos);
    if (pos == std::string::npos) {
      break;
    }
    std::string directive = header.substr(opos, pos+1-opos);
    opos = pos+1;
    pos = header.find('\n', opos);
    if (pos == std::string::npos) {
      err_data = "newline not found in header after directive " + directive;
      return 1;
    }
    std::string value = header.substr(opos, pos-opos);
    opos = pos+1;
    size_t pos2 = value.find("-");
    if (directive == STATUS || directive == ERROR_MESSAGE || directive == PROTOCOL_VERSION || directive == FLAGS || directive == COMMAND) {
      header_item_v.push_back(HeaderItem(directive, value));
    } else if (pos2 != std::string::npos) {
      header_item_v.push_back(HeaderItem(directive, atoll(value.substr(0, pos2).c_str()), atoll(value.substr(pos2+1).c_str())));
    } else {
      err_data = "dash - not found in value " + value + " after directive " + directive;
      return 1;
    }
  }

  return 0;
}

int DataStreamer::parseStreamData(ClientData& client_data, const std::string& input_data, std::string& err_data)
{
  //std::string magic = RUN + " " + MABOSS_MAGIC;
  std::string magic = MABOSS_MAGIC;
  size_t pos = input_data.find(magic);
  if (pos == std::string::npos) {
    int status = 1;
    err_data = error(status, "magic " + magic + " not found in header");
    return status;
  }

  size_t offset = magic.length();
  pos = input_data.find("\n\n", offset);
  if (pos == std::string::npos) {
    int status = 2;
    err_data = error(status, "separator double nl found in header");
    return status;
  }

  offset++;
  std::string header = input_data.substr(offset, pos-offset+1);
  std::string data  = input_data.substr(pos+2);

  std::vector<HeaderItem> header_item_v;
  if (parse_header_items(header, header_item_v, err_data)) {
    return 3;
  }

  for (std::vector<HeaderItem>::const_iterator header_item_iter = header_item_v.begin(); header_item_iter != header_item_v.end(); ++header_item_iter) {
    const std::string& directive = header_item_iter->getDirective();
    std::string data_value = data.substr(header_item_iter->getFrom(), header_item_iter->getTo() - header_item_iter->getFrom() + 1);
    if (directive == PROTOCOL_VERSION) {
      client_data.setProtocolVersion(header_item_iter->getValue());
    } else if (directive == FLAGS) {
      client_data.setFlags(atoll(header_item_iter->getValue().c_str()));
    } else if (directive == COMMAND) {
      client_data.setCommand(header_item_iter->getValue());
    } else if (directive == NETWORK) {
      client_data.setNetwork(data_value);
    } else if (directive == CONFIGURATION) {
      client_data.addConfig(data_value);
    } else if (directive == CONFIGURATION_EXPRESSIONS) {
      client_data.addConfigExpr(data_value);
    } else if (directive == CONFIGURATION_VARIABLES) {
      client_data.setConfigVars(data_value);
    } else {
      err_data = "unknown directive " + directive;
      return 4;
    }
    /*
    std::cout << "directive [" << header_item_iter->getDirective() << "]\n";
    std::cout << "from [" << header_item_iter->getFrom() << "]\n";
    std::cout << "to [" << header_item_iter->getTo() << "]\n";
    */
  }

  //std::cout << "header [" << header << "]\n";
  //std::cout << "data [" << data << "]\n";
  return 0;
}

int DataStreamer::parseStreamData(ServerData& server_data, const std::string& input_data)
{
  std::string magic = RETURN + " " + MABOSS_MAGIC;
  size_t pos = input_data.find(magic);
  if (pos == std::string::npos) {
    server_data.setStatus(1);
    server_data.setErrorMessage("magic " + magic + " not found in header");
    return 1;
  }

  size_t offset = magic.length();
  pos = input_data.find("\n\n", offset);
  if (pos == std::string::npos) {
    server_data.setStatus(2);
    server_data.setErrorMessage("separator double nl found in header");
    return 1;
  }

  offset++;
  std::string header = input_data.substr(offset, pos-offset+1);
  std::string data  = input_data.substr(pos+2);

  std::vector<HeaderItem> header_item_v;
  std::string err_data;
  if (parse_header_items(header, header_item_v, err_data)) {
    server_data.setStatus(3);
    server_data.setErrorMessage(err_data);
    return 1;
  }

  for (std::vector<HeaderItem>::const_iterator header_item_iter = header_item_v.begin(); header_item_iter != header_item_v.end(); ++header_item_iter) {
    const std::string& directive = header_item_iter->getDirective();
    if (directive == STATUS) {
      server_data.setStatus(atoi(header_item_iter->getValue().c_str()));
    } else if (directive == ERROR_MESSAGE) {
      server_data.setErrorMessage(header_item_iter->getValue());
    } else {
      std::string data_value = data.substr(header_item_iter->getFrom(), header_item_iter->getTo() - header_item_iter->getFrom() + 1);

      if (directive == STATIONARY_DISTRIBUTION) {
	server_data.setStatDist(data_value);
      } else if (directive == TRAJECTORY_PROBABILITY) {
	server_data.setProbTraj(data_value);
      } else if (directive == TRAJECTORIES) {
	server_data.setTraj(data_value);
      } else if (directive == FIXED_POINTS) {
	server_data.setFP(data_value);
      } else if (directive == RUN_LOG) {
	server_data.setRunLog(data_value);
      } else {
	server_data.setErrorMessage("unknown directive " + directive);
	server_data.setStatus(4);
	return 1;
      }
    }
  }

  return 0;
}

