/**
 * @file rrc_api.h
 * @brief libRoadRunner C API 2012-2013
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of libRoadRunner.
 * See http://code.google.com/p/roadrunnerlib/ for more details.
 *
 * Copyright (C) 2012
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
#ifndef rrc_apiH
#define rrc_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
#include "rrc_utilities.h"
#include "rrc_logging_api.h"
#include "rrc_nom_api.h"
#include "rrc_libstruct_api.h"

#if defined(__cplusplus)
namespace rrc { extern "C" {
#endif

/*!
 \brief Initialize a new roadRunner instance and return a handle to it.
 \return Returns a RoadRunner instance, returns null if it fails
 \ingroup initialization
*/
C_DECL_SPEC RRHandle rrcCallConv createRRInstance(void);

/*!
 \brief Initialize a new roadRunner instance and return a handle to it.
 \param[in] tempFolder set roadrunners temporary folder
 \param[in] compiler may be NULL, if NULL, uses default compiler.
 If LLVM build is enabled, setting compiler to "llvm" enables llvm based
 model generation.
 \return Returns a RoadRunner instance, returns null if it fails
 \ingroup initialization
*/

C_DECL_SPEC RRHandle rrcCallConv createRRInstanceEx(const char* tempFolder, const char* compiler);

/*!
 \brief Free the roadRunner instance
 \param[in] handle Handle to a RoadRunner instance
 \ingroup initialization
*/
C_DECL_SPEC bool rrcCallConv freeRRInstance(RRHandle handle);

/*!
 \brief Returns the folder in which the RoadRunner API is installed
 \return Pointer to string holding the install folder
 \ingroup initialization
*/
C_DECL_SPEC char* rrcCallConv  getInstallFolder(void);

/*!
 \brief Set the internal string containing the folder in where the RoadRunner C API is installed
 \param[in] folder Pointer to string holding the install folder
 \return Boolean indicating success
 \ingroup initialization
*/
C_DECL_SPEC bool  rrcCallConv  setInstallFolder(const char* folder);

/*!
 \brief Retrieve the current version number of the C API library
 \return Returns null if it fails, otherwise it returns the version number of the C API library
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getAPIVersion(void);

/*!
 \brief Retrieve the current version number of the C++ API (Core RoadRunner API) library
 \param[in] handle RoadRunner instance handle
 \return Returns null if it fails, otherwise it returns the version number of the C++ API library
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getCPPAPIVersion(RRHandle handle);

/*!
 Get the version number.

 \return the roadrunner version number in the form or 102030 if the number
 is 1.2.3

 returns the individual version numbers as XXYYZZ where XX
 is the major version, YY the minor and ZZ the revision,
 eg 10000, or 10100, 20000 etc
 */
C_DECL_SPEC int  rrcCallConv getVersion();

/*!
 returns roadrunner as a string, i.e. "1.0.0"
 */
C_DECL_SPEC char*  rrcCallConv getVersionStr();


/*!
 returns something like "1.0.0; compiled with clang "3.3 (tags/RELEASE_33/final)" on date Dec  8 2013, 17:24:57'
 */
C_DECL_SPEC char*  rrcCallConv getVersionEx();

/*!
 \brief Retrieve extended API info.
 \return Returns null if it fails, otherwise it returns a string with the info
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getExtendedAPIInfo();

/*!
 \brief Retrieve the current build date of the library
 \return Returns null if it fails, otherwise it returns the build date
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getBuildDate(void);

/*!
 \brief Retrieve the current build time (HH:MM:SS) of the library
 \return Returns null if it fails, otherwise it returns the build time
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getBuildTime(void);

/*!
 \brief Retrieve the current build date + time of the library
 \return Returns null if it fails, otherwise it returns the build date + time
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getBuildDateTime(void);

/*!
 \brief Retrieve the current copyright notice for the library
 \return Returns null if it fails, otherwise it returns the copyright string
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getCopyright(void);

/*!
 \brief Retrieve info about current state of roadrunner, e.g. loaded model, conservationAnalysis etc.
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns a string with the info
 \ingroup utility
*/
C_DECL_SPEC char*  rrcCallConv getInfo(RRHandle handle);

 /*!
 \brief Retrieve the current version number of the libSBML library
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns the version number of the library
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getlibSBMLVersion(RRHandle handle);

 /*!
 \brief Set the path to the temporary folder where the C code will be stored

 When RoadRunner is run in C generation mode its uses a temporary folder to store the
 generated C source code. This method can be used to set the temporary folder path if necessary.
 \param[in] handle Handle to a RoadRunner instance
 \param[in] folder Pointer to string holding folder path

 \return Returns true if successful
 \ingroup utility                                                 onsole
*/
C_DECL_SPEC bool rrcCallConv setTempFolder(RRHandle handle, const char* folder);

/*!
 \brief Retrieve the current temporary folder path

 When RoadRunner is run in C generation mode its uses a temporary folder to store the
 generate C source code. This method can be used to get the current value
 for the temporary folder path.
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns the path
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getTempFolder(RRHandle handle);

/*!
 \brief Retrieve the current working directory path

 \return Returns null if it fails, otherwise it returns the path
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getWorkingDirectory(void);

/*!
 \brief Retrieve the directory path of the shared rrCApi library

 \return Returns null if it fails, otherwise it returns the path
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getRRCAPILocation(void);


/*!
 \brief Set the path and filename to the compiler to be used by roadrunner.
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fNameWithPath Pointer to string holding the fileName and path to a compiler
 \return Returns true if successful
 \ingroup utility
*/
C_DECL_SPEC bool rrcCallConv setCompiler(RRHandle handle, const char* fNameWithPath);

/*!
 \brief Get the name of the compiler currently being used by roadrunner.
 \ingroup utility
*/
C_DECL_SPEC char* rrcCallConv getCompiler(RRHandle handle);

/*!
 \brief Set the path to a folder containing the compiler to be used.
 \param[in] handle Handle to a RoadRunner instance

 \param[in] folder Pointer to string holding the path to a compiler
 \return Returns true if successful
 \ingroup utility
*/
C_DECL_SPEC bool rrcCallConv setCompilerLocation(RRHandle handle, const char* folder);

/*!
 \brief Get the path to a folder containing the compiler being used.
 \param[in] handle Handle to a RoadRunner instance

 \return Returns the path if successful, NULL otherwise
 \ingroup utility
*/

C_DECL_SPEC char* rrcCallConv getCompilerLocation(RRHandle handle);

/*!
 \brief Set the path to a folder containing support code for model generation.
 \param[in] handle Handle to a RoadRunner instance
 \param[in] folder Pointer to string holding the path to the support code folder
 \return Returns true if successful
 \ingroup utility
*/
C_DECL_SPEC bool rrcCallConv setSupportCodeFolder(RRHandle handle, const char* folder);

/*!
 \brief Get the path to a folder containing support code.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns the path if successful, NULL otherwise
 \ingroup utility
*/

C_DECL_SPEC char* rrcCallConv getSupportCodeFolder(RRHandle handle);

/*!
 \brief Set the runtime generation option [Not yet implemented]

 RoadRunner can either execute a model by generating, compiling and linking self-generated
 C code or it can employ an internal interpreter to evaluate the model equations. The
 later method is useful when the OS forbids the compiling of externally generated code.

 \param[in] handle Handle to a RoadRunner instance
 \param[in] mode is set to 0 RoadRunner generates C Code,
 if set to 1 RoadRunner uses its internal math interpreter.
 \return Returns false if it fails,
 \ingroup utility
*/
C_DECL_SPEC bool rrcCallConv setCodeGenerationMode(RRHandle handle, int mode);


/*!
 \brief Check if there is an error string to retrieve

 Example: status = hasError (void)

 \return Returns true if there is an error waiting to be retrieved
 \ingroup errorfunctions
*/
C_DECL_SPEC bool rrcCallConv hasError(void);

/*!
 \brief Retrieve the current error string

 Example: \code str = getLastError (void); \endcode

 \return Return null if fails, otherwise returns a pointer to the error string
 \ingroup errorfunctions
*/
C_DECL_SPEC char* rrcCallConv getLastError(void);

