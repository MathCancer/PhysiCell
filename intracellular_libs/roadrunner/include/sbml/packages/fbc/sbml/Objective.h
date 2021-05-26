/**
 * @file    Objective.h
 * @brief   Definition of Objective, the SBase derived class of the fbc package.
 * @author  Frank T. Bergmann
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
 * Copyright (C) 2009-2013 jointly by the following organizations:
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
 * @class Objective
 * @sbmlbrief{fbc} An objective function.
 *
 * An integral component in a complete description of a steady-state model is
 * the so-called <em>objective function</em>, which generally consists of a
 * linear combination of model variables (fluxes) and a sense (direction). In
 * the SBML Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;) package, this concept
 * is succinctly captured in the Objective class.
 *
 * The Objective class is derived from the normal SBML SBase class and
 * inherits the "metaid" and "sboTerm" attributes, as well as the
 * subcomponents for Annotation and Notes.  To these, the Objective class
 * adds an optional attribute named "type".  The type attribute can take one
 * of two literal values: @c "maximize" or @c "minimize".  The values
 * represent the sense of the optimality constraint for the FBC model.
 *
 * The &ldquo;fbc&rdquo; package allows for the definition of multiple model
 * objectives, with one being designated as active.  The active objective is
 * indicated using the attribute "activeObjective" on the ListOfObjectives
 * object. Here is an example of the XML encoding of a model with a list of
 * objective functions:
 * @verbatim
<fbc:listOfObjectives fbc:activeObjective="obj1">
 <fbc:objective fbc:id="obj1" fbc:type="maximize">
  <fbc:listOfFluxObjectives>
   <fbc:fluxObjective fbc:reaction="R101" fbc:coefficient="1"/>
  </fbc:listOfFluxObjectives>
 </fbc:objective>
 <fbc:objective fbc:id="obj2" fbc:type="minimize">
  <fbc:listOfFluxObjectives>
   <fbc:fluxObjective fbc:reaction="R102" fbc:coefficient="-2.5"/>
   <fbc:fluxObjective fbc:reaction="R103" fbc:coefficient="1"/>
  </fbc:listOfFluxObjectives>
 </fbc:objective>
</fbc:listOfObjectives>
@endverbatim
 *
 * @see FluxObjective
 * @see ListOfObjectives
 */

#ifndef Objective_H__
#define Objective_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>

LIBSBML_CPP_NAMESPACE_BEGIN

/**
 * @enum ObjectiveType_t

 * @brief Enumeration of possible values for the 'type' attribute of an
 * Objective object.
 */
typedef enum
{
    OBJECTIVE_TYPE_MAXIMIZE /*!< Signifies the objective is to maximize. */
  , OBJECTIVE_TYPE_MINIMIZE /*!< Signifies the objective is to minimize. */
  , OBJECTIVE_TYPE_UNKNOWN  /*!< Unset or illegal value: anything other than 'maximize' or 'minimize'. */
} ObjectiveType_t;

LIBSBML_CPP_NAMESPACE_END


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>

