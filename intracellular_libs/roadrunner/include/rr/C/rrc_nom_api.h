/**
 * @file rrc_nom_api.h
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
#ifndef rrc_nom_apiH
#define rrc_nom_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif
//---------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Network Object Model (NOM) library forwarded functions
// --------------------------------------------------------------------------------

/*!
 \brief Returns the number of rules in the current model
 \param[in] handle Handle to a RoadRunner instance
 \return Returns an integer larger or equal to 0 if successful, or -1 on failure
 \ingroup NOM functions
*/
C_DECL_SPEC int rrcCallConv getNumberOfRules(RRHandle handle);

/*!
 \brief Returns the name of currently loaded SBML model
 \param[in] handle Handle to a RoadRunner instance
 \return Returns a char* containing the name if successful, NULL otherwise
 \ingroup NOM functions
*/
C_DECL_SPEC char* rrcCallConv getModelName(RRHandle handle);


//---------------------------------------------------------------------------
#if defined(__cplusplus)
}	//Extern "C"
}	//rrc namespace
#endif

#endif
