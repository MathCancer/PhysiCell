/**
 * @file    RenderExtension.h
 * @brief   Definition of RenderExtension, the core module of the render package.
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

#ifndef RenderExtension_h
#define RenderExtension_h

#include <sbml/common/extern.h>

#ifdef __cplusplus

#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/extension/SBMLExtensionRegister.h>

#ifndef RENDER_CREATE_NS
#define RENDER_CREATE_NS(variable,sbmlns)\
  EXTENSION_CREATE_NS(RenderPkgNamespaces,variable,sbmlns);
#endif

#include <vector>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderExtension : public SBMLExtension
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
  static const std::string&  getXmlnsL2();

  //
  // Other URI needed in this package (if any)
  //

  //---------------------------------------------------------------


  /**
   * Constructor
   */
  RenderExtension ();


  /**
   * Copy constructor.
   */
  RenderExtension(const RenderExtension&);


  /**
   * Destroy this object.
   */
  virtual ~RenderExtension ();


  /**
   * Assignment operator for GroupsExtension.
   */
  RenderExtension& operator=(const RenderExtension&);


  /**
   * Creates and returns a deep copy of this RenderExtension object.
   * 
   * @return a (deep) copy of this RenderExtension object
   */
  virtual RenderExtension* clone () const;


  /**
   * Returns the name of this package ("fbc")
   *
   * @pram the name of this package ("fbc")
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
  virtual const std::string& getURI(unsigned int sbmlLevel, unsigned int sbmlVersion, 
                                    unsigned int pkgVersion) const;


  /**
   * Returns the SBML level with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of layout package
   *
   * @return the SBML level with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   *
   */
  virtual unsigned int getLevel(const std::string &uri) const;


  /**
   * Returns the SBML version with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of layout package
   *
   * @return the SBML version with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   */
  virtual unsigned int getVersion(const std::string &uri) const;


  /**
   * Returns the package version with the given URI of this package.
   *
   * @param uri the string of URI that represents one of versions of layout package
   *
   * @return the package version with the given URI of this package. 0 will be returned
   * if the given URI is invalid.
   */
  virtual unsigned int getPackageVersion(const std::string &uri) const;


  /**
   * Returns an SBMLExtensionNamespaces<GroupsExtension> object whose alias type is 
   * LayoutPkgNamespace.
   * Null will be returned if the given uri is not defined in the layout package.
   *
   * @param uri the string of URI that represents one of versions of layout package
   *
   * @return an LayoutPkgNamespace object corresponding to the given uri. NULL will
   * be returned if the given URI is not defined in layout package.
   */
  virtual SBMLNamespaces* getSBMLExtensionNamespaces(const std::string &uri) const;


  /**
   * This method takes a type code of groups package and returns a string representing 
   * the code.
   */
  virtual const char* getStringFromTypeCode(int typeCode) const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Initializes layout extension by creating an object of this class with 
   * required SBasePlugin derived objects and registering the object 
   * to the SBMLExtensionRegistry class.
   *
   * (NOTE) This function is automatically invoked when creating the following
   *        global object in GroupsExtension.cpp
   *
   *        static SBMLExtensionRegister<GroupsExtension> groupsExtensionRegistry;
   *
   */

  static void init();

  /** @endcond */

  /**
   * Removes the L2 Namespace from a document. 
   *
   * This method should be overridden by all extensions that want to serialize
   * to an L2 annotation.
   */
  virtual void removeL2Namespaces(XMLNamespaces* xmlns)  const;

  
  /**
   * adds all L2 Extension namespaces to the namespace list. 
   * 
   * This method should be overridden by all extensions that want to serialize
   * to an L2 annotation.
   */
  virtual void addL2Namespaces(XMLNamespaces *xmlns) const;

  /**
   * Adds the L2 Namespace to the document and enables the extension.
   *
   * If the extension supports serialization to SBML L2 Annotations, this 
   * method should be overrridden, so it will be activated.
   */
  virtual void enableL2NamespaceForDocument(SBMLDocument* doc)  const;

  /** 
   * Determines whether this extension is being used by the given SBMLDocument
   *
   * The implementation returns true if the list of layouts contains a global render information,
   * or a layout object contains a local render information object. 
   * 
   * @param doc the sbml document to test. 
   * 
   * @return a boolean indicating whether the extension is actually being used
   *         byy the document. 
   */
  virtual bool isInUse(SBMLDocument *doc) const;

};



//
// (NOTE) 
//
// SBMLExtensionNamespaces<RenderExtension> must be instantiated
// in RenderExtension.cpp for DLL.
//
typedef SBMLExtensionNamespaces<RenderExtension> RenderPkgNamespaces; 

typedef enum
{
    SBML_RENDER_COLORDEFINITION = 1000
  , SBML_RENDER_ELLIPSE = 1001
  , SBML_RENDER_GLOBALRENDERINFORMATION = 1002
  , SBML_RENDER_GLOBALSTYLE = 1003
  , SBML_RENDER_GRADIENTDEFINITION = 1004
  , SBML_RENDER_GRADIENT_STOP = 1005
  , SBML_RENDER_GROUP = 1006
  , SBML_RENDER_IMAGE = 1007
  , SBML_RENDER_LINEENDING = 1008
  , SBML_RENDER_LINEARGRADIENT = 1009
  , SBML_RENDER_LINESEGMENT = 1010
  , SBML_RENDER_LISTOFGLOBALSTYLES = 1011
  , SBML_RENDER_LISTOFLOCALSTYLES = 1012
  , SBML_RENDER_LOCALRENDERINFORMATION = 1013
  , SBML_RENDER_LOCALSTYLE = 1014
  , SBML_RENDER_POLYGON = 1015
  , SBML_RENDER_RADIALGRADIENT = 1016
  , SBML_RENDER_RECTANGLE = 1017
  , SBML_RENDER_RELABSVECTOR = 1018
  , SBML_RENDER_CUBICBEZIER = 1019
  , SBML_RENDER_CURVE = 1020
  , SBML_RENDER_POINT = 1021
  , SBML_RENDER_TEXT = 1022
  , SBML_RENDER_TRANSFORMATION2D = 1023
} SBMLRenderTypeCode_t;


LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* RenderExtension_h */
