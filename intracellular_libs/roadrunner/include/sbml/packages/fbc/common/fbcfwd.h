/**
 * @file   fbcfwd.h
 * @brief  Implementation of the fbcfwd class
 * @author SBMLTeam
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
 *------------------------------------------------------------------------- -->
 */
#ifndef fbcfwd_H
#define fbcfwd_H

/**
 * Forward declaration of all opaque C types.
 *
 * Declaring all types up-front avoids "redefinition of type Foo" compile
 * errors and allows our combined C/C++ headers to depend minimally upon
 * each other.  Put another way, the type definitions below serve the same
 * purpose as "class Foo;" forward declarations in C++ code.
 */

#ifdef __cplusplus
#  define CLASS_OR_STRUCT class
#else
#  define CLASS_OR_STRUCT struct
#endif  /* __cplusplus */	

LIBSBML_CPP_NAMESPACE_BEGIN

typedef CLASS_OR_STRUCT FbcAssociation    FbcAssociation_t;
typedef CLASS_OR_STRUCT Association      Association_t;
typedef CLASS_OR_STRUCT FluxBound        FluxBound_t;
typedef CLASS_OR_STRUCT FluxObjective    FluxObjective_t;
typedef CLASS_OR_STRUCT GeneAssociation  GeneAssociation_t;
typedef CLASS_OR_STRUCT Objective    Objective_t;
typedef CLASS_OR_STRUCT GeneProductAssociation    GeneProductAssociation_t;
typedef CLASS_OR_STRUCT GeneProduct    GeneProduct_t;
typedef CLASS_OR_STRUCT GeneProductRef    GeneProductRef_t;
typedef CLASS_OR_STRUCT FbcAnd    FbcAnd_t;
typedef CLASS_OR_STRUCT FbcOr    FbcOr_t;

LIBSBML_CPP_NAMESPACE_END

#undef CLASS_OR_STRUCT


#endif  /* fbcfwd_H */

