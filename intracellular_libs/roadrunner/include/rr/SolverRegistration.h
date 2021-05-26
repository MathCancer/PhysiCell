// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file SolverRegistration.h
* @author JKM
* @date 08/04/2015
* @copyright Apache License, Version 2.0
* @brief Registers the solvers at startup
**/

#ifndef RR_SOLVER_REGISTRATION_H_
#define RR_SOLVER_REGISTRATION_H_

// == INCLUDES ================================================

// == CODE ====================================================

namespace rr
{

    /**
    * @author JKM
    * @brief Registers all solver at startup via @ref SolverRegistrationMgr::Register
    */
    class SolverRegistrationMgr {
        public:
            /**
             * @author JKM
             * @brief Registers all solvers at startup
             * @details Is run at first instantiation of @ref RoadRunner.
             * Subsequent calls have no effect.
             */
            static void Register();
    };

}

#endif /* RR_INTEGRATOR_REGISTRATION_H_ */
