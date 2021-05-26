/**
 * @file    FluxBound.h
 * @brief   Definition of FluxBound, the SBase derived class of the fbc package.
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
 * @class FluxBound
 * @sbmlbrief{fbc} Max or min value for a reaction flux.
 *
 * The FluxBound class of objects is used in Version&nbsp;1 of the SBML
 * Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;) package to express a single
 * (in)equality that provides the maximum or minimum value that a reaction
 * flux can obtain at steady state.  (This same information is encoded
 * differently in Version&nbsp;2 of &ldquo;fbc&rdquo;; see the
 * "upperFluxBound" and "lowerFluxBound" attributes on FbcReactionPlugin.)
 *
 * @section fluxbound-attributes Attributes on FluxBound
 *
 * In addition to the common optional attributes "id" and "name", FluxBound
 * takes three required attributes: "reaction", "operation" and "value".
 * These three attributes define the meaning of the FluxBound, and are
 * interpreted using the following expression:
 * <center>
 * <em>reaction</em>&nbsp;&nbsp;&nbsp;<em>operator</em>&nbsp;&nbsp;&nbsp;<em>value</em>
 * </center>
 *
 * The "reaction" attribute takes a value of <code>SIdRef</code>.  The value
 * must be the identifier of a Reaction object defined within the enclosing
 * model.
 *
 * The "operation" attribute takes a value from an enumeration
 * (#FluxBoundOperation_t) of Boolean operations that represent mathemetical
 * inequalities.  Possible values for "operation" include
 * <code>"greaterEqual"</code>, <code>"equal"</code>, and others.
 *
 * The "value" attribute takes a numerical value of type <code>double</code>,
 * and represents the value of the flux bound.  The permitted values include
 * positive infinity (<code>"INF"</code>) and negative infinity
 * (<code>"-INF"</code>).
 *
 * The following is an example of a set of flux bounds encoded in this form; it
 * also demonstrates the use of ListOfFluxBounds.
 *
 * @verbatim
<fbc:listOfFluxBounds>
    <fbc:fluxBound fbc:id="R1b" fbc:reaction="R1" fbc:operation="greaterEqual" fbc:value="1.2"/>
    <fbc:fluxBound fbc:id="R2b" fbc:reaction="R2" fbc:operation="lessEqual" fbc:value="-1.2"/>
    <fbc:fluxBound fbc:id="R3b" fbc:reaction="R3" fbc:operation="greaterEqual" fbc:value="-INF"/>
    <fbc:fluxBound fbc:id="R4b" fbc:reaction="R4" fbc:operation="lessEqual" fbc:value="INF"/>
    <fbc:fluxBound fbc:id="R5b" fbc:reaction="R5" fbc:operation="equal" fbc:value="1"/>
</fbc:listOfFluxBounds>
@endverbatim
 *
 * @note This class is only defined for Version&nbsp;1 of the
 * &ldquo;fbc&rdquo; package specification.  It was replaced in
 * Version&nbsp;2 by a Parameter referenced by the "upperFluxBound" or
 * "lowerFluxBound" attributes on an FbcReactionPlugin.  FluxBound is
 * therefore not used for Version&nbsp;2 &ldquo;fbc&rdquo; models.
 *
 * @see ListOfFluxBounds
 */

#ifndef FluxBound_H__
#define FluxBound_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>

LIBSBML_CPP_NAMESPACE_BEGIN

  /**
   * @enum FluxBoundOperation_t
   * @brief Possible values for the FluxBound 'operation' attribute.
   *
   * The possible legal values are less than or equal to, greater than or
   * equal to, or equal to.  The two options <i>less than</i> and <i>greater
   * than</i> are not legal values for the FluxBound 'operation' attribute,
   * but are provided to allow backwards compatibility with an earlier
   * version of the draft specification.
   */
typedef enum
{
    FLUXBOUND_OPERATION_LESS_EQUAL    /*!< Less than or equal to. */
  , FLUXBOUND_OPERATION_GREATER_EQUAL /*!< Greater than or equal to.*/
  , FLUXBOUND_OPERATION_LESS          /*!< Less than. NOTE:  ILLEGAL VALUE. */
  , FLUXBOUND_OPERATION_GREATER       /*!< Greater than. NOTE:  ILLEGAL VALUE. */
  , FLUXBOUND_OPERATION_EQUAL         /*!< Equal to. */
  , FLUXBOUND_OPERATION_UNKNOWN       /*!< Unknown operation. */
} FluxBoundOperation_t;

