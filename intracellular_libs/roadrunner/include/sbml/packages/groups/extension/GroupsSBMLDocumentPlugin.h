/**
 * @file GroupsSBMLDocumentPlugin.h
 * @brief Definition of the GroupsSBMLDocumentPlugin class.
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class GroupsSBMLDocumentPlugin
 * @sbmlbrief{groups} Extension of SBMLDocument.
 *
 * This class extends SBMLDocumentPlugin, a class that is used by libSBML
 * plugins as part of their implementation of SBML Level&nbsp;3 packages.
 *
 * @copydetails doc_what_is_sbmldocumentplugin
 */


#ifndef GroupsSBMLDocumentPlugin_H__
#define GroupsSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN GroupsSBMLDocumentPlugin : public SBMLDocumentPlugin
{

public:

  /**
   * Creates a new GroupsSBMLDocumentPlugin using the given uri, prefix and
   * package namespace.
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
   * @param groupsns the namespaces object for the package.
   */
  GroupsSBMLDocumentPlugin(const std::string& uri,
                           const std::string& prefix,
                           GroupsPkgNamespaces* groupsns);


  /**
   * Copy constructor for GroupsSBMLDocumentPlugin.
   *
   * @param orig the GroupsSBMLDocumentPlugin instance to copy.
   */
  GroupsSBMLDocumentPlugin(const GroupsSBMLDocumentPlugin& orig);


  /**
   * Assignment operator for GroupsSBMLDocumentPlugin.
   *
   * @param rhs the GroupsSBMLDocumentPlugin object whose values are to be
   * used as the basis of the assignment.
   */
  GroupsSBMLDocumentPlugin& operator=(const GroupsSBMLDocumentPlugin& rhs);


  /**
   * Creates and returns a deep copy of this GroupsSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this GroupsSBMLDocumentPlugin object.
   */
  virtual GroupsSBMLDocumentPlugin* clone() const;


  /**
   * Destructor for GroupsSBMLDocumentPlugin.
   */
  virtual ~GroupsSBMLDocumentPlugin();



  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor
   */
  virtual bool accept(SBMLVisitor& v) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Predicate indicating whether 'comp' flattening has been implemented for
   * the Groups package.
   */
  virtual bool isCompFlatteningImplemented() const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Calls check consistency for any relevant Groups validators.
   */
  virtual unsigned int checkConsistency();

  /** @endcond */




  #ifndef SWIG



  /** @cond doxygenLibsbmlInternal */

  /**
   * Reads the Groups attributes in the top-level element.
   */
  virtual void readAttributes(const XMLAttributes& attributes,
                              const ExpectedAttributes& expectedAttributes);

  /** @endcond */




  #endif /* !SWIG */


protected:

};



LIBSBML_CPP_NAMESPACE_END




#endif /* __cplusplus */




#endif /* !GroupsSBMLDocumentPlugin_H__ */


