/**
 * @file:   UncertMLNode.h
 * @brief:  Implementation of the UncertMLNode class
 * @author: Sarah Keating
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */


#ifndef UncertMLNode_H__
#define UncertMLNode_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/distrib/common/distribfwd.h>


#ifdef __cplusplus


#include <string>
#include <vector>

#include <sbml/packages/distrib/extension/DistribExtension.h>
#include <sbml/packages/distrib/sbml/DistribInput.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN UncertMLNode
{

protected:

  std::string   mElementName;
  std::string   mText;
  XMLAttributes mAttributes;
  std::vector<UncertMLNode*> mChildren;


public:

  /**
   * Creates a new UncertMLNode
   */
  UncertMLNode();


  /**
   * Creates a new UncertMLNode from an XMLNode.
   *
   * @param xml the XMLNode object
   */
  UncertMLNode(XMLNode * xml);


  /**
   * Copy constructor for UncertMLNode.
   *
   * @param orig; the UncertMLNode instance to copy.
   */
  UncertMLNode(const UncertMLNode& orig);


  /**
   * Assignment operator for UncertMLNode.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  UncertMLNode& operator=(const UncertMLNode& rhs);


  /**
   * Creates and returns a deep copy of this UncertMLNode object.
   *
   * @return a (deep) copy of this UncertMLNode object.
   */
  UncertMLNode* clone () const;


  /**
   * Destructor for UncertMLNode.
   */
  virtual ~UncertMLNode();


  /**
   * Returns the value of the "elementName" attribute of this UncertMLNode.
   *
   * @return the value of the "elementName" attribute of this 
   * UncertMLNode as a string.
   */
  const std::string& getElementName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * UncertMLNode's "elementName" attribute has been set.
   *
   * @return @c true if this UncertMLNode's "elementName" 
   * attribute has been set, otherwise @c false is returned.
   */
  bool isSetElementName() const;


  /**
   * Sets the value of the "elementName" attribute of this UncertMLNode.
   *
   * @param elementName; const std::string& value of the "elementName" 
   * attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int setElementName(const std::string& elementName);


  /**
   * Unsets the value of the "elementName" attribute of this UncertMLNode.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  int unsetElementName();


  /**
   * Returns the value of the "text" element of this UncertMLNode.
   *
   * @return the value of the "text" element of this 
   * UncertMLNode as a string.
   */
  const std::string& getText() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * UncertMLNode's "text" element has been set.
   *
   * @return @c true if this UncertMLNode's "text" 
   * element has been set, otherwise @c false is returned.
   */
  bool isSetText() const;


  /**
   * Sets the value of the "text" element of this UncertMLNode.
   *
   * @param text; const std::string& value of the "text" 
   * element to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int setText(const std::string& text);


  /**
   * Unsets the value of the "text" element of this UncertMLNode.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  int unsetText();


  /**
   * Returns the XMLAttributes associated with this UncertMLNode object.
   *
   * @return the XMLAttributes object of this UncertMLNode object.
   */
  const XMLAttributes& getAttributes() const;


  /**
   * Returns the number of XMLAttributes associated with this 
   * UncertMLNode object.
   *
   * @return the number XMLAttributes object of this UncertMLNode object 
   * as an unsigned int.
   */
  unsigned int getNumAttributes() const;


  /**
   * Sets the value of the XMLAttributes for this UncertMLNode.
   *
   * @param attributes; XMLAttributes & for the attributes to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int setAttributes(const XMLAttributes & attr);


  /**
   * Unsets the value of the XMLAttributes of this UncertMLNode.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  int unsetAttributes();

  /**
   * Get a child of this node according to its index number.
   *
   * @param n the index of the child to get
   * 
   * @return the nth child of this UncertMLNode or @c NULL if this node has no nth
   * child (<code>n &gt; </code>
   * @if clike getNumChildren()@else UncertMLNode::getNumChildren()@endif@~
   * <code>- 1</code>).
   */
   UncertMLNode * getChild(unsigned int index) const;


  /**
   * Get the number of children that this node has.
   * 
   * @return the number of children of this UncertMLNode, or 0 is this node has
   * no children.
   */
  unsigned int getNumChildren () const;

   
  /**
   * Adds the given node as a child of this UncertMLNode.  Child nodes are added
   * in-order, from left to right.
   *
   * @param child the UncertMLNode instance to add
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED @endlink
   */
  int addChild (UncertMLNode* child);


  /**
   * Predicate returning @c true if all the required attributes
   * for this UncertMLNode object have been set.
   *
   * @note The required attributes for a UncertMLNode object are:
   * @li "elementName"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
  */
  bool hasRequiredAttributes() const;



  /**
   * Outputs a string representation of the XML represented by 
   * this UncertMLNode object
   *
   * Helper function - primarily for debugging
   *
   * @return string representation of the XML of this UncertMLNode.
   */
  std::string toXMLString() const;


  /**
   * Creates an UncertMLNode representing the statistical elements
   * listed.
   *
   * @param arguments - a comma seperated list of the statistical elements
   * @param argumentsIds - a comma seperated list of the ids to apply
   * 
   * Example usage would be
   *            createStatisticsNode("Mean,Variance", "mu, sigma")
   * which would produce a node representing the xml:
   *
   * <UncertML xmlns=\"http://www.uncertml.org/3.0\">
   *   <StatisticsCollection definition=\"http://www.uncertml.org/statistics\">
   *     <Mean definition=\"http://www.uncertml.org/statistics\">
   *       <value>
   *         <var varId=\"V_pop\"/>
   *       </value>
   *     </Mean>
   *     <Variance definition=\"http://www.uncertml.org/statistics\">
   *       <value>
   *         <var varId=\"V_omega\"/>
   *       </value>
   *     </Variance>
   *   </StatisticsCollection>
   * </UncertML>;
   *
   * @returns an UncertMLNode representing the statistics
   */
  static UncertMLNode * createStatisticsNode(std::string arguments, 
                                             std::string argumentIds); 


  /**
   * Creates an UncertMLNode representing the distribution with arguments
   * listed.
   *
   * @param name - name of the distribution to create
   * @param arguments - a comma seperated list of the argument names
   * @param argumentsIds - a comma seperated list of the ids to apply
   * 
   * Example usage would be
   *            createDistributionNode("NormalDistribution", 
   *                                   "mean,variance", "mu, sigma")
   * which would produce a node representing the xml:
   *
   * <UncertML xmlns=\"http://www.uncertml.org/3.0\">
   *  <NormalDistribution definition=\"http://www.uncertml.org/distributions\">
   *    <mean>
   *      <var varId=\"mu\"/>
   *    </mean>
   *    <variance>
   *      <var varId=\"sigma\"/>
   *    </variance>
   *  </NormalDistribution>
   * </UncertML>;
   *
   * @returns an UncertMLNode representing the distribution
   */
  static UncertMLNode * createDistributionNode(std::string name, 
    std::string arguments, std::string argumentIds); 

  /**
   * Creates an UncertMLNode representing the distribution with arguments
   * listed.
   *
   * @param name - name of the distribution to create
   * @param arguments - a comma seperated list of the argument names
   * @param argumentsIds - a comma seperated list of the values to apply
   * @param argumentElementNames - a comma seperated list of the elements to use, or 'varId' for the attribute
   * 
   * Example usage would be
   *     createDistributionNodeWithValues("NormalDistribution", 
   *                  "mean,variance", "4, 0.9", "rVal, prVal")
   * which would produce a node representing the xml:
   *
   * <UncertML xmlns=\"http://www.uncertml.org/3.0\">
   *  <NormalDistribution definition=\"http://www.uncertml.org/distributions\">
   *    <mean>
   *      <var> <rVal> 4 </rVal> </var>
   *    </mean>
   *    <variance>
   *      <var> <prVal> 0.9 </prVal> </var>
   *    </variance>
   *  </NormalDistribution>
   * </UncertML>;
   *
   * @returns an UncertMLNode representing the distribution
   */
  static UncertMLNode * createDistributionNodeWithValues(std::string name, 
    std::string arguments, std::string argumentValues, std::string argumentElementNames); 

protected:

  /** @cond doxygenLibsbmlInternal */

  /*
   * parse the information from the XMLNode into the uncertMLNode
   * return boolean indicating success/failure
   */
  bool parseXMLNode(const XMLNode* xml);

  /** @endcond doxygenLibsbmlInternal */

  /** @cond doxygenLibsbmlInternal */

  /*
   * write the uncertMLNode to the output stream
   * at present this creates an XMLNode out of the uncertMLNode
   * and writes that out 
   */
  void write(XMLOutputStream & ostream) const;

  /** @endcond doxygenLibsbmlInternal */

  /** @cond doxygenLibsbmlInternal */

  /* 
   * creates an XMLNode with the UncertML element top-level element
   */
  XMLNode * constructXMLNode() const;

  /** @endcond doxygenLibsbmlInternal */

  /** @cond doxygenLibsbmlInternal */

  /*
   * Recreate an XMLNode from an uncertMLNode
   *
   * called recursively
   *
   * used by constructXMLNode
   */
  XMLNode * reconstructXML() const;

  /** @endcond doxygenLibsbmlInternal */

  /** @cond doxygenLibsbmlInternal */

  /*
  * some of these functions are hacks - so dont want them to be accessible
   *
   * so need some firends
   */
  friend class DrawFromDistribution;
  friend class Uncertainty;

   
   /** @endcond doxygenLibsbmlInternal */

  


};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSBML_EXTERN
UncertMLNode_t *
UncertMLNode_create();


