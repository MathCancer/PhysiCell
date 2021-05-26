/**
 * @file   QualSBMLDocumentPlugin.h
 * @brief  Implementation of the QualSBMLDocumentPlugin class
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
 * ------------------------------------------------------------------------ -->
 *
 * @class QualSBMLDocumentPlugin
 * @sbmlbrief{qual} Extension of SBMLDocument.
 *
 * The QualSBMLDocumentPlugin class inherits from the SBMLDocumentPlugin
 * class, and codifies the extensions to the SBMLDocument class defined in
 * the SBML Level&nbsp;3 @ref qual (&ldquo;qual&rdquo;) package.
 *
 * The QualSBMLDocumentPlugin defines a
 * required flag named <code>required</code>, which indicates whether the
 * 'qual' constructs can be used to change the core mathematics of the
 * <code>&lt;model&gt;</code> child of the <code>&lt;sbml&gt;</code> element.
 * Because they can not, this attribute must be set @c false.
 */


#ifndef QualSBMLDocumentPlugin_H__
#define QualSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/qual/extension/QualExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN QualSBMLDocumentPlugin : public SBMLDocumentPlugin
{
public:

  /**
   * Creates a new QualSBMLDocumentPlugin object using the given parameters.
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
   * @param qualns the namespaces object for the package.
   */
  QualSBMLDocumentPlugin(const std::string& uri, const std::string& prefix, 
                                 QualPkgNamespaces* qualns);


  /**
   * Copy constructor for QualSBMLDocumentPlugin.
   *
   * @param orig the QualSBMLDocumentPlugin instance to copy.
   */
  QualSBMLDocumentPlugin(const QualSBMLDocumentPlugin& orig);


  /**
   * Assignment operator for QualSBMLDocumentPlugin.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment
   */
  QualSBMLDocumentPlugin& operator=(const QualSBMLDocumentPlugin& rhs);


  /**
   * Creates and returns a deep copy of this QualSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this QualSBMLDocumentPlugin object.
   */
  virtual QualSBMLDocumentPlugin* clone () const;


  /**
   * Destructor for QualSBMLDocumentPlugin.
   */
  virtual ~QualSBMLDocumentPlugin();


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
  /**
   * Accepts the SBMLVisitor.
   */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */
  /** @endcond */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* QualSBMLDocumentPlugin_H__ */


