/**
 * @file    CompSBasePlugin.h
 * @brief   Definition of CompSBasePlugin, the plugin class of
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
 * @class CompSBasePlugin
 * @sbmlbrief{comp} Extension of SBase.
 *
 * The CompSBasePlugin class inherits from the SBasePlugin class, and
 * codifies the extensions to the SBase class defined in the @ref comp
 * (&ldquo;comp&rdquo;) package.  This extension allows the modeler to define
 * one or more submodel elements which the parent SBase object replaces,
 * and/or a single submodel element which replaces the parent SBase object.

 * This is accomplished through the addition of an optional ListOfReplacedElements
 * child, which may contain one or more ReplacedElement objects, each of which 
 * references a submodel object to be replaced by the containing SBase object, 
 * and through the addition of a single optional ReplacedBy child, which 
 * references a submodel object which is to replace the containing SBase object.

 * If a single SBase element both contains a ListOfReplacedElements and has a ReplacedBy
 * child, it and all the referenced ReplacedElement objects are to be replaced
 * by the object referenced by the ReplacedBy element.
 *
 * @see ReplacedElement
 * @see ReplacedBy
 */

#ifndef CompSBasePlugin_h
#define CompSBasePlugin_h


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

#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/ListOfReplacedElements.h>
#include <sbml/packages/comp/sbml/ReplacedBy.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN CompSBasePlugin : public SBasePlugin
{
protected:
  /** @cond doxygenLibsbmlInternal */
  ListOfReplacedElements* mListOfReplacedElements;
  ReplacedBy*             mReplacedBy;
  /** @endcond */

public:

  /**
   * Creates a new CompSBasePlugin object using the given parameters.
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
  CompSBasePlugin (const std::string &uri, const std::string &prefix, CompPkgNamespaces *compns);


  /**
   * Copy constructor. Creates a copy of this CompSBasePlugin object.
   *
   * @param orig the instance to copy.
   */
  CompSBasePlugin(const CompSBasePlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~CompSBasePlugin ();


  /**
   * Assignment operator for CompSBasePlugin.
   *
   * @param orig the object whose values are used as the basis of the
   * assignment.
   */
  CompSBasePlugin& operator=(const CompSBasePlugin& orig);


  /**
   * Creates and returns a deep copy of this CompSBasePlugin object.
   * 
   * @return a (deep) copy of this CompSBasePlugin object.
   */
  virtual CompSBasePlugin* clone () const;

 
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


  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the id of the object to find.
   *
   * @return a pointer to the SBase element with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
  
  /**
   * Returns the first child element it can find with the given @p metaid, or
   * itself if it has the given @p metaid, or @c NULL if no such object is
   * found.
   *
   * @param metaid string representing the metaid of the object to find.
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


  /** @cond doxygenLibsbmlInternal */
  /**
   * Writes the attributes of corresponding package in SBMLDocument element.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;

  /** @endcond */


#endif //SWIG
 

  /*
   * ============================
   * Additional public functions
   * ============================
   */

  /**
   * Returns the ListOf object that holds all replacedElements.
   *
   * @return the ListOf object that holds all replacedElements.
   */
  const ListOfReplacedElements* getListOfReplacedElements () const;


  /**
   * Returns the ReplacedElement with the given index.
   *
   * @param n the index number of the ReplacedElement to get.
   *
   * @return the nth ReplacedElement in the ListOfReplacedElements.  If the
   * index is invalid, @c NULL is returned.
   */
  ReplacedElement* getReplacedElement (unsigned int n);


  /**
   * Returns the ReplacedElement with the given index.
   *
   * @param n the index number of the ReplacedElement to get.
   *
   * @return the nth ReplacedElement in the ListOfReplacedElements.  If the
   * index is invalid, @c NULL is returned.
   */
  const ReplacedElement* getReplacedElement (unsigned int n) const;


  /**
   * Adds a copy of the given ReplacedElement object to the list of ReplacedElements.
   *
   * @param replacedElement the ReplacedElement object to be added to the
   * list of ReplacedElements.  Fails if the added ReplacedElement is @c NULL,
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
  int addReplacedElement (const ReplacedElement* replacedElement);


  /**
   * Returns the number of ReplacedElements for this CompSBasePlugin.
   *
   * @return the number of ReplacedElements for this CompSBasePlugin.
   */
  unsigned int getNumReplacedElements () const;


  /**
   * Remove all ReplacedElements, if any exist.
   */
  virtual void clearReplacedElements();
  

  /**
   * Creates a ReplacedElement object, adds it to the end of the
   * ReplacedElement objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created ReplacedElement object.
   */
  ReplacedElement* createReplacedElement ();


  /**
   * Removes the ReplacedElement with the given index.
   *
   * A pointer to the ReplacedElement that was removed is returned.
   * If no ReplacedElement has been removed, @c NULL is returned.
   *
   * @param index the index of the ReplacedElement object to remove.
   *
   * @return the ReplacedElement object removed.  As mentioned above, 
   * the caller owns the returned object. @c NULL is returned if 
   * the given index is out of range.
   */
  ReplacedElement* removeReplacedElement(unsigned int index);


  /**
   * Get the child ReplacedBy of this SBase.
   * 
   * @return the const ReplacedBy child of this SBase.
   */
  const ReplacedBy* getReplacedBy () const;


  /**
   * Get the child ReplacedBy of this SBase.
   * 
   * @return the ReplacedBy child of this SBase.
   */
  ReplacedBy* getReplacedBy ();


  /**
   * Predicate for testing whether the ReplacedBy for this SBase is set.
   *
   * @return @c true if the ReplacedBy of this SBase is set, @c false
   * otherwise.
   */
  bool isSetReplacedBy () const;


  /**
   * Sets the ReplacedBy definition of this SBase to a copy of the given
   * ReplacedBy object instance.
   *
   * This method fails if the added ReplacedBy does not match the
   * level/version/package of the parent object or if the added ReplacedBy
   * cannot be copied.
   *
   * @param replacedBy the ReplacedBy object instance to use.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int setReplacedBy (const ReplacedBy* replacedBy);


  /**
   * Creates a new, empty ReplacedBy, adds it to this CompSBasePlugin and 
   * returns the created ReplacedBy.
   *
   * @return the newly created ReplacedBy object instance.
   */
  ReplacedBy* createReplacedBy ();


  /**
   * Unsets the child ReplacedBy of this SBase.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetReplacedBy();


  /**
   * Helper to log a common type of error.
   */
  virtual void logInvalidId(const std::string& attribute,
                            const std::string& wrongattribute);

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


  /** @cond doxygenLibsbmlInternal */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */


private:
  virtual void createListOfReplacedElements();

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* CompSBasePlugin_h */
