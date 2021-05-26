/**
 * @file rrc_utilities.h
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

#ifndef rrc_utilitiesH
#define rrc_utilitiesH
#include "rrc_exporter.h"
#include "rrc_types.h"
#include "rrc_exporter.h"
#include "rrc_macros.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

/*!
 \brief Global parameter holding last error, if any
*/
extern char* gLastError;

/*!
 \brief Global parameter holding C API install folder
*/
extern char* gInstallFolder;

//Error/Warning Messages
/*!
 \brief Global parameter holding message for an un-allocated API
*/
extern const char*  ALLOCATE_API_ERROR_MSG;

/*!
 \brief Global parameter holding message when an invalid is handed to a function
*/
extern const char*  INVALID_HANDLE_ERROR_MSG;

/*!
 \brief Retrieves the the content of a file.
 \param[in] fName Pointer to a string holding the name and optionla path to a file
 \return Content of file as a string, returns null if it fails
 \ingroup utilities
*/
C_DECL_SPEC char*        rrcCallConv    getFileContent(const char* fName);

/*!
 \brief Creates memory for holding a string
 \param[in] text Pointer to a string
 \return Pointer to a memory holding a copy of the input string
 \ingroup utilities
*/
C_DECL_SPEC char*        rrcCallConv    createText(const char* text);

/*!
 \brief Creates memory for holding a string
 \param[in] count integer indicating number of elements to allocate
 \return Pointer to string memory
 \ingroup utilities
*/
C_DECL_SPEC char*        rrcCallConv    createTextMemory(const int count);

// --------------------------------------------------------------------
// List support routines
// --------------------------------------------------------------------

/*!
 \brief Create a new list

 A list is a container for storing list items. List items can represent integers, double, strings and lists.
 To populate a list, create list items of the appropriate type and add them to the list

 Example, build the list [123, [3.1415926]]

 \code
 l = createRRList(RRHandle handle);
 item = createIntegerItem (123);
 addItem (l, item);
 item1 = createListItem(RRHandle handle);
 item2 = createDoubleItem (3.1415926);
 addItem (item1, item2);
 addItem (l, item1);

 item = getListItem (l, 0);
 printf ("item = %d\n", item->data.iValue);

 printf (listToString (l));
 freeRRList (l);
 \endcode

 \return Returns null if fails, otherwise returns a pointer to a new list structure
 \ingroup list
*/
C_DECL_SPEC RRListPtr rrcCallConv createRRList(void);

/*!
 \brief Free RRListPtr structure, i.e destroy a list
 \ingroup list
*/
C_DECL_SPEC void rrcCallConv freeRRList (RRListPtr list);

/*!
 \brief Returns the length of a given list

 \param[in] myList The list to retrieve the length from
 \return Length of list
\ingroup list
*/
C_DECL_SPEC int rrcCallConv getListLength (RRListPtr myList);


/*!
 \brief Create a list item to store an integer

 \param[in] value The integer to store in the list item
 \return A pointer to the list item
 \ingroup list
*/
C_DECL_SPEC RRListItemPtr rrcCallConv createIntegerItem (int value);

/*!
 \brief Create a list item to store a double value

 \param[in] value The double to store in the list item
 \return A pointer to the list item
 \ingroup list
*/
C_DECL_SPEC RRListItemPtr rrcCallConv createDoubleItem  (double value);

/*!
 \brief Create a list item to store a pointer to a char*

 \param[in] value The string to store in the list item
 \return A pointer to the list item
 \ingroup list
*/
C_DECL_SPEC RRListItemPtr rrcCallConv createStringItem  (char* value);

/*!
 \brief Create a list item to store a list

 \param[in] value The list to store in the list item
 \return A pointer to the list item
 \ingroup list
*/
C_DECL_SPEC RRListItemPtr rrcCallConv createListItem (struct RRList* value);

/*!
 \brief Add a list item to a list and return index to the added item

 \code
 x = createRRList(RRHandle handle);
 item1 = createIntegerItem (4);
 add (x, item1);
 \endcode

 \param[in] list The list to store the item in
 \param[in] item The list item to store in the list
 \return The index to where the list item was added
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv addItem (RRListPtr list, RRListItemPtr *item);


/*!
 \brief Returns the index^th item from the list

 \param[in] list The list to retrieve the list item from
 \param[in] index The index list item we are interested in

 \return A pointer to the retrieved list item
 \ingroup list
*/
C_DECL_SPEC RRListItemPtr rrcCallConv getListItem (RRListPtr list, int index);

