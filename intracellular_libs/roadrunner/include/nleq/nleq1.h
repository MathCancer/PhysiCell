#ifndef nleq1H
#define nleq1H

//How is this library being built?
//If you are building, or using, this as a static library, then you need to define STATIC_NLEQ
#if defined(WIN32)

#if defined(STATIC_NLEQ)
#define DLLEXPORT
#else

//DLL import/export
#if defined(SHARED_NLEQ)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif	//DLL import/export

#endif //STATIC_NLEQ
#else
#define DLLEXPORT
#endif


#if defined(WIN32)
#define STDCALL   __stdcall
#else
#define STDCALL  
//__attribute__((stdcall))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "f2c_nleq.h"

DLLEXPORT int STDCALL NLEQ1
(
    integer *n, 		
    c_NLMFCN fcn,
    U_fp jac, 			
    doublereal *x,
    doublereal *xscal, 
    doublereal *rtol,
    integer *iopt, 		
    integer *ierr,
    integer *liwk, 		
    integer *iwk,
    integer *lrwk, 		
    doublereal *rwk
);

#ifdef __cplusplus
}
#endif

#endif

