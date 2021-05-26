/**
 * @file Group.h
 * @brief Definition of the Group class.
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
 * @class Group
 * @sbmlbrief{groups} Representation of a group of SBML components.
 *
 * The Group class is the first and most central class in the SBML
 * Level&nbsp;3 Groups package.  This class represents a <em>group of
 * entities</em>: a simple mechanism for indicating that particular
 * components of an SBML model are related in some way.  The nature of the
 * relationship is left up to the modeler, and can be clarified by means of
 * annotations on model components.  Groups may contain either the same or
 * different types of SBML objects, and groups may be nested if desired.
 * There are no predefined modeling or mathematical semantics associated with
 * groups.
 *
 * The Groups class has one required attribute, "kind"; two optional
 * attributes, "id" and "name"; and a single child element, ListOfMembers.
 * The membership of a group is determined by the contents of the list of
 * members stored in Member objects within the ListOfMembers child of a Group
 * object.  Since the Group class of objects is derived from SBase, and SBase
 * provides the ability to attach SBO terms as well as MIRIAM annotations,
 * the semantics of a given group in a model can be made more precise using
 * references to external controlled vocabularies and ontologies.
 *
 * @section group-kinds Group kinds
 *
 * The attribute "kind" on Group is used to indicate the nature of the group
 * defined by a Group instance.  The "kind" attribute must always have
 * one of the following three possible values:
 *
 * @li @c "classification": The group represents a class, and its members
 * have an <em>is-a</em> relationship to the group.  For example, the group
 * could represent a type of molecule such as ATP, and the members could be
 * species located in different compartments, thereby establishing that the
 * species are pools of the same molecule in different locations.
 *
 * @li @c "partonomy": The group represents a collection of parts, and its
 * members have a <em>part-of</em> relationship to the group.  For example, the
 * group could represent a cellular structure, and individual compartments
 * could be made members of the group to indicate they represent subparts of
 * that cellular structure.
 *
 * @li @c "collection": The grouping is merely a collection for convenience,
 * without an implied relationship between the members.  For example, the
 * group could be used to collect together multiple disparate components of a
 * model&mdash;species, reactions, events&mdash;involved in a particular
 * phenotype, and apply a common annotation rather than having to copy the
 * same annotation to each component individually.
 *
 * In the libSBML API for Groups, these possible values for the "kind"
 * attribute are programmatically represented as constants so that callers
 * will not normally need to deal with text string values.  The following
 * are the constants defined for the three values of "kind" (plus an
 * additional constant to represent unrecognized values):
 *
 * @li @sbmlconstant{GROUP_KIND_CLASSIFICATION, GroupKind_t}
 * @li @sbmlconstant{GROUP_KIND_PARTONOMY, GroupKind_t}
 * @li @sbmlconstant{GROUP_KIND_COLLECTION, GroupKind_t}
 * @li @sbmlconstant{GROUP_KIND_UNKNOWN, GroupKind_t}
 *
 * @section group-membership-rules Groups and their members
 *
 * If an SBML element is referenced by a Group's child Member (directly or
 * indirectly), it is considered to be a member of that Group.  If the same
 * element is referenced by multiple Member objects, this is equivalent to
 * including it just once.  (It is considered best practice to avoid this,
 * but does not make for an invalid SBML document.)
 *
 * Children of referenced elements are not considered to be members of the
 * Group: a KineticLaw of a referenced Reaction is not itself a Group
 * member. Even the membership of so-called SBML container classes (e.g.,
 * ListOfSpecies, ListOfCompartments, etc.) does not imply inclusion of
 * children as members of the Group. The sole exception to this rule is the
 * handling of ListOfMembers class, described below.
 *
 * @section groups-semantics Semantics of group memberships
 *
 * @copydetails doc_group_semantics
 *
 * @see Member
 * @see ListOfMembers
 * @see ListOfGroups
 */

