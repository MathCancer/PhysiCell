/**
 * @file    Replacing.h
 * @brief   Definition of Replacing, the SBaseRef-derived class of the comp package.
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
 * @class Replacing
 * @sbmlbrief{comp} Convenience class.
 *
 * The Replacing class does not exist officialy in the the @ref comp
 * (&ldquo;comp&rdquo;) package, but is implemented here as a convenience
 * subclass of the ReplacedElement and ReplacedBy classes, since both of
 * those classes define a 'submodelRef' attribute.
 *
 * The required attribute "submodelRef" takes a value of type
 * SIdRef, which must be the identifier of a Submodel object in
 * the containing model.  The model referenced by the
 * Submodel object establishes the object namespaces for the
 * "portRef", "idRef", "unitRef" and "metaIdRef"
 * attributes: only objects within the Model object may be referenced by
 * those attributes.
 */


#ifndef Replacing_H__
#define Replacing_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/comp/common/compfwd.h>

#ifdef __cplusplus

#include <string>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/SBaseRef.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Replacing : public SBaseRef
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string   mSubmodelRef;
  std::string   mConversionFactor;
  /** @endcond */

public:

  /**
   * Creates a new Replacing with the given level, version, and package
   * version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Replacing(unsigned int level      = CompExtension::getDefaultLevel(),
            unsigned int version    = CompExtension::getDefaultVersion(),
            unsigned int pkgVersion = CompExtension::getDefaultPackageVersion());


  /**
   * Creates a new Replacing with the given CompPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param compns the CompPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Replacing(CompPkgNamespaces* compns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
  Replacing(const Replacing& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
  Replacing& operator=(const Replacing& source);


  /**
   * Destructor.
   */
  virtual ~Replacing ();


  /**
   * Returns the value of the "submodelRef" attribute of this SBaseRef.
   *
   * @return the value of the "submodelRef" attribute of this SBaseRef.
   */
  virtual const std::string& getSubmodelRef () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SBaseRef's "submodelRef" attribute has been set.
   *
   * @return @c true if this SBaseRef's "submodelRef" attribute has been set, 
   * otherwise @c false is returned.
   */
  virtual bool isSetSubmodelRef () const;

  
  /**
   * Sets the value of the "submodelRef" attribute of this SBaseRef.  Fails
   * if the id is not a valid syntax for an SIdRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setSubmodelRef (const std::string& id);


  /**
   * Unsets the value of the "SubmodelRef" attribute of this SBaseRef.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetSubmodelRef ();


  /**
   * Returns @c true if getNumReferents() is exactly 1 and if the submodelRef is set.
   *
   * @return boolean: 'true' if the attributes are correctly set; 'false' if not.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Finds and stores the referenced object.  Finds the Submodel to which
   * it refers, getting the instantiated Model inside that Submodel, calling
   * 'getReferencedElementFrom' on that model, and storing the result.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int saveReferencedElement();


  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * DEPRECATED FUNCTION:  DO NOT USE
   * 
   * To retain old functionality, this function calls performReplacementAndCollect,
   * and then actually removes the now-redundant element.  However, this can lead
   * to doubly-deleted elements, as well as the incorrect interpretation of some
   * models.  The replacement function performReplacementAndCollect
   * has been marked protected, in the hopes that people will instead simply
   * use CompModelPlugin::instantiateSubmodels, which hides all the complexity while
   * still allowing access to a non-flattened version of a hierarchical model.
   */
  virtual int performReplacement();


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


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


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use.
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /* Replaces the referenced object with the replacement element, instead of the direct parent of this replaced element.  This is so we can call this recursively, when replacing something that itself replaces something.
   */
  virtual int replaceWithAndMaybeDelete(SBase* replacement, bool deleteme, ASTNode* conversionFactor);
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


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "conversionFactor", mConversionFactor );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Searches the model that @p oldnames came from for references to any of its ids,
   * and replaces them with references to @p newnames.  
   *
   * @param oldnames the object being replaced, and whose parent Model contains
   * the references that need to be updated.
   *
   * @param newnames the object that should now be referenced instead, to which 
   * any references to @p oldnames should now point.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   */
  virtual int updateIDs(SBase* oldnames, SBase* newnames);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Modify mathematical references to the referenced object according to the @p conversionFactor.  
   *
   * Find all idrefs in the Model that have mathematical meaning, and convert 
   * them to instead reference the @p replacement, modified according to the 
   * @p conversionFactor.  Will modify the referenced object's use in MathML 
   * nodes according to replacementID/conversionFactor, and will multiply the 
   * MathML of elements that assign to the referenced object by the @p conversionFactor.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int performConversions(SBase* replacement, ASTNode** conversionFactor);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Multiply this element's conversion factor (if present) to the @p conversionFactor.
   */
  virtual int convertConversionFactor(ASTNode** conversionFactor);
  /** @endcond */


  /**
   * An internal flattening routine, necessarily overridden by any subclass, to 
   * rename the necessary elements, perform any conversions, and add the now-redundant
   * element to the 'toremove' list.
   */
  virtual int performReplacementAndCollect(std::set<SBase*>* removed, 
                                           std::set<SBase*>* toremove) = 0;

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
#endif  /* Replacing_H__ */
