/**
 * @file    Text.h
 * @brief   class for representing text objects
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
 * @class Text
 * @brief Text element from the SBML render extension. Is used to represent text render information.
 *
 * The Text class represents text to be rendered in the context of a style.
 * The Text class inherits all attributes and methods from its base class GraphicalPrimitive1D.
 * 
 * The text also holds a string for the actual text that is to be rendered for the Text object.
 *
 * Additional attributes specify how the text is to be rendered, e.g. which font family is to be used
 * and how the text is to be aligned within the viewport.
 */

#ifndef Text_H__
#define Text_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/GraphicalPrimitive1D.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLAttributes.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN Text : public GraphicalPrimitive1D
{
public:
  enum FONT_WEIGHT
  {
    WEIGHT_UNSET, 
    WEIGHT_NORMAL,
    WEIGHT_BOLD
  };

  enum FONT_STYLE
  {
    STYLE_UNSET, 
    STYLE_NORMAL,
    STYLE_ITALIC
  };

  enum TEXT_ANCHOR 
  {
    ANCHOR_UNSET=0,
    ANCHOR_START=1,
    ANCHOR_MIDDLE=2,
    ANCHOR_END=3,
    ANCHOR_TOP=1,
    ANCHOR_BOTTOM=3
    //,ANCHOR_BASELINE=4
  };


protected:
  /** @cond doxygenLibsbmlInternal */
  RelAbsVector mX;
  RelAbsVector mY;
  RelAbsVector mZ;
  std::string mFontFamily; 
  RelAbsVector mFontSize;
  FONT_WEIGHT mFontWeight; 
  FONT_STYLE mFontStyle; 
  TEXT_ANCHOR mTextAnchor; 
  TEXT_ANCHOR mVTextAnchor; 
  std::string mText;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new Text object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Text (unsigned int level      = RenderExtension::getDefaultLevel(),
    unsigned int version    = RenderExtension::getDefaultVersion(),
    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Text object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Text (RenderPkgNamespaces* renderns);

  /**
   * Creates a new Text object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Text object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Text
   * object to be instantiated.
   */
  Text(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this Text object.
   */
  virtual ~Text ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates a new Text object with the given @p id and position offset.
   * The position offset coordinates can be omitted and will be set to 0 in
   * that case.
   *
   * All attributes are set as described for the default constructor
   * of GraphicalPrimitive1D.
   * All the font rendering attributes as well 
   * as the text to be rendered are unset.
   *
   * @param id id string for the Text object
   * @param x x coordinate of the position offset
   * @param y y coordinate of the position offset
   * @param z z coordinate of the position offset
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Text(RenderPkgNamespaces* renderns, const std::string& id,const RelAbsVector& x=RelAbsVector(0.0,0.0),const RelAbsVector& y=RelAbsVector(0.0,0.0),const RelAbsVector& z=RelAbsVector(0.0,0.0));
#endif // OMIT_DEPRECATED

  /**
   * Sets the position of the text within the viewport.
   * This is like an offset that is applied after alignment.
   * If the z coordinate is omitted, it is set to 0.
   *
   * @param x x coordinate of the position offset
   * @param y y coordinate of the position offset
   * @param z z coordinate of the position offset
   */
  void setCoordinates(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Sets the x position of the text within the viewport.
   * This is like an offset that is applied after alignment.
   *
   * @param x x coordinate of the position offset
   */
  void setX(const RelAbsVector& x);

  /**
   * Sets the y position of the text within the viewport.
   * This is like an offset that is applied after alignment.
   *
   * @param y y coordinate of the position offset
   */
  void setY(const RelAbsVector& y);

  /**
   * Sets the z position of the text within the viewport.
   * This is like an offset that is applied after alignment.
   *
   * @param z z coordinate of the position offset
   */
  void setZ(const RelAbsVector& z);

  /**
   * Returns the x position offset as a const reference.
   * This offset is applied after alignment.
   *
   * @return const reference of x position offset
   */
  const RelAbsVector& getX() const;

  /**
   * Returns the y position offset as a const reference.
   * This offset is applied after alignment.
   *
   * @return const reference of y position offset
   */
  const RelAbsVector& getY() const;

  /**
   * Returns the z position offset as a const reference.
   * This offset is applied after alignment.
   *
   * @return const reference of z position offset
   */
  const RelAbsVector& getZ() const;

  /**
   * Returns the x position offset as a reference.
   * This offset is applied after alignment.
   *
   * @return reference of x position offset
   */
  RelAbsVector& getX();

  /**
   * Returns the y position offset as a reference.
   * This offset is applied after alignment.
   *
   * @return reference of y position offset
   */
  RelAbsVector& getY();

  /**
   * Returns the z position offset as a reference.
   * This offset is applied after alignment.
   *
   * @return reference of z position offset
   */
  RelAbsVector& getZ();

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
   * Returns the font family.
   *
   * @return The name of the font family to be used for text rendering.
   */
  const std::string& getFontFamily() const;

  /**
   * Returns the font size as a const reference.
   *
   * @return const reference to the size to be used for rendering text.
   */
  const RelAbsVector& getFontSize() const;

  /**
   * Returns the font size as a reference.
   *
   * @return A reference to the size to be used for rendering text.
   */
  RelAbsVector& getFontSize();


  /**
   * Returns the font weight.
   *
   * @return font weight used to render text children
   */
  FONT_WEIGHT getFontWeight() const;

  /**
   * Returns the font style.
   *
   * @return font style used to render text children
   */
  FONT_STYLE getFontStyle() const;

  /**
   * Returns the text anchor.
   *
   * @return the horizontal text alignment flag
   */
  TEXT_ANCHOR getTextAnchor() const;

  /**
   * Returns the vertical text anchor.
   *
   * @return the vertical text alignment flag
   */
  TEXT_ANCHOR getVTextAnchor() const;

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
   * Returns the text for the Text object.
   *
   * @return the text string to be rendered for the Text object.
   */
  const std::string& getText() const;

  /**
   * Sets the text for the text element.
   *
   * @param text The text to be rendered for the Text object.
   */
  void setText(const std::string& text);

  /**
   * Returns true if the text is set to something else than the empty string.
   *
   * @return true if the text is not empty.
   */
  bool isSetText() const;

  /**
   * Accepts the given SBMLVisitor for this instance of Group.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  bool accept(SBMLVisitor& visitor) const;

  /**
   * Creates and returns a deep copy of this Text object.
   * 
   * @return a (deep) copy of this Text object
   */
  Text* clone() const;

  /**
   * Returns the XML element name of this object, which for
   * Text, is always @c "text".
   * 
   * @return the name of this element, i.e., @c "text".
   */
  const std::string& getElementName() const;

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
   * This method is purely abstract and has to be implemented by derived
   * classes.
   *
   * @see getElementName()
   */
  virtual int getTypeCode() const;

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
   * Creates an Text object from this Group object.
   *
   * @return the XMLNode with the XML representation for the 
   * Text object.
   */
  XMLNode toXML() const;

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


  /** 
   * When overridden allows SBase elements to use the text included in between
   * the elements tags. The default implementation does nothing.
   * 
   * @param text the text string found between the element tags.
   */ 
  virtual void setElementText(const std::string &text);

  /** @cond doxygenLibsbmlInternal */
  /*
   * Writes (serializes) this SBML object by writing it to XMLOutputStream.
   */
  void write (XMLOutputStream& stream) const;
  /** @endcond */

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
   * Adds the text rendering attributes of the given Text object
   * to the given XMLAttributes object.
   */
  static void addTextAttributes(const Text& text,XMLAttributes& att);
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

#endif /* Text_H__ */
