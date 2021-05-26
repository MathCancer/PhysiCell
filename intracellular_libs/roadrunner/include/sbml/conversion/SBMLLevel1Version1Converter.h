/**
 * @file    SBMLLevel1Version1Converter.h
 * @brief   Definition of SBMLLevel1Version1Converter.
 * @author  Frank Bergmann
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
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class SBMLLevel1Version1Converter
 * @sbmlbrief{core} Whole-document SBML Level/Version converter.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This SBML converter takes an SBML document having one SBML Level+Version
 * combination, and attempts to convert it to an SBML document having a
 * different Level+Version combination.  This converter
 * (SBMLLevel1Version1Converter) converts models to SBML Level&nbsp;1
 * Version&nbsp;1, to the extent possible by the limited features of
 * that Level/Version combination of SBML.
 *
 * @section SBMLLevel1Version1Converter-usage Configuration and use of SBMLLevel1Version1Converter
 *
 * SBMLLevel1Version1Converter is enabled by creating a ConversionProperties
 * object with the option @c "convertToL1V1", and passing this
 * properties object to SBMLDocument::convert(@if java
 * ConversionProperties@endif).  The target SBML Level and Version
 * combination are determined by the value of the SBML namespace set on the
 * ConversionProperties object (using
 * ConversionProperties::setTargetNamespaces(SBMLNamespaces *targetNS)).
 *
 * In addition, this converter offers the following options:
 *
 * @li @c "changePow": Mathematical expressions for exponentiation of
 * the form <code>pow(s1, 2)</code> will be converted to the expression
 * <code>s1^2</code>.
 *
 * @li @c "inlineCompartmentSizes": Back in the days of SBML Level&nbsp;1
 * Version&nbsp;1, many software tools assumed that the "kinetic laws" of
 * SBML were written in terms of units of
 * <em>concentration</em>/<em>time</em>.  These tools would not expect (and
 * thus not handle) rate expressions such as
 * <code>CompartmentOfS1 * k * S1</code>.
 * When the option @c "inlineCompartmentSizes" is enabled, libSBML will
 * replace the references to compartments (such as @c "CompartmentOfS1" in
 * this example) with their initial sizes.  This is not strictly correct in
 * all cases; in particular, if the compartment volume varies during
 * simulation, this conversion will not reflect the expected behavior.
 * However, many models do not have time-varying compartment sizes, so this
 * option makes it easy to get modern SBML rate expressions into a form that
 * old software tools may better understand.
 *
 * @copydetails doc_section_using_sbml_converters
 */

#ifndef SBMLLEVEL1VERSION1CONVERTER_H
#define SBMLLEVEL1VERSION1CONVERTER_H

#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN  SBMLLevel1Version1Converter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new SBMLLevel1Version1Converter object.
   */
  SBMLLevel1Version1Converter ();


  /**
   * Copy constructor; creates a copy of an SBMLLevel1Version1Converter
   * object.
   *
   * @param obj the SBMLLevel1Version1Converter object to copy.
   */
  SBMLLevel1Version1Converter(const SBMLLevel1Version1Converter& obj);


  /**
   * Destroys this object.
   */
  virtual ~SBMLLevel1Version1Converter ();


  /**
   * Assignment operator for SBMLLevel1Version1Converter.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  SBMLLevel1Version1Converter& operator=(const SBMLLevel1Version1Converter& rhs);


  /**
   * Creates and returns a deep copy of this SBMLLevel1Version1Converter
   * object.
   *
   * @return a (deep) copy of this converter.
   */
  virtual SBMLLevel1Version1Converter* clone() const;


  /**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an SBMLLevel1Version1Converter object to find out if the object's
   * property values match the given ones.  This method is also used by
   * SBMLConverterRegistry::getConverterFor(@if java ConversionProperties@endif)
   * to search across all registered converters for one matching particular
   * properties.
   *
   * @param props the properties to match.
   *
   * @return @c true if this converter's properties match, @c false
   * otherwise.
   */
  virtual bool matchesProperties(const ConversionProperties &props) const;


  /**
   * Perform the conversion.
   *
   * This method causes the converter to do the actual conversion work,
   * that is, to convert the SBMLDocument object set by
   * SBMLConverter::setDocument(@if java SBMLDocument@endif) and
   * with the configuration options set by
   * SBMLConverter::setProperties(@if java ConversionProperties@endif).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_CONV_INVALID_TARGET_NAMESPACE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_CONV_PKG_CONVERSION_NOT_AVAILABLE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_CONV_INVALID_SRC_DOCUMENT, OperationReturnValues_t}
   */
  virtual int convert();


  /**
   * Returns the default properties of this converter.
   *
   * A given converter exposes one or more properties that can be adjusted
   * in order to influence the behavior of the converter.  This method
   * returns the @em default property settings for this converter.  It is
   * meant to be called in order to discover all the settings for the
   * converter object.
   *
   * @return the ConversionProperties object describing the default properties
   * for this converter.
   */
  virtual ConversionProperties getDefaultProperties() const;


  /* Convenience functions for this converter */


private:
  /** @cond doxygenLibsbmlInternal */
  /*
   * Predicate returning true if compartment sizes in kinetic laws should
   * be inlined with their values.
   */
  bool inlineCompartmentSizes();

  /*
   * Predicate returning true if calls to pow should be replaced with the hat notation.
   */
  bool shouldChangePow();

  /** @endcond */
};


LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* SBMLLEVEL1VERSION1CONVERTER_H */

