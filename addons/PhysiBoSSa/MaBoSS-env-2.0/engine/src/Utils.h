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
     Utils.h

   Authors:
     Eric Viara <viara@sysra.com>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     May 2018
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <assert.h>
#include <iostream>

class ConfigOpt {
  std::string file_or_expr;
  bool is_expr;
  static unsigned int runconfig_file_cnt;
  static unsigned int runconfig_expr_cnt;
public:
  ConfigOpt(const std::string& file_or_expr, bool is_expr) : file_or_expr(file_or_expr), is_expr(is_expr) {
    if (is_expr) {runconfig_expr_cnt++;} else {runconfig_file_cnt++;}
  }
  bool isExpr() const {return is_expr;}
  const std::string& getExpr() const {assert(is_expr); return file_or_expr;}
  const std::string& getFile() const {assert(!is_expr); return file_or_expr;}

  static unsigned int getFileCount() {return runconfig_file_cnt;}
  static unsigned int getExprCount() {return runconfig_expr_cnt;}
};

extern int checkArgMissing(const char* prog, const char* opt, int nn, int argc);
extern int fileGetContents(const std::string& file, std::string& contents);
extern int filePutContents(const std::string& file, const std::string& data);
extern std::string stringReplaceAll(const std::string& subject, const std::string& from, const std::string& to);

extern const std::string NL_PATTERN;
extern const char* fmthexdouble(double d);

class NullBuffer : public std::streambuf
{
public:
  int overflow(int c) { return c; }
};

static NullBuffer null_buffer;

char* maboss_strdup (const char* s);

#endif
