/**
 * @file   FbcAssociation.h
 * @brief  Implementation of the FbcAssociation class
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
 * @class FbcAssociation
 * @sbmlbrief{fbc} Base class for FbcAnd, FbcOr, and GeneProductRef
 *
 * The FbcAssociation class is the abstract base class for the classes that
 * can be used as children of the GeneProductAssociation child of a Reaction.
 * The FbcAnd class is used when all of its children are definitely associated
 * with the Reaction; the FbcOr class is used when at least one of its children
 * are associated with the Reaction; and the GeneProductRef class is used to
 * denote a particular GeneProduct.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 *
 * @see ListOfFbcAssociations
 * @see FbcAnd
 * @see FbcOr
 * @see GeneProductRef
 */

#ifndef FbcAssociation_H__
#define FbcAssociation_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN

class FbcAnd;
class FbcOr;
class GeneProductRef;
class FbcModelPlugin;


class LIBSBML_EXTERN FbcAssociation : public SBase
{

protected:
  /** @cond doxygenLibsbmlInternal */
  std::string   mElementName;
  /** @endcond */

public:

  /**
   * Creates a new FbcAssociation with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * FbcAssociation.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * FbcAssociation.
   *
   * @param pkgVersion an unsigned int, the SBML &ldquo;fbc&rdquo; package
   * Version to assign to this FbcAssociation.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcAssociation(unsigned int level      = FbcExtension::getDefaultLevel(),
                 unsigned int version    = FbcExtension::getDefaultVersion(),
                 unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new FbcAssociation with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcAssociation(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for FbcAssociation.
   *
   * @param orig the FbcAssociation instance to copy.
   */
  FbcAssociation(const FbcAssociation& orig);


   /**
   * Assignment operator for FbcAssociation.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcAssociation& operator=(const FbcAssociation& rhs);


   /**
   * Creates and returns a deep copy of this FbcAssociation object.
   *
   * @return a (deep) copy of this FbcAssociation object.
   */
  virtual FbcAssociation* clone () const;


   /**
   * Destructor for FbcAssociation.
   */
  virtual ~FbcAssociation();


   /**
   * Returns @c true, if this abstract FbcAssociation is of type FbcAnd.
   *
   * @return @c true, if this abstract FbcAssociation is of type FbcAnd.
   *
   */
  virtual bool isFbcAnd() const;


  /**
   * Returns @c true, if this abstract FbcAssociation is of type FbcOr.
   *
   * @return @c true, if this abstract FbcAssociation is of type FbcOr.
   *
   */
  virtual bool isFbcOr() const;


  /**
   * Returns @c true, if this abstract FbcAssociation is of type
   * GeneProductRef.
   *
   * @return @c true, if this abstract FbcAssociation is of type
   * GeneProductRef.
   *
   */
  virtual bool isGeneProductRef() const;


  /**
   * Returns the XML element name of this object.
   *
   * For FbcAssociation, the XML element name is always @c "fbcAssociation".
   *
   * @return the name of this element, i.e. @c "fbcAssociation".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_ASSOCIATION, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this FbcAssociation object have been set.
   *
   * @note FbcAssociation has no required attributes, so this
   * method always returns @c true.
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


  /**
  * Parses a gene association in infix format and returns a corresponding
  * Association object.
  *
  * This parses a string that has a list of gene names and conjunctions
  * or disjunctions.  For example:
  * @verbatim
  (b2422) and (b2425) and (b2423) and (b2424) or (b2422) and (b2423) and (b2424) and (b2413) and (b3917)
@endverbatim
  *
  * The 'and' operator takes precedence over the 'or' operator, meaning that
  * the above input string would turn into two groups of gene names: either
  * "b2422, b2425, b2423, and b2424" or "b2422, b2423, b2424, b2413, and b3917".
  * Parentheses may be added to make things more clear, and to encode
  * alternative schemes.
  * 
  * This method also creates missing GeneProduct objects, in case the unique
  * reference does not yet exist.
  *
  * @param association the string to parse.
  * @param plugin the FbcModelPlugin on which to add the geneProduct elements.
  *
  * @return the parsed association, or @c NULL in case of an error.
  *
  * @copydetails doc_note_static_methods
  */
  static FbcAssociation* parseFbcInfixAssociation(const std::string& association,
                                                  FbcModelPlugin* plugin);


  /**
  * Converts this FbcAssociation object into an infix string representation.
  *
  * @return the association as infix string.
  */
  virtual std::string toInfix() const;


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

