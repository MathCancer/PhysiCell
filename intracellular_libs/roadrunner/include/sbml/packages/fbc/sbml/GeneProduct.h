/**
 * @file   GeneProduct.h
 * @brief  Implementation of the GeneProduct class
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
 * @class GeneProduct
 * @sbmlbrief{fbc} Association of a gene product with a Reaction
 *
 * In the SBML Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;) package
 * representation format, a GeneProduct object represents a single gene or
 * implied gene product.  The gene or gene product is identified using the
 * required attribute "label", which is a text string.  (This attribute is
 * separate from the usual SBML "id" attribute, which is used to
 * cross-reference entities within an SBML model.)  A GeneProduct object can
 * also possess an optional "associatedSpecies" attribute; if this a
 * attribute is defined, it should have a value of type <code>SIdRef</code>
 * and be the identifier of a Species object defined in the enclosing Model.
 * An "associatedSpecies", if it exists, is interpreted to be a species
 * associated with the gene or gene product that is represented by the
 * GeneProduct object.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 */

#ifndef GeneProduct_H__
#define GeneProduct_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN GeneProduct : public SBase
{

protected:
  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
//  std::string   mName;
  std::string   mLabel;
  std::string   mAssociatedSpecies;
  /** @endcond */


public:

  /**
   * Creates a new GeneProduct with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this GeneProduct.
   *
   * @param version an unsigned int, the SBML Version to assign to this GeneProduct.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to this GeneProduct.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProduct(unsigned int level      = FbcExtension::getDefaultLevel(),
              unsigned int version    = FbcExtension::getDefaultVersion(),
              unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new GeneProduct with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProduct(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for GeneProduct.
   *
   * @param orig the GeneProduct instance to copy.
   */
  GeneProduct(const GeneProduct& orig);


   /**
   * Assignment operator for GeneProduct.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  GeneProduct& operator=(const GeneProduct& rhs);


   /**
   * Creates and returns a deep copy of this GeneProduct object.
   *
   * @return a (deep) copy of this GeneProduct object.
   */
  virtual GeneProduct* clone () const;


   /**
   * Destructor for GeneProduct.
   */
  virtual ~GeneProduct();


  /**
   * Returns the value of the "id" attribute of this GeneProduct.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this GeneProduct.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this GeneProduct object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName() const;


  /**
   * Returns the value of the "label" attribute of this GeneProduct.
   *
   * @return the value of the "label" attribute of this GeneProduct as a
   * string.
   */
  virtual const std::string& getLabel() const;


  /**
   * Returns the value of the "associatedSpecies" attribute of this
   * GeneProduct.
   *
   * @return the value of the "associatedSpecies" attribute of this
   * GeneProduct as a string.
   */
  virtual const std::string& getAssociatedSpecies() const;


  /**
   * Predicate returning @c true if this GeneProduct's "id" attribute is set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true if this GeneProduct's "name" attribute is
   * set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true if this GeneProduct's "label" attribute is
   * set.
   *
   * @return @c true if this GeneProduct's "label" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLabel() const;


  /**
   * Predicate returning @c true if this GeneProduct's "associatedSpecies"
   * attribute is set.
   *
   * @return @c true if this GeneProduct's "associatedSpecies" attribute has
   * been set, otherwise @c false is returned.
   */
  virtual bool isSetAssociatedSpecies() const;


  /**
   * Sets the value of the "id" attribute of this GeneProduct.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Sets the value of the "name" attribute of this GeneProduct.
   *
   * @copydetails doc_set_name
   */
  virtual int setName(const std::string& name);


  /**
   * Sets the value of the "label" attribute of this GeneProduct.
   *
   * @param label the value of the "label" attribute to be
   * set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setLabel(const std::string& label);


  /**
   * Sets the value of the "associatedSpecies" attribute of this GeneProduct.
   *
   * @param associatedSpecies the value of the
   * "associatedSpecies" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setAssociatedSpecies(const std::string& associatedSpecies);


  /**
   * Unsets the value of the "id" attribute of this GeneProduct.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this GeneProduct.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName();


  /**
   * Unsets the value of the "label" attribute of this GeneProduct.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetLabel();


  /**
   * Unsets the value of the "associatedSpecies" attribute of this
   * GeneProduct.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAssociatedSpecies();


  /**
   * @copydoc doc_renamesidref_common
   */
   virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the XML element name of this object.
   *
   * For GeneProduct, the XML element name is always @c "geneProduct".
   *
   * @return the name of this element, i.e. @c "geneProduct".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_GENEPRODUCT, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this GeneProduct object have been set.
   *
   * @note The required attributes for a GeneProduct object are:
   * @li "id"
   * @li "label"
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


/**
 * @class ListOfGeneProducts
 * @sbmlbrief{fbc} A list of GeneProduct objects.
 *
 * The ListOfGeneProducts is a container for the SBML extended Model
 * that lists all the possible GeneProduct elements in the model.  These
 * GeneProduct elements are referenced in the GeneProductAssociation child
 * of an extended Reaction.
 *
 * @copydetails doc_what_is_listof
 *
 * @see GeneProduct
 * @see FbcModelPlugin
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 */
class LIBSBML_EXTERN ListOfGeneProducts : public ListOf
{

public:

  /**
   * Creates a new GeneProduct with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * ListOfGeneProducts.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * ListOfGeneProducts.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this ListOfGeneProducts.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfGeneProducts(unsigned int level      = FbcExtension::getDefaultLevel(),
                     unsigned int version    = FbcExtension::getDefaultVersion(),
                     unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfGeneProducts with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfGeneProducts(FbcPkgNamespaces* fbcns);


   /**
   * Creates and returns a deep copy of this ListOfGeneProducts object.
   *
   * @return a (deep) copy of this ListOfGeneProducts object.
   */
  virtual ListOfGeneProducts* clone () const;


   /**
   * Get a GeneProduct from the ListOfGeneProducts.
   *
   * @param n the index number of the GeneProduct to get.
   *
   * @return the nth GeneProduct in this ListOfGeneProducts.
   *
   * @see size()
   */
  virtual GeneProduct* get(unsigned int n);


  /**
   * Get a GeneProduct from the ListOfGeneProducts.
   *
   * @param n the index number of the GeneProduct to get.
   *
   * @return the nth GeneProduct in this ListOfGeneProducts.
   *
   * @see size()
   */
  virtual const GeneProduct* get(unsigned int n) const;


  /**
   * Get a GeneProduct from the ListOfGeneProducts based on its identifier.
   *
   * @param sid a string representing the identifier of the GeneProduct to
   * get.
   *
   * @return GeneProduct in this ListOfGeneProducts with the given id or NULL
   * if no such GeneProduct exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual GeneProduct* get(const std::string& sid);


  /**
   * Get a GeneProduct from the ListOfGeneProducts based on its identifier.
   *
   * @param sid a string representing the identifier of the GeneProduct to
   * get.
   *
   * @return GeneProduct in this ListOfGeneProducts with the given id or NULL
   * if no such GeneProduct exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const GeneProduct* get(const std::string& sid) const;


  /**
   * Adds a copy the given GeneProduct to this ListOfGeneProducts.
   *
   * @param gp the GeneProduct object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int addGeneProduct(const GeneProduct* gp);


  /**
   * Get the number of GeneProduct objects in this ListOfGeneProducts.
   *
   * @return the number of GeneProduct objects in this ListOfGeneProducts.
   */
  unsigned int getNumGeneProducts() const;


  /**
   * Creates a new GeneProduct object, adds it to the ListOfGeneProducts and
   * returns the GeneProduct object created.
   *
   * @return a new GeneProduct object instance.
   *
   * @see addGeneProduct(const GeneProduct* gp)
   */
  GeneProduct* createGeneProduct();


  /**
   * Removes the nth GeneProduct from this ListOfGeneProducts
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the GeneProduct to remove.
   *
   * @see size()
   */
  virtual GeneProduct* remove(unsigned int n);


  /**
   * Removes the GeneProduct from this ListOfGeneProducts with the given
   * identifier and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the GeneProduct to remove.
   *
   * @return the GeneProduct removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual GeneProduct* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object.
   *
   * For ListOfGeneProducts, the XML element name is always @c
   * "listOfGeneProducts".
   *
   * @return the name of this element, i.e. @c "listOfGeneProducts".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LIST_OF, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Returns the libSBML type code for the objects contained in this ListOf.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for the objects contained in this ListOf
   * instance: @sbmlconstant{SBML_FBC_GENEPRODUCT, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new GeneProduct in this ListOfGeneProducts
   */
  virtual SBase* createObject(XMLInputStream& stream);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the &ldquo;fbc&rdquo; package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;

  /** @endcond */

};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new GeneProduct_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * GeneProduct_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * GeneProduct_t structure.
 *
 * @returns the newly-created GeneProduct_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof GeneProduct_t
 */
LIBSBML_EXTERN
GeneProduct_t *
GeneProduct_create(unsigned int level, unsigned int version,
                   unsigned int pkgVersion);


/**
 * Frees the given GeneProduct_t structure.
 * 
 * @param gp the GeneProduct_t structure to be freed.
 *
 * @memberof GeneProduct_t
 */
LIBSBML_EXTERN
void
GeneProduct_free(GeneProduct_t * gp);


/**
 * Creates a deep copy of the given GeneProduct_t structure.
 * 
 * @param gp the GeneProduct_t structure to be copied.
 *
 * @returns a (deep) copy of the given GeneProduct_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof GeneProduct_t
 */
LIBSBML_EXTERN
GeneProduct_t *
GeneProduct_clone(GeneProduct_t * gp);


/**
 * Returns the value of the "id" attribute of the given GeneProduct_t
 * structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return the id of this structure.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
const char *
GeneProduct_getId(const GeneProduct_t * gp);


/**
 * Returns the value of the "name" attribute of the given GeneProduct_t
 * structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return the name of this structure.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
const char *
GeneProduct_getName(const GeneProduct_t * gp);


/**
 * Returns the value of the "label" attribute of the given GeneProduct_t
 * structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return the label of this structure.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
const char *
GeneProduct_getLabel(const GeneProduct_t * gp);


/**
 * Returns the value of the "associatedSpecies" attribute of the given GeneProduct_t
 * structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return the associatedSpecies of this structure.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
const char *
GeneProduct_getAssociatedSpecies(const GeneProduct_t * gp);


/**
 * Predicate returning @c 1 if the given GeneProduct_t structure's "id"
 * is set.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return @c 1 if the "id" of this GeneProduct_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_isSetId(const GeneProduct_t * gp);


/**
 * Predicate returning @c 1 if the given GeneProduct_t structure's "name"
 * is set.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return @c 1 if the "name" of this GeneProduct_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_isSetName(const GeneProduct_t * gp);


/**
 * Predicate returning @c 1 if the given GeneProduct_t structure's "label"
 * is set.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return @c 1 if the "label" of this GeneProduct_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_isSetLabel(const GeneProduct_t * gp);


/**
 * Predicate returning @c 1 if the given GeneProduct_t structure's "associatedSpecies"
 * is set.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @return @c 1 if the "associatedSpecies" of this GeneProduct_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_isSetAssociatedSpecies(const GeneProduct_t * gp);


/**
 * Sets the "id" attribute of the given GeneProduct_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProduct_unsetId() instead.
 *
 * @param gp the GeneProduct_t structure.
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
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_setId(GeneProduct_t * gp, const char * id);


/**
 * Sets the "name" attribute of the given GeneProduct_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProduct_unsetName() instead.
 *
 * @param gp the GeneProduct_t structure.
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
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_setName(GeneProduct_t * gp, const char * name);


/**
 * Sets the "label" attribute of the given GeneProduct_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProduct_unsetLabel() instead.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @param label the string to which the structures "label" attribute should be
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
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_setLabel(GeneProduct_t * gp, const char * label);


/**
 * Sets the "associatedSpecies" attribute of the given GeneProduct_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProduct_unsetAssociatedSpecies() instead.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @param associatedSpecies the string to which the structures "associatedSpecies" attribute should be
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
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_setAssociatedSpecies(GeneProduct_t * gp, const char * associatedSpecies);


/**
 * Unsets the value of the "id" attribute of the given 
 * GeneProduct_t structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_unsetId(GeneProduct_t * gp);


/**
 * Unsets the value of the "name" attribute of the given 
 * GeneProduct_t structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_unsetName(GeneProduct_t * gp);


/**
 * Unsets the value of the "label" attribute of the given 
 * GeneProduct_t structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_unsetLabel(GeneProduct_t * gp);


/**
 * Unsets the value of the "associatedSpecies" attribute of the given 
 * GeneProduct_t structure.
 *
 * @param gp the GeneProduct_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_unsetAssociatedSpecies(GeneProduct_t * gp);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given GeneProduct_t structure have been set.
 *
 * @param gp the GeneProduct_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of GeneProduct_t
 */
LIBSBML_EXTERN
int
GeneProduct_hasRequiredAttributes(const GeneProduct_t * gp);


LIBSBML_EXTERN
GeneProduct_t *
ListOfGeneProducts_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
GeneProduct_t *
ListOfGeneProducts_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  GeneProduct_H__  */

