/**
 * @file    Deletion.h
 * @brief   Definition of Deletion, the SBase derived class of deletions package.
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
 * @class Deletion
 * @sbmlbrief{comp} %Deletion of an object from a submodel.
 *
 * The Deletion class was introduced by the SBML Level&nbsp;3 @ref comp
 * (&ldquo;comp&rdquo;) package to allow elements of submodels to be removed
 * before instantiation.

 * The Deletion object class is used to define a deletion operation
 * to be applied when a submodel instantiates a model definition.
 * Deletions may be useful in hierarchical model composition scenarios for
 * various reasons.  For example, some components in a submodel may be
 * redundant in the composed model, perhaps because the same features are
 * implemented in a different way in the new model.

 * Deletions function as follows.  When the Model to which the Submodel
 * object refers (via the "modelRef" attribute) is read and processed for
 * inclusion into the composed model, each Deletion object identifies an
 * object to remove from that Model instance.  The resulting submodel
 * instance consists of everything in the Model object instance minus the
 * entities referenced by the list of Deletion objects.
 * 
 * As might be expected, deletions can have wide-ranging implications,
 * especially when the object deleted has substantial substructure, as in
 * the case of reactions.  The following are rules regarding deletions and
 * their effects.
 * 
 * @li An object that has been deleted is considered inaccessible.
 * Any element that has been deleted (or replaced)
 * may not be referenced by an SBaseRef object.
 * @li If the deleted object has child objects and other structures, the
 * child objects and substructure are also considered to be deleted.
 * @li It is not an error to delete explicitly an object that is already
 * deleted by implication (for example as a result of the second point
 * above).  The resulting model is the same.
 * @li If the deleted object is from an SBML namespace that is not
 * understood by the interpreter, the deletion must be ignored---the 
 * object will not need to be deleted, as the interpreter could not
 * understand the package.  If an interpreter cannot tell whether 
 * a referenced object does not exist or if exists in an unparsed namespace
 * it may produce a warning.
 *
 * The Deletion object class is subclassed from SBaseRef, and reuses all the 
 * machinery provided by SBaseRef.  In addition, it defines two optional 
 * attributes, "id" and "name".  The "id" attribute can be used to give an
 * identifier to a given deletion operation.  The identifier has no
 * mathematical meaning, but it may be useful for creating submodels that
 * can be manipulated more directly by other submodels.  (Indeed, it is
 * legitimate for an enclosing model definition to delete a deletion!)
 *
 * The optional "name" attribute is provided on Deletion for the
 * same reason it is provided on other elements that have identifiers;
 * viz., to provide for the possibility of giving a human-readable name to
 * the object.  The name may be useful in situations when deletions are
 * displayed to modelers.
 */


