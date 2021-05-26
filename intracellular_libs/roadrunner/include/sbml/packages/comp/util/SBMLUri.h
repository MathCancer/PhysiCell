/**
 * @file    SBMLUri.h
 * @brief   Definition of SBMLUri, the utility class for handling URIs.
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
 * @class SBMLUri
 * @sbmlbrief{comp} Utility class for handling URIs.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This class implements functionality for parsing URIs and extracting
 * information about them.
 *
 * @see SBMLResolver
 * @see SBMLFileResolver
 */

#ifndef SBMLUri_h
#define SBMLUri_h

#include <sbml/common/sbmlfwd.h>

#ifdef __cplusplus

#include <string>


LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN SBMLUri
{
public:

  /**
   * Creates a new SBMLUri from the given string URI.
   */
  SBMLUri(const std::string& uri); 


  /**
   * Copy constructor.  Creates a copy of an SBMLUri object.
   *
   * @param orig the SBMLUri object to copy.
   */
  SBMLUri(const SBMLUri& orig); 


  /**
   * Destroy this SBMLUri object.
   */
  virtual ~SBMLUri(); 


  /**
   * Assignment operator for SBMLUri.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  SBMLUri& operator=(const SBMLUri& rhs);


  /**
   * Assignment operator for SBMLUri.  Allows parsing of the given string.
   *
   * @param uri the URI to parse.
   */
  SBMLUri& operator=(const std::string& uri);


  /**
   * Creates and returns a deep copy of this SBMLUri object.
   *
   * @return a (deep) copy of this SBMLFileResolver object.
   */
  SBMLUri* clone () const;


  /**
   * Returns the scheme of the stored URI.
   *
   * The @em scheme of the URI is the text before the first colon character.
   * Typical examples of what this might return are the strings @c "file" or
   * @c "http".  If the current URI does not have a scheme, this method
   * returns an empty string.
   *
   * @return the parsed scheme, such as @c "http", or an empty string if no
   * scheme exists for the current URI.
   */
  const std::string& getScheme() const;


  /**
   * Returns the host portion of the stored URI.
   *
   * For a scheme such as @c "http", this method returns the part of the URI
   * after @c "http://" and before the next @c "/" character.  URIs with file
   * or URN schemes have no host; in that case, this method returns an empty
   * string.
   *
   * @return the host of the URI, or an empty string in the case of files
   * or URNs schemes that do not possess a host portion.
   */
  const std::string& getHost() const;


  /**
   * Returns the path and filename portion of the stored URI.
   *
   * This method returns the text after the scheme, colon, and host (if
   * present), and before the next @c "?" character.  The result may be an
   * empty string for some URIs.
   *
   * @return the path of the URI (i.e., the full filename with path).
   */
  const std::string& getPath() const;


  /**
   * Returns the query portion of the stored URI.
   *
   * The equery portion of a URI is the text after a filename, starting with
   * the character @c "?".  For many URIs, this is an empty string.
   *
   * @return the query of the URI (i.e., the part after the full filename
   * with path).
   */
  const std::string& getQuery() const;


  /**
   * Returns the full stored URI, after replacing backslashes with slashes.
   *
   * @return the original URI, with backslashes replaced with slashes.
   */
  const std::string& getUri() const;


  /**
   * Constructs a new URI relative to this object and the given URI.
   *
   * For example,
   * @if cpp
   @code{.cpp}
SBMLUri("c:\\test")->relativeTo("test.xml");
@endcode
@endif
@if python
@code
SBMLUri("c:\\test").relativeTo("test.xml")
@endcode
@endif
@if java
@code
SBMLUri("c:\\test").relativeTo("test.xml");
@endcode
@endif
   * would construct a new file URI, with path
   * <code>c:/test/test.xml</code>.
   *
   * @param uri a URI to be added to this object.
   *
   * @return the resulting new URI.
   */
  SBMLUri relativeTo(const std::string& uri) const;


private: 
  void parse(const std::string& uri);
private: 
  std::string mScheme;
  std::string mHost;
  std::string mPath;
  std::string mQuery;
  std::string mUri;
};


LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* SBMLUri_h */

