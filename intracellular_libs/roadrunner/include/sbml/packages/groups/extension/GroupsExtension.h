/**
 * @file GroupsExtension.h
 * @brief Definition of GroupsExtension.
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
 * @class GroupsExtension
 * @sbmlbrief{groups} Base extension class for the package.
 *
 * This is the Groups package extension of the SBMLExtension class that
 * every libSBML plug-in must implement in order to implement an
 * SBML Level&nbsp;3 package.
 *
 * @copydetails doc_what_is_sbmlextension
 *
 * @class GroupsPkgNamespaces
 * @sbmlbrief{groups} SBMLNamespaces extension for the package.
 */


#ifndef GroupsExtension_H__
#define GroupsExtension_H__


#include <sbml/common/extern.h>
#include <sbml/SBMLTypeCodes.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/extension/SBMLExtensionRegister.h>

#ifndef GROUPS_CREATE_NS
#define GROUPS_CREATE_NS(variable, sbmlns)\
EXTENSION_CREATE_NS(GroupsPkgNamespaces, variable, sbmlns);
#endif

#include <vector>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN GroupsExtension : public SBMLExtension
{
public:

  /**
   * Returns the nickname of the SBML Level&nbsp;3 package implemented by this
   * libSBML extension.
   *
   * @return the package nickname, as a string.
   *
   * @copydetails doc_note_static_methods
   */
  static const std::string& getPackageName();


  /**
   * Returns the default SBML Level implemented by this libSBML extension.
   *
   * @return the SBML Level, as an unsigned integer.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultLevel();


  /**
   * Returns the default SBML Version implemented by this libSBML extension.
   *
   * @return the Version within the default SBML Level, as an unsigned integer.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultVersion();


  /**
   * Returns the default version of the SBML Level&nbsp;3 package implemented
   * by this libSBML extension.
   *
   * @return the default version number of the SBML Level&nbsp;3 package
   * definition, as an unsigned integer.
   *
   * @copydetails doc_note_static_methods
   */
  static unsigned int getDefaultPackageVersion();


  /**
   * Returns the XML namespace URI of the SBML Level&nbsp;3 package implemented
   * by this libSBML extension.
   *
   * @return the XML namespace, as a string.
   *
   * @copydetails doc_note_static_methods
   */
  static const std::string& getXmlnsL3V1V1();


  /**
   * Creates a new GroupsExtension instance.
   */
  GroupsExtension();


  /**
   * Copy constructor for GroupsExtension.
   *
   * @param orig the GroupsExtension instance to copy.
   */
  GroupsExtension(const GroupsExtension& orig);


  /**
   * Assignment operator for GroupsExtension.
   *
   * @param rhs the GroupsExtension object whose values are to be used as the
   * basis of the assignment.
   */
  GroupsExtension& operator=(const GroupsExtension& rhs);


  /**
   * Creates and returns a deep copy of this GroupsExtension object.
   *
   * @return a (deep) copy of this GroupsExtension object.
   */
  virtual GroupsExtension* clone() const;


  /**
   * Destructor for GroupsExtension.
   */
  virtual ~GroupsExtension();


  /**
   * Returns the name of this SBML Level&nbsp;3 package ("groups").
   *
   * @return a string representing the name of this package ("groups").
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
   *
   * @param sbmlVersion the version of SBML.
   *
   * @param pkgVersion the version of this package.
   *
   * @return a string representing the name of this package ("groups").
   */
  virtual const std::string& getURI(unsigned int sbmlLevel,
                                    unsigned int sbmlVersion,
                                    unsigned int pkgVersion) const;


  /**
   * Returns the SBML Level for the given URI of this package.
   *
   * @param uri the string of the URI that represents one of the versions of
   * the "groups" package.
   *
   * @return the SBML Level for the given URI of this package, or @c 0 if the
   * given URI is invalid, or for a different package.
   */
  virtual unsigned int getLevel(const std::string& uri) const;


  /**
   * Returns the Version within the SBML Level for the given URI of this
   * package.
   *
   * @param uri the string of the URI that represents one of the versions of
   * the "groups" package.
   *
   * @return the SBML Version within the SBML Level for the given URI of this
   * package, or @c 0 if the given URI is invalid, or for a different package.
   */
  virtual unsigned int getVersion(const std::string& uri) const;


  /**
   * Returns the SBML Level&nbsp;3 package version for the given URI of this
   * package.
   *
   * @param uri the string of the URI that represents one of the versions of
   * the "groups" package.
   *
   * @return the version of the SBML Level&nbsp;3 package for the given URI of
   * this package, or @c 0 if the given URI is invalid, or for a different package.
   */
  virtual unsigned int getPackageVersion(const std::string& uri) const;


  /**
   * Returns a GroupsPkgNamespaces object.
   *
   * @param uri the string of the URI that represents one of the versions of
   * the "groups" package.
   *
   * @return GroupsPkgNamespaces object corresponding to the given URI of this
   * package, or @c NULL if the given URI is not defined in the "groups"
   * package.
   */
  virtual SBMLNamespaces* getSBMLExtensionNamespaces(const std::string& uri)
    const;


  /**
   * Returns a string describing the type code of the &ldquo;groups&rdquo;
   * package.
   *
   * @param typeCode a libSBML type code defined by the libSBML extension
   * implementing support for the SBML Level&nbsp;3 &ldquo;groups&rdquo;
   * package.
   *
   * @return a text string representing the type code given by @p typeCode. If
   * the type code is unrecognized for this implementation of the libSBML
   * &ldquo;groups&rdquo; package, the string returned will be <code>"(Unknown
   * SBML Groups Type)"</code>.
   */
  virtual const char* getStringFromTypeCode(int typeCode) const;



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the entry in the error table at this index.
   *
   * @param index an unsigned integer representing the index of the error.
   *
   * @return packageErrorTableEntry object in the GroupsSBMLErrorTable.
   */
  virtual packageErrorTableEntry getErrorTable(unsigned int index) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Return the index in the error table with the given errorId.
   *
   * @param errorId an unsigned integer representing the errorId of the error.
   *
   * @return unsigned int representing the index in the GroupsSBMLErrorTable
   * corresponding to the errorId given.
   */
  virtual unsigned int getErrorTableIndex(unsigned int errorId) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the offset for the errorId range for the "groups" package.
   *
   * @return unsigned int representing the offset for errors in the
   * GroupsSBMLErrorTable.
   */
  virtual unsigned int getErrorIdOffset() const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Initializes groups extension by creating an object of this class with the
   * required SBasePlugin derived objects and registering the object to the
   * SBMLExtensionRegistry class
   *
   * This function is automatically invoked when creatingthe following global
   * object in GroupsExtension.cpp
   *
   * static SBMLExtensionRegister<GroupsExtension> groupsExtensionRegistry;
   */
  static void init();

  /** @endcond */


};