// Flags/Options
/*!
 \brief Enable or disable conservation analysis
 \param[in] handle Handle to a RoadRunner instance
 \param[in] On_Or_Off Set true to switch on conservation analysis
 \return Returns true if successful
 \ingroup initialization
*/
C_DECL_SPEC bool rrcCallConv setComputeAndAssignConservationLaws(RRHandle handle, const bool On_Or_Off);

/*!
 \brief Get the value of the moiety conservation setting
 \param[in] handle Handle to a RoadRunner instance
 \param[out] value A place to store the value
 \return Returns true if successful
 \ingroup initialization
*/
C_DECL_SPEC int rrcCallConv getComputeAndAssignConservationLaws(RRHandle handle, int* value);

// -----------------------------------------------------------------------
// Read and Write models
// -----------------------------------------------------------------------

/*!
 \brief Load a model from an SBML string
 \param[in] handle Handle to a RoadRunner instance
 \param[in] sbml string
 \return Returns true if successful
 \ingroup loadsave
*/
C_DECL_SPEC bool rrcCallConv loadSBML(RRHandle handle, const char* sbml);

/*!
 \brief Load a model from an SBML string
 \param[in] handle Handle to a RoadRunner instance
 \param[in] sbml string
 \param[in] forceRecompile boolean. True means the model is recompiled. False causes roadrunner to use an already compiled model
 \return Returns true if successful
 \ingroup loadsave
*/
C_DECL_SPEC bool rrcCallConv loadSBMLEx(RRHandle handle, const char* sbml, bool forceRecompile);

/*!
 \brief Load a model from a SBML file
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \return Returns true if successful
 \ingroup loadsave
*/
//C_DECL_SPEC bool rrcCallConv loadSBMLFromFile(const char* fileName);
C_DECL_SPEC bool rrcCallConv loadSBMLFromFile(RRHandle handle, const char* fileName);

/*!
 \brief Load a model from a SBML file, force recompilation
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fileName file name (or full path) to file that holds the SBML model
 \param[in] forceRecompile Boolean that forces recompilation if true. If false, no compilation occur if model dll exists
 \return Returns true if successful
 \ingroup loadsave
*/
C_DECL_SPEC bool rrcCallConv loadSBMLFromFileE(RRHandle handle, const char* fileName, bool forceRecompile);

/*!
 \brief Unload current model
 \param[in] handle Handle to a RoadRunner instance
 \return Returns true if successful
 \ingroup loadsave
*/
C_DECL_SPEC bool rrcCallConv clearModel(RRHandle handle);

/*!
 \brief check if a model is loaded
 \param[in] handle Handle to a RoadRunner instance
 \return Returns true/false indicating whether a model is loaded or not
 \ingroup loadsave
*/
C_DECL_SPEC bool rrcCallConv isModelLoaded(RRHandle handle);

/*!
 \brief Load simulation settings from a file
 \param[in] handle Handle to a RoadRunner instance
 \param[in] fileName file name (or full path) to file that holds simulation settings
 \return Returns true if successful
 \ingroup loadsave
*/

C_DECL_SPEC bool rrcCallConv loadSimulationSettings(RRHandle handle, const char* fileName);

/*!
 \brief Retrieve the <b>current state</b> of the model in the form of an SBML string
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if the call fails, otherwise returns a pointer to the SBML string
 \ingroup loadsave
*/
C_DECL_SPEC char* rrcCallConv getCurrentSBML(RRHandle handle);

/*!
 \brief Retrieve the SBML model that was last loaded into roadRunner
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if the call fails, otherwise returns a pointer to the SBML string
 \ingroup loadsave
*/
C_DECL_SPEC char* rrcCallConv getSBML(RRHandle handle);

// -------------------------------------------------------------------------
// SBML utility methods
// -----------------------------------------------------------------------
/*!
 \brief Promote any local parameters to global status.

 This routine will convert any local reaction parameters and promote
 them to global status. The promoted parameters are prefixed with the
 name of the reaction to make them unique.

 \param[in] handle Handle to a RoadRunner instance
 \param[in] sArg points to the SBML model to promote
 \return Returns null if it fails otherwise it returns the promoted SBML model as a string
 \ingroup sbml
*/
C_DECL_SPEC char* rrcCallConv getParamPromotedSBML(RRHandle handle, const char* sArg);

/*!
 \brief Set the simulator's capabilities
 \param[in] handle Handle to a RoadRunner instance
 \param[out] caps An XML string that specifies the simulators capabilities
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setConfigurationXML (RRHandle handle, const char* caps);

/*!
 \brief Get the simulator's capabilities

 Example:

 \code
 <caps name="RoadRunner" description="Settings For RoadRunner">
  <section name="integration" method="CVODE" description="CVODE Integrator">
    <cap name="BDFOrder" value="5" hint="Maximum order for BDF Method" type="integer" />
    <cap name="AdamsOrder" value="12" hint="Maximum order for Adams Method" type="integer" />
    <cap name="rtol" value="1E-06" hint="Relative Tolerance" type="double" />
    <cap name="atol" value="1E-12" hint="Absolute Tolerance" type="double" />
    <cap name="maxsteps" value="10000" hint="Maximum number of internal stepsc" type="integer" />
    <cap name="initstep" value="0" hint="the initial step size" type="double" />
    <cap name="minstep" value="0" hint="specifies a lower bound on the magnitude of the step size." type="double" />
    <cap name="maxstep" value="0" hint="specifies an upper bound on the magnitude of the step size." type="double" />
    <cap name="conservation" value="1" hint="enables (=1) or disables (=0) the conservation analysis of models for timecourse simulations." type="int" />
    <cap name="allowRandom" value="1" hint="if enabled (=1), reinterprets certain function definitions as distributions and draws random numbers for it." type="int" />
    <cap name="usekinsol" value="0" hint="Is KinSol used as steady state integrator" type="int" />
  </section>

  <section name="SteadyState" method="NLEQ2" description="NLEQ2 Steady State Solver">
    <cap name="MaxIterations" value="100" hint="Maximum number of newton iterations" type="integer" />
    <cap name="relativeTolerance" value="0.0001" hint="Relative precision of solution components" type="double" />
  </section>
</caps>
\endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns the simulator's capabilities in the form of an XML string
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getConfigurationXML(RRHandle handle);

/* Registered Integrators *****************************************************/

/*!
\brief Get the number of registered integrators.
\return Returns an integer that corresponds to the number of currently registered integrators.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getNumRegisteredIntegrators ();

/*!
\brief Get the name of a registered integrator (e.g. cvode etc.)
\param[in] n The index of the registered integrator
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredIntegratorName (int n);

/*!
\brief Get the hint of a registered integrator (e.g. cvode etc.)
\param[in] n The index of the registered integrator
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredIntegratorHint (int n);

/*!
\brief Get the description of a registered integrator (e.g. cvode etc.)
\param[in] n The index of the registered integrator
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredIntegratorDescription (int n);

/* Instantiated Integrators ***************************************************/

