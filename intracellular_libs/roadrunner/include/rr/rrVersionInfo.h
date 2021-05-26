#ifndef rrVersionInfoH
#define rrVersionInfoH
#include "rrExporter.h"
#include <string>

namespace rr {

#ifndef SWIG

#define RR_STRINGIZE2(s) #s
#define RR_STRINGIZE(s) RR_STRINGIZE2(s)

#define RR_VERSION_MAJOR 1
#define RR_VERSION_MINOR 4
#define RR_VERSION_PATCH 18
#define RR_VERSION_STR RR_STRINGIZE(RR_VERSION_MAJOR) "." \
  RR_STRINGIZE(RR_VERSION_MINOR) "." \
  RR_STRINGIZE(RR_VERSION_PATCH)


#if defined(__llvm__)
    #define RR_COMPILER "clang " RR_STRINGIZE(__clang_version__)
#endif

#if defined(__GNUC__) && !defined(__llvm__)
    #define RR_COMPILER "gcc " RR_STRINGIZE(__GNUC__) "." RR_STRINGIZE(__GNUC_MINOR__) "." RR_STRINGIZE(__GNUC_PATCHLEVEL__)
#endif

#if defined (_MSC_VER)
    #if (_MSC_VER == 1800)
        #define RR_COMPILER "Microsoft Visual Studio 2013"
    #elif (_MSC_VER == 1700)
        #define RR_COMPILER "Microsoft Visual Studio 2012"
    #elif (_MSC_VER == 1600)
        #define RR_COMPILER "Microsoft Visual Studio 2010"
    #elif (_MSC_VER == 1500)
        #define RR_COMPILER "Microsoft Visual Studio 2008"
    #elif (_MSC_VER == 1400)
        #define RR_COMPILER "Microsoft Visual Studio 2005"
    #elif (_MSC_VER == 1310)
        #define RR_COMPILER "Microsoft Visual Studio 2003"
    #else
        #define RR_COMPILER "Microsoft Visual Studio Unknown Version"
    #endif
#endif

#if defined (__BORLANDC__)
    #define RR_COMPILER "Borland"
#endif

#if !defined(RR_COMPILER)
    #define RR_COMPILER "Unknown, but evidently working compiler"
#endif

#endif // SWIG

/**
 * various options of how the get the version string.
 */
enum VersionStrOptions
{
    /**
     * the basic version string, i.e. "1.0.0"
     */
    VERSIONSTR_BASIC                  = (0x1 << 0),

    /**
     * the compiler string
     */
    VERSIONSTR_COMPILER               = (0x1 << 1),

    /**
     * when roadrunner was compiled
     */
    VERSIONSTR_DATE                   = (0x1 << 2),

    /**
     * the version of libSBML we're using
     */
    VERSIONSTR_LIBSBML                = (0x1 << 3),

    VERSIONSTR_JITCOMPILER            = (0x1 << 4),

    /**
     * what git branch we are on.
     */
    VERSIONSTR_GIT_BRANCH             = (0x1 << 5),

    /**
     * the last commit.
     */
    VERSIONSTR_GIT_COMMIT             = (0x1 << 6)
};

/**
 * get the roadrunner version string.
 *
 * The options can be any set of the values from the VersionStrOptions enum.
 *
 * Each one of these options are concatenated into the output string and there
 * are ';' separators between each of them.
 */
RR_DECLSPEC std::string getVersionStr(unsigned options = VERSIONSTR_BASIC
        | VERSIONSTR_COMPILER | VERSIONSTR_LIBSBML | VERSIONSTR_JITCOMPILER | VERSIONSTR_DATE);


/**
 * get the copyright string.
 */
RR_DECLSPEC std::string getCopyrightStr();


}

#endif
