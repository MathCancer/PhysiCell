/**
 * LibStruct, original author: Frank Bergmann.
 * Fixes and improvments: Totte Karsson
 */

#ifndef lsMatrixH
#define lsMatrixH

#if defined __unix || defined(__APPLE__)
#include <cstring> //for memset on unix platforms
#endif
#include <ostream>
#include <complex>
#include <vector>
#include "lsExporter.h"


namespace ls
{


typedef std::complex<double> Complex;


/**
 * @class lassMatrix
 * @brief Matrix is the matrix class used by ls::LibLA and LIB_STRUTURAL::LibStructural
 * This class implements a template to hold real, ls::Complex and integer matrices. It also implements basic
 * operations on matrices.
 */
template<class T>
class Matrix
{
public:

    /**
     * Creates a new matrix with the given numbers of rows and columns
     */
    Matrix(unsigned int rows = 0, unsigned int cols = 0);

    /**
     * Copy constructors
     */
    Matrix(const Matrix<double>& src);

    /**
     * copy constructor
     */
    Matrix(const Matrix<Complex>& src, bool real = true);

    /**
     * Constructor taking a matrix mapped to a vector and reconstructing the 2D form
     */
    Matrix(T* &oRawData, int nRows, int nCols, bool transpose = true);

    /**
     * constructs a matrix from 2D data
     */
    Matrix(T** &oRawData, int nRows, int nCols);

    /**
     * constructs a matrix from 2D const data
     */
    Matrix(const T** oRawData, int nRows, int nCols);

    /**
     * destructor
     */
    ~Matrix();

    T* GetPointer();
    bool isAllocated() const;

    /**
     * returns the number of columns
     */
    unsigned int CSize() const;
    unsigned int RSize() const;
    unsigned int Length() const;

    /**
     * returns a pointer to the underlying 1D array
     */
    T* getArray();

    /**
     * returns a copy of the data, optionally transposing it
     */
    T* getCopy(bool transpose = false);


    /**
     * initializes the matrix from 2D data
     */
    void initializeFrom2DMatrix(T** &oRawData, int nRows, int nCols);

    /**
     * initializes the matrix from 2D const data
     */
    void initializeFromConst2DMatrix(const T** oRawData, int nRows, int nCols);

    /**
     * returns a 2D data array
     */
    T** get2DMatrix(int &nRows, int &nCols);

    /**
     * swaps the given rows
     */
    void swapRows(unsigned int row1, unsigned int row2);

    /**
     * swaps the given columns
     */
    void swapCols(unsigned int col1, unsigned int col2);

    /**
     * resizes the matrix to the given number of rows and columns
     */
    void resize(unsigned int rows, unsigned int cols);

    bool Allocate(unsigned int rows, unsigned int cols);

    /**
     * creates a new matrix holding the transpose
     */
    Matrix<T>* getTranspose();

    /**
     * assignment operator
     */
    Matrix<T>& operator =(const Matrix<T>& rhs);

    /**
     * scalar assignment operator
     */
    Matrix<T>& operator =(const T & value);

    /**
     * returns the size of the matrix ??
     */
    unsigned int size() const;

    /**
     * returns the number of rows
     */
    unsigned int numRows() const;

    /**
     * returns the number of columns
     */
    unsigned int numCols() const;

    /**
     * returns the selected row
     */
    T* operator[](unsigned int row);

    /**
     * returns the selected row
     */
    const T* operator[](unsigned int row) const;

    /**
     * returns the selected matrix element
     */
    T& operator()(const unsigned int & row, const unsigned int & col);

    /**
     * returns the selected matrix element (const)
     */
    const T& operator()(const unsigned int & row,
            const unsigned int & col) const;

    const T& Element(const unsigned int & row, const unsigned int & col) const;

    /**
     * set the row names
     */
    void setRowNames(const std::vector<std::string> &rows) {
        rowNames = rows;
    }

    void setColNames(const std::vector<std::string> &cols) {
        colNames = cols;
    }

    /**
     * set the row names, same args as std::vector ctor
     */
    template<class strType>
    void setRowNames(const strType begin, const strType end) {
        rowNames = std::vector<std::string>(begin, end);
    }

    /**
     * set the column names, same args as std::vector ctor
     */
    template<class strType>
    void setColNames(const strType begin, const strType end) {
        colNames = std::vector<std::string>(begin, end);
    }