/*!
\brief Get the number of instantiated integrators.
\description To instantiate an integrator, use @ref setCurrentIntegrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns an integer that corresponds to the number of currently registered integrators.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getNumInstantiatedIntegrators (RRHandle handle);

/*!
\brief Specify the current integrator to be used for simulation.
\description This method instantiates a new integrator of the given type (e.g. cvode, gillespie) if
one does not currently exist. Otherwise, the existing integrator of this type is used.
\param[in] handle Handle to a RoadRunner instance.
\param[in] nameOfIntegrator Name of the integrator to be used.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegrator (RRHandle handle, char *nameOfIntegrator);

/*!
\brief Obtain a description of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a description of the current integrator.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorName (RRHandle handle);

/*!
\brief Obtain a description of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a description of the current integrator.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorDescription (RRHandle handle);

/*!
\brief Obtain a short hint for the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a short hint of the current integrator.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorHint (RRHandle handle);

/*!
\brief Get the number of adjustable settings for the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns an integer that corresponds to the number of adjustable integrator settings.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getNumberOfCurrentIntegratorParameters (RRHandle handle);

/*!
\brief Get the name of a parameter of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentIntegratorParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorNthParameterName (RRHandle handle, int n);

/*!
\brief Get the description of a parameter of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentIntegratorParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorNthParameterDescription (RRHandle handle, int n);

/*!
\brief Get the display name of a parameter of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentIntegratorParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorNthParameterDisplayName (RRHandle handle, int n);

/*!
\brief Get the hint of a parameter of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentIntegratorParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorNthParameterHint (RRHandle handle, int n);

/*!
\brief Get the type of a parameter of the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentIntegratorParameters)
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentIntegratorNthParameterType (RRHandle handle, int n);

/*!
\brief Reset the integrator parameters to their default values.
\param[in] handle Handle to a RoadRunner instance.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv resetCurrentIntegratorParameters (RRHandle handle);

/*!
\brief Get the names of adjustable settings for the current integrator.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a list that contains the names of adjustable integrator settings.
\ingroup simopts
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getListOfCurrentIntegratorParameterNames (RRHandle handle);

/*!
\brief Get the description for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a description for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorParameterDescription (RRHandle handle, char *parameterName);

/*!
\brief Get the hint for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a hint for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorParameterHint (RRHandle handle, char *parameterName);

/*!
\brief Get the return type for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a integer that indicates the return type for the integrator setting. 0-STRING, 1-BOOL, 2-INT32, 3-UINT32, 4-INT64, 5-UINT64, 6-FLOAT, 7-DOUBLE, 8-CHAR, 9-UCHAR, 10-EMPTY
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentIntegratorParameterType (RRHandle handle, char *parameterName);

/*!
\brief Get the integer value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns an integer value for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentIntegratorParameterInt (RRHandle handle, char *parameterName);

/*!
\brief Set the integer value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\param[in] value The integer value for the integrator setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegratorParameterInt (RRHandle handle, char *parameterName, int value);

/*!
\brief Get the unsigned integer value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns an integer value for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC unsigned int rrcCallConv getCurrentIntegratorParameterUInt (RRHandle handle, char *parameterName);

/*!
\brief Set the unsigned integer value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\param[in] value The integer value for the integrator setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegratorParameterUInt (RRHandle handle, char *parameterName, unsigned int value);

/*!
\brief Get the double value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a double value for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC double rrcCallConv getCurrentIntegratorParameterDouble (RRHandle handle, char *parameterName);

/*!
\brief Set the double value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\param[in] value The double value for the integrator setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegratorParameterDouble (RRHandle handle, char *parameterName, double value);

/*!
\brief Get the string value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a string value for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentIntegratorParameterString (RRHandle handle, char *parameterName);

/*!
\brief Set the string value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\param[in] value The string value for the integrator setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegratorParameterString (RRHandle handle, char *parameterName, char* value);

/*!
\brief Get the boolean value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\return Returns a boolean value for the integrator setting.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentIntegratorParameterBoolean (RRHandle handle, char *parameterName);

/*!
\brief Set the boolean value for a specific integrator setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the integrator setting.
\param[in] value The boolean value for the integrator setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentIntegratorParameterBoolean (RRHandle handle, char *parameterName, int value);


/* Steady State Solvers *******************************************************/

/*!
\brief Get the number of registered steady state solvers.
\return Returns an integer that corresponds to the number of currently registered steady state solvers.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getNumRegisteredSteadyStateSolvers ();

/*!
\brief Get the name of a registered steady state solver (e.g. cvode etc.)
\param[in] n The index of the registered steady state solver
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredSteadyStateSolverName (int n);

/*!
\brief Get the hint of a registered steady state solver (e.g. cvode etc.)
\param[in] n The index of the registered steady state solver
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredSteadyStateSolverHint (int n);

/*!
\brief Get the description of a registered steady state solver (e.g. cvode etc.)
\param[in] n The index of the registered steady state solver
\note Callee owns memory
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getRegisteredSteadyStateSolverDescription (int n);


/*!
\brief Specify the current steady state solver to be used for simulation.
\description This method instantiates a new steady state solver of the given type (e.g. cvode, gillespie) if
one does not currently exist. Otherwise, the existing steady state solver of this type is used.
\param[in] handle Handle to a RoadRunner instance.
\param[in] nameOfSteadyStateSolver Name of the steady state solver to be used.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolver (RRHandle handle, char *nameOfSteadyStateSolver);

/*!
\brief Obtain a description of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a description of the current steady state solver.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverName (RRHandle handle);

/*!
\brief Obtain a description of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a description of the current steady state solver.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverDescription (RRHandle handle);

/*!
\brief Obtain a short hint for the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a short hint of the current steady state solver.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverHint (RRHandle handle);

/*!
\brief Get the number of adjustable settings for the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\return Returns an integer that corresponds to the number of adjustable steady state solver settings.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getNumberOfCurrentSteadyStateSolverParameters (RRHandle handle);

/*!
\brief Get the name of a parameter of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentSteadyStateSolverParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverNthParameterName (RRHandle handle, int n);

/*!
\brief Get the display name of a parameter of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentSteadyStateSolverParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverNthParameterDisplayName (RRHandle handle, int n);

/*!
\brief Get the description of a parameter of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentSteadyStateSolverParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverNthParameterDescription (RRHandle handle, int n);

/*!
\brief Get the hint of a parameter of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentSteadyStateSolverParameters)
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverNthParameterHint (RRHandle handle, int n);

/*!
\brief Get the type of a parameter of the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\param[in] n The index of the parameter (0 <= n < @ref getNumberOfCurrentSteadyStateSolverParameters)
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentSteadyStateSolverNthParameterType (RRHandle handle, int n);

/*!
\brief Reset the steady state solver parameters to their default values.
\param[in] handle Handle to a RoadRunner instance.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv resetCurrentSteadyStateSolverParameters (RRHandle handle);

/*!
\brief Get a string description of the type [STATIC MEMORY - DO NOT FREE]
\description Can call on return value of e.g. @ref getCurrentSteadyStateSolverNthParameterType
to retrieve human-readable string representation.
\param[in] code Type code for the parameter
\ingroup simopts
*/
C_DECL_SPEC const char* rrcCallConv solverTypeToString (int code);

/*!
\brief Get the names of adjustable settings for the current steady state solver.
\param[in] handle Handle to a RoadRunner instance.
\return Returns a list that contains the names of adjustable steady state solver settings.
\ingroup simopts
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getListOfCurrentSteadyStateSolverParameterNames (RRHandle handle);

/*!
\brief Get the description for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a description for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverParameterDescription (RRHandle handle, char *parameterName);

/*!
\brief Get the hint for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a hint for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverParameterHint (RRHandle handle, char *parameterName);

/*!
\brief Get the return type for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a integer that indicates the return type for the steady state solver setting. 0-STRING, 1-BOOL, 2-INT32, 3-UINT32, 4-INT64, 5-UINT64, 6-FLOAT, 7-DOUBLE, 8-CHAR, 9-UCHAR, 10-EMPTY
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentSteadyStateSolverParameterType (RRHandle handle, char *parameterName);

/*!
\brief Get the integer value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns an integer value for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentSteadyStateSolverParameterInt (RRHandle handle, char *parameterName);

/*!
\brief Set the integer value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\param[in] value The integer value for the steady state solver setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolverParameterInt (RRHandle handle, char *parameterName, int value);

/*!
\brief Get the unsigned integer value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns an integer value for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC unsigned int rrcCallConv getCurrentSteadyStateSolverParameterUInt (RRHandle handle, char *parameterName);

/*!
\brief Set the unsigned integer value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\param[in] value The integer value for the steady state solver setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolverParameterUInt (RRHandle handle, char *parameterName, unsigned int value);

/*!
\brief Get the double value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a double value for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC double rrcCallConv getCurrentSteadyStateSolverParameterDouble (RRHandle handle, char *parameterName);

/*!
\brief Set the double value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\param[in] value The double value for the steady state solver setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolverParameterDouble (RRHandle handle, char *parameterName, double value);

/*!
\brief Get the string value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a string value for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC char* rrcCallConv getCurrentSteadyStateSolverParameterString (RRHandle handle, char *parameterName);

/*!
\brief Set the string value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\param[in] value The string value for the steady state solver setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolverParameterString (RRHandle handle, char *parameterName, char* value);

/*!
\brief Get the boolean value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\return Returns a boolean value for the steady state solver setting.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv getCurrentSteadyStateSolverParameterBoolean (RRHandle handle, char *parameterName);

/*!
\brief Set the boolean value for a specific steady state solver setting.
\param[in] handle Handle to a RoadRunner instance.
\param[in] paramterName Name of the steady state solver setting.
\param[in] value The boolean value for the steady state solver setting.
\return Returns True if successful.
\ingroup simopts
*/
C_DECL_SPEC int rrcCallConv setCurrentSteadyStateSolverParameterBoolean (RRHandle handle, char *parameterName, int value);


