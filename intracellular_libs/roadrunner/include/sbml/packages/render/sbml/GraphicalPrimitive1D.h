/**
 * @file    GraphicalPrimitive1D.h
 * @brief   abstract base class for 1D graphical objects
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
 * @class GraphicalPrimitive1D
 * @brief base class for all graphical primitives which implements all 1D attributes
 *
 * The GraphicalPrimitive1D class implements attributes and methods necessary for 1D objects
 * like lines. The attributes that are implemented are a stroke color, a stroke width and 
 * a stroke dasharray for dashed line drawing.
 *
 * Additionally this class adds an id attribute with which all graphical primitives can be
 * referenced.
 *
 * The GraphicalPrimitive1D class is derived from Transformation2D and inherits all its methods
 * and attributes.
 */

#ifndef GraphicalPrimitive1D_H__
#define GraphicalPrimitive1D_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/sbml/Transformation2D.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <string>
#include <vector>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN GraphicalPrimitive1D : public Transformation2D
{
protected:
  /** @cond doxygenLibsbmlInternal */
////   std::string mId;
   std::string mStroke;
   double mStrokeWidth;
   std::vector<unsigned int> mStrokeDashArray;
  /** @endcond */

public:
   /**
    * Creates a new GraphicalPrimitive1D object with the given SBML level
    * and SBML version.
    *
    * @param level SBML level of the new object
    * @param level SBML version of the new object
    */
   GraphicalPrimitive1D (unsigned int level      = RenderExtension::getDefaultLevel(),
                    unsigned int version    = RenderExtension::getDefaultVersion(),
                    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


   /**
    * Creates a new GraphicalPrimitive1D object with the given SBMLNamespaces.
    *
    * @param sbmlns The SBML namespace for the object.
    */
   GraphicalPrimitive1D (RenderPkgNamespaces* renderns);

   /**
    * Copy constructor.
    */
   GraphicalPrimitive1D(const GraphicalPrimitive1D& other);

   /**
    * Creates a new GraphicalPrimitive1D object from the given XMLNode object.
    * The XMLNode object has to contain a valid XML representation of a 
    * GraphicalPrimitive1D object as defined in the render extension specification.
    * This method is normally called when render information is read from a file and 
    * should normally not have to be called explicitely.
    *
    * @param node the XMLNode object reference that describes the GraphicalPrimitive1D
    * object to be instantiated.
    */
   GraphicalPrimitive1D(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this GraphicalPrimitive1D object.
   */
  virtual ~GraphicalPrimitive1D ();



#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a GraphicalPrimitive1D.
   * The transformation properties are not set, neither is the stroke or the stroke width.
   * The id is set to the given string.
   *
   * @param id The id for the GraphicalPrimitive1D object
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  GraphicalPrimitive1D(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Sets the stroke color to the given color definition id or color value string.
   * (@see ColorDefinition)
   *
   * @param stroke id of a ColorDefinition object or a valid color value string.
   */
  void setStroke(const std::string& stroke);
  
  /**
   * Sets the stroke width.
   *
   * @param width New width for strokes. Should be a positive value.
   */
  void setStrokeWidth(double width);
  
  /**
   * Sets the dasharray to the values in the given array.
   *
   * @param array Array of alternating stroke and gap length values.
   */
  void setDashArray(const std::vector<unsigned int>& array);
  
  /**
   * Sets the dasharray from the given string.
   * If the string is not a valid dasharray string, false
   * is returned and the dasharray remains in the state is was
   * before the call.
   *
   * The individual numerical values in the string have to be separated by kommas.
   *
   * @param arrayString a string with number representing a dash array.
   *
   * @return true is setting the dasharray from the string succeed or false otherwise.
   */
  bool setDashArray(const std::string& arrayString);
  
  
  /**
   * Returns the stroke color.
   *
   * @return the id of the color definition or a color value string.
   */
  const std::string& getStroke() const;
  
  /**
   * Returns the stroke width.
   *
   * @return the stroke width
   */
  double getStrokeWidth() const;
  
  /**
   * Returns a const reference to the stroke dasharray.
   *
   * @return const reference to stroke dash array
   */
  const std::vector<unsigned int>& getDashArray() const;
  
  
  /**
   * Returns a reference to the stroke dasharray.
   *
   * @return reference to stroke dash array
   */
  std::vector<unsigned int>& getDashArray();
  
  /**
   * Returns true is the stroke width has been set or false otherwise.
   * The stroke width is considered set if it is not NaN.
   *
   * @return true is the stroke width is set.
   */
  bool isSetStrokeWidth() const;
  
  /**
   * Returns true is the stroke has been set or false otherwise.
   * The stroke color is considered set if the string is not empty.
   *
   * @return true if the stroke color is set.
   */
  bool isSetStroke() const;
  
  /**
   * Returns true is the dash array has been set or false otherwise.
   * The array is considered set if it is not empty and if the first entry is
   * not NaN.
   *
   * @true if the dasharray is set.
   */
  bool isSetDashArray() const;
  
  /** 
   * Returns the number of defined dashes.
   */
  unsigned int getNumDashes() const;

  /**
   * Returns the dash at the given index. 
   */
  unsigned int getDashByIndex(unsigned int index) const;
  
  /**
   * Adds a dash at the end of the current list
   */
  void addDash(unsigned int dash);

  /**
   * Clears all defined dashes.
   */
  void clearDashes();

  /** 
   * Sets the dash at the given position.
   */
  void setDashByIndex(unsigned int index, unsigned int dash);

  /** 
   * Inserts the dash at the given position.
   */
  void insertDash(unsigned int index, unsigned int dash);


  /**
   * Removes the dash at the given index
   */
  void removeDash(unsigned int index);

  /**
   * Creates an XMLNode object from this GraphicalPrimitive1D object.
   *
   * @return the XMLNode with the XML representation for the 
   * GraphicalPrimitive1D object.
   *
   * This method is purely virtual and has to be implemented by subclasses.
   */
  virtual XMLNode toXML() const = 0;
  
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
   * Adds all set attributes specific to the given GraphicalPrimitive1D objects to the given
   * XMLAttributes object.
   */
  static void addGraphicalPrimitive1DAttributes(const GraphicalPrimitive1D& primitive,XMLAttributes& attributes);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   *  This method parses a dasharray string into the given vector.
   *  The vector is first cleared.
   *  If the dasharray is invalid, false is returned.
   */
  static bool parseDashArray(const std::string& s,std::vector<unsigned int>& array);
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

#endif /* GraphicalPrimitive1D_H__ */
