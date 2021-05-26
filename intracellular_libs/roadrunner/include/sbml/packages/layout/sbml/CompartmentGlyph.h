/**
 * @file    CompartmentGlyph.h
 * @brief   Definition of CompartmentGlyph for SBML Layout.
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
 * @class CompartmentGlyph
 * @sbmlbrief{layout} A glyph for an SBML compartment.
 */

#ifndef CompartmentGlyph_H__
#define CompartmentGlyph_H__

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/layout/common/layoutfwd.h>


#ifdef __cplusplus 


#include <string>
#include <sbml/packages/layout/sbml/GraphicalObject.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN CompartmentGlyph : public GraphicalObject
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mCompartment;
  double      mOrder;
  bool        mIsSetOrder;

  /** @endcond */

  friend class LayoutHandler;


public:
        
  /**
   * Default Constructor which creates a new CompartmentGlyph.  Id and
   * associated compartment id are unset.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  CompartmentGlyph (unsigned int level      = LayoutExtension::getDefaultLevel(),
                    unsigned int version    = LayoutExtension::getDefaultVersion(),
                    unsigned int pkgVersion = LayoutExtension::getDefaultPackageVersion());

  /**
   * Constructor.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param layoutns the LayoutPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   CompartmentGlyph(LayoutPkgNamespaces* layoutns);

        
  /**
   * Constructor which creates a new CompartmentGlyph with the given @p id.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
  CompartmentGlyph (LayoutPkgNamespaces* layoutns, const std::string& id);

  /**
   * Constructor which creates a new CompartmentGlyph.  Id and associated
   * compartment id are set to copies of the values given as arguments.
   *
   * (FOR BACKWARD COMPATIBILITY)
   */
  CompartmentGlyph (LayoutPkgNamespaces* layoutns, 
                    const std::string& id, const std::string& compartmentId);


  /**
   * Creates a new CompartmentGlyph from the given XMLNode
   *
   * (FOR BACKWARD COMPATIBILITY)
   */
   CompartmentGlyph(const XMLNode& node, unsigned int l2version=4);

  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
   CompartmentGlyph(const CompartmentGlyph& source);

  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  virtual  CompartmentGlyph& operator=(const CompartmentGlyph& source);

  /**
   * Destructor.
   */
  virtual ~CompartmentGlyph ();

  /**
   * Returns the id of the associated compartment.
   *
   * @return the value of the "compartment" attribute.
   */
  const std::string& getCompartmentId () const;
        
  /**
   * Sets the id of the associated compartment.
   *
   * @param id the identifier of a Compartment object defined elsewhere
   * in this Model.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setCompartmentId (const std::string& id);

  /**
   * Returns @c true if the id of the associated compartment is not the empty
   * string.
   *
   * @return @c true if the "compartment" attribute of this CompartmentGlyph is
   * set, @c false otherwise.
   */
  bool isSetCompartmentId () const;
     
  /**
   * Returns the compartment order.
   *
   * @return the value of the "order" attribute.
   */
  double getOrder () const;
        
  /**
   * Sets the compartment order.
   *
   * @param order the value to use for the "order" attribute.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int setOrder (double order);

  /**
   * Unsets the compartment order.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int unsetOrder ();
  
  /**
   * Returns @c true if the compartment order has been set.
   *
   * @return @c true if the "order" attribute of this CompartmentGlyph is
   * set, @c false otherwise.
   */
  bool isSetOrder () const;
   
  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);

  /**
   * Calls initDefaults from GraphicalObject.
   */
  void initDefaults ();

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
   * Creates and returns a deep copy of this CompartmentGlyph.
   * 
   * @return a (deep) copy of this CompartmentGlyph.
   */
  virtual CompartmentGlyph* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LAYOUT_COMPARTMENTGLYPH, SBMLLayoutTypeCode_t}
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


   /**
    * Creates an XMLNode object from this CompartmentGlyph.
    *
    * @return an XMLNode object based off of the information in this CompartmentGlyph.
    */
    virtual XMLNode toXML() const;
    
protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase*
  createObject (XMLInputStream& stream);
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


#endif /* __cplusplus */


#ifndef SWIG


BEGIN_C_DECLS


/**
 * Creates a new CompartmentGlyph and returns the pointer to it.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
CompartmentGlyph_t *
CompartmentGlyph_create ();


/**
 * Creates a new CompartmentGlyph from a template.
 *
 * @param temp the CompartmentGlyph_t structure to copy.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
CompartmentGlyph_t *
CompartmentGlyph_createFrom (const CompartmentGlyph_t *temp);

/**
 * Creates a new CompartmentGlyph with the given @p sid
 *
 * @param sid the id of the created CompartmentGlyph_t.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
CompartmentGlyph_t *
CompartmentGlyph_createWith (const char *sid);

/**
 * Creates a new CompartmentGlyph with the given @p sid
 *
 * @param sid the id of the created CompartmentGlyph_t.
 * @param compId the compartment id of the created CompartmentGlyph_t.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
CompartmentGlyph_t *
CompartmentGlyph_createWithCompartmentId (const char *sid, const char *compId);

/**
 * Frees the memory taken by the given compartment glyph.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
void
CompartmentGlyph_free (CompartmentGlyph_t *cg);


/**
 * Sets the reference compartment for the compartment glyph.
 *
 * @param cg the CompartmentGlyph_t structure.
 * @param id the string to use as the compartment id.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
void
CompartmentGlyph_setCompartmentId (CompartmentGlyph_t *cg, const char *id);

/**
 * Gets the reference compartments id for the given compartment glyph.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
const char *
CompartmentGlyph_getCompartmentId (const CompartmentGlyph_t *cg);

/**
 * Returns 0 if the reference compartment has not been set for this glyph
 * and 1 otherwise.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
int
CompartmentGlyph_isSetCompartmentId (const CompartmentGlyph_t *cg);

/**
 * Returns the compartment order.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
double
CompartmentGlyph_getOrder (const CompartmentGlyph_t *cg);
      
/**
 * Sets the compartment order
 *
 * @param cg the CompartmentGlyph_t structure.
 * @param order the value to use as the order of the CompartmentGlyph_t.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
int
CompartmentGlyph_setOrder (CompartmentGlyph_t *cg, double order);

/**
 * Sets the compartment order
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
int
CompartmentGlyph_unsetOrder (CompartmentGlyph_t *cg);

/**
 * Returns true if the compartment order has been set
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
int
CompartmentGlyph_isSetOrder (const CompartmentGlyph_t *cg);

/**
 * Calls initDefaults from GraphicalObject.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
void
CompartmentGlyph_initDefaults (CompartmentGlyph_t *cg);

/**
 * @return a (deep) copy of this CompartmentGlyph_t.
 *
 * @param cg the CompartmentGlyph_t structure.
 *
 * @memberof CompartmentGlyph_t
 */
LIBSBML_EXTERN
CompartmentGlyph_t *
CompartmentGlyph_clone (const CompartmentGlyph_t *cg);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* CompartmentGlyph_H__ */
