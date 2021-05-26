/**
 * @file    RenderPoint.h
 * @brief   class to represent a point in the render extension
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
 * @class RenderPoint
 * @brief Represents a point where the coordinates can be made up of absolute
 * as well as relative values (@see RelAbsVector)
 *
 * Render objects are often specified relative to the current viewport, i.e. we need
 * a way to specify relative coordinate values in curves. For this we introduced the RenderPoint
 * and the RenderCubicBezier class in the render extension.
 * Those two classes are used to specify curve and polygon elements (@see RenderCurve or @see Polygon).
 *
 *
 */

#ifndef RenderPoint_H__
#define RenderPoint_H__

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>

#include <sbml/SBase.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderPoint : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mXOffset;
  RelAbsVector mYOffset;
  RelAbsVector mZOffset;
  std::string mElementName;
  /** @endcond */

public:
  /**
   * Creates a new RenderPoint object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RenderPoint (unsigned int level      = RenderExtension::getDefaultLevel(),
                         unsigned int version    = RenderExtension::getDefaultVersion(),
                         unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RenderPoint object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RenderPoint (RenderPkgNamespaces* renderns);


  /**
   * Copy constructor for RenderPoint objects.
   */
  RenderPoint(const RenderPoint& orig);
  
  /**
   * Creates a new point with the given ccordinates.
   *
   * @param x x coordinate of the RenderPoint object
   * @param y y coordinate of the RenderPoint object
   * @param z z coordinate of the RenderPoint object
   * If the z value is omitted, it is set to 0.
   */ 
  RenderPoint (RenderPkgNamespaces* renderns, const RelAbsVector& x, const RelAbsVector& y, const RelAbsVector& z = RelAbsVector(0.0,0.0));
        

  /**
   * Creates a new RenderPoint object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RenderPoint object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RenderPoint
   * object to be instantiated.
   */
   RenderPoint(const XMLNode& node, unsigned int l2version=4);

  /**
   * Destroys the RenderPoint object.
   */ 
  virtual ~RenderPoint ();


  /**
   * Assignment operator for RenderPoint objects.
   */
  RenderPoint& operator=(const RenderPoint& orig);

  /**
   * Comparison operator for RenderPoint objects.
   */
  bool operator==(const RenderPoint& left) const;


  /**
   * Returns the x coordinate of the RenderPoint as a const reference.
   *
   * @return const reference to x coordinate.
   */ 
  const RelAbsVector& x() const;
        
  /**
   * Returns the y coordinate of the RenderPoint as a const reference.
   *
   * @return const reference to y coordinate.
   */ 
  const RelAbsVector& y() const;
        
  /**
   * Returns the z coordinate of the RenderPoint as a const reference.
   *
   * @return const reference to z coordinate.
   */ 
  const RelAbsVector& z() const;
  
  /**
   * Returns the x coordinate of the RenderPoint as a reference.
   *
   * @return reference to x coordinate.
   */ 
  RelAbsVector& x();
        
  /**
   * Returns the y coordinate of the RenderPoint as a reference.
   *
   * @return reference to y coordinate.
   */ 
  RelAbsVector& y();
        
  /**
   * Returns the z coordinate of the RenderPoint as a reference.
   *
   * @return reference to z coordinate.
   */ 
  RelAbsVector& z();
  
  /**
   * Sets the x ccordiante of the RenderPoint object.
   *
   * @param x x coordinate to be set.
   */ 
  void setX (const RelAbsVector& x);
        
  /**
   * Sets the y ccordiante of the RenderPoint object.
   *
   * @param y y coordinate to be set.
   */ 
  void setY (const RelAbsVector& y);
        
  /**
   * Sets the z ccordiante of the RenderPoint object.
   *
   * @param z z coordinate to be set.
   */ 
  void setZ (const RelAbsVector& z);

  /**
   * Sets the coordinates of the RenderPoint to the given values.
   *
   * @param x x coordinate to be set.
   * @param y y coordinate to be set.
   * @param z z coordinate to be set. If the z coordinate is omitted, it is set to 0.
   */ 
  void setCoordinates (const RelAbsVector& x, const RelAbsVector& y, const RelAbsVector& z = RelAbsVector(0.0,0.0));

