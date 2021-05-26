/**
 * @file:   DistribSBMLDocumentPlugin.h
 * @brief:  Implementation of the DistribSBMLDocumentPlugin class
 * @author: SBMLTeam
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
 * ------------------------------------------------------------------------ -->
 */


#ifndef DistribSBMLDocumentPlugin_H__
#define DistribSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/distrib/extension/DistribExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DistribSBMLDocumentPlugin : public SBMLDocumentPlugin
{
public:

  /**
   * Creates a new DistribSBMLDocumentPlugin
   */
  DistribSBMLDocumentPlugin(const std::string& uri, const std::string& prefix, 
                                 DistribPkgNamespaces* distribns);


  /**
   * Copy constructor for DistribSBMLDocumentPlugin.
   *
   * @param orig; the DistribSBMLDocumentPlugin instance to copy.
   */
  DistribSBMLDocumentPlugin(const DistribSBMLDocumentPlugin& orig);


   /**
   * Assignment operator for DistribSBMLDocumentPlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DistribSBMLDocumentPlugin& operator=(const DistribSBMLDocumentPlugin& rhs);


   /**
   * Creates and returns a deep copy of this DistribSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this DistribSBMLDocumentPlugin object.
   */
  virtual DistribSBMLDocumentPlugin* clone () const;


   /**
   * Destructor for DistribSBMLDocumentPlugin.
   */
  virtual ~DistribSBMLDocumentPlugin();


 #ifndef SWIG

  /** @cond doxygenLibsbmlInternal */

  /**
   * Reads the attributes of corresponding package in SBMLDocument element
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond doxygenLibsbmlInternal */


#endif // SWIG

  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns boolean based on whether flattening of a comp model has been implemented.
   *
   * @returns @c true if flattening for composed models has been implemented,
   * false otherwise.
   */
  virtual bool isCompFlatteningImplemented() const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Check consistency function.
   */
  virtual unsigned int checkConsistency();


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the SBMLVisitor.
   */
  virtual bool accept(SBMLVisitor& v) const;


   /** @endcond doxygenLibsbmlInternal */


protected:

  /** @cond doxygenLibsbmlInternal */

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* DistribSBMLDocumentPlugin_H__ */


