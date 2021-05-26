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
     RandomGenerator.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
     Vincent Noël <vincent.noel@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_

// DO NOT define USE_DUMMY_RANDOM: except for profiling purpose: this flag has been introduced to get an estimation of random number generation time
//#define USE_DUMMY_RANDOM

//#define RANDOM_TRACE
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <random>
#include <exception>
#include "maboss-config.h"

class RandomGenerator {

  static size_t generated_number_count;

 protected:
  static void incrGeneratedNumberCount() {generated_number_count++;}

 public:

  static void resetGeneratedNumberCount() {generated_number_count = 0;}
  virtual std::string getName() const = 0;

  virtual bool isPseudoRandom() const = 0;

  virtual unsigned int generateUInt32() = 0;

  virtual double generate() = 0;

  virtual void setSeed(int seed) { }

  static size_t getGeneratedNumberCount() {return generated_number_count;}

  virtual ~RandomGenerator() {}
};

class PhysicalRandomGenerator : public RandomGenerator {
  int fd;

 public:
  PhysicalRandomGenerator() {
    fd = open("/dev/urandom", O_RDONLY);
    assert(fd >= 0);
  }

  bool isPseudoRandom() const {
    return false;
  }

  std::string getName() const {
    return "physical";
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
    unsigned int result;
    // When compiling with NDEBUG, we don't have asserts, to res is not used
    // This is just to make the compiler happy even in this case
#ifndef NDEBUG 
    int ret = read(fd, &result, sizeof(result));
    assert(ret == sizeof(result));
#else
    int res = read(fd, &result, sizeof(result));
    if (res != sizeof(result)) 
      throw std::exception();
#endif
#ifdef RANDOM_TRACE
    std::cout << result << '\n';
#endif
    return result;
  }

  virtual double generate() {
    double result = ((double)generateUInt32())/~0U; // fixed this 2014-10-17, but I think I added /2 because it did not work
#ifdef RANDOM_TRACE
    std::cout << result << '\n';
#endif
    return result;
  }

  ~PhysicalRandomGenerator() {
    if (fd >= 0) {
      close(fd);
    }
  }
};

class Rand48RandomGenerator: public RandomGenerator
{

#define RAND48_N	16
#define RAND48_MASK	((unsigned)(1 << (RAND48_N - 1)) + (1 << (RAND48_N - 1)) - 1)
#define RAND48_LOW(x)	((unsigned)(x) & RAND48_MASK)
#define RAND48_HIGH(x)	RAND48_LOW((x) >> RAND48_N)
#define RAND48_MUL(x, y, z)	{ long l = (long)(x) * (long)(y); \
		(z)[0] = RAND48_LOW(l); (z)[1] = RAND48_HIGH(l); }
#define RAND48_CARRY(x, y)	((long)(x) + (long)(y) > (long)(RAND48_MASK))
#define RAND48_ADDEQU(x, y, z)	(z = RAND48_CARRY(x, (y)), x = RAND48_LOW(x + (y)))
#define RAND48_X0	0x330E
#define RAND48_X1	0xABCD
#define RAND48_X2	0x1234
#define RAND48_A0	0xE66D
#define RAND48_A1	0xDEEC
#define RAND48_A2	0x5
#define RAND48_C	0xB
#define RAND48_SET3(x, x0, x1, x2)	((x)[0] = (x0), (x)[1] = (x1), (x)[2] = (x2))
#define RAND48_SETLOW(x, y, n) RAND48_SET3(x, RAND48_LOW((y)[n]), RAND48_LOW((y)[(n)+1]), RAND48_LOW((y)[(n)+2]))
#define RAND48_SEED(x0, x1, x2) (RAND48_SET3(x, x0, x1, x2), RAND48_SET3(a, RAND48_A0, RAND48_A1, RAND48_A2), c = RAND48_C)
#define RAND48_REST(v)	for (i = 0; i < 3; i++) { xsubi[i] = x[i]; x[i] = temp[i]; } \
		return (v);
#define RAND48_NEST(TYPE, f, F)	TYPE f(xsubi) register unsigned short *xsubi; { \
	register int i; register TYPE v; unsigned temp[3]; \
	for (i = 0; i < 3; i++) { temp[i] = x[i]; x[i] = RAND48_LOW(xsubi[i]); }  \
	v = F(); RAND48_REST(v); }
#define RAND48_HI_BIT	(1L << (2 * RAND48_N - 1))

  unsigned x[3] = { RAND48_X0, RAND48_X1, RAND48_X2 }, a[3] = { RAND48_A0, RAND48_A1, RAND48_A2 }, c = RAND48_C;
  int seed;
public:
  Rand48RandomGenerator(int seed)
  {
    setSeed(seed);
  }
  bool isPseudoRandom() const {
    return true;
  }

  std::string getName() const {
    return "rand48";
  }

  void next()
  {
    unsigned p[2], q[2], r[2], carry0, carry1;

    RAND48_MUL(a[0], x[0], p);
    RAND48_ADDEQU(p[0], c, carry0);
    RAND48_ADDEQU(p[1], carry0, carry1);
    RAND48_MUL(a[0], x[1], q);
    RAND48_ADDEQU(p[1], q[0], carry0);
    RAND48_MUL(a[1], x[0], r);
    x[2] = RAND48_LOW(carry0 + carry1 + RAND48_CARRY(p[1], r[0]) + q[1] + r[1] +
      a[0] * x[2] + a[1] * x[1] + a[2] * x[0]);
    x[1] = RAND48_LOW(p[1] + r[0]);
    x[0] = RAND48_LOW(p[0]);
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
	  
    next();
	
#ifdef RANDOM_TRACE
    std::cout << ((unsigned int)(((long)x[2] << (RAND48_N - 1)) + (x[1] >> 1))) << '\n';
#endif
    return (((unsigned int)x[2] << (RAND48_N - 1)) + (x[1] >> 1));
  }

  virtual double generate() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return 0.5;
