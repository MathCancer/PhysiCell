/**
 * @file    Association.h
 * @brief   Definition of Association, the contents of a GeneAssociation.
 * @author  Frank T. Bergmann
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
 * Copyright (C) 2009-2013 jointly by the following organizations:
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
 * @class Association
 * @sbmlbrief{fbc} Annotation helper class for &ldquo;fbc&rdquo; Version&nbsp;1.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The Association class is a helper class for creating annotations to store
 * gene association information in Version&nbsp;1 of the SBML Level&nbsp;3
 * @ref fbc (&ldquo;fbc&rdquo;) package.  In &ldquo;fbc&rdquo;
 * Version&nbsp;2, the equivalent capability is handled by the FbcAssociation
 * class and this class (Association) is not used.
 *
 * @see FbcAssociation
 * @see ListOfFbcAssociations
 */

#ifndef Association_H__
#define Association_H__

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>

#ifdef __cplusplus

#include <string>
#include <vector>

#include <sbml/SBase.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN


/**
 * @enum  AssociationTypeCode_t
 * @brief Enumeration of possible association children of the proposed
 * GeneAssociation class.
 *
 * These type codes are not part of the SBML Level&nbsp;3 @ref fbc
 * (&ldquo;fbc&rdquo;) package specification, but are used exclusively as
 * helpers for custom annotations of Version&nbsp;1 of the specification.  In
 * Version&nbsp;2, this information is handled bthough the use of the
 * FbcAssociation abstract class, with its FbcAnd, FbcOr, and
 * GeneProductAssociation derived classes.
 */
typedef enum
{
     GENE_ASSOCIATION     = 0 /*!< A 'gene' association (<code>&lt;fbc:gene&gt;</code>) */
   , AND_ASSOCIATION      = 1 /*!< An 'and' association (<code>&lt;fbc:and&gt;</code>) */
   , OR_ASSOCIATION       = 2 /*!< An 'or' association (<code>&lt;fbc:or&gt;</code>) */
   , UNKNOWN_ASSOCIATION  = 3 /*!< An unknown or unset association (no legal XML) */
} AssociationTypeCode_t;


class LIBSBML_EXTERN Association : public SBase
{
protected:
  /** @cond doxygenLibsbmlInternal */
  AssociationTypeCode_t mType;
  std::string mReference;
  std::vector<Association*> mAssociations;
  /** @endcond */

public:

  /**
   * Creates a new Association objet with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo; package version.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   Association(unsigned int level      = FbcExtension::getDefaultLevel(),
               unsigned int version    = FbcExtension::getDefaultVersion(),
               unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());

  /**
   * Creates a new Association with the given FbcPkgNamespaces object,
   * and constructed to mimic the XMLNode: a '&lt;gene&gt;' node 
   * argument will result in a  
   * @sbmlconstant{GENE_ASSOCIATION, AssociationTypeCode_t} node; an
   * &lt;and&gt; node argument will result in a
   * @sbmlconstant{AND_ASSOCIATION, AssociationTypeCode_t} node; an
   * &lt;or&gt; node argument will result in a
   * @sbmlconstant{OR_ASSOCIATION, AssociationTypeCode_t} node.  Any
   * other node will result in a
   * @sbmlconstant{UNKNOWN_ASSOCIATION, AssociationTypeCode_t} node.
   * 'And' and 'or' nodes will have Association children that again 
   * parallel the children of the given XMLNode.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   * @param node the XMLNode to copy.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   Association(const XMLNode& node, FbcPkgNamespaces* fbcns);


  /**
   * Creates a new Association with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
   Association(FbcPkgNamespaces* fbcns);


  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
   Association(const Association& source);


  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
   Association& operator=(const Association& source);


  /**
   * Destructor.
   */
  virtual ~Association ();


  /**
   * Returns the string of the "type" attribute of this Association object.
   *
   * @return the string of the "type" attribute of this Association object.
   */
  virtual AssociationTypeCode_t getType () const;


