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
     MaBoSS-client.cc

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#include <iostream>
#include <assert.h>
#include "Client.h"
#include "Server.h"
#include "Utils.h"
#include "DataStreamer.h"

const char* prog = "MaBoSS-client";

static int usage(std::ostream& os = std::cerr)
{
  os << "\nUsage:\n\n";
  os << "  " << prog << " [-h|--help]\n\n";
  os << "  " << prog << " [--version]\n\n";
  os << "  " << prog << " [--verbose]\n\n";
  os << "  " << prog << " --port PORT [--host HOST] [-c|--config CONF_FILE] [-v|--config-vars VAR1=NUMERIC[,VAR2=...]] [-e|--config-expr CONFIG_EXPR] -o|--output OUTPUT BOOLEAN_NETWORK_FILE\n";
  os << "  " << prog << " [--check]\n";
  os << "  " << prog << " [--override]\n";
  os << "  " << prog << " [--augment]\n";
  os << "  " << prog << " [--hexfloat\n\n";
  return 1;
}

static int help()
{
  //  std::cout << "\n=================================================== " << prog << " help " << "===================================================\n";
  (void)usage(std::cout);
  std::cout << "\nOptions:\n\n";
  std::cout << "  --version                               : displays MaBoSS-client version\n";
  std::cout << "  --host HOST                             : reaches server on given host\n";
  std::cout << "  --port PORT                             : reaches server on given PORT (number or filename)\n";
  std::cout << "  -c --config CONF_FILE                   : uses CONF_FILE as a configuration file\n";
  std::cout << "  -v --config-vars VAR=NUMERIC[,VAR2=...] : sets the value of the given variables to the given numeric values\n";
  std::cout << "  -e --config-expr CONFIG_EXPR            : evaluates the configuration expression; may have multiple expressions\n";
  std::cout << "                                            separated by semi-colons\n";
  std::cout << "  --override                              : if set, a new node definition will override a previous one\n";
  std::cout << "  --augment                               : if set, a new node definition will complete (add non existing attributes) / override (replace existing attributes) a previous one\n";
  std::cout << "  -o --output OUTPUT                      : prefix to be used for output files; when present run MaBoSS simulation process\n";
  std::cout << "  --check                                 : checks network and configuration files and exits\n";
  std::cout << "  --hexfloat                              : displays double in hexadecimal format\n";
  std::cout << "  --verbose                               : verbose mode\n";
  std::cout << "  -h --help                               : displays this message\n";
  std::cout << "\nNotices:\n";
  std::cout << "\n1. --config and --config-expr options can be used multiple times;\n";
  std::cout << "   multiple --config and/or --config-expr options are managed in the order given at the command line;\n";
  std::cout << "   --config-vars VAR=VALUE always overrides any VAR assignment in a configuration file or expression\n";
  std::cout << '\n';
  return 0;
}

