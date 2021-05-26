/**
 * @file    CompBase.h
 * @brief   Definition of CompBase, the base class of extension entities
 *          plugged in SBase derived classes in the SBML Core package.
 * @author  Lucian Smith
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
 * Copyright 2011-2012 jointly by the following organizations:
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
 * @class CompBase
 * @sbmlbrief{comp} Convenience class for SBase-derived classes.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The CompBase class derives from SBase, and defines a few functions and
 * features common to all SBase-derived classes in the SBML Level&nbsp;3
 * @ref comp (&ldquo;comp&rdquo;) package.
 */

#ifndef CompBase_h
#define CompBase_h

#include <set>

#include <sbml/common/sbmlfwd.h>
#include <sbml/extension/SBMLExtension.h>
#include <sbml/extension/SBMLExtensionNamespaces.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/SBase.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN CompBase : public SBase
{
public:

  /**
   * Creates a new CompBase with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  CompBase(unsigned int level        = CompExtension::getDefaultLevel(), 
           unsigned int version      = CompExtension::getDefaultVersion(), 
           unsigned int pkgVersion   = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new CompBase with the given SBMLExtensionNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  CompBase(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the object to copy.
   */
  CompBase(const CompBase& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  CompBase& operator=(const CompBase& source);


  /**
   * Destructor.
   */
  virtual ~CompBase ();


  /**
   * Returns the XML namespace (URI) of the package extension
   * of this object.
   *
   * @return the URI of the package extension of this plugin object.
   */
  const std::string& getPackageURI() const;


  /**
   * Returns the package name of this plugin object.
   *
   * @return the package name of this plugin object.
   */
  const std::string& getPackageName() const;


  /**
   * Returns the package version of the package extension of
   * this plugin object.
   *
   * @return the package version of the package extension of
   * this plugin object.
   */
  unsigned int getPackageVersion() const;


  /**
   * Returns the Model object to which the referenced child object belongs.
   */
  static Model* getParentModel(SBase* child);


protected:

  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parent's implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "submodel" , mSubmodel );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /**
   * Helper to log a common type of error for elements.
   */
  virtual void logUnknownElement(const std::string &element );


  /**
   * Helper to log a common type of error.
   */
  virtual void logUnknownAttribute(const std::string &attribute,
                                   const std::string& element);


  /**
   * Helper to log a common type of error.
   */
  virtual void logEmptyString(const std::string &attribute,
                              const std::string& element);



  /**
   * Helper to log a common type of error.
   */
  virtual void logInvalidId(const std::string& attribute,
                            const std::string& wrongattribute,
                            const std::string& object = "");



  /**
   * Helper to log a common type of error.
   */
  virtual void logMissingAttribute(const std::string& attribute,
                                   const std::string& element);



  /**
   * Predicate returning @c true if this
   * object's level/version and namespace values correspond to a valid
   * SBML specification.
   *
   * The valid combination of SBML Level and Version, &ldquo;comp&rdquo; package version, and Namespace as of this
   * release of libSBML is the following:
   * <ul>
   * <li> Level&nbsp;3 Version&nbsp;1 Package&nbsp;Version&nbsp;1: <code>"http://www.sbml.org/sbml/level3/version1/comp/version1"</code>
   * </ul>
   *
   * @note  This function is provided as convenience method to be called from constructors. This 
   *        allows to use it in scenarios where the namespaces or typecode have not yet been initialized. 
   * 
   * @return @c true if the level, version and namespace values of this 
   * SBML object correspond to a valid set of values, @c false otherwise.
   */
  bool hasValidLevelVersionNamespaceCombination();


protected:
  /**
   * Remove the given SBase object, and any Ports that point to it.
   * A static function for removing elements--it is illegal to reference
   * an element from a port that has been deleted or replaced, but if 
   * it happens, we need to not actually crash.  This function finds and 
   * deletes all such invalid ports before deleting the object.
   */
  static int removeFromParentAndPorts(SBase* todelete, std::set<SBase*>* removed);
  friend class CompModelPlugin;


  /** @cond doxygenLibsbmlInternal */
  /**
   * DEPRECATED FUNCTION:  DO NOT USE
   *
   * Remove the given SBase object, and any Ports that point to it.  Unsafe,
   * because the program might later attempt to delete any removed Port.
   */
  int removeFromParentAndPorts(SBase* todelete);
  /** @endcond */

private:

  /** @cond doxygenLibsbmlInternal */
  //
  // An SBMLExtension derived object of corresponding package extension
  // The owner of this object is SBMLExtensionRegistry class.
  //
  const SBMLExtension  *mSBMLExt;
  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* CompBase_h */