/*!
 \brief Set the time start for a time course simulation
 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart
 \return Returns True if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setTimeStart(RRHandle handle, double timeStart);

/*!
 \brief Set the time end for a time course simulation
 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeEnd
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setTimeEnd(RRHandle handle, double timeEnd);

/*!
 \brief Set the number of points to generate in a time course simulation
 \param[in] handle Handle to a RoadRunner instance
 \param[in] numberOfPoints Number of points to generate in the time course simulation
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setNumPoints(RRHandle handle, int numberOfPoints);


/*!
 \brief Set the selection list for output from simulate(void) or simulateEx(void)

 Use getAvailableTimeCourseSymbols(void) to retrieve the list of all possible symbols.

 Example: \code setTimeCourseSelectionList ("Time, S1, J1, J2"); \endcode

 or

 setTimeCourseSelectionList ("Time S1 J1 J2")

 \param[in] handle Handle to a RoadRunner instance
 \param[in] list A string of Ids separated by spaces <b>or</b> comma characters
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setTimeCourseSelectionList(RRHandle handle, const char* list);
//C_DECL_SPEC bool rrcCallConv setTimeCourseSelectionList(const char* list);

/*!
 \brief Get the current selection list for simulate(void) or simulateEx(void)

 \param[in] handle Handle to a RoadRunner instance
 \return A list of symbol Ids indicating the current selection list
 \ingroup simulation
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getTimeCourseSelectionList(RRHandle handle);

/*!
 \brief Carry out a time-course simulation. setTimeStart, setTimeEnd,
 setNumPoints, etc are used to set the simulation characteristics.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup simulation
*/
C_DECL_SPEC RRCDataPtr rrcCallConv simulate(RRHandle handle);

/*!
 \brief Retrieve the result of the last simulation.
 \param[in] handle Handle to a RoadRunner instance
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup simulation
*/
C_DECL_SPEC RRCDataPtr rrcCallConv getSimulationResult(RRHandle handle);


/*!
 \brief Carry out a time-course simulation based on the given arguments, time start,
 time end and number of points.

 Example:
 \code
    RRCDataPtr m;

    double timeStart = 0.0;
    double timeEnd = 25;
    int numberOfPoints = 200;

    m = simulateEx (rrHandle, timeStart, timeEnd, numberOfPoints);
    \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart Time start
 \param[in] timeEnd Time end
 \param[in] numberOfPoints Number of points to generate
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup simulation
*/
C_DECL_SPEC RRCDataPtr rrcCallConv simulateEx(RRHandle handle, const double timeStart, const double timeEnd, const int numberOfPoints);

/*!
 \brief Carry out a one step integration of the model

 Example: \code status = OneStep (rrHandle, currentTime, stepSize, newTime); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] currentTime The current time in the simulation
 \param[in] stepSize The step size to use in the integration
 \param[in] newTime The new time (currentTime + stepSize)

 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv oneStep(RRHandle handle, const double currentTime, const double stepSize, double *value);

/*!
 \brief Get the value of the current time start

 Example: \code status = getTimeStart (rrHandle, &timeStart); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[out] timeStart The current value for the time start
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv getTimeStart(RRHandle handle, double* timeStart);

/*!
 \brief Get the value of the current time end

 Example: \code status = getTimeEnd (rrHandle, &timeEnd); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param timeEnd The current value for the time end
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv getTimeEnd(RRHandle handle, double* timeEnd);

/*!
 \brief Get the value of the current number of points

 Example: \code status = getNumPoints (rrHandle, &numberOfPoints); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param numPoints The current value for the number of points
 \return Returns true if successful
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv getNumPoints (RRHandle handle, int* numPoints);

/*!
 \brief Compute the steady state of the current model

 Example: \code status = steadyState (rrHandle, &closenessToSteadyState); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param value This value is set during the call and indicates how close the solution is to the steady state.
 The smaller the value the better. Values less than 1E-6 usually indicate a steady state has been found. If necessary
 call the method a second time to improve the solution.
 \return Returns true if successful
 \ingroup steadystate
*/
C_DECL_SPEC bool rrcCallConv steadyState(RRHandle handle, double* value);

/*!
 \brief A convenient method for returning a vector of the steady state species concentrations

 Example: \code RRVectorHandle values = computeSteadyStateValues (void); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns the vector of steady state values or null if an error occurred. The order of
 species in the vector is indicated by the order of species Ids in a call to getFloatingSpeciesIds(void)
 \ingroup steadystate
*/
C_DECL_SPEC RRVectorPtr rrcCallConv computeSteadyStateValues(RRHandle handle);

/*!
 \brief Set the selection list of the steady state analysis

 Use getAvailableTimeCourseSymbols(void) to retrieve the list of all possible symbols.

 Example:

 \code
 setSteadyStateSelectionList ("S1, J1, J2")

 or

 setSteadyStateSelectionList ("S1 J1 J2")
 \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] list The string argument should be a space separated list of symbols.

 \return Returns true if successful
 \ingroup steadystate
*/
C_DECL_SPEC bool rrcCallConv setSteadyStateSelectionList(RRHandle handle, const char* list);

/*!
 \brief Get the selection list for the steady state analysis

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns a list of strings representing symbols in the selection list
 \ingroup steadystate
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getSteadyStateSelectionList(RRHandle handle);


// --------------------------------------------------------------------------------
// Get and Set Routines
// --------------------------------------------------------------------------------

/*!
 \brief Get the value for a given symbol, use getAvailableTimeCourseSymbols(void) for a list of symbols

 Example: \code status = getValue (rrHandle, "S1", &value); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param symbolId The symbol that we wish to obtain the value for
 \param value The value that will be retrieved
 \return Returns true if successful
 \ingroup state
*/
C_DECL_SPEC bool rrcCallConv getValue(RRHandle handle, const char* symbolId, double* value);


/*!
 \brief Set the value for a given symbol, use getAvailableTimeCourseSymbols(void) for a list of symbols

 Example: \code status = setValue (rrHandle, "S1", 0.5); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param symbolId The symbol that we wish to set the value
 \param value The value that will be set to the symbol
 \return Returns true if successful
 \ingroup state
*/
C_DECL_SPEC bool rrcCallConv setValue(RRHandle handle, const char* symbolId, const double value);


/*!
 \brief Retrieve in a vector the concentrations for all the floating species

 Example: \code RRVectorPtr values = getFloatingSpeciesConcentrations (void); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns the vector of floating species concentrations or null if an error occurred
 \ingroup floating
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getFloatingSpeciesConcentrations(RRHandle handle);


/*!
 \brief Retrieve the concentrations for all the boundary species in a vector

 Example: \code RRVectorPtr values = getBoundarySpeciesConcentrations (void); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns the vector of boundary species concentrations or null if an error occurred
 \ingroup boundary
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getBoundarySpeciesConcentrations(RRHandle handle);

// --------------------------------------------------------------------------------
// Parameter Group
// --------------------------------------------------------------------------------

/*!
 \brief Retrieve the values for all the global parameter values in a vector

 Example: \code RRVectorPtr values = getGlobalParameterValues (void); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns the vector of global parameter values or null if an error occurred
 \ingroup parameters
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getGlobalParameterValues(RRHandle handle);

/*!
 \brief Set the concentration for a particular boundary species.

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the boundary species (corresponds to position in getBoundarySpeciesIds(void))
 \param value The concentration of the species to set
 \return Returns true if successful
 \ingroup boundary
*/
C_DECL_SPEC bool rrcCallConv setBoundarySpeciesByIndex(RRHandle handle, const int index, const double value);

/*!
 \brief Set the initial concentration for a particular floating species.

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the floating species (corresponds to position in getFloatingSpeciesIds(RRHandle handle))
 \param value The concentration of the species to set
 \return Returns true if successful
 \ingroup floating
*/
C_DECL_SPEC bool rrcCallConv setFloatingSpeciesInitialConcentrationByIndex(RRHandle handle, int index, double value);

