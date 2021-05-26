/**
 * @file    CompModelPlugin.h
 * @brief   Definition of CompModelPlugin, the plugin class of
 *          comp package for the Model element.
 * @author  Lucian Smith
 *
 *<!---------------------------------------------------------------------------
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
 * @class CompModelPlugin
 * @sbmlbrief{comp} Extension of Model.
 *
 * The CompModelPlugin class inherits from the SBMLSBasePlugin class, and
 * codifies the extensions to the Model class defined in the SBML
 * Level&nbsp;3 @ref comp (&ldquo;comp&rdquo;) package.  This extension
 * allows a Model to define Submodels (other Models that are instantiated as
 * new parts of the parent Model), and Ports, a defined interface for
 * including the given Model as a Submodel of a different Model.
 *
 * Submodels are stored in an optional child ListOfSubmodels object, which, 
 * if present, must contain one or more Submodel objects.  All of the Submodels
 * present in the ListOfSubmodels are defined to be instantiated in the
 * 'complete' Model.
 *
 * Ports are stored in an optional child ListOfPorts object, which, 
 * if present, must contain one or more Port objects.  All of the Ports
 * present in the ListOfPorts collectively define the 'port interface'
 * of the Model.
 */

#ifndef CompModelPlugin_h
#define CompModelPlugin_h


#ifdef __cplusplus

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/SBMLTypeCodes.h>
#include <sbml/SBMLErrorLog.h>
#include <sbml/SBase.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/extension/SBasePlugin.h>

#include <iostream>
#include <string>
#include <set>

#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/extension/CompSBasePlugin.h>
#include <sbml/packages/comp/sbml/ListOfSubmodels.h>
#include <sbml/packages/comp/sbml/ListOfPorts.h>
#include <sbml/packages/comp/util/CompFlatteningConverter.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class PrefixTransformer;

class LIBSBML_EXTERN CompModelPlugin : public CompSBasePlugin
{
protected:

  /** @cond doxygenLibsbmlInternal */
  ListOfSubmodels    mListOfSubmodels;
  ListOfPorts        mListOfPorts;
  std::string        mDivider;

  /** @endcond */

public:

  /**
   * Creates a new CompModelPlugin object using the given parameters.
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
  CompModelPlugin (const std::string &uri, const std::string &prefix,
                   CompPkgNamespaces *compns);


  /**
   * Copy constructor. Creates a copy of this CompModelPlugin object.
   *
   * @param orig the instance to copy.
   */
  CompModelPlugin(const CompModelPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~CompModelPlugin ();


  /**
   * Assignment operator for CompModelPlugin.
   *
   * @param orig the object whose values are used as the basis of the
   * assignment.
   */
  CompModelPlugin& operator=(const CompModelPlugin& orig);


  /**
   * Creates and returns a deep copy of this CompModelPlugin object.
   * 
   * @return a (deep) copy of this CompModelPlugin object.
   */
  virtual CompModelPlugin* clone () const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


  /*
   * ============================
   * Additional public functions
   * ============================
   */

  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id a string representing the id of the object to find.
   *
   * @return a pointer to the SBase element with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
  
  /**
   * Returns the first child element it can find with the given meta
   * identifier, or itself if it has the given @p metaid, or @c NULL if no
   * such object is found.
   *
   * @param metaid a string representing the metaid of the object to find.
   *
   * @return a pointer to the SBase element with the given @p metaid.
   */
  virtual SBase* getElementByMetaId(const std::string& metaid);
  
  
 /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @param filter a pointer to an ElementFilter, which causes the function 
   * to return only elements that match a particular set of constraints.  
   * If NULL (the default), the function will return all child objects.
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);
  
  
 /**
   * Returns the ListOf object that holds all submodels.
   *
   * @return the ListOf object that holds all submodels.
   */
  const ListOfSubmodels* getListOfSubmodels () const;


  /**
   * Returns the submodel with the given index.
   * If the index is invalid, @c NULL is returned.
   *
   * @param n the index number of the Submodel to get.
   *
   * @return the nth Submodel in the ListOfSubmodels.
   */
  Submodel* getSubmodel (unsigned int n);