#endif
    
    double two16m = 1.0 / (1L << RAND48_N);
    next();

#ifdef RANDOM_TRACE
    std::cout << (two16m * (two16m * (two16m * x[0] + x[1]) + x[2])) << '\n';
#endif
    return (two16m * (two16m * (two16m * x[0] + x[1]) + x[2]));
  }

  virtual void setSeed(int seed) {
    this->seed = seed;
	  RAND48_SEED(RAND48_X0, RAND48_LOW(seed), RAND48_HIGH(seed));
  }
};

class GLibCRandomGenerator : public RandomGenerator
{
  // Info on this PRNG : https://www.mscs.dal.ca/~selinger/random/
  // rand() call simplification : https://stackoverflow.com/a/26630526
  // This is the default prng used by rand, when srand is called
  // To be more accurate, this is the TYPE_3 rand() algorithm (https://code.woboq.org/userspace/glibc/stdlib/random.c.html)
  // It is based on a linear-feedback shift register, with the polynomial being x^31 + x^3 + 1
  // The nice thing about having it here and not in the GNU C library is that we can make it thread_safe
  
  int seed;
  
  #define SIZE_R 344
  #define GLIBCRAND_MAX 2147483647
  
  int n;
  int r[SIZE_R];

  void glibc_srand(int seed) {

    /* We must make sure the seed is not 0.  Take arbitrarily 1 in this case.  
       Source: https://sourceware.org/git/?p=glibc.git;a=blob;f=stdlib/random_r.c;hb=glibc-2.15#l180
    */
    if (seed == 0)
      seed = 1;
    
    int i;
    r[0] = seed;
    for (i=1; i<31; i++) {
        r[i] = (16807LL * r[i-1]) % GLIBCRAND_MAX;
        if (r[i] < 0) {
        r[i] += GLIBCRAND_MAX;
        }
    }
    for (i=31; i<34; i++) {
        r[i] = r[i-31];
    }
    for (i=34; i<SIZE_R; i++) {
        r[i] = r[i-31] + r[i-3];
    }
    n = 0;
  }

  unsigned int glibc_rand() {
    unsigned int x = r[n%SIZE_R] = r[(n+313)%SIZE_R] + r[(n+341)%SIZE_R];
    n = (n+1)%SIZE_R;
    return x >> 1;
  }

 public:
  GLibCRandomGenerator(int seed) : seed(seed) {
    glibc_srand(seed);
  }

  bool isPseudoRandom() const {
    return true;
  }

  std::string getName() const {
    return "glibc";
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
    return glibc_rand();
  }

  virtual double generate() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return 0.5;
#endif
    return double(glibc_rand()) / GLIBCRAND_MAX;
  }

  virtual void setSeed(int seed) {
    this->seed = seed;
    glibc_srand(seed);
  }

};

class MT19937RandomGenerator : public RandomGenerator
{
  // Info on this PRNG : http://www.cplusplus.com/reference/random/mt19937/

  int seed;
  std::mt19937 generator;

  void mt19937_srand(int seed) {
    std::mt19937 generator(seed);
  }

  unsigned int mt19937_rand() {
    return generator();
  }

 public:
  MT19937RandomGenerator(int seed) : seed(seed) {
    mt19937_srand(seed);
  }

  bool isPseudoRandom() const {
    return true;
  }

  std::string getName() const {
    return "mt19937";
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
    return mt19937_rand();
  }

  virtual double generate() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return 0.5;
#endif
    return (double(mt19937_rand()) / generator.max());
  }

  virtual void setSeed(int seed) {
    this->seed = seed;
    mt19937_srand(seed);
  }

};


class RandomGeneratorFactory {

public:
  enum Type {
    DEFAULT = 1,
    GLIBC,
    MERSENNE_TWISTER,
    PHYSICAL
  };

private:
  Type type;

public:
  RandomGeneratorFactory(Type type) : type(type) { }

  RandomGenerator* generateRandomGenerator(int seed=1) const {
    switch(type) {
    case DEFAULT:
      return new Rand48RandomGenerator(seed);
    case GLIBC:
      return new GLibCRandomGenerator(seed);
    case MERSENNE_TWISTER:
      return new MT19937RandomGenerator(seed);
    case PHYSICAL:
      return new PhysicalRandomGenerator();
    default:
      abort();
      return NULL;
    }
  }

  std::string getName() const {
    switch(type) {
    case DEFAULT:
      return "rand48";
    case GLIBC:
      return "glibc";   
    case MERSENNE_TWISTER:
      return "mt19937";
    case PHYSICAL:
      return "physical";
    default:
      abort();
      return NULL;
    }
  }

  bool isPseudoRandom() const {
    switch(type) {
    case DEFAULT:
      return true;      
    case GLIBC:
      return true;
    case MERSENNE_TWISTER:
      return true;
    case PHYSICAL:
      return false;
    default:
      abort();
      return false;
    }
  }

  bool isThreadSafe() const {
    switch(type) {
    case DEFAULT:
      return true;
    case GLIBC:
      return true;
    case MERSENNE_TWISTER:
      return true;
    case PHYSICAL:
      return true;
    default:
      abort();
      return false;
    }
  }
};

#endif
