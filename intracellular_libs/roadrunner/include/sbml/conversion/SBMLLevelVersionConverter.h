/**
 * @file    SBMLLevelVersionConverter.h
 * @brief   Definition of SBMLLevelVersionConverter, the base class for SBML conversion.
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
 * @class SBMLLevelVersionConverter
 * @sbmlbrief{core} Whole-document SBML Level/Version converter.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This SBML converter takes an SBML document having one SBML Level+Version
 * combination, and attempts to convert it to an SBML document having a
 * different Level+Version combination.
 *
 * This class is also the basis for
 * SBMLDocument::setLevelAndVersion(@if java long, long, boolean@endif).
 *
 * @section SBMLLevelVersionConverter-usage Configuration and use of SBMLLevelVersionConverter
 *
 * SBMLLevelVersionConverter is enabled by creating a ConversionProperties
 * object with the option @c "setLevelAndVersion", and passing this
 * properties object to SBMLDocument::convert(@if java
 * ConversionProperties@endif).  The target SBML Level and Version
 * combination are determined by the value of the SBML namespace set on the
 * ConversionProperties object (using
 * ConversionProperties::setTargetNamespaces(SBMLNamespaces *targetNS)).
 *
 * In addition, this converter offers the following options:
 *
 * @li @c "strict": If this option has the value @c true, then the validity
 * of the SBML document will be strictly preserved.  This means that SBML
 * validation will be performed, and if the original model is not valid or
 * semantics cannot be preserved in the converted model, then conversion will
 * not be performed.  Conversely, if this option is set to @c false, model
 * conversion will always be performed; if any errors are detected related to
 * altered semantics, the errors will be logged in the usual way (i.e., the
 * error log on the SBMLDocument object).
 *
 * @li @c "addDefaultUnits": By default, a conversion from SBML Level&nbsp;2
 * to Level&nbsp;3 will explicitly add UnitDefinition objects and unit
 * attributes on the Model object to define units that are implicitly defined
 * in SBML Level&nbsp;2.  This is usually desirable because in SBML
 * Level&nbsp;3, there are no default units and a conversion from
 * Level&nbsp;2 that did @em not add unit definitions would actually result
 * in a loss of information.  However, some users or software tools may not
 * need or want this, or worse, may be fooled into thinking that libSBML has
 * somehow inferred the proper units for model quantities.  (It has not; it
 * merely adds generic predefined units.)  This option lets callers control
 * this behavior.
 *
 * @copydetails doc_section_using_sbml_converters
 */

#ifndef SBMLLevelVersionConverter_h
#define SBMLLevelVersionConverter_h

#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>

#include <sbml/validator/ConsistencyValidator.h>
#include <sbml/validator/IdentifierConsistencyValidator.h>
#include <sbml/validator/MathMLConsistencyValidator.h>
#include <sbml/validator/SBOConsistencyValidator.h>
#include <sbml/validator/UnitConsistencyValidator.h>
#include <sbml/validator/OverdeterminedValidator.h>
#include <sbml/validator/ModelingPracticeValidator.h>
#include <sbml/validator/L1CompatibilityValidator.h>
#include <sbml/validator/L2v1CompatibilityValidator.h>
#include <sbml/validator/L2v2CompatibilityValidator.h>
#include <sbml/validator/L2v3CompatibilityValidator.h>
#include <sbml/validator/L2v4CompatibilityValidator.h>
#include <sbml/validator/L3v1CompatibilityValidator.h>
#include <sbml/validator/InternalConsistencyValidator.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN  SBMLLevelVersionConverter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new SBMLLevelVersionConverter object.
   */
  SBMLLevelVersionConverter ();


  /**
   * Copy constructor; creates a copy of an SBMLLevelVersionConverter
   * object.
   *
   * @param obj the SBMLLevelVersionConverter object to copy.
   */
  SBMLLevelVersionConverter(const SBMLLevelVersionConverter& obj);


  /**
   * Destroys this object.
   */
  virtual ~SBMLLevelVersionConverter ();


  /**
   * Assignment operator for SBMLLevelVersionConverter.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  SBMLLevelVersionConverter& operator=(const SBMLLevelVersionConverter& rhs);


  /**
   * Creates and returns a deep copy of this SBMLLevelVersionConverter
   * object.
   *
   * @return a (deep) copy of this converter.
   */
  virtual SBMLLevelVersionConverter* clone() const;


  /**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an SBMLLevelVersionConverter object to find out if the object's
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

  /**
   * Returns the target SBML Level for the conversion.
   *
   * @return an integer indicating the SBML Level.
   */
  unsigned int getTargetLevel();


  /**
   * Returns the target SBML Version for the conversion.
   *
   * @return an integer indicating the Version within the SBML Level.
   */
  unsigned int getTargetVersion();


  /**
   * Returns the flag indicating whether the conversion has been set to "strict".
   *
   * @return @c true if strict validity has been requested, @c false
   * otherwise.
   */
  bool getValidityFlag();

  /**
   * Returns the flag indicating whether default units should be added when
   * converting to L3 or not.
   *
   * @return @c true if default units should be added, @c false
   * otherwise.
   */
  bool getAddDefaultUnits();


#ifndef SWIG

#endif // SWIG


private:
  /** @cond doxygenLibsbmlInternal */
  bool conversion_errors(unsigned int errors, bool strictUnits = true);

  /*
   * Predicate returning true if model has strict unit consistency.
   */
  bool hasStrictUnits();

  /*
   * Predicate returning true if model has strict sbo consistency.
   */
  bool hasStrictSBO();

  /*
   * do actual conversion
   */
  bool performConversion(bool strict, bool strictUnits, bool duplicateAnn);

  unsigned int validateConvertedDocument();

  bool has_fatal_errors(unsigned int level, unsigned int version);

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
#endif  /* SBMLLevelVersionConverter_h */

