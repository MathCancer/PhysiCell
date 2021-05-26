/**
 * @file GroupsSBMLErrorTable.h
 * @brief Definition of the GroupsSBMLErrorTable class.
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


#ifndef GroupsSBMLErrorTable_H__
#define GroupsSBMLErrorTable_H__


#include <sbml/packages/groups/validator/GroupsSBMLError.h>


LIBSBML_CPP_NAMESPACE_BEGIN



/** @cond doxygenLibsbmlInternal */

static const packageErrorTableEntry groupsErrorTable[] =
{
  // 4010100
  { GroupsUnknown,
    "Unknown error from Groups",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "Unknown error from Groups",
    { ""
    }
  },

  // 4010101
  { GroupsNSUndeclared,
    "The Groups namespace is not correctly declared.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "To conform to the Groups Package specification for SBML Level 3 Version 1, "
    "an SBML document must declare "
    "'http://www.sbml.org/sbml/level3/version1/groups/version1' as the "
    "XMLNamespaceto use for elements of this package.",
    { "L3V1 Groups V1 Section 3.1"
    }
  },

  // 4010102
  { GroupsElementNotInNs,
    "Element not in Groups namespace",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "Wherever they appear in an SBML document, elements and attributes from the "
    "Groups Package must use the "
    "'http://www.sbml.org/sbml/level3/version1/groups/version1' namespace, "
    "declaring so either explicitly or implicitly.",
    { "L3V1 Groups V1 Section 3.1"
    }
  },

  // 4010301
  { GroupsDuplicateComponentId,
    "Duplicate 'id' attribute value",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "(Extends validation rule #10301 in the SBML Level 3 Core specification. TO "
    "DO list scope of ids)",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4010302
  { GroupsIdSyntaxRule,
    "Invalid SId syntax",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of a 'groups:id' must conform to the syntax of the <sbml> data "
    "type 'SId'",
    { "L3V1 Groups V1 Section 3.3.1"
    }
  },

  // 4020101
  { GroupsAttributeRequiredMissing,
    "Required groups:required attribute on <sbml>",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "In all SBML documents using the Groups Package, the <sbml> object must "
    "have the 'groups:required' attribute.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 4020102
  { GroupsAttributeRequiredMustBeBoolean,
    "The groups:required attribute must be Boolean",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'groups:required' on the <sbml> object must be of "
    "data type 'boolean'.",
    { "L3V1 Core Section 4.1.2"
    }
  },

  // 4020103
  { GroupsAttributeRequiredMustHaveValue,
    "The groups:required attribute must be 'false'",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of attribute 'groups:required' on the <sbml> object must be set "
    "to 'false'.",
    { "L3V1 Groups V1 Section 3.1"
    }
  },

  // 4020201
  { GroupsModelAllowedElements,
    "Elements allowed on <model>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <model> object may contain one and only one instance of the "
    "<listOfGroups> element. No other elements from the SBML Level 3 Groups "
    "namespaces are permitted on a <model> object. ",
    { "L3V1 Groups V1 Section 3.6"
    }
  },

  // 4020202
  { GroupsModelEmptyLOElements,
    "No Empty ListOf elements allowed on <Model>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The <listOfGroups> subobject on a <model> object is optional, but if "
    "present, this container object must not be empty.",
    { "L3V1 Groups V1 Section 3.6"
    }
  },

  // 4020203
  { GroupsModelLOGroupsAllowedCoreElements,
    "Core elements allowed on <model>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotations subobjects permitted on all "
    "SBML objects, a <listOfGroups> container object may only contain <group> "
    "objects.",
    { "L3V1 Groups V1 Section 3.6"
    }
  },

  // 4020204
  { GroupsModelLOGroupsAllowedCoreAttributes,
    "Core attributes allowed on <model>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <listOfGroups> object may have the optional SBML Level 3 Core attributes "
    "'metaid' and 'sboTerm'. No other attributes from the SBML Level 3 Core "
    "namespaces are permitted on a <listOfGroups> object.",
    { "L3V1 Groups V1 Section 3.6"
    }
  },

  // 4020301
  { GroupsGroupAllowedCoreAttributes,
    "Core attributes allowed on <group>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <group> object may have the optional SBML Level 3 Core attributes "
    "'metaid' and 'sboTerm'. No other attributes from the SBML Level 3 Core "
    "namespaces are permitted on a <group>.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020302
  { GroupsGroupAllowedCoreElements,
    "Core elements allowed on <group>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <group> object may have the optional SBML Level 3 Core subobjects for "
    "notes and annotations. No other elements from the SBML Level 3 Core "
    "namespaces are permitted on a <group>.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020303
  { GroupsGroupAllowedAttributes,
    "Attributes allowed on <group>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <group> object must have the required attribute 'groups:kind', and may "
    "have the optional attributes 'groups:id' and 'groups:name'. No other "
    "attributes from the SBML Level 3 Groups namespaces are permitted on a "
    "<group> object. ",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4020304
  { GroupsGroupAllowedElements,
    "Elements allowed on <group>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <group> object may contain one and only one instance of the "
    "<listOfMembers> element. No other elements from the SBML Level 3 Groups "
    "namespaces are permitted on a <group> object. ",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4020305
  { GroupsGroupKindMustBeGroupKindEnum,
    "Kind attribute must be GroupKindEnum.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'groups:kind' of a <group> object must conform "
    "to the syntax of SBML data type 'groupKind' and may only take on the "
    "allowed values of 'groupKind' defined in SBML; that is, the value must be "
    "one of the following: 'classification', 'partonomy' or 'collection'.",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4020306
  { GroupsGroupNameMustBeString,
    "Name attribute must be String.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The attribute 'groups:name' on a <group> must have a value of data type "
    "'string'.",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4020307
  { GroupsGroupEmptyLOElements,
    "No Empty ListOf elements allowed on <group>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The <listOfMembers> subobject on a <group> object is optional, but if "
    "present, this container object must not be empty.",
    { "L3V1 Groups V1 Section 3.3"
    }
  },

  // 4020308
  { GroupsGroupLOMembersAllowedCoreElements,
    "Core elements allowed on <listOfMembers>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "Apart from the general notes and annotations subobjects permitted on all "
    "SBML objects, a <listOfMembers> container object may only contain <member> "
    "objects.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020309
  { GroupsGroupLOMembersAllowedCoreAttributes,
    "Core attributes allowed on <listOfMembers>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <listOfMembers> object may have the optional SBML Level 3 Core "
    "attributes 'metaid' and 'sboTerm'. No other attributes from the SBML Level "
    "3 Core namespaces are permitted on a <listOfMembers> object.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020310
  { GroupsGroupLOMembersAllowedAttributes,
    "Attributes allowed on <listOfMembers>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <listOfMembers> object may have the optional attributes 'groups:id' and "
    "'groups:name'. No other attributes from the SBML Level 3 Groups namespaces "
    "are permitted on a <listOfMembers> object. ",
    { "L3V1 Groups V1 Section 3.4"
    }
  },

  // 4020311
  { GroupsLOMembersNameMustBeString,
    "Name attribute must be String.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The attribute 'groups:name' on a <listOfMembers> must have a value of data "
    "type 'string'.",
    { "L3V1 Groups V1 Section 3.4"
    }
  },

  // 4020312
  { GroupsLOMembersConsistentReferences,
    "Consistent references by multiple <member> objects.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_WARNING,
    "If <listOfMembers> objects from different <group> objects contain "
    "<member> elements that reference the same SBase object, the 'sboTerm' "
    "attribute and any child <notes> or <annotation> elements set for those "
    "<listOfMembers> should be consistent, as they should all apply to the "
    "same referenced object.",
    { "L3V1 Groups V1 Section 3.5.4"
    }
  },

  // 4020313
  { GroupsNotCircularReferences,
    "Circular references by multiple <member> objects.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "Member references may not be circular: no <member>'s 'idRef' or "
    "'metaIdRef' may reference itself, its parent <listOfMembers>, nor its "
    "parent <group>. If a <member> references a <group> or a <listOfMembers>, "
    "the same restrictions apply to that subgroup's children: they may not "
    "reference the <member>, its parent <listOfMembers>, nor its parent "
    "<group>, and if any of those children reference a <group>, the same "
    "restrictions apply to them, etc.",
    { "L3V1 Groups V1 Section 3.5.4"
    }
  },

  // 4020401
  { GroupsMemberAllowedCoreAttributes,
    "Core attributes allowed on <member>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <member> object may have the optional SBML Level 3 Core attributes "
    "'metaid' and 'sboTerm'. No other attributes from the SBML Level 3 Core "
    "namespaces are permitted on a <member>.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020402
  { GroupsMemberAllowedCoreElements,
    "Core elements allowed on <member>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <member> object may have the optional SBML Level 3 Core subobjects for "
    "notes and annotations. No other elements from the SBML Level 3 Core "
    "namespaces are permitted on a <member>.",
    { "L3V1 Core Section 3.2"
    }
  },

  // 4020403
  { GroupsMemberAllowedAttributes,
    "Attributes allowed on <member>.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "A <member> object may have the optional attributes 'groups:id' and "
    "'groups:name' and must have a value for one (and exactly one) of "
    "the attributes 'groups:idRef' and 'groups:metaIdRef'. No other attributes "
    "from the SBML Level 3 Groups namespaces are permitted on a <member> "
    "object. ",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

  // 4020404
  { GroupsMemberNameMustBeString,
    "Name attribute must be String.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The attribute 'groups:name' on a <member> must have a value of data type "
    "'string'.",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

  // 4020405
  { GroupsMemberIdRefMustBeSBase,
    "Attribute 'idRef' must point to SBase object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'groups:idRef' of a <member> object must be the "
    "identifier of an existing <SBase> object defined in the enclosing <model> "
    "object.",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

  // 4020406
  { GroupsMemberMetaIdRefMustBeSBase,
    "Attribute 'metaIdRef' must point to SBase object.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'groups:metaIdRef' of a <member> object must be the "
    "'metaid' of an existing <SBase> object defined in the enclosing <model> "
    "object.",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

  // 4020407
  { GroupsMemberIdRefMustBeSId,
    "Attribute 'idRef' must be type 'SId'.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'groups:idRef' of a <member> object must "
    "conform to the syntax of the SBML data type 'SId'.",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

  // 4020408
  { GroupsMemberMetaIdRefMustBeID,
    "Attribute 'metaIdRef' must be type 'ID'.",
    LIBSBML_CAT_GENERAL_CONSISTENCY,
    LIBSBML_SEV_ERROR,
    "The value of the attribute 'groups:metaIdRef' of a <member> object must "
    "conform to the syntax of the SBML data type 'ID'.",
    { "L3V1 Groups V1 Section 3.5"
    }
  },

};

/** @endcond */



LIBSBML_CPP_NAMESPACE_END




#endif /* !GroupsSBMLErrorTable_H__ */


