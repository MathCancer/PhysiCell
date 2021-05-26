/**
 * @file    ModelCreator.h
 * @brief   ModelCreator I/O
 * @author  Sarah Keating
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
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 * 
 * @class ModelCreator
 * @sbmlbrief{core} MIRIAM-compliant data about a model's creator.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The SBML specification beginning with Level&nbsp;2 Version&nbsp;2
 * defines a standard approach to recording model history and model creator
 * information in a form that complies with MIRIAM ("Minimum Information
 * Requested in the Annotation of biochemical Models", <i>Nature
 * Biotechnology</i>, vol. 23, no. 12, Dec. 2005).  For the model creator,
 * this form involves the use of parts of the <a target="_blank"
 * href="http://en.wikipedia.org/wiki/VCard">vCard</a> representation.
 * LibSBML provides the ModelCreator class as a convenience high-level
 * interface for working with model creator data.  Objects of class
 * ModelCreator can be used to store and carry around creator data within a
 * program, and the various methods in this object class let callers
 * manipulate the different parts of the model creator representation.
 *
 * @section parts The different parts of a model creator definition
 *
 * The ModelCreator class mirrors the structure of the MIRIAM model creator
 * annotations in SBML.  The following template illustrates these different
 * fields when they are written in XML form:
 *
 <pre class="fragment">
 &lt;vCard:N rdf:parseType="Resource"&gt;
   &lt;vCard:Family&gt;<span style="background-color: #bbb">family name</span>&lt;/vCard:Family&gt;
   &lt;vCard:Given&gt;<span style="background-color: #bbb">given name</span>&lt;/vCard:Given&gt;
 &lt;/vCard:N&gt;
 ...
 &lt;vCard:EMAIL&gt;<span style="background-color: #bbb">email address</span>&lt;/vCard:EMAIL&gt;
 ...
 &lt;vCard:ORG rdf:parseType="Resource"&gt;
   &lt;vCard:Orgname&gt;<span style="background-color: #bbb">organization</span>&lt;/vCard:Orgname&gt;
 &lt;/vCard:ORG&gt;
 </pre>
 *
 * Each of the separate data values
 * <span class="code" style="background-color: #bbb">family name</span>,
 * <span class="code" style="background-color: #bbb">given name</span>,
 * <span class="code" style="background-color: #bbb">email address</span>, and
 * <span class="code" style="background-color: #bbb">organization</span> can
 * be set and retrieved via corresponding methods in the ModelCreator 
 * class.  These methods are documented in more detail below.
 *
 * <!-- leave this next break as-is to work around some doxygen bug -->
 */


#ifndef ModelCreator_h
#define ModelCreator_h


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/common/operationReturnValues.h>
#include <sbml/util/List.h>

#include <sbml/xml/XMLNode.h>


#ifdef __cplusplus

