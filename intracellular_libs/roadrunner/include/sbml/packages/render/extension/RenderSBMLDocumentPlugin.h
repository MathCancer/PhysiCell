/**
 * @file:   RenderSBMLDocumentPlugin.h
 * @brief:  Implementation of the RenderSBMLDocumentPlugin class
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
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */


#ifndef RenderSBMLDocumentPlugin_H__
#define RenderSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/render/extension/RenderExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN RenderSBMLDocumentPlugin : public SBMLDocumentPlugin
{
public:

  /**
   * Creates a new RenderSBMLDocumentPlugin
   */
  RenderSBMLDocumentPlugin(const std::string& uri, const std::string& prefix, 
                                 RenderPkgNamespaces* multins);


  /**
   * Copy constructor for RenderSBMLDocumentPlugin.
   *
   * @param orig; the RenderSBMLDocumentPlugin instance to copy.
   */
  RenderSBMLDocumentPlugin(const RenderSBMLDocumentPlugin& orig);


   /**
   * Assignment operator for RenderSBMLDocumentPlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  RenderSBMLDocumentPlugin& operator=(const RenderSBMLDocumentPlugin& rhs);


   /**
   * Creates and returns a deep copy of this RenderSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this RenderSBMLDocumentPlugin object.
   */
  virtual RenderSBMLDocumentPlugin* clone () const;


   /**
   * Destructor for RenderSBMLDocumentPlugin.
   */
  virtual ~RenderSBMLDocumentPlugin();


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
#if (0)
  virtual unsigned int checkConsistency();
#endif

  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the SBMLVisitor.
   */
//  virtual bool accept(SBMLVisitor& v) const;


   /** @endcond doxygenLibsbmlInternal */


protected:

  /** @cond doxygenLibsbmlInternal */

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* RenderSBMLDocumentPlugin_H__ */