/*!
 \brief Get the initial concentration for a particular floating species.

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the floating species (corresponds to position in getFloatingSpeciesIds(RRHandle handle))
 \param[in] value The concentration of the species
 \return Returns true if successful
 \ingroup floating
*/
C_DECL_SPEC bool rrcCallConv getFloatingSpeciesInitialConcentrationByIndex(RRHandle handle, int index, double* value);

/*!
 \brief Set the concentration for a particular floating species.

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the floating species (corresponds to position in getFloatingSpeciesIds(RRHandle handle))
 \param value The concentration of the species to set
 \return Returns true if successful
 \ingroup floating
*/
C_DECL_SPEC bool rrcCallConv setFloatingSpeciesByIndex(RRHandle handle, const int index, const double value);

/*!
 \brief Set the value for a particular global parameter

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the global parameter (corresponds to position in getGlobalParameterIds(RRHandle handle))
 \param value The value of the parameter to set
 \return Returns true if successful
 \ingroup parameters
*/
C_DECL_SPEC bool rrcCallConv setGlobalParameterByIndex(RRHandle handle, const int index, const double value);

/*!
 \brief Retrieve the concentration for a particular floating species.
 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the boundary species (corresponds to position in getBoundarySpeciesIds(RRHandle handle))
 \param value The value returned by the method
 \return Returns true if successful
 \ingroup boundary
*/
C_DECL_SPEC bool rrcCallConv getBoundarySpeciesByIndex(RRHandle handle, const int index, double* value);

/*!
 \brief Retrieve the concentration for a particular floating species.

 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the floating species (corresponds to position in getFloatingSpeciesIds(RRHandle handle))
 \param value The value returned by the method
 \return Returns true if successful
 \ingroup floating
*/
C_DECL_SPEC bool rrcCallConv getFloatingSpeciesByIndex(RRHandle handle, const int index, double* value);

/*!
 \brief Retrieve the global parameter value
 \param[in] handle Handle to a RoadRunner instance
 \param index The index to the global parameter (corresponds to position in getGlobalParametersIds(RRHandle handle))
 \param value The value returned by the method
 \return Returns true if successful, false otherwise
 \ingroup parameters
*/
C_DECL_SPEC bool rrcCallConv getGlobalParameterByIndex(RRHandle handle, const int index, double* value);

/*!
 \brief Retrieve the compartment volume for a particular compartment.

 \param[in] handle Handle to a RoadRunner instance
 \param[in] index The index to the compartment (corresponds to position in getCompartmentIds(RRHandle handle))
 \param[in] value The value returned by the method
 \return Returns true if successful, false otherwise
 \ingroup compartment
*/
C_DECL_SPEC bool rrcCallConv getCompartmentByIndex (RRHandle handle, const int index, double* value);


/*!
 \brief Set the volume for a particular compartment

 \param[in] handle Handle to a RoadRunner instance
 \param[in] index The index to the compartment (corresponds to position in getCompartmentIds(RRHandle handle))
 \param[in] value The volume of the compartment to set
 \return Returns true if successful, false otherwise
 \ingroup compartment
*/
C_DECL_SPEC bool rrcCallConv setCompartmentByIndex (RRHandle handle, const int index, const double value);


/*!
 \brief Set the floating species concentration to the vector vec

 Example:

 \code
 myVector = createVector (getNumberOfFloatingSpecies(RRHandle handle));
 setVectorElement (myVector, 0, 1.2);
 setVectorElement (myVector, 1, 5.7);
 setVectorElement (myVector, 2, 3.4);
 setFloatingSpeciesConcentrations(myVector);
 \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] vec Pointer to a RRVector
 \return Returns true if successful
 \ingroup floating
*/
C_DECL_SPEC bool rrcCallConv setFloatingSpeciesConcentrations(RRHandle handle, const RRVectorPtr vec);

/*!
 \brief Set the boundary species concentration to the vector vec

 Example:

 \code
 myVector = createVector (getNumberOfBoundarySpecies(RRHandle handle));
 setVectorElement (myVector, 0, 1.2);
 setVectorElement (myVector, 1, 5.7);
 setVectorElement (myVector, 2, 3.4);
 setBoundarySpeciesConcentrations(myVector);
\endcode

 \param[in] handle Handle to a RoadRunner instance
 \param vec A vector of boundary species concentrations
 \return Returns true if successful
 \ingroup boundary
*/
C_DECL_SPEC bool rrcCallConv setBoundarySpeciesConcentrations(RRHandle handle, const RRVectorPtr vec);


/*!
 \brief Retrieve the full Jacobian for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns the full Jacobian matrix
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getFullJacobian(RRHandle handle);

/*!
 \brief Retrieve the reduced Jacobian for the current model

 setComputeAndAssignConservationLaws (true) must be enabled.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns the reduced Jacobian matrix
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getReducedJacobian(RRHandle handle);

/*!
 \brief Retrieve the eigenvalue matrix for the current model
 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of eigenvalues.
 The first column will contain the real values and the second column the imaginary values
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getEigenvalues(RRHandle handle);


// --------------------------------------------------------------------------------
// Stoichiometry methods
// --------------------------------------------------------------------------------

/*!
 \brief Retrieve the stoichiometry matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns the stoichiometry matrix.
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getStoichiometryMatrix(RRHandle handle);

/*!
 \brief Retrieve the Link matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns the Link matrix.
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getLinkMatrix(RRHandle handle);

/*!
 \brief Retrieve the reduced stoichiometry matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns reduced stoichiometry matrix
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getNrMatrix(RRHandle handle);

/*!
 \brief Retrieve the conservation matrix for the current model.

 The conservation laws as describe by row where the columns indicate the species Id.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns the conservation matrix.
 \ingroup Stoich
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getConservationMatrix(RRHandle handle);

// --------------------------------------------------------------------------------
// Initial condition Methods
// --------------------------------------------------------------------------------

/*!
 \brief Set the initial floating species concentrations

 Example: \code status = setFloatingSpeciesInitialConcentrations (vec); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param vec A vector of species concentrations: order given by getFloatingSpeciesIds(RRHandle handle)
 \return Returns true if successful
 \ingroup initialConditions
*/
C_DECL_SPEC bool rrcCallConv setFloatingSpeciesInitialConcentrations (RRHandle handle, const RRVectorPtr vec);

/*!
 \brief Get the initial floating species concentrations

 Example: \code vec = getFloatingSpeciesInitialConcentrations (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails otherwise returns a vector containing the initial conditions
 \ingroup initialConditions
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getFloatingSpeciesInitialConcentrations (RRHandle handle);

/*!
 \deprecated Use @ref getFloatingSpeciesInitialConcentrationIds
 \brief Get the initial floating species Ids

 Example: \code vec = getFloatingSpeciesInitialConditionIds (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails otherwise returns a vector containing names of the floating species
 \ingroup initialConditions
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getFloatingSpeciesInitialConditionIds(RRHandle handle);

/*!
 \brief Get the initial floating species Ids

 Example: \code vec = getFloatingSpeciesInitialConcentrationIds (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails otherwise returns a vector containing names of the floating species
 \ingroup initialConditions
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getFloatingSpeciesInitialConcentrationIds(RRHandle handle);


// --------------------------------------------------------------------------------
// Reaction rates
// --------------------------------------------------------------------------------

/*!
 \brief Obtain the number of reactions in the loaded model

 Example: \code number = getNumberOfReactions (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns -1 if it fails, if successful it return 0 or more, indicating the number of reactions
 \ingroup reaction
*/
C_DECL_SPEC int rrcCallConv getNumberOfReactions(RRHandle handle);


/*!
 \brief Retrieve a give reaction rate as indicated by the index parameter

 \param[in] handle Handle to a RoadRunner instance
 \param index - The index is used to specify which reaction rate to retrieve
 \param rate - The reaction rate is returned in the rate argument
 \return Returns false if it fails
 \ingroup reaction
*/
C_DECL_SPEC bool rrcCallConv getReactionRate(RRHandle handle, const int index, double* rate);


