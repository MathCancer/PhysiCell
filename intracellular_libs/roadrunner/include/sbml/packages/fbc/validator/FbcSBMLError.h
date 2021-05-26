/**
 * @file    FbcSBMLError.h
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
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */

#ifndef FbcSBMLError_h
#define FbcSBMLError_h

#include <sbml/common/extern.h>
#include <sbml/SBMLError.h>
#include <sbml/SBMLNamespaces.h>


LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * @enum FbcSBMLErrorCode_t
 * Codes for all SBML-level errors and warnings from the 'fbc' package.
 *
 * These are distinguished from other SBML error codes 
 * by having a number between 2000000 and 2099999.  
 * 
 * @copydetails doc_sbml_error_code_ranges
 */
typedef enum
{
  FbcUnknown                           = 2010100 /*!< Unknown error from fbc */
, FbcNSUndeclared                      = 2010101 /*!< The fbc ns is not correctly declared */
, FbcElementNotInNs                    = 2010102 /*!< Element not in fbc namespace */
, FbcDuplicateComponentId              = 2010301 /*!< Duplicate 'id' attribute value */
, FbcSBMLSIdSyntax                     = 2010302 /*!< Invalid 'id' attribute */

, FbcAttributeRequiredMissing          = 2020101 /*!< Required fbc:required attribute on <code>&lt;sbml&gt;</code> */
, FbcAttributeRequiredMustBeBoolean    = 2020102 /*!< The fbc:required attribute must be Boolean */
, FbcRequiredFalse                     = 2020103 /*!< The fbc:required attribute must be 'false' */

, FbcOnlyOneEachListOf                 = 2020201 /*!< One of each list of allowed */
, FbcNoEmptyListOfs                    = 2020202 /*!< ListOf elements cannot be empty */
, FbcLOFluxBoundsAllowedElements       = 2020203 /*!< Allowed elements on ListOfFluxBounds */
, FbcLOObjectivesAllowedElements       = 2020204 /*!< Allowed elements on ListOfObjectives */
, FbcLOFluxBoundsAllowedAttributes     = 2020205 /*!< Allowed attributes on ListOfFluxBounds */
, FbcLOObjectivesAllowedAttributes     = 2020206 /*!< Allowed attributes on ListOfObjectives */
, FbcActiveObjectiveSyntax             = 2020207 /*!< Type of activeObjective attribute */
, FbcActiveObjectiveRefersObjective    = 2020208 /*!< ActiveObjective must reference Objective */
, FbcModelMustHaveStrict               = 2020209 /*!< 'Strict' attribute required on <code>&lt;model&gt;</code> */
, FbcModelStrictMustBeBoolean          = 2020210 /*!< 'Strict' attribute must be boolean */
, FbcLOGeneProductsAllowedElements     = 2020211 /*!< Allowed elements on ListOfGeneProducts */
, FbcLOGeneProductsAllowedAttributes   = 2020212 /*!< Allowed attributes on ListOfGeneProducts */

, FbcSpeciesAllowedL3Attributes        = 2020301 /*!< Species allowed attributes */
, FbcSpeciesChargeMustBeInteger        = 2020302 /*!< Charge must be integer */
, FbcSpeciesFormulaMustBeString        = 2020303 /*!< Chemical formula must be string */

, FbcFluxBoundAllowedL3Attributes      = 2020401 /*!< <code>&lt;fluxBound&gt;</code> may only have 'metaId' and 'sboTerm' from L3 namespace */
, FbcFluxBoundAllowedElements          = 2020402 /*!< <code>&lt;fluxBound&gt;</code> may only have <code>&lt;notes&gt;</code> and <code>&lt;annotations&gt;</code> from L3 Core */
, FbcFluxBoundRequiredAttributes       = 2020403 /*!< Invalid attribute found on <code>&lt;fluxBound&gt;</code> object */
, FbcFluxBoundRectionMustBeSIdRef      = 2020404 /*!< Datatype for 'fbc:reaction' must be SIdRef */
, FbcFluxBoundNameMustBeString         = 2020405 /*!< The attribute 'fbc:name' must be of the data type string */
, FbcFluxBoundOperationMustBeEnum      = 2020406 /*!< The attribute 'fbc:operation' must be of data type FbcOperation */
, FbcFluxBoundValueMustBeDouble        = 2020407 /*!< The attribute 'fbc:value' must be of the data type double */
, FbcFluxBoundReactionMustExist        = 2020408 /*!< 'fbc:reaction' must refer to valid reaction */
, FbcFluxBoundsForReactionConflict     = 2020409 /*!< Conflicting set of FluxBounds for a reaction */

, FbcObjectiveAllowedL3Attributes      = 2020501 /*!< <code>&lt;objective&gt;</code> may only have 'metaId' and 'sboTerm' from L3 namespace */
, FbcObjectiveAllowedElements          = 2020502 /*!< <code>&lt;objective&gt;</code> may only have <code>&lt;notes&gt;</code> and <code>&lt;annotations&gt;</code> from L3 Core */
, FbcObjectiveRequiredAttributes       = 2020503 /*!< Invalid attribute found on <code>&lt;objective&gt;</code> object */
, FbcObjectiveNameMustBeString         = 2020504 /*!< The attribute 'fbc:name' must be of the data type string */
, FbcObjectiveTypeMustBeEnum           = 2020505 /*!< The attribute 'fbc:type' must be of data type FbcType. */
, FbcObjectiveOneListOfObjectives      = 2020506 /*!< An <code>&lt;objective&gt;</code> must have one <code>&lt;listOfFluxObjectives&gt;</code>. */
, FbcObjectiveLOFluxObjMustNotBeEmpty  = 2020507 /*!< <code>&lt;listOfFluxObjectives&gt;</code> subobject must not be empty */
, FbcObjectiveLOFluxObjOnlyFluxObj     = 2020508 /*!< Invalid element found in <code>&lt;listOfFluxObjectives&gt;</code> */
, FbcObjectiveLOFluxObjAllowedAttribs  = 2020509 /*!< <code>&lt;listOfFluxObjectives&gt;</code> may only have 'metaId' and 'sboTerm' from L3 core */

, FbcFluxObjectAllowedL3Attributes     = 2020601 /*!< <code>&lt;fluxObjective&gt;</code> may only have 'metaId' and 'sboTerm' from L3 namespace */
, FbcFluxObjectAllowedElements         = 2020602 /*!< <code>&lt;fluxObjective&gt;</code> may only have <code>&lt;notes&gt;</code> and <code>&lt;annotations&gt;</code> from L3 Core */
, FbcFluxObjectRequiredAttributes      = 2020603 /*!< Invalid attribute found on <code>&lt;fluxObjective&gt;</code> object */
, FbcFluxObjectNameMustBeString        = 2020604 /*!< The attribute 'fbc:name' must be of the data type string */
, FbcFluxObjectReactionMustBeSIdRef    = 2020605 /*!< Datatype for 'fbc:reaction' must be SIdRef */
, FbcFluxObjectReactionMustExist       = 2020606 /*!< 'fbc:reaction' must refer to valid reaction */
, FbcFluxObjectCoefficientMustBeDouble = 2020607 /*!< The attribute 'fbc:coefficient' must be of the data type double */
, FbcFluxObjectCoefficientWhenStrict   = 2020608 /*!< The 'fbc:coefficient' must be declared when strict */

, FbcReactionOnlyOneGeneProdAss         = 2020701 /*!< One GeneProductAssociation in a Reaction */
, FbcReactionAllowedAttributes          = 2020702 /*!< Fbc attributes on a Reaction */
, FbcReactionLwrBoundSIdRef             = 2020703 /*!< LowerBound must be SIdRef */
, FbcReactionUpBoundSIdRef              = 2020704 /*!< UpperBound must be SIdRef */
, FbcReactionLwrBoundRefExists          = 2020705 /*!< LowerBound must point to Parameter */
, FbcReactionUpBoundRefExists           = 2020706 /*!< UpperBound must point to Parameter */
, FbcReactionMustHaveBoundsStrict       = 2020707 /*!< Reaction must have bounds when strict */
, FbcReactionConstantBoundsStrict       = 2020708 /*!< Reaction bounds constant when strict */
, FbcReactionBoundsMustHaveValuesStrict = 2020709 /*!< Reaction bounds must have values when strict */
, FbcReactionBoundsNotAssignedStrict    = 2020710 /*!< Reaction bounds not assigned when strict */
, FbcReactionLwrBoundNotInfStrict       = 2020711 /*!< LowerBound cannot be INF when strict */
, FbcReactionUpBoundNotNegInfStrict     = 2020712 /*!< UpperBound cannot be -INF when strict */
, FbcReactionLwrLessThanUpStrict        = 2020713 /*!< LowerBound less than upperBound when strict */
, FbcSpeciesReferenceConstantStrict     = 2020714 /*!< SpeciesReferences must be constant when strict */
, FbcSpeciesRefsStoichMustBeRealStrict  = 2020715 /*!< Stoichiometry of SpeciesReferences must be real valued when strict */
, FbcSpeciesRefNotAssignedStrict        = 2020716 /*!< SpeciesReference not target of InitialAssignment when strict" */

, FbcGeneProdAssocAllowedCoreAttribs    = 2020801 /*!< Allowed core attributes on <code>&lt;GeneProductAssociation&gt;</code> */
, FbcGeneProdAssocAllowedCoreElements   = 2020802 /*!< Allowed core elements on <code>&lt;GeneProductAssociation&gt;</code> */
, FbcGeneProdAssocAllowedAttribs        = 2020803 /*!< Allowed fbc attributes on <code>&lt;GeneProductAssociation&gt;</code> */
, FbcGeneProdAssocIdSyntax              = 2020804 /*!< 'fbc:id' must have SId syntax */
, FbcGeneProdAssocContainsOneElement    = 2020805 /*!< <code>&lt;GeneProductAssociation&gt;</code> must contain one concrete object */
, FbcGeneProdAssocNameMustBeString      = 2020806 /*!< 'fbc:name' must be string */

, FbcGeneProdRefAllowedCoreAttribs      = 2020901 /*!< Allowed core attributes on <code>&lt;GeneProductRef&gt;</code> */
, FbcGeneProdRefAllowedCoreElements     = 2020902 /*!< Allowed core elements on <code>&lt;GeneProductRef&gt;</code> */
, FbcGeneProdRefAllowedAttribs          = 2020903 /*!< Allowed fbc attributes on <code>&lt;GeneProductRef&gt;</code> */
, FbcGeneProdRefGeneProductSIdRef       = 2020904 /*!< 'fbc:geneProduct' must be SIdRef */
, FbcGeneProdRefGeneProductExists       = 2020908 /*!< 'fbc:geneProduct' must point to existing <code>&lt;GeneProduct&gt;</code> */

, FbcAndAllowedCoreAttributes           = 2021001 /*!< Allowed core attributes on <code>&lt;And&gt;</code> */
, FbcAndAllowedCoreElements             = 2021002 /*!< Allowed core elements on <code>&lt;And&gt;</code> */
, FbcAndTwoChildren                     = 2021003 /*!< <code>&lt;And&gt;</code> must have at least two child elements */

, FbcOrAllowedCoreAttributes            = 2021101 /*!< Allowed core attributes on <code>&lt;Or&gt;</code> */
, FbcOrAllowedCoreElements              = 2021102 /*!< Allowed core elements on <code>&lt;Or&gt;</code> */
, FbcOrTwoChildren                      = 2021103 /*!< <code>&lt;Or&gt;</code> must have at least two child elements */

, FbcGeneProductAllowedCoreAttributes   = 2021201 /*!< Allowed core attributes on <code>&lt;GeneProduct&gt;</code> */
, FbcGeneProductAllowedCoreElements     = 2021202 /*!< Allowed core elements on <code>&lt;GeneProduct&gt;</code> */
, FbcGeneProductAllowedAttributes       = 2021203 /*!< Allowed fbc attributes on <code>&lt;GeneProduct&gt;</code> */
, FbcGeneProductLabelMustBeString       = 2021204 /*!< 'fbc:label' must be string */
, FbcGeneProductLabelMustBeUnique       = 2021205 /*!< 'fbc:label' must be unique */
, FbcGeneProductNameMustBeString        = 2021206 /*!< 'fbc:name' must be string */
, FbcGeneProductAssocSpeciesMustExist   = 2021207 /*!< 'fbc:associatedSpecies' must point to existing <code>&lt;species&gt;</code> */

} FbcSBMLErrorCode_t;


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END



#endif /* SBMLError_h */
