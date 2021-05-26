/*
 * rrCSparse.h
 *
 *  Created on: Jul 10, 2013
 *      Author: andy
 */

#ifndef RRCSPARSE_H_
#define RRCSPARSE_H_

#include "rrOSSpecifics.h"
#include <vector>
#include <ostream>

namespace rr
{

/**
 * @internal
 * sparse storage compressed row format matrix.
 *
 * This should eventually get replaced when we use a numeric library
 * which support sparse storage.
 *
 * structure layout based on  Mark Hoemmen's BeBop sparse conversion lib.
 */
typedef struct csr_matrix_t
{
    /**
     * number of rows
     */
    unsigned m;

    /**
     * number of columns
     */
    unsigned n;

    /**
     * number of stored (nonzero) entries.
     */
    unsigned nnz;

    /**
     * array of stored (nonzero) entries of the matrix
     * length: nnz
     *
     */
    double* values;

    /**
     * array of column indices of the stored (nonzero) entries of the matrix,
     * length: nnz
     */
    unsigned* colidx;

    /**
     * array of indices into the colidx and values arrays, for each column,
     * length: m + 1
     *
     * This CSR matrix has the property that even rows with zero non-zero
     * values have an entry in this array, if the i'th row has zero no values,
     * then rowptr[j] == rowptr[j+1]. This property makes it easy to set
     * values.
     */
    unsigned* rowptr;

} csr_matrix;


/**
 * allocate and initialize the buffers and fields of a dcsr_matrix
 * struct.
 *
 * TODO: These belong someplace more appropriate that this header file.
 *
 * These funcs are used by the generated LLVM model.
 */
csr_matrix* csr_matrix_new(unsigned m, unsigned n,
        const std::vector<unsigned>& rowidx, const std::vector<unsigned>& colidx,
        const std::vector<double>& values);

/**
 * free the buffers belonging to a dcsr_matrix, and the
 * matrix itself.
 */
void csr_matrix_delete(csr_matrix *mat);

/**
 * sets a (previously allocted) non-zero value to the given value.
 *
 * @returns true on success, false on failure.
 */
bool csr_matrix_set_nz(csr_matrix *mat, unsigned row, unsigned col, double val);

/**
 * sets a (previously allocted) non-zero value to the given value.
 *
 * returns NaN if the entry (i,j) has not been alocated.
 */
double csr_matrix_get_nz(const csr_matrix *mat, unsigned row, unsigned col);

/**
 * performs the matrix-vector operations y := alpha*A*x + beta*y
 *
 * LAPACK sig: SUBROUTINE DGEMV(TRANS,M,N,ALPHA,A,LDA,X,INCX,BETA,Y,INCY)
 *
 * The given vectors y and x must be the same size as number of
 * columns in the sparse matrix.
 */
void  csr_matrix_dgemv(double alpha, const csr_matrix *A,
        double const *x, double beta, double *y);

/**
 * perform a dot product between the a row in the matrix and a vector y.
 *
 * The given vector y must obviously be the same length as the
 * number of rows in matrix x.
 */
double csr_matrix_ddot(int row, const csr_matrix *x, const double *y);

/**
 * fill dense matrix
 */
void csr_matrix_fill_dense(const csr_matrix *x, double *dense);

/**
 * dump the matrix to an output stream.
 */
std::ostream& operator<< (std::ostream& os, const csr_matrix* mat);


}

#endif /* RRCSPARSE_H_ */
