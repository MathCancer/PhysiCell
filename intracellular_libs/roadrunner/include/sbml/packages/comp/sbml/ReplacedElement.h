/**
 * @file    ReplacedElement.h
 * @brief   Definition of ReplacedElement, the SBase derived class of replacedElements package.
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
 * ------------------------------------------------------------------------ -->
 *
 * @class ReplacedElement
 * @sbmlbrief{comp} Indicates an object replaces another.
 *
 * The ReplacedElement class was introduced by the SBML Level&nbsp;3 @ref
 * comp (&ldquo;comp&rdquo;) package to allow submodel elements to be
 * replaced, but still allow references to those elements to be valid.  A
 * ReplacedElement object is essentially a pointer to a submodel object that
 * should be considered 'replaced'.  The object holding the ReplacedElement
 * instance is the one doing the replacing; the object pointed to by the
 * ReplacedElement object is the object being replaced.
 *
 * A replacement implies that dependencies involving the replaced object
 * must be updated: all references to the replaced object elsewhere in the
 * model are taken to refer to the replacement object instead.  For
 * example, if one species replaces another, then any reference to the
 * original species in mathematical formulas, or lists of reactants or
 * products or modifiers in reactions, or initial assignments, or any other
 * SBML construct, are taken to refer to the replacement species, with its
 * value possibly modified by either this object's "conversionFactor"
 * attribute or the relevant submodel's conversion factors. Moreover, any 
 * annotations that refer to the
 * replaced species' "metaid" value must be made to refer to the
 * replacement species' "metaid" value instead; and anything else
 * that referred either to an object identifier (i.e., attributes such as
 * the "id" attribute whose types inherit from the SId
 * primitive data type) or the meta identifier (i.e., the "metaid"
 * attribute or any other attribute that inherits from the ID primitive
 * data type) must be made to refer to the replacement species object
 * instead.
 *
 * It is worth noting that local parameters (inside Reaction objects) pose an
 * interesting edge case for these rules. In order to determine which element
 * is pointed to by a <code>&lt;cn&gt;</code> element within the
 * <code>&lt;math&gt;</code> element of a KineticLaw object, it is necessary
 * to examine the local parameters of that kinetic law's parent Reaction
 * object.  Whether the <code>&lt;cn&gt;</code> element is considered to
 * point to something new, then, depends on whether it pointed to the local
 * parameter and whether that local parameter was replaced, even if the text
 * of the element matched the SId value of another element in the model.
 * Note that local parameters may only effectively be replaced by global
 * parameters, since references to its SId are only valid from within the
 * Reaction element to which it belongs.
 *
 * When referencing an element within the Submodel pointed to by the 
 * "submodelRef" attribute (defined in libSBML in the Replacing class), 
 * any of the four attributes inherited from 
 * SBaseRef for the purpose may be used (portRef, idRef, unitRef, or 
 * metaIdRef), or a new optional attribute "deletion" may be used.  This
 * attribute must be the identifier of a Deletion
 * object in the parent Model of the ReplacedElement (i.e., the value of
 * some Deletion object's "id" attribute).  When "deletion" is
 * set, it means the ReplacedElement object is actually an annotation to
 * indicate that the replacement object replaces something deleted
 * from a submodel.  The use of the "deletion" attribute overrides
 * the use of the attributes inherited from SBaseRef: instead of using,
 * e.g., "portRef" or "idRef", the ReplacedElement instance
 * sets "deletion" to the identifier of the Deletion object.  In
 * addition, the referenced Deletion must be a child of the Submodel
 * referenced by the "submodelRef" attribute.
 *
 * The use of ReplacedElement objects to refer to deletions has no effect
 * on the composition of models or the mathematical properties of the
 * result.  It serves instead to help record the decision-making process
 * that lead to a given model.  It can be particularly useful for
 * visualization purposes, as well as to serve as scaffolding where other
 * types of annotations can be added using the normal Annotation
 * subcomponents available on all SBase objects in SBML.
 *
 * As with the Submodel class, it may be that the units of the replaced
 * element may not match the units of the replacement element.  In this case, 
 * the optional "conversionFactor" attribute may be used.  This attribute, if
 * present, defines how to transform or rescale the replaced object's value
 * so that it is appropriate for the new contexts in which the object
 * appears.  This attribute takes a value of type SIdRef, and
 * the value must refer to a Parameter object instance defined in the
 * model.  This parameter then acts as a conversion factor.
 * 
 * The value of the conversion factor should be defined such that a single
 * unit of the replaced element multiplied by the conversion factor should
 * equal a single unit of the replacement element, and the units of the
 * conversion factor should be commensurate with that transformation.  The
 * referenced Parameter may be non-constant, particularly if a Species is
 * replaced by a Species with a different "hasOnlySubstanceUnits"
 * attribute value, thus changing amount to concentration, or visa versa.
 */


