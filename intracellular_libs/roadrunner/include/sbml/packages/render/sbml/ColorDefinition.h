/**
 * @file    ColorDefinition.h
 * @brief   class for rgba color values
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
 * @class ColorDefinition
 * @brief LibSBML implementation for the %ColorDefinition construct from 
 * the SBML render extension.
 *
 * A @em ColorDefinition specifies an id for a certain RGBA value which can
 * then be referenced by this id in other render extension constructs.
 * The use of ids like e.g. "lightyellow" might be more descriptive than the
 * corresponding RGBA value.
 *
 * A ColorDefinition has two mandatory attributes which are the id for the
 * color definition and the corresponding RGBA value. The RGBA value has the 
 * same notation as in HTML files or CSS style sheets. It starts with the '#'
 * character followed by 8 digit hexadecimal string. Optionally the alpha part
 * can be omited in which case it defaults to FF.
 *
 * Valid value string are e.g. "#000000" or "#000000FF" for fully opaque black
 * or "#FF000010" for an almost completly transparent red.
 *
 * Internally the RGBA components are stored as integer values in the range of 0 to 255
 * and most methods use integer values instead of the hexadecimal value string.
 *
 * 
 * @class ListOfColorDefinitions
 * @brief LibSBML implementation for a container which holds zero or more ColorDefinition
 * objects.
 */

#ifndef ColorDefinition_H__
#define ColorDefinition_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <sbml/packages/render/extension/RenderExtension.h>

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ColorDefinition : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId;
  unsigned char mRed;
  unsigned char mGreen;
  unsigned char mBlue;
  unsigned char mAlpha;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new ColorDefinition object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  ColorDefinition (unsigned int level      = RenderExtension::getDefaultLevel(),
                   unsigned int version    = RenderExtension::getDefaultVersion(),
                   unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new ColorDefinition object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  ColorDefinition (RenderPkgNamespaces* renderns);

  /**
   * Creates a new ColorDefinition object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ColorDefinition object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   * @param node the XMLNode object reference that describes the ColorDefinition
   * object to be instantiated.
   */
  ColorDefinition(const XMLNode& node, unsigned int l2version=4);

#ifndef OMIT_DEPRECATED
  /**
   * Constructor which sets the ColorDefinition to the given RGBA values.
   *
   * @param r Red component value. Has to be in the range of 0 to 255.
   * @param g Green component value. Has to be in the range of 0 to 255.
   * @param b Blue component value. Has to be in the range of 0 to 255.
   * @param a Alpha component value. Has to be in the range of 0 to 255. 
   * The alpha component can be omitted. In that case it has a default value of 255.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  ColorDefinition(RenderPkgNamespaces* renderns, unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor which sets the ColorDefinition to completely opaque
   * black and sets the id to the given string.
   *
   * @param id the id of the color definition. The user has to make sure 
   * that the id is unique within the given set of color definitions. 
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  ColorDefinition(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
  /**
   * Constructor which sets the ColorDefinition to the given RGBA values
   * and sets the id.
   *
   * @param id the id of the color definition. The user has to make sure 
   * that the id is unique within the given set of color definitions. 
   * @param r Red component value. Has to be in the range of 0 to 255.
   * @param g Green component value. Has to be in the range of 0 to 255.
   * @param b Blue component value. Has to be in the range of 0 to 255.
   * @param a Alpha component value. Has to be in the range of 0 to 255. 
   * The alpha component can be omitted. In that case it has a default value of 255.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  ColorDefinition(RenderPkgNamespaces* renderns, const std::string& id,unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
#endif // OMIT_DEPRECATED

  /**
   * Destroys this ColorDefinition object.
   */
  virtual ~ColorDefinition();

  /**
   * Returns the red color component.
   *
   * @return the red color component for the ColorDefinition.
   */
  unsigned char getRed() const;

  /**
   * Returns the green color component.
   *
   * @return the green color component for the ColorDefinition.
   */
  unsigned char getGreen() const;

  /**
   * Returns the blue color component.
   *
   * @return the blue color component for the ColorDefinition.
   */
  unsigned char getBlue() const;

  /**
   * Returns the alpha color component.
   *
   * @return the alpha color component for the ColorDefinition.
   */
  unsigned char getAlpha() const;

  /**
   * Sets the red color component.
   *
   * @param c the new red component value for the color definition.
   */
  void setRed(unsigned char c);

  /**
   * Sets the green color component.
   *
   * @param c the new green component value for the color definition.
   */
  void setGreen(unsigned char c);

  /**
   * Sets the blue color component.
   *
   * @param c the new blue component value for the color definition.
   */
  void setBlue(unsigned char c);

  /**
   * Sets alpha red color component.
   *
   * @param c the new alpha component value for the color definition.
   */
  void setAlpha(unsigned char c);

  /**
   * Sets the red green, blue and alpha color component.
   * The alpha value is optional and defaults to 255 if not given.
   * @param r Red component value. Has to be in the range of 0 to 255.
   * @param g Green component value. Has to be in the range of 0 to 255.
   * @param b Blue component value. Has to be in the range of 0 to 255.
   * @param a Alpha component value. Has to be in the range of 0 to 255. 
   * The alpha component can be omitted. In that case it has a default value of 255.
   */
  void setRGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);

  /**
   * Sets the color value from a given value string.
   * If the string is not a valid value string, the color value is set to
   * black and false is returned.
   *
   * @param valueString A const reference to a string that represents a valid color value,
   * e.g. "#FFFFFFFF" for fully opaque white.
   *
   * @return true or false depending on whether setting the color value from the string
   * was successfull.
   */
  bool setColorValue(const std::string& valueString);

  /**
   * Creates a string the represents the current color value.
   *
   * @return The string representation of the color value.
   */
  std::string createValueString() const;

  /**
   * Accepts the given SBMLVisitor for this instance of ColorDefinition.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this ColorDefinition object.
   * 
   * @return a (deep) copy of this ColorDefinition object
   */
  virtual ColorDefinition* clone () const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  virtual const std::string& getElementName () const;

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
  virtual int getTypeCode () const;


  /**
   * Creates an XMLNode object from this ColorDefinition object.
   *
   * @return the XMLNode with the XML representation for the 
   * ColorDefinition object.
   */
  virtual XMLNode toXML() const;

  /**
   * Returns the value of the "id" attribute of this ColorDefinition.
   *
   * @return the id of the ColorDefinition
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
   * Unsets the value of the "id" attribute of this ColorDefinition.
   */
  virtual int unsetId ();

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
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
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


};


