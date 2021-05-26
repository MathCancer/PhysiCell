/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    FluxBoundsConsistent.h
 * @brief   Checks for duplicate top level annotations
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
 * ---------------------------------------------------------------------- -->*/

#ifndef FluxBoundsConsistent_h
#define FluxBoundsConsistent_h


#ifdef __cplusplus

#include <string>

#include <sbml/validator/VConstraint.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class FluxBoundsConsistent: public TConstraint<Model>
{
public:

  /**
   * Creates a new Constraint with the given constraint id.
   */
  FluxBoundsConsistent (unsigned int id, Validator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~FluxBoundsConsistent ();


protected:

  virtual void check_ (const Model& m, const Model& object);

  void checkBounds(ListOfFluxBounds& object);

  void logInconsistency (const std::string name, const FluxBound& object);

};

LIBSBML_CPP_NAMESPACE_END
#endif  /* __cplusplus */
#endif  /* FluxBoundsConsistent_h */
/** @endcond */
