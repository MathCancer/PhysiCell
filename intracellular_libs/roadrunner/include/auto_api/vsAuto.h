#ifndef vsAutoH
#define vsAutoH
#include <string>
#include "libf2c/auto_f2c.h"
#include "auto_exporter.h"
using std::string;

typedef  void (autoCallConv *FuncCallBack2)(const doublereal *u, const doublereal *par, doublereal *f);
typedef  int  (autoCallConv *StpntCallBack)(integer ndim, doublereal t, doublereal *u, doublereal *par);
typedef  int  (autoCallConv *PvlsCallBack)( integer ndim, const void *u, doublereal *par);
typedef  int  (autoCallConv *FuncCallBack)( integer ndim, const doublereal *u, const integer *icp,
                                            const doublereal *par, integer ijac, doublereal *f, doublereal *dfdu,
                                            doublereal *dfdp);

typedef  int  (autoCallConv *BcndCallBack)( integer ndim, const doublereal *par, const integer *icp,
                                            integer nbc, const doublereal *u0, const doublereal *u1, integer ijac,
                                            doublereal *fb, doublereal *dbc);

typedef  int  (autoCallConv *IcndCallBack)( integer ndim, const doublereal *par, const integer *icp,
                                            integer nint, const doublereal *u, const doublereal *uold,
                                            const doublereal *udot, const doublereal *upold, integer ijac,
                                            doublereal *fi, doublereal *dint);

typedef  int  (autoCallConv *FoptCallBack)( integer ndim, const doublereal *u, const integer *icp,
                                            const doublereal *par, integer ijac,
                                            doublereal *fs, doublereal *dfdu, doublereal *dfdp);

#if defined(__cplusplus)
namespace autolib
{
extern "C"
{
#endif

AUTO_DECLSPEC void    autoCallConv setCallbackFunc2(FuncCallBack2 cb);
AUTO_DECLSPEC void    autoCallConv setCallbackFunc(FuncCallBack cb);
AUTO_DECLSPEC void    autoCallConv setCallbackStpnt(StpntCallBack cb);
AUTO_DECLSPEC void    autoCallConv setCallbackBcnd(BcndCallBack cb);
AUTO_DECLSPEC void    autoCallConv setCallbackIcnd(IcndCallBack cb);
AUTO_DECLSPEC void    autoCallConv setCallbackFopt(FoptCallBack cb);
AUTO_DECLSPEC void    autoCallConv setCallbackPvls(PvlsCallBack cb);
AUTO_DECLSPEC void    autoCallConv ResetAutoLib();
AUTO_DECLSPEC void    autoCallConv CallAuto(const string& tempFolder);
AUTO_DECLSPEC void    autoCallConv SetAutoNumParameters(int n);
AUTO_DECLSPEC void    autoCallConv createFort2File(const string& content, const string& fName);
AUTO_DECLSPEC void    autoCallConv createFort3File(const string& content, const string& fName);
AUTO_DECLSPEC char*   autoCallConv getFort7File(int length, const string& fldr);
AUTO_DECLSPEC char*   autoCallConv getFort8File(int length, const string& fldr);
AUTO_DECLSPEC char*   autoCallConv getFort9File(int length, const string& fldr);

#if defined( __cplusplus)
}//extern "C"
}//namespace autolib
#endif

#endif
