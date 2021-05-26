/**
 * @file:   ChangedMath.h
 * @brief:  Implementation of the ChangedMath class
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


#ifndef ChangedMath_H__
#define ChangedMath_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/req/common/reqfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/req/extension/ReqExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ChangedMath : public SBase
{

protected:

////  std::string   mId;
////  std::string   mName;
  std::string   mChangedBy;
  bool          mViableWithoutChange;
  bool          mIsSetViableWithoutChange;


public:

  /**
   * Creates a new ChangedMath with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ChangedMath
   *
   * @param version an unsigned int, the SBML Version to assign to this ChangedMath
   *
   * @param pkgVersion an unsigned int, the SBML Req Version to assign to this ChangedMath
   */
  ChangedMath(unsigned int level      = ReqExtension::getDefaultLevel(),
              unsigned int version    = ReqExtension::getDefaultVersion(),
              unsigned int pkgVersion = ReqExtension::getDefaultPackageVersion());


  /**
   * Creates a new ChangedMath with the given ReqPkgNamespaces object.
   *
   * @param reqns the ReqPkgNamespaces object
   */
  ChangedMath(ReqPkgNamespaces* reqns);


   /**
   * Copy constructor for ChangedMath.
   *
   * @param orig; the ChangedMath instance to copy.
   */
  ChangedMath(const ChangedMath& orig);


   /**
   * Assignment operator for ChangedMath.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  ChangedMath& operator=(const ChangedMath& rhs);


   /**
   * Creates and returns a deep copy of this ChangedMath object.
   *
   * @return a (deep) copy of this ChangedMath object.
   */
  virtual ChangedMath* clone () const;


   /**
   * Destructor for ChangedMath.
   */
  virtual ~ChangedMath();


   /**
   * Returns the value of the "id" attribute of this ChangedMath.
   *
   * @return the value of the "id" attribute of this ChangedMath as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this ChangedMath.
   *
   * @return the value of the "name" attribute of this ChangedMath as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Returns the value of the "changedBy" attribute of this ChangedMath.
   *
   * @return the value of the "changedBy" attribute of this ChangedMath as a string.
   */
  virtual const std::string& getChangedBy() const;


  /**
   * Returns the value of the "viableWithoutChange" attribute of this ChangedMath.
   *
   * @return the value of the "viableWithoutChange" attribute of this ChangedMath as a boolean.
   */
  virtual bool getViableWithoutChange() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ChangedMath's "id" attribute has been set.
   *
   * @return @c true if this ChangedMath's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ChangedMath's "name" attribute has been set.
   *
   * @return @c true if this ChangedMath's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ChangedMath's "changedBy" attribute has been set.
   *
   * @return @c true if this ChangedMath's "changedBy" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetChangedBy() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ChangedMath's "viableWithoutChange" attribute has been set.
   *
   * @return @c true if this ChangedMath's "viableWithoutChange" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetViableWithoutChange() const;


  /**
   * Sets the value of the "id" attribute of this ChangedMath.
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
   * Sets the value of the "name" attribute of this ChangedMath.
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
   * Sets the value of the "changedBy" attribute of this ChangedMath.
   *
   * @param changedBy; const std::string& value of the "changedBy" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setChangedBy(const std::string& changedBy);


  /**
   * Sets the value of the "viableWithoutChange" attribute of this ChangedMath.
   *
   * @param viableWithoutChange; bool value of the "viableWithoutChange" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setViableWithoutChange(bool viableWithoutChange);


  /**
   * Unsets the value of the "id" attribute of this ChangedMath.
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
   * Unsets the value of the "name" attribute of this ChangedMath.
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
   * Unsets the value of the "changedBy" attribute of this ChangedMath.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetChangedBy();


  /**
   * Unsets the value of the "viableWithoutChange" attribute of this ChangedMath.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetViableWithoutChange();


  /**
   * Returns the XML element name of this object, which for ChangedMath, is
   * always @c "changedMath".
   *
   * @return the name of this element, i.e. @c "changedMath".
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
   * for this ChangedMath object have been set.
   *
   * @note The required attributes for a ChangedMath object are:
   * @li "changedBy"
   * @li "viableWithoutChange"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


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
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);


  /** @endcond doxygenLibsbmlInternal */


protected:

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

