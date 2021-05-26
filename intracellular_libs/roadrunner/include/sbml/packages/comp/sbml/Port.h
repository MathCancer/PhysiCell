/**
 * @file    Port.h
 * @brief   Definition of Port, the SBase derived class of the comp package.
 * @author  Lucian Smith 
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *------------------------------------------------------------------------- -->
 *
 * @class Port
 * @sbmlbrief{comp} An interface to be used in composition.
 *
 * The Port class was introduced by the SBML Level&nbsp;3 @ref comp
 * (&ldquo;comp&rdquo;) package to allow a Model to define a standard
 * interface between it and other models that might use it as a submodel.  It
 * derives from the SBaseRef class, and the elements defined there refer to
 * elements in the same parent Model as the Port object.  A Port object
 * instance therefore uses those attributes to define a port for a component
 * in a model.  When other SBaseRef or SBaseRef-derived classes refer to a
 * Port object using a "portRef" attribute, the element being referenced is
 * the element the Port object itself points to.
 *
 * In the present formulation of the Hierarchical %Model Composition
 * package, the use of ports is not enforced, nor is there any
 * mechanism to restrict which ports may be used in what ways---they are
 * only an advisory construct.  Future versions of this SBML package may
 * provide additional functionality to support explicit restrictions on
 * port use.  For the present definition of Hierarchical %Model Composition,
 * users of models containing ports are encouraged to respect the modeler's
 * intention in defining ports, and use the port definitions to interact
 * with components through their ports (when they have ports defined)
 * rather than interact directly with the components.
 *
 * The required attribute "id" is used to give an identifier to a
 * Port object so that other objects can refer to it.  The attribute has
 * type PortSId and is essentially identical to the SBML
 * primitive type SId, except that its namespace is limited to
 * the identifiers of Port objects defined within a Model object.  In
 * parallel, the PortSId type has a companion type,
 * PortSIdRef, that corresponds to the SBML primitive type
 * SIdRef; the value space of PortSIdRef is limited
 * to PortSId values.  
 */