    /**
     * get the row names
     */
     const std::vector<std::string> &getRowNames() const {
        return rowNames;
    }

    /**
     * get the column names
     */
    const std::vector<std::string> &getColNames() const {
        return colNames;
    }

    /**
     * get the row names (mutable)
     */
     std::vector<std::string> &getRowNames() {
        return rowNames;
    }

    /**
     * get the column names (mutable)
     */
    std::vector<std::string> &getColNames() {
        return colNames;
    }


protected:
    /**
     * number of rows
     */
    unsigned int _Rows;

    /**
     * number of columns
     */
    unsigned int _Cols;

    /**
     * pointer to block of memory of the data, stored in row-major format.
     */
    T* _Array;

    /**
     * vector of column names, may be empty
     */
    std::vector<std::string> colNames;

    /**
     * vector of row names, may be empty
     */
    std::vector<std::string> rowNames;

};

//! hide templates in signatures
typedef Matrix< double >            DoubleMatrix;
typedef Matrix< int >               IntMatrix;
typedef Matrix< Complex >           ComplexMatrix;

//Exported Utility Matrix functions
LIB_EXTERN DoubleMatrix             real(const ComplexMatrix& m2);               //Return real part of complex matrix
LIB_EXTERN DoubleMatrix             imag(const ComplexMatrix& m2);               //Return imag part of complex matrix

LIB_EXTERN DoubleMatrix             mult(DoubleMatrix& m1, DoubleMatrix& m2);
LIB_EXTERN DoubleMatrix             mult(ComplexMatrix& m1, DoubleMatrix& m2);
LIB_EXTERN DoubleMatrix             mult(DoubleMatrix& m1, ComplexMatrix& m2);

LIB_EXTERN ComplexMatrix             subtract(ComplexMatrix& x, ComplexMatrix& y);
LIB_EXTERN ComplexMatrix               mult(ComplexMatrix& m1, ComplexMatrix& m2);
LIB_EXTERN bool                     sameDimensions(ComplexMatrix& x, ComplexMatrix& y);


LIB_EXTERN std::ostream&                 operator<<(std::ostream& stream, const IntMatrix& mat);
LIB_EXTERN std::ostream&                 operator<<(std::ostream& stream, const DoubleMatrix& mat);
LIB_EXTERN std::ostream&                 operator<<(std::ostream& stream, const ComplexMatrix& mat);

template<class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols) :
_Rows(rows),
_Cols(cols),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new T[_Rows * _Cols]; //Todo: memoryleak
        memset(_Array, 0, (sizeof(T)*_Rows*_Cols));
    }
}

//We can have various init scenarios
template<class T>
inline Matrix<T>::Matrix(const Matrix<double> & src)
:
_Rows(src.RSize()),
_Cols(src.CSize()),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new T[_Rows * _Cols]; //Todo: memoryleak
        //memcpy(_Array, src._Array, _Rows * _Cols * sizeof(double));
    }
}

template<>
inline Matrix<double>::Matrix(const Matrix<double> & src)
:
_Rows(src._Rows),
_Cols(src._Cols),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new double[_Rows * _Cols];
        memcpy(_Array, src._Array, _Rows * _Cols * sizeof(double));
    }

    rowNames = src.rowNames;
    colNames = src.colNames;
}

template<>
inline Matrix<double>::Matrix(const Matrix<Complex> & src, bool copyReal) :
_Rows(src.RSize()),
_Cols(src.CSize()),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new double[_Rows * _Cols];
    }

    Matrix<double>& refMat = *this;
    for(unsigned int r = 0; r < _Rows; r++)
    {
        for(unsigned int c = 0; c < _Cols; c++)
        {
            if(copyReal)
            {
                refMat(r,c) = std::real(src(r,c));
            }
            else
            {
                refMat(r,c) = std::imag(src(r,c));
            }
        }
    }
}

template<class T>
inline Matrix<T>::Matrix(const Matrix<Complex> & src, bool copyReal) :
_Rows(src.RSize()),
_Cols(src.CSize()),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new T[_Rows * _Cols];
    }

    Matrix<T>& refMat = *this;
    for(unsigned int r = 0; r < _Rows; r++)
    {
        for(unsigned int c = 0; c < _Cols; c++)
        {
            //refMat(r,c) = src(r,c);
        }
    }
}

