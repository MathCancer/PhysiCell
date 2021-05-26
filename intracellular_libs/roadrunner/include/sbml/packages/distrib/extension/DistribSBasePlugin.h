/**
 * @file:   DistribSBasePlugin.h
 * @brief:  Implementation of the DistribSBasePlugin class
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


#ifndef DistribSBasePlugin_H__
#define DistribSBasePlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/distrib/sbml/Uncertainty.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DistribSBasePlugin : public SBasePlugin
{
public:

  /**
   * Creates a new DistribSBasePlugin
   */
  DistribSBasePlugin(const std::string& uri, const std::string& prefix, 
                                 DistribPkgNamespaces* distribns);


  /**
   * Copy constructor for DistribSBasePlugin.
   *
   * @param orig; the DistribSBasePlugin instance to copy.
   */
  DistribSBasePlugin(const DistribSBasePlugin& orig);


   /**
   * Assignment operator for DistribSBasePlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DistribSBasePlugin& operator=(const DistribSBasePlugin& rhs);


   /**
   * Creates and returns a deep copy of this DistribSBasePlugin object.
   *
   * @return a (deep) copy of this DistribSBasePlugin object.
   */
  virtual DistribSBasePlugin* clone () const;


   /**
   * Destructor for DistribSBasePlugin.
   */
  virtual ~DistribSBasePlugin();


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
   * Returns the Uncertainty from this DistribSBasePlugin object.
   *
   * @return the Uncertainty from object in this DistribSBasePlugin object.
   */
  const Uncertainty* getUncertainty () const;


  /**
   * Predicate returning @c true or @c false depending on whether the
    "Uncertainty" element has been set.
   *
   * @return @c true if the "Uncertainty" element has been set,
   * otherwise @c false is returned.
   */
  bool isSetUncertainty() const;


  /**
   * Sets the Uncertainty element in this DistribSBasePlugin object.
   *
   * @param uncertainty the uncertainty* to be set.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   */
  int setUncertainty (const Uncertainty* uncertainty);


  /**
   * Creates a new Uncertainty object and adds it to the DistribSBasePlugin object.
   *
   * @return the newly created Uncertainty object.
   */
  Uncertainty* createUncertainty ();


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond doxygenLibsbmlInternal */


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

  Uncertainty* mUncertainty;

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* DistribSBasePlugin_H__ */


