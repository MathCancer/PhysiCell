/**
 * @file    CompSBMLDocumentPlugin.h
 * @brief   Definition of CompSBMLDocumentPlugin, the plugin class of
 *          comp package for the Model element.
 * @author  Lucian Smith
 *
 * <!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *------------------------------------------------------------------------- -->
 *
 * @class CompSBMLDocumentPlugin
 * @sbmlbrief{comp} Extension of SBMLDocument.
 *
 * The CompSBMLDocumentPlugin class inherits from the SBMLDocumentPlugin
 * class, and codifies the extensions to the SBMLDocument class defined in
 * the SBML Level&nbsp;3 @ref comp (&ldquo;comp&rdquo;) package.  This
 * extension allows multiple Model objects to be defined in a single
 * SBMLDocument, stored in an optional child ListOfModelDefinitions object,
 * as well as define references to Model objects in other files, stored in
 * the optional child ListOfExternalModelDefinitions object.  These model
 * definitions, if present, allow Submodel objects to reference other Models
 * to instantiate.
 *
 * The presence of any ModelDefinition or ExternalModelDefinition in an
 * SBMLDocument does not change the default Model in the file.  If a
 * SBMLDocument is submitted somewhere to be simulated, it is still the
 * <code>&lt;model&gt;</code> child of the <code>&lt;sbml&gt;</code> element
 * that should be simulated.
 *
 * In addition, as all packages do, the CompSBMLDocumentPlugin defines a
 * required flag named <code>required</code>, which indicates whether 
 * &ldquo;comp&rdquo; constructs can be used to change the core mathematics of the
 * <code>&lt;model&gt;</code> child of the <code>&lt;sbml&gt;</code> element.
 * Because they can, this attribute must be set @c true.
 */

#ifndef CompSBMLDocumentPlugin_h
#define CompSBMLDocumentPlugin_h


#ifdef __cplusplus

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/SBMLTypeCodes.h>
#include <sbml/SBMLErrorLog.h>
#include <sbml/SBase.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/extension/SBasePlugin.h>
#include <sbml/extension/SBMLDocumentPlugin.h>

#include <iostream>
#include <string>
#include <map>

#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/common/compfwd.h>
#include <sbml/packages/comp/sbml/ModelDefinition.h>
#include <sbml/packages/comp/sbml/ListOfModelDefinitions.h>
#include <sbml/packages/comp/sbml/ListOfExternalModelDefinitions.h>
#include <sbml/packages/comp/validator/CompValidator.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class CompValidator;

class LIBSBML_EXTERN CompSBMLDocumentPlugin : public SBMLDocumentPlugin
{
protected:

  /** @cond doxygenLibsbmlInternal */
  ListOfModelDefinitions  mListOfModelDefinitions;
  ListOfExternalModelDefinitions  mListOfExternalModelDefinitions;
  std::map<std::string, SBMLDocument*> mURIToDocumentMap;
  /** @endcond */

public:

  /**
   * Creates a new CompSBMLDocumentPlugin object using the given parameters.
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
   * @param compns the namespaces object for the package.
   */
  CompSBMLDocumentPlugin (const std::string &uri, const std::string &prefix,
                          CompPkgNamespaces *compns);


  /**
   * Copy constructor. Creates a copy of this CompSBMLDocumentPlugin object.
   *
   * @param orig the instance to copy.
   */
  CompSBMLDocumentPlugin(const CompSBMLDocumentPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~CompSBMLDocumentPlugin ();


  /**
   * Assignment operator for CompSBMLDocumentPlugin.
   *
   * @param orig the object whose values are used as the basis of the
   * assignment.
   */
  CompSBMLDocumentPlugin& operator=(const CompSBMLDocumentPlugin& orig);


  /**
   * Creates and returns a deep copy of this CompSBMLDocumentPlugin object.
   * 
   * @return a (deep) copy of this CompSBMLDocumentPlugin object.
   */
  virtual CompSBMLDocumentPlugin* clone () const;


  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the identifier of the object to find.
   *
   * @return a pointer to the SBase element with the given @p id.
   *
   * @note The comp SBML document plugin has multiple model-wide SId
   * namespaces, so a valid document may well contain multiple elements with
   * the same SId that reside in separate models.  It is not recommended to
   * ever call this function---instead, call the function on the child
   * ModelDefinition objects.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
  
  /**
   * Returns the first child element it can find with the given @p metaid, or
   * itself if it has the given @p metaid, or @c NULL if no such object is
   * found.
   *
   * @param metaid string representing the meta identifier of the object to find.
   *
   * @return a pointer to the SBase element with the given @p metaid.
   */
  virtual SBase* getElementByMetaId(const std::string& metaid);
  
  
  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @param filter a pointer to an ElementFilter, which causes the function 
   * to return only elements that match a particular set of constraints.  
   * If NULL (the default), the function will return all child objects.
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);
  
  
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
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

#ifndef SWIG

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Reads the attributes of corresponding package in SBMLDocument element.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


#endif //SWIG
 