#include <sbml/packages/fbc/sbml/FluxObjective.h>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN Objective : public SBase
{

protected:
  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
//  std::string   mName;
  ObjectiveType_t   mType;
  ListOfFluxObjectives   mFluxObjectives;
  bool mIsSetListOfFluxObjectives;
  std::string mTypeString;
  /** @endcond */

public:

  /**
   * Creates a new Objective with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this Objective.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * Objective.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this Objective.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Objective(unsigned int level      = FbcExtension::getDefaultLevel(),
            unsigned int version    = FbcExtension::getDefaultVersion(),
            unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new Objective with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Objective(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for Objective.
   *
   * @param orig the Objective instance to copy.
   */
  Objective(const Objective& orig);


   /**
   * Assignment operator for Objective.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  Objective& operator=(const Objective& rhs);


   /**
   * Creates and returns a deep copy of this Objective object.
   *
   * @return a (deep) copy of this Objective object.
   */
  virtual Objective* clone () const;


   /**
   * Destructor for Objective.
   */
  virtual ~Objective();


  /**
   * Returns the value of the "id" attribute of this Objective.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this Objective.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this Objective object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName() const;


  /**
   * Returns the enumeration value of the "type" attribute of this Objective.
   *
   * @return the enumeration value of the "type" attribute of this Objective.
   */
  virtual ObjectiveType_t getObjectiveType () const;


  /**
  * Returns the string of the "type" attribute of this Objective.
  *
  * @return the string of the "type" attribute of this Objective.
  */
  const std::string& getType();


  /**
   * Predicate returning @c true if this Objective's "id" attribute is set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true if this Objective's "name" attribute is set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true if this Objective's "type" attribute is set.
   *
   * @return @c true if this Objective's "type" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetType() const;


  /**
   * Sets the value of the "id" attribute of this Objective.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Sets the value of the "name" attribute of this Objective.
   *
   * @copydetails doc_set_name
   */
  virtual int setName(const std::string& name);


  /**
   * Sets the SIdRef string of the "type" attribute of this Objective.
   *
   * @param type a SIdRef string to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setType (ObjectiveType_t type);


  /**
   * Sets the value of the "type" attribute of this Objective.
   *
   * @param type string value of the "type" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setType(const std::string& type);


  /**
   * Unsets the value of the "id" attribute of this Objective.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this Objective.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName();


  /**
   * Unsets the value of the "type" attribute of this Objective.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetType();


  /**
   * Returns the  "ListOfFluxObjectives" in this Objective object.
   *
   * @return the "ListOfFluxObjectives" attribute of this Objective.
   */
  const ListOfFluxObjectives* getListOfFluxObjectives() const;


  /**
   * Returns the  "ListOfFluxObjectives" in this Objective object.
   *
   * @return the "ListOfFluxObjectives" attribute of this Objective.
   */
  ListOfFluxObjectives* getListOfFluxObjectives();


  /**
   * Get a FluxObjective from the ListOfFluxObjectives.
   *
   * @param n the index number of the FluxObjective to get.
   *
   * @return the nth FluxObjective in the ListOfFluxObjectives within this
   * Objective.
   *
   * @see getNumFluxObjectives()
   */
  FluxObjective* getFluxObjective(unsigned int n);


  /**
   * Get a FluxObjective from the ListOfFluxObjectives.
   *
   * @param n the index number of the FluxObjective to get.
   *
   * @return the nth FluxObjective in the ListOfFluxObjectives within this
   * Objective.
   *
   * @see getNumFluxObjectives()
   */
  const FluxObjective* getFluxObjective(unsigned int n) const;


  /**
   * Get a FluxObjective from the ListOfFluxObjectives based on its
   * identifier.
   *
   * @param sid a string representing the identifier of the FluxObjective to
   * get.
   *
   * @return the FluxObjective in the ListOfFluxObjectives with the given id
   * or NULL if no such FluxObjective exists.
   *
   * @see getFluxObjective(unsigned int n)
   *
   * @see getNumFluxObjectives()
   */
  FluxObjective* getFluxObjective(const std::string& sid);


  /**
   * Get a FluxObjective from the ListOfFluxObjectives based on its
   * identifier.
   *
   * @param sid a string representing the identifier of the FluxObjective to
   * get.
   *
   * @return the FluxObjective in the ListOfFluxObjectives with the given id
   * or NULL if no such FluxObjective exists.
   *
   * @see getFluxObjective(unsigned int n)
   *
   * @see getNumFluxObjectives()
   */
  const FluxObjective* getFluxObjective(const std::string& sid) const;


  /**
   * Adds a copy the given FluxObjective to this Objective.
   *
   * @param fo the FluxObjective object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int addFluxObjective(const FluxObjective* fo);


  /**
   * Get the number of FluxObjective objects in this Objective.
   *
   * @return the number of FluxObjective objects in this Objective.
   */
  unsigned int getNumFluxObjectives() const;


  /**
   * Creates a new FluxObjective object.
   *
   * This method creates the FluxObjective object, adds it to this Objective
   * object's ListOfFluxObjectives, and returns the FluxObjective object
   * created.
   *
   * @return a new FluxObjective object instance.
   *
   * @see addFluxObjective(const FluxObjective* fo)
   */
  FluxObjective* createFluxObjective();


  /**
   * Removes the nth FluxObjective from the ListOfFluxObjectives.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the FluxObjective to remove.
   *
   * @see getNumFluxObjectives()
   */
  FluxObjective* removeFluxObjective(unsigned int n);


  /**
   * Removes the FluxObjective with the given identifier from the
   * ListOfFluxObjectives.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the FluxObjective to remove.
   *
   * @return the FluxObjective removed. As mentioned above, the caller owns the
   * returned item.
   */
  FluxObjective* removeFluxObjective(const std::string& sid);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object.
   *
   * For Objective, the XML element name is always @c "objective".
   *
   * @return the name of this element, i.e. @c "objective".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_OBJECTIVE, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes for this
   * Objective object have been set.
   *
   * @note The required attributes for a Objective object are:
   * @li "id"
   * @li "type"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements for this
   * Objective object have been set.
   *
   * @note The required elements for a Objective object are:
   * @li at least one FluxObjective child of the ListOfFluxObjectives.
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  virtual bool hasRequiredElements() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Connects to child elements.
   */
  virtual void connectToChild ();

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  bool getIsSetListOfFluxObjectives() const;
  /** @endcond */

protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * return the SBML object corresponding to next XMLToken.
   */
  virtual SBase* createObject(XMLInputStream& stream);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;

  /** @endcond */

};


