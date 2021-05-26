/**
 * @file    RenderGraphicalObjectPlugin.h
 * @brief   Definition of RenderGraphicalObjectPlugin, the plugin class of
 *          the render package for the GraphicalObject element.
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

#ifndef RenderGraphicalObjectPlugin_h
#define RenderGraphicalObjectPlugin_h


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/render/extension/RenderExtension.h>

#ifdef __cplusplus

#include <sbml/SBMLErrorLog.h>
#include <sbml/Model.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/extension/SBasePlugin.h>


LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderGraphicalObjectPlugin : public SBasePlugin
{
public:

  /**
   * Constructor
   */
  RenderGraphicalObjectPlugin  (const std::string &uri, const std::string &prefix,
                    RenderPkgNamespaces *groupsns);


  /**
   * Copy constructor. Creates a copy of this SBase object.
   */
  RenderGraphicalObjectPlugin (const RenderGraphicalObjectPlugin & orig);


  /**
   * Destroy this object.
   */
  virtual ~RenderGraphicalObjectPlugin  ();


  /**
   * Assignment operator for RenderGraphicalObjectPlugin .
   */
  RenderGraphicalObjectPlugin & operator=(const RenderGraphicalObjectPlugin & orig);


  /**
   * Creates and returns a deep copy of this RenderGraphicalObjectPlugin  object.
   * 
   * @return a (deep) copy of this RenderGraphicalObjectPlugin object
   */
  virtual RenderGraphicalObjectPlugin * clone () const;


  // --------------------------------------------------------
  //
  // overridden virtual functions for reading/writing/checking 
  // elements
  //
  // --------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Reads the attributes of corresponding package in SBMLDocument element.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Writes the attributes of corresponding package in SBMLDocument element.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */

  /** ------------------------------------------------------------------
   *
   *  Additional public functions
   *
   * ------------------------------------------------------------------
   */
  
    /**
     * Returns the object role string for the object.
     */
    const std::string& getObjectRole() const;

    /**
     * Sets the object role string for the object.
     */
    void setObjectRole(const std::string& role);

    /**
     * Returns whether the object role has been set or not.
     */
    bool isSetObjectRole() const;

  // ---------------------------------------------------------
  //
  // virtual functions (internal implementation) which should
  // be overridden by subclasses.
  //
  // ---------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;

  /** @endcond */
 

protected:
  /** @cond doxygenLibsbmlInternal */

  /*-- data members --*/
  std::string mObjectRole;   
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
#endif  /* RenderGraphicalObjectPlugin_h */