  /**
   * Returns the submodel with the given index.
   *
   * @param n the index number of the Submodel to get.
   *
   * @return the nth Submodel in the ListOfSubmodels.  If the index @p n is
   * invalid, @c NULL is returned.
   */
  const Submodel* getSubmodel (unsigned int n) const;


  /**
   * Returns the submodel with the given identifier.
   *
   * @param id the identifier of the Submodel to get.
   *
   * @return the Submodel in the ListOfSubmodels with the given identifier.
   * If no such submodel with identifier @p id exists, @c NULL is returned.
   */
  Submodel* getSubmodel (std::string id);


  /**
   * Returns the submodel with the given identifier.
   *
   * @param id the identifier of the Submodel to get.
   *
   * @return the Submodel in the ListOfSubmodels with the given
   * identifier. If no submodel with identifier @p id exists, @c NULL is
   * returned.

   */
  const Submodel* getSubmodel (std::string id) const;


  /**
   * Adds a copy of the given Submodel object to the list of submodels.
   *
   * Fails if the added submodel is @c NULL, does not match the
   * level/version/package of the parent object, or cannot be added to the
   * list of submodels.
   *
   * @param submodel the Submodel object to be added to the list of
   * submodels.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addSubmodel (const Submodel* submodel);


  /**
   * Returns the number of submodels for this CompModelPlugin.
   *
   * @return the number of submodels for this CompModelPlugin.
   */
  unsigned int getNumSubmodels () const;


  /**
   * Creates a Submodel object, adds it to the end of the
   * submodel objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created Submodel object.
   */
  Submodel* createSubmodel ();


  /**
   * Removes the submodel with the given index.
   * A pointer to the submodel that was removed is returned.
   * The caller owns the returned item and is responsible for deleting it.
   * 
   * @param index the index of the Submodel object to remove.
   *
   * @return the Submodel object removed.  As mentioned above, the caller
   * owns the returned object. @c NULL is returned if the given @p index is
   * out of range and no submodel has been removed, @c NULL is returned.
   */
  Submodel* removeSubmodel(unsigned int index);


  /**
   * Returns the ListOf object that holds all ports.
   *
   * @return the ListOf object that holds all ports.
   */
  const ListOfPorts* getListOfPorts () const;


  /**
   * Returns the port with the given index.
   *
   * @param n the index number of the Port to get.
   *
   * @return the nth Port in the ListOfPorts.  If the index @p n is invalid,
   * @c NULL is returned.
   */
  Port* getPort (unsigned int n);


  /**
   * Returns the port with the given index.
   *
   * @param n the index number of the Port to get.
   *
   * @return the nth Port in the ListOfPorts. If the index @p n is invalid,
   * @c NULL is returned.
   */
  const Port* getPort (unsigned int n) const;


  /**
   * Returns the port with the given identifier.
   *
   * @param id the id of the Port to get.
   *
   * @return the Port in the ListOfPorts with the given identifier.  If the
   * identifier is invalid, @c NULL is returned.
   */
  Port* getPort (std::string id);


  /**
   * Returns the port with the given identifier.
   *
   * @param id the id of the Port to get.
   *
   * @return the Port in the ListOfPorts with the given identifier.  If the
   * identifier is invalid, @c NULL is returned.
   */
  const Port* getPort (std::string id) const;


  /**
   * Adds a copy of the given Port object to the list of ports.
   *
   * @param port the Port object to be added to the list of ports.  Fails if
   * the added port is @c NULL, does not match the level/version/package of the
   * parent object, or cannot be added to the list of ports.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addPort (const Port* port);


  /**
   * Returns the number of ports for this CompModelPlugin.
   *
   * @return the number of ports for this CompModelPlugin.
   */
  unsigned int getNumPorts () const;


  /**
   * Creates a Port object, adds it to the end of the
   * port objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created Port object.
   */
  Port* createPort ();


