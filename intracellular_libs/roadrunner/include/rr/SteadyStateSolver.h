// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file SteadyStateSolver.h
* @author ETS, WBC, JKM
* @date Sep 7, 2013
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner solvers
**/

# ifndef RR_STEADY_STATE_SOLVER_H_
# define RR_STEADY_STATE_SOLVER_H_

// == INCLUDES ================================================

# include "rrLogger.h"
# include "rrOSSpecifics.h"
# include "Dictionary.h"
# include "rrException.h"
# include "Solver.h"

# include "tr1proxy/rr_memory.h"
# include "tr1proxy/rr_unordered_map.h"
# include <stdexcept>

// == CODE ====================================================

namespace rr
{
    class ExecutableModel;


    /*-------------------------------------------------------------------------------------------
        SteadyStateSolver is an abstract base class that provides an interface to specific steady-state solver
        class implementations.
    ---------------------------------------------------------------------------------------------*/
    class RR_DECLSPEC SteadyStateSolver : public Solver
    {
    public:

        virtual ~SteadyStateSolver() {};

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m) = 0;

        virtual double solve() = 0;

        virtual void loadConfigSettings();
    };

    /**
     * @author JKM, WBC
     * @brief Handles constructing a solver and contains meta
     * information about it
     */
    class RR_DECLSPEC SteadyStateSolverRegistrar
    {
    protected:
        typedef SteadyStateSolver* (*SteadyStateSolverCtor)(ExecutableModel *model);
    public:
        virtual ~SteadyStateSolverRegistrar();

        /**
         * @author JKM, WBC
         * @brief Gets the name associated with this integrator type
         */
        virtual std::string getName() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the description associated with this integrator type
         */
        virtual std::string getDescription() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the hint associated with this integrator type
         */
        virtual std::string getHint() const = 0;

        /**
         * @author JKM, WBC
         * @brief Constructs a new integrator of a given type
         */
        virtual SteadyStateSolver* construct(ExecutableModel *model) const = 0;
    };

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::SteadyStateSolver.
     */
    class RR_DECLSPEC SteadyStateSolverFactory
    {
    public:
        virtual ~SteadyStateSolverFactory();

        /**
         * @author JKM, WBC
         * @brief Constructs a new solver given the name
         * (e.g. cvode, gillespie)
         */
        SteadyStateSolver* New(std::string name, ExecutableModel *m) const;

        /**
         * @author JKM, WBC
         * @brief Registers a new solver with the factory
         * so that it can be constructed
         * @details Should be called at startup for new solvers.
         */
        void registerSteadyStateSolver(SteadyStateSolverRegistrar* i);

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the solver factory
         */
        static SteadyStateSolverFactory& getInstance();

        // ** Indexing *********************************************************

        std::size_t getNumSteadyStateSolvers() const;

		std::vector<std::string> getListSteadyStateSolverNames();

        std::string getSteadyStateSolverName(std::size_t n) const;

        std::string getSteadyStateSolverHint(std::size_t n) const;

        std::string getSteadyStateSolverDescription(std::size_t n) const;

    private:
        /**
         * @author JKM, WBC
         * @brief Prevents external instantiation
         */
        SteadyStateSolverFactory() {}
        typedef std::vector<SteadyStateSolverRegistrar*> SteadyStateSolverRegistrars;
        SteadyStateSolverRegistrars mRegisteredSteadyStateSolvers;
    };

}

# endif /* RR_STEADY_STATE_SOLVER_H_ */
