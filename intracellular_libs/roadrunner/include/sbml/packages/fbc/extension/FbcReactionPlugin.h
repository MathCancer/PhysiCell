/**
 * @file   FbcReactionPlugin.h
 * @brief  Implementation of the FbcReactionPlugin class
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
 * @class FbcReactionPlugin
 * @sbmlbrief{fbc} Extension of Reaction by the &ldquo;fbc&rdquo; package.
 *
 * The FbcReactionPlugin class inherits from the SBasePlugin class, and
 * codifies the extentions to the Reaction class defined in the @ref fbc
 * package (&ldquo;fbc&rdquo;).  This extention allows the modeler to define
 * (in Version&nbsp;2 of the &ldquo;fbc&rdquo; package) an upper and lower
 * flux bound, with the 'upperFluxBound' and 'lowerFluxBound' attributes,
 * as well as a way to reference any GeneProduct associated with
 * this Reaction, through the GeneProductAssociation child.
 *
 * @note In Version&nbsp;1 of &ldquo;fbc&rdquo;, the FluxBound element is
 * used instead of the 'upperFluxBound' and 'lowerFluxBound' attributes.
 * There is no equivalent of the GeneProductAssociation, which was added 
 * in Version&nbsp;2.
 */


#ifndef FbcReactionPlugin_H__
#define FbcReactionPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/fbc/sbml/GeneProductAssociation.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FbcReactionPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new FbcReactionPlugin object using the given parameters.
   *
   * @copydetails doc_what_are_xmlnamespaces
   *
   * @copydetails doc_what_are_sbmlnamespaces
   *
   * @param uri the URI of the SBML Level&nbsp;3 package implemented by
   * this libSBML package extension.
   *
   * @param prefix the XML namespace prefix being used for the package.
   *
   * @param fbcns the namespaces object for the package.
   */
  FbcReactionPlugin(const std::string& uri, const std::string& prefix, 
                                 FbcPkgNamespaces* fbcns);


  /**
   * Copy constructor for FbcReactionPlugin.
   *
   * @param orig the FbcReactionPlugin instance to copy.
   */
  FbcReactionPlugin(const FbcReactionPlugin& orig);


   /**
   * Assignment operator for FbcReactionPlugin.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcReactionPlugin& operator=(const FbcReactionPlugin& rhs);


   /**
   * Creates and returns a deep copy of this FbcReactionPlugin object.
   *
   * @return a (deep) copy of this FbcReactionPlugin object.
   */
  virtual FbcReactionPlugin* clone () const;


   /**
   * Destructor for FbcReactionPlugin.
   */
  virtual ~FbcReactionPlugin();


   //---------------------------------------------------------------
  //
  // overridden virtual functions for read/write/check
  //
  //---------------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to create, store, and then
   * return an SBML object corresponding to the next XMLToken in the
   * XMLInputStream if they have their specific elements.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);


  /** @endcond */

  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond */


  //---------------------------------------------------------------


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond */

  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


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


  //---------------------------------------------------------------
  //
  // Functions for interacting with the members of the plugin
  //
  //---------------------------------------------------------------

  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the GeneProductAssociation from this FbcReactionPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the GeneProductAssociation from object in this FbcReactionPlugin object.
   */
  const GeneProductAssociation* getGeneProductAssociation () const;


  /**
   * Returns the GeneProductAssociation from this FbcReactionPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the GeneProductAssociation from object in this FbcReactionPlugin object.
   */
  GeneProductAssociation* getGeneProductAssociation ();


  /**
   * Predicate returning @c true if this FbcReactionPlugin's
   * "GeneProductAssociation" element has been set.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return @c true if the "GeneProductAssociation" element has been set,
   * otherwise @c false is returned.
   */
  bool isSetGeneProductAssociation() const;


  /**
   * Sets the GeneProductAssociation element in this FbcReactionPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param geneProductAssociation the geneProductAssociation to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int setGeneProductAssociation (const GeneProductAssociation* geneProductAssociation);


  /**
   * Creates a new GeneProductAssociation object and adds it to the FbcReactionPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the newly created GeneProductAssociation object.
   */
  GeneProductAssociation* createGeneProductAssociation ();


  /**
   * Returns the value of the "lowerFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @return the value of the "lowerFluxBound" attribute of this FbcReactionPlugin as a string.
   */
  virtual const std::string& getLowerFluxBound() const;


  /**
   * Returns the value of the "upperFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @return the value of the "upperFluxBound" attribute of this FbcReactionPlugin as a string.
   */
  virtual const std::string& getUpperFluxBound() const;


  /**
   * Predicate returning @c true if this FbcReactionPlugin's "lowerFluxBound"
   * attribute is set.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @return @c true if this FbcReactionPlugin's "lowerFluxBound" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLowerFluxBound() const;


  /**
   * Predicate returning @c true if this FbcReactionPlugin's "upperFluxBound"
   * attribute is set.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @return @c true if this FbcReactionPlugin's "upperFluxBound" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetUpperFluxBound() const;


  /**
   * Sets the value of the "lowerFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @param lowerFluxBound the value of the "lowerFluxBound" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setLowerFluxBound(const std::string& lowerFluxBound);


  /**
   * Sets the value of the "upperFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @param upperFluxBound the value of the "upperFluxBound" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setUpperFluxBound(const std::string& upperFluxBound);


  /**
   * Unsets the value of the "lowerFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetLowerFluxBound();


  /**
   * Unsets the value of the "upperFluxBound" attribute of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetUpperFluxBound();

  /**
   * Unsets the the "geneProduct" element of this FbcReactionPlugin.
   *
   * @copydetails note_fluxbound_v2_only
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetGeneProductAssociation();

  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  virtual void connectToParent (SBase* sbase);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  virtual bool accept (SBMLVisitor& v) const;

  /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */

  GeneProductAssociation* mGeneProductAssociation;
  std::string   mLowerFluxBound;
  std::string   mUpperFluxBound;

  /** @endcond */


};

