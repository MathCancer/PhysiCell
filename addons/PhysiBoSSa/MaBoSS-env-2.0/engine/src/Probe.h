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
     Probe.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _PROBE_H_
#define _PROBE_H_

#ifndef WINDOWS

#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

class Probe {

  struct tms tms0, tms1;
  struct timeval tv0, tv1;
  double ticks_per_second;

 public:
  Probe(bool _start = true) {
    ticks_per_second = (double)sysconf(_SC_CLK_TCK);
    if (_start) {
      start();
    }
  }

  void start() {
    gettimeofday(&tv0, NULL);
    times(&tms0);
  }

  void stop() {
    gettimeofday(&tv1, NULL);
    times(&tms1);
  }

  long long elapsed_usecs() const {
    return Probe::usecs(tv0, tv1);
  }

  long long elapsed_msecs() const {
    return Probe::msecs(tv0, tv1);
  }

  long long user_msecs() const {
    return ((tms1.tms_utime - tms0.tms_utime)/ticks_per_second)*1000;
  }

  long long sys_msecs() const {
    return ((tms1.tms_stime - tms0.tms_stime)/ticks_per_second)*1000;
  }

  static long long usecs(const timeval& tv0, const timeval& tv1) {
    return (tv1.tv_sec - tv0.tv_sec) * 1000000 + (tv1.tv_usec - tv0.tv_usec);
  }

  static long long msecs(const timeval& tv0, const timeval& tv1) {
    return usecs(tv0, tv1)/1000;
  }
};

#else

#include <ctime>   // CPU (system, user) clock

#include <chrono>  // wall (real) clock



class Probe {



  // struct tms tms0, tms1;

  // struct timeval tv0, tv1;



//  std::chrono::time_point tms0,tms1;

//  std::chrono::high_resolution_clock::time_point tms0,tms1;



  std::clock_t c_start, c_stop;     // CPU (sys, user process) time

  std::chrono::high_resolution_clock::time_point tv0,tv1;   // real (wall) time



  double ticks_per_second;



 public:

  Probe(bool _start = true) {

    // ticks_per_second = (double)sysconf(_SC_CLK_TCK);  // = 100

    ticks_per_second = CLOCKS_PER_SEC;  // = 100

    // std::cout << "ticks_per_second= " << ticks_per_second << "\n";

    if (_start) {

      start();

    }

  }



  void start() {

//    gettimeofday(&tv0, NULL);   // for "real" time (wall clock)

//    times(&tms0);   // for "user" and "system" CPU time



    // std::chrono::system_clock represents the system-wide real time wall clock.

    tv0 = std::chrono::high_resolution_clock::now();



    c_start = std::clock();

  }



  void stop() {

    // gettimeofday(&tv1, NULL);

    // times(&tms1);



    tv1 = std::chrono::high_resolution_clock::now();



    c_stop = std::clock();

  }



  long long elapsed_usecs() const {

    auto int_ms = std::chrono::duration_cast<std::chrono::microseconds>(tv1 - tv0);

    return( int_ms.count());

  }



 long long elapsed_msecs() const {  // long long = 8 bytes (64 bits)

    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tv1 - tv0);

    return( int_ms.count());

  }



  long long user_msecs() const {

    return (1000 * (c_stop - c_start) / CLOCKS_PER_SEC);

  }



  long long sys_msecs() const {

    return (1000.0 * (c_stop - c_start) / CLOCKS_PER_SEC);

  }



  // static long long usecs(const timeval& tv0, const timeval& tv1) {

  //   return (tv1.tv_sec - tv0.tv_sec) * 1000000 + (tv1.tv_usec - tv0.tv_usec);

  // }



  // static long long msecs(const timeval& tv0, const timeval& tv1) {

  //   return usecs(tv0, tv1)/1000;

  // }

};
#endif

#endif