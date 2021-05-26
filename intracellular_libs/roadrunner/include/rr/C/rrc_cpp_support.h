/**
 * @file rrc_cpp_support.h
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

#ifndef rrc_cpp_supportH
#define rrc_cpp_supportH

#include <vector>
#include "rr-libstruct/lsMatrix.h"
#include "rrRoadRunnerData.h"
#include "rrcStringList.h"
#include "rrArrayList.h"
#include "rrc_types.h"
#include "rrc_exporter.h"

namespace rr
{
class RoadRunner;
}

//When using the rrc_core_api from C++, the following routines are useful
namespace rrc
{
using std::string;
using std::vector;

/*!
 \brief Set API error

 \param[in] error A string containg the error
 \ingroup cpp_support
*/
C_DECL_SPEC void                        setError(const string& err);

/*!
 \brief Cast a handle to RoadRunner instance pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner instance
 \return Pointer to a roadrunner instance
 \ingroup cpp_support
*/
C_DECL_SPEC rr::RoadRunner*             castToRoadRunner(RRHandle rrHandle);


/*!
 \brief Copy a C vector to a std::vector
 \param[in] source Pointer to a RRVector structure instance
 \param[in] dest  Destination std::vector
 \return True or false, indicating the result
 \ingroup cpp_support
*/
C_DECL_SPEC bool                                copyVector(const RRVector* source, vector<double>& dest);

/*!
 \brief Creates a C vector from a std::vector
 \param[in] vec Input double vector
 \return A handle to a RRVector. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRVectorPtr                       createVector(const vector<double>& vec);

/*!
 \brief Creates a C Complex vector from a std::vector<ls::Complex>
 \param[in] vec Input Complex vector
 \return A handle to a RRComplexVector. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRComplexVectorPtr             createVector(const vector<ls::Complex>& vec);

/*!
 \brief Creates a std vector from a C vector
 \param[in] vec Input C vector
 \return A std vector object
 \ingroup cpp_support
*/
C_DECL_SPEC vector<double>                      createVector(const RRVector* vec);

/*!
 \brief Creates a C matrix  from a ls::DoubleMatrix, supplied as a pointer
 \param[in] mat  Input DoubleMatrix
 \return A handle to a RRMatrix. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRDoubleMatrixPtr              createMatrix(const ls::DoubleMatrix* mat);

/*!
 \brief Creates a ls::DoubleMatrix  from a RRMatrix, supplied as a pointer
 \param[in] mat  Input RRMatrix pointer
 \return A handle to a DoubleMatrix. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC ls::DoubleMatrix*                   createMatrix(const RRDoubleMatrixPtr mat);


/*!
 \brief Creates a C complex matrix from a ls::ComplexMatrix, supplied as a pointer
 \param[in] mat  Input ls::ComplexMatrix
 \return A handle to a RRComplexMatrix. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRComplexMatrixPtr             createMatrix(const ls::ComplexMatrix* mat);

//Lists and arrays
/*!
 \brief Creates a C StringArray from a rr::StringList
 \param[in] list  Input StringList
 \return A handle to a RRStringArray. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRStringArrayPtr               createList(const rrc::StringList& list);

/*!
 \brief Creates a heterogenoeus RRList from a rr::ArrayList
 \param[in] list  Input Array list
 \return A handle to a RRList. Null if it fails
 \ingroup cpp_support
*/
C_DECL_SPEC RRListPtr                      createArrayList(const ArrayList& list);


/**
 * Creates an RRCDataPtr from the current state of a RoadRunner object.
 *
 * This may be used directly after RoadRunner::simulate() has been called
 * to package up the simulation result data and column names into one of
 * these RRCDataPtr structs.
 */
C_DECL_SPEC RRCDataPtr    createRRCData(const rr::RoadRunner&);

}
#endif