int main(int argc, char* argv[])
{
  const char* output = NULL;
  std::vector<ConfigOpt> runconfig_file_or_expr_v;
  std::string config_vars;
  const char* ctbndl_file = NULL;
  std::string port;
  std::string host;
  bool verbose = false;
  bool check = false;
  bool override = false;
  bool augment = false;
  bool hexfloat = false;

  for (int nn = 1; nn < argc; ++nn) {
    const char* opt = argv[nn];
    if (opt[0] == '-') {
      if (!strcmp(opt, "-version") || !strcmp(opt, "--version")) { // keep -version for backward compatibility
	//std::cout << "MaBoSS version " + MaBEstEngine::VERSION << " [networks up to " << MAXNODES << " nodes]\n";
	std::cout << "MaBoSS version <TBD>\n";
	return 0;
      } else if (!strcmp(opt, "--host")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	host = argv[++nn];
      } else if (!strcmp(opt, "--port")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	port = argv[++nn];
      } else if (!strcmp(opt, "--config-vars")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	if (config_vars.length() > 0) {
	  config_vars += ",";
	}
	config_vars += argv[++nn];
      } else if (!strcmp(opt, "--config-expr") || !strcmp(opt, "-e")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	runconfig_file_or_expr_v.push_back(ConfigOpt(argv[++nn], true));
      } else if (!strcmp(opt, "-o") || !strcmp(opt, "--output")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	output = argv[++nn];
      } else if (!strcmp(opt, "-c") || !strcmp(opt, "--config")) {
	if (checkArgMissing(prog, opt, nn, argc)) {
	  return usage();
	}
	runconfig_file_or_expr_v.push_back(ConfigOpt(argv[++nn], false));
      } else if (!strcmp(opt, "--verbose")) {
	verbose = true;
      } else if (!strcmp(opt, "--override")) {
	if (augment) {
	  std::cerr << '\n' << prog << ": --override and --augment are exclusive options\n"; return usage();
	}
	override = true;
      } else if (!strcmp(opt, "--augment")) {
	if (override) {
	  std::cerr << '\n' << prog << ": --override and --augment are exclusive options\n"; return usage();
	}
	augment = true;
      } else if (!strcmp(opt, "--check")) {
	check = true;
      } else if (!strcmp(opt, "--hexfloat")) {
	hexfloat = true;
      } else if (!strcmp(opt, "--help") || !strcmp(opt, "-h")) {
	return help();
      } else {
	std::cerr << '\n' << prog << ": unknown option " << opt << std::endl;
	return usage();
      }
    } else if (ctbndl_file == NULL) {
      ctbndl_file = argv[nn];
    } else {
      std::cerr << '\n' << prog << ": boolean network file is already set to " << ctbndl_file << " [" << opt << "]" << std::endl;
    }
  }

  if (NULL == ctbndl_file) {
    std::cerr << '\n' << prog << ": boolean network file is missing\n";
    return usage();
  }
    

  if (port.length() == 0) {
    std::cerr << '\n' << prog << ": port is missing\n";
    return usage();
  }

  ClientData client_data;

  std::string contents;
  if (fileGetContents(ctbndl_file, contents)) {
    return 1;
  }
  client_data.setNetwork(contents);

  std::vector<ConfigOpt>::const_iterator begin = runconfig_file_or_expr_v.begin();
  std::vector<ConfigOpt>::const_iterator end = runconfig_file_or_expr_v.end();
  while (begin != end) {
    const ConfigOpt& cfg = *begin;
    if (cfg.isExpr()) {
      client_data.addConfigExpr(cfg.getExpr());
    } else {
      std::string contents;
      if (fileGetContents(cfg.getFile(), contents)) {
	return 1;
      }
      client_data.addConfig(contents);
    }
    ++begin;
  }

  client_data.setConfigVars(config_vars);

  client_data.setCommand(check ? DataStreamer::CHECK_COMMAND : DataStreamer::RUN_COMMAND);
  unsigned long long flags = 0;
  if (hexfloat) {
    flags |= DataStreamer::HEXFLOAT_FLAG;
  }
  if (override) {
    flags |= DataStreamer::OVERRIDE_FLAG;
  }
  if (augment) {
    flags |= DataStreamer::AUGMENT_FLAG;
  }
  client_data.setFlags(flags);

  Client* client = new Client(host, port, verbose);
  ServerData server_data;
  client->send(client_data, server_data);

  if (server_data.getTraj().length() > 0) {
    filePutContents(std::string(output) + "_traj.txt", server_data.getTraj());
  }
  if (server_data.getRunLog().length() > 0) {
    filePutContents(std::string(output) + "_run.txt", server_data.getRunLog());
  }
  if (server_data.getProbTraj().length() > 0) {
    filePutContents(std::string(output) + "_probtraj.csv", server_data.getProbTraj());
  }
  if (server_data.getStatDist().length() > 0) {
    filePutContents(std::string(output) + "_statdist.csv", server_data.getStatDist());
  }
  if (server_data.getFP().length() > 0) {
    filePutContents(std::string(output) + "_fp.csv", server_data.getFP());
  }

  if (server_data.getStatus() != 0) {
    std::cerr << prog << " error: [" << server_data.getErrorMessage() << "] [status=" << server_data.getStatus() << "]\n";
    return 1;
  }

  delete client;

  return 0;
}