#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ModelCreator
{
public:

  /**
   * Creates a new ModelCreator object.
   */
  ModelCreator ();


  /**
   * Creates a new ModelCreator from an XMLNode.
   *
   * @param creator the XMLNode from which to create the ModelCreator.
   */
  ModelCreator(const XMLNode creator);


  /**
   * Destroys the ModelCreator.
   */
  ~ModelCreator();


  /**
   * Copy constructor; creates a copy of the ModelCreator.
   *
   * @param orig the object to copy.
   */
  ModelCreator(const ModelCreator& orig);


  /**
   * Assignment operator.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  ModelCreator& operator=(const ModelCreator& rhs);


  /**
   * Creates and returns a deep copy of this ModelCreator object.
   *
   * @return the (deep) copy of this ModelCreator object.
   */
  ModelCreator* clone () const;


  /**
   * Returns the "family name" stored in this ModelCreator object.
   *
   * @return the "family name" portion of the ModelCreator object.
   */
  const std::string& getFamilyName()  const  {  return  mFamilyName;  }


  /**
   * Returns the "given name" stored in this ModelCreator object.
   *
   * @return the "given name" portion of the ModelCreator object.
   */
  const std::string& getGivenName() const    {  return  mGivenName;  }


  /**
   * Returns the "email" stored in this ModelCreator object.
   *
   * @return email from the ModelCreator.
   */
  const std::string& getEmail() const       {  return  mEmail;  }


  /**
   * Returns the "organization" stored in this ModelCreator object.
   *
   * @return organization from the ModelCreator.
   */
  const std::string& getOrganization() const{  return  mOrganization;  }


  /**
   * (Alternate spelling) Returns the "organization" stored in this
   * ModelCreator object.
   *
   * @note This function is an alias of getOrganization().
   *
   * @return organization from the ModelCreator.
   *
   * @see getOrganization()
   */
  const std::string& getOrganisation() const{  return  mOrganization;  }

 
  /**
   * Returns the "fn" stored in this ModelCreator object when using vCard4.
   *
   * @return the "fn" portion of the ModelCreator object.
   */
  const std::string& getName()  const  {  return  mFamilyName;  }


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ModelCreator's "family name" part is set.
   *
   * @return @c true if the familyName of this ModelCreator is set, @c false otherwise.
   */
  bool isSetFamilyName();


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ModelCreator's "given name" part is set.
   *
   * @return @c true if the givenName of this ModelCreator is set, @c false otherwise.
   */
  bool isSetGivenName();


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ModelCreator's "email" part is set.
   *
   * @return @c true if the email of this ModelCreator is set, @c false otherwise.
   */
  bool isSetEmail();


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ModelCreator's "organization" part is set.
   *
   * @return @c true if the organization of this ModelCreator is set, @c false otherwise.
   */
  bool isSetOrganization();


  /**
   * (Alternate spelling) Predicate returning @c true or @c false depending
   * on whether this ModelCreator's "organization" part is set.
   *
   * @note This function is an alias of isSetOrganization().
   *
   * @return @c true if the organization of this ModelCreator is set, @c false otherwise.
   *
   * @see isSetOrganization()
   */
  bool isSetOrganisation();


  /**
   * Predicate returning @c true or @c false depending on whether this
   * ModelCreator's "fn" part is set (when using vCard4).
   *
   * @return @c true if the fn of this ModelCreator is set, @c false otherwise.
   */
  bool isSetName();


  /**
   * Sets the "family name" portion of this ModelCreator object.
   *  
   * @param familyName a string representing the familyName of the ModelCreator.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setFamilyName(const std::string& familyName);


  /**
   * Sets the "given name" portion of this ModelCreator object.
   *  
   * @param givenName a string representing the givenName of the ModelCreator.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setGivenName(const std::string& givenName);


  /**
   * Sets the "email" portion of this ModelCreator object.
   *  
   * @param email a string representing the email of the ModelCreator.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setEmail(const std::string& email);


  /**
   * Sets the "organization" portion of this ModelCreator object.
   *  
   * @param organization a string representing the organization of the 
   * ModelCreator. 
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setOrganization(const std::string& organization);


  /**
   * (Alternate spelling) Sets the "organization" portion of this
   * ModelCreator object.
   *
   * @param organization a string representing the organization of the
   * ModelCreator.
   *
   * @note This function is an alias of setOrganization(const std::string& organization).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   *
   * @see setOrganization(std::string organization)
   */
  int setOrganisation(const std::string& organization);


  /**
   * Sets the "fn" portion of this ModelCreator object (when using vCard4).
   *  
   * @param name a string representing the fn of the ModelCreator. 
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setName(const std::string& name);


  /**
   * Unsets the "family name" portion of this ModelCreator object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetFamilyName();


  /**
   * Unsets the "given name" portion of this ModelCreator object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetGivenName();


  /**
   * Unsets the "email" portion of this ModelCreator object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetEmail();


  /**
   * Unsets the "organization" portion of this ModelCreator object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetOrganization();


  /**
   * (Alternate spelling) Unsets the "organization" portion of this ModelCreator object.
   *
   * @note This function is an alias of unsetOrganization().
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @see unsetOrganization()
   */
  int unsetOrganisation();


  /**
   * Unsets the "fn" portion of this ModelCreator object (when using vCard4).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetName();


  /** @cond doxygenLibsbmlInternal */
  XMLNode * getAdditionalRDF();
  /** @endcond */

  /**
   * Predicate returning @c true if all the required elements for this
   * ModelCreator object have been set.
   *
   * The only required elements for a ModelCreator object are the "family
   * name" and "given name".
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  bool hasRequiredAttributes();
  

  /** @cond doxygenLibsbmlInternal */
  
  bool hasBeenModified();

  void resetModifiedFlags();

  bool usingFNVcard4() { return mUsingFNVcard4; };
   
  
  /** @endcond */

protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mFamilyName;
  std::string mGivenName;
  std::string mEmail;
  std::string mOrganization;

  XMLNode * mAdditionalRDF;

  bool mHasBeenModified;
  bool mUsingFNVcard4;

  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new ModelCreator_t structure and returns a pointer to it.
 *
 * @return pointer to newly created ModelCreator_t structure.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
ModelCreator_t *
ModelCreator_create();

/**
 * Creates a new ModelCreator_t structure from an XMLNode_t structure
 * and returns a pointer to it.
 *
 * @return pointer to newly created ModelCreator_t structure.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
ModelCreator_t *
ModelCreator_createFromNode(const XMLNode_t * node);

/**
 * Destroys this ModelCreator_t.
 *
 * @param mc ModelCreator_t structure to be freed.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
void
ModelCreator_free(ModelCreator_t* mc);

/**
 * Creates a deep copy of the given ModelCreator_t structure
 * 
 * @param mc the ModelCreator_t structure to be copied.
 * 
 * @return a (deep) copy of the given ModelCreator_t structure.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
ModelCreator_t *
ModelCreator_clone (const ModelCreator_t* mc);


/**
 * Returns the familyName from the ModelCreator_t.
 * 
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return familyName from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getFamilyName(ModelCreator_t *mc);

/**
 * Returns the givenName from the ModelCreator_t.
 * 
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return givenName from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getGivenName(ModelCreator_t *mc);

/**
 * Returns the email from the ModelCreator_t.
 * 
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return email from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getEmail(ModelCreator_t *mc);

/**
 * Returns the organization from the ModelCreator_t.
 *
 * @note This function is an alias of ModelCreator_getOrganization().
 * 
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return organization from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getOrganisation(ModelCreator_t *mc);

/**
 * Returns the organization from the ModelCreator_t.
 * 
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return organization from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getOrganization(ModelCreator_t *mc);

/**
 * Returns the fn from the ModelCreator_t (when using vCard4).
 * 
 * @param mc the ModelCreator_t structure to be queried
 *
 * @return fn from the ModelCreator_t.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
const char * 
ModelCreator_getName(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's familyName is set.
 *
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return @c true (non-zero) if the familyName of this 
 * ModelCreator_t structure is set, @c false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetFamilyName(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's givenName is set.
 *
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return @c true (non-zero) if the givenName of this 
 * ModelCreator_t structure is set, @c false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetGivenName(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's email is set.
 *
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return @c true (non-zero) if the email of this 
 * ModelCreator_t structure is set, @c false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetEmail(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's organization is set.
 *
 * @note This function is an alias of ModelCretor_isSetOrganization().
 *
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return @c true (non-zero) if the organization of this 
 * ModelCreator_t structure is set, @c false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetOrganisation(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's organization is set.
 *
 * @param mc the ModelCreator_t structure to be queried.
 *
 * @return @c true (non-zero) if the organization of this 
 * ModelCreator_t structure is set, @c false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetOrganization(ModelCreator_t *mc);

/**
 * Predicate indicating whether this
 * ModelCreator_t's fn is set (when using vCard4).
 *
 * @param mc the ModelCreator_t structure to be queried
 *
 * @return true (non-zero) if the fn of this 
 * ModelCreator_t structure is set, false (0) otherwise.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_isSetName(ModelCreator_t *mc);

/**
 * Sets the family name
 *  
 * @param mc the ModelCreator_t structure.
 * @param name a string representing the familyName of the ModelCreator_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setFamilyName(ModelCreator_t *mc, const char * name);

/**
 * Sets the given name
 *  
 * @param mc the ModelCreator_t structure.
 * @param name a string representing the givenName of the ModelCreator_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setGivenName(ModelCreator_t *mc, const char * name);

/**
 * Sets the email
 *  
 * @param mc the ModelCreator_t structure.
 * @param email a string representing the email of the ModelCreator_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setEmail(ModelCreator_t *mc, const char * email);

/**
 * Sets the organization
 *  
 * @param mc the ModelCreator_t structure.
 * @param org a string representing the organisation of the ModelCreator_t.
 *
 * @note This function is an alias of ModelCretor_setOrganization().
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setOrganisation(ModelCreator_t *mc, const char* org);

/**
 * Sets the organization
 *  
 * @param mc the ModelCreator_t structure.
 * @param org a string representing the organisation of the ModelCreator_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setOrganization(ModelCreator_t *mc, const char* org);

/**
 * Sets the name
 *  
 * @param mc the ModelCreator_t structure
 * @param name a string representing the familyName of the ModelCreator_t. 
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_setName(ModelCreator_t *mc, const char * name);

/**
 * Unsets the familyName of this ModelCreator_t.
 *
 * @param mc the ModelCreator_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetFamilyName(ModelCreator_t *mc);

/**
 * Unsets the givenName of this ModelCreator_t.
 *
 * @param mc the ModelCreator_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetGivenName(ModelCreator_t *mc);

/**
 * Unsets the email of this ModelCreator_t.
 *
 * @param mc the ModelCreator_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetEmail(ModelCreator_t *mc);

/**
 * Unsets the organization of this ModelCreator_t.
 *
 * @param mc the ModelCreator_t structure.
 *
 * @note This function is an alias of ModelCreator_unsetOrganization().
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetOrganisation(ModelCreator_t *mc);

/**
 * Unsets the organization of this ModelCreator_t.
 *
 * @param mc the ModelCreator_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetOrganization(ModelCreator_t *mc);

/**
 * Unsets the fn of this ModelCreator_t (when using vCard4).
 *
 * @param mc the ModelCreator_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int 
ModelCreator_unsetName(ModelCreator_t *mc);

/** 
 * Checks if the model creator has all the required attributes.
 *
 * @param mc the ModelCreator_t structure.
 * 
 * @return @c true (1) if this ModelCreator_t has all the required elements,
 * otherwise @c false (0) will be returned. If an invalid ModelHistory_t 
 * was provided LIBSBML_INVALID_OBJECT is returned.
 *
 * @memberof ModelCreator_t
 */
LIBSBML_EXTERN
int
ModelCreator_hasRequiredAttributes(ModelCreator_t *mc);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /** ModelCreator_h **/

