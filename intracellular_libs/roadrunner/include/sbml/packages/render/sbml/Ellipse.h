/**
 * @file    Ellipse.h
 * @brief   class for ellipse objects
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
 * @class Ellipse
 * @brief graphical representation of an ellipse from the SBML render extension
 *
 * The ellipse class is derived from GraphicalPrimitive2D, so it inherits all its attributes
 * and methods. Therefore ellipses can have a transformation, a stroke and a stroke with to draw the edge
 * as well as a fill style and fill style related settings.
 *
 * Besides those inherited attributes, an ellipse if defined by its center point which can be specified 
 * as a combination of absolute and relative values and its radii for the two axes. The radii can also be
 * specified in terms absolute and/or relative values.
 */

#ifndef Ellipse_H__
#define Ellipse_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <sbml/packages/render/extension/RenderExtension.h>
#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Ellipse : public GraphicalPrimitive2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mCX;
  RelAbsVector mCY;
  RelAbsVector mCZ;
  RelAbsVector mRX;
  RelAbsVector mRY;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new Ellipse object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Ellipse (unsigned int level      = RenderExtension::getDefaultLevel(),
           unsigned int version    = RenderExtension::getDefaultVersion(),
           unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Ellipse object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Ellipse (RenderPkgNamespaces* renderns);

  /**
   * Creates a new RadialGradient object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RadialGradient object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RadialGradient
   * object to be instantiated.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this Ellipse object.
   */
  virtual ~Ellipse ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new ellipse object with the center set to 0,0,0
   * and the radii also set to 0.
   * The id is set to the given string.
   *
   * @param id the id of the ellipse.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with 2D center and radius.
   * instantiates a new ellipse object with the center.
   * The z coordinate of the center is set to 0.
   * The id is unset and both radii are set to the given radius.
   *
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param r radius along both axis
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& r);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with 2D center and radii.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& rx,const RelAbsVector& ry);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with 3D center and radii.
   * instantiates a new ellipse object with the center and radii.
   * The id is unset.
   *
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param cz z value of the center point 
   * @param rx radius along the x axis
   * @param ry radius along the y axis
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& cz,const RelAbsVector& rx,const RelAbsVector& ry);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with id, 2D center and radius.
   * instantiates a new ellipse object with the given @p id and center.
   * Both radii are set to the given radius r. This actually yields a circle.
   *
   * @param id id for the ellipse
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param r radius along both axis
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& r);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with id, 2D center and radii.
   * instantiates a new ellipse object with the given @p id, center and radii.
   *
   * @param id id for the ellipse
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param rx radius along the x axis
   * @param ry radius along the y axis
   *
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& rx,const RelAbsVector& ry);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor with id, 3D center and radii.
   * instantiates a new ellipse object with the given @p id, center and radii.
   *
   * @param id id for the ellipse
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param cz z value of the center point 
   * @param rx radius along the x axis
   * @param ry radius along the y axis
   *
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Ellipse(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& cz,const RelAbsVector& rx,const RelAbsVector& ry);
#endif // OMIT_DEPRECATED

  /**
   * Returns the x coordinate for the center point as a const reference.
   *
   * @return const reference to the x coordinatee of the center point.
   */
  const RelAbsVector& getCX() const;

  /**
   * Returns the y coordinate for the center point as a const reference.
   *
   * @return const reference to the y coordinatee of the center point.
   */
  const RelAbsVector& getCY() const;

  /**
   * Returns the z coordinate for the center point as a const reference.
   *
   * @return const reference to the z coordinatee of the center point.
   */
  const RelAbsVector& getCZ() const;

  /**
   * Returns the radius along the x axis as a const reference.
   *
   * @return const reference to the radius along the x axis
   */
  const RelAbsVector& getRX() const;

  /**
   * Returns the radius along the y axis as a const reference.
   *
   * @return const reference to the radius along the y axis
   */
  const RelAbsVector& getRY() const;

  /**
   * Returns the x coordinate for the center point as a reference.
   *
   * @return reference to the x coordinatee of the center point.
   */
  RelAbsVector& getCX();

  /**
   * Returns the y coordinate for the center point as a reference.
   *
   * @return reference to the y coordinatee of the center point.
   */
  RelAbsVector& getCY();

  /**
   * Returns the z coordinate for the center point as a reference.
   *
   * @return reference to the z coordinatee of the center point.
   */
  RelAbsVector& getCZ();

  /**
   * Returns the radius along the x axis as a reference.
   *
   * @return reference to the radius along the x axis
   */
  RelAbsVector& getRX();

  /**
   * Returns the radius along the y axis as a reference.
   *
   * @return reference to the radius along the y axis
   */
  RelAbsVector& getRY();

  /**
   * Sets the x coordinates for the center point.
   *
   * @param cx x value of the center point 
   */
  void setCX(const RelAbsVector& cx);

  /**
   * Sets the y coordinates for the center point.
   *
   * @param cy y value of the center point 
   */
  void setCY(const RelAbsVector& cy);

  /**
   * Sets the z coordinates for the center point.
   *
   * @param cz z value of the center point 
   */
  void setCZ(const RelAbsVector& cz);

  /**
   * Sets the radius along the x axis
   *
   * @param rx radius along the x axis
   */
  void setRX(const RelAbsVector& rx);

  /**
   * Sets the radius along the y axis
   *
   * @param ry radius along the y axis
   */
  void setRY(const RelAbsVector& ry);

  /**
   * Sets the 2D coordinates for the center point.
   * The z coodintate is set to 50%
   *
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   */
  void setCenter2D(const RelAbsVector& cx,const RelAbsVector& cy);

  /**
   * Sets the 3D coordinates for the center point.
   *
   * @param cx x value of the center point 
   * @param cy y value of the center point 
   * @param cz z value of the center point 
   */
  void setCenter3D(const RelAbsVector& cx,const RelAbsVector& cy,const RelAbsVector& cz);

  /**
   * Sets the radii of the ellipse
   *
   * @param rx radius along the x axis
   * @param ry radius along the y axis
   */
  void setRadii(const RelAbsVector& rx,const RelAbsVector& ry);

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
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Ellipse defines it as returning "ellipse",
   */
  virtual const std::string& getElementName () const ;


  /**
   * Creates and returns a deep copy of this Ellipse object.
   * 
   * @return a (deep) copy of this Ellipse object
   */
  virtual Ellipse* clone () const;

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
  int getTypeCode () const;

  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;


  /**
   * Creates an XMLNode object from this Ellipse object.
   *
   * @return the XMLNode with the XML representation for the 
   * Ellipse object.
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
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */

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

#endif /* Ellipse_H__ */