  /**
   * Removes the port with the given index.
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param index the index of the Port object to remove.
   *
   * @return the Port object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  Port* removePort(unsigned int index);


  /**
   * Set the string used as the divider between names when renaming and
   * flattening models.
   *
   * The divider string consists of two underscore characters
   * (&quot;<code>__</code>&quot;) by default.  This method will fail if called
   * with an empty @p divider, or a @p divider that cannot be used internally as part
   * of a valid SBML SId.
   * 
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int setDivider(const std::string& divider);


  /**
   * Get the string used as the divider between names when renaming and
   * flattening models.
   *
   * The divider string consists of two underscore characters
   * (&quot;<code>__</code>&quot;) by default, and can be overridden
   * with the setDivider() function.
   *
   * @return the divider that will be used by any call to flattenModel().
   *
   * @see setDivider(const std::string& divider)
   */
  std::string getDivider();


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


  /** @cond doxygenLibsbmlInternal */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */

  /**
   * Sets the custom transformer that is to be used, instead of the standard
   * prefixing with the given divider. This makes it possible to finely control
   * how elements are altered. 
   * 
   * If not set, only ids and meta ids will be prefixed. 
   * 
   * NOTE: the model plugin only holds the pointer to the element it does not 
   *       take ownership of it. Thus the calling program is responsible of 
   *       freeing the transformer when no longer needed (i.e after the 
   *       SBML document has been deleted)
   *
   * @param transformer the prefix transformer to use.
   *      
   */
  void setTransformer(PrefixTransformer* transformer);

  /**
   * @return any custom transformer set for prefix operations, will be NULL by default.
   */
  PrefixTransformer* getTransformer() const;
  
  /**
   * @return an indicator, whether a custom transformer has been set.
   */
  bool isSetTransformer() const;

  /**
   * Unsets any custom prefix transformers. 
   */
  void unsetTransformer();

protected:

  /**
   * Flatten and return a copy of this hierarchical model.
   *
   * Follows all the rules of the hierarchical model composition package and
   * returns a version with all submodels copied into the main model, with all
   * deletions removed and all replaced elements replaced, following any and
   * all rules of conversion factors.  Only the ports created for this model
   * will remain.
   *
   *@return a Model object with no submodels.  On failure, return @c NULL.
   */
  virtual Model* flattenModel() const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Deletes any elements in 'toremove' that do not already exist in 'removed', 
   * while taking care to not double-delete any element.  Intended for use with
   * collectDeletionsAndDeleteCompConstructs() and 
   * collectRenameAndConvertReplacements().
   */
  virtual int removeCollectedElements(std::set<SBase*>* removed, std::set<SBase*>* toremove);
  /** @endcond */

  /**
   * Loop through all Submodels in this Model, instantiate all of them,
   * perform all deletions, and synchronize all replacements, including using
   * any conversion factors that may exist.  The resulting models are stored
   * in the Submodel objects, and available from
   * 'Submodel::getInstantiation()'.  This may be useful if separate simulation
   * or analysis of submodels is desired, as opposed to using flattenModel()
   * to get a single SBML model.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t} (to indicate
   * that this Submodel itself is invalid, and no Model can be instantiated from it)
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int instantiateSubmodels();


  /** @cond doxygenLibsbmlInternal */
  std::set<SBase*>* getRemovedSet();
  friend class CompBase;
  friend class CompFlatteningConverter;
  friend class Replacing;
  friend class ReplacedElement;
  friend class SBaseRef;
  /** @endcond */

private:

  /** @cond doxygenLibsbmlInternal */
  /*
   * This is a nuisance variable, used for the deprecated functions 
   * performDeletions and performReplacementsAndConversions.  It has 
   * no other use.
   */
  std::set<SBase*>  mRemoved;
  /** @endcond */

protected:
  

  /** @cond doxygenLibsbmlInternal */
  /*
   * A custom transformer that can be provided by the user, if they prefer
   * customized modification of elements, instead of simply prefixing their ids.
   * 
   * It is declared here, as we don't want to upset the compile order established
   * in previous releases. 
   */
  PrefixTransformer* mTransformer;
  /** @endcond */

private:

