/**
 * @file    BoundingBox.h
 * @brief   Definition of BoundingBox for SBML Layout.
 * @author  Ralph Gauges
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
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2004-2008 by European Media Laboratories Research gGmbH,
 *     Heidelberg, Germany
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class BoundingBox
 * @sbmlbrief{layout} A bounding box for an item in a diagram.
 */

#ifndef BoundingBox_H__
#define BoundingBox_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/layout/common/layoutfwd.h>

#ifdef __cplusplus


#include <string>

#include <sbml/SBase.h>

#include <sbml/packages/layout/sbml/Point.h>
#include <sbml/packages/layout/sbml/Dimensions.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN BoundingBox : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
//  std::string mId;
  Point mPosition;
  Dimensions mDimensions;
  bool mPositionExplicitlySet;
  bool mDimensionsExplicitlySet;
  /** @endcond */


public:
        
  /**
   * Default Constructor.  Set position and dimensions to (0.0,0.0,0.0) and
   * the id to an empty string.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  BoundingBox (unsigned int level      = LayoutExtension::getDefaultLevel(),
               unsigned int version    = LayoutExtension::getDefaultVersion(),
               unsigned int pkgVersion = LayoutExtension::getDefaultPackageVersion());


  /**
   * Creates a new BoundingBox object with the given LayoutPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param layoutns the LayoutPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  BoundingBox(LayoutPkgNamespaces* layoutns);

  
  /**
   * Copy constructor.
   *
   * @param orig the instance to copy.
   */
  BoundingBox(const BoundingBox& orig); 

  /**
   * Constructor.  Set position and dimensions to (0.0,0.0,0.0) and the id to
   * a copy of the given string.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
  BoundingBox (LayoutPkgNamespaces* layoutns, const std::string id);
        
  /**
   * Constructor which sets the id, the coordinates and the dimensions to
   * the given 2D values.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
  BoundingBox (LayoutPkgNamespaces* layoutns, const std::string id, double x, double y,
               double width, double height);
        
  /**
   * Constructor which sets the id, the coordinates and the dimensions to
   * the given 3D values.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
  BoundingBox (LayoutPkgNamespaces* layoutns, const std::string id, double x, double y, double z,
               double width, double height, double depth);
  
  /**
   * Constructor which sets the id, the coordinates and the dimensions to
   * the given values.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
  BoundingBox (LayoutPkgNamespaces* layoutns, const std::string id, const Point* p, const Dimensions* d);

  /**
   * Creates a new BoundingBox from the given XMLNode
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
   BoundingBox(const XMLNode& node, unsigned int l2version=4);

  /**
   * Destructor.
   */
  virtual ~BoundingBox ();
        
  /**
   * Assignment operator
   *
   * @param orig the object whose values are used as the basis of the
   * assignment.
   */
  BoundingBox& operator=(const BoundingBox& orig);

  /**
   * Returns the value of the "id" attribute of this BoundingBox.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this BoundingBox.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * BoundingBox's "id" attribute has been set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId () const;

  
  /**
   * Sets the value of the "id" attribute of this BoundingBox.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Unsets the value of the "id" attribute of this BoundingBox.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId ();


  /**
   * Returns the position of the BoundingBox as const reference to a Point
   * object.
   *
   * @return the Point representing the position.
   */
  const Point* getPosition () const;

  /**
   * Returns the dimensions of the BoundingBox as const reference to a
   * Dimensions object.
   *
   * @return the Dimensions representing the dimensions.
   */
  const Dimensions* getDimensions () const;
     
  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @param filter a pointer to an ElementFilter, which causes the function 
   * to return only elements that match a particular set of constraints.  
   * If NULL (the default), the function will return all child objects.
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Returns the position of the BoundingBox as reference to a Point object.
   *
   * @return the Point representing the position.
   */
  Point* getPosition ();
        
  /**
   * Returns the dimensions of the BoundingBox as reference to a Dimensions
   * object.
   *
   * @return the Dimensions representing the dimensions.
   */
  Dimensions* getDimensions ();
        
  /**
   * Sets the position to a copy of the Point object given.
   *
   * @param p the Point to use as the position.
   */
  void setPosition (const Point* p);
        
  /**
   * Sets the dimensions to a copy of the Dimensions object given.
   *
   * @param d the Dimensions to use.
   */
  void setDimensions (const Dimensions* d);  
    

  /**
   * Return @c true or @c false based on whether Dimensions have been set.
   *
   * @return @c true if the Dimensions were set explicitly, @c false otherwise.
   */
  bool getDimensionsExplicitlySet() const;

  /**
   * Return @c true or @c false based on whether Position has been set.
   *
   * @return @c true if the Position was set explicitly, @c false otherwise.
   */
  bool getPositionExplicitlySet() const;

  /**
   * Does nothing since there are no defaults for a BoundingBox. 
   */
  void initDefaults ();


  /**
   * Get the x offset of the bounding box.
   *
   * @return the double value of the x offset.
   */
  double x() const;
  
  /**
   * Get the y offset of the bounding box.
   *
   * @return the double value of the y offset.
   */
  double y() const;
  
  /**
   * Get the z offset of the bounding box.
   *
   * @return the double value of the z offset.
   */
  double z() const;
  
  /**
   * Get the width of the bounding box.
   *
   * @return the double value of the width.
   */
  double width() const;
  
  /**
   * Get the height of the bounding box.
   *
   * @return the double value of the height.
   */
  double height() const;
  
  /**
   * Get the depth of the bounding box.
   *
   * @return the double value of the depth.
   */
  double depth() const;

  /**
   * Set x offset of the bounding box.
   *
   * @param x the double value to set the x offset to.
   */
  void setX(double x);

  /**
   * Set y offset of the bounding box.
   *
   * @param y the double value to set the y offset to.
   */
  void setY(double y);

  /**
   * Set z offset of the bounding box.
   *
   * @param z the double value to set the z offset to.
   */
  void setZ(double z);

  /**
   * Set width of the bounding box.
   *
   * @param width the double value to set the width to.
   */
  void setWidth(double width);

  /**
   * Set height of the bounding box.
   *
   * @param height the double value to set the height to.
   */
  void setHeight(double height);

  /**
   * Set depth of the bounding box.
   *
   * @param depth the double value to set the depth to.
   */
  void setDepth(double depth);

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
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
   * Returns the XML element name of
   * this SBML object.
   *
   * @return the string of the name of this element.
   */
  virtual const std::string& getElementName () const ;

  /**
   * Creates and returns a deep copy of this BoundingBox.
   * 
   * @return a (deep) copy of this BoundingBox.
   */
  virtual BoundingBox* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LAYOUT_BOUNDINGBOX, SBMLLayoutTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


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
   
   
   /**
    * Creates an XMLNode object from this.
    *
    * @return an XMLNode representing this object.
    */
   XMLNode toXML() const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use.
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
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
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
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parent's implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
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


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Function that creates a BoundingBox_t structure with position set to
 * (0.0,0.0,0.0) and dimensions set to (0.0,0.0,0.0). The id is set to the
 * empty string.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
