/**
 * @file    compfwd.h
 * @brief   SBML Comp C structure declarations
 * @author  Lucian Smith
 *
 *<!---------------------------------------------------------------------------
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
 *------------------------------------------------------------------------- -->
 */


#ifndef compfwd_h__
#define compfwd_h__

/**
 * Forward declaration of all opaque C types.
 *
 * Declaring all types up-front avoids "redefinition of type 'Foo'" compile
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

/** @cond (!conly) */


/**
 * @var typedef class SBaseRef SBaseRef_t
 * @copydoc SBaseRef
 */
typedef CLASS_OR_STRUCT SBaseRef                     SBaseRef_t;


/**
 * @var typedef class Deletion Deletion_t
 * @copydoc Deletion
 */
typedef CLASS_OR_STRUCT Deletion                     Deletion_t;


/**
 * @var typedef class ExternalModelDefinition ExternalModelDefinition_t
 * @copydoc ExternalModelDefinition
 */
typedef CLASS_OR_STRUCT ExternalModelDefinition      ExternalModelDefinition_t;


/**
 * @var typedef class Port Port_t
 * @copydoc Port
 */
typedef CLASS_OR_STRUCT Port                         Port_t;


/**
 * @var typedef class ReplacedBy ReplacedBy_t
 * @copydoc ReplacedBy
 */
typedef CLASS_OR_STRUCT ReplacedBy                   ReplacedBy_t;


/**
 * @var typedef class ReplacedElement ReplacedElement_t
 * @copydoc ReplacedElement
 */
typedef CLASS_OR_STRUCT ReplacedElement              ReplacedElement_t;


/**
 * @var typedef class Submodel Submodel_t
 * @copydoc Submodel
 */
typedef CLASS_OR_STRUCT Submodel                     Submodel_t;


/**
 * @var typedef class SBMLDocumentPlugin SBMLDocumentPlugin_t
 * @copydoc SBMLDocumentPlugin
 */
typedef CLASS_OR_STRUCT CompSBMLDocumentPlugin       CompSBMLDocumentPlugin_t;
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/**
 * @var typedef class ModelPlugin ModelPlugin_t
 * @copydoc ModelPlugin
 */
typedef CLASS_OR_STRUCT CompModelPlugin               CompModelPlugin_t;
/** @endcond */


/** @cond (!conly) */

/**
 * @var typedef class ModelDefinition ModelDefinition_t
 * @copydoc ModelDefinition
 */
typedef CLASS_OR_STRUCT ModelDefinition              ModelDefinition_t;
/** @endcond */


LIBSBML_CPP_NAMESPACE_END

#undef CLASS_OR_STRUCT

#endif  /* compfwd_h__ */
