/**
 * @file    RenderCubicBezier.h
 * @brief   class for representing a cubic bezier element in a render curve
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
 * @class RenderCubicBezier
 * @brief CubicBezier representation for RenderCurve objects and Polygon objects.
 *
 * The RenderCubicBezier is derived from RenderPoint and is the second element needed to 
 * represent arbitrary curves with relative coordinates as they can appear in RenderCurves
 * and Polygon objects.
 * In addition to the attributes inherited from RenderPoint, RenderCubicBezier has two additional
 * attributes for the two base points that define a cubic bezier curve.
 *
 * Segments in a RenderCurve or a Polygon are always defined by two consecutive RenderPoint or 
 * RenderCubicBezier elements. The first element in a list of RenderPoints has to be a RenderPoint 
 * object, all following elements can either be RenderPoints or RenderCubicBezier elements.
 * If the second element is a RenderPoint, the two elements represent a straight line segement,
 * if the second element if a RenderCubicBezier, the two elements represent a cubic bezier curve
 * segment.
 *
 * For further  details please have a look at the SBML render extension specification.
 */

#ifndef RenderCubicBezier_H__
#define RenderCubicBezier_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/layout/util/LayoutUtilities.h>
#include <sbml/packages/render/extension/RenderExtension.h>

#include <sbml/packages/render/sbml/RenderPoint.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/xml/XMLNode.h>


#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderCubicBezier : public RenderPoint
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mBasePoint1_X;
  RelAbsVector mBasePoint1_Y;
  RelAbsVector mBasePoint1_Z;
  RelAbsVector mBasePoint2_X;
  RelAbsVector mBasePoint2_Y;
  RelAbsVector mBasePoint2_Z;
  /** @endcond */


