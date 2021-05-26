/**
 * @file    renderfwd.h
 * @brief   SBML RENDER C structure declarations
 * @author  Frank T. Bergmann
 *
 * $Id$
 * $HeadURL$
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright 2009-2013 California Institute of Technology.
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *------------------------------------------------------------------------- -->
 */


#ifndef renderfwd_h__
#define renderfwd_h__


#include <sbml/common/libsbml-namespace.h>
#include <sbml/common/libsbml-config.h>

/**
 * Forward declaration of all opaque C types.
 *
 * Declaring all types up-front avoids "redefinition of type 'Foo'" compile
 * errors and allows our combined C/C++ headers to depend minimally upon
 * each other.  Put another way, the type definitions below serve the same
 * purpose as "class Foo;" forward declarations in C++ code.
 */

#ifdef __cplusplus
#  define CLASS_OR_STRUCT class
#else
#  define CLASS_OR_STRUCT struct
#endif  /* __cplusplus */

LIBSBML_CPP_NAMESPACE_BEGIN

typedef CLASS_OR_STRUCT ColorDefinition         ColorDefinition_t;
typedef CLASS_OR_STRUCT Ellipse                 Ellipse_t;
typedef CLASS_OR_STRUCT GlobalRenderInformation GlobalRenderInformation_t;
typedef CLASS_OR_STRUCT GlobalStyle             GlobalStyle_t;
typedef CLASS_OR_STRUCT GradientBase            GradientBase_t;
typedef CLASS_OR_STRUCT GradientStop            GradientStop_t;
typedef CLASS_OR_STRUCT GraphicalPrimitive1D    GraphicalPrimitive1D_t;
typedef CLASS_OR_STRUCT GraphicalPrimitive2D    GraphicalPrimitive2D_t;
typedef CLASS_OR_STRUCT RenderGroup             RenderGroup_t;
typedef CLASS_OR_STRUCT Image                   Image_t;
typedef CLASS_OR_STRUCT LinearGradient          LinearGradient_t;
typedef CLASS_OR_STRUCT LineEnding              LineEnding_t;
typedef CLASS_OR_STRUCT ListOfCurveElements     ListOfCurveElements_t;
typedef CLASS_OR_STRUCT LocalRenderInformation  LocalRenderInformation_t;
typedef CLASS_OR_STRUCT LocalStyle              LocalStyle_t;
typedef CLASS_OR_STRUCT Polygon                 Polygon_t;
typedef CLASS_OR_STRUCT RadialGradient          RadialGradient_t;
typedef CLASS_OR_STRUCT Rectangle               Rectangle_t;
typedef CLASS_OR_STRUCT RelAbsVector            RelAbsVector_t;
typedef CLASS_OR_STRUCT RenderCubicBezier       RenderCubicBezier_t;
typedef CLASS_OR_STRUCT RenderCurve             RenderCurve_t;
typedef CLASS_OR_STRUCT RenderInformationBase   RenderInformationBase_t;
typedef CLASS_OR_STRUCT RenderPoint             RenderPoint_t;
typedef CLASS_OR_STRUCT Style                   Style_t;
typedef CLASS_OR_STRUCT Text                    Text_t;
typedef CLASS_OR_STRUCT RenderPoint             RenderPoint_t;
typedef CLASS_OR_STRUCT Transformation          Transformation_t;
typedef CLASS_OR_STRUCT Transformation2D        Transformation2D_t;

LIBSBML_CPP_NAMESPACE_END

#undef CLASS_OR_STRUCT


#endif  /* renderfwd_h__ */
