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
     Utils.cc

   Authors:
     Eric Viara <viara@sysra.com>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     May 2018
*/

#include "Utils.h"
#include "MaBEstEngine.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

const std::string NL_PATTERN = "@--NL--@";
unsigned int ConfigOpt::runconfig_file_cnt = 0;
unsigned int ConfigOpt::runconfig_expr_cnt = 0;

int checkArgMissing(const char* prog, const char* opt, int nn, int argc)
{
  if (nn == argc-1) {
    std::cerr << '\n' << prog << ": missing value after option " << opt << '\n';
    return 1;
  }
  return 0;
}

int fileGetContents(const std::string& file, std::string& contents)
{
  int fd = open(file.c_str(), O_RDONLY);
  if (fd < 0) {
    std::cerr << "cannot open file " + file + " for reading" << std::endl;
    return 1;
  }
  struct stat st;
  if (fstat(fd, &st) < 0) {
    std::cerr << "cannot stat file " + file + "" << std::endl;
    return 1;
  }

  size_t size = st.st_size;
  char* buffer = new char[size+1];
  buffer[size] = 0;

  size_t size_read = 0;
  for (;;) {
    ssize_t rsize = read(fd, buffer+size_read, size-size_read);
    if (rsize <= 0) {
      perror("read");
      return 1;
    }
    size_read += rsize;
    if (size_read == size) {
      break;
    }
    if (size_read > size) {
      perror("read");
      return 1;
    }
  }
  close(fd);

  contents = buffer;
  delete [] buffer;
  return 0;
}

int filePutContents(const std::string& file, const std::string& data)
{
  FILE* fd = fopen(file.c_str(), "w");
  if (fd == NULL) {
    std::cerr << "cannot open file " + file + " for writing" << std::endl;
    return 1;
  }
  fputs(data.c_str(), fd);
  fclose(fd);

  return 0;
}

std::string stringReplaceAll(const std::string& subject, const std::string& from, const std::string& to)
{
  std::string str = subject;
  std::string::size_type from_size = from.length();
  std::string::size_type to_size = to.length();
  std::string::size_type itpos;
  std::string::size_type pos;

  pos = 0;
  while ((itpos = str.find(from, pos)) != std::string::npos) {
    str = str.replace(str.begin()+itpos, str.begin()+itpos+from_size, to);
    pos = itpos + to_size;
  }

  return str;

}

// EV: 2018-12-19
// std::hexfloat is missing on some gcc compiler version
// introduced this function to print double in hexa format
const char* fmthexdouble(double d)
{
  static const int MAXBUF_FMTDOUBLE = 8;
  static char buf[MAXBUF_FMTDOUBLE][64];
  static int buf_ind = 0;
  if (buf_ind >= MAXBUF_FMTDOUBLE) {
    buf_ind = 0;
  }
  sprintf(buf[buf_ind], "%a", d);
  return buf[buf_ind++];
}


char* maboss_strdup (const char* s)
{
  size_t slen = strlen(s);
  char* result = (char*) malloc(slen + 1);
  if(result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}