/**
 * <!-- ~ ~ ~ ~ ~ Start of common documentation strings ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
 * The following text is used as common documentation blocks copied multiple
 * times elsewhere in this file.  The use of @class is a hack needed because
 * Doxygen's @copydetails command has limited functionality.  Symbols
 * beginning with "doc_" are marked as ignored in our Doxygen configuration.
 * ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  -->
 *
 * @class doc_group_kind
 *
 * @par
 * The attribute "kind" on a Group object is used to indicate the nature of
 * the group defined by a particular Group instance.  In the SBML
 * Level&nbsp;3 Version&nbsp;1 Group specification, there are only three
 * allowable values for "kind":
 * <ul>
 * <li> @c "classification", to indicate that the group represents a class,
 * and its members have an <em>is-a</em> relationship to the group.
 *
 * <li> @c "partonomy", to indicate that the group represents a collection of
 * parts, and its members have a <em>part-of</em> relationship to the group.
 *
 * <li> @c "collection", to indicate that the grouping is merely a collection
 * for convenience, without an implied relationship between the members.
 * </ul>
 */

#ifndef Group_H__
#define Group_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/groups/common/groupsfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/sbml/ListOfMembers.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Group : public SBase
{
protected:

  /** @cond doxygenLibsbmlInternal */

//  std::string mId;
//  std::string mName;
  GroupKind_t mKind;
  ListOfMembers mMembers;

  /** @endcond */

public:

  /**
   * Creates a new Group using the given SBML Level, Version and
   * &ldquo;groups&rdquo; package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this Group.
   *
   * @param version an unsigned int, the SBML Version to assign to this Group.
   *
   * @param pkgVersion an unsigned int, the SBML Groups Version to assign to
   * this Group.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Group(unsigned int level = GroupsExtension::getDefaultLevel(),
        unsigned int version = GroupsExtension::getDefaultVersion(),
        unsigned int pkgVersion = GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new Group using the given GroupsPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param groupsns the GroupsPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Group(GroupsPkgNamespaces *groupsns);


  /**
   * Copy constructor for Group.
   *
   * @param orig the Group instance to copy.
   */
  Group(const Group& orig);


  /**
   * Assignment operator for Group.
   *
   * @param rhs the Group object whose values are to be used as the basis of
   * the assignment.
   */
  Group& operator=(const Group& rhs);


  /**
   * Creates and returns a deep copy of this Group object.
   *
   * @return a (deep) copy of this Group object.
   */
  virtual Group* clone() const;


  /**
   * Destructor for Group.
   */
  virtual ~Group();


  /**
   * Returns the value of the "id" attribute of this Group.
   *
   * @return the value of the "id" attribute of this Group as a string.
   */
  const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this Group.
   *
   * @return the value of the "name" attribute of this Group as a string.
   */
  const std::string& getName() const;


  /**
   * Returns the value of the "kind" attribute of this Group.
   *
   * @copydetails doc_group_kind
   *
   * @return the value of the "kind" attribute of this Group.
   * @if clike The value is drawn from the enumeration
   * @ref GroupKind_t@endif.
   * The possible values returned by this method are:
   * @li @sbmlconstant{GROUP_KIND_CLASSIFICATION, GroupKind_t}
   * @li @sbmlconstant{GROUP_KIND_PARTONOMY, GroupKind_t}
   * @li @sbmlconstant{GROUP_KIND_COLLECTION, GroupKind_t}
   * @li @sbmlconstant{GROUP_KIND_UNKNOWN, GroupKind_t}
   */
  GroupKind_t getKind() const;


  /**
   * Returns the value of the "kind" attribute of this Group.
   *
   * @copydetails doc_group_kind
   *
   * @return the value of the "kind" attribute of this Group as a string.
   * The possible values returned by this method are:
   * @li @c "classification"
   * @li @c "partonomy"
   * @li @c "collection"
   * @li @c "unknown"
   */
  const std::string& getKindAsString() const;


  /**
   * Predicate returning @c true if this Group's "id" attribute is set.
   *
   * @return @c true if this Group's "id" attribute has been set, otherwise @c
   * false is returned.
   */
  bool isSetId() const;


  /**
   * Predicate returning @c true if this Group's "name" attribute is set.
   *
   * @return @c true if this Group's "name" attribute has been set, otherwise
   * @c false is returned.
   */
  bool isSetName() const;


  /**
   * Predicate returning @c true if this Group's "kind" attribute is set.
   *
   * @copydetails doc_group_kind
   *
   * @return @c true if this Group's "kind" attribute has been set, otherwise
   * @c false is returned.
   */
  bool isSetKind() const;


  /**
   * Sets the value of the "id" attribute of this Group.
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
   * Sets the value of the "name" attribute of this Group.
   *
   * @param name std::string& value of the "name" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int setName(const std::string& name);


  /**
   * Sets the value of the "kind" attribute of this Group.
   *
   * @copydetails doc_group_kind
   *
   * @param kind @if clike GroupKind_t@else int@endif value of the 
   * "kind" attribute to be set.
   * The value must be one of the predefined constants that represent
   * valid SBML Level&nbsp;3 Version&nbsp;1 Group "kind" names, which
   * means it must be one of the following values:
   * @li @sbmlconstant{GROUP_KIND_CLASSIFICATION, GroupKind_t}
   * @li @sbmlconstant{GROUP_KIND_PARTONOMY, GroupKind_t}
   * @li @sbmlconstant{GROUP_KIND_COLLECTION, GroupKind_t}
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setKind(const GroupKind_t kind);


  /**
   * Sets the value of the "kind" attribute of this Group.
   *
   * @copydetails doc_group_kind
   *
   * @param kind std::string& of the "kind" attribute to be set.  The value
   * must be one of the following possible strings:
   * @li "classification"
   * @li "partonomy"
   * @li "collection"
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setKind(const std::string& kind);


  /**
   * Unsets the value of the "id" attribute of this Group.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetId();


  /**
   * Unsets the value of the "name" attribute of this Group.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetName();


  /**
   * Unsets the value of the "kind" attribute of this Group.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetKind();


  /**
   * Returns the ListOfMembers from this Group.
   *
   * Using the SBML Level&nbsp;3 Groups package, the membership of a group
   * is determined by the contents of the list of members stored in Member
   * objects within the ListOfMembers child of a Group object.
   *
   * @return the ListOfMembers from this Group.
   */
  const ListOfMembers* getListOfMembers() const;


  /**
   * Returns the ListOfMembers from this Group.
   *
   * Using the SBML Level&nbsp;3 Groups package, the membership of a group
   * is determined by the contents of the list of members stored in Member
   * objects within the ListOfMembers child of a Group object.
   *
   * @return the ListOfMembers from this Group.
   */
  ListOfMembers* getListOfMembers();


  /**
   * Get a Member from the Group.
   *
   * @param n an unsigned int representing the index of the Member to
   * retrieve.
   *
   * @return the nth Member in the ListOfMembers within this Group.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  Member* getMember(unsigned int n);


  /**
   * Get a Member from the Group.
   *
   * @param n an unsigned int representing the index of the Member to
   * retrieve.
   *
   * @return the nth Member in the ListOfMembers within this Group.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  const Member* getMember(unsigned int n) const;


  /**
   * Get a Member from the Group based on its identifier.
   *
   * @param sid a string representing the identifier of the Member to retrieve.
   *
   * @return the Member in the ListOfMembers within this Group with the given
   * id or NULL if no such Member exists.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  Member* getMember(const std::string& sid);


  /**
   * Get a Member from the Group based on its identifier.
   *
   * @param sid a string representing the identifier of the Member to retrieve.
   *
   * @return the Member in the ListOfMembers within this Group with the given
   * id or NULL if no such Member exists.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  const Member* getMember(const std::string& sid) const;


  /**
   * Get a Member from the Group based on the IdRef to which it refers.
   *
   * @param sid a string representing the idRef attribute of the Member object
   * to retrieve.
   *
   * @return the first Member in this Group based on the given idRef attribute
   * or NULL if no such Member exists.
   */
  const Member* getMemberByIdRef(const std::string& sid) const;


  /**
   * Get a Member from the Group based on the IdRef to which it refers.
   *
   * @param sid a string representing the idRef attribute of the Member object
   * to retrieve.
   *
   * @return the first Member in this Group based on the given idRef attribute
   * or NULL if no such Member exists.
   */
  Member* getMemberByIdRef(const std::string& sid);


  /**
   * Adds a copy of the given Member to this Group.
   *
   * @param m the Member object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_NAMESPACES_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  int addMember(const Member* m);


  /**
   * Get the number of Member objects in this Group.
   *
   * @return the number of Member objects in this Group.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getMember(unsigned int n)
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  unsigned int getNumMembers() const;


  /**
   * Creates a new Member object, adds it to this Group object and returns the
   * Member object created.
   *
   * @return a new Member object instance.
   *
   * @see addMember(const Member* m)
   * @see getMember(const std::string& sid)
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   * @see removeMember(unsigned int n)
   */
  Member* createMember();


  /**
   * Removes the nth Member from this Group and returns a pointer to it.
   *
   * @param n an unsigned int representing the index of the Member to remove.
   *
   * @return a pointer to the nth Member in this Group.
   *
   * @note the caller owns the returned object and is responsible for deleting
   * it.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(const std::string& sid)
   */
  Member* removeMember(unsigned int n);


  /**
   * Removes the Member from this Group based on its identifier and returns a
   * pointer to it.
   *
   * @param sid a string representing the identifier of the Member to remove.
   *
   * @return the Member in this Group based on the identifier or NULL if no
   * such Member exists.
   *
   * @note the caller owns the returned object and is responsible for deleting
   * it.
   *
   * @see addMember(const Member* m)
   * @see createMember()
   * @see getMember(const std::string& sid)
   * @see getMember(unsigned int n)
   * @see getNumMembers()
   * @see removeMember(unsigned int n)
   */
  Member* removeMember(const std::string& sid);


  /**
   * Returns the XML element name of this Group object.
   *
   * For Group, the XML element name is always @c "group".
   *
   * @return the name of this element, i.e. @c "group".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libSBML type code for this Group object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_GROUPS_GROUP, SBMLGroupsTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode() const;


  /**
   * Predicate returning @c true if all the required attributes for this Group
   * object have been set.
   *
   * @return @c true to indicate that all the required attributes of this Group
   * have been set, otherwise @c false is returned.
   *
   *
   * @note The required attributes for the Group object are:
   * @li "kind"
   */
  virtual bool hasRequiredAttributes() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write any contained elements
   */
  virtual void writeElements(XMLOutputStream& stream) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor
   */
  virtual bool accept(SBMLVisitor& v) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument
   */
  virtual void setSBMLDocument(SBMLDocument* d);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Connects to child elements
   */
  virtual void connectToChild();

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/disables the given package with this element
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix,
                                     bool flag);

  /** @endcond */


  /**
   * Returns the first child element that has the given @p id in the model-wide
   * SId namespace, or @c NULL if no such object is found.
   *
   * @param id a string representing the id attribute of the object to
   * retrieve.
   *
   * @return a pointer to the SBase element with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);


  /**
   * Returns the first child element that has the given @p metaid, or @c NULL
   * if no such object is found.
   *
   * @param metaid a string representing the metaid attribute of the object to
   * retrieve.
   *
   * @return a pointer to the SBase element with the given @p metaid.
   */
  virtual SBase* getElementByMetaId(const std::string& metaid);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * filter, an ElementFilter that may impose restrictions on the objects to be
   * retrieved.
   *
   * @return a List* pointer of pointers to all SBase child objects with any
   * restriction imposed.
   */
  virtual List* getAllElements(ElementFilter * filter = NULL);


