/**
 * @file:   DistribInput.h
 * @brief:  Implementation of the DistribInput class
 * @author: SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */


#ifndef DistribInput_H__
#define DistribInput_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/distrib/common/distribfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/distrib/extension/DistribExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DistribInput : public SBase
{

protected:

////  std::string   mId;
////  std::string   mName;
  unsigned int  mIndex;
  bool          mIsSetIndex;


public:

  /**
   * Creates a new DistribInput with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this DistribInput
   *
   * @param version an unsigned int, the SBML Version to assign to this DistribInput
   *
   * @param pkgVersion an unsigned int, the SBML Distrib Version to assign to this DistribInput
   */
  DistribInput(unsigned int level      = DistribExtension::getDefaultLevel(),
               unsigned int version    = DistribExtension::getDefaultVersion(),
               unsigned int pkgVersion = DistribExtension::getDefaultPackageVersion());


  /**
   * Creates a new DistribInput with the given DistribPkgNamespaces object.
   *
   * @param distribns the DistribPkgNamespaces object
   */
  DistribInput(DistribPkgNamespaces* distribns);


   /**
   * Copy constructor for DistribInput.
   *
   * @param orig; the DistribInput instance to copy.
   */
  DistribInput(const DistribInput& orig);


   /**
   * Assignment operator for DistribInput.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DistribInput& operator=(const DistribInput& rhs);


   /**
   * Creates and returns a deep copy of this DistribInput object.
   *
   * @return a (deep) copy of this DistribInput object.
   */
  virtual DistribInput* clone () const;


   /**
   * Destructor for DistribInput.
   */
  virtual ~DistribInput();


   /**
   * Returns the value of the "id" attribute of this DistribInput.
   *
   * @return the value of the "id" attribute of this DistribInput as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this DistribInput.
   *
   * @return the value of the "name" attribute of this DistribInput as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Returns the value of the "index" attribute of this DistribInput.
   *
   * @return the value of the "index" attribute of this DistribInput as a unsigned integer.
   */
  virtual unsigned int getIndex() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DistribInput's "id" attribute has been set.
   *
   * @return @c true if this DistribInput's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DistribInput's "name" attribute has been set.
   *
   * @return @c true if this DistribInput's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DistribInput's "index" attribute has been set.
   *
   * @return @c true if this DistribInput's "index" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetIndex() const;


  /**
   * Sets the value of the "id" attribute of this DistribInput.
   *
   * @param id; const std::string& value of the "id" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setId(const std::string& id);


  /**
   * Sets the value of the "name" attribute of this DistribInput.
   *
   * @param name; const std::string& value of the "name" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setName(const std::string& name);


  /**
   * Sets the value of the "index" attribute of this DistribInput.
   *
   * @param index; unsigned int value of the "index" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setIndex(unsigned int index);


  /**
   * Unsets the value of the "id" attribute of this DistribInput.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this DistribInput.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetName();


  /**
   * Unsets the value of the "index" attribute of this DistribInput.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetIndex();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object, which for DistribInput, is
   * always @c "distribInput".
   *
   * @return the name of this element, i.e. @c "distribInput".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this DistribInput object have been set.
   *
   * @note The required attributes for a DistribInput object are:
   * @li "id"
   * @li "index"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this DistribInput object have been set.
   *
   * @note The required elements for a DistribInput object are:
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  virtual bool hasRequiredElements() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Connects to child elements.
   */
  virtual void connectToChild ();


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);


  /** @endcond doxygenLibsbmlInternal */


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * return the SBML object corresponding to next XMLToken.
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */



};

class LIBSBML_EXTERN ListOfDistribInputs : public ListOf
{

public:

