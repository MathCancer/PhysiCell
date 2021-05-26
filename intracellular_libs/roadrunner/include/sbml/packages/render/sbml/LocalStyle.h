/**
 * @file    LocalStyle.h
 * @brief   class representing a local style object
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
 * @class LocalStyle
 * @brief implementation of the LocalStyle concept of the SBML render extension.
 *
 * Local styles are the style information objects used in LocalRenderInformation (@see LocalRenderInformation).
 * Local styles can be associated with layout objects by role and type as well as by 
 * id of layout objects from the layout the local style belongs to.
 *
 * Since LocalStyle is derived from Styles, it inherits all of the methods and attributes from Style. (@see Style) 
 *
 * @class ListOfLocalStyles
 * @brief ListOfLocalStyles is the container class that stores LocalStyles in LocalRenderInformation objects.
 *
 * Each LocalRenderInformation object contains a ListOfLocalStyles which contains zero or
 * more local style objects. 
 */

#ifndef LocalStyle_H__
#define LocalStyle_H__

#include <sbml/common/sbmlfwd.h>

#include <sbml/packages/render/sbml/Style.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus

#include <set>
#include <string>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN LocalStyle : public Style
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::set<std::string> mIdList;
  static const std::string ELEMENT_NAME;
  /** @endcond */

public:
  /**
   * Creates a new LocalStyle object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  LocalStyle (unsigned int level      = RenderExtension::getDefaultLevel(),
              unsigned int version    = RenderExtension::getDefaultVersion(),
              unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new LocalStyle object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  LocalStyle (RenderPkgNamespaces* renderns);

  /**
   * Creates a new LocalStyle object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * LocalStyle object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the LocalStyle
   * object to be instantiated.
   */
  LocalStyle(const XMLNode& node, unsigned int l2version=4);


  /**
   * Destroy this LocalStyle object.
   */
  virtual ~LocalStyle ();


#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a LocalStyle with an empty group
   * and empty id, role and type list.
   * The group has to be filled before the
   * object is valid.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  LocalStyle(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

  /**
   * Returns the number of ids in the id set.
   *
   * @return the number of ids in the id set
   */
  unsigned int getNumIds() const;

  /**
   * Returns the id list.
   *
   * @return the reference to the list of ids for the local style.
   */
  std::set<std::string>& getIdList();

  /**
   * Returns the id list.
   *
   * @return the const reference to the list of ids for the local style.
   */
  const std::set<std::string>& getIdList() const;

  /**
   * Sets the id list.
   *
   * @param idList The list of ids to be set on the local style.
   */
  void setIdList(const std::set<std::string>& idList);

  /**
   * Adds another id to the set.
   *
   * @param id the id string to be added to the id list.
   */
  void addId(const std::string& id);


  /** 
   * @return the string of all roles
   */
  std::string createIdString() const;

  /**
   * Checks whether a given @p id is in the id list.
   *
   * @param id the id to be searched for
   *
   * @return true or false depending on whether the given @p id is in the id list or not.
   */
  bool isInIdList(const std::string& id) const;

  /**
   * Removes an id from the set.
   *
   * @param the id to be removed from the id list.
   */
  void removeId(const std::string& id);

  /**
   * Returns the libSBML type code for this %SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.
   * The set of possible type codes is defined in the enumeration
   * #SBMLTypeCode_t.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if java LibSBML attaches an
   * identifying code to every kind of SBML object.  These are known as
   * <em>SBML type codes</em>.  In other languages, the set of type codes
   * is stored in an enumeration; in the Java language interface for
   * libSBML, the type codes are defined as static integer constants in
   * interface class {@link libsbmlConstants}.  The names of the type codes
   * all begin with the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or @c SBML_UNKNOWN (default).
   *
   * @see getElementName()
   */
  int getTypeCode() const; 

  /**
   * Creates and returns a deep copy of this LocalStyle object.
   * 
   * @return a (deep) copy of this LocalStyle object
   */
  LocalStyle* clone() const;

  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * SBML component.  For example, Model defines it as returning "model",
   * CompartmentType defines it as returning "compartmentType", etc.
   */
  const std::string& getElementName() const;

  /**
   * Creates an XMLNode object from this LocalStyle object.
   *
   * @return the XMLNode with the XML representation for the 
   * LocalStyle object.
   */
  XMLNode toXML() const;
protected:
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
   * This method adds the attribute for the list of ids to
   * the given XMLnode.
   *
   * @param node the node where the attribute needs to be added
   */
  void addListOfIds(XMLToken& node) const;
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
   * Writes the id list to an XML stream.
   */
  void writeIdList(XMLOutputStream& stream) const;
  /** @endcond */
};


