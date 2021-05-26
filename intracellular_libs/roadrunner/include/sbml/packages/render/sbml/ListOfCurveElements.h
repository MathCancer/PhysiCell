/**
 * @file    ListOfCurveElements.h
 * @brief   storage class for the elements in a curve
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
 * @class ListOfCurveElements
 * @brief A container to store curve elements (RenderPoint and RenderCubicBezier). Use in RenderCurve and Polygon
 *
 * The ListOfCurveElements is a container class to store the curve elements of RenderCurve 
 * objects and Polygon objects.
 */

#ifndef ListOfCurveElements_H__
#define ListOfCurveElements_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/packages/render/sbml/RenderPoint.h>
#include <sbml/packages/render/sbml/RenderCubicBezier.h>
#include <sbml/packages/render/extension/RenderExtension.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN ListOfCurveElements : public ListOf
{
public:
  /**
   * Creates a new ListOfCurveElements object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfCurveElements object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfCurveElements
   * object to be instantiated.
   */
  ListOfCurveElements(const XMLNode& node, unsigned int l2version=4);

  /**
   * Creates and returns a deep copy of the ListOfCurveElements object.
   *
   * @return a (deep) copy of this ListOfCurveElements
   */
  virtual ListOfCurveElements* clone () const;

  /**
   * Constructor which instantiates an empty ListOfCurveElements object.
   */
  ListOfCurveElements(unsigned int level      = RenderExtension::getDefaultLevel(),
                      unsigned int version    = RenderExtension::getDefaultVersion(),
                      unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfCurveElements(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor for ListOfCurveElements objects.
   */
  ListOfCurveElements(const ListOfCurveElements& source);

  /**
   * Assignment operator for ListOfCurveElements objects.
   */
  ListOfCurveElements& operator=(const ListOfCurveElements& source);


  /**
   * Returns a pointer to the RenderPoint with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the RenderPoint object to be returned
   * 
   * @return pointer to the RenderPoint at the given index or NULL.
   */
  RenderPoint* get(unsigned int i);

  /**
   * Returns a const pointer to the RenderPoint with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the RenderPoint object to be returned
   * 
   * @return const pointer to the RenderPoint at the given index or NULL.
   */
  const RenderPoint* get(unsigned int i) const;

  /**
   * Removes the RenderPoint with the given index and returns a pointer to the 
   * removed object. The caller is responsible for freeing the associated memory.
   * 
   * @param i index of the RenderPoint object to be removed
   * 
   * @return pointer to the removed RenderPoint or NULL if the index
   *  was not valid.
   */
  RenderPoint* remove(unsigned int i);

  /**
   * Returns the XML element name of this object, which for
   * ListOfCurveElements, is always @c "listOfCurveElements".
   * 
   * @return the name of this element, i.e., @c "listOfCurveElements".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfCurveElements object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfCurveElements object.
   */
  XMLNode toXML() const;


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

#endif // ListOfCurveElements_H__

