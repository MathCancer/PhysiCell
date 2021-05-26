/**
 * @file    ListOfPorts.h
 * @brief   Definition of ListOfPorts, the SBase derived class of ports package.
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
 *------------------------------------------------------------------------- -->
 *
 * @class ListOfPorts
 * @sbmlbrief{comp} A list of Port objects.
 * 
 * The ListOfPorts is a container for the extended 
 * Model for Port objects for that Model.
 * 
 * @copydetails doc_what_is_listof
 *
 * @see Port
 * @see ListOfDeletions
 * @see ListOfExternalModelDefinitions
 * @see ListOfModelDefinitions
 * @see ListOfReplacedElements
 * @see ListOfSubmodels
 */


#ifndef ListOfPorts_H__
#define ListOfPorts_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/ListOf.h>
#include <sbml/packages/comp/sbml/Port.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN ListOfPorts : public ListOf
{
public:

  /**
   * Creates and returns a deep copy of this ListOfPorts object.
   *
   * @return a (deep) copy of this ListOfPorts.
   */
  virtual ListOfPorts* clone () const;


  /**
   * Creates a new ListOfPorts with the given level, version, and package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfPorts(unsigned int level      = CompExtension::getDefaultLevel(), 
              unsigned int version    = CompExtension::getDefaultVersion(), 
              unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfPorts with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfPorts(CompPkgNamespaces* compns);


  /**
   * Get a Port from the ListOfPorts.
   *
   * @param n the index number of the Port to get.
   * 
   * @return the nth Port in this ListOfPorts.
   *
   * @see size()
   */
  virtual Port* get(unsigned int n); 


  /**
   * Get a Port from the ListOfPorts.
   *
   * @param n the index number of the Port to get.
   * 
   * @return the nth Port in this ListOfPorts.
   *
   * @see size()
   */
  virtual const Port * get(unsigned int n) const; 


  /**
   * Get a Port from the ListOfPorts
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Port to get.
   * 
   * @return Port in this ListOfPorts
   * with the given @p sid or @c NULL if no such
   * Member exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual Port* get (const std::string& sid);


  /**
   * Get a Port from the ListOfPorts
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Port to get.
   * 
   * @return Port in this ListOfPorts
   * with the given @p sid or @c NULL if no such
   * Port exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const Port* get (const std::string& sid) const;

  
  /**
   * Removes an item from this ListOfPorts items based on the identifier and
   * returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param sid string representing the id of the Port to remove.
   *
   * @see size()
   */
  virtual Port* remove (const std::string& sid);


  /**
   * Removes the nth item from this ListOfPorts items and returns a pointer
   * to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove.
   *
   * @see size()
   */
  virtual Port* remove (unsigned int n);


  /**
   * Returns the libSBML type code for the objects contained in this ListOf
   * (i.e., Port objects, if the list is non-empty).
   * 
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for objects contained in this list:
   * @sbmlconstant{SBML_COMP_PORT, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


  /**
   * Returns the XML element name of
   * this SBML object.
   *
   * @return the name of this element, as a text string.
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.  Since the
   * id of Port objects are in the PortSId namespace, no Port object is
   * returned by this function.
   *
   * @param id string representing the id of the object to find.
   *
   * @return a pointer to the SBase element with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
 
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

//
// C API will be added here.
//

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* ListOfPorts_H__ */
