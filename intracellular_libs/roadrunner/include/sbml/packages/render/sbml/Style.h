/**
 * @file    Style.h
 * @brief   abstract base class for style objects
 * @author  Ralph Gauges
 * @author  Frank T. Bergmann
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
 * Copyright (C) 2011-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright 2010 Ralph Gauges
 *     Group for the modeling of biological processes 
 *     University of Heidelberg
 *     Im Neuenheimer Feld 267
 *     69120 Heidelberg
 *     Germany
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class Style
 * @brief abstract base class for local and global styles
 *
 * Local and global styles in the SBML render extension have many attributes and methods in common.
 * These have been implemented in the abstract base class Style.
 *
 * A style is a graphical representation for certain layout objects. The assignment of styles to
 * individual layout objects can either be done through layout object ids (local styles only), 
 * layout object types (SPECIES, COMPARTMENT, etc.) or layout object roles.
 */

#ifndef Style_H__
#define Style_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/SBase.h>
#include <sbml/packages/render/sbml/RenderGroup.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <set>
#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN Style : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
////  std::string mId; 
////  std::string mName;
  std::set<std::string> mRoleList;
  std::set<std::string> mTypeList;
  RenderGroup mGroup;
  /** @endcond */

protected:
  /**
   * Creates a new Style object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  Style (unsigned int level      = RenderExtension::getDefaultLevel(),
    unsigned int version    = RenderExtension::getDefaultVersion(),
    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new Style object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  Style (RenderPkgNamespaces* renderns);


  /**
   * Creates a new Style object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * Style object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the Style
   * object to be instantiated.
   */
  Style(const XMLNode& node, unsigned int l2version);


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a Style with an empty group
   * and empty role and type list.
   * The group has to be filled before the * object is valid.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  Style(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

public:

  /**
   * Destroy this Style object.
   */
  virtual ~Style ();


  /**
   * Sets the group of the Style.
   *
   * @param group New group to be set on the style.
   * The group is copied.
   */
  void setGroup(const RenderGroup* group);

  /**
   * Returns a const pointer to the group of the Style.
   *
   * @return const pointer to the group.
   */
  const RenderGroup* getGroup() const;

  /**
   * Returns a pointer to the group of the Style.
   *
   * @return pointer to the group.
   */
  RenderGroup* getGroup();

  /**
   * Returns the number of ids in the role list.
   *
   * @return the number of roles in the role list. 
   */
  unsigned int getNumRoles() const;

  /** 
   * @return the string of all roles
   */
  std::string createRoleString() const;

  /** 
   * @return the string of all types
   */
  std::string createTypeString() const;

  /**
   * Adds an id to the role list.
   *
   * @param role New role to be added to the role list.
   */
  void addRole(const std::string& role);

  /**
   * Checks whether a given role is in the role list.
   *
   * @param role role string to check for in the role list.
   */
  bool isInRoleList(const std::string& role) const;

  /**
   * Removes the given role from the role list.
   *
   * @param role role string to be removed from the role list.
   */
  void removeRole(const std::string& role);

  /**
   * Sets the complete role list to a copy of the given list.
   *
   * @param roleList New list of role strings to be set on the style.
   */
  void setRoleList(const std::set<std::string>& roleList);

  /**
   * Returns a const reference to the role list.
   *
   * @return reference to the role list.
   */
  std::set<std::string>& getRoleList();

  /**
   * Returns the type list.
   *
   * @return reference to the type list.
   */
  std::set<std::string>& getTypeList();

  /**
   * Returns a const reference to the role list.
   *
   * @return const reference to the role list.
   */
  const std::set<std::string>& getRoleList() const;

  /**
   * Returns the type list.
   *
   * @return const reference to the type list.
   */
  const std::set<std::string>& getTypeList() const;

  /**
   * Sets the complete type list to a copy of the given list.
   *
   * @param typeList the list of types to be set for the style.
   */
  void setTypeList(const std::set<std::string>& typeList);

  /**
   * Returns the number of types in the type list.
   *
   * @return number of types in type list.
   */
  unsigned int getNumTypes() const;

  /**
   * Adds a type string to the type list.
   *
   * @param type new type string to be added to the type list
   */
  void addType(const std::string& type);

  /**
   * Checks whether a given type string is in the type list.
   *
   * @param type string to be searched for in the type list
   *
   * @return true or false depending on whether the given string was
   * found in the type list.
   */
  bool isInTypeList(const std::string& type) const;

  /**
   * Removes a type string from the type list.
   *
   * @param type type string to be removed from the type list.
   */
  void removeType(const std::string& type);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @return a List* of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);


  /**
   * Accepts the given SBMLVisitor for this instance of Style.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;

  /**
   * Creates and returns a deep copy of this Style object.
   * This method is purely abstract and has to be implemented in derived classes.
   * 
   * @return a (deep) copy of this Style object
   */
  virtual Style* clone () const = 0;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   * 
   * NOTE: this function is only ever going to be called from the constructor
   */
  virtual const std::string& getElementName () const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */

  /**
   * Creates an XMLNode object from this Style object.
   *
   * @return the XMLNode with the XML representation for the 
   * Style object.
   */
  XMLNode toXML() const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use
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

  /**
   * Returns the value of the "id" attribute of this Style.
   *
   * @return the id of the Style
   */
  const std::string& getId () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Style's "id" attribute has been set.
   *
   * @return returns true or false depending on whether the id on the 
   * Style has been set.
   */
  bool isSetId () const;


  /**
   * Sets the value of the "id" attribute of this Style.
   *
   * @param id the new id for the Style 
   *
   * @return status if the operation succeeded
   */
  int setId (const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this Style.
   */
  virtual int unsetId ();

  /**
   * Returns the value of the "name" attribute of this Style.
   *
   * @return the name of the Style
   */
  const std::string& getName () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Style's "name" attribute has been set.
   *
   * @return returns true or false depending on whether the name on the 
   * Style has been set.
   */
  bool isSetName () const;


  /**
   * Sets the value of the "name" attribute of this Style.
   *
   * @param name the new name for the Style 
   *
   * @return status if the operation succeeded
   */
  int setName (const std::string& id);


  /**
   * Unsets the value of the "name" attribute of this Style.
   */
  virtual int unsetName ();

  /**
   * Sets the parent SBML object of this SBML object.
   *
   * @param sb the SBML object to use
   */
  virtual void setParentSBMLObject (SBase* sb);

  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * attributes
   */
  virtual bool hasRequiredAttributes() const ;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * elements
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond */


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes);
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
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Parse the list of roles into the role list.
   */
  void readListOfRoles(const XMLAttributes& attr);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Parse the list of types into the type list.
   */
  void readListOfTypes(const XMLAttributes& attr);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds the typeList attribute to an XMLAttributes object.
   */
  void addListOfRoles(XMLAttributes& attr) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Adds the roleList attribute to an XMLAttributes object.
   */
  void addListOfTypes(XMLAttributes& attr) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Writes the type list to an XML stream.
   */
  void writeTypeList(XMLOutputStream& stream) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Writes the role list to an XML stream.
   */
  void writeRolesList(XMLOutputStream& stream) const;
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Devides a string into individual elements and stores them in the given set.
   */
  static void readIntoSet(const std::string& s,std::set<std::string>& set);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  /**
   * Concatenates individual elements from a set to a string.
   * The string is returned.
   */
  static std::string createStringFromSet(const std::set<std::string>& set);
  /** @endcond */

};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* Style_H__ */