#ifndef Port_H__
#define Port_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/SBase.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/SBaseRef.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Port : public SBaseRef
{
protected:
  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
//  std::string   mName;
  /** @endcond */

public:

  /**
   * Creates a new Port with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Port(unsigned int level      = CompExtension::getDefaultLevel(),
       unsigned int version    = CompExtension::getDefaultVersion(),
       unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new Port with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Port(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
  Port(const Port& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  Port& operator=(const Port& source);


  /**
   * Creates and returns a deep copy of this Port object.
   * 
   * @return a (deep) copy of this Port object.
   */
  virtual Port* clone () const;


  /**
   * Destructor.
   */
  virtual ~Port ();


  /**
   * Returns the value of the "id" attribute of this Port.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this Port.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Port's "id" attribute has been set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId () const;

  
  /**
   * Sets the value of the "id" attribute of this Port.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Unsets the value of the "id" attribute of this Port.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId ();


  /**
   * Returns the value of the "name" attribute of this Port object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Port's "name" attribute has been set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName () const;

  
  /**
   * Sets the value of the "name" attribute of this Port.
   *
   * @copydetails doc_set_name
   */
  virtual int setName (const std::string& name);


  /**
   * Unsets the value of the "name" attribute of this Port.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName ();


  /**
   * Overrides SBaseRef::setPortRef to always fail, because Port objects
   * themselves cannot refer to model elements by PortSId.
   *
   * @param id the identifier to set for the port reference.
   *
   * @return integer value indicating failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible value
   * returned by this function is:
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setPortRef (const std::string& id);


  /**
   * Returns @c true if the 'id' attribute is set, and if exactly one of
   * the optional attributes of SBaseRef (portRef, idRef, metaIdRef, 
   * and unitRef)are set.
   *
   * @return boolean: 'true' if the attributes are correctly set; 'false' if not.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Returns the XML element name of
   * this SBML object.
   *
   * @return the name of this element, as a text string.
   */
  virtual const std::string& getElementName () const ;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_COMP_PORT, SBMLCompTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Finds and stores the referenced object by finding its Model parent,
   * calling 'getReferencedElementFrom()' on that model, and storing the
   * result.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int saveReferencedElement();


  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * @copydoc doc_renameunitsidref_common
   */
  virtual void renameUnitSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * @copydoc doc_renamemetasidref_common
   */
  virtual void renameMetaIdRefs(const std::string& oldid, const std::string& newid);


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


protected:
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
   *   stream.writeAttribute( "submodel", mSubmodel );
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
 * Creates a new Port_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * Port_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * Port_t.
 * @param pkgVersion an unsigned int, the SBML 'Qual' package Version to assign to this
 * Port_t.
 *
 * @return a pointer to the newly created Port_t structure.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
Port_t *
Port_create(unsigned int level, unsigned int version,
            unsigned int pkgVersion);


/**
 * Frees the given Port_t structure.
 *
 * @param p the Port_t structure to free.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
void
Port_free(Port_t * p);


/**
 * Returns a copy of the given Port_t structure.
 *
 * @param p the Port_t structure to copy.
 * 
 * @return a (deep) copy of the Port_t.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
Port_t *
Port_clone(Port_t * p);


/**
 * Takes an Port_t structure and returns its identifier.
 *
 * @param p the Port_t structure whose identifier is sought.
 * 
 * @return the identifier of the given Port_t, as a pointer to a string.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
char *
Port_getId(Port_t * p);


/**
 * Takes a Port_t structure and returns its name.
 *
 * @param p the Port_t whose name is sought.
 *
 * @return the name of the given Port_t, as a pointer to a string.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
char *
Port_getName(Port_t * p);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * Port_t structure's identifier is set.
 *
 * @param p the Port_t structure to query.
 * 
 * @return @c non-zero (true) if the "id" attribute of the given
 * Port_t structure is set, zero (false) otherwise.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_isSetId(Port_t * p);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * Port_t structure's name is set.
 *
 * @param p the Port_t structure to query.
 * 
 * @return @c non-zero (true) if the "name" attribute of the given
 * Port_t structure is set, zero (false) otherwise.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_isSetName(Port_t * p);


/**
 * Assigns the identifier of an Port_t structure.
 *
 * This makes a copy of the string passed in the param @p sid.
 *
 * @param p the Port_t structure to set.
 * @param sid the string to use as the identifier.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with an id of NULL is equivalent to
 * unsetting the "id" attribute.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_setId(Port_t * p, const char * sid);


/**
 * Sets the name of the given Port_t to a copy of @p name.
 *
 * @param p the Port_t structure to set.
 * @param name the name to assign to the given Port_t's "name" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "name" attribute.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_setName(Port_t * p, const char * name);


/**
 * Unsets the "id" attribute of the given Port_t structure.
 *
 * @param p the Port_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_unsetId(Port_t * p);


/**
 * Unsets the "name" attribute of the given Port_t structure.
 *
 * @param p the Port_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_unsetName(Port_t * p);


/**
  * Predicate returning @c true or @c false depending on whether
  * all the required attributes for the given Port_t structure
  * have been set.
  *
  * @note The required attributes for a Port_t structure are:
  * @li useValuesfromTriggerTime ( L3 onwards )
  *
 * @memberof Port_t
 */
LIBSBML_EXTERN
int
Port_hasRequiredAttributes(Port_t * p);


/**
 * Return the Port_t indicated by the given @p sid.
 *
 * @param lo the ListOf_t structure to use.
 *
 * @param sid a string, the identifier of the
 * Port_t is being sought.
 *
 * @return the Port_t for the given variable, or @c NULL if no such
 * Port_t exits.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
Port_t *
ListOfPorts_getById(ListOf_t * lo, const char * sid);


/**
 * Removes the Port_t structure with the given @p sid
 * from the given ListOf_t structure and returns a pointer to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param lo the ListOf_t structure.
 * @param sid the string of the "id" attribute of the Port_t sought.
 *
 * @return the Port_t structure removed.  As mentioned above, the 
 * caller owns the returned structure. @c NULL is returned if no Port_t
 * structure with the "id" attribute exists in the given ListOf_t structure.
 *
 * @memberof Port_t
 */
LIBSBML_EXTERN
Port_t *
ListOfPorts_removeById(ListOf_t * lo, const char * sid);



END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* Port_H__ */
