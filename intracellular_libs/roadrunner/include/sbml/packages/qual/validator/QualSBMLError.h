/**
 * @file    QualSBMLError.h
 * @brief   Represents SBML errors and other diagnostics
 * @author  Michael Hucka
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
 */

#ifndef QualSBMLError_h
#define QualSBMLError_h



LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * @enum QualSBMLErrorCode_t
 * Codes for all SBML-level errors and warnings from the 'qual' package.
 *
 * These are distinguished from other SBML error codes 
 * by having a number between 3000000 and 3099999.  
 * 
 * @copydetails doc_sbml_error_code_ranges
 */
typedef enum
{
  QualUnknown                           = 3010100 /*!< Unknown error from qual. */
, QualNSUndeclared                      = 3010101 /*!< The qual ns is not correctly declared. */
, QualElementNotInNs                    = 3010102 /*!< Element not in qual namespace. */

, QualFunctionTermBool                  = 3010201 /*!< FunctionTerm should return boolean. */
, QualMathCSymbolDisallowed             = 3010202 /*!< CSymbol time or delay not allowed. */

, QualDuplicateComponentId              = 3010301 /*!< Duplicate 'id' attribute value. */

, QualAttributeRequiredMissing          = 3020101 /*!< Required qual:required attribute on <code>&lt;sbml&gt;</code>. */
, QualAttributeRequiredMustBeBoolean    = 3020102 /*!< The qual:required attribute must be Boolean. */
, QualRequiredTrueIfTransitions         = 3020103 /*!< The qual:required attribute must be 'true' if math changes. */

, QualOneListOfTransOrQS                = 3020201 /*!< Only one <code>&lt;listOfTransitions&gt;</code> or <code>&lt;listOfQualitativeSpecies&gt;</code> allowed. */
, QualEmptyLONotAllowed                 = 3020202 /*!< Empty <code>&lt;listOfTransitions&gt;</code> or <code>&lt;listOfQualitativeSpecies&gt;</code> not allowed. */
, QualLOTransitiondAllowedElements      = 3020203 /*!< Elements allowed on <code>&lt;listOfTransitions&gt;</code>. */
, QualLOQualSpeciesAllowedElements      = 3020204 /*!< Elements allowed on <code>&lt;listOfTransitions&gt;</code>. */
, QualLOQualSpeciesAllowedAttributes    = 3020205 /*!< Attributes allowed on <code>&lt;listOfQualitativeSpecies&gt;</code>. */
, QualLOTransitionsAllowedAttributes    = 3020206 /*!< Attributes allowed on <code>&lt;listOfTransitions&gt;</code>. */

, QualQualSpeciesAllowedCoreAttributes  = 3020301 /*!< Core attributes allowed on <code>&lt;qualitativeSpecies&gt;</code>. */
, QualQualSpeciesAllowedElements        = 3020302 /*!< Elements allowed on <code>&lt;qualitativeSpecies&gt;</code>. */
, QualQualSpeciesAllowedAttributes      = 3020303 /*!< Attributes allowed on <code>&lt;qualitativeSpecies&gt;</code>. */
, QualConstantMustBeBool                = 3020304 /*!< Attribute 'constant' on <code>&lt;qualitativeSpecies&gt;</code> must be boolean. */
, QualNameMustBeString                  = 3020305 /*!< Attribute 'name' on <code>&lt;qualitativeSpecies&gt;</code> must be string. */
, QualInitialLevelMustBeInt             = 3020306 /*!< Attribute 'initialLevel' on <code>&lt;qualitativeSpecies&gt;</code> must be integer. */
, QualMaxLevelMustBeInt                 = 3020307 /*!< Attribute 'maxLevel' on <code>&lt;qualitativeSpecies&gt;</code> must be integer. */
, QualCompartmentMustReferExisting      = 3020308 /*!< Attribute 'compartment' on <code>&lt;qualitativeSpecies&gt;</code> must reference compartment. */
, QualInitialLevelCannotExceedMax       = 3020309 /*!< Attribute 'initialLevel' on <code>&lt;qualitativeSpecies&gt;</code> cannot exceed maxLevel. */
, QualConstantQSCannotBeOutput          = 3020310 /*!< Constant <code>&lt;qualitativeSpecies&gt;</code> cannot be an Output. */
, QualQSAssignedOnlyOnce                = 3020311 /*!< A <code>&lt;qualitativeSpecies&gt;</code> can only be assigned once. */
, QualInitalLevelNotNegative            = 3020312 /*!< Attribute 'initialLevel' on <code>&lt;qualitativeSpecies&gt;</code> cannot be negative. */
, QualMaxLevelNotNegative               = 3020313 /*!< Attribute 'maxLevel' on <code>&lt;qualitativeSpecies&gt;</code> cannot be negative. */

, QualTransitionAllowedCoreAttributes   = 3020401 /*!< Core attributes allowed on <code>&lt;transition&gt;</code>. */
, QualTransitionAllowedElements         = 3020402 /*!< Elements allowed on <code>&lt;transition&gt;</code>. */
, QualTransitionAllowedAttributes       = 3020403 /*!< Attributes allowed on <code>&lt;transition&gt;</code>. */
, QualTransitionNameMustBeString        = 3020404 /*!< Attribute 'name' on <code>&lt;transition&gt;</code> must be string. */
, QualTransitionLOElements              = 3020405 /*!< ListOf elements on <code>&lt;transition&gt;</code>. */
, QualTransitionEmptyLOElements         = 3020406 /*!< ListOf elements on <code>&lt;transition&gt;</code> not empty. */
, QualTransitionLOInputElements         = 3020407 /*!< Elements on <code>&lt;listOfInputs&gt;</code>. */
, QualTransitionLOOutputElements        = 3020408 /*!< Elements on <code>&lt;listOfOutputs&gt;</code>. */
, QualTransitionLOFuncTermElements      = 3020409 /*!< Elements on <code>&lt;listOfFunctionTerms&gt;</code>. */
, QualTransitionLOInputAttributes       = 3020410 /*!< Attributes allowed on <code>&lt;listOfInputs&gt;</code>. */
, QualTransitionLOOutputAttributes      = 3020411 /*!< Attributes allowed on <code>&lt;listOfOutputs&gt;</code>. */
, QualTransitionLOFuncTermAttributes    = 3020412 /*!< Attributes allowed on <code>&lt;listOfFunctionTerms&gt;</code>. */
, QualTransitionLOFuncTermExceedMax     = 3020413 /*!< <code>&lt;listOfFunctionTerms&gt;</code> cannot make qualitativeSpecies exceed maxLevel. */
, QualTransitionLOFuncTermNegative      = 3020414 /*!< <code>&lt;listOfFunctionTerms&gt;</code> cannot make qualitativeSpecies negative. */

, QualInputAllowedCoreAttributes        = 3020501 /*!< Core attributes allowed on <code>&lt;input&gt;</code>. */
, QualInputAllowedElements              = 3020502 /*!< Elements allowed on <code>&lt;input&gt;</code>. */
, QualInputAllowedAttributes            = 3020503 /*!< Attributes allowed on <code>&lt;input&gt;</code>. */
, QualInputNameMustBeString             = 3020504 /*!< Attribute 'name' on <code>&lt;input&gt;</code> must be string. */
, QualInputSignMustBeSignEnum           = 3020505 /*!< Attribute 'sign' on <code>&lt;input&gt;</code> must be enum. */
, QualInputTransEffectMustBeInputEffect = 3020506 /*!< Attribute 'transitionEffect' on <code>&lt;input&gt;</code> must be enum. */
, QualInputThreshMustBeInteger          = 3020507 /*!< Attribute 'thresholdLevel' on <code>&lt;input&gt;</code> must be non negative integer. */
, QualInputQSMustBeExistingQS           = 3020508 /*!< Attribute 'qualitativeSpecies' on <code>&lt;input&gt;</code> must refer to existing. */
, QualInputConstantCannotBeConsumed     = 3020509 /*!< Constant <code>&lt;input&gt;</code> cannot be consumed. */
, QualInputThreshMustBeNonNegative      = 3020510 /*!< Attribute 'thresholdLevel' on <code>&lt;input&gt;</code> must be non negative integer. */

, QualOutputAllowedCoreAttributes       = 3020601 /*!< Core attributes allowed on <code>&lt;output&gt;</code>. */
, QualOutputAllowedElements             = 3020602 /*!< Elements allowed on <code>&lt;output&gt;</code>. */
, QualOutputAllowedAttributes           = 3020603 /*!< Attributes allowed on <code>&lt;output&gt;</code>. */
, QualOutputNameMustBeString            = 3020604 /*!< Attribute 'name' on <code>&lt;output&gt;</code> must be string. */
, QualOutputTransEffectMustBeOutput     = 3020605 /*!< Attribute 'transitionEffect' on <code>&lt;output&gt;</code> must be enum. */
, QualOutputLevelMustBeInteger          = 3020606 /*!< Attribute 'outputLevel' on <code>&lt;output&gt;</code> must be non negative integer. */
, QualOutputQSMustBeExistingQS          = 3020607 /*!< Attribute 'qualitativeSpecies' on <code>&lt;output&gt;</code> must refer to existing. */
, QualOutputConstantMustBeFalse         = 3020608 /*!< Constant 'qualitativeSpecies' cannot be <code>&lt;output&gt;</code>. */
, QualOutputProductionMustHaveLevel     = 3020609 /*!< <code>&lt;output&gt;</code> being produced must have level. */
, QualOutputLevelMustBeNonNegative      = 3020610 /*!< Attribute 'outputLevel' on <code>&lt;output&gt;</code> must be non negative integer. */

, QualDefaultTermAllowedCoreAttributes  = 3020701 /*!< Core attributes allowed on <code>&lt;defaultTerm&gt;</code>. */
, QualDefaultTermAllowedElements        = 3020702 /*!< Elements allowed on <code>&lt;defaultTerm&gt;</code>. */
, QualDefaultTermAllowedAttributes      = 3020703 /*!< Attributes allowed on <code>&lt;defaultTerm&gt;</code>. */
, QualDefaultTermResultMustBeInteger    = 3020704 /*!< Attribute 'resultLevel' on <code>&lt;defaultTerm&gt;</code> must be non negative integer. */
, QualDefaultTermResultMustBeNonNeg     = 3020705 /*!< Attribute 'resultLevel' on <code>&lt;defaultTerm&gt;</code> must be non negative integer. */

, QualFuncTermAllowedCoreAttributes     = 3020801 /*!< Core attributes allowed on <code>&lt;functionTerm&gt;</code>. */
, QualFuncTermAllowedElements           = 3020802 /*!< Elements allowed on <code>&lt;functionTerm&gt;</code>. */
, QualFuncTermAllowedAttributes         = 3020803 /*!< Attributes allowed on <code>&lt;functionTerm&gt;</code>. */
, QualFuncTermOnlyOneMath               = 3020804 /*!< Only one <code>&lt;math&gt;</code> on <code>&lt;functionTerm&gt;</code>. */
, QualFuncTermResultMustBeInteger       = 3020805 /*!< Attribute 'resultLevel' on <code>&lt;functionTerm&gt;</code> must be non negative integer. */
, QualFuncTermResultMustBeNonNeg        = 3020806 /*!< Attribute 'resultLevel' on <code>&lt;functionTerm&gt;</code> must be non negative integer.. */

} QualSBMLErrorCode_t;


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END



#endif /* SBMLError_h */
