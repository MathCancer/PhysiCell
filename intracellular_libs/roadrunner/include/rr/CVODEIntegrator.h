// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file CVODEIntegrator.h
* @author WBC, ETS, MTK, JKM
* @copyright Apache License, Version 2.0
* @brief Contains the RoadRunner CVODE integrator interface
**/

#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH

// == INCLUDES ================================================

#include "Integrator.h"
#include "rrRoadRunnerOptions.h"

#include <string>
#include <vector>

// == CODE ====================================================

/**
* CVode vector struct
*/
typedef struct _generic_N_Vector *N_Vector;

namespace rr
{
    using std::string;

    class ExecutableModel;
    class RoadRunner;

    /**
     * @author WBC, ETS, MTK
     * @brief A RoadRunner integrator based on CVODE; serves as RoadRunner's main integrator for ODEs
     * @details Provides a wrapper around CVODE from the SUNDIALS suite.
     * CVODE has two main solvers: an Adams-Moulton solver for non-stiff problems,
     * and a backward differentiation formula (BDF) solver for stiff problems.
     * See: https://computation.llnl.gov/casc/sundials/documentation/toms_sundials.pdf
     */
    class CVODEIntegrator : public Integrator
    {

    public:
        /**
         * @author WBC, ETS, MTK
         * @brief Constructor: takes an executable model, does not own the pointer
         */
        CVODEIntegrator(ExecutableModel* oModel);

        /**
         * @author WBC, ETS, MTK
         * @brief Destructor
         */
        virtual ~CVODEIntegrator();

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m);

        // ** Loading Settings *************************************************

        void loadConfigSettings();

        /**
         * @author WBC
         * @brief Load an SBML settings file and apply the configuration options
         * @note Can assign relative and absolute tolerances
         */
        void loadSBMLSettings(const std::string& filename);

        // ** Meta Info ********************************************************

        /**
         * @author WBC
         * @brief Get the name for this integrator
         * @note Delegates to @ref getName
         */
        std::string getName() const;

        /**
         * @author JKM
         * @brief Get the name for this integrator
         */
        static std::string getCVODEIntegratorName();

        /**
         * @author WBC
         * @brief Get the description for this integrator
         * @note Delegates to @ref getDescription
         */
        std::string getDescription() const;

        /**
         * @author JKM
         * @brief Get the description for this integrator
         */
        static std::string getCVODEIntegratorDescription();

        /**
         * @author WBC
         * @brief Get the hint for this integrator
         * @note Delegates to @ref getHint
         */
        std::string getHint() const;

        /**
         * @author JKM
         * @brief Get the hint for this integrator
         */
        static std::string getCVODEIntegratorHint();

        // ** Getters / Setters ************************************************

        /**
         * @author WBC, ETS, MTK
         * @brief Always deterministic for CVODE
         */
        IntegrationMethod getIntegrationMethod() const;

        /**
         * @author WBC, ETS, MTK
         * @brief Sets the value of an integrator setting (e.g. absolute_tolerance)
         */
        void setValue(std::string setting, const Variant& value);

        /**
        * @author JKM
        * @brief Reset all integrator settings to their respective default values
        */
        void resetSettings();

        /**
         * @author JKM, WBC, ETS, MTK
         * @brief Fix tolerances for SBML tests
         * @details In order to ensure that the results of the SBML test suite
         * remain valid, this method enforces a lower bound on tolerance values.
         * Sets minimum absolute and relative tolerances to
         * Config::CVODE_MIN_ABSOLUTE and Config::CVODE_MIN_RELATIVE resp.
         */
        void tweakTolerances();

        // ** Integration Routines *********************************************

        /**
         * @author WBC, ETS, MTK
         * @brief Main integration routine
         */
        double integrate(double t0, double tf);

        /**
         * @author WBC, ETS, MTK
         * @brief Reset time to zero and reinitialize model
         * @details Applies events which occur before time zero.
         * Reinitializes CVODE and the executable model.
         */
        void restart(double timeStart);

        // ** Listeners ********************************************************

        /**
         * @author WBC, ETS
         * @brief Gets the integrator listener
         */
        IntegratorListenerPtr getListener();

        /**
         * @author WBC, ETS
         * @brief Sets the integrator listener
         */
        void setListener(IntegratorListenerPtr);

        /**
         * @author JKM
         * @brief Does a RT type check which throws if it fails, EVEN IF RTTI IS DISABLED
         */
        void checkType() const;

        /**
         * @brief decode the cvode error code to a string
         */
        std::string cvodeDecodeError(int cvodeError, bool exInfo = true);

    private:
        static const int mDefaultMaxNumSteps;
        static const int mDefaultMaxAdamsOrder;
        static const int mDefaultMaxBDFOrder;

        void* mCVODE_Memory;
        N_Vector mStateVector;
        ExecutableModel* mModel;

        IntegratorListenerPtr listener;
        double lastEventTime;
        bool variableStepPendingEvent;
        bool variableStepTimeEndEvent;
        double *variableStepPostEventState;
        std::vector<unsigned char> eventStatus;

        void testRootsAtInitialTime();
        bool haveVariables();
        void assignResultsToModel();
        /**
         * @author WBC, ETS, JKM
         * @brief Propagates changes in the "absolute_tolerance" and
         * "relative_tolerance" settings to the CVODE library.
         */
        void setCVODETolerances();
        void reInit(double t0);

        /**
         * @author WBC, ETS, JKM
         * @brief Propagates changes in the following settings to CVODE:\n
         * *  initial_time_step \n
         * *  minimum_time_step \n
         * *  maximum_time_step \n
         * *  maximum_num_steps \n
         * *  absolute_tolerance (via @ref setCVODETolerances) \n
         * *  relative_tolerance (via @ref setCVODETolerances) \n
         */
        void updateCVODE();
        void applyPendingEvents(double timeEnd);
        void applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus);
        double applyVariableStepPendingEvents();

        void createCVode();
        void freeCVode();
        bool stateVectorVariables;


        friend int cvodeDyDtFcn(double t, N_Vector cv_y, N_Vector cv_ydot, void *f_data);
        friend int cvodeRootFcn(double t, N_Vector y, double *gout, void *g_data);

        unsigned long typecode_;
    };


    // ** Registration *********************************************************


    class CVODEIntegratorRegistrar : public IntegratorRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return CVODEIntegrator::getCVODEIntegratorName();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return CVODEIntegrator::getCVODEIntegratorDescription();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return CVODEIntegrator::getCVODEIntegratorHint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual Integrator* construct(ExecutableModel *model) const {
                return new CVODEIntegrator(model);
            }
    };
}

#endif
