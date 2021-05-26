// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Integrator.h
* @author ETS, WBC, JKM
* @date Apr 23, 2014
* @copyright Apache License, Version 2.0
* @brief RoadRunner's Gillespie SSA integrator
**/

#ifndef GILLESPIEINTEGRATOR_H_
#define GILLESPIEINTEGRATOR_H_

// == INCLUDES ================================================

#include "Integrator.h"
#include "rrRoadRunnerOptions.h"
#include "rrExecutableModel.h"
#include "tr1proxy/rr_random.h"

// == CODE ====================================================

namespace rr
{

    class ExecutableModel;

    /**
     * @author WBC, ETS
     * @brief RoadRunner's implementation of the Gillespie SSA
     */
    class GillespieIntegrator: public Integrator
    {
    public:
        GillespieIntegrator(ExecutableModel* model);
        virtual ~GillespieIntegrator();

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m);

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
        static std::string getGillespieName();

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
        static std::string getGillespieDescription();

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
        static std::string getGillespieHint();

        // ** Getters / Setters ************************************************

        /**
         * @author WBC, ETS
         * @brief Always stochastic for Gillespie
         */
        IntegrationMethod getIntegrationMethod() const;

        /**
         * @author WBC, ETS
         * @brief Sets the value of an integrator setting (e.g. absolute_tolerance)
         */
        void setValue(std::string setting, const Variant& value);

        /**
        * @author JKM
        * @brief Reset all integrator settings to their respective default values
        */
        void resetSettings();

        // ** Integration Routines *********************************************

        /**
         * @author WBC, ETS
         * @brief Main integration routine
         */
        double integrate(double t0, double tf);

        /**
         * @author WBC, ETS
         * @brief Reset time to zero and reinitialize model
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

    private:
        ExecutableModel *model;
        cxx11_ns::mt19937 engine;
        //unsigned long seed;
        double timeScale;
        double stoichScale;
        int nReactions;
        int floatingSpeciesStart;		// starting index of floating species
        double* reactionRates;
        double* reactionRatesBuffer;
        int stateVectorSize;
        double* stateVector;
        double* stateVectorRate;
        // m rows x n cols
        // offset = row*NUMCOLS + column
        int stoichRows;
        int stoichCols;
        double* stoichData;
        std::vector<unsigned char> eventStatus;
        std::vector<unsigned char> previousEventStatus;

        void testRootsAtInitialTime();
        void applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus);

        double urand();
        void setEngineSeed(unsigned long seed);
        unsigned long getSeed() const;

        inline double getStoich(uint species, uint reaction)
        {
            return stoichData[species * stoichCols + reaction];
        }

        /**
        * @author JKM
        * @brief Initialize model-specific variables
        * @details Called whenever a model is loaded or a Gillespie
        * integrator is constructed
        */
        void initializeFromModel();
    };


    // ** Registration *********************************************************


    class GillespieIntegratorRegistrar : public IntegratorRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return GillespieIntegrator::getGillespieName();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return GillespieIntegrator::getGillespieDescription();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return GillespieIntegrator::getGillespieHint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual Integrator* construct(ExecutableModel *model) const {
                return new GillespieIntegrator(model);
            }
    };

} /* namespace rr */

#endif /* GILLESPIEINTEGRATOR_H_ */
