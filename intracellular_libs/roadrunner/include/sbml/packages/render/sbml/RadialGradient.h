/**
 * @file    RadialGradient.h
 * @brief   class representing a radial gradient
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
 * @class RadialGradient
 * @brief Representation of a radial gradient object from the SBML render extension.
 *
 * The concept of a radial gradient is more or or less taken from SVG.
 * A radial gradient is defined by a center point, a radius and an optional focal point.
 * So for a valid gradient the radius should have a positive length different from 0 and
 * the focal point should be within the circle defined by the center point and the radius.
 * Otherwise all restrictions for the GradientBase class apply. (@see GradientBase)
 *
 * The center and the focal point of a radial gradient are defined by three pairs of
 * absolute-relative value. (@see RelAbsVector)
 * The radius is also defined asn an absolute-relative value pair.
 *
 * For examples of RadialGradients see the render extension specification and/or
 * the SVG specification.
 */


#ifndef RadialGradient_H__
#define RadialGradient_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GradientBase.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RadialGradient : public GradientBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mCX;
  RelAbsVector mCY;
  RelAbsVector mCZ;
  RelAbsVector mRadius;
  RelAbsVector mFX;
  RelAbsVector mFY;
  RelAbsVector mFZ;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new RadialGradient object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RadialGradient (unsigned int level      = RenderExtension::getDefaultLevel(),
                  unsigned int version    = RenderExtension::getDefaultVersion(),
                  unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RadialGradient object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RadialGradient (RenderPkgNamespaces* renderns);

  /**
   * Creates a new RadialGradient object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RadialGradient object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RadialGradient
   * object to be instantiated.
   */
  RadialGradient(const XMLNode& node, unsigned int l2version);


  /**
   * Destroy this RadialGradient object.
   */
  virtual ~RadialGradient ();


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a RadialGradient with no gradient stops.
   * The id is set to the given value.
   * The RadialGradient object is invalid until it has an id and at least two 
   * gradient stops.
   * The start and the end of the linear gradient vector are set to (0,0,0).
   * A linear gradient with a vector of length zero should also be considered invalid.
   *
   * @param id the new id for the RadialGradient.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  RadialGradient(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets the 3D coordinates for the center and the focal
   * point as well as the radius.
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * @param x x value of the center point of the radial gradient vector
   * @param y y value of the center point of the radial gradient vector
   * @param z z value of the center point of the radial gradient vector
   * @param r x value of the radius of the radial gradient vector
   * @param fx x value of the focal point of the radial gradient vector
   * @param fy y value of the focal point of the radial gradient vector
   * @param fz z value of the focal point of the radial gradient vector
   */
  void setCoordinates(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z,const RelAbsVector& r,const RelAbsVector& fx,const RelAbsVector& fy,const RelAbsVector& fz);

  /**
   * Sets the 2D coordinates for the center and the focal
   * point as well as the radius.
   * The z values are automatically set to 0.
   * Each value can be a combination of absolute and relative value and is represented by 
   * a RelAbsVector object.
   *
   * @param x x value of the center point of the radial gradient vector
   * @param y y value of the center point of the radial gradient vector
   * @param r x value of the radius of the radial gradient vector
   * @param fx x value of the focal point of the radial gradient vector
   * @param fy y value of the focal point of the radial gradient vector
   */
  void setCoordinates(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& r,const RelAbsVector& fx,const RelAbsVector& fy);

  /**
   * Sets the coordinates for the center point.
   *
   * @param x x value of the center point of the radial gradient vector
   * @param y y value of the center point of the radial gradient vector
   * @param z z value of the center point of the radial gradient vector
   * The z argument can be omitted. In that case it is set to 0.
   */
  void setCenter(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Sets the coordinates for the focal point.
   *
   * @param x x value of the focal point of the radial gradient vector
   * @param y y value of the focal point of the radial gradient vector
   * @param z z value of the focal point of the radial gradient vector.
   * The z argument can be omitted. In that case it is set to 0.
   */
  void setFocalPoint(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Sets the radius of the radial gradient.
   *
   * @param r radius of the radial gradient vector.
   */
  void setRadius(const RelAbsVector& r);

  /**
   * Returns the x coordinate for the center point as a const reference.
   *
   * @return const reference to the x coordinatee of the center point.
   */
  const RelAbsVector& getCenterX() const;

  /**
   * Returns the y coordinate for the center point as a const reference.
   *
   * @return const reference to the y coordinatee of the center point.
   */
  const RelAbsVector& getCenterY() const;

  /**
   * Returns the z coordinate for the center point as a const reference.
   *
   * @return const reference to the z coordinatee of the center point.
   */
  const RelAbsVector& getCenterZ() const;

  /**
   * Returns the x coordinate for the focal point as a const reference.
   *
   * @return const reference to the x coordinatee of the focal point.
   */
  const RelAbsVector& getFocalPointX() const;

  /**
   * Returns the y coordinate for the focal point as a const reference.
   *
   * @return const reference to the y coordinatee of the focal point.
   */
  const RelAbsVector& getFocalPointY() const;

  /**
   * Returns the z coordinate for the focal point as a const reference.
   *
   * @return const reference to the z coordinatee of the focal point.
   */
  const RelAbsVector& getFocalPointZ() const;

  /**
   * Returns the radius as a const reference.
   *
   * @return const reference to the radius 
   */
  const RelAbsVector& getRadius() const;

  /**
   * Returns the x coordinate for the center point as a reference.
   *
   * @return reference to the x coordinatee of the center point.
   */
  RelAbsVector& getCenterX();

  /**
   * Returns the y coordinate for the center point as a reference.
   *
   * @return reference to the y coordinatee of the center point.
   */
  RelAbsVector& getCenterY();

  /**
   * Returns the z coordinate for the center point as a reference.
   *
   * @return reference to the z coordinatee of the center point.
   */
  RelAbsVector& getCenterZ();

  /**
   * Returns the x coordinate for the focal point as a reference.
   *
   * @return reference to the x coordinatee of the focal point.
   */
  RelAbsVector& getFocalPointX();

  /**
   * Returns the y coordinate for the focal point as a reference.
   *
   * @return reference to the y coordinatee of the focal point.
   */
  RelAbsVector& getFocalPointY();

  /**
   * Returns the z coordinate for the focal point as a reference.
   *
   * @return reference to the z coordinatee of the focal point.
   */
  RelAbsVector& getFocalPointZ();

  /**
   * Returns the radius as a reference.
   *
   * @return reference to the radius 
   */
  RelAbsVector& getRadius();

  /**
   * Accepts the given SBMLVisitor for this instance of RadialGradient.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this RadialGradient object.
   * 
   * @return a (deep) copy of this RadialGradient object
   */
  virtual RadialGradient* clone () const;

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
   * Creates an XMLNode object from this RadialGradient object.
   *
   * @return the XMLNode with the XML representation for the 
   * RadialGradient object.
   */
  XMLNode toXML() const;
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

#endif /* RadialGradient_H__ */