/*!
 \brief Returns true or false if the list item is an integer

 \param[in] item The list
 \return If true, then the list item holds an integer
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv isListItemInteger (RRListItemPtr item);

/*!
 \brief Returns true or false if the list item is a double

 \param[in] item The list
 \return If true, then the list item holds a double
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv isListItemDouble (RRListItemPtr item);

/*!
 \brief Returns true or false if the list item is a character array

 \param[in] item The list
 \return If true, then the list item holds an character array
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv isListItemString (RRListItemPtr item);

/*!
 \brief Returns true or false if the list item is a list itself

 \param[in] item The list
 \return If true, then the list item holds a list
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv isListItemList (RRListItemPtr item);

/*!
 \brief Returns true or false if the list item is the given itemType

 \param[in] item The list
 \param[in] itemType The list item type to check for
 \return If true, then the list item holds a list
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv isListItem (RRListItemPtr item, enum ListItemType itemType);

/*!
 \brief Returns the integer from a list item

 \param[in] item The list item to work with
 \param[out] value The integer value returned by the method
 \return Returns true is successful, else false
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv getIntegerListItem (RRListItemPtr item, int *value);

/*!
 \brief Returns the double from a list item

 \param[in] item The list item to work with
 \param[out] value The double value returned by the method
 \return Returns true is successful, else false
 \ingroup list
*/
C_DECL_SPEC int rrcCallConv getDoubleListItem (RRListItemPtr item, double *value);

/*!
 \brief Returns the string from a list item

 \param[in] item The list item to work with
 \return Returns NULL if it fails, otherwise returns a pointer to the string
 \ingroup list
*/
C_DECL_SPEC char* rrcCallConv getStringListItem (RRListItemPtr item);


/*!
 \brief Returns a list from a list item if it contains a list

 \param[in] item The list item to retrieve the list type from
 \return Returns NULL if item isn't a list, otherwise it returns a list from the item
\ingroup list
*/
C_DECL_SPEC RRListPtr rrcCallConv getList(RRListItemPtr item);

// --------------------------------------------------------------------------------
// Free memory functions
// --------------------------------------------------------------------------------

/*!
 \brief Free the memory associated to a RRCData object
 \ingroup freeRoutines
*/
C_DECL_SPEC int rrcCallConv freeRRCData(RRCDataPtr handle);

/*!
 \brief Free char* generated by library routines
 \ingroup freeRoutines
*/
C_DECL_SPEC int rrcCallConv freeText(char* text);


/*!
 \brief Free RRStringListHandle structures
 \ingroup freeRoutines
*/
//C_DECL_SPEC int rrcCallConv freeStringList(RRStringListHandle sl);

/*!
 \brief Free RRStringArrayPtr structures
 \ingroup freeRoutines
*/
C_DECL_SPEC int rrcCallConv freeStringArray(RRStringArrayPtr sl);

/*!
 \brief Free RRVectorHandle structures
 \ingroup freeRoutines
*/
C_DECL_SPEC int rrcCallConv freeVector(RRVectorPtr vector);

/*!
 \brief Free RRDoubleMatrixPtr structures
 \ingroup freeRoutines
*/
C_DECL_SPEC int rrcCallConv freeMatrix(RRDoubleMatrixPtr matrix);

// --------------------------------------------------------------------------------
// Helper Methods
// --------------------------------------------------------------------------------

/*!
 \brief Get the number of elements in a vector type

 Vectors are indexed from zero

 Example: \code count = getVectorLength (myVector); \endcode

 \param vector A pointer to the vector variable type
 \return Returns -1 if it fails, otherwise returns the number of elements in the vector
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getVectorLength (RRVectorPtr vector);

/*!
 \brief Create a new vector with a given size

 Vectors are indexed from zero

 Example: \code myVector = createVector (10); \endcode

 \param size The number of element in the new vector
 \return Returns null if it fails, otherwise returns a pointer to the new vector
 \ingroup helperRoutines
*/
C_DECL_SPEC RRVectorPtr rrcCallConv createVector (int size);