class LIBSBML_EXTERN ListOfChangedMaths : public ListOf
{

public:

  /**
   * Creates a new ListOfChangedMaths with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfChangedMaths
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfChangedMaths
   *
   * @param pkgVersion an unsigned int, the SBML Req Version to assign to this ListOfChangedMaths
   */
  ListOfChangedMaths(unsigned int level      = ReqExtension::getDefaultLevel(),
                     unsigned int version    = ReqExtension::getDefaultVersion(),
                     unsigned int pkgVersion = ReqExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfChangedMaths with the given ReqPkgNamespaces object.
   *
   * @param reqns the ReqPkgNamespaces object
   */
  ListOfChangedMaths(ReqPkgNamespaces* reqns);


   /**
   * Creates and returns a deep copy of this ListOfChangedMaths object.
   *
   * @return a (deep) copy of this ListOfChangedMaths object.
   */
  virtual ListOfChangedMaths* clone () const;


   /**
   * Get a ChangedMath from the ListOfChangedMaths.
   *
   * @param n the index number of the ChangedMath to get.
   *
   * @return the nth ChangedMath in this ListOfChangedMaths.
   *
   * @see size()
   */
  virtual ChangedMath* get(unsigned int n);


  /**
   * Get a ChangedMath from the ListOfChangedMaths.
   *
   * @param n the index number of the ChangedMath to get.
   *
   * @return the nth ChangedMath in this ListOfChangedMaths.
   *
   * @see size()
   */
  virtual const ChangedMath* get(unsigned int n) const;


  /**
   * Get a ChangedMath from the ListOfChangedMaths
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the ChangedMath to get.
   *
   * @return ChangedMath in this ListOfChangedMaths
   * with the given id or NULL if no such
   * ChangedMath exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual ChangedMath* get(const std::string& sid);


  /**
   * Get a ChangedMath from the ListOfChangedMaths
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the ChangedMath to get.
   *
   * @return ChangedMath in this ListOfChangedMaths
   * with the given id or NULL if no such
   * ChangedMath exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const ChangedMath* get(const std::string& sid) const;


  /**
   * Removes the nth ChangedMath from this ListOfChangedMaths
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the ChangedMath to remove.
   *
   * @see size()
   */
  virtual ChangedMath* remove(unsigned int n);


  /**
   * Removes the ChangedMath from this ListOfChangedMaths with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the ChangedMath to remove.
   *
   * @return the ChangedMath removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual ChangedMath* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfChangedMaths, is
   * always @c "listOfChangedMaths".
   *
   * @return the name of this element, i.e. @c "listOfChangedMaths".
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
   * Creates a new ChangedMath in this ListOfChangedMaths
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the Req package.
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
ChangedMath_t *
ChangedMath_create(unsigned int level, unsigned int version,
                   unsigned int pkgVersion);


LIBSBML_EXTERN
void
ChangedMath_free(ChangedMath_t * cm);


LIBSBML_EXTERN
ChangedMath_t *
ChangedMath_clone(ChangedMath_t * cm);


LIBSBML_EXTERN
char *
ChangedMath_getId(ChangedMath_t * cm);


LIBSBML_EXTERN
char *
ChangedMath_getName(ChangedMath_t * cm);


LIBSBML_EXTERN
const char *
ChangedMath_getChangedBy(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_getViableWithoutChange(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_isSetId(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_isSetName(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_isSetChangedBy(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_isSetViableWithoutChange(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_setId(ChangedMath_t * cm, const char * id);


LIBSBML_EXTERN
int
ChangedMath_setName(ChangedMath_t * cm, const char * name);


LIBSBML_EXTERN
int
ChangedMath_setChangedBy(ChangedMath_t * cm, const char * changedBy);


LIBSBML_EXTERN
int
ChangedMath_setViableWithoutChange(ChangedMath_t * cm, int viableWithoutChange);


LIBSBML_EXTERN
int
ChangedMath_unsetId(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_unsetName(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_unsetChangedBy(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_unsetViableWithoutChange(ChangedMath_t * cm);


LIBSBML_EXTERN
int
ChangedMath_hasRequiredAttributes(ChangedMath_t * cm);


LIBSBML_EXTERN
ChangedMath_t *
ListOfChangedMaths_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
ChangedMath_t *
ListOfChangedMaths_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  ChangedMath_H__  */

