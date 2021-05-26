/**
 * @file    CompSBMLErrorTable.h
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
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/
#ifndef CompSBMLErrorTable_h
#define CompSBMLErrorTable_h


#include <sbml/packages/comp/validator/CompSBMLError.h>


LIBSBML_CPP_NAMESPACE_BEGIN

/** @cond doxygenLibsbmlInternal */
static const packageErrorTableEntry compErrorTable[] =
{
  // 1010100
  { CompUnknown, 
    "Unknown error from comp",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Unknown error from comp",
    { ""
    }
  },

  // 1010101
  { CompNSUndeclared, 
    "The comp ns is not correctly declared",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "To conform to Version 1 of the Hierarchical Model "
    "Composition package specification for SBML Level 3, an "
    "SBML document must declare the use of the following XML Namespace: "
    "'http://www.sbml.org/sbml/level3/version1/comp/version1'",
    { "L3V1 Comp V1 Section 3.1"
    }
  },

  // 1010102
  { CompElementNotInNs, 
    "Element not in comp namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Wherever they appear in an SBML document, "
    "elements and attributes from the Hierarchical Model Composition "
    "package must be declared either implicitly or explicitly to be in the "
    "XML namespace 'http://www.sbml.org/sbml/level3/version1/comp/version1'",
    { "L3V1 Comp V1 Section 3.1"
    }
  },

    // 1010301
  { CompDuplicateComponentId, 
    "Duplicate 'id' attribute value",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "(Extends validation rule #10301 in the SBML Level 3 Version 1 Core "
    "specification.) Within a <model> or <modelDefinition> object, "
    "the values of the attributes id and comp:id on every instance of the "
    "following classes of objects must be unique across the set of all id "
    "and comp:id attribute values of all such objects in a model: the "
    "model itself, plus all contained <functionDefinition>, <compartment>, "
    "<species>, <reaction>, <speciesReference>, <modifierSpeciesReference>, "
    "<event>, and <parameter> objects, plus the <submodel> and <deletion> "
    "objects defined by the Hierarchical Model Composition package, plus "
    "any objects defined by any other package with 'package:id' "
    "attributes defined as falling in the 'SId' namespace.", 
    { "L3V1 Comp V1 Section 3.9"
    }
  },

    // 1010302
  { CompUniqueModelIds, 
    "Model and ExternalModelDefinitions must have unique ids",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The values of the attributes 'id' and "
    "'comp:id' on every instance of all <model>, <modelDefinition>, and "
    "<externalModelDefinition> objects must be unique across the set of all "
    "'id' and 'comp:id' attribute values of such objects "
    "in the SBML document to which they belong.",
    { "L3V1 Comp V1 Section 3.9"
    }
  },

    // 1010303
  { CompUniquePortIds, 
    "Ports must have unique ids",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Within <model> and <modelDefinition> objects inside an SBML document, "
    "the value of the attribute 'comp:id' on every instance of a "
    "<port> object must be unique across the set of all 'comp:id' "
    "attribute values of all such objects in the model.",
    { "L3V1 Comp V1 Section 3.9"
    }
  },

    // 1010304
  { CompInvalidSIdSyntax, 
    "Invalid SId syntax",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a comp:id attribute must always conform to the "
    "syntax of the SBML data type SId.",
    { "L3V1 Core Section 3.1.7"
    }
  },


  // 1010308
  { CompInvalidSubmodelRefSyntax, 
    "Invalid submodelRef syntax",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:submodelRef' attribute "
    "on <replacedElement> and <replacedBy> objects must always conform to the "
    "syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1010309
  { CompInvalidDeletionSyntax, 
    "Invalid deletion syntax",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:deletion' attribute "
    "on <replacedElement> objects must always conform to the "
    "syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1010310
  { CompInvalidConversionFactorSyntax, 
    "Invalid conversionFactor syntax",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:conversionFactor' attribute "
    "on <replacedElement> objects must always conform to the "
    "syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1010311
  { CompInvalidNameSyntax, 
    "Invalid name syntax",
    LIBSBML_CAT_IDENTIFIER_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:name' attribute "
    "must always conform to the "
    "syntax of type string.",
    { "L3V1 Core Section 3.1.1"
    }
  },

  // 1010501
  { CompReplacedUnitsShouldMatch, 
    "Units of replaced elements should match replacement units.",
    LIBSBML_CAT_UNITS_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "If one element replaces another, whether it is the target of "
    "a <replacedBy> element, or whether it has a child <replacedElement>, "
    "the units of the replaced element, multiplied by the units of any "
    "applicable conversion factor, should equal the units of the "
    "replacement element.",
    { "L3V1 Comp V1 Section 3.6.5"
    }
  },


  // 1020101
  { CompOneListOfReplacedElements, 
    "Only one <listOfReplacedElements> allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Any object derived from the extended SBase class "
    "(defined in the Hierarchical Model Composition package) may contain "
    "at most one instance of a <listOfReplacedElements> subobject.",
    { "L3V1 Comp V1 Section 3.6"
    }
  },

  // 1020102
  { CompLOReplaceElementsAllowedElements, 
    "Allowed children of <listOfReplacedElements>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfReplacedElements> container object may "
    "only contain <replacedElement> objects.",
    { "L3V1 Comp V1 Section 3.6"
    }
  },

  // 1020103
  { CompLOReplacedElementsAllowedAttribs, 
    "Allowed <listOfReplacedElements> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfReplacedElements> object may have the optional attributes "
    "'metaid' and 'sboTerm' defined by SBML Level 3 Core. No other "
    "attributes from the SBML Level 3 Core namespace or the "
    "HierarchicalModel Composition namespace are permitted on a "
    "<listOfReplacedElements> object.",
    { "L3V1 Comp V1 Section 3.6"
    }
  },

  // 1020104
  { CompEmptyLOReplacedElements, 
    "<listOfReplacedElements> must not be empty",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The <listOfReplacedElements> in an SBase object is optional, but "
    "if present, must not be empty.",
    { "L3V1 Comp V1 Section 3.6"
    }
  },

  // 1020105
  { CompOneReplacedByElement, 
    "Only one <replacedBy> object allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Any object derived from the extended SBase class "
    "(defined in the Hierarchical Model Composition package) may contain "
    "at most one instance of a <replacedBy> subobject.",
    { "L3V1 Comp V1 Section 3.6"
    }
  },

  // 1020201
  { CompAttributeRequiredMissing, 
    "Required comp:required attribute on <sbml>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "In all SBML documents using the HierarchicalModel Composition "
    "package, the SBML object must include a value for the attribute "
    "'comp:required' attribute.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 1020202
  { CompAttributeRequiredMustBeBoolean, 
    "The comp:required attribute must be Boolean",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of attribute 'comp:required' on the SBML object must "
    "be of the data type Boolean.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 1020203
  { CompRequiredTrueIfElementsRemain, 
    "The comp:required attribute must be 'true' if math changes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    "The value of attribute 'comp:required' on the SBML object must "
    "be set to 'true' if the Model object within the SBML object "
    "contains any Submodel with Species, Parameter, Compartment, "
    "Reaction, or Event objects that have not been either directly or "
    "indirectly replaced.",
    { "L3V1 Comp V1 Section 3.1"
    }
  },

  // 1020204
  { CompRequiredFalseIfAllElementsReplaced, 
    "The comp:required attribute must be 'false' if math does not change",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_NOT_APPLICABLE,
    "The value of attribute 'comp:required' on the SBML object must be "
    "set to 'false' if the Model object within the SBML object contains "
    "no Submodel objects, or if all Submodel objects that are present "
    "contain no Species, Parameter, Compartment, Reaction, or Event "
    "objects that have not been either directly or indirectly replaced.",
    { "L3V1 Comp V1 Section 3.1"
    }
  },

  // 1020205
  { CompOneListOfModelDefinitions, 
    "Only one <listOfModelDefinitions> allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of the <listOfModelDefinitions> "
    "within an SBML object that uses the SBML Level 3 Hierarchical "
    "Model Composition package.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020206
  { CompEmptyLOModelDefs, 
    "<listOfModelDefinitions> and <listOfExternalModelDefinitions> must not be empty",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The various 'ListOf' subobjects within an SBML object are optional, "
    "but if present, these container objects must not be empty. "
    "Specifically, if any of the following classes of objects is present "
    "within the SBML object, it must not be empty: "
    "<listOfModelDefinitions> and <listOfExternalModelDefinitions>.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020207
  { CompLOModelDefsAllowedElements, 
    "Only <modelDefinitions> in <listOfModelDefinitions>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfModelDefinitions> container may only "
    "contain <modelDefinition> objects.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020208
  { CompLOExtModelDefsAllowedElements, 
    "Only <externalModelDefinitions> in <listOfExternalModelDefinitions>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfExternalModelDefinitions> container may "
    "only contain <externalModelDefinition> objects.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020209
  { CompLOModelDefsAllowedAttributes, 
    "Allowed <listOfModelDefinitions> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfModelDefinitions> object may have the optional attributes "
    "'metaid' and 'sboTerm'. No other attributes from the SBML Level 3 "
    "Core namespace or the HierarchicalModel Composition namespace are "
    "permitted on a <listOfModelDefinitions> object.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020210
  { CompLOExtModDefsAllowedAttributes, 
    "Allowed <listOfExternalModelDefinitions> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfExternalModelDefinitions> object may have the optional "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the "
    "SBML Level 3 Core namespace or the HierarchicalModel Composition "
    "namespace are permitted on a <listOfExternalModelDefinitions> object.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020211
  { CompOneListOfExtModelDefinitions, 
    "Only one <listOfExternalModelDefinitions> allowed.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of the "
    "<listOfExternalModelDefinitions> within an SBML object that uses the "
    "SBML Level 3 Hierarchical Model Composition package.",
    { "L3V1 Comp V1 Section 3.3"
    }
  },

  // 1020212
  { CompAttributeRequiredMustBeTrue, 
    "The comp:required attribute must be 'true'",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of attribute 'comp:required' on the SBML object must "
    "be set to 'true'.",
    { "L3V1 Comp V1 Section 3.1"
    }
  },

  // 1020301
  { CompExtModDefAllowedCoreAttributes, 
    "Allowed <externalModelDefinitions> core attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <externalModelDefinition> object may have the optional "
    "SBML Level 3 Core attributes 'metaid' and 'sboTerm'.  No "
    "other attributes from the SBML Level 3 Core namespace are permitted on "
    "an <externalModelDefinition> object.",
    { "L3V1 Comp V1 Section 3.2"
    }
  },

  // 1020302
  { CompExtModDefAllowedElements, 
    "Allowed <externalModelDefinitions> elements",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <externalModelDefinition> object may have the optional "
    "SBML Level 3 Core subobjects for notes and annotation.  No "
    "other subobjects from the SBML Level 3 Core namespace or the "
    "Hierarchical Model Composition namespace are permitted on "
    "an <externalModelDefinition> object.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020303
  { CompExtModDefAllowedAttributes, 
    "Allowed <externalModelDefinitions> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <externalModelDefinition> object must have the attributes "
    "'comp:id' and 'comp:source', and may have the optional attributes "
    "'comp:name', 'comp:modelRef', and 'comp:md5'. No other attributes "
    "from the Hierarchical Model Composition namespace are permitted on "
    "an <externalModelDefinition> object.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020304
  { CompReferenceMustBeL3, 
    "External models must be L3",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the 'comp:source' attribute on an <externalModelDefinition> "
    "object must reference an SBML Level 3 Version 1 document.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020305
  { CompModReferenceMustIdOfModel, 
    "'modelRef' must be the 'id' of a model in the 'source' document",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of the 'comp:modelRef' attribute on an <externalModelDefinit"
    "ion> object must be the value of an id attribute on a <model>, "
    "<modelDefinition>, or <externalModelDefinition> object in the SBML "
    "document referenced by the 'comp:source' attribute.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020306
  { CompExtModMd5DoesNotMatch, 
    "MD5 checksum does not match the 'source' document",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The value of the 'comp:md5' attribute, if present on an "
    "<externalModelDefinition> object, should match the calculated MD5 "
    "checksum of the SBML document referenced by the 'comp:source' "
    "attribute. ",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020307
  { CompInvalidSourceSyntax, 
    "The 'comp:source' attribute must be of type 'anyURI'",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:source' attribute on an <externalModelDefinition> "
    "object must always conform to the syntax of the XML Schema 1.0 data "
    "type 'anyURI'.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020308
  { CompInvalidModelRefSyntax, 
    "The 'comp:modelRef' attribute must have the syntax of 'SId'",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a comp:modelRef attribute on an <externalModelDefinition> "
    "object must always conform to the syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020308
  { CompInvalidMD5Syntax, 
    "The 'comp:md5' attribute must have the syntax of 'string'",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a comp:md5 attribute on an <externalModelDefinition> "
    "object must always conform to the syntax of type string.",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020310
  { CompCircularExternalModelReference, 
    "Circular reference in <externalModelDefinition>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <externalModelDefinition> object must not reference an "
    "<externalModelDefinition> in a different SBML document that, in turn, "
    "refers back to the original <externalModelDefinition object, "
    "whether directly or indirectly through a chain of "
    "<externalModelDefinition> objects. ",
    { "L3V1 Comp V1 Section 3.3.2"
    }
  },

  // 1020501
  { CompOneListOfOnModel, 
    "Only one <listOfSubmodels> and one <listOfPorts> allowed",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "There may be at most one instance of each of the following kinds of "
    "objects within a <model> or <modelDefinition> object using "
    "Hierarchical Model Composition: <listOfSubmodels> and <listOfPorts>.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

  // 1020502
  { CompNoEmptyListOfOnModel, 
    "No empty listOf elements allowed",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The various ListOf subobjects with a <model> object are optional, "
    "but if present, these container object must not be empty.  "
    "Specifically, if any of the following classes of objects "
    "are present on the <model>, it must not be empty: <listOfSubmodels> "
    "and <listOfPorts>.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

  // 1020503
  { CompLOSubmodelsAllowedElements, 
    "Allowed elements on <listOfSubmodels>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects "
    "permitted on all SBML objects, a <listOfSubmodels> container "
    "object may only contain <submodel> objects.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

  // 1020504
  { CompLOPortsAllowedElements, 
    "Allowed elements on <listOfPorts>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects "
    "permitted on all SBML objects, a <listOfPorts> container "
    "object may only contain <port> objects.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

  // 1020505
  { CompLOSubmodelsAllowedAttributes, 
    "Allowed attributes on <listOfSubmodels>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfSubmodels> object may have the optional "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the "
    "SBML Level 3 Core namespace or the HierarchicalModel Composition "
    "namespace are permitted on a <listOfSubmodels> object.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

  // 1020506
  { CompLOPortsAllowedAttributes, 
    "Allowed attributes on <listOfPorts>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfPorts> object may have the optional "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the "
    "SBML Level 3 Core namespace or the HierarchicalModel Composition "
    "namespace are permitted on a <listOfPorts> object.",
    { "L3V1 Comp V1 Section 3.4"
    }
  },

    // 1010601
  { CompSubmodelAllowedCoreAttributes, 
    "Allowed core attributes on <submodel>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <submodel> object may have the optional SBML Level 3 Core "
    "attributes 'metaid' and 'sboTerm'.  No other attributes from "
    "the SBML Level 3 Core namespace are permitted on a <submodel> object.",
    { "L3V1 Core Section 3.2"
    }
  },

    // 1010602
  { CompSubmodelAllowedElements, 
    "Allowed elements on <submodel>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <submodel> object may have the optional SBML Level 3 Core "
    "subobjects for notes and annotations.  No other elements from "
    "the SBML Level 3 Core namespace are permitted on a <submodel> object.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 1020603
  { CompOneListOfDeletionOnSubmodel, 
    "Only one <listOfDeletions> on a <submodel> allowed",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "There may be at most one <listOfDeletions> container object within a "
    "<submodel> object.",
    { "L3V1 Comp V1 Section 3.5"
    }
  },

  // 1020604
  { CompSubmodelNoEmptyLODeletions, 
    "No empty listOfDeletions elements allowed",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfDeletions> container object within a <submodel> object "
    "is optional, but if present, must not be empty.",
    { "L3V1 Comp V1 Section 3.5"
    }
  },

  // 1020605
  { CompLODeletionsAllowedElements, 
    "Allowed elements on <listOfDeletions>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, a <listOfDeletions> container object may only contain "
    "<deletion> objects.",
    { "L3V1 Comp V1 Section 3.5"
    }
  },

  // 1020606
  { CompLODeletionAllowedAttributes, 
    "Allowed <listOfDeletions> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <listOfDeletions> object may have the optional attributes "
    "'metaid' and 'sboTerm' defined by SBML Level 3 Core. No other "
    "attributes from the SBML Level 3 Core namespace or the "
    "HierarchicalModel Composition namespace are permitted on a "
    "<listOfReplacedElements> object.",
    { "L3V1 Comp V1 Section 3.5"
    }
  },

  // 1020607
  { CompSubmodelAllowedAttributes, 
    "Allowed <submodel> attributes",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <submodel> object must have the attributes 'comp:id' and "
    "'comp:modelRef' because they are required, and may also have the "
    "optional attributes 'comp:name', 'comp:timeConversionFactor, and/or "
    "'comp:extentConversionFactor'.  No other attributes from the "
    "Hierarchical Model Composition namespace are permitted on a "
    "<submodel> object.",
    { "L3V1 Comp V1 Section 3.5"
    }
  },

  // 1020608
  { CompModReferenceSyntax, 
    "'comp:modelRef' must conform to SId syntax",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:modelRef' attribute on a <submodel> object must "
    "always conform to the syntax of the SBML data type SId. ",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020613
  { CompInvalidTimeConvFactorSyntax, 
    "'comp:timeConversionFactor' must conform to SId syntax",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:timeConversionFactor' attribute on a <submodel> object must "
    "always conform to the syntax of the SBML data type SId. ",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020614
  { CompInvalidExtentConvFactorSyntax, 
    "'comp:extentConversionFactor' must conform to SId syntax",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:extentConversionFactor' attribute on a <submodel> object must "
    "always conform to the syntax of the SBML data type SId. ",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020615
  { CompSubmodelMustReferenceModel, 
    "The 'comp:modelRef' attribute must reference a model",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:modelRef' attribute on a <submodel> must be the "
    "identifier of a <model>, <modelDefinition>, or "
    "<externalModelDefinition> object in the same SBML object as the "
    "<submodel>.",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020616
  { CompSubmodelCannotReferenceSelf, 
    "The 'comp:modelRef' attribute cannot reference own model",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <model> or <modelDefinition> object must not contain a <submodel> "
    "which references that model object itself. That is, the value of a "
    "'comp:modelRef' attribute on a <submodel> must not be the value of "
    "the parent <model> or <modelDefinition>'s 'id' attribute.",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020617
  { CompModCannotCircularlyReferenceSelf, 
    "<model> may not reference <submodel> that references itself.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <model> object must not contain a <submodel> which references that "
    "<model> indirectly. That is, the 'comp:modelRef' attribute of a "
    "<submodel> may not point to the 'id' of a <model> containing a "
    "<submodel> object that references the original <model> directly or "
    "indirectly through a chain of <model>/<submodel> pairs.",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020622
  { CompTimeConversionMustBeParameter, 
    "The 'comp:timeConversionFactor' must reference a parameter",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:timeConversionFactor' attribute on a given "
    "<submodel> object must be the identifier of a <parameter> object "
    "defined in the same Model containing the <submodel>.",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },

  // 1020623
  { CompExtentConversionMustBeParameter, 
    "The 'comp:extentConversionFactor' must reference a parameter",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:extentConversionFactor' attribute on a given "
    "<submodel> object must be the identifier of a <parameter> object "
    "defined in the same Model containing the <submodel>.",
    { "L3V1 Comp V1 Section 3.5.1"
    }
  },
  
  // 1020701
  { CompPortRefMustReferencePort, 
    "The 'comp:portRef' attribute must be the 'id' of a <port>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:portRef' attribute on an <sBaseRef> object "
    "must be the identifier of a <port> object in the <model> referenced "
    "by that <sBaseRef>.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020702
  { CompIdRefMustReferenceObject, 
    "The 'comp:idRef' attribute must be the 'id' of a model element",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:idRef' attribute on an <sBaseRef> object must "
    "be the identifier of an object contained in (that is, within the "
    "SId namespace of) the <model> referenced by that <sBaseRef>. This "
    "includes objects with 'id' attributes defined in packages other "
    "than SBML Level 3 Core or the Hierarchical Model Composition "
    "package.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020703
  { CompUnitRefMustReferenceUnitDef, 
    "The 'comp:unitRef' attribute must be the 'id' of a UnitDefinition",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:unitRef' attribute on an <sBaseRef> object must "
    "be the identifier of a <unitDefinition< object contained in the "
    "<model> referenced by that <sBaseRef>.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020704
  { CompMetaIdRefMustReferenceObject, 
    "The 'comp:metaIdRef' attribute must be the 'metaid' of an object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:metaIdRef' attribute on an <sBaseRef> object "
    "must be the value of a 'comp:metaid' attribute on an element contained "
    "in the <model> referenced by that <sBaseRef>.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020705
  { CompParentOfSBRefChildMustBeSubmodel, 
    "If <sBaseRef> has a child <sBaseRef> its parent must be a <submodel>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If an <sBaseRef> object contains an <sBaseRef> child, the parent "
    "<sBaseRef> must point to a <submodel> object, or a "
    "<port> that itself points to a <submodel> object. ",
    { "L3V1 Comp V1 Section 3.7.2"
    }
  },

  // 1020706
  { CompInvalidPortRefSyntax, 
    "The 'comp:portRef' attribute must have the syntax of an SBML SId",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:portRef' attribute on an SBaseRef object must "
    "always conform to the syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020707
  { CompInvalidIdRefSyntax, 
    "The 'comp:idRef' attribute must have the syntax of an SBML SId",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:idRef' attribute on an SBaseRef object must "
    "always conform to the syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020708
  { CompInvalidUnitRefSyntax, 
    "The 'comp:unitRef' attribute must have the syntax of an SBML SId",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:unitRef' attribute on an SBaseRef object must "
    "always conform to the syntax of the SBML data type SId.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020709
  { CompInvalidMetaIdRefSyntax, 
    "The 'comp:metaIdRef' attribute must have the syntax of an XML ID",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:metaIdRef' attribute on an SBaseRef object must "
    "always conform to the syntax of the XML data type ID.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020710
  { CompOneSBaseRefOnly, 
    "Only one <sbaseRef>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotation subobjects permitted on "
    "all SBML objects, an SBaseRef object may only contain a single "
    "<sBaseRef> child.",
    { "L3V1 Comp V1 Section 3.7.2"
    }
  },

  // 1020711
  { CompDeprecatedSBaseRefSpelling, 
    "The spelling 'sbaseRef' is deprecated",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The 'sbaseRef' spelling of an SBaseRef child of an SBaseRef object "
    "is considered deprecated, and 'sBaseRef' should be used instead.",
    { "L3V1 Comp V1 Section 3.7.2"
    }
  },

  // 1020712
  { CompSBaseRefMustReferenceObject, 
    "An SBaseRef must reference an object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <sBaseRef> object must point to another object; that is, a "
    "<sBaseRef> object must always have a value for one of the attributes "
    "'comp:portRef', 'comp:idRef', 'comp:unitRef', or 'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020713
  { CompSBaseRefMustReferenceOnlyOneObject, 
    "An SBaseRef must reference only one other object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An <sBaseRef> object can only point to one other object; that is, "
    "a given <sBaseRef> object can only have a value for one of the "
    "attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', or "
    "'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020714
  { CompNoMultipleReferences, 
    "Objects may not be referenced by mutiple SBaseRef constructs.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Any one SBML object may only be referenced in one of the following ways:  "
    "referenced by a single <port> object; referenced by a single <deletion> "
    "object; referenced by a single <replacedElement>; be the parent of a "
    "single <replacedBy> child; be referenced by one or more <replacedBy> "
    "objects; or be referenced by one or more <replacedElement> objects all "
    "using the 'deletion' attribute.  Essentially, once an object has been "
    "referenced in one of these ways it cannot be referenced again.",
    { "L3V1 Comp V1 Section 3.7.1"
    }
  },

  // 1020801
  { CompPortMustReferenceObject, 
    "Port must reference an object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <port> object must point to another object; that "
    "is, a <port> object must always have a value for one of the attributes "
    "'comp:idRef', 'comp:unitRef', or 'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.4.3"
    }
  },

  // 1020802
  { CompPortMustReferenceOnlyOneObject, 
    "Port must reference only one other object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <port> object can only point to one other "
    "object; that is, a given <port> object can only have a value for one of "
    "the attributes 'comp:idRef', 'comp:unitRef', or 'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.4.3"
    }
  },

  // 1020803
  { CompPortAllowedAttributes, 
    "Allowed attributes on a Port",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <port> object must have a value for the required "
    "attribute 'comp:id', and one, and only one, of the "
    "attributes 'comp:idRef', 'comp:unitRef', or "
    "'comp:metaIdRef'.  No other attributes from the Hierarchical "
    "Model Composition namespace are permitted on a <port> object.",
    { "L3V1 Comp V1 Section 3.4.3"
    }
  },

  // 1020804
  { CompPortReferencesUnique, 
    "Port definitions must be unique.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Port definitions must be unique; that is, no two "
    "<port> objects in a given Model may reference the same object in that "
    "Model.",
    { "L3V1 Comp V1 Section 3.4.3"
    }
  },

  // 1020901
  { CompDeletionMustReferenceObject, 
    "Deletion must reference an object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <deletion> object must point to another object; that "
    "is, a <deletion> object must always have a value for one of the attributes "
    "'comp:portRef', 'comp:idRef', 'comp:unitRef', or 'comp:metaIdRef'",
    { "L3V1 Comp V1 Section 3.5.3"
    }
  },

  // 1020902
  { CompDeletionMustReferOnlyOneObject, 
    "Deletion must reference only one other object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <deletion> object can only point to one other "
    "object; that is, a given <deletion> object can only have a value for one of "
    "the attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', or "
    "'comp:metaIdRef'",
    { "L3V1 Comp V1 Section 3.5.3"
    }
  },

  // 1020903
  { CompDeletionAllowedAttributes, 
    "Allowed attributes on a Deletion",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <deletion> object must have a value for one, and only one, of the "
    "attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', and "
    "'comp:metaIdRef'.  It may also have the optional attributes 'comp:id' "
    "and 'comp:name'.  No other attributes from the Hierarchical "
    "Model Composition namespace are permitted on a <deletion> object.",
    { "L3V1 Comp V1 Section 3.5.3"
    }
  },

  // 1021001
  { CompReplacedElementMustRefObject, 
    "ReplacedElement must reference an object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedElement> object must point to another object; that "
    "is, a <replacedElement> object must always have a value for one of the "
    "attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', "
    "'comp:metaIdRef' or 'comp:deletion'.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021002
  { CompReplacedElementMustRefOnlyOne, 
    "ReplacedElement must reference only one other object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedElement> object can only point to one other "
    "object; that is, a given <replacedElement> object can only have a value "
    "for one of the attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', "
    "'comp:metaIdRef' or 'comp:deletion'.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021003
  { CompReplacedElementAllowedAttributes, 
    "Allowed attributes on <replacedElement>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedElement> object must have a value for the required attribute "
    "'comp:submodelRef', and a value for one, and only one, of the following "
    "attributes: 'comp:portRef', 'comp:idRef', 'comp:unitRef', "
    "'comp:metaIdRef', or 'comp:deletion'. It may also have a value for the "
    "optional attribute 'comp:conversionFactor'. No other attributes from "
    "the HierarchicalModel Composition namespace are permitted on a "
    "<replacedElement> object.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021004
  { CompReplacedElementSubModelRef, 
    "The 'comp:submodelRef' attribute must point to a <submodel>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:submodelRef' attribute on a <replacedElement> "
    "object must be the identifier of a <submodel> present in the "
    "<replacedElement> object's parent Model.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021005
  { CompReplacedElementDeletionRef, 
    "The 'comp:deletion' attribute must point to a <deletion>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:deletion' attribute on a <replacedElement> "
    "object must be the identifier of a <deletion> present in the "
    "<replacedElement> object's parent Model.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021006
  { CompReplacedElementConvFactorRef, 
    "The 'comp:conversionFactor attribute must point to a <parameter>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:conversionFactor' attribute on a "
    "<replacedElement> object must be the identifier of a <parameter> "
    "present in the <replacedElement> object's parent Model",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021010
  { CompReplacedElementSameReference, 
    "No <replacedElement> refer to same object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "No two <replacedElement> objects in the same Model may reference the same "
    "object unless that object is a <deletion>",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021011
  { CompReplacedElementNoDelAndConvFact, 
    "No <replacedElement> with deletion and conversionfactor",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If a <replacedElement> uses the 'comp:deletion' attribute, then it "
    "should not also use the 'comp:conversionFactor' attribute.",
    { "L3V1 Comp V1 Section 3.6.2"
    }
  },

  // 1021101
  { CompReplacedByMustRefObject, 
    "ReplacedBy must reference an object",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedBy> object must point to another object; that "
    "is, a <replacedBy> object must always have a value for one of the "
    "attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef' or "
    "'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.6.4"
    }
  },

  // 1021102
  { CompReplacedByMustRefOnlyOne, 
    "ReplacedBy must reference only one other object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedBy> object can only point to one other "
    "object; that is, a given <replacedBy> object can only have a value "
    "for one of the attributes 'comp:portRef', 'comp:idRef', 'comp:unitRef', "
    "or 'comp:metaIdRef'.",
    { "L3V1 Comp V1 Section 3.6.4"
    }
  },

  // 1021103
  { CompReplacedByAllowedAttributes, 
    "Allowed attributes on <replacedBy>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "A <replacedBy> object must have a value for the required attribute "
    "'comp:submodelRef', and a value for one, and only one, of the following "
    "attributes: 'comp:portRef', 'comp:idRef', 'comp:unitRef' "
    "or 'comp:metaIdRef'. No other attributes from "
    "the HierarchicalModel Composition namespace are permitted on a "
    "<replacedElement> object.",
    { "L3V1 Comp V1 Section 3.6.4"
    }
  },

  // 1021104
  { CompReplacedBySubModelRef, 
    "The 'comp:submodelRef' attribute must point to a <submodel>",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The value of a 'comp:submodelRef' attribute on a <replacedBy> "
    "object must be the identifier of a <submodel> present in the "
    "<replacedBy> object's parent Model.",
    { "L3V1 Comp V1 Section 3.6.4"
    }
  },

  // 1021201
  { CompMustReplaceSameClass, 
    "Replaced classes must match.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If one element replaces another, whether it is the target of a "
    "<replacedBy> element, or whether it has a child <replacedElement>, "
    "the SBML class of the replacement element must match the SBML class "
    "of the replaced element, with two exceptions: an element of a derived "
    "class may replace an object of its base class (for base classes other "
    "than SBase), and any SBML class with mathematical meaning may "
    "replace a <parameter>. A base class may not replace a derived class, "
    "however, nor may a <parameter> replace some other SBML element with "
    "mathematical meaning.",
    { "L3V1 Comp V1 Section 3.6.5"
    }
  },

  // 1021202
  { CompMustReplaceIDs, 
    "Replaced IDs must be replaced with IDs.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If one element replaces another, whether it is the target of a "
    "<replacedBy> element, or whether it has a child <replacedElement>, "
    "if the replaced element has the 'id' attribute set, the replacement !"
    "element must also have the 'id' attribute set.",
    { "L3V1 Comp V1 Section 3.6.5"
    }
  },

  // 1021203
  { CompMustReplaceMetaIDs, 
    "Replaced metaids must be replaced with metaids.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If one element replaces another, whether it is the target of a "
    "<replacedBy> element, or whether it has a child <replacedElement>, "
    "if the replaced element has the 'metaid' attribute set, the replacement "
    "element must also have the 'metaid' attribute set.",
    { "L3V1 Comp V1 Section 3.6.5"
    }
  },

  // 1021204
  { CompMustReplacePackageIDs, 
    "Replaced package IDs must be replaced with package IDs.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "If one element replaces another, whether it is the target of a "
    "<replacedBy> element, or whether it has a child <replacedElement>, "
    "if the replaced element has an identifier attribute from some other "
    "SBML package set, the replacement element must also have that same "
    "identifier attribute set.",
    { "L3V1 Comp V1 Section 3.6.5"
    }
  },

  // 1090101
  { CompUnresolvedReference, 
    "Unresolved reference.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The external model referenced in this model could not be "
    "resolved. ",
    { ""
    }
  },

  // 1090102
  { CompNoModelInReference, 
    "No model in referenced document.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The external document referenced in this model did not contain any models.",
    { ""
    }
  },

  // 1090103
  { CompExtModDefBad, 
    "Referenced <externalModelDefinition> unresolvable.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "An external model definition referenced by an external model "
    "definition in this model was itself unresolvable.",
    { ""
    }
  },

  // 1090104
  { CompModelFlatteningFailed,
    "Model failed to flatten.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "Errors arose during the attempt to flatten the model.",
    { ""
    }
  },

  // 1090105
  { CompFlatModelNotValid,
    "Flat model not valid.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The interpretation the Hierarchical Model Composition constructs to "
    "produce a kind of 'flattened' version of the model devoid of the comp "
    "package constructs must produce a valid SBML Level 3 model.",
    { "L3V1 Comp V1 Appendix A1"
    }
  },

    // 1090106
  { CompLineNumbersUnreliable,
    "Line numbers unreliable.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "Due to the need to instantiate models, modelDefinitions, submodels etc. "
    "for the purposes of validation it is problematic to reliably report "
    "line numbers when performing validation on models using the "
    "Hierarchical Model Composition package." ,
    { ""
    }
  },

    // 1090107
  { CompFlatteningNotRecognisedReqd,
    "Flattening not implemented for required package.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The CompFlatteningConverter has encountered a required package for "
    "which libSBML does not recognize the information.",
    { ""
    }
  },

    // 1090108
  { CompFlatteningNotRecognisedNotReqd,
    "Flattening not implemented for unrequired package.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The CompFlatteningConverter has encountered an unrequired package for "
    "which libSBML does not recognize the information.",
    { ""
    }
  },

    // 1090109
  { CompFlatteningNotImplementedNotReqd,
    "Flattening not implemented for unrequired package.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The CompFlatteningConverter has encountered an unrequired package for "
    "which "
    "the necessary routines to allow flattening have not yet been implemented. "
    "" ,
    { ""
    }
  },

    // 1090110
  { CompFlatteningNotImplementedReqd,
    "Flattening not implemented for required package.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The CompFlatteningConverter has encountered a required package for which "
    "the necessary routines to allow flattening have not yet been implemented. "
    "" ,
    { ""
    }
  },

  // 1090111
  { CompFlatteningWarning,
    "Flattening reference may come from package.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The CompFlatteningConverter has encountered an unknown reference which may "
    "be due to the presence of an unknown package."
    "" ,
    { ""
    }
  },

  // 1090112
  { CompDeprecatedDeleteFunction,
    "The performDeletions functions is deprecated.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The software used to process this hierarchical model used the deprecated "
    "function performDeletions to do so.  Unfortunately, it is "
    "impossible to properly use this function "
    "as it was originally designed, "
    "without some models either causing the program to crash, or causing them "
    "to be interpreted incorrectly.  Instead, the software should use "
    "collectDeletionsAndDeleteCompConstructs, in conjunction with "
    "collectRenameAndConvertReplacements and removeCollectedElements "
    "to properly process hierarchical models."
    "" ,
    { ""
    }
  },

  // 1090113
  { CompDeprecatedReplaceFunction,
    "The performReplacementsAndConversions fuctions is deprecated.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The software used to process this hierarchical model used the deprecated "
    "function performReplacementsAndConversions to do so.  Unfortunately, it is "
    "impossible to properly use this function "
    "as it was originally designed, "
    "without some models either causing the program to crash, or causing them "
    "to be interpreted incorrectly.  Instead, the software should use "
    "collectDeletionsAndDeleteCompConstructs, in conjunction with "
    "collectRenameAndConvertReplacements and removeCollectedElements "
    "to properly process hierarchical models."
    "" ,
    { ""
    }
  },

  // 1090114
  { CompDeletedReplacement,
    "Element deleted before a subelement could be replaced.",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_ERROR,
    "The model contained a deletion whose subelement was replaced.  This "
    "is perfectly legal, but unfortunately, the subroutine used to implement "
    "this actually removed the deleted element and all of its children before "
    "replacing the child, making it impossible to discover any IDs that need to "
    "be replaced."
    "" ,
    { ""
    }
  },

  // 1090115
  { CompIdRefMayReferenceUnknownPackage,
    "The 'comp:idRef' attribute must be the 'id' of a model element",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The value of a 'comp:idRef' attribute on an <sBaseRef> object must "
    "be the identifier of an object contained in (that is, within the "
    "SId namespace of) the <model> referenced by that <sBaseRef>. This "
    "includes objects with 'id' attributes defined in packages other "
    "than SBML Level 3 Core or the Hierarchical Model Composition "
    "package.",
    { ""
    }
  },

  // 1090116
  { CompMetaIdRefMayReferenceUnknownPkg,
    "The 'comp:metaIdRef' attribute must be the 'metaid' of a model element",
    LIBSBML_CAT_GENERAL_CONSISTENCY, 
    LIBSBML_SEV_WARNING,
    "The value of a 'comp:metaIdRef' attribute on an <sBaseRef> object "
    "must be the value of a 'comp:metaid' attribute on an element contained "
    "in the <model> referenced by that <sBaseRef>.",
    { ""
    }
  }



};

LIBSBML_CPP_NAMESPACE_END

#endif
/** @endcond */
