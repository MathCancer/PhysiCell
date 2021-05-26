/**
 * @file    RenderInformationBase.h
 * @brief   abstract base class for render information 
 * @author  Ralph Gauges
 * @author  Frank T. Bergmann
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
 * Copyright (C) 2011-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright 2010 Ralph Gauges
 *     Group for the modeling of biological processes 
 *     University of Heidelberg
 *     Im Neuenheimer Feld 267
 *     69120 Heidelberg
 *     Germany
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class RenderInformationBase
 * @brief abstract base class for local and global render information.
 *
 * In the SBML render extension, local and global render information representations
 * share many attributes. These are implemented in this abstract base class.
 *
 * GlobalRenderInformation and LocalRenderInformation are the classes that are derived
 * from this base class.
 *
 * All render information objects have the following things in common:
 *
 * a) a set of color definitions
 * b) a set of gradient definitions
 * c) a set of line endings
 *
 * In addition to those, they share attributes for background color and some meta information
 * as to which program created the render information etc.
 */

#ifndef RenderInformationBase_H__
#define RenderInformationBase_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/SBase.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/ColorDefinition.h>
#include <sbml/packages/render/sbml/GradientBase.h>
#include <sbml/packages/render/sbml/LinearGradient.h>
#include <sbml/packages/render/sbml/RadialGradient.h>
#include <sbml/packages/render/sbml/LineEnding.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN RenderInformationBase:public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId;
////  std::string mName;
  std::string mProgramName;
  std::string mProgramVersion;
  std::string mReferenceRenderInformation;
  std::string mBackgroundColor;
  ListOfColorDefinitions mListOfColorDefinitions;
  ListOfGradientDefinitions mListOfGradientDefinitions;
  ListOfLineEndings mListOfLineEndings;
  /** @endcond */

protected:
  /**
   * Creates a new RenderInformationBase object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RenderInformationBase (unsigned int level      = RenderExtension::getDefaultLevel(),
                         unsigned int version    = RenderExtension::getDefaultVersion(),
                         unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RenderInformationBase object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RenderInformationBase (RenderPkgNamespaces* renderns);


  /**
   * Parses the xml information in the given node and sets the attributes.
   * This method should never be called by the user. It is only used to read render 
   * information from annotations.
   *
   * @param node the XMLNode object reference that describes the RenderInfromationBase
   * object to be instantiated.
   */
  void parseXML(const XMLNode& pNode); 



