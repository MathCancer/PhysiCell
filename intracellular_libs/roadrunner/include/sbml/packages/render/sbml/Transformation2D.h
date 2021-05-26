/**
 * @file    Transformation2D.h
 * @brief   abstract base class for representing 2D affine transformations
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
 * @class Transformation2D
 * @brief implementation of a 2D transformation matrix.
 *
 * The Transformation2D class represents a 2D transformation. it is derived from Transformation 
 * and inherits all the attributes of a 3D transformation. In addition is provides new methods to 
 * explicitly get and set 2D transformation properties.
 * A 2D transformation normally consists of a 3x3 matrix, but since the last row is always 0 0 1,
 * this is reduced to a 6 value array.
 *
 * Using one of the new 2D specific functions to set the matrix always updates the 3D matrix
 * automatically and vice versa, so the 2D data and the 3D data inherited from Transformation should 
 * always be consistent. 
 */

#ifndef Transformation2D_H__
#define Transformation2D_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/Transformation.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN Transformation2D : public Transformation
{
  static const double IDENTITY2D[6];

public:


  /**
   * Returns a 2D identity matrix.
   * The matrix contains 6 double values.
   */
  static const double* getIdentityMatrix2D();

  /**
   * Creates a new Transformation2D object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Transformation2D (unsigned int level      = RenderExtension::getDefaultLevel(),
    unsigned int version    = RenderExtension::getDefaultVersion(),
    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Transformation2D object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Transformation2D (RenderPkgNamespaces* renderns);

  /**
   * Copy constructor.
   */
  Transformation2D(const Transformation2D& other);

  /**
   * Creates a new Transformation2D object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Transformation2D object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Transformation2D
   * object to be instantiated.
   */
  Transformation2D(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this Transformation2D object.
   */
  virtual ~Transformation2D ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a Transformation with all numerical values set to 
   * the values in the given array.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Transformation2D(RenderPkgNamespaces* renderns, const double m[6]);
#endif // OMIT_DEPRECATED

  /**
   * Sets the 2D matrix to the values given in the array.
   * The 3D matrix is updated accordingly.
   *
   * @param m array with new values to be set for this Transformation object.
   */
  void setMatrix2D(const double m[6]);

  /**
   * Sets the 2D matrix to the values given in the array.
   * The 2D matrix is updated accordingly.
   *
   * @param m array with new values to be set for this Transformation object.
   */
  void setMatrix(const double m[12]);

  /**
   * Returns the 2D matrix which is an array of double values of length 6.
   *
   * @return a pointer to the array of numbers for the 2D transformation.
   */
  const double* getMatrix2D() const;

  /**
   * Creates an XMLNode object from this Transformation2D object.
   *
   * @return the XMLNode with the XML representation for the 
   * Transformation2D object.
   *
   * This method is purely virtual and has to be overwritten by derived classes.
   */
  virtual XMLNode toXML() const = 0;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes);
  /** @endcond */

protected:
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
   * Tries to parse the numerical values from the given string
   * and fill the matrix with them. The method will accept strings
   * representing 6 or 12 numerical values and fill the 2D or 3D matrix
   * accordingly.
   * The other matrix is updated automatically.
   *
   * @param transformationString string representing 6 or 12 numerical values.
   */
  void parseTransformation(const std::string& transformationString);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds the transformation attribute to the given XMLAttributes object.
   *
   * @param transformation the transformation to add as attribute.
   *
   * @param att XMLAttributes where the attribute needs to be added to
   */
  static void addTransformation2DAttributes(const Transformation2D& transformation,XMLAttributes& att);
  /** @endcond */


  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   * 
   * NOTE: this function is only ever going to be called from the constructor
   */
  virtual const std::string& getElementName () const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Returns the transformation array as a string for storage in an XML
   * attribute.
   */
  std::string get2DTransformationString() const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the 3D matrix from the 2D matrix.
   */
  void updateMatrix3D();
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Fills the 2D matrix with data from the 3D matrix.
   */
  void updateMatrix2D();
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * the 2D matrix.
   */
  double mMatrix2D[6];
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

#endif /* Transformation2D_H__ */
