/**
 * @file:   Uncertainty.h
 * @brief:  Implementation of the Uncertainty class
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


#ifndef Uncertainty_H__
#define Uncertainty_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/distrib/common/distribfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/distrib/extension/DistribExtension.h>

#include <sbml/packages/distrib/util/UncertMLNode.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Uncertainty : public SBase
{

protected:

////  std::string   mId;
////  std::string   mName;
  UncertMLNode* mUncertML;


public:

  /**
   * Creates a new Uncertainty with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this Uncertainty
   *
   * @param version an unsigned int, the SBML Version to assign to this Uncertainty
   *
   * @param pkgVersion an unsigned int, the SBML Distrib Version to assign to this Uncertainty
   */
  Uncertainty(unsigned int level      = DistribExtension::getDefaultLevel(),
              unsigned int version    = DistribExtension::getDefaultVersion(),
              unsigned int pkgVersion = DistribExtension::getDefaultPackageVersion());


  /**
   * Creates a new Uncertainty with the given DistribPkgNamespaces object.
   *
   * @param distribns the DistribPkgNamespaces object
   */
  Uncertainty(DistribPkgNamespaces* distribns);


  /**
   * Copy constructor for Uncertainty.
   *
   * @param orig; the Uncertainty instance to copy.
   */
  Uncertainty(const Uncertainty& orig);


  /**
   * Assignment operator for Uncertainty.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  Uncertainty& operator=(const Uncertainty& rhs);


  /**
   * Creates and returns a deep copy of this Uncertainty object.
   *
   * @return a (deep) copy of this Uncertainty object.
   */
  virtual Uncertainty* clone () const;


  /**
   * Destructor for Uncertainty.
   */
  virtual ~Uncertainty();


  /**
   * Returns the value of the "id" attribute of this Uncertainty.
   *
   * @return the value of the "id" attribute of this Uncertainty as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this Uncertainty.
   *
   * @return the value of the "name" attribute of this Uncertainty as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Returns the "UncertML" element of this Uncertainty.
   *
   * @return the "UncertML" element of this Uncertainty.
   */
  virtual const UncertMLNode* getUncertML() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Uncertainty's "id" attribute has been set.
   *
   * @return @c true if this Uncertainty's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Uncertainty's "name" attribute has been set.
   *
   * @return @c true if this Uncertainty's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Uncertainty's "UncertML" element has been set.
   *
   * @return @c true if this Uncertainty's "UncertML" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetUncertML() const;


  /**
   * Sets the value of the "id" attribute of this Uncertainty.
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
   * Sets the value of the "name" attribute of this Uncertainty.
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
   * Sets the "UncertML" element of this Uncertainty.
   *
   * @param UncertML; UncertMLNode* to be set.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setUncertML(UncertMLNode* UncertML);


  /**
   * Unsets the value of the "id" attribute of this Uncertainty.
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
   * Unsets the value of the "name" attribute of this Uncertainty.
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
   * Unsets the "UncertML" element of this Uncertainty.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetUncertML();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object, which for Uncertainty, is
   * always @c "uncertainty".
   *
   * @return the name of this element, i.e. @c "uncertainty".
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
   * for this Uncertainty object have been set.
   *
   * @note The required attributes for a Uncertainty object are:
   * @li "UncertML"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this Uncertainty object have been set.
   *
   * @note The required elements for a Uncertainty object are:
   * @li "UncertML"
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


  virtual bool readOtherXML(XMLInputStream &stream);

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



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSBML_EXTERN
Uncertainty_t *
Uncertainty_create(unsigned int level, unsigned int version,
                   unsigned int pkgVersion);


LIBSBML_EXTERN
void
Uncertainty_free(Uncertainty_t * u);


LIBSBML_EXTERN
Uncertainty_t *
Uncertainty_clone(Uncertainty_t * u);


LIBSBML_EXTERN
char *
Uncertainty_getId(Uncertainty_t * u);


LIBSBML_EXTERN
char *
Uncertainty_getName(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_isSetId(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_isSetName(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_setId(Uncertainty_t * u, const char * id);


LIBSBML_EXTERN
int
Uncertainty_setName(Uncertainty_t * u, const char * name);


LIBSBML_EXTERN
int
Uncertainty_unsetId(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_unsetName(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_hasRequiredAttributes(Uncertainty_t * u);


LIBSBML_EXTERN
int
Uncertainty_hasRequiredElements(Uncertainty_t * u);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  Uncertainty_H__  */

