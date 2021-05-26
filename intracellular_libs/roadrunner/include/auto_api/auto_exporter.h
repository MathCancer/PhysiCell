#ifndef autoExporterH
#define autoExporterH

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(STATIC_AUTO)
        #define AUTO_DECLSPEC
    #else
        #if defined(EXPORT_AUTO)
            #define AUTO_DECLSPEC __declspec(dllexport)
        #else
            #define AUTO_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define AUTO_DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
    #define autoCallConv __cdecl
#else
    #define autoCallConv
#endif

#endif