LIBSBML_EXTERN
void
UncertMLNode_free(UncertMLNode_t * umln);


LIBSBML_EXTERN
UncertMLNode_t *
UncertMLNode_clone(UncertMLNode_t * umln);


LIBSBML_EXTERN
char *
UncertMLNode_getElementName(UncertMLNode_t * umln);


LIBSBML_EXTERN
const XMLAttributes_t *
UncertMLNode_getAttributes(UncertMLNode_t * umln);


LIBSBML_EXTERN
int
UncertMLNode_isSetElementName(UncertMLNode_t * umln);


LIBSBML_EXTERN
unsigned int
UncertMLNode_getNumAttributes(UncertMLNode_t * umln);


LIBSBML_EXTERN
int
UncertMLNode_setElementName(UncertMLNode_t * umln, const char * elementName);


LIBSBML_EXTERN
int
UncertMLNode_setAttributes(UncertMLNode_t * umln, XMLAttributes_t* attributes);


LIBSBML_EXTERN
int
UncertMLNode_unsetElementName(UncertMLNode_t * umln);


LIBSBML_EXTERN
int
UncertMLNode_unsetAttributes(UncertMLNode_t * umln);


LIBSBML_EXTERN
int
UncertMLNode_hasRequiredAttributes(UncertMLNode_t * umln);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  UncertMLNode_H__  */