/*!
 \brief Get a particular element from a vector

 Vectors are indexed from zero

 Example: \code status = getVectorElement (myVector, 10, *value); \endcode

 \param vector A pointer to the vector variable type
 \param index An integer indicating the ith element to retrieve (indexing is from zero)
 \param value A pointer to the retrieved double value
 \return Returns true if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getVectorElement (RRVectorPtr vector, int index, double* value);


/*!
 \brief Set a particular element in a vector

 Vectors are indexed from zero

 Example: \code status = setVectorElement (myVector, 10, 3.1415); \endcode

 \param vector A pointer to the vector variable type
 \param index An integer indicating the ith element to set (indexing is from zero)
 \param value The value to store in the vector at the indexth position
 \return Returns true if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv setVectorElement (RRVectorPtr vector, int index, double value);

/*!
 \brief Create an empty matrix of size r by c

 Matrices are indexed from zero

 Example: \code m = createRRMatrix (2, 3); \endcode

 \param r,c Row and column sizes
 \return Returns NULL if fails, otherwise returns a handle to the matrix
 \ingroup helperRoutines
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv createRRMatrix (int r, int c);

/*!
 \brief Retrieve the number of rows in the given matrix

 Matrices are indexed from zero

 Example: \code nRows = getMatrixNumRows (m); \endcode

 \param m A pointer to a matrix type variable
 \return Returns -1 if fails, otherwise returns the number of rows
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getMatrixNumRows (RRDoubleMatrixPtr m);

/*!
 \brief Retrieve the number of columns in the given matrix

 Matrices are indexed from zero

 Example: \code nRows = getMatrixNumCols (m); \endcode

 \param m A pointer to a matrix type variable
 \return Returns -1 if fails, otherwise returns the number of columns
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getMatrixNumCols (RRDoubleMatrixPtr m);

/*!
 \brief Retrieve an element at a given row and column from a matrix type variable

 Matrices are indexed from zero

 Example:
 \code
 status = getMatrixElement (m, 2, 4, &value);
 \endcode

 \param[in] m A pointer to a matrix type variable
 \param[in] r The row index to the matrix
 \param[in] c The column index to the matrix
 \param[out] value The retrieved value from the matrix
 \return Returns True if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getMatrixElement (RRDoubleMatrixPtr m, int r, int c, double* value);

/*!
 \brief Set an element at a given row and column with a given value in a matrix type variable

 Matrices are indexed from zero

 Example:
 \code
 status = setMatrixElement (m, 2, 4, value);
 \endcode

 \param[in] m A pointer to a matrix type variable
 \param[in] r The row index to the matrix
 \param[in] c The column index to the matrix
 \param[out] value The value to set to the matrix element
 \return Returns True if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv setMatrixElement (RRDoubleMatrixPtr m, int r, int c, double value);


/*!
 \brief Retrieve an element at a given row and column from a complex matrix type variable

 Matrices are indexed from zero

 Example:
 \code
 status = getComplexMatrixElement (m, 2, 4, &value);
 \endcode

 \param[in] m A pointer to a complex matrix type variable
 \param[in] r The row index to the matrix
 \param[in] c The column index to the matrix
 \param[out] value The retrieved value from the matrix
 \return Returns True if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getComplexMatrixElement (RRComplexMatrixPtr m, int r, int c, RRComplexPtr value);


/*!
 \brief Set an element at a given row and column with a given value in a complex matrix type variable

 Matrices are indexed from zero

 Example:
 \code
 status = setComplexMatrixElement (m, 2, 4, value);
 \endcode

 \param[in] m A pointer to a complex matrix type variable
 \param[in] r The row index to the matrix
 \param[in] c The column index to the matrix
 \param[out] value The value to set to the complex matrix element
 \return Returns True if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv setComplexMatrixElement (RRComplexMatrixPtr m, int r, int c, RRComplexPtr value);


/*!
 \brief Retrieve the number of rows in the given RoadRunner numerical data (returned from simulate(RRHandle handle))

 Example: \code nRows = getRRDataNumRows (result); \endcode

 \param[in] rrData A pointer to a RoadRunner numerical data type variable
 \return Returns -1 if fails, otherwise returns the number of rows
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getRRDataNumRows (RRCDataPtr rrData);

/*!
 \brief Retrieve the number of columns in the given rrData data (returned form simulate(RRHandle handle))

 Example: \code nRows = getResultNumCols (rrData); \endcode

 \param[in] rrData A pointer to a rrData type variable
 \return Returns -1 if fails, otherwise returns the number of columns
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getRRDataNumCols (RRCDataPtr rrData);

/*!
 \brief Retrieves an element at a given row and column from a RoadRunner data type variable

 RoadRunner numerical data are indexed from zero

 Example: \code status = getRRCDataElement (rrData, 2, 4, *value); \endcode

 \param[in] rrData A pointer to a rrData type variable
 \param[in] r -The row index to the rrData data
 \param[in] c - The column index to the rrData data
 \param[out] value - The retrieved value from the rrData data
 \return Returns true if successful
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv getRRCDataElement (RRCDataPtr rrData, int r, int c, double *value);

/*!
 \brief Retrieves a label for a given column in a rrData type variable

 Result data are indexed from zero

 Example: \code str = getResultColumnLabel (rrData, 2, 4); \endcode

 \param[in] rrData A pointer to a rrData type variable
 \param[in] column - The column index for the rrData data (indexing from zero)
 \return Returns null if fails, otherwise returns a pointer to the string column label
 \ingroup helperRoutines
*/
C_DECL_SPEC char* rrcCallConv getRRDataColumnLabel (RRCDataPtr rrData, int column);

