/**
 * @file    RenderGroup.h
 * @brief   class to group render elements
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
 * @class RenderGroup
 * @brief The RenderGroup concept from the SBML render extension is used to group graphical primitives together
 * to create composite representations from simple primitives.
 *
 * The RenderGroup class is derived from GrphicalPrimitive2D and inherits all its methods and attributes.
 * In addition to those, the class defines attributes to specify text render properties (@see Text),
 * curve decorations (@see RenderCurve) an id and a list of child elements which can be any 
 * graphical primitive or other groups.
 *
 * The attributes of a group are inherited by all children of the group unless they specify 
 * the attribute themselves.
 *
 *
 * @class ListOfDrawables
 * @brief container class that stores objects derived from Transformation2D.
 *
 * Layouts can contain ListOfDrawables which holds all
 * Transformation2D objects for a certain group.
 */

#ifndef RenderGroup_H__
#define RenderGroup_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/sbml/Text.h>
#include <sbml/packages/render/sbml/Rectangle.h>
#include <sbml/packages/render/sbml/Image.h>
#include <sbml/packages/render/sbml/Ellipse.h>
#include <sbml/packages/render/sbml/Polygon.h>
#include <sbml/packages/render/sbml/RenderCurve.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/ListOf.h>

#ifdef __cplusplus
#include <sbml/packages/render/extension/RenderExtension.h>
#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN ListOfDrawables : public ListOf
{
public:

  /**
   * Creates and returns a deep copy of the ListOfDrawables object.
   *
   * @return a (deep) copy of this ListOfDrawables
   */
  virtual ListOfDrawables* clone () const;

  /**
   * Constructor which instantiates an empty ListOfDrawables object.
   */
  ListOfDrawables(unsigned int level      = RenderExtension::getDefaultLevel(),
                  unsigned int version    = RenderExtension::getDefaultVersion(),
                  unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfDrawables(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor. Creates a copy of this ListOfDrawables object.
   */
  ListOfDrawables(const ListOfDrawables& source);

  /**
   * Assignment operator for ListOfColroDefinitions objects.
   */
  ListOfDrawables& operator=(const ListOfDrawables& source);


  /**
   * Returns a pointer to the Transformation2D with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the Transformation2D object to be returned
   * 
   * @return pointer to the Transformation2D at the given index or NULL.
   */
  Transformation2D* get(unsigned int i);

  /**
   * Returns a const pointer to the Transformation2D with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the Transformation2D object to be returned
   * 
   * @return const pointer to the Transformation2D at the given index or NULL.
   */
  const Transformation2D* get(unsigned int i) const;

  /**
   * Returns a pointer to the Transformation2D with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the Transformation2D object to be returned
   * 
   * @return pointer to the Transformation2D at the given @p id or @c NULL.
   */
  Transformation2D* get(const std::string& id);

  /**
   * Returns a const pointer to the Transformation2D with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the Transformation2D object to be returned
   * 
   * @return const pointer to the Transformation2D at the given @p id or @c NULL.
   */
  const Transformation2D* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfDrawables items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual Transformation2D* remove (unsigned int n);


  /**
   * Removes item in this ListOfDrawables items with the given identifier.
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
  virtual Transformation2D* remove (const std::string& sid);

  /**
   * Returns the name of this element.
   * 
   * @return the name of the element, in this case "listOfDrawables"
   */
  virtual const std::string& getElementName() const;


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

  static const std::string ELEMENT_NAME;
  /** @endcond */

  friend class RenderGroup;
};


class LIBSBML_EXTERN RenderGroup : public GraphicalPrimitive2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mFontFamily;
  RelAbsVector mFontSize;
  Text::FONT_WEIGHT mFontWeight;
  Text::FONT_STYLE mFontStyle;
  Text::TEXT_ANCHOR mTextAnchor;
  Text::TEXT_ANCHOR mVTextAnchor;
  std::string mStartHead;
  std::string mEndHead;
  ListOfDrawables mElements;

  static const std::string ELEMENT_NAME;
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

public:
  /**
   * Creates a new RenderGroup object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  RenderGroup (unsigned int level      = RenderExtension::getDefaultLevel(),
               unsigned int version    = RenderExtension::getDefaultVersion(),
               unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new RenderGroup object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  RenderGroup (RenderPkgNamespaces* renderns);


  /**
   * Creates a new RenderGroup object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * RenderGroup object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the RenderGroup
   * object to be instantiated.
   */
  RenderGroup(const XMLNode& group, unsigned int l2version=4);

#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new RenderGroup object.
   * All attributes are set as described for the default constructor
   * of GraphicalPrimitive2D.
   * All the font rendering attributes and the curve decorations
   * are unset. The id is set to the given string.
   *
   * @param id the id for the RenderGroup object.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  RenderGroup(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Destroys the RenderGroup object and all it's children.
   */
  virtual ~RenderGroup();

  /**
   * Sets the font family.
   *
   * @param family The name of the font family, e.g. Helvetica
   */
  void setFontFamily(const std::string& family);

  /**
   * Sets the font size.
   * Normally this is an absolute value, e.g. 18 for a 18pt font.
   * It is however allowed the specify the font size in terms of relative values
   * in relation to the current viewport. In most cases the viewport will be the 
   * dimensions of a bounding box of a layout object.
   *
   * @param size the new font size.
   */
  void setFontSize(const RelAbsVector& size);

  /**
   * Sets the font weight.
   * Valid values are Text::WEIGHT_UNSET, Text::WEIGHT_NORMAL or
   * Text::WEIGHT_BOLD.
   *
   * @param weight The new text weight to be set.
   */
  void setFontWeight(Text::FONT_WEIGHT weight);

  /**
   * Sets the font style.
   * Valid values are Text::STYLE_UNSET, Text::STYLE_NORMAL or
   * Text::STYLE_ITALIC
   *
   * @param style The new font style to be set.
   */
  void setFontStyle(Text::FONT_STYLE style);

  /**
   * Sets the text anchor.
   * This is defines the horizontal text position.
   * Valid values are Text::ANCHOR_UNSET, Text::ANCHOR_START,
   * Text::ANCHOR_MIDDLE and Text_ANCHOR_END.
   * Text::ANCHOR_BASELINE is not a valid value
   * for the text-anchor attribute. If you set the text anchor to 
   * Text::ANCHOR_BASELINE, it will be set to Text::ANCHOR_UNSET.
   *
   * @param anchor The new horizontal alignment flag.
   */
  void setTextAnchor(Text::TEXT_ANCHOR anchor);

  /**
   * Sets the vertical text anchor.
   * This is defines the vertical text position.
   * Valid values are Text::ANCHOR_UNSET, Text::ANCHOR_TOP,
   * Text::ANCHOR_MIDDLE and Text_ANCHOR_BOTTOM.
   *
   * @param anchor The new vertical alignment flag.
   */
  void setVTextAnchor(Text::TEXT_ANCHOR anchor);

  /**
   * Sets the id of the start head.
   *
   * @param The id of a LineEnding object to be applied to the start of curve children.
   */
  void setStartHead(const std::string& id);

  /**
   * Sets the id of the end head.
   *
   * @param The id of a LineEnding object to be applied to the end of curve children. 
   */
  void setEndHead(const std::string& id);

  /**
   * Returns the font family.
   *
   * @return The name of the font family to be used for text rendering.
   */
  const std::string& getFontFamily() const;

  /**
   * Returns the font size as a reference.
   *
   * @return A reference to the size to be used for rendering text.
   */
  RelAbsVector& getFontSize();

  /**
   * Returns the font size as a const reference.
   *
   * @return A const reference to the size to be used for rendering text.
   */
  const RelAbsVector& getFontSize() const;

  /**
   * Returns the font weight.
   *
   * @return font weight used to render text children
   */
  Text::FONT_WEIGHT getFontWeight() const;

  /**
   * Returns the font style.
   *
   * @return font style used to render text children
   */
  Text::FONT_STYLE getFontStyle() const;

  /**
   * Returns the text anchor.
   *
   * @return the horizontal text alignment flag
   */
  Text::TEXT_ANCHOR getTextAnchor() const;

  /**
   * Returns the vertical text anchor.
   *
   * @return the vertical text alignment flag
   */
  Text::TEXT_ANCHOR getVTextAnchor() const;

  /**
   * Returns the id of the LineEnding object to be applied to the start of the curve.
   *
   * @return id of the LineEnding for the start of curves.
   */
  const std::string& getStartHead() const;

  /**
   * Returns the id of the LineEnding object to be applied to the end of the curve.
   *
   * @return id of the LineEnding for the end of curves.
   */
  const std::string& getEndHead() const;

  /**
   * Returns the number of children in the group.
   *
   * @return The number of child elements in the group.
   */
  unsigned int getNumElements() const;

  /**
   * Returns a const pointer to the list of  elements.
   *
   * @return const pointer to the list of children
   */
  const ListOfDrawables* getListOfElements() const;

  /**
   * Returns the list of  elements.
   *
   * @return pointer to the list of children
   */
  ListOfDrawables* getListOfElements();

  /**
   * Returns pointer to the element with index n.
   * If there is no such element, @c NULL is returned.
   *
   * @param index index of element to be returned
   *
   * @return pointer to element with index index or NULL if
   * index is out of bounds.
   */
  Transformation2D* getElement(unsigned int index); 

  /**
   * Returns const pointer to the element with index n.
   * If there is no such element, @c NULL is returned.
   *
   * @param index index of element to be returned
   *
   * @return pointer to element with index index or NULL if
   * index is out of bounds.
   */
  const Transformation2D* getElement(unsigned int index) const; 

  /**
   * Returns pointer to the element with the given @p id.
   * If there is no such element, @c NULL is returned.
   * Since the id on all those object is optional, this routine
   * might not be as helpful as similar routines in other classes.
   *
   * @param id id of element to be returned
   *
   * @return pointer to element with id or NULL if
   * there is no object with that id
   */
  Transformation2D* getElement(const std::string& id); 

  /**
   * Returns const pointer to the element with given index.
   * If there is no such element, @c NULL is returned.
   * Since the id on all those object is optional, this routine
   * might not be as helpful as similar routines in other classes.
   *
   * @param id id of element to be returned
   *
   * @return pointer to element with the given @p id or @c NULL if
   *  there is no object with that id
   */
  const Transformation2D* getElement(const std::string& id) const; 


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Accepts the given SBMLVisitor for this instance of RenderGroup.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Returns true if the horizonal alignment attribute has been set.
   *
   * @return true is flag is not Text::ANCHOR_UNSET
   */
  bool isSetTextAnchor() const;

  /**
   * Returns true if the vertical alignment attribute has been set.
   *
   * @return true is flag is not Text::ANCHOR_UNSET
   */
  bool isSetVTextAnchor() const;


  /**
   * Creates and returns a deep copy of this RenderGroup object.
   * 
   * @return a (deep) copy of this RenderGroup object
   */
  virtual RenderGroup* clone () const;

  /**
   * Returns the XML element name of this object, which for
   * RenderGroup, is always @c "g".
   * 
   * @return the name of this element, i.e., @c "g".
   */
  virtual const std::string& getElementName () const;

  /**
   * Creates an image object and adds it to the end of the list of child
   * elements. The new element is owned by the group.
   *
   * @return pointer to the new Image child.
   */
  Image* createImage();

  /**
   * Creates an group object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new RenderGroup child.
   */
  RenderGroup* createGroup();

  /**
   * Creates a rectangle object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new Rectangle child.
   */
  Rectangle* createRectangle();

  /**
   * Creates an ellipse object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new Ellipse child.
   */
  Ellipse* createEllipse();

  /**
   * Creates a curve object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new RenderCurve child.
   */
  RenderCurve* createCurve();

  /**
   * Creates a polygon object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new Polygon child.
   */
  Polygon* createPolygon();

  /**
   * Creates a text object and adds it to the end of the list of child
   * elements The new element is owned by the group..
   *
   * @return pointer to the new Text child.
   */
  Text* createText();

  /**
   * Adds a copy of the given element to the end of the list of children elements.
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
   * one contained in this RenderGroup.  Changes made to the original object
   * instance (such as resetting attribute values) will <em>not affect the
   * instance in the RenderGroup</em>.  In addition, the caller should make
   * sure to free the original object if it is no longer being used, or
   * else a memory leak will result.  Please see RenderGroup::createXXX()
   * for methods that do not lead to these issues.
   *
   * @see createEllipse()
   * @see createRectangle()
   * @see createPolygon()
   * @see createText()
   * @see createCurve()
   * @see createRenderGroup()
   * @see createImage()
   */
  int addChildElement(const Transformation2D* pChild);

  /**
   * Returns true if the start head is set or false otherwise.
   * The start decoration is considered set if the string is not empty and if
   * it is not the string "none"
   *
   * @return true is the start decoration id is set
   */
  bool isSetStartHead() const;

  /**
   * Returns true if the end head is set or false otherwise.
   * The end decoration is considered set if the string is not empty and if
   * it is not the string "none"
   *
   * @return true is the end decoration id is set
   */
  bool isSetEndHead() const;

  /**
   * Returns true if the font family has been set or false otherwise.
   *
   * @return true if the font family string is not empty
   */
  bool isSetFontFamily() const;

  /**
   * Returns true if the font size has been set or false otherwise.
   *
   * @return true if the RelAbsVector specifying the font size does not
   * contain NaN either as the absolute or the relative value.
   */
  bool isSetFontSize() const;

  /**
   * Returns true if the font weight has been set or false otherwise.
   *
   * @return true is the flag is not Text::WEIGHT_UNSET
   */
  bool isSetFontWeight() const;

  /**
   * Returns true if the font style has been set or false otherwise.
   *
   * @return true is the flag is not Text::STYLE_UNSET
   */
  bool isSetFontStyle() const;

  /**
   * Creates an XMLNode object from this RenderGroup object.
   *
   * @return the XMLNode with the XML representation for the 
   * RenderGroup object.
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
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
  virtual void setParentSBMLObject (SBase* sb);

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
  int getTypeCode () const;
protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Imports curves that follow the older curve specification from an XMLNode object.
   */
  void importOldCurve(const XMLNode& node);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds the text rendering attributes of the given RenderGroup object
   * to the given XMLAttributes object.
   */
  static void addTextAttributes(const RenderGroup& group,XMLAttributes& att);
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
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
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

#endif /* RenderGroup_H__ */
