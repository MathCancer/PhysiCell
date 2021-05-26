/**
 * @file    CompSBMLError.h
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
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */

#ifndef CompSBMLError_h
#define CompSBMLError_h


LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * @enum CompSBMLErrorCode_t
 * Codes for all SBML-level errors and warnings from the 'comp' package.
 *
 * These are distinguished from other SBML error codes 
 * by having a number between 1000000 and 1099999.  
 * 
 * @copydetails doc_sbml_error_code_ranges
 */
typedef enum
{
  CompUnknown                           = 1010100 /*!< Unknown error from comp */
, CompNSUndeclared                      = 1010101 /*!< The comp ns is not correctly declared */
, CompElementNotInNs                    = 1010102 /*!< Element not in comp namespace */

, CompDuplicateComponentId              = 1010301 /*!< Duplicate 'id' attribute value */
, CompUniqueModelIds                    = 1010302 /*!< Model and ExternalModelDefinitions must have unique ids */
, CompUniquePortIds                     = 1010303 /*!< Ports must have unique ids */
, CompInvalidSIdSyntax                  = 1010304 /*!< Invalid SId syntax */
, CompInvalidSubmodelRefSyntax          = 1010308 /*!< Invalid submodelRef syntax */
, CompInvalidDeletionSyntax             = 1010309 /*!< Invalid deletion syntax */
, CompInvalidConversionFactorSyntax     = 1010310 /*!< Invalid conversionFactor syntax */
, CompInvalidNameSyntax                 = 1010311 /*!< Invalid name syntax */

, CompReplacedUnitsShouldMatch          = 1010501 /*!< Units of replaced elements should match replacement units. */

, CompOneListOfReplacedElements         = 1020101 /*!< Only one <code>&lt;listOfReplacedElements&gt;</code> allowed. */
, CompLOReplaceElementsAllowedElements  = 1020102 /*!< Allowed children of <code>&lt;listOfReplacedElements&gt;</code> */
, CompLOReplacedElementsAllowedAttribs  = 1020103 /*!< Allowed <code>&lt;listOfReplacedElements&gt;</code> attributes */
, CompEmptyLOReplacedElements           = 1020104 /*!< <code>&lt;listOfReplacedElements&gt;</code> must not be empty */
, CompOneReplacedByElement              = 1020105 /*!< Only one <code>&lt;replacedBy&gt;</code> object allowed. */

, CompAttributeRequiredMissing          = 1020201 /*!< Required comp:required attribute on <code>&lt;sbml&gt;</code> */
, CompAttributeRequiredMustBeBoolean    = 1020202 /*!< The comp:required attribute must be Boolean */
, CompRequiredTrueIfElementsRemain      = 1020203 /*!< The comp:required attribute must be 'true' if math changes. NOTE:  Deprecated */
, CompRequiredFalseIfAllElementsReplaced= 1020204 /*!< The comp:required attribute must be 'false' if math does not change. NOTE:  Deprecated */
, CompOneListOfModelDefinitions         = 1020205 /*!< Only one <code>&lt;listOfModelDefinitions&gt;</code> allowed. */
, CompEmptyLOModelDefs                  = 1020206 /*!< <code>&lt;listOfModelDefinitions&gt;</code> and <code>&lt;listOfExternalModelDefinitions&gt;</code> must not be empty */
, CompLOModelDefsAllowedElements        = 1020207 /*!< Only <code>&lt;modelDefinitions&gt;</code> in <code>&lt;listOfModelDefinitions&gt;</code> */
, CompLOExtModelDefsAllowedElements     = 1020208 /*!< Only <code>&lt;externalModelDefinitions&gt;</code> in <code>&lt;listOfExternalModelDefinitions&gt;</code> */
, CompLOModelDefsAllowedAttributes      = 1020209 /*!< Allowed <code>&lt;listOfModelDefinitions&gt;</code> attributes */
, CompLOExtModDefsAllowedAttributes     = 1020210 /*!< Allowed <code>&lt;listOfExternalModelDefinitions&gt;</code> attributes */
, CompOneListOfExtModelDefinitions      = 1020211 /*!< Only one <code>&lt;listOfExternalModelDefinitions&gt;</code> allowed. */
, CompAttributeRequiredMustBeTrue       = 1020212 /*!< The comp:required attribute must be 'true' */

, CompExtModDefAllowedCoreAttributes    = 1020301 /*!< Allowed <code>&lt;externalModelDefinitions&gt;</code> core attributes */
, CompExtModDefAllowedElements          = 1020302 /*!< Allowed <code>&lt;externalModelDefinitions&gt;</code> elements */
, CompExtModDefAllowedAttributes        = 1020303 /*!< Allowed <code>&lt;externalModelDefinitions&gt;</code> attributes */
, CompReferenceMustBeL3                 = 1020304 /*!< External models must be L3 */
, CompModReferenceMustIdOfModel         = 1020305 /*!< 'modelRef' must be the 'id' of a model in the 'source' document */
, CompExtModMd5DoesNotMatch             = 1020306 /*!< MD5 checksum does not match the 'source' document */
, CompInvalidSourceSyntax               = 1020307 /*!< The 'comp:source' attribute must be of type 'anyURI' */
, CompInvalidModelRefSyntax             = 1020308 /*!< The 'comp:modelRef' attribute must have the syntax of 'SId' */
, CompInvalidMD5Syntax                  = 1020309 /*!< The 'comp:md5' attribute must have the syntax of 'string' */
, CompCircularExternalModelReference    = 1020310 /*!< Circular reference in <code>&lt;externalModelDefinition&gt;</code> */

, CompOneListOfOnModel                  = 1020501 /*!< Only one <code>&lt;listOfSubmodels&gt;</code> and one <code>&lt;listOfPorts&gt;</code> allowed */
, CompNoEmptyListOfOnModel              = 1020502 /*!< No empty listOf elements allowed */
, CompLOSubmodelsAllowedElements        = 1020503 /*!< Allowed elements on <code>&lt;listOfSubmodels&gt;</code> */
, CompLOPortsAllowedElements            = 1020504 /*!< Allowed elements on <code>&lt;listOfPorts&gt;</code> */
, CompLOSubmodelsAllowedAttributes      = 1020505 /*!< Allowed attributes on <code>&lt;listOfSubmodels&gt;</code> */
, CompLOPortsAllowedAttributes          = 1020506 /*!< Allowed attributes on <code>&lt;listOfPorts&gt;</code> */

, CompSubmodelAllowedCoreAttributes     = 1020601 /*!< Allowed core attributes on <code>&lt;submodel&gt;</code> */
, CompSubmodelAllowedElements           = 1020602 /*!< Allowed elements on <code>&lt;submodel&gt;</code> */
, CompOneListOfDeletionOnSubmodel       = 1020603 /*!< Only one <code>&lt;listOfDeletions&gt;</code> on a <code>&lt;submodel&gt;</code> allowed */
, CompSubmodelNoEmptyLODeletions        = 1020604 /*!< No empty listOfDeletions elements allowed */
, CompLODeletionsAllowedElements        = 1020605 /*!< Allowed elements on <code>&lt;listOfDeletions&gt;</code> */
, CompLODeletionAllowedAttributes       = 1020606 /*!< Allowed <code>&lt;listOfDeletions&gt;</code> attributes */
, CompSubmodelAllowedAttributes         = 1020607 /*!< Allowed <code>&lt;submodel&gt;</code> attributes */
, CompModReferenceSyntax                = 1020608 /*!< 'comp:modelRef' must conform to SId syntax */
, CompInvalidTimeConvFactorSyntax       = 1020613 /*!< 'comp:timeConversionFactor' must conform to SId syntax */
, CompInvalidExtentConvFactorSyntax     = 1020614 /*!< 'comp:extentConversionFactor' must conform to SId syntax */
, CompSubmodelMustReferenceModel        = 1020615 /*!< The 'comp:modelRef' attribute must reference a model */
, CompSubmodelCannotReferenceSelf       = 1020616 /*!< The 'comp:modelRef' attribute cannot reference own model */
, CompModCannotCircularlyReferenceSelf  = 1020617 /*!< <code>&lt;model&gt;</code> may not reference <code>&lt;submodel&gt;</code> that references itself. */
, CompTimeConversionMustBeParameter     = 1020622 /*!< The 'comp:timeConversionFactor' must reference a parameter */
, CompExtentConversionMustBeParameter   = 1020623 /*!< The 'comp:extentConversionFactor' must reference a parameter */

, CompPortRefMustReferencePort          = 1020701 /*!< The 'comp:portRef' attribute must be the 'id' of a <code>&lt;port&gt;</code> */
, CompIdRefMustReferenceObject          = 1020702 /*!< The 'comp:idRef' attribute must be the 'id' of a model element */
, CompUnitRefMustReferenceUnitDef       = 1020703 /*!< The 'comp:unitRef' attribute must be the 'id' of a UnitDefinition */
, CompMetaIdRefMustReferenceObject      = 1020704 /*!< The 'comp:metaIdRef' attribute must be the 'metaid' of an object */
, CompParentOfSBRefChildMustBeSubmodel  = 1020705 /*!< If <code>&lt;sBaseRef&gt;</code> has a child <code>&lt;sBaseRef&gt;</code> its parent must be a <code>&lt;submodel&gt;</code> */
, CompInvalidPortRefSyntax              = 1020706 /*!< The 'comp:portRef' attribute must have the syntax of an SBML SId */
, CompInvalidIdRefSyntax                = 1020707 /*!< The 'comp:idRef' attribute must have the syntax of an SBML SId */
, CompInvalidUnitRefSyntax              = 1020708 /*!< The 'comp:unitRef' attribute must have the syntax of an SBML SId */
, CompInvalidMetaIdRefSyntax            = 1020709 /*!< The 'comp:metaIdRef' attribute must have the syntax of an XML ID */
, CompOneSBaseRefOnly                   = 1020710 /*!< Only one <code>&lt;sbaseRef&gt;</code> */
, CompDeprecatedSBaseRefSpelling        = 1020711 /*!< The spelling 'sbaseRef' is deprecated */
, CompSBaseRefMustReferenceObject       = 1020712 /*!< An SBaseRef must reference an object. */
, CompSBaseRefMustReferenceOnlyOneObject= 1020713 /*!< An SBaseRef must reference only one other object. */
, CompNoMultipleReferences              = 1020714 /*!< Objects may not be referenced by mutiple SBaseRef constructs. */

, CompPortMustReferenceObject           = 1020801 /*!< Port must reference an object */
, CompPortMustReferenceOnlyOneObject    = 1020802 /*!< Port must reference only one other object. */
, CompPortAllowedAttributes             = 1020803 /*!< Allowed attributes on a Port */
, CompPortReferencesUnique              = 1020804 /*!< Port definitions must be unique. */

, CompDeletionMustReferenceObject       = 1020901 /*!< Deletion must reference an object */
, CompDeletionMustReferOnlyOneObject    = 1020902 /*!< Deletion must reference only one other object. */
, CompDeletionAllowedAttributes         = 1020903 /*!< Allowed attributes on a Deletion */

, CompReplacedElementMustRefObject      = 1021001 /*!< ReplacedElement must reference an object */
, CompReplacedElementMustRefOnlyOne     = 1021002 /*!< ReplacedElement must reference only one other object. */
, CompReplacedElementAllowedAttributes  = 1021003 /*!< Allowed attributes on <code>&lt;replacedElement&gt;</code> */
, CompReplacedElementSubModelRef        = 1021004 /*!< The 'comp:submodelRef' attribute must point to a <code>&lt;submodel&gt;</code> */
, CompReplacedElementDeletionRef        = 1021005 /*!< The 'comp:deletion' attribute must point to a <code>&lt;deletion&gt;</code> */
, CompReplacedElementConvFactorRef      = 1021006 /*!< The 'comp:conversionFactor attribute must point to a <code>&lt;parameter&gt;</code> */
, CompReplacedElementSameReference      = 1021010 /*!< No <code>&lt;replacedElement&gt;</code> refer to same object */
, CompReplacedElementNoDelAndConvFact   = 1021011 /*!< No <code>&lt;replacedElement&gt;</code> with deletion and conversionfactor */

, CompReplacedByMustRefObject           = 1021101 /*!< ReplacedBy must reference an object */
, CompReplacedByMustRefOnlyOne          = 1021102 /*!< ReplacedBy must reference only one other object. */
, CompReplacedByAllowedAttributes       = 1021103 /*!< Allowed attributes on <code>&lt;replacedBy&gt;</code> */
, CompReplacedBySubModelRef             = 1021104 /*!< The 'comp:submodelRef' attribute must point to a <code>&lt;submodel&gt;</code> */

, CompMustReplaceSameClass              = 1021201 /*!< Replaced classes must match. */
, CompMustReplaceIDs                    = 1021202 /*!< Replaced IDs must be replaced with IDs. */
, CompMustReplaceMetaIDs                = 1021203 /*!< Replaced metaids must be replaced with metaids. */
, CompMustReplacePackageIDs             = 1021204 /*!< Replaced package IDs must be replaced with package IDs. */

, CompUnresolvedReference               = 1090101 /*!< Unresolved reference. */
, CompNoModelInReference                = 1090102 /*!< No model in referenced document. */
, CompExtModDefBad                      = 1090103 /*!< Referenced <code>&lt;externalModelDefinition&gt;</code> unresolvable. */
, CompModelFlatteningFailed             = 1090104 /*!< Model failed to flatten. */
, CompFlatModelNotValid                 = 1090105 /*!< Flat model not valid. */
, CompLineNumbersUnreliable             = 1090106 /*!< Line numbers unreliable. */

, CompFlatteningNotRecognisedReqd       = 1090107 /*!< Flattening not implemented for required package. */
, CompFlatteningNotRecognisedNotReqd    = 1090108 /*!< Flattening not implemented for unrequired package. */
, CompFlatteningNotImplementedNotReqd   = 1090109 /*!< Flattening not implemented for unrequired package. */
, CompFlatteningNotImplementedReqd      = 1090110 /*!< Flattening not implemented for required package. */
, CompFlatteningWarning                 = 1090111 /*!< Flattening reference may come from package. */
, CompDeprecatedDeleteFunction          = 1090112 /*!< The performDeletions functions is deprecated. */
, CompDeprecatedReplaceFunction         = 1090113 /*!< The performReplacementsAndConversions fuctions is deprecated. */
, CompDeletedReplacement                = 1090114 /*!< Element deleted before a subelement could be replaced. */

, CompIdRefMayReferenceUnknownPackage   = 1090115 /*!< The 'comp:idRef' attribute must be the 'id' of a model element */
, CompMetaIdRefMayReferenceUnknownPkg   = 1090116 /*!< The 'comp:metaIdRef' attribute must be the 'metaid' of a model element */

} CompSBMLErrorCode_t;


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END



#endif /* SBMLError_h */
