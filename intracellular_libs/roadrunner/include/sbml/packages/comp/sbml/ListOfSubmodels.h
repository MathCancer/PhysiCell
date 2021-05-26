/**
 * @file    ListOfSubmodels.h
 * @brief   Definition of ListOfSubmodels, the SBase derived class of submodels package.
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
 * @class ListOfSubmodels
 * @sbmlbrief{comp} A list of Submodel objects.
 * 
 * The ListOfSubmodels is a container for the extended 
 * Model that contains Submodel objects to be instantiated in that Model.
 * 
 * @copydetails doc_what_is_listof
 *
 * @see Submodel
 * @see ListOfDeletions
 * @see ListOfExternalModelDefinitions
 * @see ListOfModelDefinitions
 * @see ListOfPorts
 * @see ListOfReplacedElements
 */


#ifndef ListOfSubmodels_H__
#define ListOfSubmodels_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/ListOf.h>
#include <sbml/packages/comp/sbml/Submodel.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ListOfSubmodels : public ListOf
{
public:

  /**
   * Creates and returns a deep copy of this ListOfSubmodels object.
   *
   * @return a (deep) copy of this ListOfSubmodels.
   */
  virtual ListOfSubmodels* clone () const;


  /**
   * Creates a new ListOfSubmodels with the given level, version, and package
   * version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfSubmodels(unsigned int level      = CompExtension::getDefaultLevel(), 
                  unsigned int version    = CompExtension::getDefaultVersion(), 
                  unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfSubmodels with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ListOfSubmodels(CompPkgNamespaces* compns);


  /**
   * Get a Submodel from the ListOfSubmodels.
   *
   * @param n the index number of the Submodel to get.
   * 
   * @return the nth Submodel in this ListOfSubmodels.
   *
   * @see size()
   */
  virtual Submodel* get(unsigned int n); 


  /**
   * Get a Submodel from the ListOfSubmodels.
   *
   * @param n the index number of the Submodel to get.
   * 
   * @return the nth Submodel in this ListOfSubmodels.
   *
   * @see size()
   */
  virtual const Submodel* get(unsigned int n) const; 


  /**
   * Get a Model from the ListOfSubmodels
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Model to get.
   * 
   * @return Model in this ListOfSubmodels
   * with the given @p sid or @c NULL if no such
   * Member exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual Submodel* get (const std::string& sid);


  /**
   * Get a Model from the ListOfSubmodels
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Model to get.
   * 
   * @return Model in this ListOfSubmodels
   * with the given @p sid or @c NULL if no such
   * Model exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const Submodel* get (const std::string& sid) const;


  /**
   * Removes the nth item from this ListOfSubmodels items and returns a
   * pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove.
   *
   * @see size()
   */
  virtual Submodel* remove (unsigned int n);


  /**
   * Removes an item from this ListOfSubmodels items based on its identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param sid string representing the identifier of the item to remove.
   *
   * @see size()
   */
  virtual Submodel* remove (const std::string& sid);


  /**
   * Returns the libSBML type code for the objects contained in this ListOf
   * (i.e., Submodel objects, if the list is non-empty).
   * 
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for objects contained in this list:
   * @sbmlconstant{SBML_COMP_SUBMODEL, SBMLTypeCode_t} (default).
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
#endif  /* ListOfSubmodels_H__ */
