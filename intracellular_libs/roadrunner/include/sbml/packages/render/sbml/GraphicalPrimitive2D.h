/**
 * @file    GraphicalPrimitive2D.h
 * @brief   abstract base class for 2D graphical objects
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
 * @class GraphicalPrimitive2D
 * @brief base class for all graphical primitives which implements all 2D attributes
 *
 * The GraphicalPrimitive2D class implements attributes and methods necessary for 2D objects
 * like rectangles, polygons or ellipses. The attributes that are implemented are a fill color
 * and a fill rule that specifies how the fill color is applied.
 *
 * The GraphicalPrimitive2D class is derived from GraphicalPrimitive1D and inherits all its methods
 * and attributes.
 */

#ifndef GraphicalPrimitvide2D_H__
#define GraphicalPrimitvide2D_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/GraphicalPrimitive1D.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN GraphicalPrimitive2D: public GraphicalPrimitive1D
{
public:
  enum FILL_RULE
  {
    UNSET,
    NONZERO,
    EVENODD,
    INHERIT
  };

protected:
  FILL_RULE mFillRule;
  std::string mFill;

public:
  /**
   * Creates a new GraphicalPrimitive2D object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  GraphicalPrimitive2D (unsigned int level      = RenderExtension::getDefaultLevel(),
                        unsigned int version    = RenderExtension::getDefaultVersion(),
                        unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new GraphicalPrimitive2D object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  GraphicalPrimitive2D (RenderPkgNamespaces* renderns);

  /**
   * Copy constructor.
   */
  GraphicalPrimitive2D(const GraphicalPrimitive2D& other);

  /**
   * Creates a new GraphicalPrimitive2D object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * GraphicalPrimitive2D object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the GraphicalPrimitive2D
   * object to be instantiated.
   */
  GraphicalPrimitive2D(const XMLNode& node, unsigned int l2Version=4);


  /**
   * Destroy this GraphicalPrimitive2D object.
   */
  virtual ~GraphicalPrimitive2D ();


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a GraphicalPrimitive2D.
   * The attributes inherited from GraphicalPrimitive1D are set as described
   * in the corresponding constructor for GraphicalPrimitive1D (@see GraphicalPrimitive1D).
   *
   * The fill and the fill rule are unset.
   * 
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  GraphicalPrimitive2D(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Set fill color to the id of a color definition, the id of a gradient
   * definition or a color value string.
   *
   * @param color the id of a color deifnition or a gradient or a color value string.
   */
  void setFillColor(const std::string& color);

  /**
   * Sets the fill rule. Possible values are GraphicalPrimitive2D::NONZERO,
   * GraphicalPrimitive2D::EVENODD, GraphicalPrimitive::INHERIT or
   * GraphicalPrimitive2D::UNSET.
   *
   * If the fill rule for an object is unset, it default to INHERIT,
   * which means that it inherits the attribute from it's parent group.
   * The topmost group in an object hierarchy has a default value for this 
   * attribute which is GraphicalPrimitive2D::NONZERO.
   *
   * For more details please consult the render extension specification.
   *
   * @param rule the fill rule to be set.
   */
  void setFillRule(FILL_RULE rule);

  /**
   * Returns the fill color.
   *
   * @return this id of the fill color or the fill gradient or the fill color value string.
   */
  const std::string& getFillColor() const;

  /**
   * Returns the fill rule.
   *
   * @return this fill rule enum
   */
  FILL_RULE getFillRule() const;

#ifndef OMIT_DEPRECATED
  /**
   * Returns true if the fill attribute is set or false otherwise.
   * The fill attribute is considered set if the string is not empty.
   *
   * This function is deprecated, please use isSetFillColor instead.
   *
   * @return true is the fill color is set.
   */
  bool isSetFill() const;
#endif // OMIT_DEPRECATED

  /**
   * Returns true if the fill attribute is set or false otherwise.
   * The fill attribute is considered set if the string is not empty.
   *
   * @return true is the fill color is set.
   */
  bool isSetFillColor() const;

  /**
   * Returns true if the fill rule attribute is set or false otherwise.
   * The fill rule is considered as set if it is not GraphicalPrimitive2D::UNSET.
   * 
   * @return true is the fill color is set.
   */
  bool isSetFillRule() const;

protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds all set attributes specific to the given GraphicalPrimitive2D objects to the given
   * XMLAttributes object.
   */
  static void addGraphicalPrimitive2DAttributes(const GraphicalPrimitive2D& gp,XMLAttributes& attr);
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

LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */

#endif /* GraphicalPrimitive2D_H__ */