BoundingBox_t *
BoundingBox_create (void);

/**
 * ZFunction that creates a BoundingBox_t structure with position set to
 * (0.0,0.0,0.0) and dimensions set to (0.0,0.0,0.0).  The id is set to the
 * given string.
 *
 * @param sid the id of the created BoundingBox_t.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
BoundingBox_t *
BoundingBox_createWith (const char *sid);

/**
 * Function that creates a BoundingBox_t structure with the coordinates and
 * sizes given as arguments. The id is set to the empty string.
 *
 * @param sid the id of the created BoundingBox_t.
 * @param x the value of the x coordinate of the position Point_t.
 * @param y the value of the y coordinate of the position Point_t.
 * @param z the value of the z coordinate of the position Point_t.
 * @param width  the value of the width.
 * @param height the value of the height.
 * @param depth  the value of the depth.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
BoundingBox_t *
BoundingBox_createWithCoordinates (const char *sid, double x, double y, double z,
                                   double width, double height, double depth);

/**
 * Frees all memory taken by the given BoundingBox_t structure.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_free (BoundingBox_t *bb);

/**
 * Does nothing since no defaults are defined for BoundingBox.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_initDefaults (BoundingBox_t *bb);

/**
 * Returns the position as a Point_t structure.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
Point_t *
BoundingBox_getPosition (BoundingBox_t *bb);

/**
 * Returns the dimensions as a Dimensions_t structure.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
Dimensions_t *
BoundingBox_getDimensions (BoundingBox_t *bb);

/**
 * Sets the position to a copy of the Point_t structure given as argument.
 *
 * @param bb the BoundingBox_t structure.
 * @param p the Point_t to use as the position of the BoundingBox_t.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setPosition (BoundingBox_t *bb, const Point_t *p);

/**
 * Sets the dimensions to a copy of the Dimensions_t structure given.
 *
 * @param bb the BoundingBox_t structure.
 * @param d the Dimensions_t to use as the dimensions of the BoundingBox_t.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setDimensions (BoundingBox_t *bb, const Dimensions_t *d);

/**
 * Sets the x offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param x the value of the x coordinate.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setX(BoundingBox_t* bb, double x);


/**
 * Sets the y offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param y the value of the y coordinate.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setY(BoundingBox_t* bb, double y);


/**
 * Sets the z offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param z the value of the z coordinate.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setZ(BoundingBox_t* bb, double z);


/**
 * Sets the width of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param width  the value of the width.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setWidth(BoundingBox_t* bb, double width);


/**
 * Sets the height of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param height the value of the height.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setHeight(BoundingBox_t* bb, double height);


/**
 * Sets the depth of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 * @param depth  the value of the depth.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_setDepth(BoundingBox_t* bb, double depth);

/**
 * Returns the x offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_x(BoundingBox_t* bb);


/**
 * Returns the y offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_y(BoundingBox_t* bb);


/**
 * Returns the z offset of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_z(BoundingBox_t* bb);


/**
 * Returns the width of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_width(BoundingBox_t* bb);

/**
 * Returns the height of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_height(BoundingBox_t* bb);

/**
 * Returns the depth of the bounding box.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
double
BoundingBox_depth(BoundingBox_t* bb);

/**
 * @return a (deep) copy of this BoundingBox_t.
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
BoundingBox_t *
BoundingBox_clone (const BoundingBox_t *bb);


/**
 * Returns non-zero if the id is set
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
int
BoundingBox_isSetId (const BoundingBox_t *bb);


/**
 * Returns the id
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
const char *
BoundingBox_getId (const BoundingBox_t *bb);


/**
 * Sets the id
 *
 * @param bb the BoundingBox_t structure.
 * @param sid the value of the id.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
int
BoundingBox_setId (BoundingBox_t *bb, const char *sid);


/**
 * Unsets the id
 *
 * @param bb the BoundingBox_t structure.
 *
 * @memberof BoundingBox_t
 */
LIBSBML_EXTERN
void
BoundingBox_unsetId (BoundingBox_t *bb);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* BoundingBox_H__ */