protected:


  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new object from the next XMLToken on the XMLInputStream
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


};



LIBSBML_CPP_NAMESPACE_END




#endif /* __cplusplus */




#ifndef SWIG




LIBSBML_CPP_NAMESPACE_BEGIN




BEGIN_C_DECLS


/**
 * Creates a new Group_t using the given SBML Level, Version and
 * &ldquo;groups&rdquo; package version.
 *
 * @param level an unsigned int, the SBML Level to assign to this Group_t.
 *
 * @param version an unsigned int, the SBML Version to assign to this Group_t.
 *
 * @param pkgVersion an unsigned int, the SBML Groups Version to assign to this
 * Group_t.
 *
 * @copydetails doc_note_setting_lv_pkg
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Group_t *
Group_create(unsigned int level = GroupsExtension::getDefaultLevel(),
             unsigned int version = GroupsExtension::getDefaultVersion(),
             unsigned int pkgVersion =
               GroupsExtension::getDefaultPackageVersion());


/**
 * Creates and returns a deep copy of this Group_t object.
 *
 * @param g the Group_t structure.
 *
 * @return a (deep) copy of this Group_t object.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Group_t*
Group_clone(const Group_t* g);


/**
 * Frees this Group_t object.
 *
 * @param g the Group_t structure.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
void
Group_free(Group_t* g);


/**
 * Returns the value of the "id" attribute of this Group_t.
 *
 * @param g the Group_t structure whose id is sought.
 *
 * @return the value of the "id" attribute of this Group_t as a pointer to a
 * string.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const char *
Group_getId(const Group_t * g);


/**
 * Returns the value of the "name" attribute of this Group_t.
 *
 * @param g the Group_t structure whose name is sought.
 *
 * @return the value of the "name" attribute of this Group_t as a pointer to a
 * string.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const char *
Group_getName(const Group_t * g);


/**
 * Returns the value of the "kind" attribute of this Group_t.
 *
 * @param g the Group_t structure whose kind is sought.
 *
 * @return the value of the "kind" attribute of this Group_t as a GroupKind_t.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
GroupKind_t
Group_getKind(const Group_t * g);


/**
 * Returns the value of the "kind" attribute of this Group_t.
 *
 * @param g the Group_t structure whose kind is sought.
 *
 * @return the value of the "kind" attribute of this Group_t as a const char *.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const char *
Group_getKindAsString(const Group_t * g);


/**
 * Predicate returning @c 1 if this Group_t's "id" attribute is set.
 *
 * @param g the Group_t structure.
 *
 * @return @c 1 if this Group_t's "id" attribute has been set, otherwise @c 0
 * is returned.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_isSetId(const Group_t * g);


/**
 * Predicate returning @c 1 if this Group_t's "name" attribute is set.
 *
 * @param g the Group_t structure.
 *
 * @return @c 1 if this Group_t's "name" attribute has been set, otherwise @c 0
 * is returned.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_isSetName(const Group_t * g);


/**
 * Predicate returning @c 1 if this Group_t's "kind" attribute is set.
 *
 * @param g the Group_t structure.
 *
 * @return @c 1 if this Group_t's "kind" attribute has been set, otherwise @c 0
 * is returned.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_isSetKind(const Group_t * g);


/**
 * Sets the value of the "id" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @param id const char * value of the "id" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_setId(Group_t * g, const char * id);


/**
 * Sets the value of the "name" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @param name const char * value of the "name" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_setName(Group_t * g, const char * name);


/**
 * Sets the value of the "kind" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @param kind GroupKind_t value of the "kind" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_setKind(Group_t * g, GroupKind_t kind);


/**
 * Sets the value of the "kind" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @param kind const char * of the "kind" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_setKindAsString(Group_t * g, const char * kind);


/**
 * Unsets the value of the "id" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_unsetId(Group_t * g);


/**
 * Unsets the value of the "name" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_unsetName(Group_t * g);


/**
 * Unsets the value of the "kind" attribute of this Group_t.
 *
 * @param g the Group_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_unsetKind(Group_t * g);


/**
 * Returns a ListOf_t* containing Member_t objects from this Group_t.
 *
 * @param g the Group_t structure whose "ListOfMembers" is sought.
 *
 * @return the "ListOfMembers" from this Group_t as a ListOf_t *.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
ListOf_t*
Group_getListOfMembers(Group_t* g);


/**
 * Get a Member_t from the Group_t.
 *
 * @param g the Group_t structure to search.
 *
 * @param n an unsigned int representing the index of the Member_t to
 * retrieve.
 *
 * @return the nth Member_t in the ListOfMembers within this Group.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const Member_t*
Group_getMember(Group_t* g, unsigned int n);


/**
 * Get a Member_t from the Group_t based on its identifier.
 *
 * @param g the Group_t structure to search.
 *
 * @param sid a string representing the identifier of the Member_t to retrieve.
 *
 * @return the Member_t in the ListOfMembers within this Group with the given
 * id or NULL if no such Member_t exists.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const Member_t*
Group_getMemberById(Group_t* g, const char *sid);


/**
 * Get a Member_t from the Group_t based on the IdRef to which it refers.
 *
 * @param g the Group_t structure to search.
 *
 * @param sid a string representing the idRef attribute of the Member_t object
 * to retrieve.
 *
 * @return the first Member_t in this Group_t based on the given idRef
 * attribute or NULL if no such Member_t exists.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const Member_t*
Group_getMemberByIdRef(Group_t* g, const char *sid);


/**
 * Adds a copy of the given Member_t to this Group_t.
 *
 * @param g the Group_t structure to which the Member_t should be added.
 *
 * @param m the Member_t object to add.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_addMember(Group_t* g, const Member_t* m);


/**
 * Get the number of Member_t objects in this Group_t.
 *
 * @param g the Group_t structure to query.
 *
 * @return the number of Member_t objects in this Group_t.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
unsigned int
Group_getNumMembers(Group_t* g);


/**
 * Creates a new Member_t object, adds it to this Group_t object and returns
 * the Member_t object created.
 *
 * @param g the Group_t structure to which the Member_t should be added.
 *
 * @return a new Member_t object instance.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Member_t*
Group_createMember(Group_t* g);


/**
 * Removes the nth Member_t from this Group_t and returns a pointer to it.
 *
 * @param g the Group_t structure to search.
 *
 * @param n an unsigned int representing the index of the Member_t to remove.
 *
 * @return a pointer to the nth Member_t in this Group_t.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Member_t*
Group_removeMember(Group_t* g, unsigned int n);


/**
 * Removes the Member_t from this Group_t based on its identifier and returns a
 * pointer to it.
 *
 * @param g the Group_t structure to search.
 *
 * @param sid a string representing the identifier of the Member_t to remove.
 *
 * @return the Member_t in this Group_t based on the identifier or NULL if no
 * such Member_t exists.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Member_t*
Group_removeMemberById(Group_t* g, const char* sid);


/**
 * Predicate returning @c 1 if all the required attributes for this Group_t
 * object have been set.
 *
 * @param g the Group_t structure.
 *
 * @return @c 1 to indicate that all the required attributes of this Group_t
 * have been set, otherwise @c 0 is returned.
 *
 *
 * @note The required attributes for the Group_t object are:
 * @li "kind"
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_hasRequiredAttributes(const Group_t * g);


/**
 * Predicate returning @c 1 if all the required elements for this Group_t
 * object have been set.
 *
 * @param g the Group_t structure.
 *
 * @return @c 1 to indicate that all the required elements of this Group_t have
 * been set, otherwise @c 0 is returned.
 *
 *
 * @note The required elements for the Group_t object are:
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
int
Group_hasRequiredElements(const Group_t * g);




END_C_DECLS




LIBSBML_CPP_NAMESPACE_END




#endif /* !SWIG */




#endif /* !Group_H__ */