LIBSBML_CPP_NAMESPACE_END


#ifdef __cplusplus

#include <string>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FluxBound : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
//  std::string   mId;
//  std::string   mName;
  std::string   mReaction;
  FluxBoundOperation_t   mOperation;
  std::string   mOperationString;
  double        mValue;
  /** @endcond */

public:

  /**
   * Creates a new FbcAssociation with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * FluxObjective.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * FluxObjective.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this FluxObjective.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FluxBound(unsigned int level      = FbcExtension::getDefaultLevel(),
            unsigned int version    = FbcExtension::getDefaultVersion(),
            unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new FluxBound with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   FluxBound(FbcPkgNamespaces* fbcns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
   FluxBound(const FluxBound& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
   FluxBound& operator=(const FluxBound& source);


  /**
   * Destructor.
   */
  virtual ~FluxBound ();


  /**
   * Returns the value of the "id" attribute of this FluxBound.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this FluxBound.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId () const;


  /**
   * Predicate returning @c true if this FluxBound's "id" attribute is set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this FluxBound.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Unsets the value of the "id" attribute of this FluxBound object.
   *
   * @copydetails doc_unset_id
   */
  virtual int unsetId ();


  /**
   * Returns the value of the "name" attribute of this FluxBound object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName () const;


  /**
   * Predicate returning @c true if this FluxBound's "name" attribute is set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName () const;


  /**
   * Sets the value of the "name" attribute of this FluxBound.
   *
   * @copydetails doc_set_name
   */
  virtual int setName (const std::string& name);


  /**
   * Unsets the value of the "name" attribute of this FluxBound object.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName ();


  /**
   * Returns the value of the "reaction" attribute of this FluxBound object.
   *
   * @return the value of the "reaction" attribute of this FluxBound object.
   */
  virtual const std::string& getReaction () const;


  /**
   * Predicate returning @c true if this FluxBound's "reaction" attribute is
   * set.
   *
   * @return @c true if this FluxBound object's "reaction" attribute has been
   * set, otherwise @c false is returned.
   */
  virtual bool isSetReaction () const;


  /**
   * Sets the value of the "reaction" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setReaction (const std::string& reaction);


  /**
   * Unsets the value of the "reaction" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetReaction ();


  /**
   * Returns the value of the "operation" attribute of this FluxBound object.
   *
   * @return the value of the "operation" attribute of this FluxBound object.
   */
  const std::string& getOperation ();


  /**
   * Returns the value of the "operation" attribute of this FluxBound object.
   *
   * @return the value of the "operation" attribute of this FluxBound object.
   */
  FluxBoundOperation_t getFluxBoundOperation () const;


  /**
   * Predicate returning @c true if this FluxBound's "operation" attribute is
   * set.
   *
   * @return @c true if this FluxBound object's "operation" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetOperation () const;


  /**
   * Sets the value of the "operation" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setOperation (const std::string& operation);


  /**
   * Sets the value of the "operation" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setOperation (FluxBoundOperation_t operation);


  /**
   * Unsets the value of the "operation" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetOperation ();

  /**
   * Returns the value of the "value" attribute of this FluxBound object.
   *
   * @return the value of the "value" attribute of this FluxBound object.
   */
  virtual double getValue () const;


  /**
   * Predicate returning @c true if this FluxBound's "value" attribute is
   * set.
   *
   * @return @c true if this FluxBound object's "value" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetValue () const;


  /**
   * Sets the value of the "value" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setValue (const double value);


  /**
   * Unsets the value of the "value" attribute of this FluxBound object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetValue ();


  /**
   * @copydoc doc_renamesidref_common
   */
   virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the XML element name of this object.
   *
   * For FluxBound, the XML element name is always @c "fluxBound".
   *
   * @return the name of this element, i.e. @c "fluxBound".
   */
  virtual const std::string& getElementName () const ;


  /**
   * Creates and returns a deep copy of this FluxBound object.
   *
   * @return a (deep) copy of this FluxBound object.
   */
  virtual FluxBound* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_FLUXBOUND, SBMLFbcTypeCode_t}.
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


