/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UniqueGeneProductLabels.h
 * @brief   Base class for Id constraints
 * @author  Ben Bornstein
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

#ifndef UniqueGeneProductLabels_h
#define UniqueGeneProductLabels_h


#ifdef __cplusplus


#include <string>
#include <sstream>
#include <set>

#include <sbml/validator/VConstraint.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class SBase;
class Validator;


/**
 * The UniqueGeneProductLabels Constraint is the base class for all SBML id constraints.  It
 * provides mechanisms for checking only certain subsets of ids within an
 * SBML model and tailoring the error messages logged.
 *
 * To customize:
 *
 *   1.  Override doCheck() to iterate over the SBML objects you are
 *       interested in and call checkId() for each.
 *
 *       checkId() does the work of extracting the unique identifier
 *       (whether it be an id or variable name) from the SBML object and
 *       then delegates to doCheckId().
 *
 *   2.  Override doCheckId() to perform the actual check.  If the check
 *       fails, call logFailure().
 *
 *   3.  Override getMessage() to log error messages.  GetMessage() should
 *       use getPreamble() and getFieldName() when constructing the error
 *       message.
 *
 *   4.  Override getPreamble() to customize the part of the actual error
 *       message that remains constant (e.g. the part that doesn't report
 *       line numbers, SBML object ids and typenames, etc).
 *
 *   5.  Override getFieldName() if you are checking a field that isn't
 *       called 'id', for instance, the 'variable' field of a Rule.
 *
 * Finally, if you need the type name of the SBML object that failed,
 * e.g. 'Compartment' or 'Species', when constructing an error message,
 * call getTypename().
 */
class UniqueGeneProductLabels: public TConstraint<Model>
{
public:

  /**
   * Creates a new UniqueGeneProductLabels with the given constraint id.
   */
  UniqueGeneProductLabels (unsigned int id, Validator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~UniqueGeneProductLabels ();


protected:
  /**
   * Called by check().  Override this method to define your own subset.
   */
  void doCheck (const Model& m);


  /* ------------------------------------------------------------ */
  /*  You should not need to override methods beyond this point.  */
  /* ------------------------------------------------------------ */


  /**
   * Checks that all ids for some given subset of the Model adhere to this
   * Constraint.  Override the doCheck() method to define your own subset.
   */
  virtual void check_ (const Model& m, const Model& object);



  /**
   * Logs a message that the given @p id (and its corresponding object) have
   * failed to satisfy this constraint.
   */
  void logConflict (const std::string& label, const SBase& object);


  typedef std::set<std::string> ExistingLabels;
  ExistingLabels mLabels;
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* UniqueGeneProductLabels_h */
/** @endcond */
