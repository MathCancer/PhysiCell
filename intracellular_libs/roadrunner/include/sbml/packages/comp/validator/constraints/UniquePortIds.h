/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UniquePortIds.h
 * @brief   Ensures the appropriate ids within a Model are unique
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

#ifndef UniquePortIds_h
#define UniquePortIds_h


#ifdef __cplusplus

#include <string>

#include "UniqueCompIdBase.h"

LIBSBML_CPP_NAMESPACE_BEGIN

class UniquePortIds: public UniqueCompIdBase
{
public:

  /**
   * Creates a new Constraint with the given constraint id.
   */
  UniquePortIds (unsigned int id, CompValidator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~UniquePortIds ();


protected:

  /**
   * Checks that all ids on the Port objects with the
   * given Model object are unique.
   */
  virtual void doCheck (const Model& m);
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* UniquePortIds_h */
/** @endcond */