  /*
   * Combine mListOfPorts and mListOfSubmodels.  If this is called from
   * 'flattenModel' (indirectly through Model::appendFrom) it will be deleted
   * anyway, but just in case the function is called in some other context.
   */
  virtual int appendFrom(const Model* model);

  /*
   * Find all SBaseRef-derived objects in this Model and in instantiated
   * Submodels and find and save pointers to the elements they point to.
   */
  virtual int saveAllReferencedElements();

  /*
   * Renames all ids of all elements in the instantiated submodel (SIds,
   * MetaIDs, UnitSIds, and PortSIDs) using the ID of that Submodel,
   * additionally pre-pending the passed-in string (if provided).
   * Recursively renames things in instantiated versions of sub-submodels,
   * etc.
   */
  virtual int renameAllIDsAndPrepend(const std::string& prefix);

  /** @cond doxygenLibsbmlInternal */
  /*
   * DEPRECATED FUNCTION:  DO NOT USE!!!
   *
   * It is impossible to properly use this function as it was originally designed,
   * without some models either causing the program to crash, or causing them
   * to be interpreted incorrectly.  Instead, one should use 
   * collectDeletionsAndDeleteCompConstructs, in conjunction with 
   * collectRenameAndConvertReplacements and removeCollectedElements
   * to properly process hierarchical models.
   */
  virtual int performDeletions();
  /** @endcond */
 
  
  /**
   * Collects all elements from instantiated submodels slated to be deleted,
   * and stores them in 'toremove', and also actually deletes the comp constructs
   * Deletions, ReplacedElements, and ReplacedBy's.  This is so that
   * it is possible to delete a deletion or replacement, and end up with a model 
   * that still has the element that would have otherwise been deleted.
   * Also, actually deletes local parameters, because this potentially affects
   * the naming conventions when replacing.
   *
   * Any comp elements or local parameters that have been removed will be added to 'removed', and
   * any elements that are to be removed will be added to 'toremove'.
   */
virtual int collectDeletionsAndDeleteSome(std::set<SBase*>* removed, 
                                          std::set<SBase*>* toremove);


  /** @cond doxygenLibsbmlInternal */
  /*
   * DEPRECATED FUNCTION:  DO NOT USE!!!
   *
   * It is impossible to properly use this function as it was originally designed,
   * without some models either causing the program to crash, or causing them
   * to be interpreted incorrectly.  Instead, one should use 
   * collectDeletionsAndDeleteCompConstructs, in conjunction with 
   * collectRenameAndConvertReplacements and removeCollectedElements
   * to properly process hierarchical models.
   */
  virtual int performReplacementsAndConversions();
  /** @endcond */

  /**
   * Removes all elements from instantiated submodels slated to be replaced,
   * and points all old references to that element to the replacement
   * element.  Also takes any 'replacedBy' construct, deleting the original
   * object, renaming the replacement object with the replaced object's
   * identifiers, and points all old references to the replacement object's
   * old identifiers to the new identifiers.
   */
  virtual int collectRenameAndConvertReplacements(std::set<SBase*>* removed, 
                                                  std::set<SBase*>* toremove);

  /** @cond doxygenLibsbmlInternal */
  virtual void findUniqueSubmodPrefixes(std::vector<std::string>& submodids, List* allElements);
  virtual void renameIDs(List* allElements, const std::string& prefix);
  virtual void resetPorts();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  protected:
  virtual int saveAllReferencedElements(std::set<SBase*> uniqueRefs, std::set<SBase*> replacedBys, SBMLDocument* doc);
  /** @endcond */

};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new, empty Submodel_t structure, adds it to the given
 * CompModelPlugin_t, and returns the Submodel_t.
 *
 * @param modelPlug the CompModelPlugin_t structure to which the Submodel_t should be
 * added.
 *
 * @return the newly-created empty Submodel_t.
 *
 * @memberof CompModelPlugin_t
 */
LIBSBML_EXTERN
Submodel_t *
CompModelPlugin_createSubmodel(CompModelPlugin_t * modelPlug);

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* CompModelPlugin_h */
