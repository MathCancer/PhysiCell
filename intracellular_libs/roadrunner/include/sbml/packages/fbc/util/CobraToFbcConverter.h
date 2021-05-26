/**
 * @file    CobraToFbcConverter.h
 * @brief   Definition of a cobra 2 fbc converter.
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
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->
 *
 * @class CobraToFbcConverter
 * @sbmlbrief{fbc} COBRA to SBML Level 3 &ldquo;fbc&rdquo; converter.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This converter takes a model in COBRA format and converts it to the
 * &ldquo;fbc&rdquo; Version&nbsp;2 format.
 *
 * CobraToFbcConverter is enabled by creating a ConversionProperties object
 * with the option <code>"convert cobra"</code> (literally that full string,
 * including the spaces), and passing this properties object to
 * SBMLDocument::convert(@if java ConversionProperties@endif).  The converter
 * offers two options:
 *
 * @li @c "checkCompatibility": whether to check the SBML Level/Version
 * compatibility
 * @li @c "removeUnits": whether to remove unit definitions
 *
 * @copydetails doc_section_using_sbml_converters
 */

#ifndef CobraToFbcConverter_h
#define CobraToFbcConverter_h

#include <sbml/common/extern.h>
#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN CobraToFbcConverter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new CobraToFbcConverter object.
   */
  CobraToFbcConverter();


  /**
   * Copy constructor; creates a copy of an CobraToFbcConverter
   * object.
   *
   * @param orig the CobraToFbcConverter object to copy.
   */
  CobraToFbcConverter(const CobraToFbcConverter& orig);


  /**
   * Creates and returns a deep copy of this CobraToFbcConverter object.
   *
   * @return a (deep) copy of this CobraToFbcConverter.
   */
  virtual CobraToFbcConverter* clone() const;


  /**
   * Destroy this CobraToFbcConverter object.
   */
  virtual ~CobraToFbcConverter ();


  /**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an CobraToFbcConverter object to find out if the object's
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
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
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


  /**
   * Returns @c true if the option property to check Level and Version of the
   * source document (@c "checkCompatibility") is @c true.
   *
   * @return @c true if the option @c "checkCompatibility" has been set to
   * @c true, @c false otherwise.
   */
  bool checkCompatibility() const;

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

  
#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* CobraToFbcConverter_h*/

