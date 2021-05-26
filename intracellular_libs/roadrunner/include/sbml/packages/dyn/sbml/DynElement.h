/**
 * @file:   DynElement.h
 * @brief:  Implementation of the DynElement class
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


#ifndef DynElement_H__
#define DynElement_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/dyn/common/dynfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/dyn/extension/DynExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN DynElement : public SBase
{

protected:

  std::string   mIdRef;
////  std::string   mId;
////  std::string   mName;
  std::string   mMetaIdRef;


public:

  /**
   * Creates a new DynElement with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this DynElement
   *
   * @param version an unsigned int, the SBML Version to assign to this DynElement
   *
   * @param pkgVersion an unsigned int, the SBML Dyn Version to assign to this DynElement
   */
  DynElement(unsigned int level      = DynExtension::getDefaultLevel(),
             unsigned int version    = DynExtension::getDefaultVersion(),
             unsigned int pkgVersion = DynExtension::getDefaultPackageVersion());


  /**
   * Creates a new DynElement with the given DynPkgNamespaces object.
   *
   * @param dynns the DynPkgNamespaces object
   */
  DynElement(DynPkgNamespaces* dynns);


   /**
   * Copy constructor for DynElement.
   *
   * @param orig; the DynElement instance to copy.
   */
  DynElement(const DynElement& orig);


   /**
   * Assignment operator for DynElement.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DynElement& operator=(const DynElement& rhs);


   /**
   * Creates and returns a deep copy of this DynElement object.
   *
   * @return a (deep) copy of this DynElement object.
   */
  virtual DynElement* clone () const;


   /**
   * Destructor for DynElement.
   */
  virtual ~DynElement();


   /**
   * Returns the value of the "idRef" attribute of this DynElement.
   *
   * @return the value of the "idRef" attribute of this DynElement as a string.
   */
  virtual const std::string& getIdRef() const;


  /**
   * Returns the value of the "id" attribute of this DynElement.
   *
   * @return the value of the "id" attribute of this DynElement as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this DynElement.
   *
   * @return the value of the "name" attribute of this DynElement as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Returns the value of the "metaIdRef" attribute of this DynElement.
   *
   * @return the value of the "metaIdRef" attribute of this DynElement as a string.
   */
  virtual const std::string& getMetaIdRef() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynElement's "idRef" attribute has been set.
   *
   * @return @c true if this DynElement's "idRef" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetIdRef() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynElement's "id" attribute has been set.
   *
   * @return @c true if this DynElement's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynElement's "name" attribute has been set.
   *
   * @return @c true if this DynElement's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynElement's "metaIdRef" attribute has been set.
   *
   * @return @c true if this DynElement's "metaIdRef" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetMetaIdRef() const;


  /**
   * Sets the value of the "idRef" attribute of this DynElement.
   *
   * @param idRef; const std::string& value of the "idRef" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setIdRef(const std::string& idRef);


  /**
   * Sets the value of the "id" attribute of this DynElement.
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
   * Sets the value of the "name" attribute of this DynElement.
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
   * Sets the value of the "metaIdRef" attribute of this DynElement.
   *
   * @param metaIdRef; const std::string& value of the "metaIdRef" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setMetaIdRef(const std::string& metaIdRef);


  /**
   * Unsets the value of the "idRef" attribute of this DynElement.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetIdRef();


  /**
   * Unsets the value of the "id" attribute of this DynElement.
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
   * Unsets the value of the "name" attribute of this DynElement.
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
   * Unsets the value of the "metaIdRef" attribute of this DynElement.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetMetaIdRef();


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
   * Returns the XML element name of this object, which for DynElement, is
   * always @c "dynElement".
   *
   * @return the name of this element, i.e. @c "dynElement".
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
   * for this DynElement object have been set.
   *
   * @note The required attributes for a DynElement object are:
   * @li "idRef"
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

class LIBSBML_EXTERN ListOfDynElements : public ListOf
{

public:

  /**
   * Creates a new ListOfDynElements with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfDynElements
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfDynElements
   *
   * @param pkgVersion an unsigned int, the SBML Dyn Version to assign to this ListOfDynElements
   */
  ListOfDynElements(unsigned int level      = DynExtension::getDefaultLevel(),
                    unsigned int version    = DynExtension::getDefaultVersion(),
                    unsigned int pkgVersion = DynExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfDynElements with the given DynPkgNamespaces object.
   *
   * @param dynns the DynPkgNamespaces object
   */
  ListOfDynElements(DynPkgNamespaces* dynns);


   /**
   * Creates and returns a deep copy of this ListOfDynElements object.
   *
   * @return a (deep) copy of this ListOfDynElements object.
   */
  virtual ListOfDynElements* clone () const;


   /**
   * Get a DynElement from the ListOfDynElements.
   *
   * @param n the index number of the DynElement to get.
   *
   * @return the nth DynElement in this ListOfDynElements.
   *
   * @see size()
   */
	virtual DynElement* get(unsigned int n);


  /**
   * Get a DynElement from the ListOfDynElements.
   *
   * @param n the index number of the DynElement to get.
   *
   * @return the nth DynElement in this ListOfDynElements.
   *
   * @see size()
   */
	virtual const DynElement* get(unsigned int n) const;


  /**
   * Get a DynElement from the ListOfDynElements
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DynElement to get.
   *
   * @return DynElement in this ListOfDynElements
   * with the given id or NULL if no such
   * DynElement exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
	virtual DynElement* get(const std::string& sid);


  /**
   * Get a DynElement from the ListOfDynElements
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DynElement to get.
   *
   * @return DynElement in this ListOfDynElements
   * with the given id or NULL if no such
   * DynElement exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const DynElement* get(const std::string& sid) const;


	/**
	 * Adds a copy the given "DynElement" to this ListOfDynElements.
	 *
	 * @param de; the DynElement object to add
	 *
	 * @return integer value indicating success/failure of the
	 * function.  @if clike The value is drawn from the
	 * enumeration #OperationReturnValues_t. @endif The possible values
	 * returned by this function are:
	 * @li LIBSEDML_OPERATION_SUCCESS
	 * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
	 */
	int addDynElement(const DynElement* de);


	/**
	 * Get the number of DynElement objects in this ListOfDynElements.
	 *
	 * @return the number of DynElement objects in this ListOfDynElements
	 */
	unsigned int getNumDynElements() const;


	/**
	 * Creates a new DynElement object, adds it to the
	 * ListOfDynElements and returns the DynElement object created. 
	 *
	 * @return a new DynElement object instance
	 *
	 * @see addDynElement(const DynElement* de)
	 */
	DynElement* createDynElement();


  /**
   * Removes the nth DynElement from this ListOfDynElements
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DynElement to remove.
   *
   * @see size()
   */
	virtual DynElement* remove(unsigned int n);


  /**
   * Removes the DynElement from this ListOfDynElements with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the DynElement to remove.
   *
   * @return the DynElement removed. As mentioned above, the caller owns the
   * returned item.
   */
	virtual DynElement* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfDynElements, is
   * always @c "listOfDynElements".
   *
   * @return the name of this element, i.e. @c "listOfDynElements".
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
   * Creates a new DynElement in this ListOfDynElements
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
 * Creates a new DynElement_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * DynElement_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * DynElement_t structure.
 *
 * @returns the newly-created DynElement_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof DynElement_t
 */
LIBSBML_EXTERN
DynElement_t *
DynElement_create(unsigned int level, unsigned int version,
                  unsigned int pkgVersion);


/**
 * Frees the given DynElement_t structure.
 * 
 * @param de the DynElement_t structure to be freed.
 *
 * @memberof DynElement_t
 */
LIBSBML_EXTERN
void
DynElement_free(DynElement_t * de);


/**
 * Creates a deep copy of the given DynElement_t structure.
 * 
 * @param de the DynElement_t structure to be copied.
 *
 * @returns a (deep) copy of the given DynElement_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof DynElement_t
 */
LIBSBML_EXTERN
DynElement_t *
DynElement_clone(DynElement_t * de);


/**
 * Returns the value of the "idRef" attribute of the given DynElement_t
 * structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return the idRef of this structure.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
const char *
DynElement_getIdRef(const DynElement_t * de);


/**
 * Returns the value of the "id" attribute of the given DynElement_t
 * structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return the id of this structure.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
const char *
DynElement_getId(const DynElement_t * de);


/**
 * Returns the value of the "name" attribute of the given DynElement_t
 * structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return the name of this structure.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
const char *
DynElement_getName(const DynElement_t * de);


/**
 * Returns the value of the "metaIdRef" attribute of the given DynElement_t
 * structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return the metaIdRef of this structure.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
const char *
DynElement_getMetaIdRef(const DynElement_t * de);


/**
 * Predicate returning @c 1 if the given DynElement_t structure's "idRef"
 * is set.
 *
 * @param de the DynElement_t structure.
 *
 * @return @c 1 if the "idRef" of this DynElement_t structure is
 * set, @c 0 otherwise.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_isSetIdRef(const DynElement_t * de);


/**
 * Predicate returning @c 1 if the given DynElement_t structure's "id"
 * is set.
 *
 * @param de the DynElement_t structure.
 *
 * @return @c 1 if the "id" of this DynElement_t structure is
 * set, @c 0 otherwise.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_isSetId(const DynElement_t * de);


/**
 * Predicate returning @c 1 if the given DynElement_t structure's "name"
 * is set.
 *
 * @param de the DynElement_t structure.
 *
 * @return @c 1 if the "name" of this DynElement_t structure is
 * set, @c 0 otherwise.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_isSetName(const DynElement_t * de);


/**
 * Predicate returning @c 1 if the given DynElement_t structure's "metaIdRef"
 * is set.
 *
 * @param de the DynElement_t structure.
 *
 * @return @c 1 if the "metaIdRef" of this DynElement_t structure is
 * set, @c 0 otherwise.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_isSetMetaIdRef(const DynElement_t * de);


/**
 * Sets the "idRef" attribute of the given DynElement_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs DynElement_unsetIdRef() instead.
 *
 * @param de the DynElement_t structure.
 *
 * @param idRef the string to which the structures "idRef" attribute should be
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
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_setIdRef(DynElement_t * de, const char * idRef);


/**
 * Sets the "id" attribute of the given DynElement_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs DynElement_unsetId() instead.
 *
 * @param de the DynElement_t structure.
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
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_setId(DynElement_t * de, const char * id);


/**
 * Sets the "name" attribute of the given DynElement_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs DynElement_unsetName() instead.
 *
 * @param de the DynElement_t structure.
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
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_setName(DynElement_t * de, const char * name);


/**
 * Sets the "metaIdRef" attribute of the given DynElement_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs DynElement_unsetMetaIdRef() instead.
 *
 * @param de the DynElement_t structure.
 *
 * @param metaIdRef the string to which the structures "metaIdRef" attribute should be
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
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_setMetaIdRef(DynElement_t * de, const char * metaIdRef);


/**
 * Unsets the value of the "idRef" attribute of the given 
 * DynElement_t structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_unsetIdRef(DynElement_t * de);


/**
 * Unsets the value of the "id" attribute of the given 
 * DynElement_t structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_unsetId(DynElement_t * de);


/**
 * Unsets the value of the "name" attribute of the given 
 * DynElement_t structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_unsetName(DynElement_t * de);


/**
 * Unsets the value of the "metaIdRef" attribute of the given 
 * DynElement_t structure.
 *
 * @param de the DynElement_t structure.
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif@~ The possible values
 * returned by this function are:
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
 * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
 * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_unsetMetaIdRef(DynElement_t * de);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given DynElement_t structure have been set.
 *
 * @param de the DynElement_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of DynElement_t
 */
LIBSBML_EXTERN
int
DynElement_hasRequiredAttributes(const DynElement_t * de);


LIBSBML_EXTERN
DynElement_t *
ListOfDynElements_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
DynElement_t *
ListOfDynElements_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  DynElement_H__  */

