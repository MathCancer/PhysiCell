/**
 * @file   FbcOr.h
 * @brief  Implementation of the FbcOr class
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
 * @class FbcOr
 * @sbmlbrief{fbc} An &ldquo;or&rdquo; relationship for gene products
 *
 * FbcOr class is used in Version 2 of the SBML Level&nbsp;3 @ref fbc
 * (&ldquo;fbc&rdquo;) package to represent an "or" relationship between two
 * or more child FbcAssociation objects.  In other words, it indicates that
 * only one of the child objects are included.  Note that since the
 * FbcAssociation class is the parent class of GeneProductRef, FbcAnd, and
 * FbcOr, a given FbcOr can potentially include nested "and"/"or"
 * combinations of gene products.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 *
 * @see FbcAssociation
 * @see FbcAnd
 * @see GeneProductRef
 */


#ifndef FbcOr_H__
#define FbcOr_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>
#include <sbml/packages/fbc/sbml/FbcAssociation.h>

#include <sbml/packages/fbc/sbml/FbcAssociation.h>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN FbcOr : public FbcAssociation
{

protected:

  /** @cond doxygenLibsbmlInternal */
  ListOfFbcAssociations   mAssociations;
  /** @endcond */


public:

  /**
   * Creates a new FbcOr with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this FbcOr.
   *
   * @param version an unsigned int, the SBML Version to assign to this FbcOr.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this FbcOr.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcOr(unsigned int level      = FbcExtension::getDefaultLevel(),
        unsigned int version    = FbcExtension::getDefaultVersion(),
        unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new FbcOr with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcOr(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for FbcOr.
   *
   * @param orig the FbcOr instance to copy.
   */
  FbcOr(const FbcOr& orig);


   /**
   * Assignment operator for FbcOr.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcOr& operator=(const FbcOr& rhs);


   /**
   * Creates and returns a deep copy of this FbcOr object.
   *
   * @return a (deep) copy of this FbcOr object.
   */
  virtual FbcOr* clone () const;


   /**
   * Destructor for FbcOr.
   */
  virtual ~FbcOr();


   /**
   * Returns the  "ListOfFbcAssociations" in this FbcOr object.
   *
   * @return the "ListOfFbcAssociations" attribute of this FbcOr.
   */
  const ListOfFbcAssociations* getListOfAssociations() const;


  /**
   * Returns the  "ListOfFbcAssociations" in this FbcOr object.
   *
   * @return the "ListOfFbcAssociations" attribute of this FbcOr.
   */
  ListOfFbcAssociations* getListOfAssociations();


  /**
   * Get a Association from the ListOfFbcAssociations.
   *
   * @param n the index number of the Association to get.
   *
   * @return the nth Association in the ListOfFbcAssociations within this FbcOr.
   *
   * @see getNumAssociations()
   */
  FbcAssociation* getAssociation(unsigned int n);


  /**
   * Get a Association from the ListOfFbcAssociations.
   *
   * @param n the index number of the Association to get.
   *
   * @return the nth Association in the ListOfFbcAssociations within this FbcOr.
   *
   * @see getNumAssociations()
   */
  const FbcAssociation* getAssociation(unsigned int n) const;


  /**
   * Get a Association from the ListOfFbcAssociations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Association to get.
   *
   * @return the Association in the ListOfFbcAssociations
   * with the given id or NULL if no such
   * Association exists.
   *
   * @see getAssociation(unsigned int n)
   *
   * @see getNumAssociations()
   */
  FbcAssociation* getAssociation(const std::string& sid);


  /**
   * Get a Association from the ListOfFbcAssociations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Association to get.
   *
   * @return the Association in the ListOfFbcAssociations
   * with the given id or NULL if no such
   * Association exists.
   *
   * @see getAssociation(unsigned int n)
   *
   * @see getNumAssociations()
   */
  const FbcAssociation* getAssociation(const std::string& sid) const;


  /**
   * Adds a copy the given FbcAssociation to this FbcOr.
   *
   * @param fa the FbcAssociation object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_NAMESPACES_MISMATCH, OperationReturnValues_t}
   */
  int addAssociation(const FbcAssociation* fa);


  /**
   * Get the number of FbcAssociation objects in this FbcOr.
   *
   * @return the number of FbcAssociation objects in this FbcOr.
   */
  unsigned int getNumAssociations() const;


  /**
  * Converts this FbcAssociation object into an infix string representation.
  *
  * @return the association as infix string.
  */
  virtual std::string toInfix() const;

  /**
   * Creates a new FbcAnd object, adds it to this FbcOr's
   * ListOfFbcAssociations and returns the FbcAnd object created. 
   *
   * @return a new FbcAnd object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  FbcAnd* createAnd();


  /**
   * Creates a new FbcOr object, adds it to this FbcOr's
   * ListOfFbcAssociations and returns the FbcOr object created. 
   *
   * @return a new FbcOr object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  FbcOr* createOr();


  /**
   * Creates a new GeneProductRef object, adds it to this FbcOr's
   * ListOfFbcAssociations and returns the GeneProductRef object created. 
   *
   * @return a new GeneProductRef object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  GeneProductRef* createGeneProductRef();


  /**
   * Removes the nth Association from the ListOfFbcAssociations within this FbcOr
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Association to remove.
   *
   * @see getNumAssociations()
   */
  FbcAssociation* removeAssociation(unsigned int n);


  /**
   * Removes the Association with the given identifier from the ListOfFbcAssociations within this FbcOr
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Association to remove.
   *
   * @return the Association removed. As mentioned above, the caller owns the
   * returned item.
   */
  FbcAssociation* removeAssociation(const std::string& sid);


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
   * For FbcOr, the XML element name is always @c "fbcOr".
   *
   * @return the name of this element, i.e. @c "fbcOr".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_OR, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this FbcOr object have been set.  Will always return
   * @c true, since FbcOr has no required attributes.
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this FbcOr object have been set.
   *
   * @note An FbcOr object must have at least two FbcAssociation
   * children.
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
 * Creates a new FbcOr_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * FbcOr_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * FbcOr_t structure.
 *
 * @returns the newly-created FbcOr_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof FbcOr_t
 */
LIBSBML_EXTERN
FbcOr_t *
FbcOr_create(unsigned int level, unsigned int version,
             unsigned int pkgVersion);


/**
 * Frees the given FbcOr_t structure.
 * 
 * @param fo the FbcOr_t structure to be freed.
 *
 * @memberof FbcOr_t
 */
LIBSBML_EXTERN
void
FbcOr_free(FbcOr_t * fo);


/**
 * Creates a deep copy of the given FbcOr_t structure.
 * 
 * @param fo the FbcOr_t structure to be copied.
 *
 * @returns a (deep) copy of the given FbcOr_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof FbcOr_t
 */
LIBSBML_EXTERN
FbcOr_t *
FbcOr_clone(FbcOr_t * fo);


LIBSBML_EXTERN
int
FbcOr_addAssociation(FbcOr_t * fo, FbcAssociation_t * fa);


LIBSBML_EXTERN
FbcAnd_t *
FbcOr_createAnd(FbcOr_t * fo);


LIBSBML_EXTERN
FbcOr_t *
FbcOr_createOr(FbcOr_t * fo);


LIBSBML_EXTERN
GeneProductRef_t *
FbcOr_createGeneProductRef(FbcOr_t * fo);


LIBSBML_EXTERN
ListOf_t *
FbcOr_getListOfFbcAssociations(FbcOr_t * fo) ;


LIBSBML_EXTERN
FbcAssociation_t *
FbcOr_getAssociation(FbcOr_t * fo, unsigned int n);


LIBSBML_EXTERN
FbcAssociation_t *
FbcOr_getAssociationById(FbcOr_t * fo, const char * sid);


LIBSBML_EXTERN
unsigned int
FbcOr_getNumAssociations(FbcOr_t * fo);


LIBSBML_EXTERN
FbcAssociation_t *
FbcOr_removeAssociation(FbcOr_t * fo, unsigned int n);


LIBSBML_EXTERN
FbcAssociation_t *
FbcOr_removeAssociationById(FbcOr_t * fo, const char * sid);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given FbcOr_t structure have been set.
 *
 * @param fo the FbcOr_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of FbcOr_t
 */
LIBSBML_EXTERN
int
FbcOr_hasRequiredAttributes(const FbcOr_t * fo);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * sub-elements of the given FbcOr_t structure have been set.
 *
 * @param fo the FbcOr_t structure to check.
 *
 * @return @c 1 if all the required sub-elements for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of FbcOr_t
 */
LIBSBML_EXTERN
int
FbcOr_hasRequiredElements(const FbcOr_t * fo);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  FbcOr_H__  */