    virtual void setElementName(const std::string& name);

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

  friend class GeneProductAssociation;

};

/**
 * @class ListOfFbcAssociations
 * @sbmlbrief{fbc} A list of FbcAssociation objects.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The ListOfFbcAssociations is a container of FbcAssociation objects for the
 * FbcAnd and FbcOr classes.  Unlike the ListOf___ classes in SBML, the
 * ListOfFbcAssociations is not a class that appears in the &ldquo;fbc&rdquo;
 * Version&nbsp;2 specification, and instead is used internally in libSBML as
 * a convenience class to store arbitrary numbers of FbcAssociation objects.
 * Also unlike other ListOf___ classes, the FbcAnd and FbcOr classes require
 * at least two child FbcAssociation objects, so valid ListOfFbcAssociations
 * libsbml objects will always contain two or more children.  These children
 * will have element names associated with their derived class, not the base
 * FbcAssociation class: <code>&lt;fbc:and&gt;</code>,
 * <code>&lt;fbc:or&gt;</code>, and <code>&lt;fbc:geneProductRef&gt;</code>.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 *
 * @warning It is important to be clear that <em>ListOfFbcAssociations is not
 * written out in the XML output produced by libSBML</em>&mdash;the
 * constructs only exist in software to enable software applications to
 * manipulate FbcAssociation objects in a way that mirrors how other lists of
 * components in SBML are manipulated.  ListOfFbcAssociations is abstracted
 * away when an SBML &ldquo;fbc&rdquo; model file is actually written out in
 * the final XML form.
 *
 * @see FbcAssociation
 * @see FbcAnd
 * @see FbcOr
 * @see GeneProductRef
 */
class LIBSBML_EXTERN ListOfFbcAssociations : public ListOf
{

public:

  /**
   * Creates a new ListOfFbcAssociations with the given SBML Level, Version,
   * and &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * ListOfFbcAssociations.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * ListOfFbcAssociations.
   *
   * @param pkgVersion an unsigned int, the SBML &ldquo;fbc&rdquo; package
   * Version to assign to this ListOfFbcAssociations.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfFbcAssociations(unsigned int level      = FbcExtension::getDefaultLevel(),
                        unsigned int version    = FbcExtension::getDefaultVersion(),
                        unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfFbcAssociations with the given FbcPkgNamespaces
   * object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfFbcAssociations(FbcPkgNamespaces* fbcns);


   /**
   * Creates and returns a deep copy of this ListOfFbcAssociations object.
   *
   * @return a (deep) copy of this ListOfFbcAssociations object.
   */
  virtual ListOfFbcAssociations* clone () const;


   /**
   * Get a FbcAssociation from the ListOfFbcAssociations.
   *
   * @param n the index number of the FbcAssociation to get.
   *
   * @return the nth FbcAssociation in this ListOfFbcAssociations.
   *
   * @see size()
   * @see getNumFbcAssociations()
   */
  virtual FbcAssociation* get(unsigned int n);


  /**
   * Get a FbcAssociation from the ListOfFbcAssociations.
   *
   * @param n the index number of the FbcAssociation to get.
   *
   * @return the nth FbcAssociation in this ListOfFbcAssociations.
   *
   * @see size()
   * @see getNumFbcAssociations()
   */
  virtual const FbcAssociation* get(unsigned int n) const;


  /**
   * Get a FbcAssociation from the ListOfFbcAssociations based on its
   * identifier.
   *
   * @param sid a string representing the identifier of the FbcAssociation to
   * get.
   *
   * @return FbcAssociation in this ListOfFbcAssociations with the given id
   * or NULL if no such FbcAssociation exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual FbcAssociation* get(const std::string& sid);


  /**
   * Get a FbcAssociation from the ListOfFbcAssociations based on its
   * identifier.
   *
   * @param sid a string representing the identifier of the FbcAssociation to
   * get.
   *
   * @return FbcAssociation in this ListOfFbcAssociations with the given id
   * or NULL if no such FbcAssociation exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const FbcAssociation* get(const std::string& sid) const;


  /**
   * Adds a copy the given FbcAssociation object to this
   * ListOfFbcAssociations.
   *
   * @param fa the FbcAssociation object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int addFbcAssociation(const FbcAssociation* fa);


  /**
   * Get the number of FbcAssociation objects in this ListOfFbcAssociations.
   *
   * @return the number of FbcAssociation objects in this
   * ListOfFbcAssociations.
   */
  unsigned int getNumFbcAssociations() const;


