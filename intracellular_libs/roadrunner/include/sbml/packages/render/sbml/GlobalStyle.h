/**
 * @file    GlobalStyle.h
 * @brief   class for representing a global style object
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
 * @class GlobalStyle
 * @brief implementation of the GlobalStyle concept of the SBML render extension.
 *
 * Global styles are the style information objects used in GlobalRenderInformation (@see GlobalRenderInformation).
 * Global styles can be associated with layout objects by role and type, but not by id, otherwise global 
 * styles and local styles are equivalent.
 *
 * Since GlobalStyle is derived from Styles, it inherits all of the methods and attributes from Style. (@see Style) 
 *
 * @class ListOfGlobalStyles
 * @brief ListOfGlobalStyles is the container class that stores GlobalStyles in GlobalRenderInformation objects.
 *
 * Each GlobalRenderInformation object contains a ListOfGlobalStyles which contains zero or
 * more global style objects. 
 */

#ifndef GlobalStyle_H__
#define GlobalStyle_H__

#include <sbml/common/sbmlfwd.h>

#include "sbml/packages/render/sbml/Style.h"
#include <sbml/ListOf.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLInputStream.h>

#ifdef __cplusplus

#include <sbml/packages/render/extension/RenderExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN GlobalStyle : public Style
{
public:
  /**
   * Creates a new GlobalStyle object with the given SBML level
   * and SBML version.
   *
   * @param level SBML level of the new object
   * @param level SBML version of the new object
   */
  GlobalStyle (unsigned int level      = RenderExtension::getDefaultLevel(),
               unsigned int version    = RenderExtension::getDefaultVersion(),
               unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());


  /**
   * Creates a new GlobalStyle object with the given SBMLNamespaces.
   *
   * @param sbmlns The SBML namespace for the object.
   */
  GlobalStyle (RenderPkgNamespaces* renderns);


  /**
   * Creates a new GlobalStyle object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * GlobalStyle object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the GlobalStyle
   * object to be instantiated.
   */
  GlobalStyle(const XMLNode& node, unsigned int l2version=4);

  
  /**
   * Destroy this GlobalStyle object.
   */
  virtual ~GlobalStyle ();



#ifndef OMIT_DEPRECATED
  /**
   * Constructor which creates a GlobalStyle with the given @p id
   * and all lists empty.
   *
   * @param id the new id for the GlobalStyle.
   *
   * This constructor is deprecated. The new libsbml API only has
   * constructors which take the SBML level and version or one that takes
   * an SBMLNamespaces object.
   */
  GlobalStyle(RenderPkgNamespaces* renderns, const std::string& id);
#endif // OMIT_DEPRECATED

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
   * This method is purely abstract and has to be implemented by derived
   * classes.
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;

  /**
   * Returns the XML element name of this object, which for
   * GlobalStyle, is always @c "renderInformation".
   * 
   * @return the name of this element, i.e., @c "renderInformation".
   */
  virtual const std::string& getElementName() const;

  /**
   * Creates and returns a deep copy of this GlobalStyle object.
   *
   * @return a (deep) copy of this GlobalStyle.
   */
  GlobalStyle* clone() const;

protected:
  /** @cond doxygenLibsbmlInternal */
  static const std::string ELEMENT_NAME;  
  /** @endcond */
};


class LIBSBML_EXTERN ListOfGlobalStyles : public ListOf
{
 public:
  /**
   * Creates and returns a deep copy of the ListOfGlobalStyles object.
   *
   * @return a (deep) copy of this ListOfGlobalStyles
   */
  virtual ListOfGlobalStyles* clone () const;
  
  /**
   * Creates a new ListOfGlobalStyles object from the given XMLNode object.
   * The XMLNode object has to contain a valid XML representation of a 
   * ListOfGlobalStyles object as defined in the render extension specification.
   * This method is normally called when render information is read from a file and 
   * should normally not have to be called explicitely.
   *
   * @param node the XMLNode object reference that describes the ListOfGlobalStyles
   * object to be instantiated.
   */
  ListOfGlobalStyles(const XMLNode& node);

  /**
   * Constructor which instantiates an empty ListOfGlobalStyles object.
   */
    ListOfGlobalStyles(unsigned int level      = RenderExtension::getDefaultLevel(),
                         unsigned int version    = RenderExtension::getDefaultVersion(),
                         unsigned int pkgVersion = RenderExtension::getDefaultPackageVersion());

  /**
   * Ctor.
   */
   ListOfGlobalStyles(RenderPkgNamespaces* renderns);

  /**
   * Copy constructor for ListOfGlobalStyles objects.
   */
   ListOfGlobalStyles(const ListOfGlobalStyles& source);

  /**
   * Assignment operator for ListOfGlobalStyles objects.
   */
   ListOfGlobalStyles& operator=(const ListOfGlobalStyles& source);

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
   * Get the type code of the objects contained in this ListOf.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   * 
   * @return the SBML type code for the objects contained in this ListOf
   * instance, or @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   */
  virtual int getItemTypeCode () const;

  /**
   * Returns the XML element name of this object, which for
   * ListOfGlobalStyles, is always @c "listOfStyles".
   * 
   * @return the name of this element, i.e., @c "listOfStyles".
   */
  virtual const std::string& getElementName () const;

   /**
    * Creates an XMLNode object from this ListOfGlobalStyles object.
    *
    * @return the XMLNode with the XML representation for the 
    * ListOfGlobalStyles object.
    */
  XMLNode toXML() const;

   /**
    * Returns a pointer to the GlobalStyle with the given index or NULL if
    * the index is invalid.
    * 
    * @param i index of the GlobalStyle object to be returned
    * 
    * @return pointer to the GlobalStyle at the given index or NULL.
    */
   GlobalStyle* get(unsigned int i);

   /**
    * Returns a const pointer to the GlobalStyle with the given index or NULL if
    * the index is invalid.
    * 
    * @param i index of the GlobalStyle object to be returned
    * 
    * @return const pointer to the GlobalStyle at the given index or NULL.
    */
   const GlobalStyle* get(unsigned int i) const;

   /**
    * Returns a pointer to the GlobalStyle with the given @p id or @c NULL if
    * the id is invalid.
    * 
    * @param id id of the GlobalStyle object to be returned
    * 
    * @return pointer to the GlobalStyle at the given @p id or @c NULL.
    */
   GlobalStyle* get(const std::string& id);

   /**
    * Returns a const pointer to the GlobalStyle with the given @p id or @c NULL if
    * the id is invalid.
    * 
    * @param id id of the GlobalStyle object to be returned
    * 
    * @return const pointer to the GlobalStyle at the given @p id or @c NULL.
    */
   const GlobalStyle* get(const std::string& id) const;

    /**
   * Removes the nth item from this ListOfGlobalStyles items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual GlobalStyle* remove (unsigned int n);


  /**
   * Removes item in this ListOfGlobalStyles items with the given identifier.
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
  virtual GlobalStyle* remove (const std::string& sid);

    
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

#endif /* GlobalStyle_H__ */
