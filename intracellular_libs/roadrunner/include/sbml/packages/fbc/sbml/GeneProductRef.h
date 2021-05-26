/**
 * @file   GeneProductRef.h
 * @brief  Implementation of the GeneProductRef class
 * @author SBMLTeam
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
 *
 * @class GeneProductRef
 * @sbmlbrief{fbc} Reference to a gene product of a reaction
 *
 * GeneProductRef encodes a references to a GeneProduct object declared in a
 * ListOfGeneProducts attached to the enclosing Model object.  GeneProductRef
 * is derived from the FbcAssociation class.
 *
 * GeneProductRef objects are used to construct the content of
 * GeneProductAssociation objects.  As described more fully in the
 * documentation of the latter, a GeneProductAssociation is a container that
 * may contain either (i) a single GeneProductRef or (ii) a logical
 * expression built up with FbcAnd and FbcOr and ultimately containing two
 * or more GeneProductRef objects.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 */

#ifndef GeneProductRef_H__
#define GeneProductRef_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>
#include <sbml/packages/fbc/sbml/FbcAssociation.h>


LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN GeneProductRef : public FbcAssociation
{

protected:

  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
  std::string   mGeneProduct;
//  std::string   mName;
  /** @endcond */


public:

  /**
   * Creates a new GeneProductRef with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * GeneProductRef.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * GeneProductRef.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this GeneProductRef.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProductRef(unsigned int level      = FbcExtension::getDefaultLevel(),
                 unsigned int version    = FbcExtension::getDefaultVersion(),
                 unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new GeneProductRef with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProductRef(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for GeneProductRef.
   *
   * @param orig the GeneProductRef instance to copy.
   */
  GeneProductRef(const GeneProductRef& orig);


   /**
   * Assignment operator for GeneProductRef.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  GeneProductRef& operator=(const GeneProductRef& rhs);


   /**
   * Creates and returns a deep copy of this GeneProductRef object.
   *
   * @return a (deep) copy of this GeneProductRef object.
   */
  virtual GeneProductRef* clone () const;


   /**
   * Destructor for GeneProductRef.
   */
  virtual ~GeneProductRef();


  /**
   * Returns the value of the "id" attribute of this GeneProductRef.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this GeneProductRef.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "geneProduct" attribute of this GeneProductRef.
   *
   * @return the value of the "geneProduct" attribute of this GeneProductRef as a string.
   */
  virtual const std::string& getGeneProduct() const;

  /**
  * Converts this FbcAssociation object into an infix string representation.
  *
  * @return the association as infix string.
  */
  virtual std::string toInfix() const;

  /**
   * Returns the value of the "name" attribute of this GeneProductRef object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true if this GeneProductRef's "id" attribute is
   * set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true if this GeneProductRef's "geneProduct"
   * attribute is set.
   *
   * @return @c true if this GeneProductRef's "geneProduct" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetGeneProduct() const;


  /**
   * Predicate returning @c true if this GeneProductRef's "name" attribute is
   * set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "id" attribute of this GeneProductRef.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Sets the value of the "geneProduct" attribute of this GeneProductRef.
   *
   * @param geneProduct the value of the "geneProduct" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setGeneProduct(const std::string& geneProduct);


  /**
   * Sets the value of the "name" attribute of this GeneProductRef.
   *
   * @copydetails doc_set_name
   */
  virtual int setName(const std::string& name);


  /**
   * Unsets the value of the "id" attribute of this GeneProductRef.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "geneProduct" attribute of this GeneProductRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetGeneProduct();


  /**
   * Unsets the value of the "name" attribute of this GeneProductRef.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName();


  /**
   * @copydoc doc_renamesidref_common
   */
   virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the XML element name of this object.
   *
   * For GeneProductRef, the XML element name is always @c "geneProductRef".
   *
   * @return the name of this element, i.e. @c "geneProductRef".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_GENEPRODUCTREF, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes for this
   * GeneProductRef object have been set.
   *
   * @note The required attributes for a GeneProductRef object are:
   * @li "geneProduct"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);


  /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * return the SBML object corresponding to next XMLToken.
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond */



};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new GeneProductRef_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * GeneProductRef_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * GeneProductRef_t structure.
 *
 * @returns the newly-created GeneProductRef_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof GeneProductRef_t
 */
