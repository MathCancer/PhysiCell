/**
 * @file GroupsSBMLError.h
 * @brief Definition of the GroupsSBMLError class.
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */


#ifndef GroupsSBMLError_H__
#define GroupsSBMLError_H__

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * @enum GroupsSBMLErrorCode_t
 * Codes for all SBML-level errors and warnings from the 'groups' package.
 *
 * These are distinguished from other SBML error codes
 * by having a number between 4000000 and 4099999.
 *
 * @copydetails doc_sbml_error_code_ranges
 */
typedef enum
{
  GroupsUnknown                                               = 4010100
, GroupsNSUndeclared                                          = 4010101
, GroupsElementNotInNs                                        = 4010102
, GroupsDuplicateComponentId                                  = 4010301
, GroupsIdSyntaxRule                                          = 4010302
, GroupsAttributeRequiredMissing                              = 4020101
, GroupsAttributeRequiredMustBeBoolean                        = 4020102
, GroupsAttributeRequiredMustHaveValue                        = 4020103
, GroupsModelAllowedElements                                  = 4020201
, GroupsModelEmptyLOElements                                  = 4020202
, GroupsModelLOGroupsAllowedCoreElements                      = 4020203
, GroupsModelLOGroupsAllowedCoreAttributes                    = 4020204
, GroupsGroupAllowedCoreAttributes                            = 4020301
, GroupsGroupAllowedCoreElements                              = 4020302
, GroupsGroupAllowedAttributes                                = 4020303
, GroupsGroupAllowedElements                                  = 4020304
, GroupsGroupKindMustBeGroupKindEnum                          = 4020305
, GroupsGroupNameMustBeString                                 = 4020306
, GroupsGroupEmptyLOElements                                  = 4020307
, GroupsGroupLOMembersAllowedCoreElements                     = 4020308
, GroupsGroupLOMembersAllowedCoreAttributes                   = 4020309
, GroupsGroupLOMembersAllowedAttributes                       = 4020310
, GroupsLOMembersNameMustBeString                             = 4020311
, GroupsLOMembersConsistentReferences                         = 4020312
, GroupsNotCircularReferences                                 = 4020313
, GroupsMemberAllowedCoreAttributes                           = 4020401
, GroupsMemberAllowedCoreElements                             = 4020402
, GroupsMemberAllowedAttributes                               = 4020403
, GroupsMemberNameMustBeString                                = 4020404
, GroupsMemberIdRefMustBeSBase                                = 4020405
, GroupsMemberMetaIdRefMustBeSBase                            = 4020406
, GroupsMemberIdRefMustBeSId                                  = 4020407
, GroupsMemberMetaIdRefMustBeID                               = 4020408
} GroupsSBMLErrorCode_t;


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END




#endif /* !GroupsSBMLError_H__ */


