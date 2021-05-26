/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    SubmodelReferenceCycles.h
 * @brief   Ensures unique variables assigned by rules and events
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

#ifndef SubmodelReferenceCycles_h
#define SubmodelReferenceCycles_h


#ifdef __cplusplus

#include <string>
#include <sbml/validator/VConstraint.h>

#include <sbml/util/IdList.h>

LIBSBML_CPP_NAMESPACE_BEGIN

typedef std::multimap<const std::string, std::string> IdMap;
typedef IdMap::iterator                               IdIter;
typedef std::pair<IdIter, IdIter>                     IdRange;

class SubmodelReferenceCycles: public TConstraint<Model>
{
public:

  /**
   * Creates a new Constraint with the given constraint id.
   */
  SubmodelReferenceCycles (unsigned int id, Validator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~SubmodelReferenceCycles ();


protected:

  virtual void check_ (const Model& m, const Model& object);

  void addAllReferences (const Model* m);

  void addAllExternalReferences(const SBMLDocument* doc, 
                                          std::string location);

  /* create pairs of ids that depend on each other */
  void addModelReferences(const std::string &id, 
                          const CompModelPlugin* modelPlug); 
  
  void addExtModelReferences(const std::string &id, 
                          const CompSBMLDocumentPlugin* docPlug,
                          const CompModelPlugin* modelPlug);

  void determineAllDependencies();


  /* helper function to check if a pair already exists */
  bool alreadyExistsInMap(IdMap map, 
                          std::pair<const std::string, std::string> dependency);

  
  /* find cycles in the map of dependencies */
  void determineCycles(const Model& m);


  /**
   * functions for logging messages about the cycle
   */

  
  void logCycle (const Model& m, std::string id, std::string id1);
  
  
  IdMap mIdMap;

  IdList mDocumentsHandled;

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* SubmodelReferenceCycles_h */
/** @endcond */

