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
     Client.cc

   Authors:
     Eric Viara <viara@sysra.com> 
   Date:
     May 2018
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <resolv.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Client.h"
#include "Server.h"
#include "DataStreamer.h"
#include "RPC.h"

void Client::send(const ClientData& client_data, ServerData& server_data)
{
  if (open() == rpc_Success) {
    std::string data;
    // 2018-11-08: warning: command and comm_mode not yet implemented
    DataStreamer::buildStreamData(data, client_data);
    if (verbose) {
      std::cout << "client sending request [" << data << "]\n";
    }

    rpc_writeStringData(sock_fd, data.c_str(), data.length());
    
    char* response = rpc_readStringData(sock_fd);
    if (response != NULL) {
      if (verbose) {
	std::cout << "client received [" << response << "]\n";
      }
      DataStreamer::parseStreamData(server_data, response);
      free(response);
    }

    close();
  }
  // else throw error
}

