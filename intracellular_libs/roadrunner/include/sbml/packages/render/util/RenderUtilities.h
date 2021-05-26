/**
 * @file    RenderUtilities.h
 * @brief   Definition of RenderUtilities, a class of utility functions for the render package
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
 */

#ifndef RenderUtilities_h
#define RenderUtilities_h

#include <sbml/common/extern.h>
#include <sbml/packages/render/common/RenderExtensionTypes.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class ListOfLayouts;
class Layout;
class XMLNode;

/*
 * This method adds a correction term to text elements from the old spec so that the text placement
 * is improved.
 */
LIBSBML_EXTERN void  fixTextElements(RenderGroup* pGroup,RelAbsVector fontSize=RelAbsVector(0.0,0.0));

/*
 * This method adds a correction term to text elements from the old spec so that the text placement
 * is improved.
 */
LIBSBML_EXTERN void fixTextElements(RenderInformationBase* pRenderInfo);

/*
 * This method adds a correction term to text elements from the old spec so that the text placement
 * is improved.
 */
LIBSBML_EXTERN void  fixTextElements(LocalRenderInformation* pRenderInfo);

/*
 * This method adds a correction term to text elements from the old spec so that the text placement
 * is improved.
 */
LIBSBML_EXTERN void fixTextElements(GlobalRenderInformation* pRenderInfo);

/*
 * takes an annotation that has been read into the model
 * identifies the listOfLayouts element and creates a List of 
 * Layouts from the annotation
 */
LIBSBML_EXTERN void parseGlobalRenderAnnotation(XMLNode * annotation, ListOfLayouts* pLOL);

/*
 * Takes an XMLNode and tries to find the render information annotation node and deletes it if it was found.
 */
LIBSBML_EXTERN XMLNode* deleteGlobalRenderAnnotation(XMLNode* pAnnotation);

/*
 * Creates an XMLNode that represents the layouts of the model from the given Model object.
 */
LIBSBML_EXTERN XMLNode* parseGlobalRenderInformation(const ListOfLayouts* pList);

LIBSBML_EXTERN XMLNode* deleteLocalRenderAnnotation(XMLNode* pAnnotation);

LIBSBML_EXTERN XMLNode* parseLocalRenderInformation(const Layout* pLayout);

LIBSBML_EXTERN void parseLocalRenderAnnotation(XMLNode * annotation, Layout* pLayout);


LIBSBML_CPP_NAMESPACE_END


#endif
