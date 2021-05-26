/**
 * @file    FbcSBMLErrorTable.h
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
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->
 */
 
#include <string>
#include <iostream>
#include <sstream>

#include <sbml/packages/fbc/validator/FbcSBMLError.h>


LIBSBML_CPP_NAMESPACE_BEGIN

/** @cond doxygenLibsbmlInternal */
static const packageErrorTableEntryV2 fbcErrorTableV2[] =
{
  // 2010100
  { FbcUnknown, 
    "Unknown error from fbc",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "Unknown error from fbc",
    { "",
      ""
    }
  },

  // 2010101
  { FbcNSUndeclared, 
    "The fbc ns is not correctly declared",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "To conform to Version 1 of the Flux Balance Constraints "
    "package specification for SBML Level 3, an "
    "SBML document must declare the use of the following XML Namespace: "
    "'http://www.sbml.org/sbml/level3/version1/fbc/version1'",
    { "L3V1 Fbc V1 Section 3.1",
      "L3V1 Fbc V2 Section 3.1"
    }
  },

  // 2010102
  { FbcElementNotInNs, 
    "Element not in fbc namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "Wherever they appear in an SBML document, "
    "elements and attributes from the Flux Balance Constraints "
    "package must be declared either implicitly or explicitly to be in the "
    "XML namespace 'http://www.sbml.org/sbml/level3/version1/fbc/version1'",
    { "L3V1 Fbc V1 Section 3.1",
      "L3V1 Fbc V2 Section 3.1"
    }
  },

    // 2010301
  { FbcDuplicateComponentId, 
  "Duplicate 'id' attribute value",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "(Extends validation rule #10301 in the SBML Level 3 Version 1 Core "
    "specification.) Within a <model> object "
    "the values of the attributes id and fbc:id on every instance of the "
    "following classes of objects must be unique across the set of all id "
    "and fbc:id attribute values of all such objects in a model: the "
    "model itself, plus all contained <functionDefinition>, <compartment>, "
    "<species>, <reaction>, <speciesReference>, <modifierSpeciesReference>, "
    "<event>, and <parameter> objects, plus the <fluxBound>, <objective>, "
    "<fluxObjective>, <geneProduct> and <geneProductAssociation> "
    "objects defined by the Flux Balance Constraints package.", 
    { "L3V1 Fbc V1 Section 3.2",
      "L3V1 Fbc V2 Section 3.2"
    }
  },

    // 2010302
  { FbcSBMLSIdSyntax, 
    "Invalid 'id' attribute",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of a fbc:id attribute must always conform to the syntax "
    "of the SBML data type SId.", 
    { "L3V1 Fbc V1 Section 3.2",
      "L3V1 Fbc V2 Section 3.2"
    }
  },

  // 2020101
  { FbcAttributeRequiredMissing, 
    "Required fbc:required attribute on <sbml>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "In all SBML documents using the Flux Balance Constraints "
    "package, the SBML object must include a value for the attribute "
    "'fbc:required.",
    { "L3V1 Core Section 4.1.2",
      "L3V1 Core Section 4.1.2"
    }
  },

  // 2020102
  { FbcAttributeRequiredMustBeBoolean, 
    "The fbc:required attribute must be Boolean",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:required' on the SBML object must "
    "be of the data type Boolean.",
    { "L3V1 Core Section 4.1.2",
      "L3V1 Core Section 4.1.2"
    }
  },

  // 2020103
  { FbcRequiredFalse, 
    "The fbc:required attribute must be 'false'",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:required' on the SBML object must "
    "be set to 'false'.",
    { "L3V1 Fbc V1 Section 3.1",
      "L3V1 Fbc V2 Section 3.1"
    }
  },

  // 2020201
  { FbcOnlyOneEachListOf, 
    "One of each list of allowed",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of each of the "
    "following kinds of objects within a <model> object using Flux Balance "
    "Constraints: <listOfFluxBounds> (V1 only), <listOfObjectives> "
    "and <listOfGeneProducts> (V2).",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020202
  { FbcNoEmptyListOfs, 
    "ListOf elements cannot be empty",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The various ListOf subobjects with a <model> object are optional, "
    "but if present, these container object must not be empty. Specifically, "
    "if any of the following classes of objects are present on the <model>, "
    "it must not be empty: <listOfFluxBounds> (V1 only), <listOfObjectives> "
    "and <listOfGeneProducts> (V2).",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020203
  { FbcLOFluxBoundsAllowedElements, 
    "Allowed elements on ListOfFluxBounds",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfFluxBounds> container object may only "
    "contain <fluxBound> objects.",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020204
  { FbcLOObjectivesAllowedElements, 
    "Allowed elements on ListOfObjectives",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfObjectives> container object may only "
    "contain <objective> objects.",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020205
  { FbcLOFluxBoundsAllowedAttributes, 
    "Allowed attributes on ListOfFluxBounds",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "A <listOfFluxBounds> object may have the optional attributes 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core. No other attributes from "
    "the SBML Level 3 Core namespace or the Flux Balance Constraints "
    "namespace are permitted on a <listOfFluxBounds> object. ",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020206
  { FbcLOObjectivesAllowedAttributes, 
    "Allowed attributes on ListOfObjectives",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A <listOfFluxBounds> object may have the optional attributes 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core. Additionally the "
    "<listOfObjectives> must contain the attribute 'activeObjective'. "
    "No other attributes from the SBML Level 3 Core namespace or the Flux "
    "Balance Constraints namespace are permitted on a "
    "<listOfFluxBounds> object. ",
    { "L3V1 Fbc V1 Section 3.3",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020207
  { FbcActiveObjectiveSyntax, 
    "Type of activeObjective attribute",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:activeObjective' on the <listOfObjectives> "
    "object must be of the data type SIdRef.",
    { "L3V1 Fbc V1 Section 3.2.2",
      "L3V1 Fbc V2 Section 3.3.1"
    }
  },

  // 2020208
  { FbcActiveObjectiveRefersObjective, 
    "ActiveObjective must reference Objective",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:activeObjective' on the <listOfObjectives> "
    "object must be the identifier of an existing <objective>.",
    { "L3V1 Fbc V1 Section 3.2.2",
      "L3V1 Fbc V2 Section 3.3.1"
    }
  },

  // 2020209
  { FbcModelMustHaveStrict, 
    "'Strict' attribute required on <model>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <model> object must have the required attributes 'strict'. "
    "No other attributes from the Flux "
    "Balance Constraints namespace are permitted on a "
    "<model> object. ",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020210
  { FbcModelStrictMustBeBoolean, 
    "'Strict' attribute must be boolean",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:strict' on the <model> "
    "object must have a value of datatype boolean.",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020211
  { FbcLOGeneProductsAllowedElements, 
    "Allowed elements on ListOfGeneProducts",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfGeneProducts> container object may only "
    "contain <geneProduct> objects.",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020212
  { FbcLOGeneProductsAllowedAttributes, 
    "Allowed attributes on ListOfGeneProducts",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <listOfGeneProducts> object may have the optional attributes 'metaid' "
    "and 'sboTerm' defined by SBML Level 3 Core. "
    "No other attributes from the SBML Level 3 Core namespace or the Flux "
    "Balance Constraints namespace are permitted on a "
    "<listOfGeneProducts> object. ",
    { "",
      "L3V1 Fbc V2 Section 3.3.1"
    }
  },

  // 2020301
  { FbcSpeciesAllowedL3Attributes, 
    "Species allowed attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A SBML <species> object may have the optional attributes 'fbc:charge' "
    "and 'fbc:chemicalFormula'. No other attributes from the Flux Balance "
    "Constraints namespaces are permitted on a <species>. ",
    { "L3V1 Fbc V1 Section 3.4",
      "L3V1 Fbc V2 Section 3.4"
    }
  },

  // 2020302
  { FbcSpeciesChargeMustBeInteger, 
    "Charge must be integer",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:charge' on SBML <species> object "
    "must be of the data type integer.",
    { "L3V1 Fbc V1 Section 3.4",
      "L3V1 Fbc V2 Section 3.4"
    }
  },

  // 2020303
  { FbcSpeciesFormulaMustBeString, 
    "Chemical formula must be string",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'fbc:chemicalFormula' on the SBML <species> "
    "object must be set to a string consisting only of atomic names or user "
    "defined compounds and their occurrence.",
    { "L3V1 Fbc V1 Section 3.4",
      "L3V1 Fbc V2 Section 3.4"
    }
  },

  // 2020401
  { FbcFluxBoundAllowedL3Attributes, 
    "<fluxBound> may only have 'metaId' and 'sboTerm' from L3 namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "A <fluxBound> object may have the optional SBML Level 3 Core "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the "
    "SBML Level 3 Core namespace are permitted on a <fluxBound>.",
    { "L3V1 Core, Section 3.2",
      ""
    }
  },
  
  // 2020402
  { FbcFluxBoundAllowedElements, 
    "<fluxBound> may only have <notes> and <annotations> from L3 Core",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "A <fluxBound> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the "
    "SBML Level 3 Core namespace are permitted on a <fluxBound>.",
    { "L3V1 Core, Section 3.2",
      ""
    }
  },
  
  // 2020403
  { FbcFluxBoundRequiredAttributes, 
    "Invalid attribute found on <fluxBound> object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "A <fluxBound> object must have the required attributes 'fbc:reaction', "
    "'fbc:operation' and 'fbc:value', and may have the optional attributes "
    "'fbc:id' and 'fbc:name'. No other attributes from the SBML Level 3 Flux "
    "Balance Constraints namespace are permitted on a <fluxBound> object.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020404
  { FbcFluxBoundRectionMustBeSIdRef, 
    "Datatype for 'fbc:reaction' must be SIdRef",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The attribute 'fbc:reaction' of a <fluxBound> must be of the data "
    "type SIdRef.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020405
  { FbcFluxBoundNameMustBeString, 
    "The attribute 'fbc:name' must be of the data type string",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The attribute 'fbc:name' of a <fluxBound> must be of the data type "
    "string.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020406
  { FbcFluxBoundOperationMustBeEnum, 
    "The attribute 'fbc:operation' must be of data type FbcOperation",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The attribute 'fbc:operation' of a <fluxBound> must be of the data "
    "type FbcOperation and thus it's value must be one of 'lessEqual', "
    "'greaterEqual' or 'equal'.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020407
  { FbcFluxBoundValueMustBeDouble, 
    "The attribute 'fbc:value' must be of the data type double",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The attribute 'fbc:value' of a <fluxBound> must be of the "
    "data type double.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020408
  { FbcFluxBoundReactionMustExist, 
    "'fbc:reaction' must refer to valid reaction",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The value of the attribute 'fbc:reaction' of a <fluxBound> object "
    "must be the identifier of an existing <reaction> object defined in "
    "the enclosing <model> object.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020409
  { FbcFluxBoundsForReactionConflict, 
    "Conflicting set of FluxBounds for a reaction",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_NOT_APPLICABLE,
    "The combined set of all <fluxBound>'s with identical values for "
    "'fbc:reaction' must be consistent. That is while it is possible "
    "to define a lower and an upper bound for a reaction, it is not "
    "possible to define multiple lower or upper bounds.",
    { "L3V1 Fbc V1 Section 3.5",
      ""
    }
  },
  
  // 2020501
  { FbcObjectiveAllowedL3Attributes, 
    "<objective> may only have 'metaId' and 'sboTerm' from L3 namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "An <objective> object may have the optional SBML Level 3 Core "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the SBML "
    "Level 3 Core namespace are permitted on an <objective>. ",
    { "L3V1 Core, Section 3.2",
      "L3V1 Core, Section 3.2"
    }
  },
  
  // 2020502
  { FbcObjectiveAllowedElements, 
    "<objective> may only have <notes> and <annotations> from L3 Core",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "An <objective> object may have the optional SBML Level 3 Core subobjects "
    "for notes and annotations. No other elements from the SBML Level 3 Core "
    "namespace are permitted on an <objective>.",
    { "L3V1 Core, Section 3.2",
      "L3V1 Core, Section 3.2"
    }
  },
  
  // 2020503
  { FbcObjectiveRequiredAttributes, 
    "Invalid attribute found on <objective> object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "An <objective> object must have the required attributes 'fbc:id' and "
    "'fbc:type' and may have the optional attribute 'fbc:name'. No other "
    "attributes from the SBML Level 3 Flux Balance Constraints namespace "
    "are permitted on an <objective> object.",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020504
  { FbcObjectiveNameMustBeString, 
    "The attribute 'fbc:name' must be of the data type string",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:name' on an <objective> must be of the data type "
    "'string'. ",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020505
  { FbcObjectiveTypeMustBeEnum, 
    "The attribute 'fbc:type' must be of data type FbcType.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:type' on an <objective> must be of the data type "
    "FbcType and thus its value must be "
    "one of 'minimize' or 'maximize.",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020506
  { FbcObjectiveOneListOfObjectives, 
    "An <objective> must have one <listOfFluxObjectives>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "An <objective> object must have one and only one instance of the "
    "<listOfFluxObjectives> object. ",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020507
  { FbcObjectiveLOFluxObjMustNotBeEmpty, 
    "<listOfFluxObjectives> subobject must not be empty",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The <listOfFluxObjectives> subobject within an <objective> object must "
    "not be empty.",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020508
  { FbcObjectiveLOFluxObjOnlyFluxObj, 
    "Invalid element found in <listOfFluxObjectives>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on all "
    "SBML objects, a <listOfFluxObjectives> container object may only contain "
    "<fluxObjective> objects.",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020509
  { FbcObjectiveLOFluxObjAllowedAttribs, 
    "<listOfFluxObjectives> may only have 'metaId' and 'sboTerm' from L3 core",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A <listOfFluxObjectives> object may have the optional 'metaid' and "
    "'sboTerm' defined by SBML Level 3 Core. No other attributes from the "
    "SBML Level 3 Core namespace or the Flux Balance Constraints namespace "
    "are permitted on a <listOfFluxObjectives> object",
    { "L3V1 Fbc V1, Section 3.6",
      "L3V1 Fbc V2, Section 3.6"
    }
  },
  
  // 2020601
  { FbcFluxObjectAllowedL3Attributes, 
    "<fluxObjective> may only have 'metaId' and 'sboTerm' from L3 namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A <fluxObjective> object may have the optional SBML Level 3 Core "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the SBML "
    "Level 3 Core namespace are permitted on a <fluxObjective>. ",
    { "L3V1 Core, Section 3.2",
      "L3V1 Core, Section 3.2"
    }
  },
  
  // 2020602
  { FbcFluxObjectAllowedElements, 
    "<fluxObjective> may only have <notes> and <annotations> from L3 Core",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A <fluxObjective> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations. No other elements from the "
    "SBML Level 3 Core namespace are permitted on a <fluxObjective>.",
    { "L3V1 Core, Section 3.2",
      "L3V1 Core, Section 3.2"
    }
  },
  
  // 2020603
  { FbcFluxObjectRequiredAttributes, 
    "Invalid attribute found on <fluxObjective> object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "A <fluxObjective> object must have the required attributes "
    "'fbc:reaction' and 'fbc:coefficient', and may have the optional "
    "attributes 'fbc:id' and 'fbc:name'. No other attributes from the "
    "SBML Level 3 Flux Balance Constraints namespace are permitted on "
    "a <fluxObjective> object.",
    { "L3V1 Fbc V1, Section 3.7",
      "L3V1 Fbc V2, Section 3.7"
    }
  },
  
  // 2020604
  { FbcFluxObjectNameMustBeString, 
    "The attribute 'fbc:name' must be of the data type string",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:name' on a <fluxObjective> must be of the data "
    "type 'string'.",
    { "L3V1 Fbc V1, Section 3.7",
      "L3V1 Fbc V2, Section 3.7"
    }
  },
  
  // 2020605
  { FbcFluxObjectReactionMustBeSIdRef, 
    "Datatype for 'fbc:reaction' must be SIdRef",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'fbc:reaction' of a <fluxObjective> object "
    "must conform to the syntax of the SBML data type 'SIdRef'.",
    { "L3V1 Fbc V1, Section 3.7",
      "L3V1 Fbc V2, Section 3.7"
    }
  },
  
  // 2020606
  { FbcFluxObjectReactionMustExist, 
    "'fbc:reaction' must refer to valid reaction",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'fbc:reaction' of a "
    "<fluxObjective> object must be the identifier of an existing <reaction> "
    "object defined in the enclosing <model> object.",
    { "L3V1 Fbc V1, Section 3.7",
      "L3V1 Fbc V2, Section 3.7"
    }
  },
  
  // 2020607
  { FbcFluxObjectCoefficientMustBeDouble, 
    "The attribute 'fbc:coefficient' must be of the data type double",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'fbc:coefficient' of a <fluxObjective> "
    "object must conform to the syntax of the SBML data type 'double'.",
    { "L3V1 Fbc V1, Section 3.7",
      "L3V1 Fbc V2, Section 3.7"
    }
  },

  // 2020608
  { FbcFluxObjectCoefficientWhenStrict, 
    "The 'fbc:coefficient' must be declared when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <model> 'fbc:strict' attribute is true, "
    "the value of the attribute 'fbc:coefficient' of a <fluxObjective> "
    "object must not be set to 'NaN', 'INF' or '-INF'.",
    { "",
      "L3V1 Fbc V2, Section 3.3"
    }
  },

  // 2020701
  { FbcReactionOnlyOneGeneProdAss,
    "One GeneProductAssociation in a Reaction",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of a "
    "<GeneProductAssociation> within a <Reaction> object using Flux Balance"
    "Constraints. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020702
  { FbcReactionAllowedAttributes,
    "Fbc attributes on a Reaction",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An SBML <Reaction> object may have the "
    "optional attributes 'fbc:lowerFluxBound' and "
    "'fbc:upperFluxBound'. No other attributes from the Flux Balance "
    "Constraints namespaces are permitted on a <Reaction>. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020703
  { FbcReactionLwrBoundSIdRef,
    "LowerBound must be SIdRef",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:lowerFluxBound' of a <Reaction> "
    "must be of the data type 'SIdRef'. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020704
  { FbcReactionUpBoundSIdRef,
    "UpperBound must be SIdRef",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:upperFluxBound' of a <Reaction> "
    "must be of the data type 'SIdRef'. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020705
  { FbcReactionLwrBoundRefExists,
    "LowerBound must point to Parameter",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:lowerFluxBound' of a <Reaction> "
    "must point to an existing <Parameter> "
    "in the model. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020706
  { FbcReactionUpBoundRefExists,
    "UpperBound must point to Parameter",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:upperFluxBound' of a <Reaction> "
    "must point to an existing <Parameter> "
    "in the model. ",
    { "",
      "L3V1 Fbc V2 Section 3.8"
    }
  },

  // 2020707
  { FbcReactionMustHaveBoundsStrict,
    "Reaction must have bounds when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', a <Reaction> must define the attributes "
    "'fbc:lowerFluxBound' and 'fbc:upperFluxBound'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020708
  { FbcReactionConstantBoundsStrict,
    "Reaction bounds constant when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> objects referred to by the attributes "
    "'fbc:lowerFluxBound' and 'fbc:upperFluxBound' must have "
    "their 'constant' attribute set to 'true'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020709
  { FbcReactionBoundsMustHaveValuesStrict,
    "Reaction bounds must have values when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> objects referred to by the attributes "
    "'fbc:lowerFluxBound' and 'fbc:upperFluxBound' must have a "
    "defined value for their 'value' attribute, which may not be 'NaN'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020710
  { FbcReactionBoundsNotAssignedStrict,
    "Reaction bounds not assigned when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> objects referred to by the attributes "
    "'fbc:lowerFluxBound' and 'fbc:upperFluxBound' may not be "
    "targeted by an <InitialAssignment>. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020711
  { FbcReactionLwrBoundNotInfStrict,
    "LowerBound cannot be INF when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> objects referred to by "
    "the attribute 'fbc:lowerFluxBound' may not have the value 'INF'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020712
  { FbcReactionUpBoundNotNegInfStrict,
    "UpperBound cannot be -INF when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> object referred to by "
    "the attribute 'fbc:upperFluxBound' may not have the value '-INF'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020713
  { FbcReactionLwrLessThanUpStrict,
    "LowerBound less than upperBound when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <Parameter> object referred to by "
    "the attribute 'fbc:lowerFluxBound' must be less than or equal to "
    "the value of the <Parameter> object referred to by the attribute "
    "'fbc:upperFluxBound' . "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020714
  { FbcSpeciesReferenceConstantStrict,
    "SpeciesReferences must be constant when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the 'constant' attribute of <SpeciesReference> "
    "elements of a <Reaction> must be set to 'true'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020715
  { FbcSpeciesRefsStoichMustBeRealStrict,
    "Stoichiometry of SpeciesReferences must be real valued when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the value of a <SpeciesReference>'s 'stoichiometry> "
    "attribute must not be set to 'NaN',  '-INF' or  'INF'. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020716
  { FbcSpeciesRefNotAssignedStrict,
    "SpeciesReference not target of InitialAssignment when strict",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "When the value of the <Model>s 'fbc:strict' "
    "attribute is 'true', the <SpeciesReference> elements of a <Reaction> may "
    "not be targeted by an <InitialAssignment>. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.3"
    }
  },

  // 2020801
  { FbcGeneProdAssocAllowedCoreAttribs,
    "Allowed core attributes on <GeneProductAssociation>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductAssociation> object may have the optional SBML "
    "Level 3 Core attributes 'metaid' and 'sboTerm'. No other "
    "attributes from the SBML Level 3 Core namespace are permitted on a "
    "<GeneProductAssociation. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2020802
  { FbcGeneProdAssocAllowedCoreElements,
    "Allowed core elements on <GeneProductAssociation>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductAssociation> object may have the optional SBML "
    "Level 3 Core subobjects for notes and annotations. No other elements "
    "from the SBML Level 3 Core namespace are permitted on a <GeneProductAssociation. "
    "",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2020803
  { FbcGeneProdAssocAllowedAttribs,
    "Allowed fbc attributes on <GeneProductAssociation>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductAssociation> object may have the "
    "optional attributes 'fbc:id' and 'fbc:name'. No other attributes from the SBML "
    "Level 3 Flux Balance Constraints namespace are permitted on a <GeneProductAssociation> "
    "object. ",
    { "",
      "L3V1 Fbc V2 Section 3.9"
    }
  },

  // 2020804
  { FbcGeneProdAssocIdSyntax,
    "'fbc:id' must have SId syntax",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:id' on a <GeneProductAssociation> "
    "must be of the data type 'SId'. ",
    { "",
      "L3V1 Fbc V2 Section 3.9"
    }
  },

  // 2020805
  { FbcGeneProdAssocContainsOneElement,
    "<GeneProductAssociation> must contain one concrete object",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductAssociation> object must have one and only one "
    "of the concrete <Association> objects:  <GeneProductRef>, <And> or <Or>.",
    { "",
      "L3V1 Fbc V2 Section 3.9"
    }
  },

  // 2020806
  { FbcGeneProdAssocNameMustBeString,
    "'fbc:name' must be string",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:name' on a <GeneProductAssociation> "
    "must be of the data type 'string'. ",
    { "",
      "L3V1 Fbc V2 Section 3.9"
    }
  },

  // 2020901
  { FbcGeneProdRefAllowedCoreAttribs,
    "Allowed core attributes on <GeneProductRef>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductRef> object may have the optional SBML "
    "Level 3 Core attributes 'metaid' and 'sboTerm'. No other "
    "attributes from the SBML Level 3 Core namespace are permitted on a "
    "<GeneProductRef>. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2020902
  { FbcGeneProdRefAllowedCoreElements,
    "Allowed core elements on <GeneProductRef>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductRef> object may have the optional SBML "
    "Level 3 Core subobjects for notes and annotations. No other elements "
    "from the SBML Level 3 Core namespace are permitted on a <GeneProductRef>. "
    "",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2020903
  { FbcGeneProdRefAllowedAttribs,
    "Allowed fbc attributes on <GeneProductRef>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProductRef> object must have the required "
    "attribute 'fbc:geneProduct' and may have the "
    "optional attribute 'fbc:id'. No other "
    "attributes from the SBML Level 3 Flux Balance Constraints namespace are "
    "permitted on a <> object. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.11"
    }
  },

  // 2020904
  { FbcGeneProdRefGeneProductSIdRef,
    "'fbc:geneProduct' must be SIdRef",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:geneProduct' on a <GeneProductRef> "
    "must be of the data type 'SIdRef'. ",
    { "",
      "L3V1 Fbc V2 Section 3.11"
    }
  },

  // 2020908
  { FbcGeneProdRefGeneProductExists,
    "'fbc:geneProduct' must point to existing <GeneProduct>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:geneProduct' on a <GeneProductRef> "
    " if set, must refer to 'id' of a <GeneProduct> in the <Model>. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.11"
    }
  },

  // 2021001
  { FbcAndAllowedCoreAttributes,
    "Allowed core attributes on <And>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <And> object may have the optional SBML "
    "Level 3 Core attributes 'metaid' and 'sboTerm'. No other "
    "attributes from the SBML Level 3 Core namespace are permitted on an "
    "<GeneAnd. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021002
  { FbcAndAllowedCoreElements,
    "Allowed core attributes on <And>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <And> object may have the optional SBML "
    "Level 3 Core subobjects for notes and annotations. No other elements "
    "from the SBML Level 3 Core namespace are permitted on an <And>. "
    "",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021003
  { FbcAndTwoChildren,
    "<And> must have at least two child elements",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <And> object must have two or more concrete "
    "<Association> objects: <GeneProductRef>, <And>, or <Or>. No other "
    "elements from the SBML Level 3 Flux Balance Constraints namespace are "
    "permitted on an <And> object. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.12"
    }
  },

  // 2021101
  { FbcOrAllowedCoreAttributes,
    "Allowed core attributes on <Or>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <Or> object may have the optional SBML "
    "Level 3 Core attributes 'metaid' and 'sboTerm'. No other "
    "attributes from the SBML Level 3 Core namespace are permitted on an "
    "<Or>. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021102
  { FbcOrAllowedCoreElements,
    "Allowed core elements on <Or>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <Or> object may have the optional SBML "
    "Level 3 Core subobjects for notes and annotations. No other elements "
    "from the SBML Level 3 Core namespace are permitted on an <Or>. "
    "",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021103
  { FbcOrTwoChildren,
    "<Or> must have at least two child elements",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "An <Or> object must have two or more concrete "
    "<Association> objects: <GeneProductRef>, <And>, or <Or>. No other "
    "elements from the SBML Level 3 Flux Balance Constraints namespace are "
    "permitted on an <Or> object. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.13"
    }
  },

  // 2021201
  { FbcGeneProductAllowedCoreAttributes,
    "Allowed core attributes on <GeneProduct>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProduct> object may have the optional SBML "
    "Level 3 Core attributes 'metaid' and 'sboTerm'. No other "
    "attributes from the SBML Level 3 Core namespace are permitted on a "
    "<GeneProduct>. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021202
  { FbcGeneProductAllowedCoreElements,
    "Allowed core elements on <GeneProduct>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProduct> object may have the optional SBML "
    "Level 3 Core subobjects for notes and annotations. No other elements "
    "from the SBML Level 3 Core namespace are permitted on a <GeneProduct>. ",
    { "",
      "L3V1 Core Section 3.2"
    }
  },

  // 2021203
  { FbcGeneProductAllowedAttributes,
    "Allowed fbc attributes on <GeneProduct>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "A <GeneProduct> object must have the required "
    "attributes 'fbc:id' and 'fbc:label' may have the "
    "optional attributes 'fbc:name' and 'fbc:associatedSpecies'. No other "
    "attributes from the SBML Level 3 Flux Balance Constraints namespace are "
    "permitted on a <GeneProduct> object. "
    "",
    { "",
      "L3V1 Fbc V2 Section 3.5"
    }
  },

  // 2021204
  { FbcGeneProductLabelMustBeString,
    "'fbc:label' must be string",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:label' on a <GeneProduct> "
    "must be of the data type 'string'. ",
    { "",
      "L3V1 Fbc V2 Section 3.5"
    }
  },

  // 2021205
  { FbcGeneProductLabelMustBeUnique,
    "'fbc:label' must be unique",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:label' on a <GeneProduct> "
    "must be unique among the set of all <GeneProduct> elements defined in the "
    "<Model>. ",
    { "",
      "L3V1 Fbc V2 Section 3.5"
    }
  },

  // 2021206
  { FbcGeneProductNameMustBeString,
    "'fbc:name' must be string",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:name' on a <GeneProduct> "
    "must be of the data type 'string'. ",
    { "",
      "L3V1 Fbc V2 Section 3.5"
    }
  },

  // 2021207
  { FbcGeneProductAssocSpeciesMustExist,
    "'fbc:associatedSpecies' must point to existing <species>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_NOT_APPLICABLE,
    LIBSBML_SEV_ERROR,
    "The attribute 'fbc:associatedSpecies' "
    "on a <GeneProduct> must be the identifier of an existing <Species> defined "
    "in the enclosing <Model>.  ",
    { "",
      "L3V1 Fbc V2 Section 3.5"
    }
  }

};

LIBSBML_CPP_NAMESPACE_END
/** @endcond */