#ifndef Deletion_H__
#define Deletion_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/packages/comp/sbml/SBaseRef.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Deletion : public SBaseRef
{
protected:

  /** @cond doxygenLibsbmlInternal */
//  std::string mId;
//  std::string mName;
  /** @endcond */

public:

  /**
   * Creates a new Deletion with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Deletion(unsigned int level      = CompExtension::getDefaultLevel(),
           unsigned int version    = CompExtension::getDefaultVersion(),
           unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new Deletion with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Deletion(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the Deletion instance to copy.
   */
  Deletion(const Deletion& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  Deletion& operator=(const Deletion& source);


  /**
   * Creates and returns a deep copy of this Deletion object.
   * 
   * @return a (deep) copy of this Deletion object.
   */
  virtual Deletion* clone () const;


  /**
   * Destructor.
   */
  virtual ~Deletion ();


  /**
   * Sets the value of the "id" attribute of this Deletion.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Returns the value of the "id" attribute of this Deletion.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this Deletion.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * object's "id" attribute has been set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId() const;


  /**
   * Unsets the value of the "id" attribute of this Deletion.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId();


  /**
   * Sets the value of the "name" attribute of this Deletion.
   *
   * @copydetails doc_set_name
   */
  virtual int setName (const std::string& name);


  /**
   * Returns the value of the "name" attribute of this Deletion object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * object's "name" attribute has been set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName() const;


  /**
   * Unsets the value of the "name" attribute of this Deletion.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName();


  /**
   * Returns the XML element name of this SBML object.
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
   * @sbmlconstant{SBML_COMP_DELETION, SBMLCompTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Finds and stores the referenced object.  It finds its Submodel parent,
   * gets its instantiated Model object, calls
   * 'getReferencedElementFrom()' using that model, and stores the result.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int saveReferencedElement();
  
  
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
   *   stream.writeAttribute( "id"  , mId   );
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
 * Creates a new Deletion_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * Deletion_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * Deletion_t.
 * @param pkgVersion an unsigned int, the SBML 'Qual' package Version to assign to this
 * Deletion_t.
 *
 * @return a pointer to the newly created Deletion_t structure.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
Deletion_t *
Deletion_create(unsigned int level, unsigned int version,
                unsigned int pkgVersion);


/**
 * Frees the given Deletion_t structure.
 *
 * @param d the Deletion_t structure to free.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
void
Deletion_free(Deletion_t * d);


/**
 * Returns a copy of the given Deletion_t structure.
 *
 * @param d the Deletion_t structure to copy.
 * 
 * @return a (deep) copy of the Deletion_t.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
Deletion_t *
Deletion_clone(Deletion_t * d);


/**
 * Takes an Deletion_t structure and returns its identifier.
 *
 * @param d the Deletion_t structure whose identifier is sought.
 * 
 * @return the identifier of the given Deletion_t, as a pointer to a string.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
char *
Deletion_getId(Deletion_t * d);


/**
 * Takes a Deletion_t structure and returns its name.
 *
 * @param d the Deletion_t whose name is sought.
 *
 * @return the name of the given Deletion_t, as a pointer to a string.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
char *
Deletion_getName(Deletion_t * d);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * Deletion_t structure's identifier is set.
 *
 * @param d the Deletion_t structure to query.
 * 
 * @return @c non-zero (true) if the "id" attribute of the given
 * Deletion_t structure is set, zero (false) otherwise.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_isSetId(Deletion_t * d);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * Deletion_t structure's name is set.
 *
 * @param d the Deletion_t structure to query.
 * 
 * @return @c non-zero (true) if the "name" attribute of the given
 * Deletion_t structure is set, zero (false) otherwise.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_isSetName(Deletion_t * d);


/**
 * Assigns the identifier of an Deletion_t structure.
 *
 * This makes a copy of the string passed in the param @p sid.
 *
 * @param d the Deletion_t structure to set.
 * @param sid the string to use as the identifier.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with an id of NULL is equivalent to
 * unsetting the "id" attribute.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_setId(Deletion_t * d, const char * sid);


/**
 * Sets the name of the given Deletion_t to a copy of @p name.
 *
 * @param d the Deletion_t structure to set.
 * @param name the name to assign to the given Deletion_t's "name" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "name" attribute.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_setName(Deletion_t * d, const char * name);


/**
 * Unsets the "id" attribute of the given Deletion_t structure.
 *
 * @param d the Deletion_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_unsetId(Deletion_t * d);


/**
 * Unsets the "name" attribute of the given Deletion_t structure.
 *
 * @param d the Deletion_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_unsetName(Deletion_t * d);


/**
  * Predicate returning @c true or @c false depending on whether
  * all the required attributes for the given Deletion_t structure
  * have been set.
  *
  * @note The required attributes for a Deletion_t structure are:
  * @li useValuesfromTriggerTime ( L3 onwards )
  *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
int
Deletion_hasRequiredAttributes(Deletion_t * d);


/**
 * Return the Deletion_t indicated by the given @p sid.
 *
 * @param lo the ListOf_t structure to use.
 *
 * @param sid a string, the identifier of the
 * Deletion_t is being sought.
 *
 * @return the Deletion_t for the given variable, or @c NULL if no such
 * Deletion_t exits.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
Deletion_t *
ListOfDeletions_getById(ListOf_t * lo, const char * sid);


/**
 * Removes the Deletion_t structure with the given @p sid
 * from the given ListOf_t structure and returns a pointer to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param lo the ListOf_t structure.
 * @param sid the string of the "id" attribute of the Deletion_t sought.
 *
 * @return the Deletion_t structure removed.  As mentioned above, the 
 * caller owns the returned structure. @c NULL is returned if no Deletion_t
 * structure with the "id" attribute exists in the given ListOf_t structure.
 *
 * @memberof Deletion_t
 */
LIBSBML_EXTERN
Deletion_t *
ListOfDeletions_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* Deletion_H__ */
