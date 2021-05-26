#ifndef rrOSSpecificsH
#define rrOSSpecificsH
#include "rrExporter.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4018) // int to unsigned int comparison
#pragma warning(disable : 4482) // prefixing enums...
#pragma warning(disable : 4251) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4221) // empty cpp file

// supported in visual studio:
// http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx
#define __FUNC__ __FUNCDNAME__
#define __func__ __FUNCDNAME__
#endif

#if defined(__BORLANDC__)
#pragma warn -8012             //comparing unsigned and signed
#pragma warn -8004             //variable never used
#endif

//---------------------------------------------------------------------------
#if defined (__MINGW32__) || defined(__linux) || defined (__APPLE__)
#define __FUNC__ __PRETTY_FUNCTION__
#endif

#ifdef _MSC_VER

#ifdef _MSC_VER
#if _MSC_VER < 1900
#  define snprintf _snprintf
#endif
#endif

//#include <stdarg.h>
//#define snprintf c99_snprintf
//RR_DECLSPEC int c99_snprintf(char* str, size_t size, const char* format, ...);
//RR_DECLSPEC int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap);

#endif // _MSC_VER


#if defined (__GNUC__) || defined(__clang__)
    #define RR_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
    #define RR_DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
    #define RR_DEPRECATED(func) func
#endif


#if defined(_MSC_VER)
    #define rrCallConv __cdecl
#elif defined(__BORLANDC__)
    #define rrCallConv __cdecl
#else
    #define rrCallConv
#endif

typedef unsigned int uint;
typedef unsigned int u_int;
typedef unsigned char uchar;


#endif
