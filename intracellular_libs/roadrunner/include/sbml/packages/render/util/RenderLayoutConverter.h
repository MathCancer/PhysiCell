/**
 * @file    RenderLayoutConverter.h
 * @brief   Simple converter to convert Layout + Render from l2 to L3 and vice versa
 * @author  Frank T. Bergmann
 *
 *<!---------------------------------------------------------------------------
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
 *------------------------------------------------------------------------- -->
 */

#ifndef RenderLayoutConverter_h
#define RenderLayoutConverter_h

#include <sbml/common/extern.h>
#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN RenderLayoutConverter : public SBMLConverter
{
protected:
  /** @cond doxygenLibsbmlInternal */
  int targetLevel;
  int targetVersion;
  std::string renderNsUri;
  std::string layoutNsUri; 
  /** @endcond */

protected:
  int convertToL3();
  int convertToL2();

public:

  /** @cond doxygenLibsbmlInternal */
  
  /* register with the ConversionRegistry */
  static void init();  

  /** @endcond */


  /**
   * Constructor.
   */
  RenderLayoutConverter();


  /**
   * Copy constructor.
   */
  RenderLayoutConverter(const RenderLayoutConverter&);

  /**
   * Creates and returns a deep copy of this RenderLayoutConverter.
   * 
   * @return a (deep) copy of this RenderLayoutConverter.
   */
  virtual SBMLConverter* clone() const;


  /**
   * Destroy this RenderLayoutConverter object.
   */
  virtual ~RenderLayoutConverter ();


  /**
   * This function determines whether a given converter matches the 
   * configuration properties given. 
   * 
   * @param props the properties to match
   * 
   * @return <c>true</c> if this converter is a match, <c>false</c> otherwise.
   */
  virtual bool matchesProperties(const ConversionProperties &props) const;

  
  /** 
   * the actual conversion 
   * 
   * @return status code represeting success/failure/conversion impossible
   */
  virtual int convert();


  /**
   * Returns the default properties of this converter.
   * 
   * A given converter exposes one or more properties that can be adjusted
   * in order to influence the behavior of the converter.  This method
   * returns the @em default property settings for this converter.  It is
   * meant to be called in order to discover all the settings for the
   * converter object.
   *
   * The properties for the RenderLayoutConverter are:
   * "convert layout" - the name of this converter 
   *
   * @return the ConversionProperties object describing the default properties
   * for this converter.
   */
  virtual ConversionProperties getDefaultProperties() const;


};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

  
#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */


#endif
