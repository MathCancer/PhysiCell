/**
 * @file    GradientBase.h
 * @brief   abstract base class for gradient definitions
 * @author  Ralph Gauges
 * @author  Frank T. Bergmann
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
 * Copyright (C) 2011-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright 2010 Ralph Gauges
 *     Group for the modeling of biological processes 
 *     University of Heidelberg
 *     Im Neuenheimer Feld 267
 *     69120 Heidelberg
 *     Germany
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class GradientBase
 * @brief abstract base class for linear and radial gradients
 * 
 * The base class implements common structures to both gradient classes.
 * Both gradients have an id attribute which is used to reference a gradient
 * within other render extension constructs. The id of a gradient can be used
 * to define the fill style of 2D objects like e.g. rectangles.
 *
 * Further both gradient classes have a ListOfGradientStop objects which holds
 * the GradientStop objects that define the gradient and bothe classes have an 
 * attribute called spreadMethod which defines how a gradient is applied to an
 * object.
 *
 *
 * @class ListOfGradientDefinitions
 * @brief a container class that holds a list of gradient definitions.
 * There are two types of gradient definitions: LinearGradient and
 * RadialGradient
 *
 * Each RenderInformation object can have it's own list of gradient definitions.
 *
 */

#ifndef GradientBase_H__
#define GradientBase_H__

#include <sbml/common/sbmlfwd.h>

#include "sbml/SBase.h"
#include <sbml/ListOf.h>

