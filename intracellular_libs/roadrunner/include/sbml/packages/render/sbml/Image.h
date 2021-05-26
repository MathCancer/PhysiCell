/**
 * @file    Image.h
 * @brief   class representing a bitmap image
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
 * @class Image
 * @brief implementation of the Image concept from the SBML
 * render extension
 *
 * The image class represents a bitmap image representation. It is derived from Transformation2D
 * and inherits all its attributes.
 *
 * There is an attribute that can be used to specify a file URL where that specifies where the image 
 * data can be found. If the URL is a relative path, it is considered to be relative to
 * the document that contains the render extension info.
 * The path should be the location of a JPEG or PNG image, other image formats are currently not supported
 * by the SBML render extension.
 *
 * Additionally it provides an id attribute as well as attributes that determine the dimensions
 * and the position of the image relative to its viewport.
 */

#ifndef Image_H__
#define Image_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/Transformation2D.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>
#include <sbml/packages/render/extension/RenderExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Image : public Transformation2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId;
  RelAbsVector mX;
  RelAbsVector mY;
  RelAbsVector mZ;
  RelAbsVector mWidth;
  RelAbsVector mHeight;
  std::string mHRef;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new Image object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Image (unsigned int level      = RenderExtension::getDefaultLevel(),
         unsigned int version    = RenderExtension::getDefaultVersion(),
         unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Image object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Image (RenderPkgNamespaces* renderns);

  /**
   * Creates a new Image object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Image object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Image
   * object to be instantiated.
   */
  Image(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this Image object.
   */
  virtual ~Image ();


#ifndef OMIT_DEPRECATED
  /**
   * Instantiates an Image object with the given @p id.
   * The image reference is unset, the position and the dimensions
   * values of the image are set to 0.
   *
   * For the image to be valid, the reference has to be set and it has to 
   * have dimensions different from and larger than 0.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Image(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets the position of the image relative to its viewport.
   * The position can either be specified in relative or in absolut coordinates
   * or a combination of both.
   * The z coordinatee can be omitted. In that case it is set to 0.
   *
   * @param x x coordinate of the image position
   * @param y y coordinate of the image position
   * @param z z coordinate of the image position
   */
  void setCoordinates(const RelAbsVector& x,const RelAbsVector& y,const RelAbsVector& z=RelAbsVector(0.0,0.0));

  /**
   * Sets the x coordinate of the image position.
   * The position can either be specified in relative or in absolut coordinates
   * or a combination of both.
   *
   * @param x x coordinate of the image position
   */
  void setX(const RelAbsVector& coord);

  /**
   * Sets the y coordinate of the image position.
   * The position can either be specified in relative or in absolut coordinates
   * or a combination of both.
   *
   * @param y y coordinate of the image position
   */
  void setY(const RelAbsVector& coord);

  /**
   * Sets the z coordinate of the image position.
   * The position can either be specified in relative or in absolut coordinates
   * or a combination of both.
   *
   * @param z z coordinate of the image position
   */
  void setZ(const RelAbsVector& coord);

  /**
   * Returns a reference to the x coordinate of the image position.
   *
   * @return reference to the x coordinate of the image position.
   */
  RelAbsVector& getX();

  /**
   * Returns a reference to the y coordinate of the image position.
   *
   * @return reference to the y coordinate of the image position.
   */
  RelAbsVector& getY();

  /**
   * Returns a reference to the z coordinate of the image position.
   *
   * @return reference to the z coordinate of the image position.
   */
  RelAbsVector& getZ();

  /**
   * Returns a const reference to the x coordinate of the image position.
   *
   * @return const reference to the x coordinate of the image position.
   */
  const RelAbsVector& getX() const;

  /**
   * Returns a const reference to the y coordinate of the image position.
   *
   * @return const reference to the y coordinate of the image position.
   */
  const RelAbsVector& getY() const;

  /**
   * Returns a const reference to the z coordinate of the image position.
   *
   * @return const reference to the z coordinate of the image position.
   */
  const RelAbsVector& getZ() const;

  /**
   * Sets the dimensions of the image.
   * The dimensions can be set as relative values or absolute values, or 
   * a combination of both.
   *
   * @param width the width of the image when rendered
   * @param height the height of the image when rendered
   */
  void setDimensions(const RelAbsVector& width,const RelAbsVector& height);

  /**
   * Sets the width of the image when rendered.
   * The width can be set as relative values or absolute values, or 
   * a combination of both.
   *
   * @param width the width of the image when rendered
   */
  void setWidth(const RelAbsVector& width);

  /**
   * Sets the height of the image when rendered.
   * The height can be set as relative values or absolute values, or 
   * a combination of both.
   *
   * @param height the height of the image when rendered
   */
  void setHeight(const RelAbsVector& height);

  /**
   * Returns a reference to the width of the image.
   *
   * @return reference to the width
   */
  RelAbsVector& getWidth();

  /**
   * Returns a reference to the height of the image.
   *
   * @return reference to the height
   */
  RelAbsVector& getHeight();

  /**
   * Returns a const reference to the width of the image.
   *
   * @return const reference to the width
   */
  const RelAbsVector& getWidth() const;

  /**
   * Returns a const reference to the height of the image.
   *
   * @return const reference to the height
   */
  const RelAbsVector& getHeight() const;

  /**
   * Sets the reference to the image location.
   * Relative paths are relative to the document that contains the render information.
   * The path should be the location to a JPEG or PNG bitmap image, other formats are
   * currently not supported.
   *
   * @param ref A URL string that specifies where the image is located on the disk.
   */
  void setImageReference(const std::string& ref);

  /**
   * Returns the image reference URL string.
   *
   * @return THe path to the image data as a string.
   */
  const std::string& getImageReference() const;

  /**
   * Returns true if the image reference has been set.
   * The image reference is considered set if the string does not
   * only contain whitespace characters.
   *
   * @return true if the image reference has been set.
   */
  bool isSetImageReference() const;

  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  bool accept(SBMLVisitor& visitor) const;

  /**
   * Creates and returns a deep copy of this Image object.
   *
   * @return a (deep) copy of this Image.
   */
  Image* clone() const;

  /**
   * Returns the XML element name of this object, which for
   * Image, is always @c "image".
   * 
   * @return the name of this element, i.e., @c "image".
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
   * @see getElementName()
   */
  virtual int getTypeCode() const;

  /**
   * Creates an XMLNode object from this Image object.
   *
   * @return the XMLNode with the XML representation for the 
   * Image object.
   */
  virtual XMLNode toXML() const;

  /**
   * Returns the value of the "id" attribute of this Image.
   *
   * @return the id of the Image
   */
  const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Image's "id" attribute has been set.
   *
   * @return returns true or false depending on whether the id on the 
   * Image has been set.
   */
  bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this Image.
   *
   * @param id the new id for the Image 
   *
   * @return status if the operation succeeded
   */
  int setId (const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this Image.
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

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* Image_H__ */
