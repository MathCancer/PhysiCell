/**
 * @file    FbcExtension.h
 * @brief   Definition of FbcExtension, the core module of fbc package.
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
 *
 * @class FbcExtension
 * @sbmlbrief{fbc} Base extension class for the &ldquo;fbc&rdquo; package.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * @class FbcPkgNamespaces
 * @sbmlbrief{fbc} SBMLNamespaces extension for the &ldquo;fbc&rdquo; package.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * There are currently two possible namespaces defined for the Flux Balance 
 * Constraints package: 
 * "http://www.sbml.org/sbml/level3/version1/fbc/version1" and 
 * "http://www.sbml.org/sbml/level3/version1/fbc/version2".  Despite both
 * referencing SBML Level&nbsp;3 Version&nbsp;1 explicitly, both versions of 
 * this package (and all such packages) can be used without change in SBML 
 * Level&nbsp;3 Version&nbsp;2 documents.  The only caveat is that features of 
 * the SBML Level&nbsp;3 Version&nbsp;2 specification that were not present in 
 * Level&nbsp;1 may not be used by constructs from the Flux Balance Constraints
 * package.
 */


#ifndef FbcExtension_H__
#define FbcExtension_H__


#include <sbml/common/extern.h>
#include <sbml/SBMLTypeCodes.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/extension/SBMLExtensionRegister.h>


#ifndef FBC_CREATE_NS
  #define FBC_CREATE_NS(variable, sbmlns)\
    EXTENSION_CREATE_NS(FbcPkgNamespaces, variable, sbmlns);
#endif

#ifndef FBC_CREATE_NS_WITH_VERSION
  #define FBC_CREATE_NS_WITH_VERSION(variable, sbmlns, version)\
    EXTENSION_CREATE_NS_WITH_VERSION(FbcPkgNamespaces, variable, sbmlns, version);
#endif

#include <vector>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FbcExtension : public SBMLExtension
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


  /**
   * Returns the XML namespace URI of the SBML Level&nbsp;3 package
   * implemented by this libSBML extension.
   *
   * @return the XML namespace as a string.
   *
   * @copydetails doc_note_static_methods
   */
  static const std::string&  getXmlnsL3V1V2();
  //
  // Other URI needed in this package (if any)
  //
  //---------------------------------------------------------------


  /**
   * Creates a new FbcExtension instance.
   */
  FbcExtension();


  /**
   * Copy constructor for FbcExtension.
   *
   * @param orig the FbcExtension instance to copy.
   */
  FbcExtension(const FbcExtension& orig);


   /**
   * Assignment operator for FbcExtension.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcExtension& operator=(const FbcExtension& rhs);


   /**
   * Creates and returns a deep copy of this FbcExtension object.
   *
   * @return a (deep) copy of this FbcExtension object.
   */
  virtual FbcExtension* clone () const;


   /**
   * Destructor for FbcExtension.
   */
  virtual ~FbcExtension();


   /**
   * Returns the name of this SBML Level&nbsp;3 package ("fbc").
   *
   * @return a string representing the name of this package ("fbc").
   */
  virtual const std::string& getName() const;


  /**
   * Returns a string representing the SBML XML namespace of this
   * SBML Level&nbsp;3 package.
   *
   * The namespace URI constructed by this method corresponds to the
   * combination of the Level and Version of SBML, and the Version of the
   * SBML Level&nbsp;3 package.  (At the time of this writing, the only SBML
   * Level that supports packages is Level&nbsp;3, so the value of
   * @p sbmlLevel must necessarily always be <code>3</code>.)
   *
   * @param sbmlLevel the level of SBML.
   * @param sbmlVersion the version of SBML.
   * @param pkgVersion the version of package.
   *
   * @return a string representing the package URI, or an empty string if no
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
   * Returns an FbcPkgNamespaces object.
   *
   * @param uri a URI that represents one of the valid versions of the
   * &ldquo;fbc&rdquo; package.
   *
   * @return an FbcPkgNamespace object corresponding to the given @p uri, or
   * @c NULL if the URI is not defined in the &ldquo;fbc&rdquo; package.
   */
  virtual SBMLNamespaces* getSBMLExtensionNamespaces(const std::string &uri) const;


  /**
   * Takes a type code of the &ldquo;fbc&rdquo; package and returns a string
   * describing the code.
   *
   * @param typeCode a libSBML type code defined by the libSBML extension
   * implementing support for the SBML Level&nbsp;3 &ldquo;fbc&rdquo; package.
   *
   * @return a text string representing the type code given by @p typeCode.
   * If the type code is unrecognized for this implementation of the libSBML
   * &ldquo;fbc&rdquo; package, the string returned will be
   * <code>"(Unknown SBML Fbc Type)"</code>.
   */
  virtual const char* getStringFromTypeCode(int typeCode) const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Initializes fbc extension by creating an object of this class with 
   * required SBasePlugin derived objects and registering the object 
   * to the SBMLExtensionRegistry class.
   *
   * (NOTE) This function is automatically invoked when creating the following
   *        global object in FbcExtension.cpp
   *
   *        static SBMLExtensionRegister<FbcExtension> fbcExtensionRegister;
   *
   */
  static void init();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the entry in the error table at this index. 
   *
   * @param index an unsigned intgere representing the index of the error in the FbcSBMLErrorTable.
   *
   * @return packageErrorTableEntry object in the FbcSBMLErrorTable corresponding to the index given.
   */
  //virtual packageErrorTableEntry getErrorTable(unsigned int index) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the entry in the error table at this index. 
   *
   * @param index an unsigned intgere representing the index of the error in the FbcSBMLErrorTable.
   *
   * @return packageErrorTableEntry object in the FbcSBMLErrorTable corresponding to the index given.
   */
  virtual packageErrorTableEntryV2 getErrorTableV2(unsigned int index) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the index in the error table with the given errorId. 
   *
   * @param errorId an unsigned intgere representing the errorId of the error in the FbcSBMLErrorTable.
   *
   * @return unsigned integer representing the index in the FbcSBMLErrorTable corresponding to the errorId given.
   */
  virtual unsigned int getErrorTableIndex(unsigned int errorId) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the offset for the errorId range for the fbc L3 package. 
   *
   * @return unsigned intege representing the  offset for errors FbcSBMLErrorTable.
   */
  virtual unsigned int getErrorIdOffset() const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual bool hasMultipleVersions() const;
  /** @endcond */


};