template<>
inline Matrix<Complex>::Matrix(const Matrix<Complex> & src, bool copyReal) :
_Rows(src.RSize()),
_Cols(src.CSize()),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new Complex[_Rows * _Cols];
    }

    Matrix<Complex>& refMat = *this;
    for(unsigned int r = 0; r < _Rows; r++)
    {
        for(unsigned int c = 0; c < _Cols; c++)
        {
            refMat(r,c) = src(r,c);
        }
    }
}

template<>
inline Matrix< Complex >::Matrix(const Matrix<double> & src):
_Rows(src.RSize()),
_Cols(src.CSize()),
_Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new Complex[_Rows * _Cols];
        Matrix< Complex >& refMat = *this;
        for(unsigned int r = 0; r < _Rows; r++)
        {
            for(unsigned int c = 0; c < _Cols; c++)
            {
                refMat(r,c) = src(r,c);
            }
        }
    }
}

template<class T>
inline Matrix<T>::Matrix( T* &oRawData, int nRows, int nCols, bool transpose) :
    _Rows(nRows),
    _Cols(nCols),
    _Array(NULL)
{
    if (_Rows && _Cols)
    {
        _Array = new T[_Rows * _Cols];
        if (!transpose)
        {
            memcpy(_Array, oRawData, sizeof(T)*nRows*nCols);
        }
        else
        {
            for (unsigned int i = 0; i < _Rows; i++)
            {
                for (unsigned int j = 0; j < _Cols; j++)
                {
                    (*this)(i,j) = oRawData[i+_Rows*j];
                }
          }
        }
    }
}

template<class T>
inline Matrix<T>::~Matrix()
{
  if (_Array)
  {
      delete [] _Array;
  }
}

template<class T>
inline unsigned int Matrix<T>::Length() const
{
    return _Rows*_Cols;
}

template<class T>
inline bool Matrix<T>::isAllocated() const
{
    return (_Array != NULL) ? true : false;
}

template<class T>
T* Matrix<T>::GetPointer()
{
    return _Array;
}

template<class T>
unsigned int Matrix<T>::CSize() const
{
  return _Cols;
}

template<class T>
unsigned int Matrix<T>::RSize() const
{
  return _Rows;
}

// Matrix <T> & operator + (const Matrix <T> & rhs)
//{
// unsigned int i, imax = _Rows * _Cols;
// T * tmp1 = _Array;
// T * tmp2 = rhs._Array;

// for (i = 0; i < imax; i++, tmp1++, tmp2++) *tmp1 += *tmp2;

// return *this;
//}


template<class T>
Matrix<T>::Matrix( T** &oRawData, int nRows, int nCols) : _Array(NULL), _Rows(0), _Cols(0)
{
  initializeFrom2DMatrix(oRawData, nRows, nCols);
}

template<class T>
Matrix<T>::Matrix(const T** oRawData, int nRows, int nCols) : _Array(NULL), _Rows(0), _Cols(0)
{
  initializeFromConst2DMatrix(oRawData, nRows, nCols);
}

template<class T>
T* Matrix<T>::getArray() { return _Array; };

template<class T>
T* Matrix<T>::getCopy(bool transpose)
{
    T* result = new T[_Rows * _Cols];
    if (_Rows * _Cols == 0)
    {
        return result;
    }

    if (!transpose)
    {
        memcpy(result, _Array, sizeof(T)*_Rows*_Cols);
    }
    else
    {
        for (unsigned int i = 0; i < _Rows; i++)
        {
            for (unsigned int j = 0; j < _Cols; j++)
            {
                result[i+_Rows*j] = (*this)(i,j) ;
            }
        }
    }
    return result;
}

//template<class T>
//void Matrix<T>::initializeFrom2DMatrix(T** &oRawData, int nRows, int nCols);
//
//template<class T>
//void Matrix<T>::initializeFromConst2DMatrix( const T** oRawData, int nRows, int nCols);

//template<class T>
//T** Matrix<T>::get2DMatrix(int &nRows, int &nCols);

template<class T>
void Matrix<T>::swapRows(unsigned int row1, unsigned int row2)
{
  for (unsigned int i = 0; i < _Cols; i++)
  {
      T tmp = (*this)(row1,i);
      (*this)(row1,i)=(*this)(row2,i);
      (*this)(row2,i)=tmp;
  }
}