  /**
   * Creates a new ListOfDistribInputs with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfDistribInputs
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfDistribInputs
   *
   * @param pkgVersion an unsigned int, the SBML Distrib Version to assign to this ListOfDistribInputs
   */
  ListOfDistribInputs(unsigned int level      = DistribExtension::getDefaultLevel(),
                      unsigned int version    = DistribExtension::getDefaultVersion(),
                      unsigned int pkgVersion = DistribExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfDistribInputs with the given DistribPkgNamespaces object.
   *
   * @param distribns the DistribPkgNamespaces object
   */
  ListOfDistribInputs(DistribPkgNamespaces* distribns);


   /**
   * Creates and returns a deep copy of this ListOfDistribInputs object.
   *
   * @return a (deep) copy of this ListOfDistribInputs object.
   */
  virtual ListOfDistribInputs* clone () const;


   /**
   * Get a DistribInput from the ListOfDistribInputs.
   *
   * @param n the index number of the DistribInput to get.
   *
   * @return the nth DistribInput in this ListOfDistribInputs.
   *
   * @see size()
   */
  virtual DistribInput* get(unsigned int n);


  /**
   * Get a DistribInput from the ListOfDistribInputs.
   *
   * @param n the index number of the DistribInput to get.
   *
   * @return the nth DistribInput in this ListOfDistribInputs.
   *
   * @see size()
   */
  virtual const DistribInput* get(unsigned int n) const;


  /**
   * Get a DistribInput from the ListOfDistribInputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DistribInput to get.
   *
   * @return DistribInput in this ListOfDistribInputs
   * with the given id or NULL if no such
   * DistribInput exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual DistribInput* get(const std::string& sid);


  /**
   * Get a DistribInput from the ListOfDistribInputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DistribInput to get.
   *
   * @return DistribInput in this ListOfDistribInputs
   * with the given id or NULL if no such
   * DistribInput exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const DistribInput* get(const std::string& sid) const;


   /**
   * Get a DistribInput from the ListOfDistribInputs based on its 'index' attribute.
   *
   * @param n the value of the index attribute of the DistribInput to get.
   *
   * @return the DistribInput in this ListOfDistribInputs with the given index attribute, or NULL if no such element exists.
   *
   * @see get(unsigned int n)   *
   * @see get(const std::string& sid)   *
   */
  virtual DistribInput* getByIndex(unsigned int n);


  /**
   * Get a DistribInput from the ListOfDistribInputs based on its 'index' attribute.
   *
   * @param n the value of the index attribute of the DistribInput to get.
   *
   * @return the DistribInput in this ListOfDistribInputs with the given index attribute, or NULL if no such element exists.
   *
   * @see get(unsigned int n)   *
   * @see get(const std::string& sid)   *
   */
  virtual const DistribInput* getByIndex(unsigned int n) const;


  /**
   * Removes the nth DistribInput from this ListOfDistribInputs
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DistribInput to remove.
   *
   * @see size()
   */
  virtual DistribInput* remove(unsigned int n);


  /**
   * Removes the DistribInput from this ListOfDistribInputs with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the DistribInput to remove.
   *
   * @return the DistribInput removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual DistribInput* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfDistribInputs, is
   * always @c "listOfDistribInputs".
   *
   * @return the name of this element, i.e. @c "listOfDistribInputs".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;


  /**
   * Returns the libSBML type code for the SBML objects
   * contained in this ListOf object
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for the objects in this ListOf instance, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getItemTypeCode () const;


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new DistribInput in this ListOfDistribInputs
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the Distrib package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */



};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSBML_EXTERN
DistribInput_t *
DistribInput_create(unsigned int level, unsigned int version,
                    unsigned int pkgVersion);


LIBSBML_EXTERN
void
DistribInput_free(DistribInput_t * di);


LIBSBML_EXTERN
DistribInput_t *
DistribInput_clone(DistribInput_t * di);


LIBSBML_EXTERN
char *
DistribInput_getId(DistribInput_t * di);


LIBSBML_EXTERN
char *
DistribInput_getName(DistribInput_t * di);


LIBSBML_EXTERN
unsigned int
DistribInput_getIndex(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_isSetId(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_isSetName(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_isSetIndex(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_setId(DistribInput_t * di, const char * id);


LIBSBML_EXTERN
int
DistribInput_setName(DistribInput_t * di, const char * name);


LIBSBML_EXTERN
int
DistribInput_setIndex(DistribInput_t * di, unsigned int index);


LIBSBML_EXTERN
int
DistribInput_unsetId(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_unsetName(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_unsetIndex(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_hasRequiredAttributes(DistribInput_t * di);


LIBSBML_EXTERN
int
DistribInput_hasRequiredElements(DistribInput_t * di);


LIBSBML_EXTERN
DistribInput_t *
ListOfDistribInputs_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
DistribInput_t *
ListOfDistribInputs_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  DistribInput_H__  */