  /**
   * Creates a new FbcAnd object.
   *
   * This method creates a new FbcAssociation object of subclass FbcAnd, adds
   * it to the ListOfFbcAssociations, and returns the FbcAssociation object
   * created.
   *
   * @return a new FbcAssociation object instance.
   *
   * @see createOr()
   * @see createGeneProductRef()
   * @see addFbcAssociation(const FbcAssociation* fa)
   */
  FbcAnd* createAnd();


  /**
   * Creates a new FbcOr object.
   *
   * This method creates a new FbcAssociation object of subclass FbcOr, adds
   * it to the ListOfFbcAssociations, and returns the FbcAssociation object
   * created.
   *
   * @return a new FbcAssociation object instance.
   *
   * @see createAnd()
   * @see createGeneProductRef()
   * @see addFbcAssociation(const FbcAssociation* fa)
   */
  FbcOr* createOr();


  /**
   * Creates a new GeneProductRef object.
   *
   * This method creates a new FbcAssociation object of subclass
   * GeneProductRef, adds it to the ListOfFbcAssociations, and returns the
   * FbcAssociation object created.
   *
   * @return a new FbcAssociation object instance.
   *
   * @see createOr()
   * @see createAnd()
   * @see addFbcAssociation(const FbcAssociation* fa)
   */
  GeneProductRef* createGeneProductRef();


  /**
   * Removes the nth FbcAssociation
   *
   * This method removes the nth object from this ListOfFbcAssociations and
   * returns a pointer to it.
   *
   * @param n the index of the FbcAssociation to remove.
   *
   * @see size()
   */
  virtual FbcAssociation* remove(unsigned int n);


  /**
   * Removes the FbcAssociation with the given identifier
   *
   * This method searches for and removes the FbcAssociation object with the
   * given identifier @p sid, and returns a pointer to it.  The caller owns
   * the returned item and is responsible for deleting it.  If none of the
   * items in this list have the identifier @p sid, then @c NULL is returned.
   *
   * @param sid the identifier of the FbcAssociation to remove.
   *
   * @return the FbcAssociation removed. As mentioned above, the caller owns
   * the returned item.
   */
  virtual FbcAssociation* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object.
   *
   * For ListOfFbcAssociations, the XML element name is always
   * @c "listOfFbcAssociations".
   *
   * @return the name of this element.
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
   * instance: @sbmlconstant{SBML_FBC_ASSOCIATION, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new FbcAssociation in this ListOfFbcAssociations
   */
  virtual SBase* createObject(XMLInputStream& stream);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the &ldquo;fbc&rdquo; package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;

  /** @endcond */


  virtual bool isValidTypeForList(SBase * item);


  friend class FbcAnd;
  friend class FbcOr;

};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new FbcAssociation_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * FbcAssociation_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * FbcAssociation_t structure.
 *
 * @returns the newly-created FbcAssociation_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof FbcAssociation_t
 */
LIBSBML_EXTERN
FbcAssociation_t *
FbcAssociation_create(unsigned int level, unsigned int version,
                      unsigned int pkgVersion);


/**
 * Frees the given FbcAssociation_t structure.
 * 
 * @param fa the FbcAssociation_t structure to be freed.
 *
 * @memberof FbcAssociation_t
 */
LIBSBML_EXTERN
void
FbcAssociation_free(FbcAssociation_t * fa);


/**
 * Creates a deep copy of the given FbcAssociation_t structure.
 * 
 * @param fa the FbcAssociation_t structure to be copied.
 *
 * @returns a (deep) copy of the given FbcAssociation_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof FbcAssociation_t
 */
LIBSBML_EXTERN
FbcAssociation_t *
FbcAssociation_clone(FbcAssociation_t * fa);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given FbcAssociation_t structure have been set.
 *
 * @param fa the FbcAssociation_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of FbcAssociation_t
 */
LIBSBML_EXTERN
int
FbcAssociation_hasRequiredAttributes(const FbcAssociation_t * fa);



LIBSBML_EXTERN
FbcAssociation_t *
ListOfFbcAssociations_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
FbcAssociation_t *
ListOfFbcAssociations_removeById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
char *
FbcAssociation_toInfix(const FbcAssociation_t * fa);

LIBSBML_EXTERN
FbcAssociation_t*
FbcAssociation_parseFbcInfixAssociation(const char * infix, SBasePlugin_t* plugin);

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  FbcAssociation_H__  */

