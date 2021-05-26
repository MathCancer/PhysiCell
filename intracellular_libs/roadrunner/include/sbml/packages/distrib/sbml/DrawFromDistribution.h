/**
 * @file:   DrawFromDistribution.h
 * @brief:  Implementation of the DrawFromDistribution class
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


#ifndef DrawFromDistribution_H__
#define DrawFromDistribution_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/distrib/common/distribfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/distrib/extension/DistribExtension.h>

#include <sbml/packages/distrib/sbml/DistribInput.h>
#include <sbml/packages/distrib/util/UncertMLNode.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DrawFromDistribution : public SBase
{

protected:

  ListOfDistribInputs   mDistribInputs;
  UncertMLNode* mUncertML;


public:

  /**
   * Creates a new DrawFromDistribution with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this DrawFromDistribution
   *
   * @param version an unsigned int, the SBML Version to assign to this DrawFromDistribution
   *
   * @param pkgVersion an unsigned int, the SBML Distrib Version to assign to this DrawFromDistribution
   */
  DrawFromDistribution(unsigned int level      = DistribExtension::getDefaultLevel(),
                       unsigned int version    = DistribExtension::getDefaultVersion(),
                       unsigned int pkgVersion = DistribExtension::getDefaultPackageVersion());


  /**
   * Creates a new DrawFromDistribution with the given DistribPkgNamespaces object.
   *
   * @param distribns the DistribPkgNamespaces object
   */
  DrawFromDistribution(DistribPkgNamespaces* distribns);


   /**
   * Copy constructor for DrawFromDistribution.
   *
   * @param orig; the DrawFromDistribution instance to copy.
   */
  DrawFromDistribution(const DrawFromDistribution& orig);


   /**
   * Assignment operator for DrawFromDistribution.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DrawFromDistribution& operator=(const DrawFromDistribution& rhs);


   /**
   * Creates and returns a deep copy of this DrawFromDistribution object.
   *
   * @return a (deep) copy of this DrawFromDistribution object.
   */
  virtual DrawFromDistribution* clone () const;


   /**
   * Destructor for DrawFromDistribution.
   */
  virtual ~DrawFromDistribution();


   /**
   * Returns the "UncertML" element of this DrawFromDistribution.
   *
   * @return the "UncertML" element of this DrawFromDistribution.
   */
  virtual const UncertMLNode* getUncertML() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DrawFromDistribution's "UncertML" element has been set.
   *
   * @return @c true if this DrawFromDistribution's "UncertML" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetUncertML() const;


  /**
   * Sets the "UncertML" element of this DrawFromDistribution.
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
  virtual int setUncertML(UncertMLNode* uncertML);


  /**
   * Create a new "UncertML" element object, sets it to be the child of this 
   * DrawFromDistribution and returns the UncertMLNode created.  Removes
   * and deletes any old UncertML node that may have existed previously.
   *
   * @return a new UncertMLNode object instance
   *
   * @see setUncertML(const DistribInput* di)
   */
  virtual UncertMLNode* createUncertML();


  /**
   * Unsets the "UncertML" element of this DrawFromDistribution.
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
   * Returns the  "ListOfDistribInputs" in this DrawFromDistribution object.
   *
   * @return the "ListOfDistribInputs" attribute of this DrawFromDistribution.
   */
  const ListOfDistribInputs* getListOfDistribInputs() const;


  /**
   * Returns the  "ListOfDistribInputs" in this DrawFromDistribution object.
   *
   * @return the "ListOfDistribInputs" attribute of this DrawFromDistribution.
   */
  ListOfDistribInputs* getListOfDistribInputs();


  /**
   * Get a DistribInput from the ListOfDistribInputs.
   *
   * @param n the index number of the DistribInput to get.
   *
   * @return the nth DistribInput in the ListOfDistribInputs within this DrawFromDistribution.
   *
   * @see getNumDistribInputs()
   */
  DistribInput* getDistribInput(unsigned int n);


  /**
   * Get a DistribInput from the ListOfDistribInputs.
   *
   * @param n the index number of the DistribInput to get.
   *
   * @return the nth DistribInput in the ListOfDistribInputs within this DrawFromDistribution.
   *
   * @see getNumDistribInputs()
   */
  const DistribInput* getDistribInput(unsigned int n) const;


  /**
   * Get a DistribInput from the ListOfDistribInputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DistribInput to get.
   *
   * @return the DistribInput in the ListOfDistribInputs
   * with the given id or NULL if no such
   * DistribInput exists.
   *
   * @see getDistribInput(unsigned int n)
   *
   * @see getNumDistribInputs()
   */
  DistribInput* getDistribInput(const std::string& sid);


  /**
   * Get a DistribInput from the ListOfDistribInputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DistribInput to get.
   *
   * @return the DistribInput in the ListOfDistribInputs
   * with the given id or NULL if no such
   * DistribInput exists.
   *
   * @see getDistribInput(unsigned int n)
   *
   * @see getNumDistribInputs()
   */
  const DistribInput* getDistribInput(const std::string& sid) const;


  /**
   * Adds a copy the given "DistribInput" to this DrawFromDistribution.
   *
   * @param di; the DistribInput object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int addDistribInput(const DistribInput* di);


  /**
   * Get the number of DistribInput objects in this DrawFromDistribution.
   *
   * @return the number of DistribInput objects in this DrawFromDistribution
   */
  unsigned int getNumDistribInputs() const;


  /**
   * Creates a new DistribInput object, adds it to this DrawFromDistributions
   * ListOfDistribInputs and returns the DistribInput object created. 
   *
   * @return a new DistribInput object instance
   *
   * @see addDistribInput(const DistribInput* di)
   */
  DistribInput* createDistribInput();


  /**
   * Removes the nth DistribInput from the ListOfDistribInputs within this DrawFromDistribution.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DistribInput to remove.
   *
   * @see getNumDistribInputs()
   */
  DistribInput* removeDistribInput(unsigned int n);


  /**
   * Removes the DistribInput with the given identifier from the ListOfDistribInputs within this DrawFromDistribution
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
  DistribInput* removeDistribInput(const std::string& sid);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object, which for DrawFromDistribution, is
   * always @c "drawFromDistribution".
   *
   * @return the name of this element, i.e. @c "drawFromDistribution".
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
   * for this DrawFromDistribution object have been set.
   *
   * @note The required attributes for a DrawFromDistribution object are:
   * @li "UncertML"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this DrawFromDistribution object have been set.
   *
   * @note The required elements for a DrawFromDistribution object are:
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
DrawFromDistribution_t *
DrawFromDistribution_create(unsigned int level, unsigned int version,
                            unsigned int pkgVersion);


LIBSBML_EXTERN
void
DrawFromDistribution_free(DrawFromDistribution_t * dfd);


LIBSBML_EXTERN
DrawFromDistribution_t *
DrawFromDistribution_clone(DrawFromDistribution_t * dfd);


LIBSBML_EXTERN
int
DrawFromDistribution_addDistribInput(DrawFromDistribution_t * dfd, DistribInput_t * di);


LIBSBML_EXTERN
DistribInput_t *
DrawFromDistribution_createDistribInput(DrawFromDistribution_t * dfd);


LIBSBML_EXTERN
ListOf_t *
DrawFromDistribution_getListOfDistribInputs(DrawFromDistribution_t * dfd) ;


LIBSBML_EXTERN
DistribInput_t *
DrawFromDistribution_getDistribInput(DrawFromDistribution_t * dfd, unsigned int n);


LIBSBML_EXTERN
DistribInput_t *
DrawFromDistribution_getDistribInputById(DrawFromDistribution_t * dfd, const char * sid);


LIBSBML_EXTERN
unsigned int
DrawFromDistribution_getNumDistribInputs(DrawFromDistribution_t * dfd);


LIBSBML_EXTERN
DistribInput_t *
DrawFromDistribution_removeDistribInput(DrawFromDistribution_t * dfd, unsigned int n);


LIBSBML_EXTERN
DistribInput_t *
DrawFromDistribution_removeDistribInputById(DrawFromDistribution_t * dfd, const char * sid);


LIBSBML_EXTERN
int
DrawFromDistribution_hasRequiredAttributes(DrawFromDistribution_t * dfd);


LIBSBML_EXTERN
int
DrawFromDistribution_hasRequiredElements(DrawFromDistribution_t * dfd);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  DrawFromDistribution_H__  */

