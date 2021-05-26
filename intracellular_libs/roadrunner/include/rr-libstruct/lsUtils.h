#ifndef lsUtilsH
#define lsUtilsH
/// This file contains several useful utility functions

#include "lsExporter.h"
#include "lsLibla.h"

#define CREATE_ARRAY(variableName,type,length)\
    if(variableName) { delete[] variableName; variableName = NULL;}\
    variableName = new type[length]; memset(variableName, 0, sizeof(type)*length);//

#define DELETE_ARRAY_IF_NON_NULL(target)\
    if(target) { delete[] target; target = NULL;}

#define DELETE_IF_NON_NULL(target)\
    if(target) { delete target; target = NULL;}


#include <string>
#include <ostream>
#include <stdexcept>
using std::ostream;
using std::string;
namespace ls
{
LIB_EXTERN int**            matMult(int mA, int nA, int **A, int **B, int nB);
LIB_EXTERN double**         matMult(int mA, int nA, double **A, double **B, int nB);
LIB_EXTERN void             checkTolerance(int nrows, double *A, double dTolerance);
LIB_EXTERN void             checkTolerance(int nrows, int ncols, double **A, double dTolerance);
LIB_EXTERN DoubleMatrix*    getSubMatrix(int Mb, int Nb, int ms, int ns, int mi, int nj, DoubleMatrix& A);
LIB_EXTERN DoubleMatrix*    matMult(unsigned int mA, unsigned int nA, DoubleMatrix &A, DoubleMatrix &B, unsigned int nB);
LIB_EXTERN DoubleMatrix*    matMult(DoubleMatrix &A, DoubleMatrix &B);
LIB_EXTERN DoubleMatrix*    matMult(IntMatrix &A, DoubleMatrix &B);
LIB_EXTERN IntMatrix*       matMult(IntMatrix &A, IntMatrix &B);

LIB_EXTERN bool             isPositive(DoubleMatrix &A, double dTolerance);

LIB_EXTERN void             CopyMatrix(DoubleMatrix& oMatrix, double** &outMatrix, int &numRows, int &numCols);
LIB_EXTERN void             CopyMatrix(IntMatrix& oMatrix, int** &outMatrix, int &outNumRows, int &outNumCols);
LIB_EXTERN void             CopyMatrix(ComplexMatrix& oMatrix, double** &outMatrixReal,double** &outMatrixImag, int &outNumRows, int &outNumCols);
LIB_EXTERN void             CopyStringVector(const std::vector< std::string > &vector, char** &outVector, int &outLength);
LIB_EXTERN void             CopyDoubleVector(const std::vector< double > &vector, double* &outVector, int &outLength);
LIB_EXTERN void             CopyIntVector(const std::vector< int > &vector, int* &outVector, int &outLength);
LIB_EXTERN void             CopyComplexVector(const std::vector< Complex> &vector, double* &outVectorReal, double* &outVectorImag, int &outLength);

LIB_EXTERN void             RoundMatrixToTolerance(DoubleMatrix& oMatrix, double dTolerance);
LIB_EXTERN double           RoundToTolerance(double dValue, double dTolerance);

LIB_EXTERN std::vector<int> GaussJordan(DoubleMatrix &oMatrix, double dTolerance);
LIB_EXTERN void             FullyPivotedGaussJordan(DoubleMatrix &oMatrix, double dTolerance, std::vector< int > &rowPivots, std::vector< int > &colPivots);

LIB_EXTERN void             gaussJordan(DoubleMatrix &oMatrix, double dTolerance);
LIB_EXTERN int              findRank(DoubleMatrix &oMatrix, double dTolerance);

LIB_EXTERN string           print(int mr, int nc, int *A);
//static void print(int mr, int nc, integer *A);
LIB_EXTERN string           print(int mr, int nc, int **A);
LIB_EXTERN string           print(int mr, int nc, double *A);
LIB_EXTERN string           print(int mr, int nc, double **A);
LIB_EXTERN string           print(IntMatrix& A);
LIB_EXTERN string           print(DoubleMatrix& A);
LIB_EXTERN string           print(ComplexMatrix& A);
//static void print(int mr, int nc, doublecomplex *A);
//static void print(int mr, int nc, doublecomplex **A);
LIB_EXTERN string           print(int mr, int nc, ls::Complex *A);
LIB_EXTERN string           print(int mr, int nc, ls::Complex **A);
LIB_EXTERN string           print(int mr, int nc, int *A, int *B);
LIB_EXTERN string           print(int mr, int nc, int **A, int **B);
LIB_EXTERN string           print(int mr, int nc, double **A, double **B);



LIB_EXTERN class ApplicationException: public std::runtime_error
{
public:
    explicit ApplicationException(const std::string& what) :
            std::runtime_error(what)
    {
    }

    explicit ApplicationException(const std::string& what, const std::string &msg2) :
            std::runtime_error(what + ", " + msg2)
    {
    }
};


}

#endif