template<class T>
void Matrix<T>::swapCols(unsigned int col1, unsigned int col2)
{
    for (unsigned int i = 0; i < _Rows; i++)
    {
        T tmp = (*this)(i,col1);
        (*this)(i,col1)=(*this)(i,col2);
        (*this)(i,col2)=tmp;
    }
}

template<class T>
void Matrix<T>::resize(unsigned int rows, unsigned int cols)
{
    if (rows * cols != _Rows * _Cols)
    {
        if (_Array)
        {
            delete [] _Array;
            _Array = NULL;
        }
        if (rows && cols)
        {
            _Array = new T[rows * cols];
        }
    }

    _Rows = rows;
    _Cols = cols;
}

template<class T>
bool Matrix<T>::Allocate(unsigned int rows, unsigned int cols)
{
    resize(rows,cols);
    return _Array ? true : false;
}

template<class T>
Matrix<T>* Matrix<T>::getTranspose()
{
  Matrix <T> *oResult = new Matrix <T>(_Cols, _Rows);  //Todo: memoryleak
  for (unsigned int i = 0; i < _Cols; i++)
  {
      for (unsigned int j = 0; j <_Rows; j++)
      {
          (*oResult)(i,j) = (*this)(j,i);
      }
  }
  return oResult;
}

template<class T>
Matrix<T>& Matrix<T>::operator = (const Matrix <T>& rhs)
{
    if (this == &rhs)      // Same object?
    {
      return *this;
    }

    if (_Rows != rhs._Rows || _Cols != rhs._Cols)
    {
      resize(rhs._Rows, rhs._Cols);
    }

    rowNames = rhs.rowNames;
    colNames = rhs.colNames;

  memcpy(_Array, rhs._Array, _Rows * _Cols * sizeof(T));
  return *this;
}

//template<class T>
//Matrix<T>& Matrix<T>::operator = (const Matrix<double>& rhs)
//{
////              if (_Rows != rhs._Rows || _Cols != rhs._Cols)
////              {
////                  resize(rhs._Rows, rhs._Cols);
////              }
//
//  memcpy(_Array, rhs._Array, _Rows * _Cols * sizeof(T));
//  return *this;
//}

template<class T>
Matrix <T> & Matrix<T>::operator = (const T & value)
{
    unsigned int i, imax = _Rows * _Cols;
    T * tmp = _Array;

    for (i = 0; i < imax; i++, tmp++) *tmp = value;

    return *this;
}

template<class T>
unsigned int Matrix<T>::size() const
{
    return _Rows * _Cols;
}

template<class T>
unsigned int Matrix<T>::numRows() const
{
    return _Rows;
}

template<class T>
unsigned int Matrix<T>::numCols() const
{
    return _Cols;
}

template<class T>
T* Matrix<T>::operator[](unsigned int row)
{
    return _Array + row * _Cols;
}

template<class T>
const T* Matrix<T>::operator[](unsigned int row) const
{
    return _Array + row * _Cols;
}

template<class T>
T& Matrix<T>::operator()(const unsigned int & row, const unsigned int & col)
{
    return *(_Array + row * _Cols + col);
}

template<class T>
const T& Matrix<T>::operator()(const unsigned int & row, const unsigned int & col) const
{
    return *(_Array + row * _Cols + col);
}

template<class T>
const T& Matrix<T>::Element(const unsigned int & row, const unsigned int & col) const
{
    return *(_Array + row * _Cols + col);
}

template<class T>
const Matrix<T> operator*(Matrix<T>& lhs, const double& rhs)
{
    Matrix<T> result(lhs.RSize(), lhs.CSize());
    for(unsigned int i = 0; i < lhs.RSize(); i++)
    {
        for(unsigned int j = 0; j < lhs.CSize(); j++)
        {
            result(i,j) = lhs(i,j) * rhs;
        }
    }
    return result;
}


template<class T>
Matrix<T>    multDiag(Matrix< T >& mat, T& val)
{
    Matrix<T> result(mat.RSize(), mat.CSize());
    for (int i = 0; i < mat.RSize(); i++)
    {
        result(i, i) = mat(i, i) *  val;
    }
    return result;
}

}

#endif