  /** @cond doxygenLibsbmlInternal */
  virtual bool isCompFlatteningImplemented() const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Check consistency function.
   */
  virtual unsigned int checkConsistency();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */


  /*
   * ============================
   * Additional public functions
   * ============================
   */

  /**
   * Returns the ListOf object that holds each ModelDefinition.
   *
   * @return the ListOf object that holds each ModelDefinition.
   */
  const ListOfModelDefinitions* getListOfModelDefinitions () const;


  /**
   * Returns the ModelDefinition with the given index.
   *
   * @param n the index number of the ModelDefinition to get.
   *
   * @return the nth ModelDefinition in the ListOfModelDefinitions.  If the
   * index is invalid, @c NULL is returned.
   */
  ModelDefinition* getModelDefinition (unsigned int n);


  /**
   * Returns the ModelDefinition with the given index.
   *
   * @param n the index number of the ModelDefinition to get.
   *
   * @return the nth ModelDefinition in the ListOfModelDefinitions.  If the
   * index @p n is invalid, @c NULL is returned.
   */
  const ModelDefinition* getModelDefinition (unsigned int n) const;


  /**
   * Returns the model definition object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the model definition to get.
   * 
   * @return ModelDefinition in the ListOfModelDefinitions with the given @p sid
   * or @c NULL if no such ModelDefinition exists.
   *
   * @see getModelDefinition(unsigned int n)
   * @see getListOfModelDefinitions()
   */
  ModelDefinition* getModelDefinition (const std::string& sid);


  /**
   * Returns the model definition object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the model definition to get.
   * 
   * @return ModelDefinition in the ListOfModelDefinitions with the given @p sid
   * or @c NULL if no such ModelDefinition exists.
   *
   * @see getModelDefinition(unsigned int n)
   * @see getListOfModelDefinitions()
   */
  const ModelDefinition* getModelDefinition (const std::string& sid) const;


  /**
   * Adds a copy of the given ModelDefinition object to the 
   * ListOfModelDefinitions.
   *
   * @param modelDefinition the ModelDefinition object to be added to the
   * ListOfModelDefinitions.  Fails if the added ModelDefinition is @c NULL,
   * does not match the level/version/package of the parent object, or cannot
   * be added to the list of replaced elements.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addModelDefinition (const ModelDefinition* modelDefinition);


  /**
   * Returns the number of ModelDefinition objects for this SBMLDocumentPlugin.
   *
   * @return the number of ModelDefinition objects.
   */
  unsigned int getNumModelDefinitions () const;


  /**
   * Creates a ModelDefinition object, adds it to the end of the
   * ModelDefinition objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created ModelDefinition object.
   */
  ModelDefinition* createModelDefinition ();


  /**
   * Removes the ModelDefinition with the given index from the CompSBMLDocumentPlugin.
   *
   * A pointer to the ModelDefinition that was removed is returned.
   * If no ModelDefinition has been removed, @c NULL is returned.
   *
   * @param index the index of the ModelDefinition object to remove.
   *
   * @return the ModelDefinition object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  ModelDefinition* removeModelDefinition(unsigned int index);


  /**
   * Removes the ModelDefinition with the given @p id from the CompSBMLDocumentPlugin.
   *
   * A pointer to the ModelDefinition that was removed is returned.
   * If no ModelDefinition has been removed, @c NULL is returned.
   *
   * @param id the id of the ModelDefinition object to remove.
   *
   * @return the ModelDefinition object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  ModelDefinition* removeModelDefinition(std::string id);


  /**
   * Returns the ListOf object that holds each ExternalModelDefinition.
   *
   * @return the ListOf object that each all ExternalModelDefinition.
   */
  const ListOfExternalModelDefinitions* getListOfExternalModelDefinitions () const;