#ifndef ReplacedElement_H__
#define ReplacedElement_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/Replacing.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ReplacedElement : public Replacing
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string   mDeletion;
  /** @endcond */

public:

  /**
   * Creates a new ReplacedElement with the given level, version, and package
   * version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ReplacedElement(unsigned int level      = CompExtension::getDefaultLevel(),
                  unsigned int version    = CompExtension::getDefaultVersion(),
                  unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new ReplacedElement with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ReplacedElement(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
  ReplacedElement(const ReplacedElement& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  ReplacedElement& operator=(const ReplacedElement& source);


  /**
   * Creates and returns a deep copy of this ReplacedElement object.
   * 
   * @return a (deep) copy of this ReplacedElement object.
   */
  virtual ReplacedElement* clone () const;


  /**
   * Destructor.
   */
  virtual ~ReplacedElement ();


  /**
   * Returns the value of the "conversionFactor" attribute of this ReplacedElement.
   *
   * @return the value of the "conversionFactor" attribute of this ReplacedElement.
   */
  virtual const std::string& getConversionFactor () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ReplacedElement's "conversionFactor" attribute has been set.
   *
   * @return @c true if this ReplacedElement's "conversionFactor" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetConversionFactor () const;

  
  /**
   * Sets the value of the "conversionFactor" attribute of this ReplacedElement.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setConversionFactor (const std::string& id);


  /**
   * Unsets the value of the "conversionFactor" attribute of this ReplacedElement.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetConversionFactor ();


  /**
   * Returns the value of the "deletion" attribute of this ReplacedElement.
   *
   * @return the value of the "deletion" attribute of this ReplacedElement.
   */
  virtual const std::string& getDeletion () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "deletion" attribute has been set.
   *
   * @return @c true if this ReplacedElement's "deletion" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetDeletion () const;

  
  /**
   * Sets the value of the "deletion" attribute of this ReplacedElement.
   *
   * This method fails if the id is not a valid syntax for an SIdRef (@sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}), or if the SBaseRef already
   * points to an element of the submodel using a different interface (@sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}).  A ReplacedElement must use exactly
   * one method to point to a submodel element: deletion, port, idRef,
   * unitRef, or metaIdRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setDeletion (const std::string& id);


  /**
   * Unsets the value of the "deletion" attribute of this ReplacedElement.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetDeletion ();


  /**
   * Returns the XML element name of
   * this SBML object.
   *
   * @return the name of this element, as a text string.
   */
  virtual const std::string& getElementName () const ;


  /**
   * Returns how many elements are being referred to by this ReplacedElement.  A
   * valid ReplacedElement will have exactly one.  Possible referents are deletion,
   * port, idRef, unitRef, and metaIdRef.
   *
   * @return integer value between 0 and 5: the number of different ways this
   * element points to its referent.
   */
  virtual int getNumReferents() const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_COMP_REPLACEDELEMENT, SBMLCompTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Finds the SBase object this ReplacedElement object points to, if any.
   *
   */
  virtual SBase* getReferencedElementFrom(Model* model);


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
   *   stream.writeAttribute( "conversionFactor", mConversionFactor );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */

  /**
   * Updates all IDs and references to those IDs, as well as performing all
   * necessary conversions based on the conversion factors.  Does not actually
   * remove the now-redundant element!  The elements to be removed is instead 
   * added to 'toremove', allowing one to remove the element carefully
   * to prevent double-deletion of elements, and to allow the correct
   * interpretation of 'nested' replacements and deletions.
   *
   * The 'removed' argument is present to ensure that the replaced element was
   * not already removed, which would make it impossible to check it for its
   * old IDs.  In normal comp flattening, 'removed' will only contain comp elements,
   * which should usually not be replaced, only deleted.
   */
  virtual int performReplacementAndCollect(std::set<SBase*>* removed, std::set<SBase*>* toremove);
  friend class CompModelPlugin;
};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new ReplacedElement_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * ReplacedElement_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * ReplacedElement_t.
 * @param pkgVersion an unsigned int, the SBML 'Qual' package Version to assign to this
 * ReplacedElement_t.
 *
 * @return a pointer to the newly created ReplacedElement_t structure.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
ReplacedElement_t *
ReplacedElement_create(unsigned int level, unsigned int version,
                       unsigned int pkgVersion);


