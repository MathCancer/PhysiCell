/**
 * @file    RenderExtensionTypes_h
 * @brief   Include all SBML types of render extension in a single header file.
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
 *----------------------------------------------------------------------- -->*/

#ifndef RenderExtensionTypes_h
#define RenderExtensionTypes_h

#include <sbml/packages/render/common/renderfwd.h>

#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/packages/render/extension/RenderLayoutPlugin.h>
#include <sbml/packages/render/extension/RenderListOfLayoutsPlugin.h>
#include <sbml/packages/render/extension/RenderGraphicalObjectPlugin.h>

#include <sbml/packages/render/sbml/ColorDefinition.h>
#include <sbml/packages/render/sbml/Ellipse.h>
#include <sbml/packages/render/sbml/GlobalRenderInformation.h>
#include <sbml/packages/render/sbml/GlobalStyle.h>
#include <sbml/packages/render/sbml/GradientBase.h>
#include <sbml/packages/render/sbml/GradientStop.h>
#include <sbml/packages/render/sbml/GraphicalPrimitive1D.h>
#include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
#include <sbml/packages/render/sbml/RenderGroup.h>
#include <sbml/packages/render/sbml/Image.h>
#include <sbml/packages/render/sbml/LinearGradient.h>
#include <sbml/packages/render/sbml/LineEnding.h>
#include <sbml/packages/render/sbml/ListOfCurveElements.h>
#include <sbml/packages/render/sbml/LocalRenderInformation.h>
#include <sbml/packages/render/sbml/LocalStyle.h>
#include <sbml/packages/render/sbml/Polygon.h>
#include <sbml/packages/render/sbml/RadialGradient.h>
#include <sbml/packages/render/sbml/Rectangle.h>
#include <sbml/packages/render/sbml/RelAbsVector.h>
#include <sbml/packages/render/sbml/RenderCubicBezier.h>
#include <sbml/packages/render/sbml/RenderCurve.h>
#include <sbml/packages/render/sbml/RenderInformationBase.h>
#include <sbml/packages/render/sbml/RenderPoint.h>
#include <sbml/packages/render/sbml/Style.h>
#include <sbml/packages/render/sbml/Text.h>
#include <sbml/packages/render/sbml/Transformation.h>
#include <sbml/packages/render/sbml/Transformation2D.h>

#endif  /* RenderExtensionTypes_h */
