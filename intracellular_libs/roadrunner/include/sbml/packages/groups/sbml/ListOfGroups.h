/**
 * @file ListOfGroups.h
 * @brief Definition of the ListOfGroups class.
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
 * @class ListOfGroups
 * @sbmlbrief{groups} A list of Group objects.
 *
 * The SBML Level&nbsp;3 Groups package provides a means of defining a
 * <em>group of entities</em>: a simple mechanism for indicating that
 * particular components of an SBML model are related in some way.  A group
 * is defined using a Group class object, and all the Group objects in a
 * model are stored in the parent Model object within a ListOfGroups object.
 *
 * A ListOfGroups is optional, but, if present, must contain at least one
 * Group object.  In common with other ListOf___ classes in SBML,
 * ListOfGroups is derived from SBase. It inherits SBase's attributes
 * "metaid" and "sboTerm", as well as the subcomponents for Annotation and
 * Notes, but does not add any new attributes of its own.
 *
 * @copydetails doc_what_is_listof
 */


#ifndef ListOfGroups_H__
#define ListOfGroups_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/groups/common/groupsfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/ListOf.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/sbml/Group.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ListOfGroups : public ListOf
{

public:

  /**
   * Creates a new ListOfGroups using the given SBML Level, Version and
   * &ldquo;groups&rdquo; package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this
   * ListOfGroups.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * ListOfGroups.
   *
   * @param pkgVersion an unsigned int, the SBML Groups Version to assign to
   * this ListOfGroups.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfGroups(unsigned int level = GroupsExtension::getDefaultLevel(),
               unsigned int version = GroupsExtension::getDefaultVersion(),
               unsigned int pkgVersion =
                 GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfGroups using the given GroupsPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param groupsns the GroupsPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfGroups(GroupsPkgNamespaces *groupsns);


  /**
   * Copy constructor for ListOfGroups.
   *
   * @param orig the ListOfGroups instance to copy.
   */
  ListOfGroups(const ListOfGroups& orig);


  /**
   * Assignment operator for ListOfGroups.
   *
   * @param rhs the ListOfGroups object whose values are to be used as the
   * basis of the assignment.
   */
  ListOfGroups& operator=(const ListOfGroups& rhs);


  /**
   * Creates and returns a deep copy of this ListOfGroups object.
   *
   * @return a (deep) copy of this ListOfGroups object.
   */
  virtual ListOfGroups* clone() const;


  /**
   * Destructor for ListOfGroups.
   */
  virtual ~ListOfGroups();


  /**
   * Get a Group from the ListOfGroups.
   *
   * @param n an unsigned int representing the index of the Group to retrieve.
   *
   * @return the nth Group in this ListOfGroups.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  virtual Group* get(unsigned int n);


  /**
   * Get a Group from the ListOfGroups.
   *
   * @param n an unsigned int representing the index of the Group to retrieve.
   *
   * @return the nth Group in this ListOfGroups.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  virtual const Group* get(unsigned int n) const;


  /**
   * Get a Group from the ListOfGroups based on its identifier.
   *
   * @param sid a string representing the identifier of the Group to retrieve.
   *
   * @return the Group in this ListOfGroups with the given id or NULL if no
   * such Group exists.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  virtual Group* get(const std::string& sid);


  /**
   * Get a Group from the ListOfGroups based on its identifier.
   *
   * @param sid a string representing the identifier of the Group to retrieve.
   *
   * @return the Group in this ListOfGroups with the given id or NULL if no
   * such Group exists.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   * @see getNumGroups()
   */
  virtual const Group* get(const std::string& sid) const;


  /**
   * Removes the nth Group from this ListOfGroups and returns a pointer to it.
   *
   * @param n an unsigned int representing the index of the Group to remove.
   *
   * @return a pointer to the nth Group in this ListOfGroups.
   *
   * @note The caller owns the returned object and is responsible for deleting
   * it.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   */
  virtual Group* remove(unsigned int n);


  /**
   * Removes the Group from this ListOfGroups based on its identifier and
   * returns a pointer to it.
   *
   * @param sid a string representing the identifier of the Group to remove.
   *
   * @return the Group in this ListOfGroups based on the identifier or NULL if
   * no such Group exists.
   *
   * @note The caller owns the returned object and is responsible for deleting
   * it.
   *
   * @see addGroup(const Group* g)
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(unsigned int n)
   */
  virtual Group* remove(const std::string& sid);


  /**
   * Adds a copy of the given Group to this ListOfGroups.
   *
   * @param g the Group object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_NAMESPACES_MISMATCH, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  int addGroup(const Group* g);


  /**
   * Get the number of Group objects in this ListOfGroups.
   *
   * @return the number of Group objects in this ListOfGroups.
   *
   * @see createGroup()
   * @see get(const std::string& sid)
   * @see get(unsigned int n)
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  unsigned int getNumGroups() const;


  /**
   * Creates a new Group object, adds it to this ListOfGroups object and
   * returns the Group object created.
   *
   * @return a new Group object instance.
   *
   * @see get(const std::string& sid)
   * @see get(unsigned int n)
   * @see getNumGroups()
   * @see remove(const std::string& sid)
   * @see remove(unsigned int n)
   */
  Group* createGroup();


  /**
   * Returns the XML element name of this ListOfGroups object.
   *
   * For ListOfGroups, the XML element name is always @c "listOfGroups".
   *
   * @return the name of this element, i.e. @c "listOfGroups".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libSBML type code for this ListOfGroups object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LIST_OF, SBMLTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   */
  virtual int getTypeCode() const;


  /**
   * Returns the libSBML type code for the SBML objects contained in this
   * ListOfGroups object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML typecode for the objects contained in this ListOfGroups:
   * @sbmlconstant{SBML_GROUPS_GROUP, SBMLGroupsTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode() const;


protected:


  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new Group in this ListOfGroups
   */
  virtual SBase* createObject(XMLInputStream& stream);

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
 * Get a Group_t from the ListOf_t.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param n an unsigned int representing the index of the Group_t to retrieve.
 *
 * @return the nth Group_t in this ListOf_t.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const Group_t*
ListOfGroups_getGroup(ListOf_t* lo, unsigned int n);


/**
 * Get a Group_t from the ListOf_t based on its identifier.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param sid a string representing the identifier of the Group_t to retrieve.
 *
 * @return the Group_t in this ListOf_t with the given id or NULL if no such
 * Group_t exists.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
const Group_t*
ListOfGroups_getById(ListOf_t* lo, const char *sid);


/**
 * Removes the nth Group_t from this ListOf_t and returns a pointer to it.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param n an unsigned int representing the index of the Group_t to remove.
 *
 * @return a pointer to the nth Group_t in this ListOf_t.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Group_t*
ListOfGroups_remove(ListOf_t* lo, unsigned int n);


/**
 * Removes the Group_t from this ListOf_t based on its identifier and returns a
 * pointer to it.
 *
 * @param lo the ListOf_t structure to search.
 *
 * @param sid a string representing the identifier of the Group_t to remove.
 *
 * @return the Group_t in this ListOf_t based on the identifier or NULL if no
 * such Group_t exists.
 *
 * @memberof Group_t
 */
LIBSBML_EXTERN
Group_t*
ListOfGroups_removeById(ListOf_t* lo, const char* sid);




END_C_DECLS




LIBSBML_CPP_NAMESPACE_END




#endif /* !SWIG */




#endif /* !ListOfGroups_H__ */
