/**
 * @file   FbcSBMLDocumentPlugin.h
 * @brief  Implementation of the FbcSBMLDocumentPlugin class
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
 *
 * @class FbcSBMLDocumentPlugin
 * @sbmlbrief{fbc} Extension of SBMLDocument for the &ldquo;fbc&rdquo; package.
 *
 * The FbcSBMLDocumentPlugin class inherits from the SBMLDocumentPlugin
 * class, and codifies the extensions to the SBMLDocument class defined in
 * the SBML Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;) package.  This
 * extension defines a required flag named "required", which indicates
 * whether &ldquo;fbc&rdquo; constructs can be used to change the core
 * mathematical interpretation of the Model defined in the SBML input.  Since
 * &ldquo;fbc&rdquo; constructs cannot, this attribute must be set to the
 * value @c "false".
 */


#ifndef FbcSBMLDocumentPlugin_H__
#define FbcSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FbcSBMLDocumentPlugin : public SBMLDocumentPlugin
{
public:

  /**
   * Creates a new FbcSBMLDocumentPlugin object using the given parameters.
   *
   * @copydetails doc_what_are_xmlnamespaces
   *
   * @copydetails doc_what_are_sbmlnamespaces
   *
   * @param uri the URI of the SBML Level&nbsp;3 package implemented by
   * this libSBML package extension.
   *
   * @param prefix the XML namespace prefix being used for the package.
   *
   * @param fbcns the namespaces object for the package.
   */
  FbcSBMLDocumentPlugin(const std::string& uri, const std::string& prefix, 
                                 FbcPkgNamespaces* fbcns);


  /**
   * Copy constructor for FbcSBMLDocumentPlugin.
   *
   * @param orig the FbcSBMLDocumentPlugin instance to copy.
   */
  FbcSBMLDocumentPlugin(const FbcSBMLDocumentPlugin& orig);


   /**
   * Assignment operator for FbcSBMLDocumentPlugin.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcSBMLDocumentPlugin& operator=(const FbcSBMLDocumentPlugin& rhs);


   /**
   * Creates and returns a deep copy of this FbcSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this FbcSBMLDocumentPlugin object.
   */
  virtual FbcSBMLDocumentPlugin* clone () const;


   /**
   * Destructor for FbcSBMLDocumentPlugin.
   */
  virtual ~FbcSBMLDocumentPlugin();


 #ifndef SWIG

  /** @cond doxygenLibsbmlInternal */

  /**
   * Reads the attributes of corresponding package in SBMLDocument element
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond */


#endif // SWIG

  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns boolean based on whether flattening of a comp model has been implemented.
   *
   * @returns @c true if flattening for composed models has been implemented,
   * false otherwise.
   */
  virtual bool isCompFlatteningImplemented() const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Check consistency function.
   */
  virtual unsigned int checkConsistency();


  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */

  /** @endcond */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* FbcSBMLDocumentPlugin_H__ */


