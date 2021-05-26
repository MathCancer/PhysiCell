/**
 * @file    Transformation.h
 * @brief   abstract class for representing 3D affine transformations
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
 * @class Transformation
 * @brief implementation of a 3D transformation matrix.
 *
 * The Transformation class represents a 3D transformation which normally is a 4x4 matrix.
 * Since the last row is always 0 0 0 1 for affine transformations, we leave out those values
 * and store the matrix as an array of 4x3 columns
 */

#ifndef Transformation_H__
#define Transformation_H__

#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/render/extension/RenderExtension.h>

#include <sbml/SBase.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN Transformation : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  double mMatrix[12];
  static const double IDENTITY3D[12];
  /** @endcond */

protected:
  /**
   * Creates a new Transformation object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Transformation object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Transformation
   * object to be instantiated.
   */
  Transformation(const XMLNode& node, unsigned int l2version=4);

public:


  /**
   * Returns a 3D identity matrix.
   * The matrix contains 12 double values.
   */
  static const double* getIdentityMatrix();

  /**
   * Creates a new Transformation object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Transformation (unsigned int level      = RenderExtension::getDefaultLevel(),
                  unsigned int version    = RenderExtension::getDefaultVersion(),
                  unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Transformation object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Transformation (RenderPkgNamespaces* renderns);

  /**
   * Copy constructor.
   */
  Transformation(const Transformation& other);

  
  /**
   * Destroy this Transformation object.
   */
  virtual ~Transformation ();


  /**
   * Sets the matrix to the values given in the array.
   *
   * @param m array with new values to be set for this Transformation object.
   */
  void setMatrix(const double m[12]);

  /**
   * Returns the matrix which is an array of double values of length 12.
   *
   * @return a pointer to the array of numbers for the transformation.
   */
  const double* getMatrix() const;

  /**
   * Returns true if the matrix has been set or false otherwise.
   * The matrix is considered as set if none of the values in the matrix is NaN.
   *
   * @return true or false depending on whether a NaN was found.
   */
  bool isSetMatrix() const;

};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* Transformation_H__ */
