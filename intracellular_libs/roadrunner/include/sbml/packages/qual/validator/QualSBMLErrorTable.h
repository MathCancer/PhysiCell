/**
 * @file    QualSBMLErrorTable.h
 * @brief   Represents SBML errors and other diagnostics
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
 * ---------------------------------------------------------------------- -->*/


#ifndef QualSBMLErrorTable_H__
#define QualSBMLErrorTable_H__


#include <sbml/packages/qual/validator/QualSBMLError.h>


LIBSBML_CPP_NAMESPACE_BEGIN

  /** @cond doxygenLibsbmlInternal */
static const packageErrorTableEntry qualErrorTable[] =
{
  // 3010100
  { QualUnknown, 
    "Unknown error from qual",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Unknown error from qual",
    { ""
    }
  },

  // 3010101
  { QualNSUndeclared, 
    "The qual ns is not correctly declared",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "To conform to Version 1 of the Qualitative Models "
    "package specification for SBML Level 3, an "
    "SBML document must declare the use of the following XML Namespace: "
    "'http://www.sbml.org/sbml/level3/version1/qual/version1'",
    { "L3V1 Qual V1 Section 3.1"
    }
  },

  // 3010102
  { QualElementNotInNs, 
    "Element not in qual namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Wherever they appear in an SBML document, "
    "elements and attributes from the Qualitative Models "
    "package must be declared either implicitly or explicitly to be in the "
    "XML namespace 'http://www.sbml.org/sbml/level3/version1/qual/version1'",
    { "L3V1 Qual V1 Section 3.1"
    }
  },

    // 3010201
  { QualFunctionTermBool, 
    "FunctionTerm should return boolean",
    LIBSBML_CAT_MATHML_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The MathML <math> element in a <functionTerm> object should "
    "evaluate to a value of type boolean.", 
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

    // 3010202
  { QualMathCSymbolDisallowed, 
    "CSymbol time or delay not allowed",
    LIBSBML_CAT_MATHML_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The MathML <math> element in a <functionTerm> object should not use "
    "the <csymbol> elements 'time' and 'delay' as these explicitly introduce "
    "time into the model. As yet time is not considered within the Qualitative "
    "Models package specification.", 
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

    // 3010301
  { QualDuplicateComponentId, 
    "Duplicate 'id' attribute value",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "(Extends validation rule #10301 in the SBML Level 3 Version 1 Core "
    "specification.) Within a <model> the values of the attributes "
    "'id' and 'qual:id' on every instance of the following "
    "classes of objects must be unique across the set of all 'id' and "
    "'qual:id' attribute values of all such objects in a model: the "
    "<model> itself, plus all contained <functionDefinition>, <compartment>, "
    "<species>, <reaction>, <speciesReference>, <modifierSpeciesReference>, "
    "<event>, and <parameter> objects, plus the <qualitativeSpecies>, "
    "<transition>, <input> and <output> objects defined by the Qualitative "
    "Models package.", 
    { "L3V1 Qual V1 Section 3.7"
    }
  },

  // 3020101
  { QualAttributeRequiredMissing, 
    "Required qual:required attribute on <sbml>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "In all SBML documents using the Qualitative Models "
    "package, the SBML object must include a value for the attribute "
    "'qual:required' attribute.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 3020102
  { QualAttributeRequiredMustBeBoolean, 
    "The qual:required attribute must be Boolean",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of attribute 'qual:required' on the SBML object must "
    "be of the data type Boolean.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 3020103
  { QualRequiredTrueIfTransitions, 
    "The qual:required attribute must be 'true' if math changes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of attribute 'qual:required' on the SBML object must "
    "be set to 'true' if if the model contains any <transition> objects.",
    { "L3V1 Qual V1 Section 3.1"
    }
  },

  // 3020201
  { QualOneListOfTransOrQS, 
    "Only one <listOfTransitions> or <listOfQualitativeSpecies> allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of each of the following kinds "
    "of objects within a <model> object using Qualitative Models: "
    "<listOfTransitions> and <listOfQualitativeSpecies>.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020202
  { QualEmptyLONotAllowed, 
    "Empty <listOfTransitions> or <listOfQualitativeSpecies> not allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The various ListOf subobjects with a <model> object are optional, "
    "but if present, these container object must not be empty.  Specifically, "
    "if any of the following classes of objects are present on the <model>, "
    "it must not be empty: <listOfQualitativeSpecies> and <listOfTransitions>.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020203
  { QualLOTransitiondAllowedElements, 
    "Elements allowed on <listOfTransitions>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfTransitions> container object may only "
    "contain <transition> objects.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020204
  { QualLOQualSpeciesAllowedElements, 
    "Elements allowed on <listOfTransitions>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfQualitativeSpecies> container object may only "
    "contain <qualitativeSpecies> objects.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020205
  { QualLOQualSpeciesAllowedAttributes, 
    "Attributes allowed on <listOfQualitativeSpecies>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfQualitativeSpecies> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <listOfQualitativeSpecies> object.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020206
  { QualLOTransitionsAllowedAttributes, 
    "Attributes allowed on <listOfTransitions>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfTransitions> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <listOfTransitions> object.",
    { "L3V1 Qual V1 Section 3.4"
    }
  },

  // 3020301
  { QualQualSpeciesAllowedCoreAttributes, 
    "Core attributes allowed on <qualitativeSpecies>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <qualitativeSpecies> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <qualitativeSpecies> object.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },
    
  // 3020302
  { QualQualSpeciesAllowedElements, 
    "Elements allowed on <qualitativeSpecies>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <qualitativeSpecies> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on a <qualitativeSpecies>.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020303
  { QualQualSpeciesAllowedAttributes, 
    "Attributes allowed on <qualitativeSpecies>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <qualitativeSpecies> object must have the required attributes "
    "'qual:id', 'qual:compartment' and 'qual:constant', and may have "
    "the optional attributes 'qual:name', 'qual:initialLevel' and "
    "'qual:maxLevel'.  No other attributes from the SBML Level 3 Qualitative "
    "Models namespace are permitted on a <qualitativeSpecies> object. ",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020304
  { QualConstantMustBeBool, 
    "Attribute 'constant' on <qualitativeSpecies> must be boolean.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:constant' in <qualitativeSpecies> must be of "
    "the data type boolean.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020305
  { QualNameMustBeString, 
    "Attribute 'name' on <qualitativeSpecies> must be string.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:name' in <qualitativeSpecies> must be of "
    "the data type string.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020306
  { QualInitialLevelMustBeInt, 
    "Attribute 'initialLevel' on <qualitativeSpecies> must be integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:initialLevel' in <qualitativeSpecies> must be of "
    "the data type integer.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020307
  { QualMaxLevelMustBeInt, 
    "Attribute 'maxLevel' on <qualitativeSpecies> must be integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:maxLevel' in <qualitativeSpecies> must be of "
    "the data type integer.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020308
  { QualCompartmentMustReferExisting, 
    "Attribute 'compartment' on <qualitativeSpecies> must reference compartment.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:compartment' in a <qualitativeSpecies> "
    "object must be the identifier of an existing <compartment> object "
    "defined in the enclosing <model> object.",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020309
  { QualInitialLevelCannotExceedMax, 
    "Attribute 'initialLevel' on <qualitativeSpecies> cannot exceed maxLevel.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:initialLevel' in a <qualitativeSpecies> "
    "object cannot be greater than the value of the 'qual:maxLevel' attribute "
    "for the given <qualitativeSpecies> object. ",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020310
  { QualConstantQSCannotBeOutput, 
    "Constant <qualitativeSpecies> cannot be an Output.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <qualitativeSpecies> with attribute 'qual:constant' set to 'true' can "
    "only be referred to by an <input>. It cannot be the subject of an "
    "<output> in a <transition>. ",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020311
  { QualQSAssignedOnlyOnce, 
    "A <qualitativeSpecies> can only be assigned once.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <qualitativeSpecies> that is referenced by an <output> with the "
    "'qual:transitionEffect' attribute set to 'assignmentLevel' should not be "
    "referenced by any other <output> with the same 'transitionEffect' "
    "throughout the set of transitions for the containing model. ",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020312
  { QualInitalLevelNotNegative, 
    "Attribute 'initialLevel' on <qualitativeSpecies> cannot be negative.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:initialLevel' in <qualitativeSpecies> must not "
    "be negative. ",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020313
  { QualMaxLevelNotNegative, 
    "Attribute 'maxLevel' on <qualitativeSpecies> cannot be negative.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:maxLevel' in <qualitativeSpecies> must not "
    "be negative. ",
    { "L3V1 Qual V1 Section 3.5"
    }
  },

  // 3020401
  { QualTransitionAllowedCoreAttributes, 
    "Core attributes allowed on <transition>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <transition> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <transition> object.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },
    
  // 3020402
  { QualTransitionAllowedElements, 
    "Elements allowed on <transition>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <transition> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on a <transition>.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020403
  { QualTransitionAllowedAttributes, 
    "Attributes allowed on <transition>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <transition> object may have the optional attributes 'qual:name' "
    "and 'qual:id'.  No other attributes from the SBML Level 3 Qualitative "
    "Models namespace are permitted on a <transition> object. ",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020404
  { QualTransitionNameMustBeString, 
    "Attribute 'name' on <transition> must be string.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:name' in <transition> must be of "
    "the data type string.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020405
  { QualTransitionLOElements, 
    "ListOf elements on <transition>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <transition> must have one and only one instance of the "
    "<listOfFunctionTerms> objects and may have at most one instance of the "
    "<listOfInputs> and <listOfOutputs> objects from the Qualitative Models "
    "namespace.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020406
  { QualTransitionEmptyLOElements, 
    "ListOf elements on <transition> not empty.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The <listOfInputs> and <listOfOutputs> subobjects on a <transition> "
    "object are optional, but if present, these container object must not "
    "be empty.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020407
  { QualTransitionLOInputElements, 
    "Elements on <listOfInputs>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfInputs> container object may only "
    "contain <input> objects.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020408
  { QualTransitionLOOutputElements, 
    "Elements on <listOfOutputs>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfOutputs> container object may only "
    "contain <output> objects.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020409
  { QualTransitionLOFuncTermElements, 
    "Elements on <listOfFunctionTerms>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfFunctionTerms> container object must "
    "contain one and only one <defaultTerm> object and then may only "
    "contain <functionTerm> objects.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020410
  { QualTransitionLOInputAttributes, 
    "Attributes allowed on <listOfInputs>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfInputs> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <listOfInputs> object.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020411
  { QualTransitionLOOutputAttributes, 
    "Attributes allowed on <listOfOutputs>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfOutputs> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <listOfOutputs> object.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020412
  { QualTransitionLOFuncTermAttributes, 
    "Attributes allowed on <listOfFunctionTerms>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfFunctionTerms> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <listOfFunctionTerms> object.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020413
  { QualTransitionLOFuncTermExceedMax, 
    "<listOfFunctionTerms> cannot make qualitativeSpecies exceed maxLevel.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "No element of the <listOfFunctionTerms> object may cause the level "
    "of a <qualitativeSpecies> to exceed the value 'qual:maxLevel' attribute.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020414
  { QualTransitionLOFuncTermNegative, 
    "<listOfFunctionTerms> cannot make qualitativeSpecies negative.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "No element of the <listOfFunctionTerms> object may cause the level "
    "of a <qualitativeSpecies> to become negative.",
    { "L3V1 Qual V1 Section 3.6"
    }
  },

  // 3020501
  { QualInputAllowedCoreAttributes, 
    "Core attributes allowed on <input>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <input> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on an <input> object.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },
    
  // 3020502
  { QualInputAllowedElements, 
    "Elements allowed on <input>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <input> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on an <input>.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020503
  { QualInputAllowedAttributes, 
    "Attributes allowed on <input>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <input> object must have the required attributes "
    "'qual:qualitativeSpecies' as well as 'qual:transitionEffect' and "
    "may have the optional attributes 'qual:id', 'qual:name', 'qual:sign' "
    "and 'qual:thresholdLevel'.  No other attributes from the SBML Level 3 "
    "Qualitative Models namespace are permitted on an <input> object. ",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020504
  { QualInputNameMustBeString, 
    "Attribute 'name' on <input> must be string.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:name' in <input> must be of "
    "the data type string.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020505
  { QualInputSignMustBeSignEnum, 
    "Attribute 'sign' on <input> must be enum.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:sign' of an <input> object must conform "
    "to the syntax of the SBML data type 'sign' and may only take on "
    "the allowed values of 'sign' defined in SBML; that is, the value "
    "must be one of the following: 'positive', 'negative', 'dual' or "
    "'unknown'.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020506
  { QualInputTransEffectMustBeInputEffect, 
    "Attribute 'transitionEffect' on <input> must be enum.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:transitionEffect' of an <input> object "
    "must conform to the syntax of the SBML data type 'transitionInputEffect' "
    "and may only take on the allowed values of 'transitionInputEffect' "
    "defined in SBML; that is, the value must be one of the following: 'none' "
    "or 'consumption'.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020507
  { QualInputThreshMustBeInteger, 
    "Attribute 'thresholdLevel' on <input> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:thresholdLevel' in <input> must be of the data "
    "type 'integer'.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020508
  { QualInputQSMustBeExistingQS, 
    "Attribute 'qualitativeSpecies' on <input> must refer to existing",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:qualitativeSpecies' in an <input> "
    "object must be the identifier of an existing <qualitativeSpecies> object "
    "defined in the enclosing <model> object.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020509
  { QualInputConstantCannotBeConsumed, 
    "Constant <input> cannot be consumed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <input> that refers to a <qualitativeSpecies> that has a "
    "'qual:constant' attribute set to 'true' cannot have the attribute "
    "'qual:transitionEffect' set to 'consumption'.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020510
  { QualInputThreshMustBeNonNegative, 
    "Attribute 'thresholdLevel' on <input> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:thresholdLevel' in <input> must not be negative.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020601
  { QualOutputAllowedCoreAttributes, 
    "Core attributes allowed on <output>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <output> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on an <output> object.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },
    
  // 3020602
  { QualOutputAllowedElements, 
    "Elements allowed on <output>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <output> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on an <output>.",
    { "L3V1 Qual V1 Section 3.6.1"
    }
  },

  // 3020603
  { QualOutputAllowedAttributes, 
    "Attributes allowed on <output>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <output> object must have the required attributes "
    "'qual:qualitativeSpecies' as well as 'qual:transitionEffect' and "
    "may have the optional attributes 'qual:id', 'qual:name' "
    "and 'qual:outputLevel'.  No other attributes from the SBML Level 3 "
    "Qualitative Models namespace are permitted on an <output> object. ",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020604
  { QualOutputNameMustBeString, 
    "Attribute 'name' on <output> must be string.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:name' in <output> must be of "
    "the data type string.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020605
  { QualOutputTransEffectMustBeOutput, 
    "Attribute 'transitionEffect' on <output> must be enum.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:transitionEffect' of an <output> object "
    "must conform to the syntax of the SBML data type 'transitionOutputEffect' "
    "and may only take on the allowed values of 'transitionOutputEffect' "
    "defined in SBML; that is, the value must be one of the following: "
    "'production' or 'assignmentLevel'.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020606
  { QualOutputLevelMustBeInteger, 
    "Attribute 'outputLevel' on <output> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:outputLevel' in <output> must be of the data "
    "type 'integer'.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020607
  { QualOutputQSMustBeExistingQS, 
    "Attribute 'qualitativeSpecies' on <output> must refer to existing",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'qual:qualitativeSpecies' in an <output> "
    "object must be the identifier of an existing <qualitativeSpecies> object "
    "defined in the enclosing <model> object.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020608
  { QualOutputConstantMustBeFalse, 
    "Constant 'qualitativeSpecies' cannot be <output>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The <qualitativeSpecies> referred to by the attribute "
    "'qual:qualitativeSpecies' in an <output> object must have the value of "
    "its 'qual:constant' attribute set to 'false'.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020609
  { QualOutputProductionMustHaveLevel, 
    "<output> being produced must have level",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "When the value of the attribute 'qual:transitionEffect' of an <output> "
    "object is set to the value 'production' the attribute 'qual:outputLevel' "
    "for that particular <output> object must have a value set.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020610
  { QualOutputLevelMustBeNonNegative, 
    "Attribute 'outputLevel' on <output> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:outputLevel' in <output> must not be negative.",
    { "L3V1 Qual V1 Section 3.6.2"
    }
  },

  // 3020701
  { QualDefaultTermAllowedCoreAttributes, 
    "Core attributes allowed on <defaultTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <defaultTerm> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <defaultTerm> object.",
    { "L3V1 Qual V1 Section 3.6.4"
    }
  },
    
  // 3020702
  { QualDefaultTermAllowedElements, 
    "Elements allowed on <defaultTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <defaultTerm> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on a <defaultTerm>.",
    { "L3V1 Qual V1 Section 3.6.4"
    }
  },

  // 3020703
  { QualDefaultTermAllowedAttributes, 
    "Attributes allowed on <defaultTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <defaultTerm> object must have the required attributes "
    "'qual:resultLevel'.  No other attributes from the SBML Level 3 "
    "Qualitative Models namespace are permitted on a <defaultTerm> object. ",
    { "L3V1 Qual V1 Section 3.6.4"
    }
  },

  // 3020704
  { QualDefaultTermResultMustBeInteger, 
    "Attribute 'resultLevel' on <defaultTerm> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:resultLevel' in <defaultTerm> must be of the data "
    "type 'integer'.",
    { "L3V1 Qual V1 Section 3.6.4"
    }
  },

  // 3020705
  { QualDefaultTermResultMustBeNonNeg, 
    "Attribute 'resultLevel' on <defaultTerm> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:resultLevel' in <defaultTerm> must not be negative.",
    { "L3V1 Qual V1 Section 3.6.4"
    }
  },

  // 3020801
  { QualFuncTermAllowedCoreAttributes, 
    "Core attributes allowed on <functionTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <functionTerm> object may have the optional 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core.  No other attributes from "
    "the SBML Level 3 Core namespace or the Qualitative Models namespace "
    "are permitted on a <functionTerm> object.",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },
    
  // 3020802
  { QualFuncTermAllowedElements, 
    "Elements allowed on <functionTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <functionTerm> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the SBML "
    "Level 3 Core namespaces are permitted on a <functionTerm>.",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

  // 3020803
  { QualFuncTermAllowedAttributes, 
    "Attributes allowed on <functionTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <functionTerm> object must have the required attributes "
    "'qual:resultLevel'.  No other attributes from the SBML Level 3 "
    "Qualitative Models namespace are permitted on a <functionTerm> object. ",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

  // 3020804
  { QualFuncTermOnlyOneMath, 
    "Only one <math> on <functionTerm>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <functionTerm> object may contain exactly one MathML <math> element. "
    "No other elements from the SBML Level 3 Qualitative Models namespace "
    "are permitted on a <functionTerm> object.",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

  // 3020805
  { QualFuncTermResultMustBeInteger, 
    "Attribute 'resultLevel' on <functionTerm> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:resultLevel' in <functionTerm> must be of the data "
    "type 'integer'.",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  },

  // 3020806
  { QualFuncTermResultMustBeNonNeg, 
    "Attribute 'resultLevel' on <functionTerm> must be non negative integer.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The attribute 'qual:resultLevel' in <functionTerm> must not be negative.",
    { "L3V1 Qual V1 Section 3.6.5"
    }
  }

};    

LIBSBML_CPP_NAMESPACE_END

  /** @endcond */


#endif  /*  QualSBMLErrorTable_h__  */