/**
 * @class ListOfFluxBounds
 * @sbmlbrief{fbc} A list of FluxBound objects.
 *
 * The ListOfFluxBounds is a container for the FluxBound elements of a Model.
 * It is only defined for Version&nbsp;1 of the &ldquo;fbc&rdquo;
 * specification, and was replaced in Version&nbsp;2 by Parameter elements
 * referenced by the "upperFluxBound" or "lowerFluxBound" attributes on an
 * FbcReactionPlugin.  FluxBound and ListOfFluxBounds are not used for
 * Version&nbsp;2 &ldquo;fbc&rdquo; models.
 *
 * @copydetails doc_what_is_listof
 *
 * @see FluxBound
 *
 * @note This class and FluxBound are only defined for Version&nbsp;1 of the
 * &ldquo;fbc&rdquo; package specification.  These classes are not used in
 * Version&nbsp;2 of &ldquo;fbc&rdquo;.
 */
class LIBSBML_EXTERN ListOfFluxBounds : public ListOf
{
public:

  /**
   * Creates and returns a deep copy of this ListOfFluxBounds.
   *
   * @return a (deep) copy of this ListOfFluxBounds.
   */
  virtual ListOfFluxBounds* clone () const;


  /**
   * Creates a new ListOfFluxBounds with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfFluxBounds(unsigned int level      = FbcExtension::getDefaultLevel(),
                   unsigned int version    = FbcExtension::getDefaultVersion(),
                   unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfFluxBounds with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   ListOfFluxBounds(FbcPkgNamespaces* fbcns);


  /**
   * Get a FluxBound from the ListOfFluxBounds.
   *
   * @param n the index number of the FluxBound to get.
   *
   * @return the nth FluxBound in this ListOfFluxBounds.
   *
   * @see size()
   */
  virtual FluxBound* get(unsigned int n);


  /**
   * Get a FluxBound from the ListOfFluxBounds.
   *
   * @param n the index number of the FluxBound to get.
   *
   * @return the nth FluxBound in this ListOfFluxBounds.
   *
   * @see size()
   */
  virtual const FluxBound * get(unsigned int n) const;


  /**
   * Get a FluxBound from the ListOfFluxBounds
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FluxBound to get.
   *
   * @return FluxBound in this ListOfFluxBounds
   * with the given @p sid or @c NULL if no such
   * FluxBound exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual FluxBound* get (const std::string& sid);


  /**
   * Get a FluxBound from the ListOfFluxBounds
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FluxBound to get.
   *
   * @return FluxBound in this ListOfFluxBounds
   * with the given @p sid or @c NULL if no such
   * FluxBound exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const FluxBound* get (const std::string& sid) const;


  /**
   * Removes the nth item from this ListOfFluxBounds items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove.
   *
   * @see size()
   */
  virtual FluxBound* remove (unsigned int n);


  /**
   * Removes item in this ListOfFluxBounds items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove.
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual FluxBound* remove (const std::string& sid);


  /**
   * Returns the libSBML type code for the SBML objects
   * contained in this ListOf object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for objects contained in this list:
   * @sbmlconstant{SBML_FBC_FLUXBOUND, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


  /**
   * Returns the XML element name of this object.
   *
   * For ListOfFluxBounds, the XML element name is always @c "listOfFluxBounds".
   *
   * @return the name of this element, i.e. @c "listOfFluxBounds".
   */
  virtual const std::string& getElementName () const;


protected:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  virtual void writeXMLNS (XMLOutputStream& stream) const;
  /** @endcond */
};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new FluxBound_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * FluxBound_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * FluxBound_t.
 * @param pkgVersion an unsigned int, the SBML 'Qual' package Version to assign to this
 * FluxBound_t.
 *
 * @return a pointer to the newly created FluxBound_t structure.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
FluxBound_t *
FluxBound_create(unsigned int level, unsigned int version, unsigned int pkgVersion);


