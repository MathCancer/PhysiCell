/**
 * @file rrc_libstruct_api.h
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/
#ifndef rrc_libstruct_apiH
#define rrc_libstruct_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
//---------------------------------------------------------------------------
#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

/*! \brief Returns the L0 Matrix.

L0 is defined such that  L0 Nr = N0. L0 forms part of the link matrix, L.  N0 is the set of
linear dependent rows from the lower portion of the reordered stoichiometry matrix.

\param[in] handle Handle to a RoadRunner instance
\return Returns null if it fails, otherwise returns the L0 matrix.

\remarks To free the returned matrix call freeMatrix with the matrix
as parameter.
\ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getL0Matrix(RRHandle handle);

/*! \brief Calculates the eigen-vectors of a square real matrix.
This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix
returned contains the eigenvectors in the columns, in the same order as LibLA_getEigenValues.

The right eigenvector v(j) of A satisfies:
\par
A * v(j) = lambda(j) * v(j)
\param[in] matrix Handle to a RRMatrix
\return Returns null if it fails, otherwise returns a RRComplexMatrix.
\ingroup Stoich
*/
C_DECL_SPEC RRComplexMatrixPtr rrcCallConv getEigenVectors(RRDoubleMatrixPtr matrix);


/*! \brief Calculates the eigen-vectors of a square nonsymmetrix complex matrix.
This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix
returned contains the eigenvectors in the columns, in the same order as getZEigenValues.
The right eigenvector v(j) of A satisfies:
\par
A * v(j) = lambda(j) * v(j)
\param[in] matrix Handle to a RRComplexMatrix
\return Returns null if it fails, otherwise returns a RRComplexMatrix.
\ingroup Stoich
*/
C_DECL_SPEC RRComplexMatrixPtr rrcCallConv getZEigenVectors(RRComplexMatrixPtr matrix);


/*! \brief Returns values for conservation laws using the current initial conditions
\remarks free vector using freeVector function

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has
been called yet.
\param[in] handle Handle to a RoadRunner instance
\return Returns null if it fails, otherwise returns a RRVectorHandle.
\ingroup Stoich

*/
// int LibStructural_getConservedSums(double* *outArray, int *outLength);
RRVectorPtr getConservedSums(RRHandle handle);


// --------------------------------------------------------------------------------
// General purpose linear algebra methods
// --------------------------------------------------------------------------------

/*!
 \brief Compute the eigenvalues of a double matrix
 \param[in] mat Pointer to a to double matrix 
 \return Returns NULL if it fails, otherwise returns a matrix of eigenvalues.
 The first column will contain the real values and the second column the imaginary values
 \ingroup LinearAlgebra
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getEigenvaluesMatrix(const RRDoubleMatrixPtr mat);

/*!
 \brief Compute the eigenvalues of a double matrix

 \param[in] mat Handle to input matrix
 \return Returns null if it fails, otherwise returns a complex vector of eigenvalues.
 \ingroup LinearAlgebra
*/
C_DECL_SPEC RRComplexVectorPtr rrcCallConv getEigenvaluesVector(const RRDoubleMatrixPtr mat);

//---------------------------------------------------------------------------
#if defined(__cplusplus)
}
}
#endif //rrc namespace and extern "C" scopes


#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif //Header guard