// --------------------------------------------------------------------
//
// Required typedef definitions
//
// FbcPkgNamespaces is derived from the SBMLNamespaces class and
// used when creating an object of SBase derived classes defined in
// fbc package.
//
// --------------------------------------------------------------------
//
// (NOTE)
//
// SBMLExtensionNamespaces<FbcExtension> must be instantiated
// in FbcExtension.cpp for DLL.
//
typedef SBMLExtensionNamespaces<FbcExtension> FbcPkgNamespaces;

/**
 * @enum  SBMLFbcTypeCode_t
 * @brief SBMLFbcTypeCode_t Enumeration of possible types in the libSBML
 * &ldquo;fbc&rdquo; package implementation.
 *
 * @copydetails doc_what_are_typecodes
 *
 * @copydetails doc_additional_typecode_details
 */
typedef enum
{
    SBML_FBC_V1ASSOCIATION          = 800 /*!< Association (only used in Version&nbsp;1; replaced in Version&nbsp;2 with FbcAssociation) */
  , SBML_FBC_FLUXBOUND              = 801 /*!< FluxBound */
  , SBML_FBC_FLUXOBJECTIVE          = 802 /*!< FluxObjective */
  , SBML_FBC_GENEASSOCIATION        = 803 /*!< GeneAssociation (only used in Version&nbsp;1; replaced in Version&nbsp;2 with GeneProductAssociation)*/
  , SBML_FBC_OBJECTIVE              = 804 /*!< Objective */
  , SBML_FBC_ASSOCIATION            = 805 /*!< FbcAssociation */
  , SBML_FBC_GENEPRODUCTASSOCIATION = 806 /*!< GeneProductAssociation */
  , SBML_FBC_GENEPRODUCT            = 807 /*!< GeneProduct */
  , SBML_FBC_GENEPRODUCTREF         = 808 /*!< GeneProductRef */
  , SBML_FBC_AND                    = 809 /*!< FbcAnd */
  , SBML_FBC_OR                     = 810 /*!< FbcOr */
} SBMLFbcTypeCode_t;


LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* FbcExtension_H__ */


