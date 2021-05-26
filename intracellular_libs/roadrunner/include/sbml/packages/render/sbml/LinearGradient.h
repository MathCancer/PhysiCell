/**
 * @file    LinearGradient.h
 * @brief   class representing a linear gradient definition
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
 * @class LinearGradient
 * @brief Representation of a linear gradient object from the SBML render extension.
 *
 * The concept of a linear gradient is more or or less taken from SVG.
 * A linear gradient is defined by a vector which determines the direction and the length 
 * of the gradient. So for a valid gradient this vector should have a length different from 0.
 * Otherwise all restrictions for the GradientBase class apply. (@see GradientBase)
 *
 * The vector for a linear gradient is defined by a start and an endpoint and each point consists
 * of three absolute-relative value pairs (@see RelAbsVector)
 *
 * For examples of LinearGradients see the render extension specification and/or
 * the SVG specification.
 */

#ifndef LinearGradient_H__
#define LinearGradient_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GradientBase.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN LinearGradient : public GradientBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mX1;
  RelAbsVector mY1;
  RelAbsVector mZ1;
  RelAbsVector mX2;
  RelAbsVector mY2;
  RelAbsVector mZ2;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new LinearGradient object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  LinearGradient (unsigned int level      = RenderExtension::getDefaultLevel(),
                  unsigned int version    = RenderExtension::getDefaultVersion(),
                  unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new LinearGradient object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  LinearGradient (RenderPkgNamespaces* renderns);

  /**
   * Creates a new LinearGradient object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * LinearGradient object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the LinearGradient
   * object to be instantiated.
   */
  LinearGradient(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this LinearGradient object.
   */
  virtual ~LinearGradient ();


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a LinearGradient with no gradient stops.
   * The id is set to the given value.
   * The LinearGradient object is invalid until it has an id and at least two 
   * gradient stops.
   * The start and the end of the linear gradient vector are set to (0,0,0).
   * A linear gradient with a vector of length zero should also be considered invalid.
   *
   * @param id the new id for the LinearGradient.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  LinearGradient(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets the 3D coordinates for the start and the end point of the linear gradient vector.
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * @param x1 x value of the start point of the linear gradient vector
   * @param y1 y value of the start point of the linear gradient vector
   * @param z1 z value of the start point of the linear gradient vector
   * @param x2 x value of the end point of the linear gradient vector
   * @param y2 y value of the end point of the linear gradient vector
   * @param z2 z value of the end point of the linear gradient vector
   */
  void setCoordinates(const RelAbsVector& x1,const RelAbsVector& y1,const RelAbsVector& z1,const RelAbsVector& x2,const RelAbsVector& y2,const RelAbsVector& z2);

  /**
   * Sets the 2D coordinates for the start and the end point of the linear gradient vector.
   * The z values are automatically set to 0.
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * @param x1 x value of the start point of the linear gradient vector
   * @param y1 y value of the start point of the linear gradient vector
   * @param x2 x value of the end point of the linear gradient vector
   * @param y2 y value of the end point of the linear gradient vector
   */
  void setCoordinates(const RelAbsVector& x1,const RelAbsVector& y1,const RelAbsVector& x2,const RelAbsVector& y2);

  /**
   * Sets the coordinates for the start point of the linear gradient vector.
   *
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * The z value can be omitted. In that case it is set to 0.
   *
   * @param x x value of the start point of the linear gradient vector
   * @param y y value of the start point of the linear gradient vector
   * @param z z value of the start point of the linear gradient vector
   *
   */
  void setPoint1(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Sets the coordinates for the end point of the linear gradient vector.
   *
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * The z value can be omitted. In that case it is set to 0.
   *
   * @param x x value of the end point of the linear gradient vector
   * @param y y value of the end point of the linear gradient vector
   * @param z z value of the end point of the linear gradient vector
   *
   */
  void setPoint2(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Returns the x coordinate for the start point as a const reference.
   *
   * @return RelAbsVector that represents the x value of the start point.
   */
  const RelAbsVector& getXPoint1() const;

  /**
   * Returns the y coordinate for the start point as a const reference.
   *
   * @return RelAbsVector that represents the y value of the start point.
   */
  const RelAbsVector& getYPoint1() const;

  /**
   * Returns the z coordinate for the start point as a const reference.
   *
   * @return RelAbsVector that represents the z value of the start point.
   */
  const RelAbsVector& getZPoint1() const;

  /**
   * Returns the x coordinate for the end point as a const reference.
   *
   * @return RelAbsVector that represents the x value of the start point.
   */
  const RelAbsVector& getXPoint2() const;

  /**
   * Returns the y coordinate for the end point as a const reference.
   *
   * @return RelAbsVector that represents the y value of the start point.
   */
  const RelAbsVector& getYPoint2() const;

  /**
   * Returns the z coordinate for the end point as a const reference.
   *
   * @return RelAbsVector that represents the z value of the start point.
   */
  const RelAbsVector& getZPoint2() const;

  /**
   * Returns the x coordinate for the start point as a reference.
   *
   * @return RelAbsVector that represents the x value of the start point.
   */
  RelAbsVector& getXPoint1();

  /**
   * Returns the y coordinate for the start point as a reference.
   *
   * @return RelAbsVector that represents the y value of the start point.
   */
  RelAbsVector& getYPoint1();

  /**
   * Returns the z coordinate for the start point as a reference.
   *
   * @return RelAbsVector that represents the z value of the start point.
   */
  RelAbsVector& getZPoint1();

  /**
   * Returns the x coordinate for the end point as a reference.
   *
   * @return RelAbsVector that represents the x value of the start point.
   */
  RelAbsVector& getXPoint2();

  /**
   * Returns the y coordinate for the end point as a reference.
   *
   * @return RelAbsVector that represents the y value of the start point.
   */
  RelAbsVector& getYPoint2();

  /**
   * Returns the z coordinate for the end point as a reference.
   *
   * @return RelAbsVector that represents the z value of the start point.
   */
  RelAbsVector& getZPoint2();

  /**
   * Accepts the given SBMLVisitor for this instance of LinearGradient.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this LinearGradient object.
   * 
   * @return a (deep) copy of this LinearGradient object
   */
  virtual LinearGradient* clone () const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  virtual const std::string& getElementName () const;

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
  int getTypeCode() const;

  /**
   * Creates an XMLNode object from this LinearGradient object.
   *
   * @return the XMLNode with the XML representation for the 
   * LinearGradient object.
   */
  virtual XMLNode toXML() const;

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

#endif /* LinearGradient_H__ */
