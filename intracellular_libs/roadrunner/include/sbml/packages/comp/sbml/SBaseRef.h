/**
 * @file    SBaseRef.h
 * @brief   Definition of SBaseRef, the SBase derived class of the comp package.
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
 * @class SBaseRef
 * @sbmlbrief{comp} Base class for references to objects.
 *
 * The SBaseRef class was introduced by the SBML Level&nbsp;3 @ref comp
 * (&ldquo;comp&rdquo;) package as the principle way by which submodel
 * elements may be referenced.  The SBaseRef class is usually found as the
 * base class of a Port, Deletion, ReplacedElement, or ReplacedBy class, but
 * may appear as an child of one of the above classes if the parent object
 * references a Submodel.
 *
 * An SBaseRef object must reference an element using exactly one of the
 * optional attributes of the class.  Subclasses of SBaseRef may define
 * additional optional attributes that are legal ways to reference an element.
 *
 * SBaseRef objects may reference elements that do not live in the Model parent 
 * of the SBaseRef object.  However, the SBaseRef class itself does not 
 * provide a method of determining which Model or Submodel is being referenced.
 * The subclasses of SBaseRef provide methods for this instead.
 *
 * Once the Model to which the SBaseRef object is referencing has been established,
 * there are four optional attributes defined in the SBaseRef class that
 * are each methods of referencing an element:
 *
 * @li "portRef" (type <code>PortSIdRef</code>):  As its name implies, this attribute is used to
 * refer to a port identifier, in the case when the reference being
 * constructed with the SBaseRef is intended to refer to a port on a
 * submodel.  The namespace of the <code>PortSIdRef</code> value is the set
 * of identifiers of type <code>PortSId</code> defined in the submodel, not
 * the parent model.
 * @li "idRef" (type <code>SIdRef</code>): As its name implies, this attribute is used to
 * refer to a regular identifier (i.e., the value of an "id"
 * attribute on some other object), in the case when the reference being
 * constructed with the SBaseRef is intended to refer to an object that
 * does not have a port identifier.  The namespace of the <code>SIdRef</code>
 * value is the set of identifiers of type SId defined in the
 * submodel, not the parent model.
 * @li "unitRef" (type <code>UnitSIdRef</code>): This attribute is used to refer to the identifier
 * of a UnitDefinition object.  The namespace of the <code>UnitSIdRef</code>
 * value is the set of unit identifiers defined in the submodel, not the
 * parent model. (Note that even though this attribute is of type <code>UnitSIdRef</code>,
 * the reserved unit identifiers that are defined by SBML Level&nbsp;3 (see
 * Section 3.1.10 of the core specification) are
 * *not* permitted as values of "unitRef".  Reserved unit
 * identifiers may not be replaced or deleted.)
 * @li "metaIdRef" (type <code>IDREF</code>): This attribute is used to refer to a "metaid"
 * attribute value on some other object, in the case when the reference
 * being constructed with the SBaseRef is intended to refer to an object
 * that does not have a port identifier.  The namespace of the "metaIdRef"
 * value is the entire document in which the referenced model resides, but
 * must refer to a subelement of the referenced model.  Since meta identifiers are
 * optional attributes of SBase, all SBML objects have the potential to
 * have a meta identifier value.
 *
 * An SBaseRef object may have up to one subcomponent named "sBaseRef", of
 * type SBaseRef.  This permits recursive structures to be constructed so
 * that objects inside submodels can be referenced.
 *
 * The form of such recursive references must be as follows.  The
 * highest-level SBaseRef object of such a chain (which will necessarily
 * be an object of class Port, Deletion, ReplacedElement or ReplacedBy,
 * because they are the only classes derived from the class SBaseRef) must
 * refer to a Submodel object in the containing model.  All child
 * SBaseRef objects in the chain must refer to components inside the
 * Model instance to which the Submodel refers.
 */


#ifndef SBaseRef_H__
#define SBaseRef_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>
#include <set>

