/**
 * @file:   DynEventPlugin.h
 * @brief:  Implementation of the DynEventPlugin class
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


#ifndef DynEventPlugin_H__
#define DynEventPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/dyn/extension/DynExtension.h>
#include <sbml/packages/dyn/sbml/DynElement.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DynEventPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new DynEventPlugin
   */
  DynEventPlugin(const std::string& uri, const std::string& prefix, 
                                 DynPkgNamespaces* dynns);


  /**
   * Copy constructor for DynEventPlugin.
   *
   * @param orig; the DynEventPlugin instance to copy.
   */
  DynEventPlugin(const DynEventPlugin& orig);


   /**
   * Assignment operator for DynEventPlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DynEventPlugin& operator=(const DynEventPlugin& rhs);


   /**
   * Creates and returns a deep copy of this DynEventPlugin object.
   *
   * @return a (deep) copy of this DynEventPlugin object.
   */
  virtual DynEventPlugin* clone () const;


   /**
   * Destructor for DynEventPlugin.
   */
  virtual ~DynEventPlugin();


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
   * Returns the value of the "cboTerm" attribute of this DynEventPlugin.
   *
   * @return the value of the "cboTerm" attribute of this DynEventPlugin as a string.
   */
  virtual const std::string& getCboTerm() const;


  /**
   * Returns the value of the "applyToAll" attribute of this DynEventPlugin.
   *
   * @return the value of the "applyToAll" attribute of this DynEventPlugin as a boolean.
   */
  virtual bool getApplyToAll() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynEventPlugin's "cboTerm" attribute has been set.
   *
   * @return @c true if this DynEventPlugin's "cboTerm" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetCboTerm() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DynEventPlugin's "applyToAll" attribute has been set.
   *
   * @return @c true if this DynEventPlugin's "applyToAll" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetApplyToAll() const;


  /**
   * Sets the value of the "cboTerm" attribute of this DynEventPlugin.
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
   * Sets the value of the "applyToAll" attribute of this DynEventPlugin.
   *
   * @param applyToAll; bool value of the "applyToAll" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setApplyToAll(bool applyToAll);


  /**
   * Unsets the value of the "cboTerm" attribute of this DynEventPlugin.
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
   * Unsets the value of the "applyToAll" attribute of this DynEventPlugin.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetApplyToAll();


  /**
   * Returns the  "ListOfDynElements" in this DynEventPlugin object.
   *
   * @return the "ListOfDynElements" attribute of this DynEventPlugin.
   */
  const ListOfDynElements* getListOfElements() const;


  /**
   * Returns the  "ListOfDynElements" in this DynEventPlugin object.
   *
   * @return the "ListOfDynElements" attribute of this DynEventPlugin.
   */
  ListOfDynElements* getListOfElements();


  /**
   * Get a Element from the ListOfDynElements.
   *
   * @param n the index number of the Element to get.
   *
   * @return the nth Element in the ListOfDynElements within this DynEventPlugin.
   *
   * @see getNumElements()
   */
	DynElement* getElement(unsigned int n);


  /**
   * Get a Element from the ListOfDynElements.
   *
   * @param n the index number of the Element to get.
   *
   * @return the nth Element in the ListOfDynElements within this DynEventPlugin.
   *
   * @see getNumElements()
   */
	const DynElement* getElement(unsigned int n) const;


  /**
   * Get a Element from the ListOfDynElements
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Element to get.
   *
   * @return the Element in the ListOfDynElements
   * with the given id or NULL if no such
   * Element exists.
   *
   * @see getElement(unsigned int n)
   *
   * @see getNumElements()
   */
	DynElement* getElement(const std::string& sid);


  /**
   * Get a Element from the ListOfDynElements
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Element to get.
   *
   * @return the Element in the ListOfDynElements
   * with the given id or NULL if no such
   * Element exists.
   *
   * @see getElement(unsigned int n)
   *
   * @see getNumElements()
   */
	const DynElement* getElement(const std::string& sid) const;


  /**
   * Adds a copy the given "DynElement" to this DynEventPlugin.
   *
   * @param de; the DynElement object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int addElement(const DynElement* de);


  /**
   * Get the number of DynElement objects in this DynEventPlugin.
   *
   * @return the number of DynElement objects in this DynEventPlugin
   */
  unsigned int getNumElements() const;


  /**
   * Creates a new DynElement object, adds it to this DynEventPlugins
   * ListOfDynElements and returns the DynElement object created. 
   *
   * @return a new DynElement object instance
   *
   * @see addElement(const DynElement* de)
   */
  DynElement* createElement();


  /**
   * Removes the nth Element from the ListOfDynElements within this DynEventPlugin.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Element to remove.
   *
   * @see getNumElements()
   */
	DynElement* removeElement(unsigned int n);


  /**
   * Removes the Element with the given identifier from the ListOfDynElements within this DynEventPlugin
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Element to remove.
   *
   * @return the Element removed. As mentioned above, the caller owns the
   * returned item.
   */
	DynElement* removeElement(const std::string& sid);


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
  bool          mApplyToAll;
  bool          mIsSetApplyToAll;
  ListOfDynElements   mElements;

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* DynEventPlugin_H__ */


