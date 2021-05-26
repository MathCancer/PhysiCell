/**
 * @file rrc_exporter.h
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/

#ifndef rrc_exporterH
#define rrc_exporterH

//Export/Import API functions
#if defined(_WIN32) || defined(__WIN32__)
    #if defined(STATIC_RRC)
        #define C_DECL_SPEC
    #else
        #if defined(EXPORT_RRC)
            #define C_DECL_SPEC __declspec(dllexport)
        #else
            #define C_DECL_SPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define C_DECL_SPEC
#endif


//C function calling conventions
#if defined(_MSC_VER)
    #define rrcCallConv __cdecl
#elif defined(__CODEGEARC__)
    #define rrcCallConv __cdecl
#else
    #define rrcCallConv
#endif

#if !defined(__cplusplus)
    #if !defined(_MSC_VER)
        #include "stdbool.h"
    #else   //VS don't have stdbool.h
        /*!
         \brief Typedef for boolean type in C
        */
        typedef int bool;
        #define false 0
        #define true 1
    #endif
#endif

#endif
