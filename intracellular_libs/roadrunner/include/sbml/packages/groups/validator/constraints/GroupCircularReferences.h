/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    GroupCircularReferences.h
 * @brief   Checks cycles in groups
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
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#ifndef GroupCircularReferences_h
#define GroupCircularReferences_h


#ifdef __cplusplus

#include <string>
#include <sbml/validator/VConstraint.h>


LIBSBML_CPP_NAMESPACE_BEGIN

typedef std::multimap<const std::string, std::string> IdMap;
typedef IdMap::iterator                               IdIter;
typedef std::pair<IdIter, IdIter>                     IdRange;

class GroupCircularReferences: public TConstraint<Model>
{
public:

  /**
   * Creates a new Constraint with the given constraint id.
   */
  GroupCircularReferences (unsigned int id, Validator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~GroupCircularReferences ();


protected:

  /**
   * Checks that &lt;ci&gt; element after an apply is already listed as a FunctionDefinition.
   */
  virtual void check_ (const Model& m, const Model& object);

  void addReferenced(const Model &model, const Group *group);

  void addAllReferences(const Member* mem);

  void addChildReferences(const Member* mem, const std::string& id);

  void checkForSelfReference(const Group* g, unsigned int index);

  void determineAllDependencies();


  /* helper function to check if a pair already exists */
  bool alreadyExistsInMap(IdMap map, 
                          std::pair<const std::string, std::string> dependency);

  
  /* find cycles in the map of dependencies */
  void determineCycles(const Model& m);


  /**
   * Logs a message .
   */
  void logCycle (const Model& m, std::string id, std::string id1);

  void logSelfReference (const Member& mem, const Group& group, 
                         unsigned int index);

  void logParentReference (const Member& mem, const Group& group, 
                         unsigned int index);
  IdMap mIdMap;

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* GroupCircularReferences_h */
/** @endcond */

