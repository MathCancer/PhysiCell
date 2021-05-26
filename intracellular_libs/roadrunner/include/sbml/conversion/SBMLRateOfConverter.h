/**
 * @file    SBMLRateOfConverter.h
 * @brief   Definition of SBMLRateOfConverter, a converter changing reactions into rate rules
 * @author  Sarah Keating
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
 * @class SBMLRateOfConverter
 * @sbmlbrief{core} Converter to inteerchange csymbol 'rateOf' with FunctionDefinition.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This converter will take an SBML model and either replace the use of
 * csymbol rateOf in the model with a FunctionDefinition, or replace a 
 * FunctionDefinition named 'rateOf' with use of the csymbol.
 *
 * @section SBMLRateOfConverter-usage Configuration and use of SBMLRateOfConverter
 *
 * SBMLRateOfConverter is enabled by creating a ConversionProperties object
 * with the option @c "replaceRateOf", and passing this properties object
 * to SBMLDocument::convert(@if java ConversionProperties@endif).  This
 * converter does not offer any additional configuration options.
 *
 * In addition, this converter offers the following options:
 *
 * @li @c "toFunction": If this option has the value @c true, then the 
 * conversion replaces csymbol with functionDefinition, if @c false, it 
 * replaces functionDefinition with csymbol.
 *
 * @copydetails doc_section_using_sbml_converters
 */

#ifndef SBMLRateOfConverter_h
#define SBMLRateOfConverter_h

#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>
#include <sbml/util/IdList.h>


#ifdef __cplusplus

#include <map>
#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN SBMLRateOfConverter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal  */

  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new SBMLRateOfConverter object.
   */
  SBMLRateOfConverter();


  /**
   * Copy constructor; creates a copy of an SBMLRateOfConverter
   * object.
   *
   * @param obj the SBMLRateOfConverter object to copy.
   */
  SBMLRateOfConverter(const SBMLRateOfConverter& obj);


  /**
   * Creates and returns a deep copy of this SBMLRateOfConverter
   * object.
   *
   * @return a (deep) copy of this converter.
   */
  virtual SBMLRateOfConverter* clone() const;


  /**
   * Destroy this SBMLRateOfConverter object.
   */
  virtual ~SBMLRateOfConverter ();


  /**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an SBMLRateOfConverter object to find out if the object's
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


  /**
   * Sets the current SBML document to the given SBMLDocument object.
   *
   * @param doc the document to use for this conversion.
   *
   * @warning Even though the @p doc is 'const', it is immediately cast
   * to a non-const version, which is then usually changed by the
   * converter upon a successful conversion.  This function is here
   * solely to preserve backwards compatibility.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  virtual int setDocument(const SBMLDocument* doc);


  /**
   * Sets the current SBML document to the given SBMLDocument object.
   *
   * @param doc the document to use for this conversion.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  virtual int setDocument(SBMLDocument* doc);

  /**
   * Returns the direction of the conversion.
   *
   * @return @c true if adding functionDefinition has been requested, @c false
   * otherwise.
   */
  bool getToFunctionDefinition();


private:

    /** @cond doxygenLibsbmlInternal */

  bool isDocumentValid();

  bool isCSymbolRateOfUsed();

  bool usesCSymbolRateOf(ASTNode * math);

  bool isCSymbolRateOf(ASTNode * math);

  bool isFDRateOfUsed();

  bool usesFDRateOf(ASTNode * math);

  bool isFDRateOf(ASTNode * math);

  bool hasFunctionDefinitionForRateOf();

  void addRateOfFunctionDefinition();
  
  void removeRateOfFunctionDefinition();

  // member variables

  std::vector<ASTNode*> mRateOfMath;

  Model * mOriginalModel;

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
#endif  /* SBMLRateOfConverter_h */

