/**
 * @file    RenderCurve.h
 * @brief   class for representing a curve in the render extension
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
 * @class RenderCurve
 * @brief implementation of the Curve concept from the SBML render extension
 *
 * The curve concept in the SBML render extension is similar to the curves in the SBML layout.
 * Each curve consists of a number of either straight line segments or cubic bezier elements.
 * The two element types can also by mixed in a single curve object.
 *
 * In contrast to layout curves, render curves can not have gaps and the individual coordinates of the 
 * curve elements can be specified as a combination of absolute and relative values.
 *
 * Another difference to layout curves is the fact that render curves can specify decorations to be applied
 * to the start and/or the end of the curve (@see LineEnding).
 *
 * Since RenderCurve is derived from GraphicalPrimitive1D, it inherits all its attributes and methods. 
 */

#ifndef RenderCurve_H__
#define RenderCurve_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive1D.h>
#include <sbml/packages/render/sbml/ListOfCurveElements.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class RenderCubicBezier;
class RenderPoint;

class LIBSBML_EXTERN RenderCurve : public GraphicalPrimitive1D
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mStartHead;
  std::string mEndHead;
  ListOfCurveElements mListOfElements;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new RenderCurve object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RenderCurve (unsigned int level      = RenderExtension::getDefaultLevel(),
                   unsigned int version    = RenderExtension::getDefaultVersion(),
                   unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RenderCurve object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RenderCurve (RenderPkgNamespaces* renderns);

  /**
   * Creates a new RenderCurve object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RenderCurve object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RenderCurve
   * object to be instantiated.
   */
  RenderCurve(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this RenderCurve object.
   */
  virtual ~RenderCurve ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates an empty curve object with the given @p id.
   * The decorations  are unset and there are no curve elements.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  RenderCurve(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets the id of the start head.
   *
   * @param The id of a LineEnding object to be applied to the start of the curve.
   */
  void setStartHead(const std::string& startHead);

  /**
   * Sets the id of the end head.
   *
   * @param The id of a LineEnding object to be applied to the end of the curve.
   */
  void setEndHead(const std::string& endHead);

  /**
   * Returns the id of the LineEnding object to be applied to the start of the curve.
   *
   * @return id of the LineEnding for the start of the curve.
   */
  const std::string& getStartHead() const;

  /**
   * Returns the id of the LineEnding object to be applied to the end of the curve.
   *
   * @return id of the LineEnding for the end of the curve.
   */
  const std::string& getEndHead() const;

  /**
   * Returns the number of curve segments.
   *
   * @return number of elements in the curve.
   */
  unsigned int getNumElements() const;

  /**
   * Creates a new bezier element.
   * The element is added to and owned by the curve.
   *
   * @return The newly created RenderCubicBezier object.
   */
  RenderCubicBezier* createCubicBezier();

  /**
   * Creates a new point element.
   * The element is added to and owned by the curve.
   *
   * @return The newly created RenderCubicBezier object.
   */
  RenderPoint* createPoint();


  /**
   * Returns a const pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   *
   * @param index the index of the curve element to be returned
   *
   * @return a const pointer to the curve element with the given index or NULL 
   * if the index was out of bounds.
   */
  const RenderPoint* getElement(unsigned int index) const;

#ifndef OMIT_DEPRECATED
  /**
   * Returns a pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   *
   * This method call is deprecated, please use getElement instead.
   *
   * @param index the index of the curve element to be returned
   *
   * @return a pointer to the curve element with the given index or NULL 
   * if the index was out of bounds.
   */
  RenderPoint* getCurveElement(unsigned int index);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Returns a const pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   *
   * This method call is deprecated, please use getElement instead.
   *
   * @param index the index of the curve element to be returned
   *
   * @return a const pointer to the curve element with the given index or NULL 
   * if the index was out of bounds.
   */
  const RenderPoint* getCurveElement(unsigned int index) const;
#endif // OMIT_DEPRECATED

  /**
   * Returns a pointer to the curve segment with the given index or NULL if
   * the id is invalid.
   *
   * @param index the index of the curve element to be returned
   *
   * @return a pointer to the curve element with the given index or NULL 
   * if the index was out of bounds.
   */
  RenderPoint* getElement(unsigned int index);


  /**
   * Adds a copy of the given curve segment to the end of the list of
   * curve segments.
   *
   * @param cs pointer to the RenderPoint object to be added to the end of the curve elements list.
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
   * one contained in this RenderCurve.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the RenderCurve</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see RenderCurve::createPoint()
   * or RenderCurve::createCubicBezier()
   * for methods that do not lead to these issues.
   *
   * @see createPoint()
   * @see createCubicBezier()
   */
  int addElement(const RenderPoint* cs);

  /**
   * Removes the curve segment with the given index.
   * If the index is valid, a pointer to the removed element is returned
   * and the caller is responsible for deleting the object.
   * If the index is not valid, @c NULL is returned.
   *
   * @param i index of element to be removed.
   *
   * @return pointer to removed element.
   */
  RenderPoint* removeElement(unsigned int i);

  /**
   * Returns a const pointer to the list of curve segments.
   *
   * @return const pointer to the ListOfCurveElements object for the RenderCurve.
   */
  const ListOfCurveElements* getListOfElements() const;

  /**
   * Returns a pointer to the list of curve segments.
   *
   * @return pointer to the ListOfCurveElements object for the RenderCurve.
   */
  ListOfCurveElements* getListOfElements();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


/**
   * Renames all the @c SIdRef attributes on this element, including any
   * found in MathML content (if such exists).
   *
   * This method works by looking at all attributes and (if appropriate)
   * mathematical formulas, comparing the identifiers to the value of @p
   * oldid.  If any matches are found, the matching identifiers are replaced
   * with @p newid.  The method does @em not descend into child elements.
   *
   * @param oldid the old identifier
   * @param newid the new identifier
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


	/**
	 * Accepts the given SBMLVisitor.
	 *
	 * @return the result of calling <code>v.visit()</code>, which indicates
	 * whether or not the Visitor would like to visit the SBML object's next
	 * sibling object (if available).
	 */
	bool accept(SBMLVisitor& visitor) const;

 /**
  * Creates and returns a deep copy of the RenderCurve object.
  *
  * @return a (deep) copy of this RenderCurve
  */
	RenderCurve* clone() const;

 /**
  * Returns the XML element name of this object, which for
  * RenderCurve, is always @c "curve".
  * 
  * @return the name of this element, i.e., @c "curve".
  */
	const std::string& getElementName() const;
        
 /**
  * Returns the libSBML type code for this %SBML object.
  * 
  * @if clike LibSBML attaches an identifying code to every
  * kind of SBML object.  These are known as <em>SBML type codes</em>.
  * The set of possible type codes is defined in the enumeration
  * #SBMLTypeCode_t.  The names of the type codes all begin with the
  * characters @c SBML_. @endif@if java LibSBML attaches an
  * identifying code to every kind of SBML object.  These are known as
  * <em>SBML type codes</em>.  In other languages, the set of type codes
  * is stored in an enumeration; in the Java language interface for
  * libSBML, the type codes are defined as static integer constants in
  * interface class {@link libsbmlConstants}.  The names of the type codes
  * all begin with the characters @c SBML_. @endif
  *
  * @return the SBML type code for this object, or @c SBML_UNKNOWN (default).
  *
  * @see getElementName()
  */
 virtual int getTypeCode() const;

 /**
  * Returns true if the start head is set or false otherwise.
  * The start decoration is considered set if the string is not empty and if
  * it is not the string "none"
  *
  * @return true is the start decoration id is set
  */
 bool isSetStartHead() const;

 /**
  * Returns true if the end head is set or false otherwise.
  * The end decoration is considered set if the string is not empty and if
  * it is not the string "none"
  *
  * @return true is the end decoration id is set
  */
 bool isSetEndHead() const;

 /**
  * Creates an XMLNode object from this RenderCurve object.
  *
  * @return the XMLNode with the XML representation for the 
  * RenderCurve object.
  */
 virtual XMLNode toXML() const;

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
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
   virtual void setParentSBMLObject (SBase* sb);


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


  friend class RenderGroup;

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
	 * @return the SBML object corresponding to next XMLToken in the
	 * XMLInputStream or NULL if the token was not recognized.
	 */
	virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */


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
};
LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* RenderCurve_H__ */