/**
 * @class ListOfObjectives
 * @sbmlbrief{fbc} A list of Objective objects.
 *
 * The ListOfObjectives is a container for the SBML extended Model
 * that lists all the possible Objective elements in the model.
 *
 * Unlike most other ListOf subclasses in SBML, SBML Level&nbsp;3 @ref fbc
 * Version&nbsp;2 defines an additional required attribute on
 * ListOfObjectives: the "activeObjective" attribute.  This attribute is of
 * type <code>SIdRef</code> and can only refer to the id of an existing
 * Objective. This required attribute exists so that when multiple
 * Objective's are included in a single model, the model will always be well
 * described; i.e., there will be a single, primary objective function which
 * defines a single optimum and its associated solution space.
 *
 * @copydetails doc_what_is_listof
 *
 * @see GeneProduct
 * @see FbcModelPlugin
 *
 * @warning The required attribute "activeObjective" on ListOfObjectives is
 * an additional attribute that is not present on typical ListOf classes.
 * The introduction of an attribute on ListOf is perfectly legal in SBML, but
 * uncommon, and software developers may have grown accustomed to ListOf
 * classes all having the same attributes and no others.  We are belaboring
 * this point so that developers are more likely to notice the presence of an
 * additional attribute on ListOfObjectives.
 */
class LIBSBML_EXTERN ListOfObjectives : public ListOf
{

public:

  /**
   * Creates a new ListOfObjectives with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * ListOfObjectives.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * ListOfObjectives.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this ListOfObjectives.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfObjectives(unsigned int level      = FbcExtension::getDefaultLevel(),
                   unsigned int version    = FbcExtension::getDefaultVersion(),
                   unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfObjectives with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfObjectives(FbcPkgNamespaces* fbcns);


  /**
   * Copy Constructor.
   *
   * @param other the instance to copy.
   */
  ListOfObjectives(const ListOfObjectives& other);


  /**
   * Assignment operator.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  ListOfObjectives& operator=(const ListOfObjectives& rhs);


   /**
   * Creates and returns a deep copy of this ListOfObjectives object.
   *
   * @return a (deep) copy of this ListOfObjectives object.
   */
  virtual ListOfObjectives* clone () const;


   /**
   * Get an Objective from the ListOfObjectives.
   *
   * @param n the index number of the Objective to get.
   *
   * @return the nth Objective in this ListOfObjectives.
   *
   * @see size()
   */
  virtual Objective* get(unsigned int n);


