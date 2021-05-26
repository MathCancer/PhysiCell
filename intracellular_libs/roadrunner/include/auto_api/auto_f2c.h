/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

    - From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef auto_f2cH
#define auto_f2cH

#include <stdlib.h>
#include <math.h>

typedef long int integer;
typedef float auto_real;
typedef double doublereal;
typedef struct { auto_real r, i; } auto_complex;
typedef struct { doublereal r, i; } doublecomplex;
typedef integer logical;

#define TRUE_ (1)
#define FALSE_ (0)

/* I/O stuff */

#define VOID void

#ifndef abs
#define abs(x) ((x) >= 0 ? (x) : -(x))
#endif
#ifndef fabs
#define fabs(x) ((x) >= 0 ? (x) : -(x))
#endif
#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

#define dmin(a,b) (doublereal)min(a,b)
#define dmax(a,b) (doublereal)max(a,b)

#define ARRAY2D(array,i,j) array[(i) + (j) * array ## _dim1]
#define ARRAY3D(array,i,j,k) array[(i) + ((j)  + (k) * array ## _dim2) * array ## _dim1]

/* cabs.c */
double f__cabs(double real, double imag);
/* d_imag.c */
double d_imag(doublecomplex *z);
/* d_lg10.c */
double d_lg10(doublereal *x);
/* d_sign.c */
double d_sign(doublereal a, doublereal b);
/* etime_.c */
double etime(float *tarray);
/* i_dnnt.c */
integer i_dnnt(doublereal *x);
/* i_nint.c */
integer i_nint(auto_real *x);
/* pow_dd.c */
double pow_dd(doublereal *ap, doublereal *bp);
/* pow_di.c */
double pow_di(doublereal *ap, integer *bp);
/* pow_ii.c */
integer pow_ii(integer ap, integer bp);
/* r_lg10.c */
double r_lg10(auto_real x);
/* z_abs.c */
double z_abs(doublecomplex *z);
/* z_exp.c */
void z_exp(doublecomplex *r, doublecomplex *z);
/* z_log.c */
void z_log(doublecomplex *r, doublecomplex *z);

#endif


