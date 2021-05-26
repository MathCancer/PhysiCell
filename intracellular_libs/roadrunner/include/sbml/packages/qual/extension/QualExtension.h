/**
 * @file    QualExtension.h
 * @brief   Definition of QualExtension, the core module of qual package. 
 * @author  Akiya Jouraku
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
 * @class QualExtension
 * @sbmlbrief{qual} Base extension class for the package.
 *
 * @class QualPkgNamespaces
 * @sbmlbrief{qual} SBMLNamespaces extension for the package.
 *
 * There is currently one possible namespace defined for the Qualitative
 * Modeling package: 
 * "http://www.sbml.org/sbml/level3/version1/qual/version1".  Despite 
 * referencing SBML Level&nbsp;3 Version&nbsp;1 explicitly, this package 
 * (and all such packages) can be used without change in SBML 
 * Level&nbsp;3 Version&nbsp;2 documents.  The only caveat is that features of 
 * the SBML Level&nbsp;3 Version&nbsp;2 specification that were not present in 
 * Level&nbsp;1 may not be used by constructs from the Qualitative Modeling
 * package.
 */

#ifndef QualExtension_h
#define QualExtension_h

#include <sbml/common/extern.h>
#include <sbml/SBMLTypeCodes.h>

#ifdef __cplusplus

#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/extension/SBMLExtensionRegister.h>


#ifndef QUAL_CREATE_NS
#define QUAL_CREATE_NS(variable,sbmlns)\
  EXTENSION_CREATE_NS(QualPkgNamespaces,variable,sbmlns);
#endif