LIBSBML_CPP_NAMESPACE_END


#endif  /* __cplusplus */
#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS



/**
 * Takes a FbcReactionPlugin_t structure and returns its UpperFluxBound.
 *
 * @param fbc the FbcReactionPlugin_t whose UpperFluxBound is sought.
 *
 * @return the UpperFluxBound of the given FbcReactionPlugin_t, as a pointer to a string.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
char *
FbcReactionPlugin_getUpperFluxBound(SBasePlugin_t * fbc);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FbcReactionPlugin_t structure's UpperFluxBound is set.
 *
 * @param fbc the FbcReactionPlugin_t structure to query.
 * 
 * @return @c non-zero (true) if the "UpperFluxBound" attribute of the given
 * FbcReactionPlugin_t structure is set, zero (false) otherwise.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_isSetUpperFluxBound(SBasePlugin_t * fbc);


/**
 * Sets the UpperFluxBound of the given FbcReactionPlugin_t to a copy of @p UpperFluxBound.
 *
 * @param fbc the FbcReactionPlugin_t structure to set.
 * @param UpperFluxBound the UpperFluxBound to assign to the given FbcReactionPlugin_t's "UpperFluxBound" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "UpperFluxBound" attribute.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_setUpperFluxBound(SBasePlugin_t * fbc, const char * UpperFluxBound);


/**
 * Unsets the "UpperFluxBound" attribute of the given FbcReactionPlugin_t structure.
 *
 * @param fbc the FbcReactionPlugin_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_unsetUpperFluxBound(SBasePlugin_t * fbc);


/**
 * Takes a FbcReactionPlugin_t structure and returns its LowerFluxBound.
 *
 * @param fbc the FbcReactionPlugin_t whose LowerFluxBound is sought.
 *
 * @return the LowerFluxBound of the given FbcReactionPlugin_t, as a pointer to a string.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
char *
FbcReactionPlugin_getLowerFluxBound(SBasePlugin_t * fbc);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FbcReactionPlugin_t structure's LowerFluxBound is set.
 *
 * @param fbc the FbcReactionPlugin_t structure to query.
 * 
 * @return @c non-zero (true) if the "LowerFluxBound" attribute of the given
 * FbcReactionPlugin_t structure is set, zero (false) otherwise.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_isSetLowerFluxBound(SBasePlugin_t * fbc);


/**
 * Sets the LowerFluxBound of the given FbcReactionPlugin_t to a copy of @p LowerFluxBound.
 *
 * @param fbc the FbcReactionPlugin_t structure to set.
 * @param LowerFluxBound the LowerFluxBound to assign to the given FbcReactionPlugin_t's "LowerFluxBound" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "LowerFluxBound" attribute.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_setLowerFluxBound(SBasePlugin_t * fbc, const char * LowerFluxBound);


/**
 * Unsets the "UpperFluxBound" attribute of the given FbcReactionPlugin_t structure.
 *
 * @param fbc the FbcReactionPlugin_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_unsetUpperFluxBound(SBasePlugin_t * fbc);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FbcReactionPlugin_t structure's GeneProductAssociation is set.
 *
 * @param fbc the FbcReactionPlugin_t structure to query.
 * 
 * @return @c non-zero (true) if the "geneProductAssopciation" elemen of the given
 * FbcReactionPlugin_t structure is set, zero (false) otherwise.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_isSetGeneProductAssociation(SBasePlugin_t * fbc);


/**
 * Takes a FbcReactionPlugin_t structure and returns its GeneProductAssociation_t.
 *
 * @param fbc the FbcReactionPlugin_t whose GeneProductAssociation_t is sought.
 *
 * @return the a pointer to the GeneProductAssociation_t of the given FbcReactionPlugin_t.
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
GeneProductAssociation_t*
FbcReactionPlugin_getGeneProductAssociation(SBasePlugin_t * fbc);


/**
 * Takes a FbcReactionPlugin_t structure and sets its GeneProductAssociation_t.
 *
 * @param fbc the FbcReactionPlugin_t whose GeneProductAssociation_t is sought.
 * @param gpa a pointer to the GeneProductAssociation_t to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcReactionPlugin_t
 */
LIBSBML_EXTERN
int
FbcReactionPlugin_setGeneProductAssociation(SBasePlugin_t * fbc, 
                                            GeneProductAssociation_t* gpa);





END_C_DECLS

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* FbcReactionPlugin_H__ */