#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a RenderInformationBase object
   * empty color definition, gradient definition
   * and line endings set.
   * For the object to be valid a valid background color value.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  RenderInformationBase(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * attributes
   */
  virtual bool hasRequiredAttributes() const ;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * elements
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond */



public:

  /**
   * Destroy this RenderInformationBase object.
   */
  virtual ~RenderInformationBase ();



  /**
   * Returns the program name that created the render information.
   *
   * @return the name string of the program
   */
  const std::string& getProgramName() const;

  /**
   * Sets the name of the program that created the render information.
   *
   * @param name the name of the programm
   */
  void setProgramName(const std::string& name);

  /**
   * Returns the version of the program that created the render information.
   *
   * @return the version of the program as a string.
   */
  const std::string& getProgramVersion() const;

  /**
   * Sets the version string of the program that created the render information.
   *
   * @param version version string of the program
   */
  void setProgramVersion(const std::string& version);

  /**
   * Returns the id of the referenced render information object.
   * RenderInfromation objects can reference other render information objects
   * and information that is not found in the current render information is then
   * expected to be in the referenced render information object.
   *
   * Global render information objects can only reference other global 
   * render information objects, local render information objects can reference other local
   * render information objects from the same list of local render information or other
   * global render infromation.
   *
   * @return the id of the referenced render infromation object.
   */
  const std::string& getReferenceRenderInformationId() const;

  /**
   * Sets the id of the referenced render information object.
   * The user has to make sure that render infromation referencing 
   * does not create loops.
   *
   * @param id the id of the referenced render infromation
   */
  void setReferenceRenderInformationId(const std::string& id);

  /**
   * Returns the number of color definitions.
   *
   * @return the number of color definitions in the render information.
   */
  unsigned int getNumColorDefinitions() const;

  /**
   * Returns a pointer to the list of color definitions.
   *
   * @return pointer to the list of color definitions.
   */
  ListOfColorDefinitions* getListOfColorDefinitions();

  /**
   * Returns a const pointer to the list of color definitions.
   *
   * @return const pointer to the list of color definitions.
   */
  const ListOfColorDefinitions* getListOfColorDefinitions() const;

  /**
   * Returns a pointer to the color definition with the given index, or @c NULL
   * if the index is invalid.
   * 
   * @param index of the ColorDefinition object to be returned
   *
   * @return pointer to the ColorDefinition object at the given index or NULL
   */
  ColorDefinition* getColorDefinition(unsigned int index);

  /**
   * Returns a const pointer to the color definition with the given index, or @c NULL
   *if the index is invalid.
   * 
   * @param index of the ColorDefinition object to be returned
   *
   * @return const pointer to the ColorDefinition object at the given index or NULL
   */
  const ColorDefinition* getColorDefinition(unsigned int index) const;

  /**
   * Returns a pointer to the color definition with the given @p id, or @c NULL
   * if there is no color definition with that id.
   *
   * @param id of the color definition object to be returned.
   *
   * @return a pointer to the color definition object with the given @p id 
   * or NULL if there is no color definition with given @p id
   */
  ColorDefinition* getColorDefinition(const std::string& id);

  /**
   * Returns a const pointer to the color definition with the given @p id, or @c NULL
   * if there is no color definition with that id.
   *
   * @param id of the color definition object to be returned.
   *
   * @return a const pointer to the color definition object with the given @p id 
   * or NULL if there is no color definition with given @p id
   */
  const ColorDefinition* getColorDefinition(const std::string& id) const;

  /**
   * Creates a new color definition object without an id.
   * For the object to be valid an id has to be set that is unique
   * within the list of color definitions and the list of gradients within 
   * the render information.
   *
   * The created ColorDefinition object is added to and owned by the render information.
   *
   * @return pointer to new ColorDefinition object
   */
  ColorDefinition* createColorDefinition();

  /**
   * Removes the color definition with the given index from the list of color definitions.
   * If the index is valid, the object is removed and a pointer to the removed object 
   * is returned.
   * The caller is responsible for deleting thew object.
   * If the index is invalid, @c NULL is returned.
   *
   * @param index index of the color definition to be removed.
   *
   * @ return pointer to the removed object
   */
  ColorDefinition* removeColorDefinition(unsigned int index);

  /**
   * Adds a copy of the given color definition to the end of the list of
   * color definitions.
   *
   * The color definition has to be valid, i.e. have a unique id and a valid color value.
   * Otherwise it is not added.
   *
   * @param cd const pointer to ColorDefinition object to be added
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_LEVEL_MISMATCH
   * @li LIBSBML_VERSION_MISMATCH
   * @li LIBSBML_OPERATION_FAILED
   *
   * @note This method should be used with some caution.  The fact that
   * this method @em copies the object passed to it means that the caller
   * will be left holding a physically different object instance than the
   * one contained in this Reaction.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the Reaction</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see RenderInformationBase::createColorDefinition()
   * for a method that does not lead to these issues.
   *
   * @see createColorDefinition()
   */
  int addColorDefinition(const ColorDefinition* cd);

  /**
   * Returns the number of gradient definitions in the render information.
   *
   * @return number of gradient definitions
   */
  unsigned int getNumGradientDefinitions() const;

  /**
   * Returns a pointer to the list of gradient definitions.
   *
   * @return pointer to ListOfGradientDefinitions
   */
  ListOfGradientDefinitions* getListOfGradientDefinitions();

  /**
   * Returns a const pointer to the list of gradient definitions.
   *
   * @return const pointer to ListOfGradientDefinitions
   */
  const ListOfGradientDefinitions* getListOfGradientDefinitions() const;

  /**
   * Returns a pointer to the gradient definition with the given index, or @c NULL
   * if the index is invalid.
   *
   * @param index index of the GradientDefinition object to be returned
   *
   * @return pointer to the GradientDefinition object with the given index or NULL 
   * if the index was invalid.
   */
  GradientBase* getGradientDefinition(unsigned int index);

  /**
   * Returns a const pointer to the gradient definition with the given index, or @c NULL
   *if the index is invalid.
   *
   * @param index index of the GradientDefinition object to be returned
   *
   * @return const pointer to the GradientDefinition object with the given index or NULL 
   * if the index was invalid.
   */
  const GradientBase* getGradientDefinition(unsigned int index) const;

  /**
   * Returns a pointer to the gradient definition with the given @p id, or @c NULL
   * if there is no gradient definition with that id.
   *
   * @param id of the gradient definition object to be returned.
   *
   * @return a pointer to the gradient definition object with the given @p id 
   * or NULL if there is no gradient definition with given @p id
   */
  GradientBase* getGradientDefinition(const std::string& id);

  /**
   * Returns a const pointer to the gradient definition with the given @p id, or @c NULL
   * if there is no gradient definition with that id.
   *
   * @param id of the gradient definition object to be returned.
   *
   * @return a const pointer to the gradient definition object with the given @p id 
   * or NULL if there is no gradient definition with given @p id
   */
  const GradientBase* getGradientDefinition(const std::string& id) const;

  /**
   * Creates a new linear gradient definition.
   * The newly created object is added to the render 
   * information and also owned by it.
   *
   * Since the newly created object has no id and no gradient stops, 
   * it is invalid until those things have been added.
   *
   * @return pointer to newly created LinearGradient object.
   */
  LinearGradient* createLinearGradientDefinition();

  /**
   * Creates a new radial gradient definition.
   * The newly created object is added to the render 
   * information and also owned by it.
   *
   * Since the newly created object has no id and no gradient stops, 
   * it is invalid until those things have been added.
   *
   * @return pointer to newly created RadialGradient object.
   */
  RadialGradient* createRadialGradientDefinition();

  /**
   * Removes the gradient definition with the given index.
   * If the index is valid, the object is removed and a pointer to the removed object 
   * is returned.
   * The caller is responsible for deleting thew object.
   * If the index is invalid, @c NULL is returned.
   *
   * @param index index of the gradient definition object to be removed.
   *
   * @ return pointer to the removed object
   */
  GradientBase* removeGradientDefinition(unsigned int index);

  /**
   * Adds a copy of the given gradient definition to the end of the list of
   * gradient definitions.
   * The Gradient definition has to be valid, so is has to have at least two
   * gradient stops and an id.
   *
   * @param gradient GradientDefinition object to be added
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_LEVEL_MISMATCH
   * @li LIBSBML_VERSION_MISMATCH
   * @li LIBSBML_OPERATION_FAILED
   *
   * @note This method should be used with some caution.  The fact that
   * this method @em copies the object passed to it means that the caller
   * will be left holding a physically different object instance than the
   * one contained in this RenderInformationBase.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the RenderInformationBase</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see 
   * RenderInformationBase::createLinearGradientDefinition() or
   * RenderInformationBase::createRadialGradientDefinition()
   * for methods that does not lead to these issues.
   *
   * @see createRadialGradientDefinition()
   * @see createLinearGradientDefinition()
   */
  int addGradientDefinition(const GradientBase* gradient);

  /**
   * Returns the number of line endings for the render information.
   *
   * @return number of line endings in the render information.
   */
  unsigned int getNumLineEndings() const;

  /**
   * Returns a pointer to the list of line endings.
   *
   * @return pointer to the list of line endings.
   */
  ListOfLineEndings* getListOfLineEndings();

  /**
   * Returns a const pointer to the list of line endings.
   *
   * @return const pointer to the list of line endings.
   */
  const ListOfLineEndings* getListOfLineEndings() const;

  /**
   * Returns a pointer to the line ending with the given index, or @c NULL
   * if the index is invalid.
   *
   * @param index of the line ending object to be returned.
   *
   * @return a pointer to the line ending object with the given index 
   * or NULL if the index was out of bounds.
   */
  LineEnding* getLineEnding(unsigned int index);

  /**
   * Returns a const pointer to the line ending with the given index, or @c NULL
   * if the index is invalid.
   *
   * @param index of the line ending object to be returned.
   *
   * @return a const pointer to the line ending object with the given index 
   * or NULL if the index was out of bounds.
   */
  const LineEnding* getLineEnding(unsigned int index) const;

  /**
   * Returns a pointer to the line ending with the given @p id, or @c NULL
   * if there is no line ending with that id.
   *
   * @param id of the line ending object to be returned.
   *
   * @return a pointer to the line ending object with the given @p id 
   * or NULL if there is no line ending with given @p id
   */
  LineEnding* getLineEnding(const std::string& id);

  /**
   * Returns a const pointer to the line ending with the given @p id, or @c NULL
   * if there is no line ending with that id.
   *
   * @param id of the line ending object to be returned.
   *
   * @return a const pointer to the line ending object with the given @p id 
   * or NULL if there is no line ending with given @p id
   */
  const LineEnding* getLineEnding(const std::string& id) const;

  /**
   * Creates a new line ending.
   * The new line ending object is added to and owned by the 
   * render information. Since it does not have an id or a valid group
   * or a valid viewport, it should be considered invalid until those
   * things have been set.
   *
   * @return pointer to the newlyy created LineEnding object
   */
  LineEnding* createLineEnding();

  /**
   * Removes the line ending with the given index.
   * If the index is valid, the object is removed and a pointer to the removed object 
   * is returned.
   * The caller is responsible for deleting thew object.
   * If the index is invalid, @c NULL is returned.
   *
   * @param index index of the object to be removed.
   *
   * @ return pointer to the removed object
   */
  LineEnding* removeLineEnding(unsigned int index);

  /**
   * Adds a copy of the given line ending to the end of the list of line
   * endings.
   * The new LineEnding is only added if it is valid.
   *
   * @param le const pointer to LineEnding to be added
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_LEVEL_MISMATCH
   * @li LIBSBML_VERSION_MISMATCH
   * @li LIBSBML_OPERATION_FAILED
   *
   * @note This method should be used with some caution.  The fact that
   * this method @em copies the object passed to it means that the caller
   * will be left holding a physically different object instance than the
   * one contained in this RenderInformationBase.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the RenderInformationBase</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see RenderInformationBase::createLineEnding()
   * for a method that does not lead to these issues.
   *
   * @see createLineEnding()
   */
  int addLineEnding(const LineEnding* le);

  /**
   * Returns the background color which is either the id of a color in the
   * list of color definitions, or a color value.
   *
   * @return background color id or value string
   */
  const std::string& getBackgroundColor() const;

  /**
   * Sets the background color to either the id of a color in the list of
   * color definitions, or a color value.
   *
   * @param bg id of a color definition or a valid color value to be used as background color.
   */
  void setBackgroundColor(const std::string& bg);

  /**
   * Returns the value of the "id" attribute of this RenderInformationBase.
   *
   * @return the id of the RenderInformationBase
   */
  const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * RenderInformationBase's "id" attribute has been set.
   *
   * @return returns true or false depending on whether the id on the 
   * RenderInformationBase has been set.
   */
  bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this RenderInformationBase.
   *
   * @param id the new id for the RenderInformationBase 
   *
   * @return status if the operation succeeded
   */
  int setId (const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this RenderInformationBase.
   */
  virtual int unsetId ();

  /**
   * Returns the value of the "name" attribute of this RenderInformationBase.
   *
   * @return the name of the RenderInformationBase
   */
  const std::string& getName () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * RenderInformationBase's "name" attribute has been set.
   *
   * @return returns true or false depending on whether the name on the 
   * RenderInformationBase has been set.
   */
  bool isSetName () const;


  /**
   * Sets the value of the "name" attribute of this RenderInformationBase.
   *
   * @param name the new name for the RenderInformationBase 
   *
   * @return status if the operation succeeded
   */
  int setName (const std::string& id);


  /**
   * Unsets the value of the "name" attribute of this RenderInformationBase.
   */
  virtual int unsetName ();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor, assignment operator.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Enables/Disables the given package with this element and child
   * elements (if any).
   * (This is an internal implementation for enablePakcage function)
   *
   * @note Subclasses in which one or more child elements are defined
   * must override this function.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
    const std::string& pkgPrefix, bool flag);
  /** @endcond */

  /**
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
  void setParentSBMLObject (SBase* sb);

protected:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds the RenderInformationBase specific attributes to the given XMLAttributes object.
   */
  static void addRenderInformationBaseAttributes(const RenderInformationBase& base,XMLAttributes& attributes);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Reads in RenderInfromationBase specific attributes from the given XMLAttributes object.
   */
  virtual void readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes);
  /** @endcond */


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
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* RenderInformationBase_H__ */
