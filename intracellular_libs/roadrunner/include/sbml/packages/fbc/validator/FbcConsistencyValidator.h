/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    FbcConsistencyValidator.h
 * @brief   Performs consistency checks on an SBML model
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
 * in the file named "LICENSE.txt" included with this software distribution and
 * also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#ifndef FbcConsistencyValidator_h
#define FbcConsistencyValidator_h


#ifdef __cplusplus


#include <sbml/packages/fbc/validator/FbcValidator.h>


LIBSBML_CPP_NAMESPACE_BEGIN

class FbcConsistencyValidator: public FbcValidator
{
public:

  FbcConsistencyValidator () :
    FbcValidator( LIBSBML_CAT_GENERAL_CONSISTENCY ) { }

  virtual ~FbcConsistencyValidator () { }

  /**
   * Initializes this Validator with a set of Constraints.
   */
  virtual void init ();
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* ConsistencyValidator_h */
/** @endcond */