// --------------------------------------------------------------------------------
// Convert data to string functions
// --------------------------------------------------------------------------------

/*!
 \brief Returns a vector in string form.
 \return Returns vector as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv vectorToString(const RRVectorPtr vec);

/*!
 \brief Returns a complex vector in string form.
 \return Returns complex vector as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv complexVectorToString(const RRComplexVectorPtr vec);

/*!
 \brief Returns a rrCData struct in string form.
 \return Returns rrCData struct as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv rrCDataToString(const RRCDataPtr rrData);

/*!
 \brief Returns a matrix in string form.
 \return Returns matrix as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv matrixToString(const RRDoubleMatrixPtr mat);

/*!
 \brief Returns a complex matrix in string form.
 \return Returns complex matrix as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv complexMatrixToString(const RRComplexMatrixPtr mat);

/*!
 \brief Returns the length of a string array
 \return Returns the length of a string array, return -1 if string array is NULL
 \ingroup stringArray
*/
C_DECL_SPEC int rrcCallConv getNumberOfStringElements(const RRStringArrayPtr list);

/*!
 \brief Returns the indexth element from the string array in the argument value
 \return Returns pointer to string else return null if error
 \ingroup stringArray
*/
C_DECL_SPEC char* rrcCallConv getStringElement (RRStringArrayPtr list, int index);

/*!
 \brief Returns a string list in string form.
 \return Returns string list as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv stringArrayToString(const RRStringArrayPtr list);

/*!
 \brief Returns a list in string form.
 \return Returns list as a character string
 \ingroup toString
*/
C_DECL_SPEC char* rrcCallConv listToString(const RRListPtr list);


//======================== DATA WRITING ROUTINES =============================
/*!
 \brief Writes RoadRunner data to file
 \param handle Handle to a Roadrunner Instance
 \param fileNameAndPath Pointer to string holding the file(with path) to write data to
 \return Returns a t/f indicating the result
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv writeRRData(RRHandle handle, const char* fileNameAndPath);


///////////////////////////////////////////////////////////////////////////////////
// TEST UTILITY functions (to be documented later. Only for internal testing)
/*!
 \brief Compiles source code
 \param handle Handle to a RRInstance
 \param sourceFileNameAndPath Pointer to string holding the file(with path) to compile
 \return Returns a t/f indicating the result
 \ingroup helperRoutines
*/
C_DECL_SPEC int rrcCallConv compileSource(RRHandle handle, const char* sourceFileNameAndPath);


#if defined(__cplusplus)

}   //namespace
}   //Extern "C"
#endif

#endif