  /**
   * Get an Objective from the ListOfObjectives.
   *
   * @param n the index number of the Objective to get.
   *
   * @return the nth Objective in this ListOfObjectives.
   *
   * @see size()
   */
  virtual const Objective* get(unsigned int n) const;


  /**
   * Get an Objective from the ListOfObjectives based on its identifier.
   *
   * @param sid a string representing the identifier of the Objective to get.
   *
   * @return Objective in this ListOfObjectives with the given id or NULL if
   * no such Objective exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual Objective* get(const std::string& sid);


  /**
   * Get an Objective from the ListOfObjectives based on its identifier.
   *
   * @param sid a string representing the identifier of the Objective to get.
   *
   * @return Objective in this ListOfObjectives with the given id or NULL if
   * no such Objective exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const Objective* get(const std::string& sid) const;


  /**
   * Adds a copy the given Objective to this ListOfObjectives.
   *
   * @param o the Objective object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int addObjective(const Objective* o);


  /**
   * Get the number of Objective objects in this ListOfObjectives.
   *
   * @return the number of Objective objects in this ListOfObjectives.
   */
  unsigned int getNumObjectives() const;


  /**
   * Creates a new Objective object, adds it to the
   * ListOfObjectives and returns the Objective object created. 
   *
   * @return a new Objective object instance.
   *
   * @see addObjective(const Objective* o)
   */
  Objective* createObjective();


  /**
   * Removes the nth Objective from this ListOfObjectives
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Objective to remove.
   *
   * @see size()
   */
  virtual Objective* remove(unsigned int n);


  /**
   * Removes the Objective from this ListOfObjectives with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Objective to remove.
   *
   * @return the Objective removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual Objective* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object.
   *
   * For ListOfObjectives, the XML element name is always @c "listOfObjectives".
   *
   * @return the name of this element, i.e. @c "listOfObjectives".
   */
  virtual const std::string& getElementName () const;


  /**
  * Predicate returning @c true if this ListOfObjective's "activeObjective"
  * attribute is set.
  *
  * @return @c true if this ListOfObjective's "activeObjective" attribute has been set,
  * otherwise @c false is returned.
  */
  virtual bool isSetActiveObjective() const;


  /**
  * Sets the value of the "activeObjective" attribute of this ListOfObjectives.
  *
  * @copydetails doc_returns_success_code
  * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
  * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
  */
  virtual int setActiveObjective(const std::string &activeObjective);


  /**
  * Returns the value of the "activeObjective" attribute of this ListOfObjectives.
  *
  * @return the value of the "activeObjective" attribute of this ListOfObjectives.
  */
  virtual const std::string& getActiveObjective() const;


  /**
  * Unsets the value of the "activeObjective" attribute of this ListOfObjectives.
  *
  * @copydetails doc_returns_success_code
  * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
  * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
  */
  virtual int unsetActiveObjective();


  /** @cond doxygenLibsbmlInternal */
  virtual bool accept(SBMLVisitor& v) const;
  /** @endcond */


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LIST_OF, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Returns the libSBML type code for the objects contained in this ListOf.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for the objects contained in this ListOf
   * instance: @sbmlconstant{SBML_FBC_OBJECTIVE, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


  /**
  * Adds a clone of all items in the provided ListOf to this object.
  *
  * This means that when this ListOf is destroyed, the original items will
  * not be destroyed.  In addition, copy over the input ListOfObjectives'
  * 'activeObjective' attribute, if none is set for this element.
  *
  * @param list a list of items to be added.
  *
  * @see append(const SBase* item)
  */
  virtual int appendFrom(const ListOf* list);


  /**
  * @copydoc doc_renamesidref_common
  */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);

protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new Objective in this ListOfObjectives
   */
  virtual SBase* createObject(XMLInputStream& stream);

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
   * Write the namespace for the &ldquo;fbc&rdquo; package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
  * Reads the attributes of corresponding package in SBMLDocument element.
  */
  virtual void readAttributes(const XMLAttributes& attributes,
    const ExpectedAttributes& expectedAttributes);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
  * Writes the attributes of corresponding package in SBMLDocument element.
  */
  virtual void writeAttributes(XMLOutputStream& stream) const;

