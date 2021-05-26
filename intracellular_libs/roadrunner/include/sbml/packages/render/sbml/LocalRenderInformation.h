/**
 * @file    LocalRenderInformation.h
 * @brief   class for representing local render information
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
 * @class LocalRenderInformation
 * @brief LocalRenderInformation is the render information stored in Layouts.
 *  LocalRenderInformation can be applied to all layouts.
 *
 * LocalRenderInformation is one of the subclasses of RenderInformationBase. A local render information object
 * contains color definitions, gradient definitions and line endings as defined in RenderInformationBase.
 * Additionally it has a list of local styles which specifies type, role and id based render information.
 * Local render information can specify id based render information because it does belong to a certain layout and it can reference ids of object in that layout.
 *
 * @class ListOfLocalRenderInformation
 * @brief container class that stores LocalRenderInformation objects.
 *
 * Layouts can contain ListOfLocalRenderInformation which holds all
 * LocalRenderInformation objects for a certain layout.
 */

#ifndef LocalRenderInformation_H__
#define LocalRenderInformation_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/RenderInformationBase.h> 
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/LocalStyle.h> 
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN LocalRenderInformation: public RenderInformationBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  ListOfLocalStyles mListOfStyles;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new LocalRenderInformation object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  LocalRenderInformation (unsigned int level      = RenderExtension::getDefaultLevel(),
                          unsigned int version    = RenderExtension::getDefaultVersion(),
                          unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new LocalRenderInformation object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  LocalRenderInformation (RenderPkgNamespaces* renderns);

  /**
   * Destroy this LocalRenderInformation object.
   */
  virtual ~LocalRenderInformation ();


  /**
   * Parses the xml information in the given node and sets the attributes.
   * This method should never be called by the user. It is only used to read render 
   * information from annotations.
   *
   * @param node the XMLNode object reference that describes the LocalRenderInformation
   * object to be instantiated.
   */
  void parseXML(const XMLNode& node);


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a LocalRenderInformation with the given @p id
   * and all lists empty.
   *
   * @param id the new id for the LocalRenderInformation.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  LocalRenderInformation(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Returns the number of styles.
   *
   * @return the number of local styles in the global render information object
   */
  unsigned int getNumStyles() const;

  /**
   * Creates and returns a deep copy of this LocalRenderInformation object.
   *
   * @return a (deep) copy of this LocalRenderInformation.
   */
  virtual LocalRenderInformation* clone () const;


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;


  /**
   * Returns a pointer to the ListOfLocalStyles object.
   *
   * @return pointer to the list of local styles.
   */
  ListOfLocalStyles* getListOfStyles();

  /**
   * Returns a const pointer to the ListOfLocalStyles object.
   *
   * @return const pointer to the list of local styles.
   */
  const ListOfLocalStyles* getListOfStyles() const;

  /**
   * Returns a pointer to the style with the given index.
   * If the index is invalid, @c NULL is returned.
   * 
   * @param i index of the LocalStyle to be returned.
   * 
   * @return pointer to the style with the given index or NULL
   */
  LocalStyle* getStyle(unsigned int i);

  /**
   * Returns a pointer to the style with the given index.
   * If the index is invalid, @c NULL is returned.
   * 
   * @param i index of the LocalStyle to be returned.
   * 
   * @return const pointer to the style with the given index or NULL
   */
  const LocalStyle* getStyle(unsigned int i) const;

  /**
   * Returns a pointer to the style with the given @p id.
   * If the id is invalid, @c NULL is returned.
   * 
   * @param id id of the LocalStyle to be returned.
   * 
   * @return pointer to the style with the given @p id or @c NULL
   */
  LocalStyle* getStyle(const std::string& id);

  /**
   * Returns a pointer to the style with the given @p id.
   * If the id is invalid, @c NULL is returned.
   * 
   * @param id id of the LocalStyle to be returned.
   * 
   * @return const pointer to the style with the given @p id or @c NULL
   */
  const LocalStyle* getStyle(const std::string& id) const;

  /**
   * Creates a new LocalStyle object. The object is added to and owned
   * by the LocalRenderInformation object.
   * 
   * @param id for the new style.
   * 
   * @ return a pointer to the newly created LocalStyle object.
   */
  LocalStyle* createStyle(const std::string& id);

  /**
   * Adds a copy of a LocalStyle to the GlobalRenderInformation object.
   * The style is only added if it is valid, i.e. it has to have an id and
   * a valid group.
   *
   * @param pointer to the local style object to be added.
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
   * one contained in this LocalRenderInformation.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the LocalRenderInformation</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see LocalRenderInformation::createStyle()
   * for a method that does not lead to these issues.
   *
   * @see createStyle()
   */
  int addStyle(const LocalStyle* pStyle);

  /**
   * Creates an XMLNode object from this LocalRenderInformation object.
   *
   * @return the XMLNode with the XML representation for the 
   * LocalRenderInformation object.
   *
   */
  XMLNode toXML() const;

  /**
   * Returns the XML element name of this object, which for
   * LocalRenderInformation, is always @c "renderInformation".
   * 
   * @return the name of this element, i.e., @c "renderInformation".
   */
  virtual const std::string& getElementName () const;

  /**
   * Returns the libSBML type code for this %SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.
   * The set of possible type codes is defined in the enumeration
   * #SBMLTypeCode_t.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if java LibSBML attaches an
   * identifying code to every kind of SBML object.  These are known as
   * <em>SBML type codes</em>.  In other languages, the set of type codes
   * is stored in an enumeration; in the Java language interface for
   * libSBML, the type codes are defined as static integer constants in
   * interface class {@link libsbmlConstants}.  The names of the type codes
   * all begin with the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or @c SBML_UNKNOWN (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode() const;

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


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
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


class LIBSBML_EXTERN ListOfLocalRenderInformation : public ListOf
{
public:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Creates a new ListOfLocalRenderInformation object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfLocalRenderInformation object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfLocalRenderInformation
   * object to be instantiated.
  ListOfLocalRenderInformation(const XMLNode& node);
   */
  /** @endcond */

  /**
   * Creates and returns a deep copy of the ListOfLocalRenderInformation object.
   *
   * @return a (deep) copy of this ListOfLocalRenderInformation
   */
  virtual ListOfLocalRenderInformation* clone () const;

  /**
   * Constructor which instantiates an empty ListOfLocalRenderInformation object.
   */
  ListOfLocalRenderInformation(unsigned int level      = RenderExtension::getDefaultLevel(),
                               unsigned int version    = RenderExtension::getDefaultVersion(),
                               unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfLocalRenderInformation(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor for ListOfLocalRenderInformation objects.
   */
  ListOfLocalRenderInformation(const ListOfLocalRenderInformation& source);

  /**
   * Assignment operator for ListOfLocalRenderInformation objects.
   */
  ListOfLocalRenderInformation& operator=(const ListOfLocalRenderInformation& source);


  /**
   * Returns the XML element name of this object, which for
   * ListOfLocalRenderInformation, is always @c "listOfRenderInformation".
   * 
   * @return the name of this element, i.e., @c "listOfRenderInformation".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfGradientDefinitions object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfGradientDefinitions object.
   */
  XMLNode toXML() const;

  /**
   * Sets the version of the render information list.
   * The version consists of a major and a minor version number.
   *
   * @param major major version number
   * @param minor minor version number
   */
  void setVersion(unsigned int major,unsigned int minor);

  /**
   * Returns the major version of the render information list.
   *
   * @return the major version number of the local render information list
   */
  unsigned int getMajorVersion() const;

  /**
   * Returns the minor version of the render information list.
   *
   * @return the minor version number of the local render information list
   */
  unsigned int getMinorVersion() const;

  /**
   * Returns the version as a string.
   *
   * @return the version of the LocalRenderInformation object
   * as a string
   */
  std::string getVersionString() const;

  /**
   * Returns a pointer to the LocalRenderInformation with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LocalRenderInformation object to be returned
   * 
   * @return pointer to the LocalRenderInformation at the given index or NULL.
   */
  LocalRenderInformation* get(unsigned int i);

  /**
   * Returns a const pointer to the LocalRenderInformation with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LocalRenderInformation object to be returned
   * 
   * @return const pointer to the LocalRenderInformation at the given index or NULL.
   */
  const LocalRenderInformation* get(unsigned int i) const;

  /**
   * Returns a pointer to the LocalRenderInformation with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LocalRenderInformation object to be returned
   * 
   * @return pointer to the LocalRenderInformation at the given @p id or @c NULL.
   */
  LocalRenderInformation* get(const std::string& id);

  /**
   * Returns a const pointer to the LocalRenderInformation with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LocalRenderInformation object to be returned
   * 
   * @return const pointer to the LocalRenderInformation at the given @p id or @c NULL.
   */
  const LocalRenderInformation* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfLocalRenderInformation items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual LocalRenderInformation* remove (unsigned int n);


  /**
   * Removes item in this ListOfLocalRenderInformation items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual LocalRenderInformation* remove (const std::string& sid);



  /**
   * Get the type code of the objects contained in this ListOf.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   * 
   * @return the SBML type code for the objects contained in this ListOf
   * instance, or @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   */
  virtual int getItemTypeCode () const;

  virtual bool isValidTypeForList(SBase * item);

protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */



  /** @cond doxygenLibsbmlInternal */
  
  
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);
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
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream. 
   *
   */
  virtual void writeXMLNS (XMLOutputStream& stream) const;
  static const std::string ELEMENT_NAME;
  unsigned int mVersionMajor;
  unsigned int mVersionMinor;
  /** @endcond */
};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* LocalRenderInformation_H__ */
