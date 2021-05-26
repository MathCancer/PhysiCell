/**
 * @file ListOfMembers.h
 * @brief Definition of the ListOfMembers class.
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class ListOfMembers
 * @sbmlbrief{groups} A list of Member objects.
 *
 * In the SBML Level&nbsp;3 Groups package, the membership of a group is
 * defined by placing Member objects within a ListOfMembers object contained
 * within a Group object.  A ListOfMembers object is optional, but, if
 * present, it must contain at least one Member object.  In common with other
 * ListOf___ classes in SBML, ListOfMembers is derived from SBase.  However,
 * an uncommon feature of ListOfMembers is that it has optional "id" and
 * "name" attributes that can be used in a manner discussed below.
 *
 * A ListOfMembers must have one or more Member children. Since ListOfMembers
 * is derived from SBase, it inherits the "sboTerm" and "metaid" attributes,
 * as well as the optional children Notes and Annotation. Unlike most lists
 * of objects in SBML, however, the "sboTerm" attribute and the Notes and
 * Annotation children are taken here to apply directly to every SBML element
 * referenced by each child Member of this ListOfMembers, if that referenced
 * element has no such definition. Thus, if a referenced element has no
 * defined "sboTerm" attribute or child Notes or Annotation objects, that
 * element should be considered to now have the "sboTerm", child Notes, or
 * child Annotation of the ListOfMembers.
 *
 * If multiple ListOfMembers have child Member elements that reference
 * the same SBML element, and more than one ListOfMembers or Member has
 * a value for an sboTerm attribute, Notes, or Annotation element, those
 * Member elements should be consistent with each other: the "sboTerm"
 * attributes should either be identical, or one should inherit from
 * the other; Notes should say the same or similar things; and Annotation
 * elements should not conflict. Interpreters may choose to resolve any
 * such conflicts arbitrarily.
 *
 * An uncommon feature about ListOfMembers is that, if it is referenced by a
 * Member of a different Group, the @em children of the referenced
 * ListOfMembers are also considered to be members of the referencing group.
 * In this way, groups may be nested semantically to create larger groups out
 * of subgroups.
 *
 * @section listofmembers-groups-semantics Semantics of group memberships
 *
 * @copydetails doc_group_semantics
 *
 * @see Group
 * @see Member
 * @see ListOfGroups
 */