public:
  /**
   * Creates a new RenderCubicBezier object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RenderCubicBezier (unsigned int level      = RenderExtension::getDefaultLevel(),
    unsigned int version    = RenderExtension::getDefaultVersion(),
    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RenderCubicBezier object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RenderCubicBezier (RenderPkgNamespaces* renderns);



  /**
   * Copy constructor for RenderCubicBezier objects.
   */
  RenderCubicBezier(const RenderCubicBezier& orig);

  /**
   * Creates a CubicBezier with the given points.
   *
   * @param bp1_x x coordinatee of the first base point.
   * @param bp1_y y coordinatee of the first base point.
   * @param bp1_z z coordinatee of the first base point.
   * @param bp1_x x coordinatee of the second base point.
   * @param bp1_y y coordinatee of the second base point.
   * @param bp1_z z coordinatee of the second base point.
   * @param bp1_x x coordinatee of the end point.
   * @param bp1_y y coordinatee of the end point.
   * @param bp1_z z coordinatee of the end point.
   */
  RenderCubicBezier (RenderPkgNamespaces* renderns, 
                     const RelAbsVector& bp1_x,
                     const RelAbsVector& bp1_y,
                     const RelAbsVector& bp1_z,
                     const RelAbsVector& bp2_x,
                     const RelAbsVector& bp2_y,
                     const RelAbsVector& bp2_z,
                     const RelAbsVector& end_x,
                     const RelAbsVector& end_y,
                     const RelAbsVector& end_z);


  /**
   * Creates a new RenderCubicBezier object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RenderCubicBezier object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RenderCubicBezier
   * object to be instantiated.
   */
  RenderCubicBezier(const XMLNode& node, unsigned int l2version=4);

  /**
   * Destroys the RenderCubicBezier object.
   */ 
  virtual ~RenderCubicBezier ();

  /**
   * Comparison operator for RenderCubicBezier objects.
   */
  bool operator==(const RenderCubicBezier& left) const;


  /**
   * Assignment operator for RenderCubicBezier objects.
   */
  virtual RenderCubicBezier& operator=(const RenderCubicBezier& orig);


  /**
   * Returns the x value of the first base point of the curve (the one closer to the
   * starting point) as a const reference.
   *
   * @return const reference to x value of first base point
   */ 
  const RelAbsVector& basePoint1_X() const;

  /**
   * Returns the y value of the first base point of the curve (the one closer to the
   * starting point) as a const reference.
   *
   * @return const reference to y value of first base point
   */ 
  const RelAbsVector& basePoint1_Y() const;

  /**
   * Returns the z value of the first base point of the curve (the one closer to the
   * starting point) as a const reference.
   *
   * @return const reference to z value of first base point
   */ 
  const RelAbsVector& basePoint1_Z() const;


  /**
   * Returns the x value of the second base point of the curve (the one further from the
   * starting point) as a const reference.
   *
   * @return const reference to x value of second base point
   */ 
  const RelAbsVector& basePoint2_X() const;

  /**
   * Returns the y value of the second base point of the curve (the one further from the
   * starting point) as a const reference.
   *
   * @return const reference to y value of second base point
   */ 
  const RelAbsVector& basePoint2_Y() const;

  /**
   * Returns the z value of the second base point of the curve (the one further from the
   * starting point) as a const reference.
   *
   * @return const reference to z value of second base point
   */ 
  const RelAbsVector& basePoint2_Z() const;

  /**
   * Returns the x value of the first base point of the curve (the one closer to the
   * starting point) as a reference.
   *
   * @return reference to x value of first base point
   */ 
  RelAbsVector& basePoint1_X();

  /**
   * Returns the y value of the first base point of the curve (the one closer to the
   * starting point) as a reference.
   *
   * @return reference to y value of first base point
   */ 
  RelAbsVector& basePoint1_Y();

  /**
   * Returns the z value of the first base point of the curve (the one closer to the
   * starting point) as a reference.
   *
   * @return reference to z value of first base point
   */ 
  RelAbsVector& basePoint1_Z();


  /**
   * Returns the x value of the second base point of the curve (the one further from the
   * starting point) as a reference.
   *
   * @return reference to x value of second base point
   */ 
  RelAbsVector& basePoint2_X();

  /**
   * Returns the y value of the second base point of the curve (the one further from the
   * starting point) as a reference.
   *
   * @return reference to y value of second base point
   */ 
  RelAbsVector& basePoint2_Y();

  /**
   * Returns the z value of the second base point of the curve (the one further from the
   * starting point) as a reference.
   *
   * @return reference to z value of second base point
   */ 
  RelAbsVector& basePoint2_Z();

  /**
   * Sets the x value of the first base point of the curve (the one closer to the
   * starting point).
   *
   * @param x x coordinate of first base point.
   */ 
  void setBasePoint1_X(const RelAbsVector& x);

  /**
   * Sets the y value of the first base point of the curve (the one closer to the
   * starting point).
   *
   * @param y y coordinate of first base point.
   */ 
  void setBasePoint1_Y(const RelAbsVector& y);

  /**
   * Sets the z value of the first base point of the curve (the one closer to the
   * starting point).
   *
   * @param z z coordinate of first base point.
   */ 
  void setBasePoint1_Z(const RelAbsVector& z);


  /**
   * Sets the x value of the second base point of the curve (the one further from the
   * starting point).
   *
   * @param x value of second base point.
   */ 
  void setBasePoint2_X(const RelAbsVector& x);

  /**
   * Sets the y value of the second base point of the curve (the one further from the
   * starting point).
   *
   * @param y value of second base point.
   */ 
  void setBasePoint2_Y(const RelAbsVector& y);

  /**
   * Sets the z value of the second base point of the curve (the one further from the
   * starting point).
   *
   * @param z value of second base point.
   */ 
  void setBasePoint2_Z(const RelAbsVector& z);

  /**
   * Sets the first basepoint to the given coordinatees.
   *
   * @param x coordinate of second base point.
   * @param y coordinate of second base point.
   * @param z coordinate of second base point.
   * If the z coodinate is omitted, it is set to 0.
   */ 
  void setBasePoint1(const RelAbsVector& x, const RelAbsVector& y, const RelAbsVector& z = RelAbsVector(0.0,0.0));

  /**
   * Sets the second basepoint to the given coordinatees.
   *
   * @param x coordinate of second base point.
   * @param y coordinate of second base point.
   * @param z coordinate of second base point.
   * If the z coodinate is omitted, it is set to 0.
   */ 
  void setBasePoint2(const RelAbsVector& x, const RelAbsVector& y, const RelAbsVector& z = RelAbsVector(0.0,0.0));

  /**
   * Returns the XML element name of this object, which for
   * RenderCubicBezier, is always @c "element".
   * 
   * @return the name of this element, i.e., @c "element".
   */
  virtual const std::string& getElementName () const ;

  /**
   * Creates and returns a deep copy of the RenderCubicBezier object.
   *
   * @return a (deep) copy of this RenderCubicBezier
   */
  virtual RenderCubicBezier* clone () const;

  /**
   * Returns the libSBML type code for the objects contained in this ListOf
   * (i.e., ColorDefinition objects, if the list is non-empty).
   * 
   * @if clike LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.
   * The set of possible type codes is defined in the enumeration
   * #int.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if java LibSBML attaches an
   * identifying code to every kind of SBML object.  These are known as
   * <em>SBML type codes</em>.  In other languages, the set of type codes
   * is stored in an enumeration; in the Java language interface for
   * libSBML, the type codes are defined as static integer constants in
   * interface class {@link libsbmlConstants}.  The names of the type codes
   * all begin with the characters @c SBML_. @endif
   * 
   * @return the SBML type code for the objects contained in this ListOf
   * instance, or @c SBML_UNKNOWN (default).
   *
   * @see getElementName()
   */ 
  int getTypeCode () const;

  /**
   * Creates an XMLNode object from this RenderCubicBezier object.
   *
   * @return the XMLNode with the XML representation for the 
   * RenderCubicBezier object.
   */
  virtual XMLNode toXML(const std::string& name) const;

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


  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream. 
   *
   */
  virtual void writeXMLNS (XMLOutputStream& stream) const;

};

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */


