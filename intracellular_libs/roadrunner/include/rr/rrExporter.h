#ifndef rrExporterH
#define rrExporterH

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(STATIC_RR) || defined(RR_STATIC)
        #define RR_DECLSPEC
    #else
        #if defined(EXPORT_RR)
            #define RR_DECLSPEC __declspec(dllexport)
        #else
            #define RR_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define RR_DECLSPEC
#endif

#endif
