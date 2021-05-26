// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file NLEQSolver.h
* @author ETS, WBC, JKM
* @date 08/03/2015
* @copyright Apache License, Version 2.0
* @brief Provides an interface to NLEQ
**/

#ifndef rrNLEQSolverH
#define rrNLEQSolverH
#include <vector>
#include "SteadyStateSolver.h"
#include "rrExecutableModel.h"
using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQSolver : public SteadyStateSolver
{
    public:
        /**
        * Creates a new Instance of NLEQ for the given Model
        */
        NLEQSolver(ExecutableModel *_model = NULL);
        ~NLEQSolver();

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m);

        void loadConfigSettings();

        /**
        * @author JKM
        * @brief Reset all integrator settings to their respective default values
        */
        void resetSettings();

        /**
        * @author WBC, JKM
        * @brief Get the name for this Solver
        * @note Delegates to @ref getName
        */
        std::string getName() const;

        /**
        * @author JKM
        * @brief Get the name for this Solver
        */
        static std::string getNLEQName();

        /**
        * @author WBC
        * @brief Get the description for this Solver
        * @note Delegates to @ref getDescription
        */
        std::string getDescription() const;

        /**
        * @author JKM
        * @brief Get the description for this Solver
        */
        static std::string getNLEQDescription();

        /**
        * @author WBC
        * @brief Get the hint for this Solver
        * @note Delegates to @ref getHint
        */
        std::string getHint() const;

        /**
        * @author JKM
        * @brief Get the hint for this Solver
        */
        static std::string getNLEQHint();

        // ** Solver routines
        double solve();

    private:
        ExecutableModel *model; // Model generated from the SBML. Static so we can access it from standalone function
};


    // ** Registration *********************************************************


    class NLEQSolverRegistrar : public SteadyStateSolverRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return NLEQSolver::getNLEQName();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return NLEQSolver::getNLEQDescription();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return NLEQSolver::getNLEQHint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual SteadyStateSolver* construct(ExecutableModel *model) const {
                return new NLEQSolver(model);
            }
    };
}

#endif
