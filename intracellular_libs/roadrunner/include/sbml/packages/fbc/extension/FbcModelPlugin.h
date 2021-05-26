/**
 * @file    FbcModelPlugin.h
 * @brief   Definition of FbcModelPlugin, the plugin class of
 *          fbc package for the Model element.
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
 * @class FbcModelPlugin
 * @sbmlbrief{fbc} Extension of Model.
 *
 * The FbcModelPlugin object is used to extend the standard SBML Model object
 * with features used in the SBML Level&nbsp;3 @ref fbc (&ldquo;fbc&rdquo;)
 * package.  In Version&nbsp;1 of the &ldquo;fbc&rdquo; specification, the
 * extended Model class has two optional subobjects: ListOfObjectives and
 * ListOfFluxBounds.  In Version&nbsp;2 of the specification, the extended
 * Model object is defined differently: it is extended with a new required
 * attribute named "strict", and the two optional subobjects ListOfObjectives
 * and ListOfGeneProducts.  (ListOfFluxBounds is not used in Version&nbsp;2.)
 *
 * @section model-strict The "strict" attribute on the (extended) Model class
 *
 * The mandatory attribute "strict", of type <code>boolean</code>, in 
 * Version&nbsp;2 of this package, is used to
 * apply an additional set of restrictions to the model.  The "strict"
 * attribute helps ensure that the Flux Balance Constraints package can be
 * used to encode legacy flux-balance analysis models expressible as Linear
 * Programs (LP's) for software that is unable to analyze arbitrary
 * mathematical expressions that may appear in an SBML model.  In addition, a
 * "strict" model is fully described and mathematically consistent, for
 * example, by ensuring that all fluxes have a valid upper or lower bound.
 *
 * The following restrictions are in effect if an &ldquo;fbc&rdquo; model
 * object has a value of <code>"true"</code> for the attribute "strict" on
 * Model:
 *
 * @li Each Reaction in a Model must define values for the attributes
 * "lowerFluxBound" and "upperFluxBound", with each attribute pointing to a
 * valid Parameter object defined in the current Model.
 *
 * @li Each Parameter object referred to by the Reaction attributes
 * "lowerFluxBound" and "upperFluxBound" must have its "constant" attribute
 * set to the value <code>"true"</code> and its "value" attribute set to a
 * value of type <code>double</code>.  This value may not be
 * <code>"NaN"</code>.
 *
 * @li SpeciesReference objects in Reaction objects must have their
 * "stoichiometry" attribute set to a <code>double</code> value that is not
 * <code>"NaN"</code>, nor <code>"-INF"</code>, nor <code>"INF"</code>. In
 * addition, the value of their "constant" attribute must be set to
 * <code>"true"</code>.
 *
 * @li InitialAssignment objects may not target the Parameter objects
 * referenced by the Reaction attributes "lowerFluxBound" and
 * "upperFluxBound", nor any SpeciesReference objects.
 *
 * @li All defined FluxObjective objects must have their coefficient
 * attribute set to a <code>double</code> value that is not
 * <code>"NaN"</code>, nor <code>"-INF"</code>, nor <code>"INF"</code>.
 *
 * @li A Reaction "lowerFluxBound" attribute may not point to a Parameter
 * object that has a value of <code>"INF"</code>.
 *
 * @li A Reaction "upperFluxBound" attribute may not point to a Parameter
 * object that has a value of <code>"-INF"</code>.
 *
 * @li For all Reaction objects, the value of a "lowerFluxBound" attribute
 * must be less than or equal to the value of the "upperFluxBound" attribute.
 *
 * While it is not compulsory for a "strict" Flux Balance Constraints model
 * to define an Objective, doing so does does allow the model to be
 * formulated as a Linear Program and optimized.  However, this decision is
 * left to the modeler.  Note that all other properties of the objects
 * referred to in the list above are to be set as specified in the relevant
 * SBML Level&nbsp;3 Version&nbsp;1 Core and @ref fbc (&ldquo;fbc&rdquo;)
 * specifications.
 *
 * Alternatively, if the value of the strict attribute is
 * <code>"false"</code>, then none of these restrictions apply and the model
 * creator can choose to define &ldquo;fbc&rdquo; models that are not
 * necessarily encodable as an LP.  For example, if strict is
 * <code>"false"</code>, the InitialAssignment construct may be used to set
 * any valid numerical entity, including Parameter values and stoichiometric
 * coefficients, with any value of type <code>double</code>.  In addition,
 * Parameter elements are no longer required to be flagged as constant, thus
 * allowing for a Flux Balance Constraints model's use in alternative, hybrid
 * modeling strategies.
 *
 *
 * @section model-subobjects Lists of subobjects on the (extended) Model class
 *
 * The ListOfObjectives is used to define the objectives of a given
 * &ldquo;fbc&rdquo; model.  Objectives generally consist of linear
 * combinations of model variables (fluxes) and a direction for the
 * optimality constraint (either maximization or minimization).  Each
 * Objective has a ListOfFluxObjectives subobjects.
 *
 * In Version&nbsp;2 of &ldquo;fbc&rdquo;, the ListOfGeneProducts is used to
 * define the gene products represented by the &ldquo;fbc&rdquo; model.
 *
 * In Version&nbsp;1 of &ldquo;fbc&rdquo;, there is no ListOfGeneProducts,
 * and instead, Model can have an optional ListOfFluxBounds.
 *
 * @see Objective
 * @see FluxObjective
 * @see FluxBound
 */

