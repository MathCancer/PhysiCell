/**
 * @file:   SpatialComponent.h
 * @brief:  Implementation of the SpatialComponent class
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


#ifndef SpatialComponent_H__
#define SpatialComponent_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/dyn/common/dynfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/dyn/extension/DynExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN SpatialComponent : public SBase
{

protected:

  SpatialKind_t   mSpatialIndex;
  std::string   mVariable;
////  std::string   mId;
////  std::string   mName;


public:

  /**
   * Creates a new SpatialComponent with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this SpatialComponent
   *
   * @param version an unsigned int, the SBML Version to assign to this SpatialComponent
   *
   * @param pkgVersion an unsigned int, the SBML Dyn Version to assign to this SpatialComponent
   */
  SpatialComponent(unsigned int level      = DynExtension::getDefaultLevel(),
                   unsigned int version    = DynExtension::getDefaultVersion(),
                   unsigned int pkgVersion = DynExtension::getDefaultPackageVersion());


  /**
   * Creates a new SpatialComponent with the given DynPkgNamespaces object.
   *
   * @param dynns the DynPkgNamespaces object
   */
  SpatialComponent(DynPkgNamespaces* dynns);


   /**
   * Copy constructor for SpatialComponent.
   *
   * @param orig; the SpatialComponent instance to copy.
   */
  SpatialComponent(const SpatialComponent& orig);


   /**
   * Assignment operator for SpatialComponent.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SpatialComponent& operator=(const SpatialComponent& rhs);


   /**
   * Creates and returns a deep copy of this SpatialComponent object.
   *
   * @return a (deep) copy of this SpatialComponent object.
   */
  virtual SpatialComponent* clone () const;


   /**
   * Destructor for SpatialComponent.
   */
  virtual ~SpatialComponent();


   /**
   * Returns the value of the "spatialIndex" attribute of this SpatialComponent.
   *
   * @return the value of the "spatialIndex" attribute of this SpatialComponent as a SpatialKind_t.
   */
  virtual SpatialKind_t getSpatialIndex() const;


  /**
   * Returns the value of the "variable" attribute of this SpatialComponent.
   *
   * @return the value of the "variable" attribute of this SpatialComponent as a string.
   */
  virtual const std::string& getVariable() const;


  /**
   * Returns the value of the "id" attribute of this SpatialComponent.
   *
   * @return the value of the "id" attribute of this SpatialComponent as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this SpatialComponent.
   *
   * @return the value of the "name" attribute of this SpatialComponent as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpatialComponent's "spatialIndex" attribute has been set.
   *
   * @return @c true if this SpatialComponent's "spatialIndex" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetSpatialIndex() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpatialComponent's "variable" attribute has been set.
   *
   * @return @c true if this SpatialComponent's "variable" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetVariable() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpatialComponent's "id" attribute has been set.
   *
   * @return @c true if this SpatialComponent's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpatialComponent's "name" attribute has been set.
   *
   * @return @c true if this SpatialComponent's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "spatialIndex" attribute of this SpatialComponent.
   *
   * @param spatialIndex; SpatialKind_t value of the "spatialIndex" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setSpatialIndex(SpatialKind_t spatialIndex);


  /**
   * Sets the value of the "spatialIndex" attribute of this SpatialComponent.
   *
   * @param spatialIndex; string value of the "spatialIndex" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setSpatialIndex(const std::string& spatialIndex);


  /**
   * Sets the value of the "variable" attribute of this SpatialComponent.
   *
   * @param variable; const std::string& value of the "variable" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setVariable(const std::string& variable);


  /**
   * Sets the value of the "id" attribute of this SpatialComponent.
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
   * Sets the value of the "name" attribute of this SpatialComponent.
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
   * Unsets the value of the "spatialIndex" attribute of this SpatialComponent.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetSpatialIndex();


  /**
   * Unsets the value of the "variable" attribute of this SpatialComponent.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetVariable();


  /**
   * Unsets the value of the "id" attribute of this SpatialComponent.
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
   * Unsets the value of the "name" attribute of this SpatialComponent.
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
   * Renames all the @c SIdRef attributes on this element, including any
   * found in MathML content (if such exists).
   *
   * This method works by looking at all attributes and (if appropriate)
   * mathematical formulas, comparing the identifiers to the value of @p
   * oldid.  If any matches are found, the matching identifiers are replaced
   * with @p newid.  The method does @em not descend into child elements.
   *
   * @param oldid the old identifier
   * @param newid the new identifier
   */
   virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the XML element name of this object, which for SpatialComponent, is
   * always @c "spatialComponent".
   *
   * @return the name of this element, i.e. @c "spatialComponent".
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
   * for this SpatialComponent object have been set.
   *
   * @note The required attributes for a SpatialComponent object are:
   * @li "spatialIndex"
   * @li "variable"
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