/*!
 \brief Retrieve a vector of reaction rates as determined by the current state of the model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise it returns a vector of reaction rates
 \ingroup reaction
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getReactionRates(RRHandle handle);


/*!
 \brief Retrieve a vector of reaction rates given a vector of species concentrations

 \param[in] handle Handle to a RoadRunner instance
 \param vec The vector of floating species concentrations
 \return Returns null if it fails otherwise it returns a vector of reaction rates
 \ingroup reaction
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getReactionRatesEx (RRHandle handle, const RRVectorPtr vec);


/*!
 \brief Retrieve the vector of rates of change as determined by the current state of the model

 Example: \code values = getRatesOfChange (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a vector of rates of change values
 \ingroup rateOfChange
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getRatesOfChange(RRHandle handle);

/*!
 \brief Retrieve the string list of rates of change Ids

 Example: \code Ids = getRatesOfChangeIds (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a list of rates of change Ids
 \ingroup rateOfChange
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getRatesOfChangeIds(RRHandle handle);


/*!
 \brief Retrieve the rate of change for a given floating species

 Example: \code status = getRateOfChange (&index, *value); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[out] value Pointer to double, holding the rate of change
 \return Returns false if it fails, otherwise value contains the rate of change.
 \ingroup rateOfChange
*/
C_DECL_SPEC bool rrcCallConv getRateOfChange(RRHandle handle, const int, double* value);


/*!
 \brief Retrieve the vector of rates of change given a vector of floating species concentrations

 Example: \code values = getRatesOfChangeEx (vector); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] vec Vector handle holding rates of change
 \return Returns null if it fails
 \ingroup rateOfChange
*/
C_DECL_SPEC RRVectorPtr rrcCallConv getRatesOfChangeEx (RRHandle handle, const RRVectorPtr vec);

/*!
 \brief Evaluate the current model, that it update all assignments and rates of change. Do not carry out an integration step

 \param[in] handle Handle to a RoadRunner instance
 \return Returns false if it fails
 \ingroup state
*/
C_DECL_SPEC bool rrcCallConv evalModel(RRHandle handle);

// Get number family
/*!
 \brief Returns the number of compartments in the model
 \param[in] handle Handle to a RoadRunner instance
 \return Returns number of compartments
 \ingroup compartment
*/
C_DECL_SPEC int rrcCallConv getNumberOfCompartments (RRHandle handle);


/*!
 \brief Returns the number of boundary species in the model
 \param[in] handle Handle to a RoadRunner instance
 \ingroup boundary
*/
C_DECL_SPEC int rrcCallConv getNumberOfBoundarySpecies(RRHandle handle);


/*!
 \brief Returns the number of floating species in the model
 \param[in] handle Handle to a RoadRunner instance
 \ingroup floating
*/
C_DECL_SPEC int rrcCallConv getNumberOfFloatingSpecies(RRHandle handle);


/*!
 \brief Returns the number of global parameters in the model
 \param[in] handle Handle to a RoadRunner instance
 \ingroup parameters
*/
C_DECL_SPEC int rrcCallConv getNumberOfGlobalParameters(RRHandle handle);

// --------------------------------------------------------------------------------
// Get number family
// --------------------------------------------------------------------------------

/*!
 \brief Returns the number of dependent species in the model
 \param[in] handle Handle to a RoadRunner instance
 \ingroup floating
*/
C_DECL_SPEC int rrcCallConv getNumberOfDependentSpecies(RRHandle handle);


/*!
 \brief Returns the number of independent species in the model

 \param[in] handle Handle to a RoadRunner instance
 \ingroup floating
*/
C_DECL_SPEC int rrcCallConv getNumberOfIndependentSpecies(RRHandle handle);

// --------------------------------------------------------------------------------
// Get Ids family
// --------------------------------------------------------------------------------

/*!
 \brief Obtain the list of reaction Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup reaction
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getReactionIds(RRHandle handle);

/*!
 \brief Obtain the list of boundary species Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup boundary
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getBoundarySpeciesIds(RRHandle handle);

/*!
 \brief Obtain the list of floating species Id

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup floating
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getFloatingSpeciesIds(RRHandle handle);

/*!
 \brief Obtain the list of global parameter Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup parameters
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getGlobalParameterIds(RRHandle handle);

/*!
 \brief Obtain the list of compartment Ids

 Example: \code str = getCompartmentIds (RRHandle handle); \endcode

 \param[in] handle Handle to a RoadRunner instance
 \return Returns -1 if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup compartment
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getCompartmentIds(RRHandle handle);

/*!
 \brief Obtain the list of eigenvalue Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns -1 if it fails, if successful it returns a pointer to a RRStringArrayPtr struct
 \ingroup state
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getEigenvalueIds(RRHandle handle);

/*!
 \brief Obtain the list of all available symbols

 \param[in] handle Handle to a RoadRunner instance
 \return Returns -1 if it fails, if successful it returns a pointer to a RRListHandle struct
 \ingroup state
*/
C_DECL_SPEC RRListPtr rrcCallConv getAvailableTimeCourseSymbols(RRHandle handle);

/*!
 \brief Obtain the list of all available steady state symbols

 \param[in] handle Handle to a RoadRunner instance
 \return Returns -1 if it fails, if successful it returns a pointer to a RRListPtr struct
 \ingroup state
*/
C_DECL_SPEC RRListPtr rrcCallConv getAvailableSteadyStateSymbols(RRHandle handle);

// --------------------------------------------------------------------------------
// MCA methods
// --------------------------------------------------------------------------------

/*!
 \brief Obtain the list of elasticity coefficient Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a list
 \ingroup mca
*/
C_DECL_SPEC RRListPtr rrcCallConv getElasticityCoefficientIds(RRHandle handle);

/*!
 \brief Obtain the list of unscaled flux control coefficient Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a list of Ids
 \ingroup mca
*/
C_DECL_SPEC RRListPtr rrcCallConv getUnscaledFluxControlCoefficientIds(RRHandle handle);

/*!
 \brief Obtain the list of flux control coefficient Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a list of Ids
 \ingroup mca
*/
C_DECL_SPEC RRListPtr rrcCallConv getFluxControlCoefficientIds(RRHandle handle);

/*!
 \brief Obtain the list of unscaled concentration control coefficient Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a list of Ids
 \ingroup mca
*/
C_DECL_SPEC RRListPtr rrcCallConv getUnscaledConcentrationControlCoefficientIds(RRHandle handle);

/*!
 \brief Obtain the list of concentration coefficient Ids

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, if successful it returns a list of Ids
 \ingroup mca
*/
C_DECL_SPEC RRListPtr rrcCallConv getConcentrationControlCoefficientIds(RRHandle handle);

/*!
 \brief Retrieve the unscaled elasticity matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns nil if it fails, otherwise returns a matrix of unscaled elasticities
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getUnscaledElasticityMatrix(RRHandle handle);

/*!
 \brief Retrieve the scaled elasticity matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of scaled elasticities.
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getScaledElasticityMatrix(RRHandle handle);


/*!
 \brief Retrieve the scaled elasticity matrix for the current model

 \param[in] handle Handle to a RoadRunner instance
 \param[in] reactionId The reaction Id for computing the elasticity
 \param[in] speciesId The floating species to compute the elasticity for
 \param[out] value The return value for the elasticity
 \return Returns false if it fails
 \ingroup mca
*/
C_DECL_SPEC bool rrcCallConv getScaledFloatingSpeciesElasticity(RRHandle handle, const char* reactionId, const char* speciesId, double* value);

/*!
 \brief Retrieve the matrix of unscaled concentration control coefficients for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of unscaled concentration control coefficients
 The first column will contain the real values and the second column the imaginary values
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getUnscaledConcentrationControlCoefficientMatrix(RRHandle handle);


/*!
 \brief Retrieve the matrix of scaled concentration control coefficients for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of scaled concentration control coefficients
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getScaledConcentrationControlCoefficientMatrix(RRHandle handle);

/*!
 \brief Retrieve the matrix of unscaled flux control coefficients for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of unscaled flux control coefficients
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getUnscaledFluxControlCoefficientMatrix(RRHandle handle);

/*!
 \brief Retrieve the matrix of scaled flux control coefficients for the current model

 \param[in] handle Handle to a RoadRunner instance
 \return Returns null if it fails, otherwise returns a matrix of scaled flux control coefficients
 \ingroup mca
*/
C_DECL_SPEC RRDoubleMatrixPtr rrcCallConv getScaledFluxControlCoefficientMatrix(RRHandle handle);