#include <vector>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN QualExtension : public SBMLExtension
{
public:

  //---------------------------------------------------------------
  //
  // Required class methods
  //
  //---------------------------------------------------------------

  /**
   * Returns the nickname of the SBML Level&nbsp;3 package implemented by
   * this libSBML extension.
   *
   * @return the package nickname, as a string.
   *
   * @copydetails doc_note_static_methods
   */
  static const std::string& getPackageName ();


  /**
   * Returns the default SBML Level used by this libSBML package extension.
   *
   * @return the SBML Level.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultLevel();


  /**
   * Returns the default SBML Version used by this libSBML package extension.
   *
   * @return the Version within the default SBML Level.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultVersion();


  /**
   * Returns the default version of the SBML Level&nbsp;3 package implemented
   * by this libSBML extension.
   *
   * @return the default version number of the SBML Level&nbsp;3 package
   * definition.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultPackageVersion();


  /**
   * Returns the XML namespace URI of the SBML Level&nbsp;3 package
   * implemented by this libSBML extension.
   *
   * @return the XML namespace as a string.
   *
   * @copydetails doc_note_static_methods
   */
  static const std::string&  getXmlnsL3V1V1();


  //
  // Other URI needed in this package (if any)
  //

  //---------------------------------------------------------------


  /**
   * Creates a new QualExtension instance.
   */
  QualExtension ();


  /**
   * Copy constructor.
   *
   * @param orig the instance to copy.
   */
  QualExtension(const QualExtension& orig);


  /**
   * Destroy this object.
   */
  virtual ~QualExtension ();


  /**
   * Assignment operator for QualExtension.
   */
  QualExtension& operator=(const QualExtension& rhs);


  /**
   * Creates and returns a deep copy of this QualExtension object.
   * 
   * @return a (deep) copy of this SBase object.
   */
  virtual QualExtension* clone () const;


  /**
   * Returns the name of this package ("qual")
   *
   * @return a string representing the name of this package ("qual").
   */
  virtual const std::string& getName() const;


  /**
   * Returns a string representing the SBML XML namespace of this SBML
   * Level&nbsp;3 package.
   *
   * The namespace URI constructed by this method corresponds to the
   * combination of the Level and Version of SBML, and the Version of the SBML
   * Level&nbsp;3 package. (At the time of this writing, the only SBML Level
   * that supports packages is Level&nbsp;3, so the value of @p sbmlLevel must
   * necessarily always be <code>3</code>.)
   *
   * @param sbmlLevel the level of SBML.
   * @param sbmlVersion the version of SBML.
   * @param pkgVersion the version of package.
   *
   * @return a string of the package URI, or an empty string if no
   * corresponding URI exists.
   */
  virtual const std::string& getURI(unsigned int sbmlLevel,
                                    unsigned int sbmlVersion,
                                    unsigned int pkgVersion) const;


  /**
   * Returns the SBML Level for the given URI of this package.
   *
   * @param uri a URI that represents a version of this package.
   *
   * @return the SBML Level for the given URI of this package, or @c 0 if the
   * given URI is invalid, or for a different package.
   */
  virtual unsigned int getLevel(const std::string &uri) const;


  /**
   * Returns the Version within the SBML Level for the given URI of this
   * package.
   *
   * @param uri a URI that represents a version of this package.
   *
   * @return the SBML Version within the SBML Level for the given URI of this
   * package, or @c 0 if the given URI is invalid, or for a different package.
   */
  virtual unsigned int getVersion(const std::string &uri) const;


  /**
   * Returns the SBML Level&nbsp;3 package version for the given URI of this
   * package.
   *
   * @param uri a URI that represents one of the valid versions of this
   * package.
   *
   * @return the version of the SBML Level&nbsp;3 package with the given URI,
   * or @c 0 if the given URI is invalid, or for a different package.
   */
  virtual unsigned int getPackageVersion(const std::string &uri) const;


  /**
   * Returns an QualPkgNamespaces object.
   *
   * @param uri a URI that represents one of the valid versions of the
   * &ldquo;qual&rdquo; package.
   *
   * @return an QualPkgNamespace object corresponding to the given @p uri, or
   * @c NULL if the URI is not defined in the Qual
   * package.
   */
  virtual SBMLNamespaces* getSBMLExtensionNamespaces(const std::string &uri) const;


  /**
   * Takes a type code of the &ldquo;qual&rdquo; package and returns a string
   * describing the code.
   *
   * @param typeCode a libSBML type code defined by the libSBML extension
   * implementing support for the SBML Level&nbsp;3 &ldquo;qual&rdquo; package.
   *
   * @return a text string representing the type code given by @p typeCode.
   * If the type code is unrecognized for this implementation of the libSBML
   * &ldquo;qual&rdquo; package, the string returned will be
   * <code>"(Unknown SBML Qual Type)"</code>.
   */
  virtual const char* getStringFromTypeCode(int typeCode) const;


  /** @cond doxygenLibsbmlInternal */
  
  /**
   * Initializes qual extension by creating an object of this class with 
   * required SBasePlugin derived objects and registering the object 
   * to the SBMLExtensionRegistry class.
   *
   * (NOTE) This function is automatically invoked when creating the following
   *        global object in QualExtension.cpp
   *
   *        static SBMLExtensionRegister<QualExtension> qualExtensionRegistry;
   *
   */
  static void init();

  /** @endcond */
  
  
  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the entry in the error table at this index. 
   *
   * @param index an unsigned intgere representing the index of the error in the QualSBMLErrorTable.
   *
   * @return packageErrorTableEntry object in the QualSBMLErrorTable corresponding to the index given.
   */
  virtual packageErrorTableEntry getErrorTable(unsigned int index) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the index in the error table with the given errorId. 
   *
   * @param errorId an unsigned intgere representing the errorId of the error in the QualSBMLErrorTable.
   *
   * @return unsigned integer representing the index in the QualSBMLErrorTable corresponding to the errorId given.
   */
  virtual unsigned int getErrorTableIndex(unsigned int errorId) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the offset for the errorId range for the qual L3 package. 
   *
   * @return unsigned intege representing the  offset for errors QualSBMLErrorTable.
   */
  virtual unsigned int getErrorIdOffset() const;


  /** @endcond */

};


// --------------------------------------------------------------------
//
// Required typedef definitions 
//
// QualPkgNamespaces is derived from the SBMLNamespaces class and
// used when creating an object of SBase derived classes defined in
// qual package.
//
// --------------------------------------------------------------------

//
// (NOTE) 
//
// SBMLExtensionNamespaces<QualExtension> must be instantiated
// in QualExtension.cpp for DLL.
//
typedef SBMLExtensionNamespaces<QualExtension> QualPkgNamespaces; 

#endif  /* __cplusplus */

/**
 * @enum  SBMLQualTypeCode_t
 * @brief SBMLFbcTypeCode_t Enumeration of possible types in the libSBML
 * &ldquo;qual&rdquo; package implementation.
 *
 * @copydetails doc_what_are_typecodes
 *
 * @copydetails doc_additional_typecode_details
 */
typedef enum
{
   SBML_QUAL_QUALITATIVE_SPECIES  = 1100 /*!< QualitativeSpecies */
 , SBML_QUAL_TRANSITION           = 1101 /*!< Transition */
 , SBML_QUAL_INPUT                = 1102 /*!< Input */
 , SBML_QUAL_OUTPUT               = 1103 /*!< Output */
 , SBML_QUAL_FUNCTION_TERM        = 1104 /*!< FunctionTerm */
 , SBML_QUAL_DEFAULT_TERM         = 1105 /*!< DefaultTerm */
} SBMLQualTypeCode_t;


LIBSBML_CPP_NAMESPACE_END

#endif  /* QualExtension_h */
