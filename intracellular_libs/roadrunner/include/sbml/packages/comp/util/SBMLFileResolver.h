/**
 * @file    SBMLFileResolver.h
 * @brief   A file-based resolver for SBML Documents.
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
 * @class SBMLFileResolver
 * @sbmlbrief{comp} Resolves documents stored on a file system.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * In SBML, @em resolvers come into play with the SBML Level&nbsp;3
 * Hierarchical %Model Composition package (&ldquo;comp&rdquo;); this package
 * includes features that allow a model to be composed from pieces that are
 * external to a given SBML document, which implies the need to be able to
 * identify and locate those external pieces.  The identifiers used in
 * &ldquo;comp&rdquo; are URIs (<a target="_blank"
 * href="http://en.wikipedia.org/wiki/Uniform_resource_identifier">Uniform
 * Resource Identifiers</a>).
 *
 * SBMLFileResolver is a class implementing the ability to resolve URIs to
 * files.  It works on the local file system only.  It can resolve relative
 * and absolute paths, and directories to be searched can be specified using
 * the methods @if clike SBMLFileResolver::setAdditionalDirs(), @endif
 * SBMLFileResolver::addAdditionalDir(@if java String@endif) and
 * SBMLFileResolver::clearAdditionalDirs().
 *
 * @see SBMLResolver
 * @see SBMLUri
 */

#ifndef SBMLFileResolver_h
#define SBMLFileResolver_h

#include <sbml/common/sbmlfwd.h>
#ifdef __cplusplus

#include <string>
#include <vector>
#include <sbml/packages/comp/util/SBMLResolver.h>


LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN SBMLFileResolver : public SBMLResolver
{
public:

  /**
   * Creates a new SBMLFileResolver object.
   */
  SBMLFileResolver ();


  /**
   * Copy constructor.  Creates a copy of an SBMLFileResolver object.
   *
   * @param c the SBMLFileResolver object to copy.
   */
  SBMLFileResolver(const SBMLFileResolver& c);


  /**
   * Destroy this SBMLFileResolver object.
   */
  virtual ~SBMLFileResolver ();


  /**
   * Assignment operator for SBMLFileResolver.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  SBMLFileResolver& operator=(const SBMLFileResolver& rhs);


  /**
   * Creates and returns a deep copy of this SBMLFileResolver object.
   *
   * @return a (deep) copy of this SBMLFileResolver object.
   */
  virtual SBMLFileResolver* clone() const;


  /**
   * Resolves the document for the given URI.
   *
   * @param uri the URI to the target document.
   * @param baseUri base URI, in case the URI is a relative one.
   *
   * @return the document, if this resolver can resolve the document or NULL.
   */
  virtual SBMLDocument* resolve(const std::string &uri,
                                const std::string& baseUri="") const;


  /**
   * Resolves the full URI for a given URI without actually reading the
   * document.
   *
   * @param uri the URI to the target document.
   * @param baseUri base URI, in case the URI is a relative one.
   *
   * @return the full URI to the document, if this resolver can resolve the
   * document or NULL.
   */
  virtual SBMLUri* resolveUri(const std::string &uri, 
                              const std::string& baseUri="") const;


  /**
   * Sets the list of directories in which to search for files to resolve.
   *
   * Unlike the similar
   * SBMLFileResolver::addAdditionalDir(@if java String@endif), this
   * method replaces any current list of search directories with the given
   * list of @p dirs.
   *
   * @param dirs a vector of strings which contain directories.
   *
   * @see addAdditionalDir(@if java String@endif)
   * @see clearAdditionalDirs()
   */
  virtual void setAdditionalDirs(const std::vector<std::string>& dirs);


  /**
   * Removes the list of directories to search for files to resolve.
   *
   * After this method is called, SBMLFileResolver::resolve(const std::string
   * &uri, const std::string& baseUri) will only search absolute or relative
   * directories.  New directories can be added using
   * SBMLFileResolver::addAdditionalDir(@if java String@endif) @if clike or
   * setAdditionalDirs()@endif.
   *
   * @see addAdditionalDir(@if java String@endif)
   * @if clike @see setAdditionalDirs()@endif
   */
  virtual void clearAdditionalDirs();


  /**
   * Adds a directory to the list of directories to search for files to
   * resolve.
   *
   * @param dir the directory to add.
   *
   * @see clearAdditionalDirs()
   * @if clike @see setAdditionalDirs()@endif
   */
  virtual void addAdditionalDir(const std::string& dir);

#ifndef SWIG

#endif // SWIG


protected:
  /** @cond doxygenLibsbmlInternal */
  std::vector<std::string> mAdditionalDirs;
  /** @endcond */


private:
  /** @cond doxygenLibsbmlInternal */
  static bool fileExists(const std::string& fileName);
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
#endif  /* SBMLFileResolver_h */