#include <sbml/SBase.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/CompBase.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN SBaseRef : public CompBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string   mMetaIdRef;
  std::string   mPortRef;
  std::string   mIdRef;
  std::string   mUnitRef;
  SBaseRef*     mSBaseRef;
  SBase*        mReferencedElement;
  SBase*        mDirectReference;
  /** @endcond */

public:

  /**
   * Creates a new SBaseRef with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  SBaseRef(unsigned int level      = CompExtension::getDefaultLevel(),
           unsigned int version    = CompExtension::getDefaultVersion(),
           unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new SBaseRef with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  SBaseRef(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
  SBaseRef(const SBaseRef& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  SBaseRef& operator=(const SBaseRef& source);


  /**
   * Creates and returns a deep copy of this SBaseRef object.
   * 
   * @return a (deep) copy of this SBaseRef object.
   */
  virtual SBaseRef* clone () const;


  /**
   * Destructor.
   */
  virtual ~SBaseRef ();


  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the id of the object to find.
   *
   * @return a pointer to the SBase element with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
  
  /**
   * Returns the first child element it can find with the given @p metaid, or
   * itself if it has the given @p metaid, or @c NULL if no such object is found.
   *
   * @param metaid string representing the metaid of the object to find.
   *
   * @return a pointer to the SBase element with the given @p metaid.
   */
  virtual SBase* getElementByMetaId(const std::string& metaid);
  
  
  /**
   * Returns a List of all child SBase* objects, including those nested to an
   * arbitrary depth.
   *
   * @param filter a pointer to an ElementFilter, which causes the function 
   * to return only elements that match a particular set of constraints.  
   * If NULL (the default), the function will return all child objects.
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);
  
  
  /**
   * Returns the value of the "metaIdRef" attribute of this SBaseRef.
   *
   * @return the value of the "metaIdRef" attribute of this SBaseRef.
   */
  virtual const std::string& getMetaIdRef () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "metaIdRef" attribute has been set.
   *
   * @return @c true if this SBaseRef's "metaIdRef" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetMetaIdRef () const;

  
  /**
   * Sets the value of the "metaIdRef" attribute of this SBaseRef.
   *
   * This method fails if the id is not a valid syntax for an <code>IDREF</code> (@sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}), or if the SBaseRef already
   * points to an element of the submodel using a different interface (@sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}).  An sBaseRef must use exactly one
   * method to point to a submodel element.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setMetaIdRef (const std::string& id);


  /**
   * Unsets the value of the "metaIdRef" attribute of this SBaseRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetMetaIdRef ();


  /**
   * Returns the value of the "portRef" attribute of this SBaseRef.
   *
   * @return the value of the "portRef" attribute of this SBaseRef.
   */
  virtual const std::string& getPortRef () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "portRef" attribute has been set.
   *
   * @return @c true if this SBaseRef's "portRef" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetPortRef () const;

  
  /**
   * Sets the value of the "portRef" attribute of this SBaseRef.  Fails if
   * the id is not a valid syntax for a <code>PortSIdRef</code> (@sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}), or if the SBaseRef already
   * points to an element of the submodel using a different interface (@sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}).  An SBaseRef must use exactly one
   * method to point to a submodel element.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setPortRef (const std::string& id);


  /**
   * Unsets the value of the "portRef" attribute of this SBaseRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetPortRef ();


  /**
   * Returns the value of the "idRef" attribute of this SBaseRef.
   *
   * @return the value of the "idRef" attribute of this SBaseRef.
   */
  virtual const std::string& getIdRef () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "idRef" attribute has been set.
   *
   * @return @c true if this SBaseRef's "idRef" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetIdRef () const;

  
  /**
   * Sets the value of the "idRef" attribute of this SBaseRef.
   *
   * This method fails if the id is not a valid syntax for an <code>SIdRef</code> (@sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}), or if the SBaseRef already
   * points to an element of the submodel using a different interface (@sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}).  A sBaseRef must use exactly one
   * method to point to a submodel element.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setIdRef (const std::string& id);


  /**
   * Unsets the value of the "idRef" attribute of this SBaseRef.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetIdRef ();


  /**
   * Returns the value of the "unitRef" attribute of this SBaseRef.
   *
   * @return the value of the "unitRef" attribute of this SBaseRef.
   */
  virtual const std::string& getUnitRef () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "unitRef" attribute has been set.
   *
   * @return @c true if this SBaseRef's "unitRef" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetUnitRef () const;

  
  /**
   * Sets the value of the "unitRef" attribute of this SBaseRef.
   *
   * This method fails if the id is not a valid syntax for a <code>UnitSIdRef</code> (@sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}), or if the SBaseRef already
   * points to an element of the submodel using a different interface (@sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}).  A sBaseRef must use exactly one
   * method to point to a submodel element.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setUnitRef (const std::string& id);


  /**
   * Unsets the value of the "unitRef" attribute of this SBaseRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetUnitRef ();


  /**
   * Get the child sBaseRef of this sBaseRef.
   * 
   * @return the const SBaseRef child of this SBaseRef, or @c NULL if none exists.
   */
  const SBaseRef* getSBaseRef () const;


  /**
   * Get the child sBaseRef of this SBaseRef.
   * 
   * @return the SBaseRef child of this SBaseRef, or @c NULL if none exists.
   */
  SBaseRef* getSBaseRef ();


  /**
   * Predicate for testing whether the sBaseRef for this SBaseRef is set.
   *
   * @return @c true if the sBaseRef of this SBaseRef is set, @c false
   * otherwise.
   */
  bool isSetSBaseRef () const;


  /**
   * Sets the sBaseRef definition of this SBaseRef to a copy of the given
   * SBaseRef object instance.
   *
   * This method fails if the added sBaseRef does not match the
   * level/version/package of the parent object or if the added sBaseRef cannot
   * be copied.
   *
   * @param sBaseRef the SBaseRef object instance to use.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int setSBaseRef (const SBaseRef* sBaseRef);


  /**
   * Creates a new, empty SBaseRef, adds it to this SBaseRef and 
   * returns the created SBaseRef.
   *
   * @return the newly created SBaseRef object instance.
   */
  SBaseRef* createSBaseRef ();


  /**
   * Unsets the child SBaseRef of this SBaseRef.  Deletes the former SBaseRef child,
   * if one existed.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetSBaseRef();


  /**
   * Returns how many elements are being referred to by this SBaseRef.  A
   * valid SBaseRef will have exactly one.  Possible referents are portRef,
   * idRef, unitRef, and metaIdRef.
   *
   * @return integer value between 0 and 4: the number of different ways this element points to its referent.
   */
  virtual int getNumReferents() const;


  /**
   * Returns @c true if getNumReferents() is exactly 1.
   *
   * @return boolean: 'true' if the attributes are correctly set; 'false' if not.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


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
   * @sbmlconstant{SBML_COMP_SBASEREF, SBMLCompTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


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


  /**
   * Examines the referenced Model for the referenced object, and returns it, if found.
   *
   * @param model the Model in which to look for the object referenced by
   * this SBaseRef.
   *
   * @return the element in the referenced Model to which this SBaseRef
   * refers.  If this object references an object in a Submodel, the returned
   * object will be in the instantiated Model in that Submodel.
   *
   */
  virtual SBase* getReferencedElementFrom(Model* model);


  /**
   * Finds and stores the referenced object by finding the Model it needs to
   * point to, calling 'saveReferencedElement' on its parent (which will also
   * be a SBaseRef or one of its subclasses), and the storing the result.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int saveReferencedElement();


  /**
   * Returns the object pointed to by this element.  If that element was
   * previously found and set with 'saveReferencedElement', that element is
   * returned; otherwise, 'saveReferencedElement' is called first, and the
   * found element is returned.
   */
  virtual SBase* getReferencedElement();


  /**
   * Removes the saved referenced element, if it had been saved earlier.
   */
  virtual void clearReferencedElement();


  /**
   * DEPRECATED FUNCTION:  DO NOT USE
   *
   * Deletes the referenced object,
   * plus any other elements that element points to through ReplacedElement
   * or ReplacedBy children.  Instead of calling this function directly, use
   * 'CompModelPlugin::instantiateSubmodels' instead, which deals with all the
   * intricacies of replacements and deletions, and gives you access to the
   * non-flattened hierarchical form of the model.
   */
  virtual int performDeletion();

  /**
   * Finds this SBaseRef's parent, which can either be a List or can be
   * another SBaseRef, and tells it to remove this.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int removeFromParentAndDelete();


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Need a version of the SBaseRef constructor that does not load plugins.
   *
   * @param compns the namespace to use.
   */
  SBaseRef(CompPkgNamespaces* compns, bool isDerived);
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
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "metaIdRef", mMetaIdRef );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Get the *direct* referenced object, which might be a Port.
   */
  virtual SBase* getDirectReference();
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  friend class CompModelPlugin; //for getDirectReference
  /** @endcond */