  /** @endcond */

  std::string mActiveObjective;

};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new Objective_t structure using the given SBML @p level and
 * @p version values.
 *
 * @param level an unsigned int, the SBML level to assign to this
 * Objective_t structure.
 *
 * @param version an unsigned int, the SBML version to assign to this
 * Objective_t structure.
 *
 * @returns the newly-created Objective_t structure, or a null pointer if
 * an error occurred during construction.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
Objective_t *
Objective_create(unsigned int level, unsigned int version,
                 unsigned int pkgVersion);


/**
 * Frees the given Objective_t structure.
 * 
 * @param o the Objective_t structure to be freed.
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
void
Objective_free(Objective_t * o);


/**
 * Creates a deep copy of the given Objective_t structure.
 * 
 * @param o the Objective_t structure to be copied.
 *
 * @returns a (deep) copy of the given Objective_t structure, or a null
 * pointer if a failure occurred.
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
Objective_t *
Objective_clone(Objective_t * o);


/**
 * Returns the value of the "id" attribute of the given Objective_t
 * structure.
 *
 * @param o the Objective_t structure.
 *
 * @return the id of this structure.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
const char *
Objective_getId(const Objective_t * o);


/**
 * Returns the value of the "name" attribute of the given Objective_t
 * structure.
 *
 * @param o the Objective_t structure.
 *
 * @return the name of this structure.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
const char *
Objective_getName(const Objective_t * o);


/**
* Takes a Objective_t structure and returns its type.
*
* @param obj the Objective_t whose type is sought.
*
* @return the type of the given Objective_t, as a pointer to a string.
*
* @memberof Objective_t
*/
LIBSBML_EXTERN
const char *
Objective_getType(Objective_t * obj);


/**
 * Predicate returning @c 1 if the given Objective_t structure's "id"
 * is set.
 *
 * @param o the Objective_t structure.
 *
 * @return @c 1 if the "id" of this Objective_t structure is
 * set, @c 0 otherwise.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_isSetId(const Objective_t * o);


/**
 * Predicate returning @c 1 if the given Objective_t structure's "name"
 * is set.
 *
 * @param o the Objective_t structure.
 *
 * @return @c 1 if the "name" of this Objective_t structure is
 * set, @c 0 otherwise.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_isSetName(const Objective_t * o);


/**
 * Predicate returning @c 1 if the given Objective_t structure's "type"
 * is set.
 *
 * @param o the Objective_t structure.
 *
 * @return @c 1 if the "type" of this Objective_t structure is
 * set, @c 0 otherwise.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_isSetType(const Objective_t * o);


/**
 * Sets the "id" attribute of the given Objective_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs Objective_unsetId() instead.
 *
 * @param o the Objective_t structure.
 *
 * @param id the string to which the structures "id" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_setId(Objective_t * o, const char * id);


/**
 * Sets the "name" attribute of the given Objective_t structure.
 *
 * This function copies the string given in @p string.  If the string is
 * a null pointer, this function performs Objective_unsetName() instead.
 *
 * @param o the Objective_t structure.
 *
 * @param name the string to which the structures "name" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @note Using this function with a null pointer for @p name is equivalent to
 * unsetting the value of the "name" attribute.
 * 
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_setName(Objective_t * o, const char * name);


/**
 * Sets the "type" attribute of the given Objective_t structure.
 *
 * @param o the Objective_t structure.
 *
 * @param type the string to which the structures "type" attribute should be
 * set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_setType(Objective_t * o, const char* type);


/**
 * Unsets the value of the "id" attribute of the given 
 * Objective_t structure.
 *
 * @param o the Objective_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_unsetId(Objective_t * o);


/**
 * Unsets the value of the "name" attribute of the given 
 * Objective_t structure.
 *
 * @param o the Objective_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_unsetName(Objective_t * o);


/**
 * Unsets the value of the "type" attribute of the given 
 * Objective_t structure.
 *
 * @param o the Objective_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_unsetType(Objective_t * o);


LIBSBML_EXTERN
int
Objective_addFluxObjective(Objective_t * o, FluxObjective_t * fo);


LIBSBML_EXTERN
FluxObjective_t *
Objective_createFluxObjective(Objective_t * o);


LIBSBML_EXTERN
ListOf_t *
Objective_getListOfFluxObjectives(Objective_t * o) ;


LIBSBML_EXTERN
FluxObjective_t *
Objective_getFluxObjective(Objective_t * o, unsigned int n);


LIBSBML_EXTERN
FluxObjective_t *
Objective_getFluxObjectiveById(Objective_t * o, const char * sid);


LIBSBML_EXTERN
unsigned int
Objective_getNumFluxObjectives(Objective_t * o);


LIBSBML_EXTERN
FluxObjective_t *
Objective_removeFluxObjective(Objective_t * o, unsigned int n);


LIBSBML_EXTERN
FluxObjective_t *
Objective_removeFluxObjectiveById(Objective_t * o, const char * sid);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * attributes of the given Objective_t structure have been set.
 *
 * @param o the Objective_t structure to check.
 *
 * @return @c 1 if all the required attributes for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_hasRequiredAttributes(const Objective_t * o);


/**
 * Predicate returning @c 1 or *c 0 depending on whether all the required
 * sub-elements of the given Objective_t structure have been set.
 *
 * @param o the Objective_t structure to check.
 *
 * @return @c 1 if all the required sub-elements for this
 * structure have been defined, @c 0 otherwise.
 *
 * @member of Objective_t
 */
