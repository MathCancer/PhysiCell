/**
 * @file:   DistribExtension.h
 * @brief:  Implementation of the DistribExtension class
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
 */


#ifndef DistribExtension_H__
#define DistribExtension_H__


#include <sbml/common/extern.h>
#include <sbml/SBMLTypeCodes.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/extension/SBMLExtensionRegister.h>


#ifndef DISTRIB_CREATE_NS
  #define DISTRIB_CREATE_NS(variable, sbmlns)\
    EXTENSION_CREATE_NS(DistribPkgNamespaces, variable, sbmlns);
#endif


#include <vector>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DistribExtension : public SBMLExtension
{
public:

  //---------------------------------------------------------------
  //
  // Required class methods
  //
  //---------------------------------------------------------------

  /**
   * Returns the package name of this extension.
   */
  static const std::string& getPackageName ();


  /**
   * Returns the default SBML Level this extension.
   */
  static unsigned int getDefaultLevel();


  /**
   * Returns the default SBML Version this extension.
   */
  static unsigned int getDefaultVersion();


  /**
   * Returns the default SBML version this extension.
   */
  static unsigned int getDefaultPackageVersion();


  /**
   * Returns URI of supported versions of this package.
   */
  static const std::string&  getXmlnsL3V1V1();


  //
  // Other URI needed in this package (if any)
  //
  //---------------------------------------------------------------


  /**
   * Creates a new DistribExtension   */
  DistribExtension();


  /**
   * Copy constructor for DistribExtension.
   *
   * @param orig; the DistribExtension instance to copy.
   */
  DistribExtension(const DistribExtension& orig);


   /**
   * Assignment operator for DistribExtension.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DistribExtension& operator=(const DistribExtension& rhs);


   /**
   * Creates and returns a deep copy of this DistribExtension object.
   *
   * @return a (deep) copy of this DistribExtension object.
   */
  virtual DistribExtension* clone () const;


   /**
   * Destructor for DistribExtension.
   */
  virtual ~DistribExtension();


   /**
   * Returns the name of this package ("distrib")
   *
   * @return a string representing the name of this package ("distrib")
   */
  virtual const std::string& getName() const;


  /**
   * Returns the URI (namespace) of the package corresponding to the combination of 
   * the given sbml level, sbml version, and package version.
   * Empty string will be returned if no corresponding URI exists.
   *
   * @param sbmlLevel the level of SBML
   * @param sbmlVersion the version of SBML
   * @param pkgVersion the version of package
   *
   * @return a string of the package URI
   */
  virtual const std::string& getURI(unsigned int sbmlLevel,
                                    unsigned int sbmlVersion,
                                    unsigned int pkgVersion) const;


  /**
   * Returns the SBML level with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of distrib package
   *
   * @return the SBML level with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   *
   */
  virtual unsigned int getLevel(const std::string &uri) const;


  /**
   * Returns the SBML version with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of distrib package
   *
   * @return the SBML version with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   *
   */
  virtual unsigned int getVersion(const std::string &uri) const;


  /**
   * Returns the package version with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of distrib package
   *
   * @return the package version with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   *
   */
  virtual unsigned int getPackageVersion(const std::string &uri) const;


  /**
   * Returns an SBMLExtensionNamespaces<DistribExtension> object whose alias type is 
   * DistribPkgNamespace.
   * Null will be returned if the given uri is not defined in the distrib package.
   *
   * @param uri the string of URI that represents one of versions of distrib package
   *
   * @return an DistribPkgNamespace object corresponding to the given uri. NULL will
   * be returned if the given URI is not defined in distrib package.
   */
  virtual SBMLNamespaces* getSBMLExtensionNamespaces(const std::string &uri) const;


  /**
   * This method takes a type code from the Distrib package and returns a string representing 
   * the code.
   */
  virtual const char* getStringFromTypeCode(int typeCode) const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Initializes distrib extension by creating an object of this class with 
   * required SBasePlugin derived objects and registering the object 
   * to the SBMLExtensionRegistry class.
   *
   * (NOTE) This function is automatically invoked when creating the following
   *        global object in DistribExtension.cpp
   *
   *        static SBMLExtensionRegister<DistribExtension> distribExtensionRegistry;
   *
   */
  static void init();


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Return the entry in the error table at this index. 
   *
   * @param index an unsigned intgere representing the index of the error in the DistribSBMLErrorTable
   *
   * @return packageErrorTableEntry object in the DistribSBMLErrorTable corresponding to the index given.
   */
  virtual packageErrorTableEntry getErrorTable(unsigned int index) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Return the index in the error table with the given errorId. 
   *
   * @param errorId an unsigned intgere representing the errorId of the error in the DistribSBMLErrorTable
   *
   * @return unsigned integer representing the index in the DistribSBMLErrorTable corresponding to the errorId given.
   */
  virtual unsigned int getErrorTableIndex(unsigned int errorId) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Return the offset for the errorId range for the distrib L3 package. 
   *
   * @return unsigned intege representing the  offset for errors DistribSBMLErrorTable.
   */
  virtual unsigned int getErrorIdOffset() const;


  /** @endcond doxygenLibsbmlInternal */


};


// --------------------------------------------------------------------
//
// Required typedef definitions
//
// DistribPkgNamespaces is derived from the SBMLNamespaces class and
// used when creating an object of SBase derived classes defined in
// distrib package.
//
// --------------------------------------------------------------------
//
// (NOTE)
//
// SBMLExtensionNamespaces<DistribExtension> must be instantiated
// in DistribExtension.cpp for DLL.
//
typedef SBMLExtensionNamespaces<DistribExtension> DistribPkgNamespaces;

typedef enum
{
    SBML_DISTRIB_DRAW_FROM_DISTRIBUTION  = 1300
  , SBML_DISTRIB_INPUT             = 1301
  , SBML_DISTRIB_UNCERTAINTY       = 1302
} SBMLDistribTypeCode_t;




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* DistribExtension_H__ */


