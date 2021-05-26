/**
 * @file rrc_types.h
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib/ for more details.
 *
 * Copyright (C) 2012
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

#ifndef rrc_typesH
#define rrc_typesH

#if defined(__cplusplus)
namespace rrc { extern "C" {
#endif

/*!@brief Void pointer to a RoadRunner instance */
typedef void* RRHandle; /*! Void pointer to a RoadRunner instance */


// ===================================== C TYPES =====================================

/*!@struct*/
/*!@brief Structure for a simple vector of doubles */
typedef struct RRVector
{
    int             Count;                  /*!< The number of elements in the vector */
    double*         Data;                   /*!< Points to an array of double items */
} *RRVectorPtr;                             /*!< Pointer to RRVectorHandle struct */

/*!@struct*/
/*!@brief Structure for a simple vector of strings */
typedef struct RRStringArray
{
    int             Count;                    /*!< The number of elements in the string array */
    char**          String;                    /*!< Points to an array of string items */
} *RRStringArrayPtr;                        /*!< Pointer to RRStringListHandle struct */

/*!@struct*/
/*!@brief Structure for a simple double Matrix type */
typedef struct RRDoubleMatrix
{
    int             RSize;                  /*!< The number of rows in the matrix */
    int             CSize;                  /*!< The number of columns in the matrix */
    double*         Data;                   /*!< Items in the matrix stored as a linear array. Access an element using Data[row*CSize + col],
                                                            where i,j represent the row and column numberof the element. Indexing is from zero */
} *RRDoubleMatrixPtr;                       /*!< Pointer to RRDoubleMatrixPtr struct */

/*!@struct*/
/*!@brief Structure for a complex number */
typedef struct RRComplex
{
    double          re;                     /*!< Real part of complex number */
    double          imag;                   /*!< imag part of complex number */
} *RRComplexPtr;                            /*!< Pointer to a RRComplex number */

/*!@struct*/
/*!@brief Structure for a simple complex Vector type */
typedef struct RRComplexVector
{
    int                 Count;              /*!< The number of elements in the vector */
    RRComplexPtr        Data;               /*!< Access an element using Data[i], where
                                                i represents the index of the element. Indexing is from zero */
} *RRComplexVectorPtr;                      /*!< Pointer to RRVectorHandle struct */

/*!@struct*/
/*!@brief Structure for a simple complex Matrix type */
typedef struct RRComplexMatrix
{
    int                 RSize;              /*!< The number of rows in the matrix */
    int                 CSize;              /*!< The number of columns in the matrix */
    RRComplexPtr        Data;               /*!< Items in the matrix stored as a linear array. Access an element using Data[row*CSize + col],
                                                  where i,j represent the row and column numberof the element. Indexing is from zero */
} *RRComplexMatrixPtr;                      /*!< Pointer to RRDoubleMatrixPtr struct */

/*!@struct*/
/*!@brief Structure for the result type from the simulate calls. The client is responsible for freeing the RRCDataPtr. */
typedef struct RRCData
{
    int             RSize;                  /*!< The number of rows in the data matrix */
    int             CSize;                  /*!< The number of columns in the data matrix */
    double*         Data;                   /*!< A pointer to the data stored in the matrix. Access an element using Data[row*CSize + col] */
    double*         Weights;                /*!< A pointer to weights stored in the Weights matrix. Access an element using Weights[row*CSize + col] */
    char**          ColumnHeaders;          /*!< Pointer to an array of column header strings */
} *RRCDataPtr;                              /*!< Pointer to RRCDataPtr struct */

/*!@enum*/
/*!@brief The list type supports strings, integers, double and lists */
enum ListItemType {litString, litInteger, litDouble, litList};

/*!@enum*/
/*!@brief A parameters type can be string, bool, integer, double, vector or a matrix */
enum RRParameterType {ptString, ptBool, ptInteger, ptDouble, ptVector, ptMatrix};

// The above enums correspond to the currently supported types in a RRArrayList
struct RRList;    //Forward declaration for RRListItem, needed for RRListItem

/*!@struct*/
/*!@brief A single list element type */
typedef struct RRListItem
{
    enum ListItemType ItemType;             /*!< The type of the item in this list element */
    union
    {
       int              iValue;             /*!< Integer value */
       double           dValue;             /*!< Double value */
       char*            sValue;             /*!< String value */
       struct RRList*   lValue;             /*!< List value */
    } data;                                 /*!< Union */
} *RRListItemPtr;                           /*!< Pointer to cRRArrayListItemHandle struct */


/*!@struct*/
/*!@brief A list type, stores int, double, strings and lists */
typedef struct RRList
{
    int                 Count;              /*!< The number elements in this list */
    RRListItemPtr      *Items;              /*!< A pointer to a list of items */
}  *RRListPtr;                              /*!< Pointer to cRRArrayListHandle struct */

#if defined( __cplusplus)
}
}// rcc namespace
#endif


#endif