LIBSBML_EXTERN
int
Objective_hasRequiredElements(const Objective_t * o);


LIBSBML_EXTERN
Objective_t *
ListOfObjectives_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
Objective_t *
ListOfObjectives_removeById(ListOf_t * lo, const char * sid);




/**
 * Returns the string version of the provided ObjectiveType_t enumeration.
 *
 * @param type the ObjectiveType_t enumeration to convert.
 *
 * @return A string corresponding to the given effect:  "maximize",
 * "minimize", or @c NULL if the value is OBJECTIVE_TYPE_UNKNOWN
 * or another invalid enumeration value.
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
const char*
ObjectiveType_toString(ObjectiveType_t type);


/**
 * Returns the ObjectiveType_t enumeration corresponding to
 * the given string, or OBJECTIVE_TYPE_UNKNOWN if there is
 * no such match.  The matching is case-sensitive:  "maximize" will
 * return OBJECTIVE_TYPE_MAXIMIZE, but "Maximize" will return
 * OBJECTIVE_TYPE_UNKNOWN.
 *
 * @param s the string to convert to an ObjectiveType_t.
 *
 * @return The corresponding ObjectiveType_t, or
 * OBJECTIVE_TYPE_UNKNOWN if no match found.
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
ObjectiveType_t
ObjectiveType_fromString(const char* s);


/**
 * Predicate returning @c true (non-zero) or @c false (zero) depending on whether the given
 * ObjectiveType_t is valid.
 *
 * @param type the ObjectiveType_t enumeration to query.
 *
 * @return @c non-zero (true) if the ObjectiveType_t is
 * OBJECTIVE_TYPE_MAXIMIZE or OBJECTIVE_TYPE_MINIMIZE;
 * zero (false) otherwise (including OBJECTIVE_TYPE_UNKNOWN).
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
int
ObjectiveType_isValidObjectiveType(ObjectiveType_t type);


/**
 * Predicate returning @c true (non-zero) or @c false (zero) depending
 * on whether the given string is a valid ObjectiveType_t.
 * The matching is case-sensitive:  "maximize" will return @c true, but
 * "Maximize" will return @c false.
 *
 * @param s the string to query.
 *
 * @return @c non-zero (true) if the string is
 * "maximize" or "minimize"; zero (false) otherwise.
 *
 * @memberof Objective_t
 */
LIBSBML_EXTERN
int
ObjectiveType_isValidObjectiveTypeString(const char* s);

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  Objective_H__  */