#ifndef FbcModelPlugin_H__
#define FbcModelPlugin_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>

#ifdef __cplusplus

#include <sbml/SBMLErrorLog.h>
#include <sbml/Model.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>
#include <sbml/packages/fbc/sbml/FluxBound.h>
#include <sbml/packages/fbc/sbml/Objective.h>
#include <sbml/packages/fbc/sbml/GeneAssociation.h>
#include <sbml/packages/fbc/sbml/GeneProduct.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN FbcModelPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new FbcModelPlugin object using the given parameters.
   *
   * @copydetails doc_what_are_xmlnamespaces
   *
   * @copydetails doc_what_are_sbmlnamespaces
   *
   * @param uri the URI of the SBML Level&nbsp;3 package implemented by
   * this libSBML package extension.
   *
   * @param prefix the XML namespace prefix being used for the package.
   *
   * @param fbcns the namespaces object for the package.
   */
  FbcModelPlugin(const std::string& uri, const std::string& prefix, 
                                 FbcPkgNamespaces* fbcns);


  /**
   * Copy constructor for FbcModelPlugin.
   *
   * @param orig the FbcModelPlugin instance to copy.
   */
  FbcModelPlugin(const FbcModelPlugin& orig);


   /**
   * Assignment operator for FbcModelPlugin.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcModelPlugin& operator=(const FbcModelPlugin& rhs);


   /**
   * Creates and returns a deep copy of this FbcModelPlugin object.
   *
   * @return a (deep) copy of this FbcModelPlugin object.
   */
  virtual FbcModelPlugin* clone () const;


   /**
   * Destructor for FbcModelPlugin.
   */
  virtual ~FbcModelPlugin();


   //---------------------------------------------------------------
  //
  // overridden virtual functions for reading/writing/checking 
  // elements
  //
  // --------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to create, store, and then
   * return an SBML object corresponding to the next XMLToken in the
   * XMLInputStream if they have their specific elements.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);


  /** @endcond */
  
  /** @cond doxygenLibsbmlInternal */
  int appendFrom(const Model* model);
  
  /** @endcond */
  
  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond */


  /*
   * Checks if this plugin object has all the required elements.
   *
   * Subclasses must override this method 
   * if they have their specific elements.
   *
   * @return @c true if this plugin object has all the required elements
   * otherwise @c false will be returned.
   */
  //virtual bool hasRequiredElements () const;

  /** @cond doxygenLibsbmlInternal */
  /**
   * Parses Gene Annotation Extension 
   */
  virtual bool readOtherXML (SBase* parentObject, XMLInputStream& stream);
  /** @endcond */


  //---------------------------------------------------------------


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond */


  //---------------------------------------------------------------
  //
  // Functions for interacting with the members of the plugin
  //
  //---------------------------------------------------------------

  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @param filter an ElementFilter that may impose restrictions on the
   * objects to be retrieved.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the value of the "strict" attribute of this FbcModelPlugin.
   *
   * @copydetails note_strict_v2_only
   *
   * @return the value of the "strict" attribute of this FbcModelPlugin as a boolean.
   */
  virtual bool getStrict() const;


  /**
   * Predicate returning @c true if this FbcModelPlugin's "strict" attribute
   * is set.
   *
   * @copydetails note_strict_v2_only
   *
   * @return @c true if this FbcModelPlugin's "strict" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetStrict() const;


  /**
   * Sets the value of the "strict" attribute of this FbcModelPlugin.
   *
   * @copydetails note_strict_v2_only
   *
   * @param strict bool value of the "strict" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  virtual int setStrict(bool strict);


  /**
   * Unsets the value of the "strict" attribute of this FbcModelPlugin.
   *
   * @copydetails note_strict_v2_only
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  virtual int unsetStrict();

 
  /**
   * Returns the ListOfFluxBounds in this plugin object.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @return ListOfFluxBounds object in this plugin object.
   */
  const ListOfFluxBounds* getListOfFluxBounds () const;


  /**
   * Returns the ListOfFluxBounds in this plugin object.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @return ListOfFluxBounds object in this plugin object.
   */
  ListOfFluxBounds* getListOfFluxBounds ();


  /**
   * Returns the FluxBound object that belongs to the given index. If the
   * index is invalid, @c NULL is returned.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param n the index number of the FluxBound to get.
   *
   * @return the nth FluxBound in the ListOfFluxBounds.
   */
  const FluxBound* getFluxBound (unsigned int n) const;


  /**
   * Returns the FluxBound object that belongs to the given index. If the
   * index is invalid, @c NULL is returned.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param n the index number of the FluxBound to get.
   *
   * @return the nth FluxBound in the ListOfFluxBounds.
   */
  FluxBound* getFluxBound (unsigned int n);


  /**
   * Returns the FluxBound object based on its identifier.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param sid a string representing the identifier 
   * of the FluxBound to get.
   * 
   * @return FluxBound in the ListOfFluxBounds with the given @p sid
   * or NULL if no such FluxBound exists.
   *
   * @see getFluxBound(unsigned int n)
   * @see getListOfFluxBounds()
   */
  FluxBound* getFluxBound (const std::string& sid);


  /**
   * Returns the FluxBound object based on its identifier.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param sid a string representing the identifier 
   * of the FluxBound to get.
   * 
   * @return FluxBound in the ListOfFluxBounds with the given @p sid 
   * or NULL if no such FluxBound exists.
   *
   * @see getFluxBound(unsigned int n)
   * @see getListOfFluxBounds()
   */
  const FluxBound* getFluxBound (const std::string& sid) const;


  /**
   * Creates a new ListOfFluxBounds object that contains only the 
   * FluxBound objects associated with the given Reaction.  If no such
   * Reaction can be found, or if there are no FluxBound objects associated
   * with it, returns NULL. @if clike The caller owns the created object
   * and is responsible for its deletion.@endif
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param reaction the id of an reaction to find FluxBound objects for.
   *
   * @return a listOfFluxBounds for the given reaction id.
   */
  ListOfFluxBounds *  getFluxBoundsForReaction(const std::string& reaction) const;


  /**
   * Adds a copy of the given FluxBound object to the list of FluxBounds.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param bound the FluxBound object to be added to the list of FluxBounds.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addFluxBound (const FluxBound* bound);


  /**
   * Creates a new FluxBound object and adds it to the list of FluxBound objects
   * and returns it.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @return a newly created FluxBound object.
   */
  FluxBound* createFluxBound();


  /**
   * Removes the nth FluxBound object from this plugin object and
   * returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   *  deleting it.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param n the index of the FluxBound object to remove.
   *
   * @return the FluxBound object removed.  As mentioned above, the 
   * caller owns the returned object. @c NULL is returned if the 
   * given index is out of range.
   */
  FluxBound* removeFluxBound (unsigned int n);


  /**
   * Removes the FluxBound object with the given @p sid attribute from 
   * this plugin object and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @param sid the id attribute of the FluxBound object to remove.
   *
   * @return the FluxBound object removed.  As mentioned above, the 
   * caller owns the returned object. @c NULL is returned if the 
   * given index is out of range.
   */
  FluxBound* removeFluxBound (const std::string& sid);


  /**
   * Returns the number of FluxBound object in this plugin object.
   *
   * @copydetails note_fluxbound_v1_only
   *
   * @return the number of FluxBound object in this plugin object.
   */
  unsigned int getNumFluxBounds() const;


  /**
   * Returns the ListOfObjectives in this FbcModelPlugin object.
   *
   * @return the ListOfObjectives child of this FbcModelPlugin.
   */
  const ListOfObjectives* getListOfObjectives() const;


  /**
   * Returns the ListOfObjectives in this FbcModelPlugin object.
   *
   * @return the ListOfObjectives child of this FbcModelPlugin.
   */
  ListOfObjectives* getListOfObjectives();


  /**
   * Get an Objective from the ListOfObjectives.
   *
   * @param n the index number of the Objective to get.
   *
   * @return the nth Objective in the ListOfObjectives within this FbcModelPlugin.
   *
   * @see getNumObjectives()
   */
  Objective* getObjective(unsigned int n);


  /**
   * Get an Objective from the ListOfObjectives.
   *
   * @param n the index number of the Objective to get.
   *
   * @return the nth Objective in the ListOfObjectives within this FbcModelPlugin.
   *
   * @see getNumObjectives()
   */
  const Objective* getObjective(unsigned int n) const;


  /**
   * Get an Objective from the ListOfObjectives
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Objective to get.
   *
   * @return the Objective in the ListOfObjectives
   * with the given id or NULL if no such
   * Objective exists.
   *
   * @see getObjective(unsigned int n)
   *
   * @see getNumObjectives()
   */
  Objective* getObjective(const std::string& sid);


  /**
   * Get an Objective from the ListOfObjectives
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Objective to get.
   *
   * @return the Objective in the ListOfObjectives
   * with the given id or NULL if no such
   * Objective exists.
   *
   * @see getObjective(unsigned int n)
   *
   * @see getNumObjectives()
   */
  const Objective* getObjective(const std::string& sid) const;


  /**
   * Adds a copy the given Objective to this FbcModelPlugin.
   *
   * @param o the Objective object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addObjective(const Objective* o);


  /**
   * Get the number of Objective objects in this FbcModelPlugin.
   *
   * @return the number of Objective objects in this FbcModelPlugin.
   */
  unsigned int getNumObjectives() const;


  /**
   * Creates a new Objective object, adds it to this FbcModelPlugin's
   * ListOfObjectives and returns the Objective object created. 
   *
   * @return a new Objective object instance.
   *
   * @see addObjective(const Objective* o)
   */
  Objective* createObjective();


  /**
   * Removes the nth Objective from the ListOfObjectives within this FbcModelPlugin.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Objective to remove.
   *
   * @see getNumObjectives()
   */
  Objective* removeObjective(unsigned int n);


  /**
   * Removes the Objective with the given identifier from the ListOfObjectives within this FbcModelPlugin
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Objective to remove.
   *
   * @return the Objective removed. As mentioned above, the caller owns the
   * returned item.
   */
  Objective* removeObjective(const std::string& sid);


  /** 
   * Returns the current active objective. 
   *
   * @return the Objective pointed to by the 'activeObjective' 
   * attribute, or NULL if no such Objective can be found.
   */
  Objective* getActiveObjective();

  /** 
   * Returns the current active objective. 
   *
   * @return the Objective pointed to by the 'activeObjective' 
   * attribute, or NULL if no such Objective can be found.
   */
  const Objective *getActiveObjective() const;
  
  /** 
   * Sets the id of the active objective.
  *
  * @copydetails doc_returns_success_code
  * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
  * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   */
  int setActiveObjectiveId(const std::string& objectiveId);

  /** 
   * Returns the id of the current active objective.
   *
   * @return the value of the "activeObjective" attribute of the ListOfObjectives.
   */
  std::string getActiveObjectiveId() const;
  
  /** 
   * Unsets the "activeObjective" attribute of the ListOfObjectives.
   */
  void unsetActiveObjectiveId();

  /**
   * Returns the "ListOfGeneProducts" in this FbcModelPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the "ListOfGeneProducts" attribute of this FbcModelPlugin.
   */
  const ListOfGeneProducts* getListOfGeneProducts() const;


  /**
   * Returns the  "ListOfGeneProducts" in this FbcModelPlugin object.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the "ListOfGeneProducts" attribute of this FbcModelPlugin.
   */
  ListOfGeneProducts* getListOfGeneProducts();


  /**
   * Get a GeneProduct from the ListOfGeneProducts.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param n the index number of the GeneProduct to get.
   *
   * @return the nth GeneProduct in the ListOfGeneProducts within this FbcModelPlugin.
   *
   * @see getNumGeneProducts()
   */
  GeneProduct* getGeneProduct(unsigned int n);


  /**
   * Get a GeneProduct from the ListOfGeneProducts.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param n the index number of the GeneProduct to get.
   *
   * @return the nth GeneProduct in the ListOfGeneProducts within this FbcModelPlugin.
   *
   * @see getNumGeneProducts()
   */
  const GeneProduct* getGeneProduct(unsigned int n) const;


  /**
   * Get a GeneProduct from the ListOfGeneProducts
   * based on its identifier.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param sid a string representing the identifier
   * of the GeneProduct to get.
   *
   * @return the GeneProduct in the ListOfGeneProducts
   * with the given id or NULL if no such
   * GeneProduct exists.
   *
   * @see getGeneProduct(unsigned int n)
   *
   * @see getNumGeneProducts()
   */
  GeneProduct* getGeneProduct(const std::string& sid);

  /**
  * Get a GeneProduct from the ListOfGeneProducts
  * based on its label.
  *
   * @copydetails note_geneproduct_v2_only
   *
  * @param label a string representing the label
  * of the GeneProduct to get.
  *
  * @return the GeneProduct in the ListOfGeneProducts
  * with the given label or NULL if no such
  * GeneProduct exists.
  *
  * @see getGeneProduct(unsigned int n)
  *
  * @see getNumGeneProducts()
  */
  GeneProduct* getGeneProductByLabel(const std::string& label);


  /**
   * Get a GeneProduct from the ListOfGeneProducts
   * based on its identifier.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param sid a string representing the identifier
   * of the GeneProduct to get.
   *
   * @return the GeneProduct in the ListOfGeneProducts
   * with the given id or NULL if no such
   * GeneProduct exists.
   *
   * @see getGeneProduct(unsigned int n)
   *
   * @see getNumGeneProducts()
   */
  const GeneProduct* getGeneProduct(const std::string& sid) const;


  /**
   * Adds a copy the given GeneProduct to this FbcModelPlugin.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param gp the GeneProduct object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addGeneProduct(const GeneProduct* gp);


  /**
   * Get the number of GeneProduct objects in this FbcModelPlugin.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return the number of GeneProduct objects in this FbcModelPlugin.
   */
  unsigned int getNumGeneProducts() const;


  /**
   * Creates a new GeneProduct object, adds it to this FbcModelPlugin's
   * ListOfGeneProducts and returns the GeneProduct object created. 
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @return a new GeneProduct object instance.
   *
   * @see addGeneProduct(const GeneProduct* gp)
   */
  GeneProduct* createGeneProduct();


  /**
   * Removes the nth GeneProduct from the ListOfGeneProducts within this FbcModelPlugin.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param n the index of the GeneProduct to remove.
   *
   * @see getNumGeneProducts()
   */
  GeneProduct* removeGeneProduct(unsigned int n);


  /**
   * Removes the GeneProduct with the given identifier from the ListOfGeneProducts within this FbcModelPlugin
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @copydetails note_geneproduct_v2_only
   *
   * @param sid the identifier of the GeneProduct to remove.
   *
   * @return the GeneProduct removed. As mentioned above, the caller owns the
   * returned item.
   */
  GeneProduct* removeGeneProduct(const std::string& sid);

  /**
   * Returns the ListOfGeneAssociations annotation object for level 1 in this plugin object.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @return ListOfGeneAssociations annotation object for level 1 in this plugin object.
   */
  const ListOfGeneAssociations* getListOfGeneAssociations () const;

  /**
   * Returns the ListOfGeneAssociations annotation object for level 1 in this plugin object.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @return ListOfGeneAssociations annotation object for level 1 in this plugin object.
   */
  ListOfGeneAssociations* getListOfGeneAssociations ();

  /**
   * Returns the GeneAssociation annotation object that belongs to the given index. If the
   * index is invalid, @c NULL is returned.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param n the index number of the GeneAssociation annotation to get.
   *
   * @return the nth GeneAssociation annotation in the ListOfGeneAssociations.
   */
  const GeneAssociation* getGeneAssociation (unsigned int n) const;

  /**
   * Returns the GeneAssociation annotation object that belongs to the given index. If the
   * index is invalid, @c NULL is returned.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param n the index number of the GeneAssociation annotation to get.
   *
   * @return the nth GeneAssociation annotation in the ListOfGeneAssociations.
   */
  GeneAssociation* getGeneAssociation (unsigned int n);

  /**
   * Returns the GeneAssociation annotation object based on its identifier.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param sid a string representing the identifier 
   * of the GeneAssociation annotation to get.
   * 
   * @return GeneAssociation annotation in the ListOfGeneAssociations with the given @p sid
   * or NULL if no such GeneAssociation annotation exists.
   *
   * @see getGeneAssociation(unsigned int n)
   * @see getListOfGeneAssociations()
   */
  GeneAssociation* getGeneAssociation (const std::string& sid);

  /**
   * Returns the GeneAssociation annotation object based on its identifier.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param sid a string representing the identifier 
   * of the GeneAssociation annotation to get.
   * 
   * @return GeneAssociation annotation in the ListOfGeneAssociations with the given @p sid 
   * or NULL if no such GeneAssociation annotation exists.
   *
   * @see getGeneAssociation(unsigned int n)
   * @see getListOfGeneAssociations()
   */
  const GeneAssociation* getGeneAssociation (const std::string& sid) const;

  /**
   * Adds a copy of the given GeneAssociation annotation object to the list of GeneAssociations.
   *
   * @param association the GeneAssociation annotation object to be added to the list of GeneAssociations.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   */
  int addGeneAssociation (const GeneAssociation* association);

  /**
   * Creates a new GeneAssociation annotation object and adds it to the list of GeneAssociation objects
   * and returns it.
   *
   * @return a newly created GeneAssociation annotation object.
   */
  GeneAssociation* createGeneAssociation();

  /**
   * Removes the nth GeneAssociation annotation object from this plugin object and
   * returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   *  deleting it.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param n the index of the GeneAssociation annotation object to remove.
   *
   * @return the GeneAssociation annotation object removed.  As mentioned above, the 
   * caller owns the returned object. @c NULL is returned if the 
   * given index is out of range.
   */
  GeneAssociation* removeGeneAssociation (unsigned int n);

  /**
   * Removes the GeneAssociation annotation object with the given @p sid attribute from 
   * this plugin object and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for
   * deleting it.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @param sid the id attribute of the GeneAssociation annotation object to remove.
   *
   * @return the GeneAssociation annotation object removed.  As mentioned above, the 
   * caller owns the returned object. @c NULL is returned if the 
   * given index is out of range.
   */
  GeneAssociation* removeGeneAssociation (const std::string& sid);

  /**
   * Returns the number of GeneAssociation annotation object in this plugin object.
   *
   * @copydetails note_geneassociation_not_fbc
   *
   * @return the number of GeneAssociation annotation object in this plugin object.
   */
  int getNumGeneAssociations() const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
  * Sets the *parent* of this SBML object to child SBML objects (if any).
  * (Creates a child-parent relationship by the parent)
  *
  * @see setSBMLDocument
  * @see enablePackageInternal
  */
  virtual void connectToChild();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual void connectToParent (SBase* sbase);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */
  /** 
   * Parse L2 annotation if supported
   *
   */
  virtual void parseAnnotation(SBase *parentObject, XMLNode *annotation);
  /** @endcond */

  /** @cond doxygenLibsbmlInternal */
  bool          mStrict;
  bool          mIsSetStrict;
  ListOfObjectives   mObjectives;
  ListOfGeneProducts   mGeneProducts;
  ListOfFluxBounds mBounds;
  ListOfGeneAssociations mAssociations;
  /** @endcond */


};




LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Appends a copy of the given FluxBound_t structure to the given FbcModelPlugin_t
 * structure.
 *
 * @param fmp the FbcModelPlugin_t structure to which the FluxBound_t should be
 * added.
 *
 * @param fb a FluxBound_t structure to add.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_addFluxBound(SBasePlugin_t * fmp, FluxBound_t * fb);


/**
 * Return a specific FluxBound_t structure of the given FbcModelPlugin_t.
 *
 * @param fmp the FbcModelPlugin_t structure to use.
 *
 * @param n an integer, the index of the FluxBound_t structure to return.
 * 
 * @return the nth FluxBound_t of the given FbcModelPlugin_t, or @c NULL if no such FluxBound_t exists.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
FluxBound_t *
FbcModelPlugin_getFluxBound(SBasePlugin_t * fmp, unsigned int n);


/**
 * Returns the number of FluxBound_t structures attached to the given
 * FbcModelPlugin_t.
 *
 * @param fmp the FbcModelPlugin_t structure to use.
 * 
 * @return the number of FluxBound_t structures in the given FbcModelPlugin_t.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
unsigned int
FbcModelPlugin_getNumFluxBounds(SBasePlugin_t * fmp);


/**
 * Appends a copy of the given Objective_t structure to the given FbcModelPlugin_t
 * structure.
 *
 * @param fmp the FbcModelPlugin_t structure to which the Objective_t should be
 * added.
 *
 * @param obj an Objective_t structure to add.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_addObjective(SBasePlugin_t * fmp, Objective_t * obj);


/**
 * Return a specific Objective_t structure of the given FbcModelPlugin_t.
 *
 * @param fmp the FbcModelPlugin_t structure to use.
 *
 * @param n an integer, the index of the Objective_t structure to return.
 * 
 * @return the nth Objective_t of the given FbcModelPlugin_t, or @c NULL if no such Objective_t exists.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
Objective_t *
FbcModelPlugin_getObjective(SBasePlugin_t * fmp, unsigned int n);


/**
 * Returns the number of Objective_t structures attached to the given
 * FbcModelPlugin_t.
 *
 * @param fmp the FbcModelPlugin_t structure to use.
 * 
 * @return the number of Objective_t structures in the given FbcModelPlugin_t.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
unsigned int
FbcModelPlugin_getNumObjectives(SBasePlugin_t * fmp);


/**
 * Takes a FbcModelPlugin_t structure and returns the id of the current activeObjective.
 *
 * @param fmp the FbcModelPlugin_t whose id of the current activeObjective is sought.
 *
 * @return the id of the current activeObjective of the given FbcModelPlugin_t, as a pointer to a string.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
char *
FbcModelPlugin_getActiveObjectiveId(SBasePlugin_t * fmp);


/**
 * Sets the activeObjective of the given FbcModelPlugin_t to a copy of @p activeObjective.
 *
 * @param fmp the FbcModelPlugin_t structure to set.
 * @param activeObjective the activeObjective to assign to the given FbcModelPlugin_t's "activeObjective" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "activeObjective" attribute.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_setActiveObjectiveId(SBasePlugin_t * fmp, const char * activeObjective);


/**
 * Appends a copy of the given GeneProduct_t structure to the given FbcModelPlugin_t
 * structure.
 *
 * @param fmp the FbcModelPlugin_t structure to which the GeneProduct_t should be
 * added.
 *
 * @param fb a GeneProduct_t structure to add.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_addGeneProduct(SBasePlugin_t * fmp, GeneProduct_t * fb);


/**
 * Return a specific GeneProduct_t structure of the given FbcModelPlugin_t.
 *
 * @param fmp the FbcModelPlugin_t structure to use.
 *
 * @param n an integer, the index of the GeneProduct_t structure to return.
 * 
 * @return the nth GeneProduct_t of the given FbcModelPlugin_t, or @c NULL if no such GeneProduct_t exists.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
GeneProduct_t *
FbcModelPlugin_getGeneProduct(SBasePlugin_t * fmp, unsigned int n);


/**
 * Returns the number of GeneProduct_t structures attached to the given
 * FbcModelPlugin_t.
 *
 * @param fmp the GeneProduct_t structure to use.
 * 
 * @return the number of EventAssignment_t structures in the given FbcModelPlugin_t.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
unsigned int
FbcModelPlugin_getNumGeneProducts(SBasePlugin_t * fmp);


/**
 * Takes a FbcModelPlugin_t structure and returns the value of the strict attribute.
 *
 * @copydetails note_strict_v2_only
 *
 * @param fmp the FbcModelPlugin_t whose 'strict' attribute is sought.
 *
 * @return the id of the current activeObjective of the given FbcModelPlugin_t, as a pointer to a string.
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_getStrict(SBasePlugin_t * fmp);


/**
 * Sets the strict attribute of the given FbcModelPlugin_t.
 *
 * @copydetails note_strict_v2_only
 *
 * @param fmp the FbcModelPlugin_t structure to set.
 * @param activeObjective the activeObjective to assign to the given FbcModelPlugin_t's "activeObjective" attribute.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @memberof FbcModelPlugin_t
 */
LIBSBML_EXTERN
int
FbcModelPlugin_setStrict(SBasePlugin_t * fmp, int strict);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif /* FbcModelPlugin_H__ */