/**
 *
 * Required typedef definitions
 *
 * GroupsPkgNamespace is derived from SBMLNamespaces class and used when
 * creating an object of SBase derived classes defined in the groups package
 *
 * SBMLExtensionNamespaces<GroupsExtension> must be instantiated in
 * GroupsExtension.cpp for DLL
 *
 */
typedef SBMLExtensionNamespaces<GroupsExtension> GroupsPkgNamespaces;


LIBSBML_CPP_NAMESPACE_END




#endif /* __cplusplus */




LIBSBML_CPP_NAMESPACE_BEGIN


/**
 * @enum SBMLGroupsTypeCode_t
 * @brief SBMLGroupsTypeCode_t Enumeration of possible types in the libSBML
 * &ldquo;groups&rdquo; package implementation.
 *
 * @copydetails doc_what_are_typecodes
 *
 * @copydetails doc_additional_typecode_details
 */
typedef enum
{
  SBML_GROUPS_MEMBER     =   500  /*!<Member */
, SBML_GROUPS_GROUP      =   501  /*!<Group */
} SBMLGroupsTypeCode_t;


/**
 * @enum GroupKind_t
 * @brief Enumeration of values permitted as the value of the "kind"
 * attribute on Group objects.
 *
 * @see Group::getKind()
 * @see Group::setKind(@if java long kind@endif)
 */
typedef enum
{
  GROUP_KIND_CLASSIFICATION       /*!< The group kind is @c "classification". */
, GROUP_KIND_PARTONOMY            /*!< The group kind is @c "partonomy". */
, GROUP_KIND_COLLECTION           /*!< The group kind is @c "collection". */
, GROUP_KIND_UNKNOWN              /*!< Invalid GroupKind value. */
} GroupKind_t;


/**
 */
LIBSBML_EXTERN
const char*
GroupKind_toString(GroupKind_t gk);


/**
 */
LIBSBML_EXTERN
GroupKind_t
GroupKind_fromString(const char* code);


/**
 */
LIBSBML_EXTERN
int
GroupKind_isValid(GroupKind_t gk);


/**
 */
LIBSBML_EXTERN
int
GroupKind_isValidString(const char* code);




LIBSBML_CPP_NAMESPACE_END




#endif /* !GroupsExtension_H__ */