#ifndef OMIT_DEPRECATED
  /**
   * Sets the coordinates of the RenderPoint to the given values.
   * This method is deprecated, please use setCoordinates. 
   *
   * @param x x coordinate to be set.
   * @param y y coordinate to be set.
   * @param z z coordinate to be set. If the z coordinate is omitted, it is set to 0.
   */ 
  void setOffsets (const RelAbsVector& x, const RelAbsVector& y, const RelAbsVector& z = RelAbsVector(0.0,0.0));
#endif // OMIT_DEPRECATED
         
  /**
   * Sets the Z offset to 0.0.
   */ 
  void initDefaults ();

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
   * Sets the element name which is returned by getElementName.
   * RenderPoint objects can have different element names depending on context.
   *
   * @param name the string with the element name to be set.
   */
  virtual void setElementName(const std::string& name);
  
  
  /**
   * Returns the XML element name of this object, which for
   * RenderPoint, depends on the context.
   * The name that is returned has to be set with 
   * setElementName.
   * 
   * @return the name of this element  (@see setElementName)
   */
  virtual const std::string& getElementName () const ;

  
  /**
   * Creates and returns a deep copy of this RenderPoint object.
   * 
   * @return a (deep) copy of this RenderPoint object
   */
  virtual RenderPoint* clone () const;

  /**
   * Returns the libSBML type code for this %SBML object.
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
   * Creates an XMLNode object from this ColorDefinition object.
   *
   * @return the XMLNode with the XML representation for the 
   * ColorDefinition object.
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
//LIBSBML_CPP_NAMESPACE_BEGIN
//BEGIN_C_DECLS
//
//
//
///*
// * Creates a new point with the coordinates (0.0,0.0,0.0).
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//RenderPoint_create (void);
//
///*
// * Creates a new RenderPoint with the given coordinates.
// */ 
//LIBSBML_EXTERN
//RenderPoint_t *
//RenderPoint_createWithCoordinates (const RelAbsVector_t& x, const RelAbsVector_t& y, const RelAbsVector_t& z);
//
///*
// * Frees all memory for the RenderPoint.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_free (RenderPoint_t *p);
//
///*
// * Sets the Z offset to 0.0
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_initDefaults (RenderPoint_t *p);
//
///*
// * Sets the coordinates to the given values.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setOffsets (RenderPoint_t *p, const RelAbsVector_t& x, const RelAbsVector_t& y, const RelAbsVector_t& z);
//
///*
// * Sets the x offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setX (RenderPoint_t *p, const RelAbsVector_t& x);
//
///*
// * Sets the y offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setY (RenderPoint_t *p, const RelAbsVector_t& y);
//
///*
// * Sets the z offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setZ (RenderPoint_t *p, const RelAbsVector_t& z);
//
///*
// * Gets the x offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_x (const RenderPoint_t *p);
//
///*
// * Gets the y offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_y (const RenderPoint_t *p);
//
///*
// * Gets the z offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_z (const RenderPoint_t *p);
//
//
///*
// * Sets the x offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setXOffset (RenderPoint_t *p, const RelAbsVector_t& x);
//
///*
// * Sets the y offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setYOffset (RenderPoint_t *p, const RelAbsVector_t& y);
//
///*
// * Sets the z offset.
// */ 
//LIBSBML_EXTERN
//void
//RenderPoint_setZOffset (RenderPoint_t *p, const RelAbsVector_t& z);
//
///*
// * Gets the x offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_getXOffset (const RenderPoint_t *p);
//
///*
// * Gets the y offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_getYOffset (const RenderPoint_t *p);
//
///*
// * Gets the z offset.
// */ 
//LIBSBML_EXTERN
//const RelAbsVector_t&
//RenderPoint_getZOffset (const RenderPoint_t *p);
//
///*
// * @return a (deep) copy of this RenderPoint.
// */
//LIBSBML_EXTERN
//RenderPoint_t *
//RenderPoint_clone (const RenderPoint_t *m);
//
//
//END_C_DECLS
//LIBSBML_CPP_NAMESPACE_END

//

#endif  /* !SWIG */
#endif  /* RenderPoint_H__ */