#ifndef SWIG
//
//
//LIBSBML_CPP_NAMESPACE_BEGIN
//BEGIN_C_DECLS
//
//
///*
// * Creates a CubicBezier and returns the pointer.
// */
//LIBSBML_EXTERN
//CubicBezier_t *
//CubicBezier_create ();
//
///*
// * Creates a CubicBezier with the given points and returns the pointer.
// */
//LIBSBML_EXTERN
//CubicBezier_t *
//CubicBezier_createWithCoordinates (double x1, double y1, double z1,
//                                   double x2, double y2, double z2,
//                                   double x3, double y3, double z3,
//                                   double x4, double y4, double z4);
//
///*
// * Creates a CubicBezier with the given coordinates and returns the pointer.
// */
//LIBSBML_EXTERN
//CubicBezier_t *
//CubicBezier_createWithPoints (const RenderPoint_t *start, const RenderPoint_t *base1,
//                              const RenderPoint_t *base2, const RenderPoint_t *end);
//
//
///*
// * Creates a CubicBezier object from a template.
// */
//LIBSBML_EXTERN
//CubicBezier_t *
//CubicBezier_createFrom (const CubicBezier_t *temp);
//
//
///*
// * Frees the memory for the cubic bezier.
// */
//LIBSBML_EXTERN
//void
//CubicBezier_free (CubicBezier_t *ls);
//
///*
// * Initializes start point with a copy of the given point.
// */
//LIBSBML_EXTERN
//void
//CubicBezier_setStart (CubicBezier_t *cb, const RenderPoint_t *point);
//
///*
// * Initializes end point with a copy of the given point.
// */
//LIBSBML_EXTERN
//void
//CubicBezier_setEnd (CubicBezier_t *cb, const RenderPoint_t *point);
//
///*
// * Initializes the first base point with a copy of the given point.
// */
//LIBSBML_EXTERN
//void
//CubicBezier_setBasePoint1 (CubicBezier_t *cb, const RenderPoint_t *point);
//
///*
// * Initializes second base point with a copy of the given point.
// */
//LIBSBML_EXTERN
//void
//CubicBezier_setBasePoint2 (CubicBezier_t *cb, const RenderPoint_t *point);
//
///*
// * Returns the starting point of the curve.
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//CubicBezier_getStart (CubicBezier_t *cb);
//
///*
// * Returns the endpoint of the curve.
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//CubicBezier_getEnd (CubicBezier_t *cb);
//
///*
// * Returns the first base point of the curve (the one closer to the
// * starting point).
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//CubicBezier_getBasePoint1 (CubicBezier_t *cb);
//
//
///*
// * Returns the second base point of the curve (the one closer to the end
// * point).
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//CubicBezier_getBasePoint2 (CubicBezier_t *cb);
//
///*
// * Calls initDefaults from LineSegment.
// */ 
//LIBSBML_EXTERN
//void
//CubicBezier_initDefaults (CubicBezier_t *cb);
//
///*
// * @return a (deep) copy of this CubicBezier.
// */
//LIBSBML_EXTERN
//CubicBezier_t *
//CubicBezier_clone (const CubicBezier_t *m);
//
//
//END_C_DECLS
//LIBSBML_CPP_NAMESPACE_END
//

#endif  /* !SWIG */
#endif  /* CubicBezier_H__ */
