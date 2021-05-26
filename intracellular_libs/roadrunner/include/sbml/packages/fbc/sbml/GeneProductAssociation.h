/**
 * @file   GeneProductAssociation.h
 * @brief  Implementation of the GeneProductAssociation class
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
 * @class GeneProductAssociation
 * @sbmlbrief{fbc} Association between gene products and reactions
 *
 * In Version&nbsp;2 of the SBML Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;)
 * package specification, GeneProductAssociation is a class derived from
 * SBase used to associate one more genes or gene products with reactions.
 * GeneProductAssociation objects are essentially containers, with one such
 * container optionally attached as a subelement to a Reaction object in a
 * model.  The container can contain one of three kinds of objects, all of
 * which are subclasses of the libSBML parent class FbcAssociation.  (Note
 * that this class is named <em>%Association</em> in the &ldquo;fbc&rdquo;
 * Version&nbsp;2 specification, but in libSBML is named FbcAssociation to
 * avoid a name conflict with an existing class.)
 *
 * One of the kinds of FbcAssociation subclasses that can appear in a
 * GeneProductAssociation is GeneProductRef.  This class of objects
 * references a GeneProduct declared in a ListOfGeneProducts attached to the
 * enclosing Model object.  In the &ldquo;fbc&rdquo; approach, when more than
 * one gene (or gene product) is present in an association, they are written
 * as logical expressions using Boolean logical operators <em>and</em> and
 * <em>or</em> through the classes (in libSBML) FbcAnd and FbcOr.  (In the
 * &ldquo;fbc&rdquo; Version&nbsp;2 specification, these are simply named
 * <em>And</em> and <em>Or</em>, respectively.) The FbcAnd and FbcOr objects
 * in turn can contain either GeneProductRef objects or other FbcAnd and/or
 * FbcOr objects.
 *
 * Here is a concrete example of what the XML representation of a simple
 * <em>or</em> relationship might look like:
<pre class="fragment">
&lt;reaction id = "R_ABTA" <span class="regular-text" style="font-weight: normal; font-style: italic; background-color: #ffd; padding-left: 2px; padding-right: 2px">... rest of Reaction declaration elided for this example ...</span>&gt;
 &lt;fbc:geneProductAssociation fbc:id="ga_16"&gt;
  &lt;fbc:or&gt;
   &lt;fbc:geneProductRef fbc:geneProduct="g_b2662"/&gt;
   &lt;fbc:geneProductRef fbc:geneProduct="g_b1302"/&gt;
  &lt;/fbc:or&gt;
 &lt;/fbc:geneProductAssociation&gt;
&lt;/reaction&gt;
</pre>
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 */

#ifndef GeneProductAssociation_H__
#define GeneProductAssociation_H__


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



class LIBSBML_EXTERN GeneProductAssociation : public SBase
{

protected:

  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
//  std::string   mName;
  FbcAssociation*      mAssociation;
  /** @endcond */


public:

  /**
   * Creates a new GeneProductAssociation with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo; package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * GeneProductAssociation.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * GeneProductAssociation.
   *
   * @param pkgVersion an unsigned int, the SBML &ldquo;fbc&rdquo; Version to
   * assign to this GeneProductAssociation object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProductAssociation(unsigned int level      = FbcExtension::getDefaultLevel(),
                         unsigned int version    = FbcExtension::getDefaultVersion(),
                         unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new GeneProductAssociation with the given FbcPkgNamespaces
   * object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  GeneProductAssociation(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for GeneProductAssociation.
   *
   * @param orig the GeneProductAssociation instance to copy.
   */
  GeneProductAssociation(const GeneProductAssociation& orig);


   /**
   * Assignment operator for GeneProductAssociation.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  GeneProductAssociation& operator=(const GeneProductAssociation& rhs);


   /**
   * Creates and returns a deep copy of this GeneProductAssociation object.
   *
   * @return a (deep) copy of this GeneProductAssociation object.
   */
  virtual GeneProductAssociation* clone () const;


   /**
   * Destructor for GeneProductAssociation.
   */
  virtual ~GeneProductAssociation();


  /**
   * Returns the value of the "id" attribute of this GeneProductAssociation.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this GeneProductAssociation.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this GeneProductAssociation object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName() const;


  /**
   * Returns the "association" element of this GeneProductAssociation.
   *
   * @return the "association" element of this GeneProductAssociation.
   */
  virtual const FbcAssociation* getAssociation() const;


  /**
   * Returns the "association" element of this GeneProductAssociation.
   *
   * @return the "association" element of this GeneProductAssociation.
   */
  virtual FbcAssociation* getAssociation();


  /**
   * Creates a new "association" and sets it for this GeneProductAssociation.
   *
   * @return the FbcAnd created.
   */
  virtual FbcAnd* createAnd();


  /**
   * Creates a new "association" and sets it for this GeneProductAssociation.
   */
  virtual FbcOr* createOr();


  /**
   * Creates a new "association" and sets it for this GeneProductAssociation.
   */
  virtual GeneProductRef* createGeneProductRef();


  /**
   * Predicate returning @c true if this GeneProductAssociation's "id"
   * attribute is set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true if this GeneProductAssociation's "name"
   * attribute is set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true if this GeneProductAssociation's
   * "association" element is set.
   *
   * @return @c true if this GeneProductAssociation's "association" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetAssociation() const;


  /**
   * Sets the value of the "id" attribute of this GeneProductAssociation.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Sets the value of the "name" attribute of this GeneProductAssociation.
   *
   * @copydetails doc_set_name
   */
  virtual int setName(const std::string& name);