/**
 * Takes an FluxBound_t structure and returns its identifier.
 *
 * @param fb the FluxBound_t structure whose identifier is sought.
 *
 * @return the identifier of the given FluxBound_t, as a pointer to a string.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
const char *
FluxBound_getId(FluxBound_t * fb);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FluxBound_t structure's identifier is set.
 *
 * @param fb the FluxBound_t structure to query.
 *
 * @return @c non-zero (true) if the "id" attribute of the given
 * FluxBound_t structure is set, zero (false) otherwise.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_isSetId(FluxBound_t * fb);


/**
 * Assigns the identifier of an FluxBound_t structure.
 *
 * This makes a copy of the string passed in the param @p sid.
 *
 * @param fb the FluxBound_t structure to set.
 * @param sid the string to use as the identifier.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with an id of NULL is equivalent to
 * unsetting the "id" attribute.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_setId(FluxBound_t * fb, const char * sid);


/**
 * Unsets the "id" attribute of the given FluxBound_t structure.
 *
 * @param fb the FluxBound_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_unsetId(FluxBound_t * fb);

/**
 * Takes a FluxBound_t structure and returns its name.
 *
 * @param fb the FluxBound_t whose name is sought.
 *
 * @return the name of the given FluxBound_t, as a pointer to a string.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
const char *
FluxBound_getName(FluxBound_t * fb);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FluxBound_t structure's name is set.
 *
 * @param fb the FluxBound_t structure to query.
 *
 * @return @c non-zero (true) if the "name" attribute of the given
 * FluxBound_t structure is set, zero (false) otherwise.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_isSetName(FluxBound_t * fb);


/**
 * Sets the name of the given FluxBound_t to a copy of @p name.
 *
 * @param fb the FluxBound_t structure to set.
 * @param name the name to assign to the given FluxBound_t's "name" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "name" attribute.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_setName(FluxBound_t * fb, const char * name);


/**
 * Unsets the "name" attribute of the given FluxBound_t structure.
 *
 * @param fb the FluxBound_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_unsetName(FluxBound_t * fb);


/**
 * Takes a FluxBound_t structure and returns its reaction.
 *
 * @param fb the FluxBound_t whose reaction is sought.
 *
 * @return the reaction of the given FluxBound_t, as a pointer to a string.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
const char *
FluxBound_getReaction(FluxBound_t * fb);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FluxBound_t structure's reaction is set.
 *
 * @param fb the FluxBound_t structure to query.
 *
 * @return @c non-zero (true) if the "reaction" attribute of the given
 * FluxBound_t structure is set, zero (false) otherwise.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_isSetReaction(FluxBound_t * fb);


/**
 * Sets the reaction of the given FluxBound_t to a copy of @p reaction.
 *
 * @param fb the FluxBound_t structure to set.
 * @param reaction the reaction to assign to the given FluxBound_t's "reaction" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "reaction" attribute.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_setReaction(FluxBound_t * fb, const char * reaction);


/**
 * Unsets the "reaction" attribute of the given FluxBound_t structure.
 *
 * @param fb the FluxBound_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_unsetReaction(FluxBound_t * fb);


/**
 * Takes a FluxBound_t structure and returns its operation.
 *
 * @param fb the FluxBound_t whose operation is sought.
 *
 * @return the operation of the given FluxBound_t, as a pointer to a string.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
const char *
FluxBound_getOperation(FluxBound_t * fb);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FluxBound_t structure's operation is set.
 *
 * @param fb the FluxBound_t structure to query.
 *
 * @return @c non-zero (true) if the "operation" attribute of the given
 * FluxBound_t structure is set, zero (false) otherwise.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_isSetOperation(FluxBound_t * fb);


/**
 * Sets the operation of the given FluxBound_t to a copy of @p operation.
 *
 * @param fb the FluxBound_t structure to set.
 * @param operation the operation to assign to the given FluxBound_t's "operation" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "operation" attribute.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_setOperation(FluxBound_t * fb, const char * operation);


/**
 * Unsets the "operation" attribute of the given FluxBound_t structure.
 *
 * @param fb the FluxBound_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_unsetOperation(FluxBound_t * fb);


/**
 * Takes a FluxBound_t structure and returns its value.
 *
 * @param fb the FluxBound_t whose value is sought.
 *
 * @return the value attribute of the given FluxBound_t, as a @c double.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
double
FluxBound_getValue(FluxBound_t * fb);


/**
 * Predicate returning @c true or @c false depending on whether the given
 * FluxBound_t structure's value is set.
 *
 * @param fb the FluxBound_t structure to query.
 *
 * @return @c non-zero (true) if the "value" attribute of the given
 * FluxBound_t structure is set, zero (false) otherwise.
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_isSetValue(FluxBound_t * fb);


/**
 * Sets the "value" attribute of the given FluxBound_t
 * structure.
 *
 * @param fb the FluxBound_t structure.
 *
 * @param value the value of value to assign to the "value" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_setValue(FluxBound_t * fb, double value);


/**
 * Unsets the "value" attribute of the given FluxBound_t structure.
 *
 * @param fb the FluxBound_t structure to unset.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBound_unsetValue(FluxBound_t * fb);

/**
 * Returns the string version of the provided FluxBoundOperation_t enumeration.
 *
 * @param type the FluxBoundOperation_t enumeration to convert.
 *
 * @return A string corresponding to the given effect:  "lessEqual",
 * "greaterEqual", "equal", or @c NULL if the value is FLUXBOUND_OPERATION_UNKNOWN
 * or another invalid enumeration value.
 *
 * @note In an earlier version of this specification, "less" and "greater" were
 * options that were dropped in the final version of the specification.
 * Accordingly, "less" is always converted to "lessEqual", and "greater" is
 * always converted to "greaterEqual".
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
const char*
FluxBoundOperation_toString(FluxBoundOperation_t type);


/**
 * Returns the FluxBoundOperation_t enumeration corresponding to
 * the given string, or FLUXBOUND_OPERATION_UNKNOWN if there is
 * no such match.  The matching is case-sensitive:  "lessEqual" will
 * return FLUXBOUND_OPERATION_LESS_EQUAL, but "lessequal" will return
 * FLUXBOUND_OPERATION_UNKNOWN.
 *
 * @param s the string to convert to an FluxBoundOperation_t.
 *
 * @return The corresponding FluxBoundOperation_t, or
 * FLUXBOUND_OPERATION_UNKNOWN if no match found.
 *
 * @note In an earlier version of this specification, "less" and "greater" were
 * options that were dropped in the final version of the specification.
 * Accordingly, "less" is always converted to "lessEqual", and "greater" is
 * always converted to "greaterEqual".
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
FluxBoundOperation_t
FluxBoundOperation_fromString(const char* s);


/**
 * Predicate returning @c true (non-zero) or @c false (zero) depending on whether the given
 * FluxBoundOperation_t is valid.
 *
 * @param type the FluxBoundOperation_t enumeration to query.
 *
 * @return @c non-zero (true) if the FluxBoundOperation_t is
 * FLUXBOUND_OPERATION_LESS_EQUAL, FLUXBOUND_OPERATION_GREATER_EQUAL,
 * FLUXBOUND_OPERATION_LESS, FLUXBOUND_OPERATION_GREATER, or
 * FLUXBOUND_OPERATION_EQUAL;
 * zero (false) otherwise (including FLUXBOUND_OPERATION_UNKNOWN).
 *
 * @note In an earlier version of this specification, "less" and "greater" were
 * options that were dropped in the final version of the specification.
 * Accordingly, "less" is always converted to "lessEqual", and "greater" is
 * always converted to "greaterEqual".
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBoundOperation_isValidFluxBoundOperation(FluxBoundOperation_t type);


/**
 * Predicate returning @c true (non-zero) or @c false (zero) depending
 * on whether the given string is a valid FluxBoundOperation_t.
 * The matching is case-sensitive:  "lessEqual" will return @c true, but
 * "lessequal" will return @c false.
 *
 * @param s the string to query.
 *
 * @return @c non-zero (true) if the string is
 * "lessEqual", "greaterEqual", "less", "greater", or "equal"; zero (false) otherwise.
 *
 * @note In an earlier version of this specification, "less" and "greater" were
 * options that were dropped in the final version of the specification.
 * Accordingly, "less" is always converted to "lessEqual", and "greater" is
 * always converted to "greaterEqual".
 *
 * @memberof FluxBound_t
 */
LIBSBML_EXTERN
int
FluxBoundOperation_isValidFluxBoundOperationString(const char* s);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* FluxBound_H__ */