class LIBSBML_EXTERN ListOfColorDefinitions : public ListOf
{
public:

  /**
   * Creates a new ListOfColorDefinitions object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfColorDefinitions object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfColorDefinitions
   * object to be instantiated.
   */
  ListOfColorDefinitions(const XMLNode& node);  

  /**
   * Creates and returns a deep copy of the ListOfColorDefinitions object.
   *
   * @return a (deep) copy of this ListOfColorDefinitions
   */
  virtual ListOfColorDefinitions* clone () const;

  /**
   * Constructor which instantiates an empty ListOfColorDefinitions object.
   */
  ListOfColorDefinitions(unsigned int level      = RenderExtension::getDefaultLevel(),
                         unsigned int version    = RenderExtension::getDefaultVersion(),
                         unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfColorDefinitions(RenderPkgNamespaces* renderns);

  /**
   * Copy constructor. Creates a copy of this ListOfColorDefinitions object.
   */
  ListOfColorDefinitions(const ListOfColorDefinitions& source);

  /**
   * Assignment operator for ListOfColorDefinitions objects.
   */
  ListOfColorDefinitions& operator=(const ListOfColorDefinitions& source);


  /**
   * Returns the XML element name of this object, which for
   * ListOfColorDefinitions, is always @c "listOfColorDefinitions".
   * 
   * @return the name of this element, i.e., @c "listOfColorDefinitions".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfColorDefinitions object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfColorDefinitions object.
   */
  XMLNode toXML() const;

  /**
   * Returns a pointer to the ColorDefinition with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the ColorDefinition object to be returned
   * 
   * @return pointer to the ColorDefinition at the given index or NULL.
   */
  ColorDefinition* get(unsigned int i);

  /**
   * Returns a const pointer to the ColorDefinition with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the ColorDefinition object to be returned
   * 
   * @return const pointer to the ColorDefinition at the given index or NULL.
   */
  const ColorDefinition* get(unsigned int i) const;

  /**
   * Returns a pointer to the ColorDefinition with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the ColorDefinition object to be returned
   * 
   * @return pointer to the ColorDefinition at the given @p id or @c NULL.
   */
  ColorDefinition* get(const std::string& id);

  /**
   * Returns a const pointer to the ColorDefinition with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the ColorDefinition object to be returned
   * 
   * @return const pointer to the ColorDefinition at the given @p id or @c NULL.
   */
  const ColorDefinition* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfColorDefinitions items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual ColorDefinition* remove (unsigned int n);


  /**
   * Removes item in this ListOfColorDefinition items with the given identifier.
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
  virtual ColorDefinition* remove (const std::string& sid);


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

  static const std::string ELEMENT_NAME;
  /** @endcond */

};

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* ColorDefinition_H__ */