class LIBSBML_EXTERN ListOfSpatialComponents : public ListOf
{

public:

  /**
   * Creates a new ListOfSpatialComponents with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfSpatialComponents
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfSpatialComponents
   *
   * @param pkgVersion an unsigned int, the SBML Dyn Version to assign to this ListOfSpatialComponents
   */
  ListOfSpatialComponents(unsigned int level      = DynExtension::getDefaultLevel(),
                          unsigned int version    = DynExtension::getDefaultVersion(),
                          unsigned int pkgVersion = DynExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfSpatialComponents with the given DynPkgNamespaces object.
   *
   * @param dynns the DynPkgNamespaces object
   */
  ListOfSpatialComponents(DynPkgNamespaces* dynns);


   /**
   * Creates and returns a deep copy of this ListOfSpatialComponents object.
   *
   * @return a (deep) copy of this ListOfSpatialComponents object.
   */
  virtual ListOfSpatialComponents* clone () const;


   /**
   * Get a SpatialComponent from the ListOfSpatialComponents.
   *
   * @param n the index number of the SpatialComponent to get.
   *
   * @return the nth SpatialComponent in this ListOfSpatialComponents.
   *
   * @see size()
   */
	virtual SpatialComponent* get(unsigned int n);


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents.
   *
   * @param n the index number of the SpatialComponent to get.
   *
   * @return the nth SpatialComponent in this ListOfSpatialComponents.
   *
   * @see size()
   */
	virtual const SpatialComponent* get(unsigned int n) const;


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpatialComponent to get.
   *
   * @return SpatialComponent in this ListOfSpatialComponents
   * with the given id or NULL if no such
   * SpatialComponent exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
	virtual SpatialComponent* get(const std::string& sid);


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpatialComponent to get.
   *
   * @return SpatialComponent in this ListOfSpatialComponents
   * with the given id or NULL if no such
   * SpatialComponent exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const SpatialComponent* get(const std::string& sid) const;


	/**
	 * Adds a copy the given "SpatialComponent" to this ListOfSpatialComponents.
	 *
	 * @param sc; the SpatialComponent object to add
	 *
	 * @return integer value indicating success/failure of the
	 * function.  @if clike The value is drawn from the
	 * enumeration #OperationReturnValues_t. @endif The possible values
	 * returned by this function are:
	 * @li LIBSEDML_OPERATION_SUCCESS
	 * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
	 */
	int addSpatialComponent(const SpatialComponent* sc);


	/**
	 * Get the number of SpatialComponent objects in this ListOfSpatialComponents.
	 *
	 * @return the number of SpatialComponent objects in this ListOfSpatialComponents
	 */
	unsigned int getNumSpatialComponents() const;


	/**
	 * Creates a new SpatialComponent object, adds it to the
	 * ListOfSpatialComponents and returns the SpatialComponent object created. 
	 *
	 * @return a new SpatialComponent object instance
	 *
	 * @see addSpatialComponent(const SpatialComponent* sc)
	 */
	SpatialComponent* createSpatialComponent();


  /**
   * Removes the nth SpatialComponent from this ListOfSpatialComponents
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the SpatialComponent to remove.
   *
   * @see size()
   */
	virtual SpatialComponent* remove(unsigned int n);


  /**
   * Removes the SpatialComponent from this ListOfSpatialComponents with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the SpatialComponent to remove.
   *
   * @return the SpatialComponent removed. As mentioned above, the caller owns the
   * returned item.
   */
	virtual SpatialComponent* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfSpatialComponents, is
   * always @c "listOfSpatialComponents".
   *
   * @return the name of this element, i.e. @c "listOfSpatialComponents".
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
   * Creates a new SpatialComponent in this ListOfSpatialComponents
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the Dyn package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */



};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new SpatialComponent_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * SpatialComponent_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * SpatialComponent_t structure.
 *
 * @returns the newly-created SpatialComponent_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof SpatialComponent_t
 */
LIBSBML_EXTERN
SpatialComponent_t *
SpatialComponent_create(unsigned int level, unsigned int version,
                        unsigned int pkgVersion);


/**
 * Frees the given SpatialComponent_t structure.
 * 
 * @param sc the SpatialComponent_t structure to be freed.
 *
 * @memberof SpatialComponent_t
 */
LIBSBML_EXTERN
void
SpatialComponent_free(SpatialComponent_t * sc);


/**
 * Creates a deep copy of the given SpatialComponent_t structure.
 * 
 * @param sc the SpatialComponent_t structure to be copied.
 *
 * @returns a (deep) copy of the given SpatialComponent_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof SpatialComponent_t
 */
LIBSBML_EXTERN
SpatialComponent_t *
SpatialComponent_clone(SpatialComponent_t * sc);


/**
 * Returns the value of the "spatialIndex" attribute of the given SpatialComponent_t
 * structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return the spatialIndex of this structure.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
SpatialKind_t
SpatialComponent_getSpatialIndex(const SpatialComponent_t * sc);


/**
 * Returns the value of the "variable" attribute of the given SpatialComponent_t
 * structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return the variable of this structure.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
const char *
SpatialComponent_getVariable(const SpatialComponent_t * sc);


/**
 * Returns the value of the "id" attribute of the given SpatialComponent_t
 * structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return the id of this structure.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
const char *
SpatialComponent_getId(const SpatialComponent_t * sc);


/**
 * Returns the value of the "name" attribute of the given SpatialComponent_t
 * structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return the name of this structure.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
const char *
SpatialComponent_getName(const SpatialComponent_t * sc);


/**
 * Predicate returning @c 1 if the given SpatialComponent_t structure's "spatialIndex"
 * is set.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return @c 1 if the "spatialIndex" of this SpatialComponent_t structure is
 * set, @c 0 otherwise.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_isSetSpatialIndex(const SpatialComponent_t * sc);


/**
 * Predicate returning @c 1 if the given SpatialComponent_t structure's "variable"
 * is set.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return @c 1 if the "variable" of this SpatialComponent_t structure is
 * set, @c 0 otherwise.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_isSetVariable(const SpatialComponent_t * sc);


/**
 * Predicate returning @c 1 if the given SpatialComponent_t structure's "id"
 * is set.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return @c 1 if the "id" of this SpatialComponent_t structure is
 * set, @c 0 otherwise.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_isSetId(const SpatialComponent_t * sc);


/**
 * Predicate returning @c 1 if the given SpatialComponent_t structure's "name"
 * is set.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return @c 1 if the "name" of this SpatialComponent_t structure is
 * set, @c 0 otherwise.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_isSetName(const SpatialComponent_t * sc);


/**
 * Sets the "spatialIndex" attribute of the given SpatialComponent_t structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @param spatialIndex the string to which the structures "spatialIndex" attribute should be
 * set.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_ATTRIBUTE_VALUE LIBSBML_INVALID_ATTRIBUTE_VALUE@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_setSpatialIndex(SpatialComponent_t * sc, SpatialKind_t spatialIndex);


/**
 * Sets the "variable" attribute of the given SpatialComponent_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs SpatialComponent_unsetVariable() instead.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @param variable the string to which the structures "variable" attribute should be
 * set.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_ATTRIBUTE_VALUE LIBSBML_INVALID_ATTRIBUTE_VALUE@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_setVariable(SpatialComponent_t * sc, const char * variable);


/**
 * Sets the "id" attribute of the given SpatialComponent_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs SpatialComponent_unsetId() instead.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @param id the string to which the structures "id" attribute should be
 * set.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_ATTRIBUTE_VALUE LIBSBML_INVALID_ATTRIBUTE_VALUE@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_setId(SpatialComponent_t * sc, const char * id);


/**
 * Sets the "name" attribute of the given SpatialComponent_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs SpatialComponent_unsetName() instead.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @param name the string to which the structures "name" attribute should be
 * set.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_ATTRIBUTE_VALUE LIBSBML_INVALID_ATTRIBUTE_VALUE@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_setName(SpatialComponent_t * sc, const char * name);


/**
 * Unsets the value of the "spatialIndex" attribute of the given 
 * SpatialComponent_t structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_unsetSpatialIndex(SpatialComponent_t * sc);


/**
 * Unsets the value of the "variable" attribute of the given 
 * SpatialComponent_t structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_unsetVariable(SpatialComponent_t * sc);


/**
 * Unsets the value of the "id" attribute of the given 
 * SpatialComponent_t structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_unsetId(SpatialComponent_t * sc);


/**
 * Unsets the value of the "name" attribute of the given 
 * SpatialComponent_t structure.
 *
 * @param sc the SpatialComponent_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_unsetName(SpatialComponent_t * sc);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given SpatialComponent_t structure have been set.
 *
 * @param sc the SpatialComponent_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of SpatialComponent_t
 */
LIBSBML_EXTERN
int
SpatialComponent_hasRequiredAttributes(const SpatialComponent_t * sc);


LIBSBML_EXTERN
SpatialComponent_t *
ListOfSpatialComponents_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
SpatialComponent_t *
ListOfSpatialComponents_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SpatialComponent_H__  */

