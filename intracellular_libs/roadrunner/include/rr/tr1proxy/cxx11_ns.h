#ifndef _INCLUDED_CXX_11_NS_H_
#define _INCLUDED_CXX_11_NS_H_

#if __APPLE__
  #include <AvailabilityMacros.h>
  #if !defined(MAC_OS_X_VERSION_10_9)
    #define MAC_OS_X_VERSION_10_9 1090
  #endif

  // is Mavericks
  #if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_9) || (__cplusplus >= 201103L)
    #define cxx11_ns std
  #else
    // need to define that we've defined std ns for SWIG.
    #define CXX11_NS_TR1
    #define cxx11_ns std::tr1
  #endif                                                   // OSX ver
#else                                                      // not __APPLE__
  #if (__cplusplus >= 201103L) || defined(_MSC_VER)
    #define cxx11_ns std
  #else
    // need to define that we've defined std ns for SWIG.
    #define CXX11_NS_TR1
    #define cxx11_ns std::tr1
  #endif
#endif                                                     // __APPLE__


#endif // _INCLUDED_CXX_11_NS_H_