class LIBSBML_EXTERN ListOfLocalStyles : public ListOf
{
public:

  /**
   * Creates and returns a deep copy of the ListOfLocalStyles object.
   *
   * @return a (deep) copy of this ListOfLocalStyles
   */
  virtual ListOfLocalStyles* clone () const;

  /**
   * Creates a new ListOfLocalStyles object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfLocalStyles object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfLocalStyles
   * object to be instantiated.
   */
  ListOfLocalStyles(const XMLNode& node);

  /**
   * Constructor which instantiates an empty ListOfLocalStyles object.
   */
  ListOfLocalStyles(unsigned int level      = RenderExtension::getDefaultLevel(),
                    unsigned int version    = RenderExtension::getDefaultVersion(),
                    unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
  ListOfLocalStyles(RenderPkgNamespaces* renderns);



  /**
   * Copy constructor for ListOfLocalStyles objects.
   */
  ListOfLocalStyles(const ListOfLocalStyles& source);

  /**
   * Assignment operator for ListOfLocalStyles objects.
   */
  ListOfLocalStyles& operator=(const ListOfLocalStyles& source);

  /**
   * Returns the libSBML type code for the objects contained in this ListOf
   * (i.e., GradientDefinition objects, if the list is non-empty).
   * 
   * @if clike LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.
   * The set of possible type codes is defined in the enumeration
   * #SBMLTypeCode_t.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if java LibSBML attaches an
   * identifying code to every kind of SBML object.  These are known as
   * <em>SBML type codes</em>.  In other languages, the set of type codes
   * is stored in an enumeration; in the Java language interface for
   * libSBML, the type codes are defined as static integer constants in
   * interface class {@link libsbmlConstants}.  The names of the type codes
   * all begin with the characters @c SBML_. @endif
   * 
   * @return the SBML type code for the objects contained in this ListOf
   * instance, or @c SBML_UNKNOWN (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;

  /**
   * Returns the XML element name of this object, which for
   * ListOfLocalStyles, is always @c "listOfLocalStyles".
   * 
   * @return the name of this element, i.e., @c "listOfLocalStyles".
   */
  virtual const std::string& getElementName () const;


  /**
   * Creates an XMLNode object from this ListOfLocalStyles object.
   *
   * @return the XMLNode with the XML representation for the 
   * ListOfLocalStyles object.
   */
  XMLNode toXML() const;

  /**
   * Returns a pointer to the LocalStyle with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LocalStyle object to be returned
   * 
   * @return pointer to the LocalStyle at the given index or NULL.
   */
  LocalStyle* get(unsigned int i);

  /**
   * Returns a const pointer to the LocalStyle with the given index or NULL if
   * the index is invalid.
   * 
   * @param i index of the LocalStyle object to be returned
   * 
   * @return const pointer to the LocalStyle at the given index or NULL.
   */
  const LocalStyle* get(unsigned int i) const;

  /**
   * Returns a pointer to the LocalStyle with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LocalStyle object to be returned
   * 
   * @return pointer to the LocalStyle at the given @p id or @c NULL.
   */
  LocalStyle* get(const std::string& id);

  /**
   * Returns a const pointer to the LocalStyle with the given @p id or @c NULL if
   * the id is invalid.
   * 
   * @param id id of the LocalStyle object to be returned
   * 
   * @return const pointer to the LocalStyle at the given @p id or @c NULL.
   */
  const LocalStyle* get(const std::string& id) const;

  /**
   * Removes the nth item from this ListOfLocalStyles items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual LocalStyle* remove (unsigned int n);


  /**
   * Removes item in this ListOfLocalStyles items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual LocalStyle* remove (const std::string& sid);

  /**
   * Returns the libSBML type code for this %SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.
   * The set of possible type codes is defined in the enumeration
   * #SBMLTypeCode_t.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if java LibSBML attaches an
   * identifying code to every kind of SBML object.  These are known as
   * <em>SBML type codes</em>.  In other languages, the set of type codes
   * is stored in an enumeration; in the Java language interface for
   * libSBML, the type codes are defined as static integer constants in
   * interface class {@link libsbmlConstants}.  The names of the type codes
   * all begin with the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or @c SBML_UNKNOWN (default).
   *
   * @see getElementName()
   */
  virtual int getItemTypeCode() const; 


protected:

  /** @cond doxygenLibsbmlInternal */
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  static const std::string ELEMENT_NAME;
  /** @endcond */

};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /* LocalStyle_H__ */