#include <sbml/packages/render/sbml/GradientStop.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>
#include <vector>
#include <sbml/packages/render/extension/RenderExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN GradientBase : public SBase
{
public:
  enum SPREADMETHOD
  {
    PAD,
    REFLECT,
    REPEAT
  };


protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId;
  SPREADMETHOD mSpreadMethod;
  ListOfGradientStops mGradientStops;
  /** @endcond */

  /**
   * Creates a new GradientBase object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  GradientBase (unsigned int level      = RenderExtension::getDefaultLevel(),
                unsigned int version    = RenderExtension::getDefaultVersion(),
                unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new GradientBase object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  GradientBase (RenderPkgNamespaces* renderns);

  /**
   * Creates a new GradientBase object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * GradientBase object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the GradientBase
   * object to be instantiated.
   */
  GradientBase(const XMLNode& node, unsigned int l2version=4);


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a GradientBase with no gradient stops.
   * The spreadMethod attribute is set to GradientBase::PAD and the id is
   * set to the given value.
   * This object is not valid until it gets at least two gradient stops.
   *
   * @param id The id for the gradient definition object
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  GradientBase(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

public:

  /**
   * Destroy this GradientBase object.
   */
  virtual ~GradientBase ();


  /**
   * Returns the spreadmethod of the gradient.
   * Valid values are GradientBase::PAD, GradientBase::REFLECT
   * and GradientBase::REPEAT.
   *
   * @return the spread method for the gradient object.
   */
  SPREADMETHOD getSpreadMethod() const;

  /**
   * Sets the spread method.
   * Valid values are GradientBase::PAD, GradientBase::REFLECT
   * and GradientBase::REPEAT.
   *
   * @param method The new spread method for the gradient.
   */
  void setSpreadMethod(SPREADMETHOD method);

  /**
   * Returns the number of gradient stops.
   * A valid gradient needs at least two gradient stops
   *
   * @return the number of gradient stops in the gradient.
   */
  unsigned int getNumGradientStops() const;

  /**
   * Returns a pointer to the gradient stop vector.
   *
   * @return a pointer to the ListOfGradientStops object
   * for the gradient.
   */
  ListOfGradientStops* getListOfGradientStops();

  /**
   * Returns a const pointer to the gradient stop vector.
   *
   * @return a const pointer to the ListOfGradientStops object
   * for the gradient.
   */
  const ListOfGradientStops* getListOfGradientStops() const;

  /**
   * Returns a pointer to the gradient stop with the given index or NULL
   * if the index is invalid.
   *
   * @param i index of the gradient stop to be returned. The index has to be between 0 and
   * getNumGradientStops() - 1.
   *
   * @return a pointer to the gradient stop with the given index
   * or NULL if the index was out of bounds.
   */
  GradientStop* getGradientStop(unsigned int i);

  /**
   * Returns a const pointer to the gradient stop with the given index or NULL
   * if the index is invalid.
   *
   * @param i index of the gradient stop to be returned. The index has to be between 0 and
   * getNumGradientStops() - 1.
   *
   * @return a const pointer to the gradient stop with the given index
   * or NULL if the index was out of bounds.
   */
  const GradientStop* getGradientStop(unsigned int i) const;

  /**
   * Creates a new GradientStop. The new GradientStop object is automatically
   * added to the gradient and the gradient own the object-
   *
   * @return a pointer to the newly created GradientStop.
   */
  GradientStop* createGradientStop();

  /**
   * Adds a copy of the given GradientStop object to the end
   * of the list of gradient stops.
   *
   * @param pStop a const pointer to the new gradient stop 
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_LEVEL_MISMATCH
   * @li LIBSBML_VERSION_MISMATCH
   * @li LIBSBML_OPERATION_FAILED
   *
   * @note This method should be used with some caution.  The fact that
   * this method @em copies the object passed to it means that the caller
   * will be left holding a physically different object instance than the
   * one contained in this GradientBase.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the GradientBase</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see GradientBase::createGradientStop()
   * for a method that does not lead to these issues.
   *
   * @see createGradientStop()
   */
  int addGradientStop(const GradientStop* pStop);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


  /**
   * Creates and returns a deep copy of this GradientBase object.
   *
   * This method is purely abstract and has to be implemented by derived
   * classes.
   *
   * @return a (deep) copy of this GradientBase.
   */
  virtual GradientBase* clone () const = 0;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   * 
   * NOTE: this function is only ever going to be called from the constructor
   */
  virtual const std::string& getElementName () const;

  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;


  /**
   * Creates an XMLNode object from this GradientBase object.
   *
   * @return the XMLNode with the XML representation for the 
   * GradientBase object.
   *
   * This method is purely abstract and needs to be implemented
   * by derived classes.
   */
  virtual XMLNode toXML() const=0;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor, assignment operator.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Enables/Disables the given package with this element and child
   * elements (if any).
   * (This is an internal implementation for enablePakcage function)
   *
   * @note Subclasses in which one or more child elements are defined
   * must override this function.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond */


  /**
   * Returns the value of the "id" attribute of this GradientBase object.
   *
   * @return the id of the gradient
   */
  const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * GradientBase's "id" attribute has been set.
   *
   * @return returns true or false depending on whether the id on the 
   * GradientBase has been set.
   */
  bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this GradientBase.
   *
   * @param id the new id for the GradientBase 
   *
   * @return status if the operation succeeded
   */
  int setId (const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this GradientBase.
   */
  virtual int unsetId ();


  /**
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
  virtual void setParentSBMLObject (SBase* sb);

  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * attributes
   */
  virtual bool hasRequiredAttributes() const ;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * elements
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond */



protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes);
  /** @endcond */


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
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the attribute string for the spread method set on the
   * gradient.
   */
  std::string getSpreadMethodString() const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Converts the given string into a spread method.
   * If the string does not represnt a valid spread method, PAD is
   * returned.
   */
  static SPREADMETHOD getSpreadMethodForString(const std::string& s);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * This method is used when writing out gradietns to XML.
   * I writes out the attributes and children that re common to linear and radial gradient.
   */
  static void addGradientAttributesAndChildren(const GradientBase& gradient,XMLAttributes& att,XMLNode& node);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */

};


class LIBSBML_EXTERN ListOfGradientDefinitions : public ListOf
{
public:
  /**
   * Creates a new ListOfGradientDefinitions object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfGradientDefinitions object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfGradientDefinitions
   * object to be instantiated.
   */
  ListOfGradientDefinitions(const XMLNode& node, unsigned int l2version=4);  

  /**
   * Creates and returns a deep copy of the ListOfGradientDefinitions object.
   *
   * @return a (deep) copy of this ListOfGradientDefinitions
   */
  virtual ListOfGradientDefinitions* clone () const;

  /**
   * Constructor which instantiates an empty ListOfGradientDefinitions object.
   */
  ListOfGradientDefinitions(unsigned int level      = RenderExtension::getDefaultLevel(),
                            unsigned int version    = RenderExtension::getDefaultVersion(),
                            unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfGradientDefinitions(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor. Creates a copy of this ListOfGradientDefinitions object.
   */
  ListOfGradientDefinitions(const ListOfGradientDefinitions& source);

  /**
   * Assignment operator for ListOfGradientDefinitions objects..
   */
  ListOfGradientDefinitions& operator=(const ListOfGradientDefinitions& source);

  /**
   * Get the type code of the objects contained in this ListOf.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   * 
   * @return the SBML type code for the objects contained in this ListOf
   * instance, or @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   */
  virtual int getItemTypeCode () const;

  virtual bool isValidTypeForList(SBase * item);

  /**
   * Creates an XMLNode object from this ListOfGradientDefinitions object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfGradientDefinitions object.
   */
  XMLNode toXML() const;

  /**
   * Returns the XML element name of this object, which for
   * ListOfGradientDefinitions, is always @c "listOfGradientDefinitions".
   * 
   * @return the name of this element, i.e., @c "listOfGradientDefinitions".
   */
  const std::string& getElementName() const;

  /**
   * Returns a pointer to the GradientBase with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the GradientBase object to be returned
   * 
   * @return pointer to the GradientBase at the given index or NULL.
   */
  GradientBase* get(unsigned int i);

  /**
   * Returns a const pointer to the GradientBase with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the GradientBase object to be returned
   * 
   * @return const pointer to the GradientBase at the given index or NULL.
   */
  const GradientBase* get(unsigned int i) const;

  /**
   * Returns a pointer to the GradientBase with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the GradientBase object to be returned
   * 
   * @return pointer to the GradientBase at the given @p id or @c NULL.
   */
  GradientBase* get(const std::string& id);

  /**
   * Returns a const pointer to the GradientBase with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the GradientBase object to be returned
   * 
   * @return const pointer to the GradientBase at the given @p id or @c NULL.
   */
  const GradientBase* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfGradientDefinitions items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual GradientBase* remove (unsigned int n);


  /**
   * Removes item in this ListOfGradientDefinitions items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual GradientBase* remove (const std::string& sid);


protected:

  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  static const std::string ELEMENT_NAME;
  /** @endcond */
};



LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* GradientBase_H__ */
