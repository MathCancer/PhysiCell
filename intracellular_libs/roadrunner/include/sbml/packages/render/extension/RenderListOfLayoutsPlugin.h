/**
 * @file    RenderListOfLayoutsPlugin.h
 * @brief   Definition of RenderListOfLayoutsPlugin, the plugin class of
 *          fbc package for the Model element.
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

#ifndef RenderListOfLayoutsPlugin_h
#define RenderListOfLayoutsPlugin_h


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/render/common/renderfwd.h>

#ifdef __cplusplus

#include <sbml/SBMLErrorLog.h>
#include <sbml/Model.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/extension/SBasePlugin.h>

#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/GlobalRenderInformation.h>
#include <sbml/packages/render/sbml/LocalRenderInformation.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderListOfLayoutsPlugin : public SBasePlugin
{
public:

  /**
   * Constructor
   */
  RenderListOfLayoutsPlugin  (const std::string &uri, const std::string &prefix,
                    RenderPkgNamespaces *groupsns);


  /**
   * Copy constructor. Creates a copy of this SBase object.
   */
  RenderListOfLayoutsPlugin (const RenderListOfLayoutsPlugin & orig);


  /**
   * Destroy this object.
   */
  virtual ~RenderListOfLayoutsPlugin  ();


  /**
   * Assignment operator for RenderListOfLayoutsPlugin .
   */
  RenderListOfLayoutsPlugin & operator=(const RenderListOfLayoutsPlugin & orig);


  /**
   * Creates and returns a deep copy of this RenderListOfLayoutsPlugin  object.
   * 
   * @return a (deep) copy of this RenderListOfLayoutsPlugin object
   */
  virtual RenderListOfLayoutsPlugin * clone () const;


  // --------------------------------------------------------
  //
  // overridden virtual functions for reading/writing/checking 
  // elements
  //
  // --------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */
  /**
   * Checks if this plugin object has all the required elements.
   *
   * Subclasses should override this function if they have their specific
   * elements.
   *
   * @return true if this pugin object has all the required elements,
   * otherwise false will be returned.
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */

  int appendFrom(const Model* model);

  /** @endcond */


  /** ------------------------------------------------------------------
   *
   *  Additional public functions
   *
   * ------------------------------------------------------------------
   */


  /**
   * Returns a pointer to the list object that contains local render information.
   */
  ListOfGlobalRenderInformation* getListOfGlobalRenderInformation();

  /**
   * Returns a const pointer to the list object that contains local render information.
   */
  const ListOfGlobalRenderInformation* getListOfGlobalRenderInformation() const;

  /**
   * Returns the number of local render information objects.
   */
  unsigned int getNumGlobalRenderInformationObjects() const;

  /**
   * Returns a pointer to the local render information object with the given
   * index.
   * If the index is invalid, @c NULL is returned.
   */
  GlobalRenderInformation* getRenderInformation(unsigned int index);

  /**
   * Returns a const pointer to the local render information object with the given
   * index.
   * If the index is invalid, @c NULL is returned.
   */
  const GlobalRenderInformation* getRenderInformation(unsigned int index) const;

  /**
   * Returns a pointer to the local render information object with the given
   * id.
   * If no object with the given @p id exists, @c NULL is returned.
   */
  GlobalRenderInformation* getRenderInformation(const std::string& id);

  /**
   * Returns a const pointer to the local render information object with the given
   * id.
   * If no object with the given @p id exists, @c NULL is returned.
   */
  const GlobalRenderInformation* getRenderInformation(const std::string& id) const;

  /**
   * Adds a copy of the given local render information object to the list of
   * local render information objects.
   * If an object with the same id exists, it is replaced.
   */
  void addGlobalRenderInformation(const GlobalRenderInformation* pLRI);

  /**
   * Creates a new local render information object and adds it to the list.
   * The created object does not have a id and it is the responsibility of
   * the calling code to ensure that it gets one.
   * For constraints on the id, please consult the render information document.
   */
  GlobalRenderInformation* createGlobalRenderInformation();

  /**
   * Removed the render information with the given index from the list.
   * The removed object is returned. It is the responsibility of the calling
   * code to delete the object.
   * If the index is not valid, @c NULL is returned.
   */
  GlobalRenderInformation* removeGlobalRenderInformation(unsigned int index); 

  /**
   * Removed the render information with the given @p id from the list.
   * The removed object is returned. It is the responsibility of the calling
   * code to delete the object.
   * If an object with the given @p id does not exist, @c NULL is returned.
   */
  GlobalRenderInformation* removeGlobalRenderInformation(const std::string& id); 


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Sets the value of the "annotation" subelement of this SBML object to a
   * copy of @p annotation.
   *
   * Any existing content of the "annotation" subelement is discarded.
   * Unless you have taken steps to first copy and reconstitute any
   * existing annotations into the @p annotation that is about to be
   * assigned, it is likely that performing such wholesale replacement is
   * unfriendly towards other software applications whose annotations are
   * discarded.  An alternative may be to use appendAnnotation().
   *
   * @param annotation an XML structure that is to be used as the content
   * of the "annotation" subelement of this object
   *
   * @see appendAnnotation(const XMLNode* annotation)
   */
  //virtual int setAnnotation (const XMLNode* annotation);


  /**
   * Sets the value of the "annotation" subelement of this SBML object to a
   * copy of @p annotation.
   *
   * Any existing content of the "annotation" subelement is discarded.
   * Unless you have taken steps to first copy and reconstitute any
   * existing annotations into the @p annotation that is about to be
   * assigned, it is likely that performing such wholesale replacement is
   * unfriendly towards other software applications whose annotations are
   * discarded.  An alternative may be to use appendAnnotation().
   *
   * @param annotation an XML string that is to be used as the content
   * of the "annotation" subelement of this object
   *
   * @see appendAnnotation(const std::string& annotation)
   */
  //virtual int setAnnotation (const std::string& annotation);


  /**
   * Appends annotation content to any existing content in the "annotation"
   * subelement of this object.
   *
   * The content in @p annotation is copied.  Unlike setAnnotation(), this
   * method allows other annotations to be preserved when an application
   * adds its own data.
   *
   * @param annotation an XML structure that is to be copied and appended
   * to the content of the "annotation" subelement of this object
   *
   * @see setAnnotation(const XMLNode* annotation)
   */
  //virtual int appendAnnotation (const XMLNode* annotation);


  /**
   * Appends annotation content to any existing content in the "annotation"
   * subelement of this object.
   *
   * The content in @p annotation is copied.  Unlike setAnnotation(), this 
   * method allows other annotations to be preserved when an application
   * adds its own data.
   *
   * @param annotation an XML string that is to be copied and appended
   * to the content of the "annotation" subelement of this object
   *
   * @see setAnnotation(const std::string& annotation)
   */
  //virtual int appendAnnotation (const std::string& annotation);




  // ---------------------------------------------------------
  //
  // virtual functions (internal implementation) which should
  // be overridden by subclasses.
  //
  // ---------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this plugin object.
   *
   * Subclasses which contain one or more SBase derived elements must
   * override this function.
   *
   * @param d the SBMLDocument object to use
   *
   * @see connectToParent
   * @see enablePackageInternal
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBML object of this plugin object to
   * this object and child elements (if any).
   * (Creates a child-parent relationship by this plugin object)
   *
   * This function is called when this object is created by
   * the parent element.
   * Subclasses must override this this function if they have one
   * or more child elements.Also, SBasePlugin::connectToParent()
   * must be called in the overridden function.
   *
   * @param sbase the SBase object to use
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToParent (SBase *sbase);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Enables/Disables the given package with child elements in this plugin
   * object (if any).
   * (This is an internal implementation invoked from
   *  SBase::enablePakcageInternal() function)
   *
   * @note Subclasses in which one or more SBase derived elements are
   * defined must override this function.
   *
   * @see setSBMLDocument
   * @see connectToParent
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond */

  void parseAnnotation();

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
  /*-- data members --*/

  /** @cond doxygenLibsbmlInternal */
  ListOfGlobalRenderInformation mGlobalRenderInformation; 
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Synchronizes the annotation of this SBML object.
   *
   * Annotation element (XMLNode* mAnnotation) is synchronized with the
   * current CVTerm objects (List* mCVTerm), ModelHistory object 
   * (ModelHistory* mHistory) and ListOfLayouts object (ListOfLayouts mLayouts).
   * Currently, this method is called in getAnnotation, isSetAnnotation,
   * and writeElements methods.
   */
  virtual void syncAnnotation(SBase* parentObject, XMLNode *annotation);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /** 
   * Parse L2 annotation if supported
   *
   */
  virtual void parseAnnotation(SBase *parentObject, XMLNode *annotation);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read (and store) XHTML,
   * MathML, etc. directly from the XMLInputStream.
   *
   * @return true if the subclass read from the stream, false otherwise.
   */
  virtual bool readOtherXML (SBase* parentObject, XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses must override this method to create, store, and then
   * return an SBML object corresponding to the next XMLToken in the
   * XMLInputStream if they have their specific elements.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Serialize the render information as L2 annotation
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


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
#endif  /* RenderListOfLayoutsPlugin_h */
