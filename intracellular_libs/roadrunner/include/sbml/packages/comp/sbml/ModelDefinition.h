/**
 * @file    ModelDefinition.h
 * @brief   Definition of ModelDefinition, the SBase derived class of the comp package.
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
 * @class ModelDefinition
 * @sbmlbrief{comp} A model used in model composition.
 *
 * The @ref comp (&ldquo;comp&rdquo;) package allows multiple Model objects
 * to be defined in a single SBMLDocument.  While these new Model objects are
 * not new SBML classes, they are given a new name,
 * <code>&lt;modelDefinition&gt;</code>, and reside in ListOfModelDefinition
 * objects.  In libSBML, this class inherits from the Model class, changing
 * only the expected parent of the object, and the XML name.
 *
 * An additional restriction is placed on the "id" attribute of ModelDefinition
 * objects:  not only must it be unique across all such attributes of type SId 
 * within the ModelDefintion, it must also be unique across all Model, 
 * ModelDefinition, and ExternalModelDefinition objects in the same SBMLDocument.
 */

#ifndef ModelDefinition_H__
#define ModelDefinition_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/Model.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/CompBase.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN ModelDefinition : public Model
{
public:

  /**
   * Creates a new ModelDefinition with the given level, version, and package
   * version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ModelDefinition(unsigned int level      = CompExtension::getDefaultLevel(),
                  unsigned int version    = CompExtension::getDefaultVersion(),
                  unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new ModelDefinition with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  ModelDefinition(CompPkgNamespaces* compns);


  /**
   * Copy constructor from base Model object.
   *
   * @param source the instance to copy.
   */
  ModelDefinition(const Model& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  ModelDefinition& operator=(const Model& source);


  /**
   * Creates and returns a deep copy of this ModelDefinition object.
   * 
   * @return a (deep) copy of this ModelDefinition object.
   */
  virtual ModelDefinition* clone () const;


  /**
   * Destructor.
   */
  virtual ~ModelDefinition ();


  /**
   * Returns the XML element name of this object.
   *
   * The only difference between a Model and a ModelDefinition is the
   * element name ('modelDefinition').
   *
   * @return the name of this element, as a text string.
   *
   * @see getTypeCode()
   */
  virtual const std::string& getElementName () const ;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_COMP_MODELDEFINITION, SBMLCompTypeCode_t}
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Finds this Model's parent ListOfModelDefinitions and removes itself from
   * it and deletes itself.
   *
   * This method actually just calls the SBase function, since the Model
   * class overrides it, but that's actually what we want to happen here.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int removeFromParentAndDelete();


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

protected:
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



};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new ModelDefinition_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * ModelDefinition_t.
 * @param version an unsigned int, the SBML Version to assign to this
 * ModelDefinition_t.
 * @param pkgVersion an unsigned int, the SBML 'comp' package Version to assign to this
 * ModelDefinition_t.
 *
 * @return a pointer to the newly created ModelDefinition_t structure.
 *
 * @memberof ModelDefinition_t
 */
LIBSBML_EXTERN
ModelDefinition_t *
ModelDefinition_create(unsigned int level, unsigned int version,
                       unsigned int pkgVersion);

/**
 * Frees the given ModelDefinition_t structure.
 *
 * @param md the ModelDefinition_t structure to free.
 *
 * @memberof ModelDefinition_t
 */
LIBSBML_EXTERN
void
ModelDefinition_free(ModelDefinition_t * md);


/**
 * Returns a copy of the given ModelDefinition_t structure.
 *
 * @param md the ModelDefinition_t structure to copy.
 * 
 * @return a (deep) copy of the ModelDefinition_t.
 *
 * @memberof ModelDefinition_t
 */
LIBSBML_EXTERN
ModelDefinition_t *
ModelDefinition_clone(ModelDefinition_t * md);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* ModelDefinition_H__ */
