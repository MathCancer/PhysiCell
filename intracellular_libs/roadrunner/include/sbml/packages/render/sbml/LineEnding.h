/**
 * @file    LineEnding.h
 * @brief   class representing line endings, e.g. arrow heads
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
 * @class LineEnding
 * @brief a LineEnding is a decoration element for the start and/or end
 * of curves in the SBML render extension, e.g. arrow heads
 *
 * LineEndings in the SBML render extension are used to apply certain decorations
 * to the end of curves. Since many curves in layout diagrams use the same decoration 
 * for the beginnings and start of a line, it would be highly redundant to encode
 * those decorations with each line. Therefor LineEnding objects can be defined which are 
 * then applied to the beginning or the ends of several curve objects.
 *
 * A LineEnding contains an id by which it can be referenced from curve styles, it contains
 * a visual representation of the decoration in the form of a render extension Group object
 * and it has some attributes that define the viewport and how the LineEnding is to be applied 
 * to a curve.
 *
 * A LineEnding object is only valid if it has an id, a viewport that has an area which is not 0
 * and a valid group object.
 *
 * @class ListOfLineEndings
 * @brief container class to store LineEnding objects
 *
 * Each RenderInformation object can contain its own ListOfLineEndings object.
 */


#ifndef LineEnding_H__
#define LineEnding_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/RenderGroup.h>
#include <sbml/packages/layout/sbml/BoundingBox.h>
#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN LineEnding : public GraphicalPrimitive2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId;
  bool mEnableRotationalMapping;
  BoundingBox mBoundingBox;
  RenderGroup mGroup;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new LineEnding object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  LineEnding (unsigned int level      = RenderExtension::getDefaultLevel(),
              unsigned int version    = RenderExtension::getDefaultVersion(),
              unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new LineEnding object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  LineEnding (RenderPkgNamespaces* renderns);


  /**
   * Creates a new LineEnding object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * LineEnding object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the LineEnding
   * object to be instantiated.
   */
  LineEnding(const XMLNode& node, unsigned int l2version);

  /**
   * Copy constructor.
   */
  LineEnding(const LineEnding& other);


  /**
   * Destroy this LineEnding object.
   */
  virtual ~LineEnding ();


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a LineEnding with an empty group object,
   * and a viewport with a size of 0.
   * The id is set to the given value.
   * In order to get a valid object, the group object has to be valid,
   * the group object has to have descendants other than groups and
   * the viewport has to have a positive size.
   *
   * @param id The id for the LineEnding.
   *
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  LineEnding(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets whether rotational mapping is to be done or not.
   * This flag determines whether the LineEnding is rotated
   * according to the direction of the curve when it is applied.
   * For details on this, see the render extension specification.
   *
   * @param enable Boolean flag that specifies whether rotational mapping
   * for the line ending is to be enabled or not.
   */
  void setEnableRotationalMapping(bool enable);

  /**
   * Returns whether rotational mapping is enabled or not.
   *
   * @return bool value that specifies if rotational mapping is 
   * enabled for the LineEnding or not.
   */
  bool getIsEnabledRotationalMapping() const;

  /**
   * Sets the viewport for the LineEnding.
   *
   * @param box The viewport bounding box for the LineEnding.
   */
  void setBoundingBox(const BoundingBox* box);

  /**
   * Returns a pointer to the viewport bounding box.
   *
   * @return pointer to the viewport bounding box.
   */
  BoundingBox* getBoundingBox();

  /**
   * Returns a const pointer to the viewport bounding box.
   *
   * @return const pointer to the viewport bounding box.
   */
  const BoundingBox* getBoundingBox() const;

  /**
   * Sets the group of the LineEnding to a copy of the given group.
   *
   * @param group const pointer to the group to be set for the bounding box.
   * The group object is copied.
   */
  void setGroup(const RenderGroup* group);

  /**
   * Returns a const pointer to the group object.
   *
   * @return const pointer to the group object
   */
  const RenderGroup* getGroup() const;

  /**
   * Returns a pointer to the group object.
   *
   * @return pointer to the group object
   */
  RenderGroup* getGroup();


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Accepts the given SBMLVisitor for this instance of LineEnding.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this LineEnding object.
   * 
   * @return a (deep) copy of this LineEnding object
   */
  virtual LineEnding* clone () const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  virtual const std::string& getElementName () const;

  /**
   * Creates an XMLNode object from this LineEnding object.
   *
   * @return the XMLNode with the XML representation for the 
   * LineEnding object.
   */
  XMLNode toXML() const;

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

  /**
   * Returns the value of the "id" attribute of this GraphicalPrimitive.
   *
   * @return the id of the GraphicalPrimitive
   */
  const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * GraphicalPrimitive's "id" attribute has been set.
   *
   * @return returns true or false depending on whether the id on the 
   * GraphicalPrimitive has been set.
   */
  bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this GraphicalPrimitive.
   *
   * @param id the new id for the GraphicalPrimitive 
   *
   * @return status if the operation succeeded
   */
  int setId (const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this GraphicalPrimitive.
   */
  virtual int unsetId ();


  /**
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
  virtual void setParentSBMLObject (SBase* sb);

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



protected:
  /** @cond doxygenLibsbmlInternal */

  /**
   * When reading SBML render extension, this method takes care of 
   * getting the attributes to the LineEnding object.
   *
   * @param const reference to XMLAttributes object which contains 
   * the attributes to be set on the LineEnding.
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

  
  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream. 
   *
   */
  virtual void writeXMLNS (XMLOutputStream& stream) const;
  /** @endcond */
};


class LIBSBML_EXTERN ListOfLineEndings : public ListOf
{
protected:
  /** @cond doxygenLibsbmlInternal */
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:

  /**
   * Creates and returns a deep copy of the ListOfLineEndings object.
   *
   * @return a (deep) copy of this ListOfLineEndings
   */
  virtual ListOfLineEndings* clone () const;

  /**
   * Creates a new ListOfLineEndings object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfLineEndings object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfLineEndings
   * object to be instantiated.
   */
  ListOfLineEndings(const XMLNode& node, unsigned int l2version=4);  

  /**
   * Constructor which instantiates an empty ListOfLineEndings object.
   */
  ListOfLineEndings(unsigned int level      = RenderExtension::getDefaultLevel(),
                    unsigned int version    = RenderExtension::getDefaultVersion(),
                    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfLineEndings(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor. Creates a copy of this ListOfLineEndings object.
   */
  ListOfLineEndings(const ListOfLineEndings& source);

  /**
   * Assignment operator for ListOfLineEndings objects.
   */
  ListOfLineEndings& operator=(const ListOfLineEndings& source);


  /**
   * Returns the XML element name of this object, which for
   * ListOfLineEndings, is always @c "listOfLineEndings".
   * 
   * @return the name of this element, i.e., @c "listOfLineEndings".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfLineEndings object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfLineEndings object.
   */
  XMLNode toXML() const;

  /**
   * Returns a pointer to the LineEnding with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LineEnding object to be returned
   * 
   * @return pointer to the LineEnding at the given index or NULL.
   */
  LineEnding* get(unsigned int i);

  /**
   * Returns a const pointer to the LineEnding with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LineEnding object to be returned
   * 
   * @return const pointer to the LineEnding at the given index or NULL.
   */
  const LineEnding* get(unsigned int i) const;

  /**
   * Returns a pointer to the LineEnding with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LineEnding object to be returned
   * 
   * @return pointer to the LineEnding at the given @p id or @c NULL.
   */
  LineEnding* get(const std::string& id);

  /**
   * Returns a const pointer to the LineEnding with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LineEnding object to be returned
   * 
   * @return const pointer to the LineEnding at the given @p id or @c NULL.
   */
  const LineEnding* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfLineEndings items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual LineEnding* remove (unsigned int n);


  /**
   * Removes item in this ListOfLineEndings items with the given identifier.
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
  virtual LineEnding* remove (const std::string& sid);


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

protected:
  /** @cond doxygenLibsbmlInternal */

  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* LineEnding_H__ */