/*!
 \brief Retrieve a single unscaled control coefficient

 \param[in] handle Handle to a RoadRunner instance
 \param[in] variable This is the dependent variable of the coefficient, for example a flux or species concentration
 \param[in] parameter This is the independent parameter, for example a kinetic constant or boundary species
 \param[out] value This is the value of the unscaled control coefficient returns to the caller
 \return Returns true if successful
 \ingroup mca
*/
C_DECL_SPEC bool rrcCallConv getuCC (RRHandle handle, const char* variable, const char* parameter, double* value);

/*!
 \brief Retrieve a single control coefficient

 \param[in] handle Handle to a RoadRunner instance
 \param[in] variable This is the dependent variable of the coefficient, for example a flux or species concentration
 \param[in] parameter This is the independent parameter, for example a kinetic constant or boundary species
 \param[out] value This is the value of the control coefficient returns to the caller
 \return Returns true if successful
 \ingroup mca
*/
C_DECL_SPEC bool  rrcCallConv getCC (RRHandle handle, const char* variable, const char* parameter, double* value);

/*!
 \brief Retrieve a single elasticity coefficient

 \param[in] handle Handle to a RoadRunner instance
 \param[in] name This is the reaction variable for the elasticity
 \param[in] species This is the independent parameter, for example a floating of boundary species
 \param[out] value This is the value of the elasticity coefficient returns to the caller
 \return Returns true if successful
 \ingroup mca
*/
C_DECL_SPEC bool rrcCallConv getEE(RRHandle handle, const char* name, const char* species, double* value);

/*!
 \brief Retrieve a single unscaled elasticity coefficient

 \param[in] handle Handle to a RoadRunner instance
 \param[in] name This is the reaction variable for the unscaled elasticity
 \param[in] species This is the independent parameter, for example a floating of boundary species
 \param[out] value This is the value of the unscaled elasticity coefficient returns to the caller
 \return Returns true if successful
 \ingroup mca
*/
C_DECL_SPEC bool rrcCallConv getuEE(RRHandle handle, const char* name, const char* species, double* value);

// --------------------------------------------------------------------------------
// Stochastic methods
// --------------------------------------------------------------------------------

/*!
 \brief Determine the current seed used by the random generator

 \param[in] handle Handle to a RoadRunner instance
 \param[out] seed This is the value of the current seed, returned to the caller
 \return Returns true if successful
 \ingroup stochastic
*/
C_DECL_SPEC bool rrcCallConv getSeed(RRHandle handle, long* seed);

/*!
 \brief Set the current seed used by the random generator

 \param[in] handle Handle to a RoadRunner instance
 \param[out] seed This is the value the caller will set the seed to
 \return Returns true if successful
 \ingroup stochastic
*/
C_DECL_SPEC bool rrcCallConv setSeed(RRHandle handle, long seed);

/*!
 \brief Carry out a time-course simulation using the Gillespie algorithm with
 variable step size. setTimeStart, setTimeEnd, etc are used to set the simulation
 characteristics.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespie(RRHandle handle);

/*!
 \brief Carry out a time-course simulation using the Gillespie algorithm based
 on the given arguments, time start, time end and number of points.

 Example:
 \code
    RRCDataPtr m;

    double timeStart = 0.0;
    double timeEnd = 25;

    m = gillespieEx (rrHandle, timeStart, timeEnd);
    \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart Time start
 \param[in] timeEnd Time end
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieEx(RRHandle handle, double timeStart, double timeEnd);

/*!
 \brief Carry out a time-course simulation using the Gillespie algorithm with
 fixed step size. setTimeStart, setTimeEnd, setNumPoints, etc are used to set
 the simulation characteristics.

 \param[in] handle Handle to a RoadRunner instance
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieOnGrid(RRHandle handle);

/*!
 \brief Carry out a time-course simulation using the Gillespie algorithm with
 fixed step size based on the given arguments, time start, time end, and
 number of points.

 Example:
 \code
    RRCDataPtr m;

    double timeStart = 0.0;
    double timeEnd = 25;
    int numberOfPoints = 200;

    m = gillespieOnGridEx (rrHandle, timeStart, timeEnd, numberOfPoints);
    \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart Time start
 \param[in] timeEnd Time end
 \param[in] numberOfPoints Fixed number of points to generate
 \return Returns an array (RRCDataPtr) of columns containing the results of the
 simulation including string labels for the individual columns. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieOnGridEx(RRHandle handle, double timeStart, double timeEnd, int numberOfPoints);

/*!
 \brief Carry out a series of time-course simulations using the Gillespie
 algorithm with fixed step size, then return the average of the simulations.
 setTimeStart, setTimeEnd, setNumPoints, etc are used to set the simulation
 characteristics.

 \param[in] handle Handle to a RoadRunner instance
 \param[in] numberOfSimulations Number of simulations to perform
 \return Returns an array (RRCDataPtr) of columns containing the average of the
 results of the simulations including string labels for the individual columns.
 The client is responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieMeanOnGrid(RRHandle handle, int numberOfSimulations);

/*!
 \brief Carry out a series of time-course simulations using the Gillespie
 algorithm with fixed step size, then return the average of the simulations.
 Based on the given arguments, time start, time end, and number of points.

 Example:
 \code
    RRCDataPtr m;

    double timeStart = 0.0;
    double timeEnd = 25;
    int numberOfPoints = 200;
    int numberOfSimulations = 10;

    m = gillespieMeanOnGridEx (rrHandle, timeStart, timeEnd, numberOfPoints, numberOfSimulations);
    \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart Time start
 \param[in] timeEnd Time end
 \param[in] numberOfPoints Fixed number of points to generate
 \param[in] numberOfSimulations Number of simulations to perform
 \return Returns an array (RRCDataPtr) of columns containing the average of the
 results of the simulation including string labels for the individual columns.
 The client is responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieMeanOnGridEx(RRHandle handle, double timeStart, double timeEnd, int numberOfPoints, int numberOfSimulations);

/*!
 \brief Carry out a series of time-course simulations using the Gillespie
 algorithm with fixed step size, then return the average and standard
 deviation of the simulations. setTimeStart, setTimeEnd, setNumPoints, etc
 are used to set the simulation characteristics.

 \param[in] handle Handle to a RoadRunner instance
 \param[in] numberOfSimulations Number of simulations to perform
 \return Returns an array (RRCDataPtr) of columns containing the average of the
 results of the simulations including string labels for the individual columns.
 The averages are in Data and the standard deviations are in Weights.
 The client is responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieMeanSDOnGrid(RRHandle handle, int numberOfSimulations);


/*!
 \brief Carry out a series of time-course simulations using the Gillespie
 algorithm with fixed step size, then return the average and standard deviation
 of the simulations. Based on the given arguments, time start, time end, number
 of points, and number of simulations.

 Example:
 \code
    RRCDataPtr m;

    double timeStart = 0.0;
    double timeEnd = 25;
    int numberOfPoints = 200;
    int numberOfSimulations = 10;

    m = gillespieMeanSDOnGridEx (rrHandle, timeStart, timeEnd, numberOfPoints, numberOfSimulations);
    \endcode

 \param[in] handle Handle to a RoadRunner instance
 \param[in] timeStart Time start
 \param[in] timeEnd Time end
 \param[in] numberOfPoints Fixed number of points to generate
 \param[in] numberOfSimulations Number of simulations to perform
 \return Returns an array (RRCDataPtr) of columns containing the average of the
 results of the simulation including string labels for the individual columns. The
 average values are in Data and the standard deviations are in Weights. The client is
 responsible for freeing the resulting RRCDataPtr structure.
 \ingroup stochastic
*/
C_DECL_SPEC RRCDataPtr rrcCallConv gillespieMeanSDOnGridEx(RRHandle handle, double timeStart, double timeEnd, int numberOfSteps, int numberOfSimulations);

// --------------------------------------------------------------------------------
// Reset methods
// --------------------------------------------------------------------------------

/*!
 \brief Resets all variables of the model to their current initial values.
 Does not change the parameters.

 \param[in] handle Handle to a RoadRunner instance
 \return Boolean indicating success
 \ingroup reset
*/
C_DECL_SPEC bool rrcCallConv reset(RRHandle handle);

/*!
 \brief Resets all variables of the model to their current initial values,
 and resets all parameters to their original values.

 \param[in] handle Handle to a RoadRunner instance
 \return Boolean indicating success
 \ingroup reset
*/
C_DECL_SPEC bool rrcCallConv resetAll(RRHandle handle);

