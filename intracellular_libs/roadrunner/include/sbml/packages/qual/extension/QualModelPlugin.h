/**
 * @file    QualModelPlugin.h
 * @brief   Definition of QualModelPlugin, the plugin class of
 *          qual package for the Model element.
 * @author  Akiya Jouraku
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
 * Copyright (C) 2009-2011 jointly by the following organizations: 
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
 * @class QualModelPlugin
 * @sbmlbrief{qual} Extension of Model.
 *
 * The extension of SBML Level 3 Core's Model class is relatively
 * straightforward: the Qualitative Models Package adds two lists, one for
 * holding qualitativeSpecies (ListOfQualitativeSpecies), and the other for
 * holding transitions (ListOfTransitions).  The Model element may contain at
 * most one ListOfQualitativeSpecies, which must contain at least one
 * QualitativeSpecies. It may also contain at most one ListOfTransitions
 * which must contain at least one Transition.
 */

#ifndef QualModelPlugin_h
#define QualModelPlugin_h


#include <sbml/common/extern.h>

#ifdef __cplusplus

#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/qual/sbml/QualitativeSpecies.h>
#include <sbml/packages/qual/sbml/Transition.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN QualModelPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new QualModelPlugin object using the given parameters.
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
   * @param qualns the namespaces object for the package.
   */
  QualModelPlugin (const std::string &uri, const std::string &prefix,
                    QualPkgNamespaces *qualns);


  /**
   * Copy constructor. Creates a copy of this object.
   *
   * @param orig the instance to copy.
   */
  QualModelPlugin(const QualModelPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~QualModelPlugin ();


  /**
   * Assignment operator for QualModelPlugin.
   */
  QualModelPlugin& operator=(const QualModelPlugin& orig);


  /**
   * Creates and returns a deep copy of this QualModelPlugin object.
   * 
   * @return a (deep) copy of this QualModelPlugin object.
   */
  virtual QualModelPlugin* clone () const;


  // --------------------------------------------------------
  //
  // overridden virtual functions for reading/writing/checking 
  // elements
  //
  // --------------------------------------------------------

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


  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** ------------------------------------------------------------------
   *
   *  Additional public functions
   *
   * ------------------------------------------------------------------
   */


  /** @cond doxygenLibsbmlInternal */
  int appendFrom(const Model* model);

  /** @endcond */


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the ListOfQualitativeSpecies in this plugin object.
   *
   * @return ListOfQualitativeSpecies object in this plugin object.
   */
  const ListOfQualitativeSpecies* getListOfQualitativeSpecies () const;


  /**
   * Returns the ListOfQualitativeSpecies in this plugin object.
   *
   * @return ListOfQualitativeSpecies object in this plugin object.
   */
  ListOfQualitativeSpecies* getListOfQualitativeSpecies ();


  /**
   * Returns the QualitativeSpecies object that belongs to the given index. If the
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the QualitativeSpecies to get.
   *
   * @return the nth QualitativeSpecies in the ListOfQualitativeSpecies.
   */
  const QualitativeSpecies* getQualitativeSpecies (unsigned int n) const;


  /**
   * Returns the QualitativeSpecies object that belongs to the given index. If the
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the QualitativeSpecies to get.
   *
   * @return the nth QualitativeSpecies in the ListOfQualitativeSpecies.
   */
  QualitativeSpecies* getQualitativeSpecies (unsigned int n);


  /**
   * Returns the qualitativeSpecies object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the QualitativeSpecies to get.
   * 
   * @return QualitativeSpecies in the ListOfQualitativeSpecies with the given id
   * or NULL if no such QualitativeSpecies exists.
   *
   * @see getQualitativeSpecies(unsigned int n)
   * @see getListOfQualitativeSpecies()
   */
  QualitativeSpecies* getQualitativeSpecies (const std::string& sid);


  /**
   * Returns the qualitativeSpecies object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the QualitativeSpecies to get.
   * 
   * @return QualitativeSpecies in the ListOfQualitativeSpecies with the given id 
   * or NULL if no such QualitativeSpecies exists.
   *
   * @see getQualitativeSpecies(unsigned int n)
   * @see getListOfQualitativeSpecies()
   */
  const QualitativeSpecies* getQualitativeSpecies (const std::string& sid) const;


  /**
   * Adds a copy of the given QualitativeSpecies object to the list of qual.
   *
   * @param qualitativeSpecies the QualitativeSpecies object to be added to the list of qual.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int addQualitativeSpecies (const QualitativeSpecies* qualitativeSpecies);


  /**
   * Creates a new qual object and adds it to the list of qual objects
   * and returns it.
   *
   * @return a newly created QualitativeSpecies object.
   */
  QualitativeSpecies* createQualitativeSpecies();


  /**
   * Removes the nth QualitativeSpecies object from this plugin object and
   * returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   *  deleting it.
   *
   * @param n the index of the QualitativeSpecies object to remove.
   *
   * @return the QualitativeSpecies object removed.  As mentioned above, the 
   * caller owns the returned object. NULL is returned if the 
   * given index is out of range.
   */
  QualitativeSpecies* removeQualitativeSpecies (unsigned int n);


  /**
   * Removes the QualitativeSpecies object with the given id attribute from 
   * this plugin object and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @param sid the id attribute of the QualitativeSpecies object to remove.
   *
   * @return the QualitativeSpecies object removed.  As mentioned above, the 
   * caller owns the returned object. NULL is returned if the 
   * given index is out of range.
   */
  QualitativeSpecies* removeQualitativeSpecies (const std::string& sid);


  /**
   * Returns the number of QualitativeSpecies object in this plugin object.
   *
   * @return the number of QualitativeSpecies object in this plugin object.
   */
  unsigned int getNumQualitativeSpecies() const;

  /**
   * Returns the ListOfTransitions in this plugin object.
   *
   * @return ListOfTransitions object in this plugin object.
   */
  const ListOfTransitions* getListOfTransitions () const;


  /**
   * Returns the ListOfTransitions in this plugin object.
   *
   * @return ListOfTransitions object in this plugin object.
   */
  ListOfTransitions* getListOfTransitions ();


  /**
   * Returns the Transition object that belongs to the given index. If the
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the Transition to get.
   *
   * @return the nth Transition in the ListOfTransitions.
   */
  const Transition* getTransition (unsigned int n) const;


  /**
   * Returns the Transition object that belongs to the given index. If the
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the Transition to get.
   *
   * @return the nth Transition in the ListOfTransitions.
   */
  Transition* getTransition (unsigned int n);


  /**
   * Returns the qualitativeSpecies object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Transition to get.
   * 
   * @return Transition in the ListOfTransitions with the given id
   * or NULL if no such Transition exists.
   *
   * @see getTransition(unsigned int n)
   * @see getListOfTransitions()
   */
  Transition* getTransition (const std::string& sid);


  /**
   * Returns the qualitativeSpecies object based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Transition to get.
   * 
   * @return Transition in the ListOfTransitions with the given id 
   * or NULL if no such Transition exists.
   *
   * @see getTransition(unsigned int n)
   * @see getListOfTransitions()
   */
  const Transition* getTransition (const std::string& sid) const;


  /**
   * Adds a copy of the given Transition object to the list of qual.
   *
   * @param transition the Transition object to be added to the list of qual.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int addTransition (const Transition* transition);


  /**
   * Creates a new qual object and adds it to the list of qual objects
   * and returns it.
   *
   * @return a newly created Transition object.
   */
  Transition* createTransition();


  /**
   * Removes the nth Transition object from this plugin object and
   * returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   *  deleting it.
   *
   * @param n the index of the Transition object to remove.
   *
   * @return the Transition object removed.  As mentioned above, the 
   * caller owns the returned object. NULL is returned if the 
   * given index is out of range.
   */
  Transition* removeTransition (unsigned int n);


  /**
   * Removes the Transition object with the given id attribute from 
   * this plugin object and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @param sid the id attribute of the Transition object to remove.
   *
   * @return the Transition object removed.  As mentioned above, the 
   * caller owns the returned object. NULL is returned if the 
   * given index is out of range.
   */
  Transition* removeTransition (const std::string& sid);


  /**
   * Returns the number of Transition object in this plugin object.
   *
   * @return the number of Transition object in this plugin object.
   */
  unsigned int getNumTransitions() const;

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
   * Sets the parent SBML object of this plugin object to
   * this object and child elements (if any).
   * (Creates a child-parent relationship by this plugin object)
   *
   * This function is called when this object is created by
   * the parent element.
   * Subclasses must override this this function if they have one
   * or more child elements.Also, SBasePlugin::connectToParent()
   * must be called in the overridden function.
   *
   * @param sbase the SBase object to use.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToParent (SBase *sbase);
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
  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond */


protected:
  /** @cond doxygenLibsbmlInternal */
  /*-- data members --*/

  ListOfQualitativeSpecies mQualitativeSpecies;
  ListOfTransitions mTransitions;

  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* QualModelPlugin_h */