  /**
   * Sets the "association" element of this GeneProductAssociation.
   *
   * @param association FbcAssociation to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setAssociation(FbcAssociation* association);

  /**
  * Sets the "association" element of this GeneProductAssociation.
  *
  * @param association string representation of the association to be set.
  *
   * @copydetails doc_returns_success_code
  * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
  * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
  */
  virtual int setAssociation(const std::string& association);

  /**
   * Unsets the value of the "id" attribute of this GeneProductAssociation.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this GeneProductAssociation.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName();


  /**
   * Unsets the "association" element of this GeneProductAssociation.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAssociation();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object.
   *
   * For GeneProductAssociation, the XML element name is always @c "geneProductAssociation".
   *
   * @return the name of this element, i.e. @c "geneProductAssociation".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_GENEPRODUCTASSOCIATION, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
 */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes for this
   * GeneProductAssociation object have been set.
   *
   * @note The required attributes for a GeneProductAssociation object are:
   * @li "association"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements for this
   * GeneProductAssociation object have been set.
   *
   * @note The required elements for a GeneProductAssociation object are:
   * @li "association"
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  virtual bool hasRequiredElements() const;


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
   * Connects to child elements.
   */
  virtual void connectToChild ();


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
 * Creates a new GeneProductAssociation_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * GeneProductAssociation_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * GeneProductAssociation_t structure.
 *
 * @returns the newly-created GeneProductAssociation_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof GeneProductAssociation_t
 */
LIBSBML_EXTERN
GeneProductAssociation_t *
GeneProductAssociation_create(unsigned int level, unsigned int version,
                              unsigned int pkgVersion);


/**
 * Frees the given GeneProductAssociation_t structure.
 * 
 * @param gpa the GeneProductAssociation_t structure to be freed.
 *
 * @memberof GeneProductAssociation_t
 */
LIBSBML_EXTERN
void
GeneProductAssociation_free(GeneProductAssociation_t * gpa);


/**
 * Creates a deep copy of the given GeneProductAssociation_t structure.
 * 
 * @param gpa the GeneProductAssociation_t structure to be copied.
 *
 * @returns a (deep) copy of the given GeneProductAssociation_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof GeneProductAssociation_t
 */
LIBSBML_EXTERN
GeneProductAssociation_t *
GeneProductAssociation_clone(GeneProductAssociation_t * gpa);


/**
 * Returns the value of the "id" attribute of the given GeneProductAssociation_t
 * structure.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @return the id of this structure.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
const char *
GeneProductAssociation_getId(const GeneProductAssociation_t * gpa);


/**
 * Returns the value of the "name" attribute of the given GeneProductAssociation_t
 * structure.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @return the name of this structure.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
const char *
GeneProductAssociation_getName(const GeneProductAssociation_t * gpa);


LIBSBML_EXTERN
FbcAssociation_t*
GeneProductAssociation_getAssociation(GeneProductAssociation_t * gpa);


LIBSBML_EXTERN
FbcAnd_t *
GeneProductAssociation_createAnd(GeneProductAssociation_t * gpa);


LIBSBML_EXTERN
FbcOr_t *
GeneProductAssociation_createOr(GeneProductAssociation_t * gpa);


LIBSBML_EXTERN
GeneProductRef_t *
GeneProductAssociation_createGeneProductRef(GeneProductAssociation_t * gpa);


/**
 * Predicate returning @c 1 if the given GeneProductAssociation_t structure's
 * "id" is set.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @return @c 1 if the "id" of this GeneProductAssociation_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_isSetId(const GeneProductAssociation_t * gpa);


/**
 * Predicate returning @c 1 if the given GeneProductAssociation_t structure's "name"
 * is set.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @return @c 1 if the "name" of this GeneProductAssociation_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_isSetName(const GeneProductAssociation_t * gpa);


/**
 * Predicate returning @c 1 if the given GeneProductAssociation_t structure's "association"
 * is set.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @return @c 1 if the "association" of this GeneProductAssociation_t structure is
 * set, @c 0 otherwise.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_isSetAssociation(const GeneProductAssociation_t * gpa);


/**
 * Sets the "id" attribute of the given GeneProductAssociation_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProductAssociation_unsetId() instead.
 *
 * @param gpa the GeneProductAssociation_t structure.
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
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_setId(GeneProductAssociation_t * gpa, const char * id);


/**
 * Sets the "name" attribute of the given GeneProductAssociation_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs GeneProductAssociation_unsetName() instead.
 *
 * @param gpa the GeneProductAssociation_t structure.
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
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_setName(GeneProductAssociation_t * gpa, const char * name);


LIBSBML_EXTERN
int
GeneProductAssociation_setAssociation(GeneProductAssociation_t * gpa, FbcAssociation_t* association);


/**
 * Unsets the value of the "id" attribute of the given 
 * GeneProductAssociation_t structure.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_unsetId(GeneProductAssociation_t * gpa);


/**
 * Unsets the value of the "name" attribute of the given 
 * GeneProductAssociation_t structure.
 *
 * @param gpa the GeneProductAssociation_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_unsetName(GeneProductAssociation_t * gpa);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given GeneProductAssociation_t structure have been set.
 *
 * @param gpa the GeneProductAssociation_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_hasRequiredAttributes(const GeneProductAssociation_t * gpa);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * sub-elements of the given GeneProductAssociation_t structure have been set.
 *
 * @param gpa the GeneProductAssociation_t structure to check.
 *
 * @return @c 1 if all the required sub-elements for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of GeneProductAssociation_t
 */
LIBSBML_EXTERN
int
GeneProductAssociation_hasRequiredElements(const GeneProductAssociation_t * gpa);





END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  GeneProductAssociation_H__  */