/*!
 \brief Resets the model to the state in which it was first loaded, including
 initial conditions, variables, and parameters.

 \param[in] handle Handle to a RoadRunner instance
 \return Boolean indicating success
 \ingroup reset
*/
C_DECL_SPEC bool rrcCallConv resetToOrigin(RRHandle handle);

/*!
 \brief Set a boolean configuration value

 \param[in] key The configuration key
 \param[in] value The value to assign to the configuration key
 \return Returns positive if successful
 \ingroup config
*/
C_DECL_SPEC int rrcCallConv setConfigBool(const char* key, int value);

/*!
 \brief Get a boolean configuration value

 \param[in] key The configuration key
 \note On error, sets an error message which can be retrieved via @ref getLastError
 \ingroup config
*/
C_DECL_SPEC int rrcCallConv getConfigBool(const char* key);

/*!
 \brief Set an integer configuration value

 \param[in] key The configuration key
 \param[in] value The value to assign to the configuration key
 \return Returns positive if successful
 \ingroup config
*/
C_DECL_SPEC int rrcCallConv setConfigInt(const char* key, int value);

/*!
 \brief Get an integer configuration value

 \param[in] key The configuration key
 \note On error, sets an error message which can be retrieved via @ref getLastError
 \ingroup config
*/
C_DECL_SPEC int rrcCallConv getConfigInt(const char* key);

/*!
 \brief Set a double configuration value

 \param[in] key The configuration key
 \param[in] value The value to assign to the configuration key
 \return Returns positive if successful
 \ingroup config
*/
C_DECL_SPEC int rrcCallConv setConfigDouble(const char* key, double value);

/*!
 \brief Get a double configuration value

 \param[in] key The configuration key
 \note On error, sets an error message which can be retrieved via @ref getLastError
 \ingroup config
*/
C_DECL_SPEC double rrcCallConv getConfigDouble(const char* key);

/*!
 \brief Get a list of all possible config keys

 \ingroup config
*/
C_DECL_SPEC RRStringArrayPtr rrcCallConv getListOfConfigKeys();

#if defined( __cplusplus)
}
}//namespace

#endif

#endif
/*! \mainpage RoadRunner C API Library
 *
 * \section intro_sec Introduction
 *
 * RoadRunner is a SBML compliant high performance and portable simulation engine
 * for systems and synthetic biology. To run a simple SBML model
 * and generate time series data we would write the following code:
 *
 \code
#undef __cplusplus
#define STATIC_RRC
#include <stdio.h>
#include <stdlib.h>
#include "rrc_api.h"
#include "rrc_types.h"
#include "rrc_utilities.h"
int main (int argc, char *argv[]) {
    RRHandle rrHandle;
    RRCDataPtr result;

    printf ("Starting Test Program %s\n", argv[0]);
    rrHandle = createRRInstance();
    if (!loadSBMLFromFile (rrHandle, "feedback.xml")) {
        printf ("Failed to load model: %s\n", getLastError ());
        getchar ();
        exit (0);
    }
    result = simulateEx (rrHandle, 0, 10, 100);
    printf (rrDataToString (result));
    freeRRCData(result);

    getchar ();
    exit (0);
}
 \endcode

More complex example, using C API:
\code
#undef __cplusplus
#define STATIC_RRC
#include <stdio.h>
#include <stdlib.h>
#include "rrc_api.h"
#include "rrc_types.h"
#include "rrc_utilities.h"
int main (int argc, char *argv[]) {
   RRHandle rrHandle;
   RRCDataPtr result;
   int index;
   int col;
   int row;
   printf ("Starting Test Program %s\n", argv[0]);
   rrHandle = createRRInstance();
   if (!loadSBML (rrHandle, "feedback.xml")) {
      printf ("Error while loading SBML file\n");
      printf ("Error message: %s\n", getLastError());
      getchar ();
      exit (0);
   }
   result = simulateEx (rrHandle, 0, 10, 10);  // start time, end time, and number of points
   index = 0;
   // Print out column headers... typically time and species.
   for (col = 0; col < result->CSize; col++)
   {
      printf ("%10s", result->ColumnHeaders[index++]);
      if (col < result->CSize - 1)
      {
         printf ("\t");
      }
   }
   printf ("\n");
   index = 0;
   // Print out the data
   for (row = 0; row < result->RSize; row++)
   {
      for (col = 0; col < result->CSize; col++)
      {
         printf ("%10f", result->Data[index++]);
         if (col < result->CSize -1)
         {
            printf ("\t");
         }
      }
   printf ("\n");
   }
   //Cleanup
   freeRRCData (result);
   freeRRInstance (rrHandle);
   getchar ();
   exit (0);

\endcode

Would create output as shown below:

\code
Starting Test Program: <File path Here>
Notice: Creating C based model generator using ..\compilers\tcc\tcc.exe compiler.
     time            [S1]            [S2]            [S3]            [S4]
 0.000000        0.000000        0.000000        0.000000        0.000000
 1.111111        3.295975        1.677255        1.121418        1.074708
 2.222222        0.971810        1.658970        1.841065        2.192728
 3.333333        0.137340        0.501854        1.295138        2.444883
 4.444445        0.141470        0.200937        0.549172        1.505662
 5.555556        1.831017        1.317792        1.129982        1.351300
 6.666667        0.306310        0.775477        1.304950        1.952076
 7.777778        0.193459        0.268986        0.628542        1.483161
 8.888889        1.566864        1.219950        1.105718        1.370199
10.000000        0.269437        0.678127        1.199353        1.868247

\endcode
 * \section install_sec Installation
 *
 * Installation documentation is provided at libRoadRunner.org.

 * \section license_sec License
 * Copyright (C) 2012
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


 \defgroup initialization Library initialization and termination methods
 \brief Initialize library and terminate library instance

 \defgroup loadsave Read and write models
 \brief Read and write models to files or strings. Support for SBML formats.

 \defgroup utility Utility functions
 \brief Various miscellaneous routines that return useful information about the library

 \defgroup errorfunctions Error handling functions
 \brief Error handling routines

 \defgroup logging Logging functionality
 \brief RoadRunner logging routines

 \defgroup state Current state of system
 \brief Compute derivatives, fluxes, and other values of the system at the current state

 \defgroup simulation Time-course simulation
 \brief Deterministic, stochastic, and hybrid simulation algorithms

 \defgroup steadystate Steady state routines
 \brief Compute and obtain basic information about the steady state

 \defgroup reaction Reaction group
 \brief Get information about reaction rates

 \defgroup rateOfChange Rates of change group
 \brief Get information about rates of change

 \defgroup boundary Boundary species group
 \brief Get information about boundary species

 \defgroup floating Floating species group
 \brief Get information about floating species

 \defgroup initialConditions Initial conditions group
 \brief Set or get initial conditions

 \defgroup parameters Parameters group
 \brief Set and get global and local parameters

 \defgroup compartment Compartment group
 \brief Set and Get information on compartments

 \defgroup mca Metabolic control analysis
 \brief Calculate control coefficients and sensitivities

 \defgroup stochastic Stochastic simulations
 \brief Stochastic simulation algorithms

 \defgroup Stoich Stoichiometry analysis
 \brief Linear algebra based methods for analyzing a reaction network

 \defgroup NOM Network object model (NOM) functions
 \brief Network object model functions

 \defgroup LinearAlgebra Linear algebra functions
 \brief Linear algebra utility functions

 \defgroup list List handling routines
 \brief Some methods return lists (heterogeneous arrays of data),
 these routines make it easier to manipulate lists

 \defgroup helperRoutines Helper routines
 \brief Helper routines for accessing the various C API types, eg lists and arrays

 \defgroup toString ToString routines
 \brief Render various result data types as strings

 \defgroup stringArray StringArray routines
 \brief Utility routines to deal with the string array type

 \defgroup freeRoutines Free memory routines
 \brief Routines that should be used to free various data structures generated during the course of using the library

 \defgroup reset Reset methods
 \brief Methods for resetting instances to various initial states

 \defgroup simopts Solver options API
 \brief Reflective API for interacting with steady state solvers and integrators

 \defgroup config Configuration keys/values
 \brief Loads and sets configuration options
*/


