/**
 * @file    Rectangle.h
 * @brief   class representing a rectangle with or without rounded corners
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
 */

#ifndef Rectangle_H__
#define Rectangle_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN Rectangle : public GraphicalPrimitive2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mX;
  RelAbsVector mY;
  RelAbsVector mZ;
  RelAbsVector mWidth;
  RelAbsVector mHeight;
  RelAbsVector mRX;
  RelAbsVector mRY;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new Rectangle object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Rectangle (unsigned int level      = RenderExtension::getDefaultLevel(),
             unsigned int version    = RenderExtension::getDefaultVersion(),
             unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Rectangle object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Rectangle (RenderPkgNamespaces* renderns);

  /**
   * Creates a new Rectangle object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Rectangle object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Rectangle
   * object to be instantiated.
   */
  Rectangle(const XMLNode& node, unsigned int l2version=4);



  /**
   * Destroy this Rectangle object.
   */
  virtual ~Rectangle ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new Rectangle object.
   * All attributes are set as described for the default constructor
   * of GraphicalPrimitive2D.
   *
   * The id is set to the given string and all rectangle specific attributes are set to 0.
   *
   * @param id id string for the rectangle
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Rectangle(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new Rectangle object.
   * All attributes are set as described for the default constructor
   * of GraphicalPrimitive2D.
   *
   * The id is set to the given string and all rectangle specific attributes
   * are set to the given values.
   *
   * @param id id string for the rectangle
   * @param x x coordinate of the position 
   * @param y y coordinate of the position 
   * @param z z coordinate of the position 
   * @param w w width
   * @param h h height
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Rectangle(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z,const RelAbsVector& w,const RelAbsVector& h);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new Rectangle object.
   * All attributes are set as described for the default constructor
   * of GraphicalPrimitive2D.
   *
   * The id is set to the given string and all rectangle specific attributes
   * are set to the given values. The z coordinate of the position is set to 0.
   *
   * @param id id string for the rectangle
   * @param x x coordinate of the position 
   * @param y y coordinate of the position 
   * @param w w width
   * @param h h height
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Rectangle(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& w,const RelAbsVector& h);
#endif // OMIT_DEPRECATED

  /**
   * Sets the position and the size of the Rectangle within the viewport.
   *
   * @param x x coordinate of the position 
   * @param y y coordinate of the position 
   * @param z z coordinate of the position 
   * @param w w width
   * @param h h height
   */
  void setCoordinatesAndSize(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z,const RelAbsVector& w,const RelAbsVector& h);

  /**
   * Sets the position of the Rectangle within the viewport.
   *
   * @param x x coordinate of the position 
   * @param y y coordinate of the position 
   * @param z z coordinate of the position 
   */
  void setCoordinates(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z);

  /**
   * Sets the size of the Rectangle 
   *
   * @param w w width
   * @param h h height
   */
  void setSize(const RelAbsVector& w,const RelAbsVector& h);

  /**
   * Sets the siwidth of the Rectangle 
   *
   * @param w w width
   */
  void setWidth(const RelAbsVector& w);

  /**
   * Sets the height of the Rectangle 
   *
   * @param h h height
   */
  void setHeight(const RelAbsVector& h);

  /**
   * Sets the two corner radii of the rectangle
   *
   * @param rx corner radius along the x axis
   * @param ry corner radius along the y axis
   */
  void setRadii(const RelAbsVector& rx,const RelAbsVector& ry);

  /**
   * Sets the corner radius along the x axis
   *
   * @param rx corner radius along the x axis
   */
  void setRadiusX(const RelAbsVector& rx);

  /**
   * Sets the corner radius along the y axis
   *
   * @param ry corner radius along the y axis
   */
  void setRadiusY(const RelAbsVector& ry);

  /**
   * Sets the x position of the Rectangle within the viewport.
   *
   * @param x x coordinate of the position 
   */
  void setX(const RelAbsVector& x);

  /**
   * Sets the y position of the Rectangle within the viewport.
   *
   * @param y y coordinate of the position 
   */
  void setY(const RelAbsVector& y);

  /**
   * Sets the z position of the Rectangle within the viewport.
   *
   * @param z z coordinate of the position 
   */
  void setZ(const RelAbsVector& z);

  /**
   * Returns the x coordinate of the rectangles position
   *
   * @return const reference to RelAbsVector that represents the x position
   */
  const RelAbsVector& getX() const;

  /**
   * Returns the y coordinate of the rectangles position
   *
   * @return const reference to RelAbsVector that represents the y position
   */
  const RelAbsVector& getY() const;

  /**
   * Returns the z coordinate of the rectangles position
   *
   * @return const reference to RelAbsVector that represents the z position
   */
  const RelAbsVector& getZ() const;

  /**
   * Returns the with of the rectangle
   *
   * @return const reference to the RelAbsVector that represents the width
   */
  const RelAbsVector& getWidth() const;

  /**
   * Returns the height of the rectangle
   *
   * @return const reference to the RelAbsVector that represents the height
   */
  const RelAbsVector& getHeight() const;

  /**
   * Returns the corner radius along the x axis
   *
   * @return const reference to the RelAbsVector that corner radius along the x axis
   */
  const RelAbsVector& getRadiusX() const;

  /**
   * Returns the corner radius along the y axis
   *
   * @return const reference to the RelAbsVector that corner radius along the y axis
   */
  const RelAbsVector& getRadiusY() const;

  /**
   * Returns the x coordinate of the rectangles position
   *
   * @return reference to RelAbsVector that represents the x position
   */
  RelAbsVector& getX();

  /**
   * Returns the y coordinate of the rectangles position
   *
   * @return reference to RelAbsVector that represents the y position
   */
  RelAbsVector& getY();

  /**
   * Returns the z coordinate of the rectangles position
   *
   * @return reference to RelAbsVector that represents the z position
   */
  RelAbsVector& getZ();

  /**
   * Returns the with of the rectangle
   *
   * @return reference to the RelAbsVector that represents the width
   */
  RelAbsVector& getWidth();

  /**
   * Returns the height of the rectangle
   *
   * @return reference to the RelAbsVector that represents the height
   */
  RelAbsVector& getHeight();

  /**
   * Returns the corner radius along the x axis
   *
   * @return reference to the RelAbsVector that corner radius along the x axis
   */
  RelAbsVector& getRadiusX();

  /**
   * Returns the corner radius along the y axis
   *
   * @return reference to the RelAbsVector that corner radius along the y axis
   */
  RelAbsVector& getRadiusY();

  /**
   * Accepts the given SBMLVisitor for this instance of Rectangle.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  bool accept(SBMLVisitor& visitor) const;

  /**
   * Creates and returns a deep copy of this Rectangle object.
   * 
   * @return a (deep) copy of this Rectangle object
   */
  Rectangle* clone() const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
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
   * Creates an XMLNode object from this Rectangle object.
   *
   * @return the XMLNode with the XML representation for the 
   * Rectangle object.
   */
  XMLNode toXML() const;

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
};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* Rectangle_H__ */