  /**
   * Returns the ExternalModelDefinition with the given index.
   *
   * @param n the index number of the ExternalModelDefinition to get.
   *
   * @return the nth ExternalModelDefinition in the
   * ListOfExternalModelDefinitions.  If the index is invalid, @c NULL is
   * returned.
   */
  ExternalModelDefinition* getExternalModelDefinition (unsigned int n);


  /**
   * Returns the ExternalModelDefinition with the given index.
   *
   * @param n the index number of the ExternalModelDefinition to get.
   *
   * @return the nth ExternalModelDefinition in the
   * ListOfExternalModelDefinitions.  If the index is invalid, @c NULL is
   * returned.
   */
  const ExternalModelDefinition* getExternalModelDefinition (unsigned int n) const;


  /**
   * Returns the model definition object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the model definition to get.
   * 
   * @return ExternalModelDefinition in the ListOfExternalModelDefinitions with the given @p sid
   * or @c NULL if no such ExternalModelDefinition exists.
   *
   * @see getExternalModelDefinition(unsigned int n)
   * @see getListOfExternalModelDefinitions()
   */
  ExternalModelDefinition* getExternalModelDefinition (const std::string& sid);


  /**
   * Returns the model definition object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the model definition to get.
   * 
   * @return ExternalModelDefinition in the ListOfExternalModelDefinitions with the given @p sid
   * or @c NULL if no such ExternalModelDefinition exists.
   *
   * @see getExternalModelDefinition(unsigned int n)
   * @see getListOfExternalModelDefinitions()
   */
  const ExternalModelDefinition* getExternalModelDefinition (const std::string& sid) const;


  /**
   * Searches the model namespace of the document and returns the Model,
   * ModelDefinition, or ExternalModelDefintion object with the given
   * identifier.
   *
   * @param sid a string representing the identifier of the model definition to get.
   *
   * @return The SBase* corresponding to the given @p sid or @c NULL if no such
   * model exists.  If no such model exists, this will return @c NULL.
   */
  const SBase* getModel(const std::string& sid) const;

  
  /**
   * Searches the model namespace of the document and returns the Model,
   * ModelDefinition, or ExternalModelDefintion object with the given
   * identifier.
   *
   * @param sid a string representing the identifier of the model definition to get.
   *
   * @return The SBase* corresponding to the given @p sid or @c NULL if no such
   * model exists.  If no such model exists, this will return @c NULL.
   */
  SBase* getModel(const std::string& sid);
  

  /**
   *
   * Sets the bool value of "required" attribute of corresponding package
   * in SBMLDocument element.  The only legal value is 'true' for the 
   * Hierarchical %Model Composition package.
   *
   * @param value the bool value of "required" attribute of corresponding 
   * package in SBMLDocument element.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setRequired(bool value);


  /**
   * Adds a copy of the given ExternalModelDefinition object to the
   * ListOfExternalModelDefinitions.
   *
   * @param externalModelDefinition the ExternalModelDefinition object to be
   * added to the ListOFExternalModelDefinitions.  Fails if the added
   * ExternalModelDefinition is @c NULL, does not match the
   * level/version/package of the parent object, or cannot be added to the
   * list of external model definitions.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addExternalModelDefinition (const ExternalModelDefinition* externalModelDefinition);


  /**
   * Returns the number of ExternalModelDefinition objects for this SBMLDocumentPlugin.
   *
   * @return the number of ExternalModelDefinition objects for this SBMLDocumentPlugin.
   */
  unsigned int getNumExternalModelDefinitions () const;


  /**
   * Creates a ExternalModelDefinition object, adds it to the end of the
   * ExternalModelDefinition objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created ExternalModelDefinition object.
   */
  ExternalModelDefinition* createExternalModelDefinition ();


