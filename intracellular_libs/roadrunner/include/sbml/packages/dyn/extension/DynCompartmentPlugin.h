/**
 * @file:   DynCompartmentPlugin.h
 * @brief:  Implementation of the DynCompartmentPlugin class
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


#ifndef DynCompartmentPlugin_H__
#define DynCompartmentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/dyn/extension/DynExtension.h>
#include <sbml/packages/dyn/sbml/SpatialComponent.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DynCompartmentPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new DynCompartmentPlugin
   */
  DynCompartmentPlugin(const std::string& uri, const std::string& prefix, 
                                 DynPkgNamespaces* dynns);


  /**
   * Copy constructor for DynCompartmentPlugin.
   *
   * @param orig; the DynCompartmentPlugin instance to copy.
   */
  DynCompartmentPlugin(const DynCompartmentPlugin& orig);


   /**
   * Assignment operator for DynCompartmentPlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DynCompartmentPlugin& operator=(const DynCompartmentPlugin& rhs);


   /**
   * Creates and returns a deep copy of this DynCompartmentPlugin object.
   *
   * @return a (deep) copy of this DynCompartmentPlugin object.
   */
  virtual DynCompartmentPlugin* clone () const;


   /**
   * Destructor for DynCompartmentPlugin.
   */
  virtual ~DynCompartmentPlugin();


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


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */


  //---------------------------------------------------------------
  //
  // Functions for interacting with the members of the plugin
  //
  //---------------------------------------------------------------

  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the value of the "cboTerm" attribute of this DynCompartmentPlugin.
   *
   * @return the value of the "cboTerm" attribute of this DynCompartmentPlugin as a string.
   */
  virtual const std::string& getCboTerm() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynCompartmentPlugin's "cboTerm" attribute has been set.
   *
   * @return @c true if this DynCompartmentPlugin's "cboTerm" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetCboTerm() const;


  /**
   * Sets the value of the "cboTerm" attribute of this DynCompartmentPlugin.
   *
   * @param cboTerm; const std::string& value of the "cboTerm" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setCboTerm(const std::string& cboTerm);


  /**
   * Unsets the value of the "cboTerm" attribute of this DynCompartmentPlugin.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetCboTerm();


  /**
   * Returns the  "ListOfSpatialComponents" in this DynCompartmentPlugin object.
   *
   * @return the "ListOfSpatialComponents" attribute of this DynCompartmentPlugin.
   */
  const ListOfSpatialComponents* getListOfSpatialComponents() const;


  /**
   * Returns the  "ListOfSpatialComponents" in this DynCompartmentPlugin object.
   *
   * @return the "ListOfSpatialComponents" attribute of this DynCompartmentPlugin.
   */
  ListOfSpatialComponents* getListOfSpatialComponents();


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents.
   *
   * @param n the index number of the SpatialComponent to get.
   *
   * @return the nth SpatialComponent in the ListOfSpatialComponents within this DynCompartmentPlugin.
   *
   * @see getNumSpatialComponents()
   */
	SpatialComponent* getSpatialComponent(unsigned int n);


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents.
   *
   * @param n the index number of the SpatialComponent to get.
   *
   * @return the nth SpatialComponent in the ListOfSpatialComponents within this DynCompartmentPlugin.
   *
   * @see getNumSpatialComponents()
   */
	const SpatialComponent* getSpatialComponent(unsigned int n) const;


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpatialComponent to get.
   *
   * @return the SpatialComponent in the ListOfSpatialComponents
   * with the given id or NULL if no such
   * SpatialComponent exists.
   *
   * @see getSpatialComponent(unsigned int n)
   *
   * @see getNumSpatialComponents()
   */
	SpatialComponent* getSpatialComponent(const std::string& sid);


  /**
   * Get a SpatialComponent from the ListOfSpatialComponents
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpatialComponent to get.
   *
   * @return the SpatialComponent in the ListOfSpatialComponents
   * with the given id or NULL if no such
   * SpatialComponent exists.
   *
   * @see getSpatialComponent(unsigned int n)
   *
   * @see getNumSpatialComponents()
   */
	const SpatialComponent* getSpatialComponent(const std::string& sid) const;


  /**
   * Adds a copy the given "SpatialComponent" to this DynCompartmentPlugin.
   *
   * @param sc; the SpatialComponent object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int addSpatialComponent(const SpatialComponent* sc);


  /**
   * Get the number of SpatialComponent objects in this DynCompartmentPlugin.
   *
   * @return the number of SpatialComponent objects in this DynCompartmentPlugin
   */
  unsigned int getNumSpatialComponents() const;


  /**
   * Creates a new SpatialComponent object, adds it to this DynCompartmentPlugins
   * ListOfSpatialComponents and returns the SpatialComponent object created. 
   *
   * @return a new SpatialComponent object instance
   *
   * @see addSpatialComponent(const SpatialComponent* sc)
   */
  SpatialComponent* createSpatialComponent();


  /**
   * Removes the nth SpatialComponent from the ListOfSpatialComponents within this DynCompartmentPlugin.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the SpatialComponent to remove.
   *
   * @see getNumSpatialComponents()
   */
	SpatialComponent* removeSpatialComponent(unsigned int n);


  /**
   * Removes the SpatialComponent with the given identifier from the ListOfSpatialComponents within this DynCompartmentPlugin
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the SpatialComponent to remove.
   *
   * @return the SpatialComponent removed. As mentioned above, the caller owns the
   * returned item.
   */
	SpatialComponent* removeSpatialComponent(const std::string& sid);


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

  std::string   mCboTerm;
  ListOfSpatialComponents   mSpatialComponents;

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* DynCompartmentPlugin_H__ */


