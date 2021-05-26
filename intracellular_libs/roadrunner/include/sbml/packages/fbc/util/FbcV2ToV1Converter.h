/**
 * @file    FbcV2ToV1Converter.h
 * @brief   Definition of a fbc v2 to fbc v1 converter.
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
 * @class FbcV2ToV1Converter
 * @sbmlbrief{fbc} Convert &ldquo;fbc&rdquo; Version 2 models to Version 1
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This converter takes a model in &ldquo;fbc&rdquo; Version&nbsp;2 format
 * and converts it to &ldquo;fbc&rdquo; Version&nbsp;1 format.
 *
 * FbcV2ToV1Converter is enabled by creating a ConversionProperties object
 * with the option <code>"convert fbc v2 to fbc v1"</code> (literally that
 * full string, including the spaces), and passing this properties object to
 * SBMLDocument::convert(@if java ConversionProperties@endif).  The converter
 * offers no options.  Information present in v2 that cannot be encoded in 
 * v1 is lost.
 *
 * @copydetails doc_section_using_sbml_converters
 */

#ifndef FbcV2ToV1Converter_h
#define FbcV2ToV1Converter_h

#include <sbml/common/extern.h>
#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FbcV2ToV1Converter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new FbcV2ToV1Converter object.
   */
  FbcV2ToV1Converter();


  /**
   * Copy constructor; creates a copy of an FbcV2ToV1Converter
   * object.
   *
   * @param orig the FbcV2ToV1Converter object to copy.
   */
  FbcV2ToV1Converter(const FbcV2ToV1Converter& orig);


  /**
   * Creates and returns a deep copy of this FbcV2ToV1Converter object.
   *
   * @return a (deep) copy of this FbcV2ToV1Converter.
   */
  virtual FbcV2ToV1Converter* clone() const;


  /**
   * Destroy this FbcV2ToV1Converter object.
   */
  virtual ~FbcV2ToV1Converter ();


  /**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an FbcV2ToV1Converter object to find out if the object's
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
   * setDocument(@if java SBMLDocument@endif) and
   * with the configuration options set by
   * setProperties(@if java ConversionProperties@endif).
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


};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* FbcV2ToV1Converter_h*/

