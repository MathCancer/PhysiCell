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
     RPC.h

   Authors:
     Eric Viara <viara@sysra.com>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     May 2018
*/

#ifndef _RPC_H_
#define _RPC_H_

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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <string>

#define rpc_Success 0
#define rpc_Error 1

#define rpc_isSocketValid(S) ((S) >= 0)

struct rpc_PortHandle {
  int domain, type;
  char *portname;
  union {
    struct {
      int sockin_fd;
      struct sockaddr_in sock_in_name;
    } in;
    struct {
      int sockun_fd;
      struct sockaddr_un sock_un_name;
    } un;
  } u;
};


class rpc_Server {

protected:
  rpc_PortHandle* port_h;
  std::string host;
  std::string port;

  rpc_Server(const std::string& host, const std::string& port) : host(host), port(port) {}
  int bind(const char** p_rpc_portname = NULL);
  virtual int listen();
  virtual void manageRequest(int fd, const char* request) = 0;

  virtual ~rpc_Server() {
    free(port_h->portname);
    delete port_h;
  }
};

class rpc_Client {

protected:
  int sock_fd;
  std::string host;
  std::string port;

  rpc_Client(const std::string& host, const std::string& port) : host(host), port(port) {}
  int open();
  int close();
};

extern char* rpc_readStringData(int fd);
extern ssize_t rpc_writeStringData(int fd, const char* data, size_t len);

#endif