  /**
   * Predicate returning @c true if this Association's "type" attribute is
   * set.
   *
   * @return @c true if this Association object's "type" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetType () const;


  /**
   * Sets the SIdRef string of the "type" attribute of this Association object.
   *
   * @param type a SIdRef string to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setType (const AssociationTypeCode_t type);


  /**
   * Unsets the value of the "id" attribute of this Association object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetType ();


  /**
   * Returns the string of the "reference" attribute of this Association object.
   *
   * @return the string of the "reference" attribute of this Association object.
   */
  virtual const std::string& getReference () const;


  /**
   * Predicate returning @c true if this Association's "reference" attribute
   * has been set.
   *
   * @return @c true if this Association object's "reference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetReference () const;


  /**
   * Sets the SIdRef string of the "reference" attribute of this Association object.
   *
   * @param reference a SIdRef string to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  virtual int setReference (const std::string& reference);


  /**
   * Unsets the value of the "id" attribute of this Association object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetReference ();


  /**
   * Adds a gene with the given @p id to the association.
   *
   * @param id the gene name.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int addGene(const std::string& id);


  /**
   * Returns the number of child Associations of this Association object.
   *
   * @return the number of associations.
   */
  virtual unsigned int getNumAssociations();


  /**
   * Adds a child Association to this Association object.
   *
   * @param association the Association object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int addAssociation(Association &association);


  /**
   * Removes the child Associations with the given @p index from this
   * Association object.
   *
   * @param index the index number of the item to remove.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int removeAssociation(int index);


  /**
   * Removes all children of this Association object.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int clearAssociations();


  /**
   * Creates a new Association of type "and".
   *
   * This method does not actually add the created Association as a child of
   * this Association object or do anything else with it&mdash;the returning
   * pointer is now owned by the caller.
   *
   * @return a new "and" type association.
   */
  virtual Association* createAnd();


  /**
   * Creates a new Association of type 'or'.
   *
   * This method does not actually add the created Association as a child of
   * this Association object or do anything else with it&mdash;the returning
   * pointer is now owned by the caller.
   *
   * @return a new "or" type association.
   */
  virtual Association* createOr();


  /**
   * Creates a new Association of type 'and' with a given gene reference.
   *
   * This method does not actually add the created Association as a child of
   * this Association object or do anything else with it&mdash;the returning
   * pointer is now owned by the caller.
   *
   * @param reference the gene reference, as a string.
   *
   * @return a new Association object.
   */
  virtual Association* createGene(const std::string reference = "" );


  /**
   * Creates an XMLNode object from this Association object.
   */
  XMLNode toXML() const;


  /**
   * Returns the XML element name of this SBML object.
   *
   * @return the name of this element, as a text string.
   */
  virtual const std::string& getElementName () const ;


  /**
   * Creates and returns a deep copy of this Association object.
   *
   * @return a (deep) copy of this Association object.
   */
  virtual Association* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_ASSOCIATION, SBMLFbcTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


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
   * @param v the visitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond */


  /**
   * Parses a gene association in infix format and returns a corresponding
   * Association object.
   *
   * This parses a string that has a list of gene names and conjunctions
   * or disjunctions.  For example:
   * @verbatim
(b2422) and (b2425) and (b2423) and (b2424) or (b2422) and (b2423) and (b2424) and (b2413) and (b3917)
@endverbatim
   *
   * @param association the string to parse.
   *
   * @return the parsed association, or @c NULL in case of an error.
   *
   * @copydetails doc_note_static_methods
   *
   * @see toInfix()
   */
  static Association* parseInfixAssociation(const std::string& association);


  /**
   * Converts this Association object into an infix string representation.
   * The format is the same as is found in parseInfixAssociation().
   *
   * @return the association as infix string.
   *
   * @see parseInfixAssociation(const std::string& association)
   */
  std::string toInfix() const;


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase*
  createObject (XMLInputStream& stream);
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
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */
};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/*
 * C API will be added here.
 */

END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* Association_H__ */