  /**
   * Removes the ExternalModelDefinition with the given index.
   *
   * A pointer to the ExternalModelDefinition that was removed is returned.
   * If no ExternalModelDefinition has been removed, @c NULL is returned.
   *
   * @param index the index of the ExternalModelDefinition object to remove.
   *
   * @return the ExternalModelDefinition object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  ExternalModelDefinition* removeExternalModelDefinition(unsigned int index);


  /**
   * Removes the ExternalModelDefinition with the given @p id.
   *
   * A pointer to the ExternalModelDefinition that was removed is returned.
   * If no ExternalModelDefinition has been removed, @c NULL is returned.
   *
   * @param id the id of the ExternalModelDefinition object to remove.
   *
   * @return the ExternalModelDefinition object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  ExternalModelDefinition* removeExternalModelDefinition(std::string id);


  // ---------------------------------------------------------
  //
  // virtual functions (internal implementation) which should
  // be overridden by subclasses.
  //
  // ---------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this plugin object.
   *
   * Subclasses which contain one or more SBase derived elements must
   * override this function.
   *
   * @param d the SBMLDocument object to use.
   *
   * @see connectToParent
   * @see enablePackageInternal
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the *parent* of this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBML object of this SBML object.
   * (Creates a child-parent relationship by the child)
   * This function is called when a child element is
   * set/added/created by its parent element (e.g. by setXXX,
   * addXXX, createXXX, and connectToChild functions of the
   * parent element).
   *
   * @param parent the SBML object to use.
   */
  void connectToParent (SBase* parent);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Enables/Disables the given package with child elements in this plugin
   * object (if any).
   * (This is an internal implementation invoked from
   *  SBase::enablePackageInternal() function)
   *
   * @note Subclasses in which one or more SBase derived elements are
   * defined must override this function.
   *
   * @see setSBMLDocument
   * @see connectToParent
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond */

  friend class ExternalModelDefinition; //So that it can call 'getSBMLDocumentFromURI' but nothing else can.

  /** @cond doxygenLibsbmlInternal */
  /* retrieve the override flattening flag */
  bool getOverrideCompFlattening() const;

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /* retrieve the override flattening flag */
  void setOverrideCompFlattening(bool overrideCompFlattening);

  /** @endcond */


private:

  /**
   * Uses the suite of URI resolvers to find the document referenced by the
   * URI, or @c NULL if no document can be found.  The pointer returned is a
   * *non-owning* pointer: this CompSBMLDocumentPlugin owns the document in
   * question, and will return the same pointer if the same URI is requested
   * later.  This is so that when Model* objects are returned by
   * ExternalModelDefinition functions, the document the model is from will
   * persist.  The public interface for this function is therefore to use
   * ExternalModelDefinition::getReferencedModel() function.
   */
  virtual SBMLDocument* getSBMLDocumentFromURI(const std::string& uri);


  /**
   * Used by getSBMDocumentFromURI to first resolve the URI into its
   * canonical form, for example, from 'model.xml" to
   * "file:/path/to/model.xml".
   */
  virtual std::string  getResolvedURI(const std::string& uri);


  /**
   * Clears the internal list of SBMLDocuments kept when resolving URIs.  May
   * invalidate distributed pointers, and therefore should only be used if a
   * call to getSBMLDocumentFromURI has returned an incorrect document, and
   * the URI resolvers have since been tweaked so as to no longer return the
   * same result.
   */
  virtual void clearStoredURIDocuments();

  
  /** variables and functions for consistency checking **/

  /** @cond doxygenLibsbmlInternal */
  
  bool mCheckingDummyDoc;
  bool mFlattenAndCheck;
  bool mOverrideCompFlattening;

  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new, empty ModelDefinition_t structure, adds it to the given
 * CompSBMLDocumentPlugin_t, and returns the ModelDefinition_t.
 *
 * @param docPlug the CompSBMLDocumentPlugin_t structure to which the ModelDefinition_t should be
 * added.
 *
 * @return the newly-created empty ModelDefinition_t.
 *
 * @memberof CompSBMLDocumentPlugin_t
 */
LIBSBML_EXTERN
ModelDefinition_t *
CompSBMLDocumentPlugin_createModelDefinition(CompSBMLDocumentPlugin_t * docPlug);

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* CompSBMLDocumentPlugin_h */
