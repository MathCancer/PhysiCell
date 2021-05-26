#ifndef _INCLUDED_RR_UNORDERED_MAP_H_
#define _INCLUDED_RR_UNORDERED_MAP_H_
// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file rr_unordered_map.h
  * @author JKM
  * @date 12/15/2014
  * @copyright Apache License, Version 2.0
  * @brief A proxy for maintaining compatibility across C++ std library versions
  * @details C++11 moved tr1 headers and classes into the standard namespace,
  * hence no use for a tr1 prefix.
**/
#include "cxx11_ns.h"

#if __APPLE__
  // is Mavericks
  #if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_9) || (__cplusplus >= 201103L) || RR_USE_CXX11
    #include <unordered_map>
    #define RR_UNORDERED_MAP std::unordered_map
  #else
    #include <tr1/unordered_map>
    #define RR_UNORDERED_MAP std::tr1::unordered_map
  #endif                                                   // OSX ver
#else                                                      // not __APPLE__
  #if (__cplusplus >= 201103L) || defined(_MSC_VER) || RR_USE_CXX11
    #include <unordered_map>
    #define RR_UNORDERED_MAP std::unordered_map
  #else
    #include <tr1/unordered_map>
    #define RR_UNORDERED_MAP std::tr1::unordered_map
  #endif
#endif                                                     // __APPLE__

#endif //  _INCLUDED_RR_UNORDERED_MAP_H_
