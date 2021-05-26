/**
 * @file    SBMLResolver.h
 * @brief   Definition of SBMLResolver, the base class for resolving SBML documents.
 * @author  Frank Bergmann
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
 * ------------------------------------------------------------------------ -->
 *
 * @class SBMLResolver
 * @sbmlbrief{comp} Base class for SBML resolvers.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * The SBMLResolver class is the base class for the various SBML @em
 * resolvers: facilities that take a unique identifier as input and return
 * the document associated with that identifier.  In SBML, resolvers come
 * into play with the SBML Level&nbsp;3 Hierarchical %Model Composition
 * package; this package includes features that allow a model to be composed
 * from pieces that are external to a given SBML document, which implies the
 * need to be able to identify and locate those external pieces.  The
 * SBMLResolver class and its subclasses provide facilities for software
 * applications to be able to do these tasks.
 *
 * LibSBML provides a number of built-in resolvers, and applications can
 * create their own by subclassing SBMLResolver and following the examples
 * of the existing resolvers.  The following are the built-in resolvers
 * in libSBML @htmlinclude libsbml-version.html:
 * @li SBMLFileResolver
 *
 * More resolvers may be provided by libSBML in the future.  Application
 * authors may also write their own.  @if cpp An example of how to create
 * an HTTP resolver is included with the libSBML distribution in the file
 * @ref SBMLHttpResolverExample.cpp "SBMLHttpResolverExample.cpp".@endif
 *
 * @see SBMLUri 
 */

#ifndef SBMLResolver_h
#define SBMLResolver_h

#include <sbml/common/sbmlfwd.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class SBMLUri;
class LIBSBML_EXTERN SBMLResolver
{
public:

  /**
   * Creates a new SBMLResolver object.
   */
  SBMLResolver ();


  /**
   * Copy constructor.  Creates a copy of an SBMLResolver object.
   *
   * @param c the SBMLResolver object to copy.
   */
  SBMLResolver(const SBMLResolver& c);


  /**
   * Destroy this SBMLResolver object.
   */
  virtual ~SBMLResolver ();


  /**
   * Assignment operator for SBMLResolver.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  SBMLResolver& operator=(const SBMLResolver& rhs);


  /**
   * Creates and returns a deep copy of this SBMLResolver object.
   *
   * @return a (deep) copy of this SBMLResolver object.
   */
  virtual SBMLResolver* clone() const;


  /**
   * Resolves the document for the given URI.
   *
   * @param uri the URI to the target document.
   * @param baseUri base URI, in case the URI is a relative one.
   *
   * @return  the document, if this resolver can resolve the document or NULL.
   */
  virtual SBMLDocument* resolve(const std::string &uri, const std::string& baseUri="") const;


  /**
   * Resolves the full URI for the given URI without actually reading the
   * document.
   *
   * @param uri the URI to the target document.
   * @param baseUri base URI, in case the URI is a relative one.
   *
   * @return  the full URI to the document, if this resolver can resolve the document or NULL.
   */
  virtual SBMLUri* resolveUri(const std::string &uri, const std::string& baseUri="") const;


#ifndef SWIG

#endif // SWIG


protected:
  /** @cond doxygenLibsbmlInternal */
  /** @endcond */


private:
  /** @cond doxygenLibsbmlInternal */
  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* SBMLResolver_h */

