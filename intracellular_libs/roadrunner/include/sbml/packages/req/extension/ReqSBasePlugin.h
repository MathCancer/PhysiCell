/**
 * @file:   ReqSBasePlugin.h
 * @brief:  Implementation of the ReqSBasePlugin class
 * @author: SBMLTeam
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
 */


#ifndef ReqSBasePlugin_H__
#define ReqSBasePlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/req/sbml/ChangedMath.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ReqSBasePlugin : public SBasePlugin
{
public:

  /**
   * Creates a new ReqSBasePlugin
   */
  ReqSBasePlugin(const std::string& uri, const std::string& prefix, 
                                 ReqPkgNamespaces* reqns);


  /**
   * Copy constructor for ReqSBasePlugin.
   *
   * @param orig; the ReqSBasePlugin instance to copy.
   */
  ReqSBasePlugin(const ReqSBasePlugin& orig);


   /**
   * Assignment operator for ReqSBasePlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  ReqSBasePlugin& operator=(const ReqSBasePlugin& rhs);


   /**
   * Creates and returns a deep copy of this ReqSBasePlugin object.
   *
   * @return a (deep) copy of this ReqSBasePlugin object.
   */
  virtual ReqSBasePlugin* clone () const;


   /**
   * Destructor for ReqSBasePlugin.
   */
  virtual ~ReqSBasePlugin();


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


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */


  /**
   * Checks if this plugin object has all the required elements.
   *
   * Subclasses must override this method 
   * if they have their specific elements.
   *
   * @return true if this plugin object has all the required elements
   * otherwise false will be returned.
   */
  virtual bool hasRequiredElements () const;


  //---------------------------------------------------------------


  //---------------------------------------------------------------
  //
  // Functions for interacting with the members of the plugin
  //
  //---------------------------------------------------------------

  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the ListOfChangedMaths in this plugin object.
   *
   * @return ListOfChangedMaths object in this plugin object.
   */
  const ListOfChangedMaths* getListOfChangedMaths () const;


  /**
   * Returns the ListOfChangedMaths in this plugin object.
   *
   * @return ListOfChangedMaths object in this plugin object.
   */
  ListOfChangedMaths* getListOfChangedMaths ();


  /**
   * Returns the ChangedMath object that belongs to the given index. If the 
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the ChangedMath to get
   *
   * @return the nth ChangedMath in the ListOfChangedMaths
   */
  const ChangedMath* getChangedMath(unsigned int n) const;


  /**
   * Returns the ChangedMath object that belongs to the given index. If the 
   * index is invalid, NULL is returned.
   *
   * @param n the index number of the ChangedMath to get
   *
   * @return the nth ChangedMath in the ListOfChangedMaths
   */
  ChangedMath* getChangedMath(unsigned int n);


  /**
   * Returns the ChangedMath object based on its identifier.
   *
   * @param sid a string representing the id of the ChangedMath to get
   *
   * @return ChangedMath in the ListOfChangedMaths with the given id
   * or NULL if no such ChangedMath exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  const ChangedMath* getChangedMath(const std::string& sid) const;


  /**
   * Returns the ChangedMath object based on its identifier.
   *
   * @param sid a string representing the id of the ChangedMath to get
   *
   * @return ChangedMath in the ListOfChangedMaths with the given id
   * or NULL if no such ChangedMath exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  ChangedMath* getChangedMath(const std::string& sid);


  /**
   * Adds a copy of the given ChangedMath to the ListOfChangedMaths in this plugin object.
   *
   * @param changedMath the changedMath to be added.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   */
  int addChangedMath (const ChangedMath* changedMath);


  /**
   * Creates a new ChangedMath object and adds it to the ListOfChangedMaths in this plugin object.
   *
   * @return the newly created ChangedMath object.
   */
  ChangedMath* createChangedMath ();


  /**
   * Removes the nth ChangedMath object from this plugin object
   * and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @param n the index of the ChangedMath to remove
   *
   * @return the ChangedMath object removed 
   * or NULL index was out of range.
   */
  ChangedMath* removeChangedMath(unsigned int n);


  /**
   * Removes the ChangedMath object with the given id from this plugin object
   * and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @param sid a string representing the id of the ChangedMath to remove
   *
   * @return the ChangedMath object removed 
   * or NULL if no such ChangedMath exists.
   */
  ChangedMath* removeChangedMath(const std::string& sid);


  /**
   * Returns the number of ChangedMath objects in this plugin object.
   *
   * @return the number of ChangedMath objects in this plugin object.
   */
  unsigned int getNumChangedMaths () const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond doxygenLibsbmlInternal */



  /** @cond doxygenLibsbmlInternal */
  
  virtual void connectToChild ();
  
  /** @endcond */

 /** @cond doxygenLibsbmlInternal */

  virtual void connectToParent (SBase* sbase);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  virtual bool accept (SBMLVisitor& v) const;

  /** @endcond doxygenLibsbmlInternal */


protected:

  /** @cond doxygenLibsbmlInternal */

  ListOfChangedMaths mChangedMaths;

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* ReqSBasePlugin_H__ */


