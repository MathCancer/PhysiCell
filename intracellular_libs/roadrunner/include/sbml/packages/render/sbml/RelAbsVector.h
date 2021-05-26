/**
 * @file    RelAbsVector.h
 * @brief   class for representing coordinates that consist of relative 
 *          and absolute values.
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
 * @class RelAbsVector
 * @brief This class represents a pair of numerical values where one value represents an absolute
 * value and the other value is a relative value in percent.
 *
 * For many elements in the render extension, it is necessary to specify coordinates not in terms
 * of absolute values, but rather in terms of relative values or even a combination of absolute
 * and relative values.
 * Such a pair of values where one represents an absolute value and the other represents a relative
 * value can be expressed by a RelAbsVector.
 *
 * The relative and absolute values to initialize a RelAbsVector object can either be given as
 * numerical datatypes (double) or as a valid value string.
 * A value string is a combination of an absolute value and a relative value and the absolute
 * value if given has to come first. So valid value strings would be: "5.0e3+20%", or "100%" or "4".
 * 
 */

#ifndef RelAbsVector_H__
#define RelAbsVector_H__

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/render/extension/RenderExtension.h>

#ifdef __cplusplus

#include <string>
#include <ostream>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RelAbsVector
{
protected:
  /** @cond doxygenLibsbmlInternal */
  double mAbs; // absolute coordinate value
  double mRel; // relative coordinate value 
  /** @endcond */

public:
  /**
   * Constructor with two values.
   * First value sets the absolute value, second sets the relative value (%). 
   *
   * @param a absolute value
   * @param a relative value in % (50 -> 50%)
   */
  RelAbsVector(double a=0.0,double r=0.0);

  /**
   * Constructor with a value string.
   * If the string does not represent a valid value, the relative and the
   * absolute component of the RelAbsVector are set to NaN.
   */
  RelAbsVector(const std::string& coordString);

  
  /**
   * Destroy this RelAbsVector object.
   */
  virtual ~RelAbsVector ();


  /**
   * Sets the relative and absolute value.
   *
   * @param abs absolute value
   * @param rel relative value. If the relative value is omitted, it is set to 0.
   */
  void setCoordinate(double abs,double rel=0.0);

  /**
   * Sets the coordinatees from the given string.
   * If the string does not represent a valid value, the relative and the
   * absolute component of the RelAbsVector are set to NaN.
   *
   * @param coordString value string
   */
  void setCoordinate(const std::string& coordString);

  /**
   * Sets the absolute coordinate value.
   *
   * @param abs absolute value to be set
   */
  void setAbsoluteValue(double abs);

  /**
   * Sets the relative coordinate value.
   *
   * @param rel relative value to be set
   */
  void setRelativeValue(double rel);

  /**
   * Returns the absolute coordinate value.
   *
   * @return absolute value
   */
  double getAbsoluteValue() const;

  /**
   * Returns the relative coordinate value.
   *
   * @return absolute value
   */
  double getRelativeValue() const;

  /**
   * addition operator for RelAbsVector objects
   */
  RelAbsVector operator+(const RelAbsVector& other) const;

  /**
   * Divides a RelAbsVector object by a double value.
   *
   * @param x divisor
   *
   * @return result of division as a new RelAbsVector object
   */
  RelAbsVector operator/(double x) const;

  /**
   * Comparison operator.
   * Return true if two RelAbsVector objects are equal.
   *
   * @return bool true if the two RelAbsValueObjects are equal and
   * false otherwise.
   */
  bool operator==(const RelAbsVector& other) const;

  /**
   * Inverse comparison operator.
   * Return false if two RelAbsVector objects are equal.
   *
   * @return bool false if the two RelAbsValueObjects are equal and
   * true otherwise.
   */
  bool operator!=(const RelAbsVector& other) const;

  /**
   * Assignment operator for RelAbsVector objects.
   */
  RelAbsVector& operator=(const RelAbsVector& src);

  /** @cond doxygenLibsbmlInternal */
  friend std::ostream& operator<<(std::ostream& os,const RelAbsVector& v);
  /** @endcond */
};

/**
 * Output operator for RelAbsVector objects.
 */
std::ostream& operator<<(std::ostream& os,const RelAbsVector& v);

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif // RelAbsVector_H__
