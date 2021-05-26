/**
 * @file    GradientStop.h
 * @brief   class for representing a stop in a gradient definition
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
 * @class GradientStop
 * @brief LibSBML implementation of the gradient stop concept from the
 *
 * SBML render extension. The gradient stop concept was more or less taken from 
 * the corresponding concept in SVG.
 * A GradientStop object represents the color at a certain location in a linear or
 * radial gradient.
 * Each gradient should contain two or more gradient stops which mark the edges of a region
 * within this region color are interpolated based on the distance of the location to the
 * edges of the region.
 *
 * A gradient stop has two attributes. The first attribute is an offset which determines the 
 * location for the gradient stop within the object the gradient is appllied to.
 * The offset can either be ab absolute value or a relative value or a combination of absolute 
 * and relative value. E.g. a value of "50%" for the offset means that the gradient stop is located
 * at 50% of the gradient vector. For more information and examples, see the render extension 
 * specification or the SVG specification.
 *
 * The second attribute defines the color for the gradient stop. The color can either be defined
 * be a color value string or by the id of a ColorDefinition object. (@see ColorDefinition) 
 *
 *
 * @class ListOfGradientStops
 * @brief a container that holds zero or more GradientStop objects.
 *
 * The ListOfGradientStops is used in linear and radial gradient objects to store the 
 * GradientStop objects that define the gradient. A valid gradient should have two or more 
 * gradient stops. 
 *
 */

#ifndef GradientStop_H__
#define GradientStop_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/SBase.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN GradientStop : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mOffset;
  std::string mStopColor;
  static const std::string ELEMENT_NAME;
  /** @endcond */

  friend class GradientBase;

public:
  /**
   * Creates a new GradientStop object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  GradientStop (unsigned int level      = RenderExtension::getDefaultLevel(),
                unsigned int version    = RenderExtension::getDefaultVersion(),
                unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new GradientStop object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  GradientStop (RenderPkgNamespaces* renderns);


  /**
   * Creates a new GradientStop object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * GradientStop object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the GradientStop
   * object to be instantiated.
   */
  GradientStop(const XMLNode& node, unsigned int l2version=4);

  
  /**
   * Destroy this GradientStop object.
   */
  virtual ~GradientStop ();


  /**
   * Returns the offset of the gradient.
   *
   * @return a const reference to the offset of the gradient stop.
   */
  const RelAbsVector& getOffset() const;

  /**
   * Returns the offset of the gradient.
   *
   * @return a reference to the offset of the gradient stop.
   */
  RelAbsVector& getOffset();

  /**
   * Sets the offset for the gradient stop.
   *
   * @param abs the absolute value of the offset.
   *
   * @param rel the relative value of the offset.
   */
  void setOffset(double abs,double rel);

  /**
   * Sets the offset to the value specified by the given string.
   * The string has to represent a combination of an absolute 
   * and relative value.
   * Valid value string would e.g. be "45.0", "30%" or
   * "10+5%". If the value is a combination of both relative and 
   * absolute value, the absolute value has to come before the relative
   * value. Number can be given as integer values or floating point values
   * and the two components can be combined by '+' or '-'. Depending on
   * whethr the relative value should be added or subtracted from the 
   * absolute value.
   * If the given string is not valid, the offset will have an absolute 
   * and a relative value of NaN.
   *
   * @param a string representing a valid offset value.
   */
  void setOffset(const std::string&);

  /**
   * Sets the offset to the given vector object.
   *
   * @param offset The RelAbsVector object that specifies the
   * offset of the gradient stop.
   */
  void setOffset(const RelAbsVector& offset);

  /**
   * Returns the stop color id or the value string.
   * Since ids can not start with the '#' character,
   * this is the way to determine if the gradient stop 
   * uses a color value or a color id.
   *
   * @return the color id or value string
   */
  const std::string& getStopColor() const;

  /**
   * Sets the stop color id or the stop color value.
   *
   * @param color Either the id of a ColorDefinition object, or a color
   * value string.
   */
  void setStopColor(const std::string& color);


  /**
   * Accepts the given SBMLVisitor for this instance of Group.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this GradientStop object.
   * 
   * @return a (deep) copy of this GradientStop object
   */
  virtual GradientStop* clone () const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  virtual const std::string& getElementName () const;

  /**
   * Creates an XMLNode object from this GradientStop object.
   *
   * @return the XMLNode with the XML representation for the 
   * GradientStop object.
   */
  XMLNode toXML() const;

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


class LIBSBML_EXTERN ListOfGradientStops : public ListOf
{
public:

  /**
   * Creates a deep copy of the ListOfGradientStops object.
   *
   * @return a (deep) copy of this ListOfGradientStops
   */
  virtual ListOfGradientStops* clone () const;

  /**
   * Creates a new ListOfGradientStops object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfGradientStops object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfGradientStops
   * object to be instantiated.
   */
  ListOfGradientStops(const XMLNode& node, unsigned int l2version=4);

  /**
   * Constructor which instantiated an empty ListOfGradientStops object. 
   */
  ListOfGradientStops(unsigned int level      = RenderExtension::getDefaultLevel(),
                      unsigned int version    = RenderExtension::getDefaultVersion(),
                      unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfGradientStops(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor; creates a copy of the given ListOfGradientStops object.
   *
   * @param the ListOfGradientStops object to be copied.
   */
  ListOfGradientStops(const ListOfGradientStops& source);

  ListOfGradientStops& operator=(const ListOfGradientStops& source);


  /**
   * Returns the XML element name of this object, which for
   * ListOfGradientStops, is always @c "listOfGradientStops".
   * 
   * @return the name of this element, i.e., @c "listOfGradientStops".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfGradientStops object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfGradientStops object.
   */
  XMLNode toXML() const;

  /**
   * Returns a pointer to the GradientStop with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the GradientStop object to be returned
   * 
   * @return pointer to the GradientStop at the given index or NULL.
   */
  GradientStop* get(unsigned int i);

  /**
   * Returns a const pointer to the GradientStop with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the GradientStop object to be returned
   * 
   * @return const pointer to the GradientStop at the given index or NULL.
   */
  const GradientStop* get(unsigned int i) const;

  /**
   * Removes the nth item from this ListOfGradientStops items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual GradientStop* remove (unsigned int n);

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

protected:

  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  static const std::string ELEMENT_NAME;
  /** @endcond */

  friend class GradientBase;

};



LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* GradientStop */ 
