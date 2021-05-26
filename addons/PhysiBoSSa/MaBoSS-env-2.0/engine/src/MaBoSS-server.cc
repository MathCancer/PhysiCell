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
     MaBoSS-server.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     May 2018
*/

#include <iostream>
#include "Client.h"
#include "Server.h"
#include "Utils.h"

const char* prog = "MaBoSS-server";

static int usage(std::ostream& os = std::cerr)
{
  os << "\nUsage:\n\n";
  os << "  " << prog << " [-h|--help]\n\n";
  os << "  " << prog << " [--version]\n\n";
  os << "  " << prog << " [--verbose]\n\n";
  os << "  " << prog << " --port PORT [--host HOST]\n";
  os << "  " << prog << " [-q] [--pidfile PIDFILE]\n";
  return 1;
}

static int help()
{
  //  std::cout << "\n=================================================== " << prog << " help " << "===================================================\n";
  (void)usage(std::cout);
  std::cout << "\nOptions:\n\n";
  std::cout << "  --version                               : displays MaBoSS-client version\n";
  std::cout << "  --host HOST                             : uses given host\n";
  std::cout << "  --port PORT                             : uses given PORT (number or filename)\n";
  std::cout << "  --pidfile PIDFILE                       : optional; pidfile to store pid process\n";
  std::cout << "  -q                                      : quiet mode, no output at all\n";
  std::cout << "  --verbose                               : verbose mode\n";
  std::cout << "  -h --help                               : displays this message\n";
  return 0;
}

int main(int argc, char* argv[])
{
  std::string port;
  std::string host;
  std::string pidfile;
  bool quiet = false;
  bool verbose = false;

  for (int nn = 1; nn < argc; ++nn) {
    const char* opt = argv[nn];
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
    } else if (!strcmp(opt, "--pidfile")) {
      if (checkArgMissing(prog, opt, nn, argc)) {
	return usage();
      }
      pidfile = argv[++nn];
    } else if (!strcmp(opt, "-q")) {
      quiet = true;
    } else if (!strcmp(opt, "--verbose")) {
      verbose = true;
    } else if (!strcmp(opt, "--help") || !strcmp(opt, "-h")) {
      return help();
    } else {
      std::cerr << '\n' << prog << ": unknown option " << opt << std::endl;
      return usage();
    }
  }

  if (port.length() == 0) {
    std::cerr << '\n' << prog << ": port is missing\n";
    return usage();
  }

  Server* server = Server::getServer(host, port, prog, pidfile, quiet, verbose);
  if (quiet) {
    close(0);
    close(1);
    close(2);
  }
  server->manageRequests();
  delete server;

  return 0;
}