private:
  /**
   * Collects (in 'toremove') the referenced object,
   * plus any other elements that element points to through ReplacedElement
   * or ReplacedBy children.  Does not delete the object directly; this should
   * be done carefully to avoid double-deletions or misinterpretation of 
   * nested replacements/deletions.  Any element already in 'removed' will
   * not be added to 'toremove', nor will its children be checked.
   */
  virtual int collectDeletions(std::set<SBase*>* removed, std::set<SBase*>* toremove);

};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new SBaseRef_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * SBaseRef_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * SBaseRef_t.
 * @param pkgVersion an unsigned int, the SBML 'Qual' package Version to assign to this
 * SBaseRef_t.
 *
 * @return a pointer to the newly created SBaseRef_t structure.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
SBaseRef_t *
SBaseRef_create(unsigned int level, unsigned int version,
                unsigned int pkgVersion);


/**
 * Frees the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to free.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
void
SBaseRef_free(SBaseRef_t * sbr);


/**
 * Returns a copy of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to copy.
 * 
 * @return a (deep) copy of the SBaseRef_t.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
SBaseRef_t *
SBaseRef_clone(SBaseRef_t * sbr);


/**
 * Takes a SBaseRef_t structure and returns its portRef.
 *
 * @param sbr the SBaseRef_t whose portRef is sought.
 *
 * @return the portRef of the given SBaseRef_t, as a pointer to a string.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
char *
SBaseRef_getPortRef(SBaseRef_t * sbr);


/**
 * Takes a SBaseRef_t structure and returns its idRef.
 *
 * @param sbr the SBaseRef_t whose idRef is sought.
 *
 * @return the idRef of the given SBaseRef_t, as a pointer to a string.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
char *
SBaseRef_getIdRef(SBaseRef_t * sbr);


/**
 * Takes a SBaseRef_t structure and returns its unitRef.
 *
 * @param sbr the SBaseRef_t whose unitRef is sought.
 *
 * @return the unitRef of the given SBaseRef_t, as a pointer to a string.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
char *
SBaseRef_getUnitRef(SBaseRef_t * sbr);


/**
 * Takes a SBaseRef_t structure and returns its metaIdRef.
 *
 * @param sbr the SBaseRef_t whose metaIdRef is sought.
 *
 * @return the metaIdRef of the given SBaseRef_t, as a pointer to a string.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
char *
SBaseRef_getMetaIdRef(SBaseRef_t * sbr);


/**
 * Takes a SBaseRef_t structure and returns its child SBaseRef_t, or @c NULL if 
 * no such child exists.
 *
 * @param sbr the SBaseRef_t whose child SBaseRef_t is sought.
 *
 * @return the child SBaseRef_t of the given SBaseRef_t, or @c NULL if 
 * no such child exists.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
SBaseRef_t*
SBaseRef_getSBaseRef(SBaseRef_t * sbr);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * SBaseRef_t structure's portRef is set.
 *
 * @param sbr the SBaseRef_t structure to query.
 * 
 * @return @c non-zero (true) if the "portRef" attribute of the given
 * SBaseRef_t structure is set, zero (false) otherwise.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_isSetPortRef(SBaseRef_t * sbr);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * SBaseRef_t structure's idRef is set.
 *
 * @param sbr the SBaseRef_t structure to query.
 * 
 * @return @c non-zero (true) if the "idRef" attribute of the given
 * SBaseRef_t structure is set, zero (false) otherwise.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_isSetIdRef(SBaseRef_t * sbr);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * SBaseRef_t structure's unitRef is set.
 *
 * @param sbr the SBaseRef_t structure to query.
 * 
 * @return @c non-zero (true) if the "unitRef" attribute of the given
 * SBaseRef_t structure is set, zero (false) otherwise.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_isSetUnitRef(SBaseRef_t * sbr);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * SBaseRef_t structure's metaIdRef is set.
 *
 * @param sbr the SBaseRef_t structure to query.
 * 
 * @return @c non-zero (true) if the "metaIdRef" attribute of the given
 * SBaseRef_t structure is set, zero (false) otherwise.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_isSetMetaIdRef(SBaseRef_t * sbr);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * SBaseRef_t structure's child SBaseRef_t is set.
 *
 * @param sbr the SBaseRef_t structure to query.
 * 
 * @return @c non-zero (true) if the SBaseRef_t child of the given
 * SBaseRef_t structure is set, zero (false) otherwise.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_isSetSBaseRef(SBaseRef_t * sbr);


/**
 * Sets the portRef of the given SBaseRef_t to a copy of @p portRef.
 *
 * @param sbr the SBaseRef_t structure to set.
 * @param portRef the portRef to assign to the given SBaseRef_t's "portRef" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "portRef" attribute.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_setPortRef(SBaseRef_t * sbr, const char * portRef);


/**
 * Sets the idRef of the given SBaseRef_t to a copy of @p idRef.
 *
 * @param sbr the SBaseRef_t structure to set.
 * @param idRef the idRef to assign to the given SBaseRef_t's "idRef" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "idRef" attribute.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_setIdRef(SBaseRef_t * sbr, const char * idRef);


/**
 * Sets the unitRef of the given SBaseRef_t to a copy of @p unitRef.
 *
 * @param sbr the SBaseRef_t structure to set.
 * @param unitRef the unitRef to assign to the given SBaseRef_t's "unitRef" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "unitRef" attribute.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_setUnitRef(SBaseRef_t * sbr, const char * unitRef);


/**
 * Sets the metaIdRef of the given SBaseRef_t to a copy of @p metaIdRef.
 *
 * @param sbr the SBaseRef_t structure to set.
 * @param metaIdRef the metaIdRef to assign to the given SBaseRef_t's "metaIdRef" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "metaIdRef" attribute.
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_setMetaIdRef(SBaseRef_t * sbr, const char * metaIdRef);


/**
 * Sets the sBaseRef of this SBaseRef_t to a copy of the given SBaseRef_t.
 *
 * @param sbr the SBaseRef_t structure to set.
 * @param sBaseRef the SBaseRef_t structure to use.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_setSBaseRef(SBaseRef_t * sbr, SBaseRef_t * sBaseRef);


/**
 * Unsets the "portRef" attribute of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_unsetPortRef(SBaseRef_t * sbr);


/**
 * Unsets the "idRef" attribute of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_unsetIdRef(SBaseRef_t * sbr);


/**
 * Unsets the "unitRef" attribute of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_unsetUnitRef(SBaseRef_t * sbr);


/**
 * Unsets the "metaIdRef" attribute of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_unsetMetaIdRef(SBaseRef_t * sbr);


/**
 * Removes the SBaseRef_t child of the given SBaseRef_t structure.
 *
 * @param sbr the SBaseRef_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_unsetSBaseRef(SBaseRef_t * sbr);


/**
  * Predicate returning @c true or @c false depending on whether
  * all the required attributes for the given SBaseRef_t structure
  * have been set.
  *
  * @note The required attributes for a SBaseRef_t structure are:
  * @li useValuesfromTriggerTime ( L3 onwards )
  *
 * @memberof SBaseRef_t
 */
LIBSBML_EXTERN
int
SBaseRef_hasRequiredAttributes(SBaseRef_t * sbr);



END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* SBaseRef_H__ */