/**
 * Frees the given ReplacedElement_t structure.
 *
 * @param re the ReplacedElement_t structure to free.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
void
ReplacedElement_free(ReplacedElement_t * re);


/**
 * Returns a copy of the given ReplacedElement_t structure.
 *
 * @param re the ReplacedElement_t structure to copy.
 * 
 * @return a (deep) copy of the ReplacedElement_t.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
ReplacedElement_t *
ReplacedElement_clone(ReplacedElement_t * re);


/**
 * Takes a ReplacedElement_t structure and returns its submodelRef.
 *
 * @param re the ReplacedElement_t whose submodelRef is sought.
 *
 * @return the submodelRef of the given ReplacedElement_t, as a pointer to a string.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
char *
ReplacedElement_getSubmodelRef(ReplacedElement_t * re);


/**
 * Takes a ReplacedElement_t structure and returns its deletion.
 *
 * @param re the ReplacedElement_t whose deletion is sought.
 *
 * @return the deletion of the given ReplacedElement_t, as a pointer to a string.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
char *
ReplacedElement_getDeletion(ReplacedElement_t * re);


/**
 * Takes a ReplacedElement_t structure and returns its conversionFactor.
 *
 * @param re the ReplacedElement_t whose conversionFactor is sought.
 *
 * @return the conversionFactor of the given ReplacedElement_t, as a pointer to a string.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
char *
ReplacedElement_getConversionFactor(ReplacedElement_t * re);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * ReplacedElement_t structure's submodelRef is set.
 *
 * @param re the ReplacedElement_t structure to query.
 * 
 * @return @c non-zero (true) if the "submodelRef" attribute of the given
 * ReplacedElement_t structure is set, zero (false) otherwise.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_isSetSubmodelRef(ReplacedElement_t * re);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * ReplacedElement_t structure's deletion is set.
 *
 * @param re the ReplacedElement_t structure to query.
 * 
 * @return @c non-zero (true) if the "deletion" attribute of the given
 * ReplacedElement_t structure is set, zero (false) otherwise.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_isSetDeletion(ReplacedElement_t * re);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * ReplacedElement_t structure's conversionFactor is set.
 *
 * @param re the ReplacedElement_t structure to query.
 * 
 * @return @c non-zero (true) if the "conversionFactor" attribute of the given
 * ReplacedElement_t structure is set, zero (false) otherwise.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_isSetConversionFactor(ReplacedElement_t * re);


/**
 * Sets the submodelRef of the given ReplacedElement_t to a copy of @p submodelRef.
 *
 * @param re the ReplacedElement_t structure to set.
 * @param submodelRef the submodelRef to assign to the given ReplacedElement_t's "submodelRef" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "submodelRef" attribute.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_setSubmodelRef(ReplacedElement_t * re, const char * submodelRef);


/**
 * Sets the deletion of the given ReplacedElement_t to a copy of @p deletion.
 *
 * @param re the ReplacedElement_t structure to set.
 * @param deletion the deletion to assign to the given ReplacedElement_t's "deletion" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "deletion" attribute.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_setDeletion(ReplacedElement_t * re, const char * deletion);


/**
 * Sets the conversionFactor of the given ReplacedElement_t to a copy of @p conversionFactor.
 *
 * @param re the ReplacedElement_t structure to set.
 * @param conversionFactor the conversionFactor to assign to the given ReplacedElement_t's "conversionFactor" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "conversionFactor" attribute.
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_setConversionFactor(ReplacedElement_t * re, const char * conversionFactor);


/**
 * Unsets the "submodelRef" attribute of the given ReplacedElement_t structure.
 *
 * @param re the ReplacedElement_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_unsetSubmodelRef(ReplacedElement_t * re);


/**
 * Unsets the "deletion" attribute of the given ReplacedElement_t structure.
 *
 * @param re the ReplacedElement_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_unsetDeletion(ReplacedElement_t * re);


/**
 * Unsets the "conversionFactor" attribute of the given ReplacedElement_t structure.
 *
 * @param re the ReplacedElement_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_unsetConversionFactor(ReplacedElement_t * re);


/**
  * Predicate returning @c true or @c false depending on whether
  * all the required attributes for the given ReplacedElement_t structure
  * have been set.
  *
  * @note The required attributes for a ReplacedElement_t structure are:
  * @li useValuesfromTriggerTime ( L3 onwards )
  *
 * @memberof ReplacedElement_t
 */
LIBSBML_EXTERN
int
ReplacedElement_hasRequiredAttributes(ReplacedElement_t * re);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* ReplacedElement_H__ */