#ifndef ListOfMembers_H__
#define ListOfMembers_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/groups/common/groupsfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/ListOf.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/sbml/Member.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ListOfMembers : public ListOf
{
protected:

  /** @cond doxygenLibsbmlInternal */

//  std::string mId;
//  std::string mName;

  /** @endcond */

public:

  /**
   * Creates a new ListOfMembers using the given SBML Level, Version and
   * &ldquo;groups&rdquo; package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * ListOfMembers.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * ListOfMembers.
   *
   * @param pkgVersion an unsigned int, the SBML Groups Version to assign to
   * this ListOfMembers.
   *
   * @copydetails doc_note_setting_lv
   */
  ListOfMembers(unsigned int level = GroupsExtension::getDefaultLevel(),
                unsigned int version = GroupsExtension::getDefaultVersion(),
                unsigned int pkgVersion =
                  GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfMembers using the given GroupsPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param groupsns the GroupsPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv
   */
  ListOfMembers(GroupsPkgNamespaces *groupsns);


  /**
   * Copy constructor for ListOfMembers.
   *
   * @param orig the ListOfMembers instance to copy.
   */
  ListOfMembers(const ListOfMembers& orig);


  /**
   * Assignment operator for ListOfMembers.
   *
   * @param rhs the ListOfMembers object whose values are to be used as the
   * basis of the assignment.
   */
  ListOfMembers& operator=(const ListOfMembers& rhs);


  /**
   * Creates and returns a deep copy of this ListOfMembers object.
   *
   * @return a (deep) copy of this ListOfMembers object.
   */
  virtual ListOfMembers* clone() const;


  /**
   * Destructor for ListOfMembers.
   */
  virtual ~ListOfMembers();


  /**
   * Returns the value of the "id" attribute of this ListOfMembers.
   *
   * @return the value of the "id" attribute of this ListOfMembers as a string.
   */
  const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this ListOfMembers.
   *
   * @return the value of the "name" attribute of this ListOfMembers as a
   * string.
   */
  const std::string& getName() const;


  /**
   * Predicate returning @c true if this ListOfMembers's "id" attribute is set.
   *
   * @return @c true if this ListOfMembers's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  bool isSetId() const;


  /**
   * Predicate returning @c true if this ListOfMembers's "name" attribute is
   * set.
   *
   * @return @c true if this ListOfMembers's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  bool isSetName() const;


  /**
   * Sets the value of the "id" attribute of this ListOfMembers.
   *
   * @param id std::string& value of the "id" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE,
   * OperationReturnValues_t}
   */
  int setId(const std::string& id);


  /**
   * Sets the value of the "name" attribute of this ListOfMembers.
   *
   * @param name std::string& value of the "name" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE,
   * OperationReturnValues_t}
   */
  int setName(const std::string& name);


  /**
   * Unsets the value of the "id" attribute of this ListOfMembers.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetId();


  /**
   * Unsets the value of the "name" attribute of this ListOfMembers.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetName();


  /**
   * Get a Member from the ListOfMembers.
   *
   * @param n an unsigned int representing the index of the Member to
   * retrieve.
   *
   * @return the nth Member in this ListOfMembers.
   *
   * @see size()
   */
  virtual Member* get(unsigned int n);


  /**
   * Get a Member from the ListOfMembers.
   *
   * @param n an unsigned int representing the index of the Member to
   * retrieve.
   *
   * @return the nth Member in this ListOfMembers.
   *
   * @see size()
   */
  virtual const Member* get(unsigned int n) const;


  /**
   * Get a Member from the ListOfMembers based on its identifier.
   *
   * @param sid a string representing the identifier of the Member to retrieve.
   *
   * @return the Member in this ListOfMembers with the given id or NULL if no
   * such Member exists.
   *
   * @see size()
   */
  virtual Member* get(const std::string& sid);


  /**
   * Get a Member from the ListOfMembers based on its identifier.
   *
   * @param sid a string representing the identifier of the Member to retrieve.
   *
   * @return the Member in this ListOfMembers with the given id or NULL if no
   * such Member exists.
   *
   * @see size()
   */
  virtual const Member* get(const std::string& sid) const;


  /**
   * Removes the nth Member from this ListOfMembers and returns a pointer to
   * it.
   *
   * @param n an unsigned int representing the index of the Member to remove.
   *
   * @return a pointer to the nth Member in this ListOfMembers.
   *
   * @see size()
   *
   * @note the caller owns the returned object and is responsible for deleting
   * it.
   */
  virtual Member* remove(unsigned int n);


  /**
   * Removes the Member from this ListOfMembers based on its identifier and
   * returns a pointer to it.
   *
   * @param sid a string representing the identifier of the Member to remove.
   *
   * @return the Member in this ListOfMembers based on the identifier or NULL
   * if no such Member exists.
   *
   * @note the caller owns the returned object and is responsible for deleting
   * it.
   */
  virtual Member* remove(const std::string& sid);


  /**
   * Adds a copy of the given Member to this ListOfMembers.
   *
   * @param m the Member object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @see createMember()
   */
  int addMember(const Member* m);


  /**
   * Get the number of Member objects in this ListOfMembers.
   *
   * @return the number of Member objects in this ListOfMembers.
   */
  unsigned int getNumMembers() const;


  /**
   * Creates a new Member object, adds it to this ListOfMembers object and
   * returns the Member object created.
   *
   * @return a new Member object instance.
   *
   * @see addMember(const Member* m)
   */
  Member* createMember();


  /**
   * Get a Member from the ListOfMembers based on the IdRef to which it refers.
   *
   * @param sid a string representing the idRef attribute of the Member object
   * to retrieve.
   *
   * @return the first Member in this ListOfMembers based on the given idRef
   * attribute or NULL if no such Member exists.
   */
  const Member* getByIdRef(const std::string& sid) const;


  /**
   * Get a Member from the ListOfMembers based on the IdRef to which it refers.
   *
   * @param sid a string representing the idRef attribute of the Member object
   * to retrieve.
   *
   * @return the first Member in this ListOfMembers based on the given idRef
   * attribute or NULL if no such Member exists.
   */
  Member* getByIdRef(const std::string& sid);


  /**
   * Returns the XML element name of this ListOfMembers object.
   *
   * For ListOfMembers, the XML element name is always @c "listOfMembers".
   *
   * @return the name of this element, i.e. @c "listOfMembers".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libSBML type code for this ListOfMembers object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   *
   * @sbmlconstant{SBML_LIST_OF, SBMLTypeCode_t}
   *
   * @copydetails doc_warning_typecodes_not_unique
   */
  virtual int getTypeCode() const;


  /**
   * Returns the libSBML type code for the SBML objects contained in this
   * ListOfMembers object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML typecode for the objects contained in this ListOfMembers:
   *
   * @sbmlconstant{SBML_GROUPS_MEMBER, SBMLGroupsTypeCode_t}
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode() const;


  /**
   * Predicate returning @c true if all the required attributes for this
   * ListOfMembers object have been set.
   *
   * @return @c true to indicate that all the required attributes of this
   * ListOfMembers have been set, otherwise @c false is returned.
   *
   *
   * @note The required attributes for the ListOfMembers object are:
   */
  virtual bool hasRequiredAttributes() const;


protected:


  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new Member in this ListOfMembers
   */
  virtual SBase* createObject(XMLInputStream& stream);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Adds the expected attributes for this element
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Reads the expected attributes into the member data variables
   */
  virtual void readAttributes(const XMLAttributes& attributes,
                              const ExpectedAttributes& expectedAttributes);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Writes the attributes to the stream
   */
  virtual void writeAttributes(XMLOutputStream& stream) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Writes the namespace for the Groups package
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;

  /** @endcond */


};



LIBSBML_CPP_NAMESPACE_END




#endif /* __cplusplus */




#ifndef SWIG




LIBSBML_CPP_NAMESPACE_BEGIN




BEGIN_C_DECLS


/**
 * Returns the value of the "id" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure whose id is sought.
 *
 * @return the value of the "id" attribute of this ListOf_t as a pointer to a
 * string.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
const char *
ListOfMembers_getId(const ListOf_t * lo);


/**
 * Returns the value of the "name" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure whose name is sought.
 *
 * @return the value of the "name" attribute of this ListOf_t as a pointer to a
 * string.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
const char *
ListOfMembers_getName(const ListOf_t * lo);


/**
 * Predicate returning @c 1 if this ListOf_t's "id" attribute is set.
 *
 * @param lo the ListOf_t structure.
 *
 * @return @c 1 if this ListOf_t's "id" attribute has been set, otherwise @c 0
 * is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_isSetId(const ListOf_t * lo);


/**
 * Predicate returning @c 1 if this ListOf_t's "name" attribute is set.
 *
 * @param lo the ListOf_t structure.
 *
 * @return @c 1 if this ListOf_t's "name" attribute has been set, otherwise @c
 * 0 is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_isSetName(const ListOf_t * lo);


/**
 * Sets the value of the "id" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure.
 *
 * @param id const char * value of the "id" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_setId(ListOf_t * lo, const char * id);


/**
 * Sets the value of the "name" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure.
 *
 * @param name const char * value of the "name" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_setName(ListOf_t * lo, const char * name);


/**
 * Unsets the value of the "id" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_unsetId(ListOf_t * lo);


/**
 * Unsets the value of the "name" attribute of this ListOf_t.
 *
 * @param lo the ListOf_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
ListOfMembers_unsetName(ListOf_t * lo);


/**
 * Get a Member_t from the ListOf_t.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param n an unsigned int representing the index of the Member_t to
 * retrieve.
 *
 * @return the nth Member_t in this ListOf_t.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
const Member_t*
ListOfMembers_getMember(ListOf_t* lo, unsigned int n);


/**
 * Get a Member_t from the ListOf_t based on its identifier.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param sid a string representing the identifier of the Member_t to retrieve.
 *
 * @return the Member_t in this ListOf_t with the given id or NULL if no such
 * Member_t exists.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
const Member_t*
ListOfMembers_getById(ListOf_t* lo, const char *sid);


/**
 * Removes the nth Member_t from this ListOf_t and returns a pointer to it.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param n an unsigned int representing the index of the Member_t to remove.
 *
 * @return a pointer to the nth Member_t in this ListOf_t.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
Member_t*
ListOfMembers_remove(ListOf_t* lo, unsigned int n);


/**
 * Removes the Member_t from this ListOf_t based on its identifier and returns
 * a pointer to it.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param sid a string representing the identifier of the Member_t to remove.
 *
 * @return the Member_t in this ListOf_t based on the identifier or NULL if no
 * such Member_t exists.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
Member_t*
ListOfMembers_removeById(ListOf_t* lo, const char* sid);




END_C_DECLS




LIBSBML_CPP_NAMESPACE_END




#endif /* !SWIG */




#endif /* !ListOfMembers_H__ */
