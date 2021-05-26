/**
 * @file    Polygon
 * @brief   class representing a polygon
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
 * @class Polygon
 * @brief class representing a polygon from the SBML render extension
 *
 * The Polygon is very similar to the RenderCurve class. The only difference is that in the 
 * polygon the end point of the last element in the curve segment list is
 * automatically connected to the start point of the first element.
 *
 * Since a polygon is a closed shape and doesn't really have a start or an end, it does not get
 * decorations as the RenderCurve does.
 *
 * So a polygon is always closed and can therefor have a fill style and fill style related attributes.
 * Those attributes are inherited from Polygons base class GraphicalPrimitive2D. 
 */

#ifndef Polygon_H__
#define Polygon_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/ListOfCurveElements.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class RenderPoint;
class RenderCubicBezier;

class LIBSBML_EXTERN Polygon : public GraphicalPrimitive2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
  ListOfCurveElements mListOfElements;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new Polygon object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Polygon (unsigned int level      = RenderExtension::getDefaultLevel(),
           unsigned int version    = RenderExtension::getDefaultVersion(),
           unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Polygon object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Polygon (RenderPkgNamespaces* renderns);

  /**
   * Creates a new Polygon object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Polygon object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Polygon
   * object to be instantiated.
   */
  Polygon(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this Polygon object.
   */
  virtual ~Polygon ();


#ifndef OMIT_DEPRECATED
  /**
   * Instanciates a polygon with the given @p id and no elements.
   * All attributes inherited from GraphicalPrimitive are set as described
   * in the corresponding constructor of that class (@see GraphicalPrimitive2D)
   *
   * @param id id string for the polygon
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Polygon(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Returns the number of segments.
   *
   * @return number of elements in the polygon.
   */
  unsigned int getNumElements() const;

  /**
   * Returns a const pointer to the list of segments.
   *
   * @return const pointer to the ListOfCurveElements object for the Polygon.
   */
  ListOfCurveElements* getListOfElements();

  /**
   * Returns a pointer to the list of segments.
   *
   * @return pointer to the ListOfCurveElements object for the Polygon.
   */
  const ListOfCurveElements* getListOfElements() const;

  /**
   * Creates a new point element.
   * The element is added to and owned by the polygon.
   *
   * @return The newly created RenderCubicBezier object.
   */
  RenderPoint* createPoint();

  /**
   * Creates a new bezier element.
   * The element is added to and owned by the polygon.
   *
   * @return The newly created RenderCubicBezier object.
   */
  RenderCubicBezier* createCubicBezier();


  /**
   * Returns a pointer to the segment with the given index or NULL if
   * the id is invalid.
   *
   * @param index the index of the element to be returned
   *
   * @return a pointer to the element with the given index or NULL 
   * if the index was out of bounds.
   */
  RenderPoint* getElement(unsigned int index);

  /**
   * Returns a const pointer to the segment with the given index or NULL if
   * the id is invalid.
   *
   * @param index the index of the element to be returned
   *
   * @return a const pointer to the element with the given index or NULL 
   * if the index was out of bounds.
   */
  const RenderPoint* getElement(unsigned int index) const;

  /**
   * Adds a copy of the given segment to the end of the list of
   * segments.
   *
   * @param cs pointer to the RenderPoint object to be added to the end of the elements list.
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
   * one contained in this Polygon.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the Polygon</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see Polygon::createPoint()
   * or Polygon::createCubicBezier()
   * for methods that do not lead to these issues.
   *
   * @see createPoint()
   * @see createCubicBezier()
   */
  int addElement(const RenderPoint* cs);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  bool accept(SBMLVisitor& visitor) const;

  /**
   * Creates and returns a deep copy of the Polygon object.
   *
   * @return a (deep) copy of this Polygon
   */
  Polygon* clone() const;

  /**
   * Returns the XML element name of this object, which for
   * Polygon, is always @c "polygon".
   * 
   * @return the name of this element, i.e., @c "polygon".
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
   * Creates an XMLNode object from this Polygon object.
   *
   * @return the XMLNode with the XML representation for the 
   * Polygon object.
   */
  XMLNode toXML() const;

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

#endif /* Polygon_H__ */
