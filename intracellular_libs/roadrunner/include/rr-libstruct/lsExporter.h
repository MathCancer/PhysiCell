#ifndef lsExporterH
#define lsExporterH

#if defined( _WIN32 )

    #if defined( STATIC_LIBSTRUCT )          //Libstruct static
        #define LIB_EXTERN
    #else
        #if defined( SHARED_LIBSTRUCT )       //Export libstruct
            #define LIB_EXTERN __declspec(dllexport)
        #else
            #define LIB_EXTERN __declspec(dllimport)
        #endif
    #endif

#else //WIN32
    #define LIB_EXTERN
#endif

#endif