LIBSBML_EXTERN
GeneProductRef_t *
GeneProductRef_create(unsigned int level, unsigned int version,
                      unsigned int pkgVersion);


/**
 * Frees the given GeneProductRef_t structure.
 * 
 * @param gpr the GeneProductRef_t structure to be freed.
 *
 * @memberof GeneProductRef_t
 */
LIBSBML_EXTERN
void
GeneProductRef_free(GeneProductRef_t * gpr);


/**
 * Creates a deep copy of the given GeneProductRef_t structure.
 * 
 * @param gpr the GeneProductRef_t structure to be copied.
 *
 * @returns a (deep) copy of the given GeneProductRef_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof GeneProductRef_t
 */
LIBSBML_EXTERN
GeneProductRef_t *
GeneProductRef_clone(GeneProductRef_t * gpr);


/**
 * Returns the value of the "id" attribute of the given GeneProductRef_t
 * structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return the id of this structure.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
const char *
GeneProductRef_getId(const GeneProductRef_t * gpr);


/**
 * Returns the value of the "geneProduct" attribute of the given GeneProductRef_t
 * structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return the geneProduct of this structure.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
const char *
GeneProductRef_getGeneProduct(const GeneProductRef_t * gpr);


/**
 * Returns the value of the "name" attribute of the given GeneProductRef_t
 * structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return the name of this structure.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
const char *
GeneProductRef_getName(const GeneProductRef_t * gpr);


/**
 * Predicate returning @c 1 if the given GeneProductRef_t structure's "id"
 * is set.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return @c 1 if the "id" of this GeneProductRef_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_isSetId(const GeneProductRef_t * gpr);


/**
 * Predicate returning @c 1 if the given GeneProductRef_t structure's "geneProduct"
 * is set.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return @c 1 if the "geneProduct" of this GeneProductRef_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_isSetGeneProduct(const GeneProductRef_t * gpr);


/**
 * Predicate returning @c 1 if the given GeneProductRef_t structure's "name"
 * is set.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @return @c 1 if the "name" of this GeneProductRef_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_isSetName(const GeneProductRef_t * gpr);


/**
 * Sets the "id" attribute of the given GeneProductRef_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProductRef_unsetId() instead.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @param id the string to which the structures "id" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_setId(GeneProductRef_t * gpr, const char * id);


/**
 * Sets the "geneProduct" attribute of the given GeneProductRef_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProductRef_unsetGeneProduct() instead.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @param geneProduct the string to which the structures "geneProduct" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_setGeneProduct(GeneProductRef_t * gpr, const char * geneProduct);


/**
 * Sets the "name" attribute of the given GeneProductRef_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProductRef_unsetName() instead.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @param name the string to which the structures "name" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_setName(GeneProductRef_t * gpr, const char * name);


/**
 * Unsets the value of the "id" attribute of the given 
 * GeneProductRef_t structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_unsetId(GeneProductRef_t * gpr);


/**
 * Unsets the value of the "geneProduct" attribute of the given 
 * GeneProductRef_t structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_unsetGeneProduct(GeneProductRef_t * gpr);


/**
 * Unsets the value of the "name" attribute of the given 
 * GeneProductRef_t structure.
 *
 * @param gpr the GeneProductRef_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_unsetName(GeneProductRef_t * gpr);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given GeneProductRef_t structure have been set.
 *
 * @param gpr the GeneProductRef_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of GeneProductRef_t
 */
LIBSBML_EXTERN
int
GeneProductRef_hasRequiredAttributes(const GeneProductRef_t * gpr);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  GeneProductRef_H__  */

