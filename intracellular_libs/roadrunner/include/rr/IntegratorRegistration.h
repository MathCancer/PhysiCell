// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file IntegratorRegistration.h
* @author JKM
* @date 07/31/2015
* @copyright Apache License, Version 2.0
* @brief Registers the integrators at startup
**/

#ifndef RR_INTEGRATOR_REGISTRATION_H_
#define RR_INTEGRATOR_REGISTRATION_H_

// == INCLUDES ================================================

// == CODE ====================================================

namespace rr
{

    /**
    * @author JKM
    * @brief Registers all integrators at startup via @ref IntegratorRegistrationMgr::Register
    */
    class IntegratorRegistrationMgr {
        public:
            /**
             * @author JKM
             * @brief Registers all integrators at startup
             * @details Is run at first instantiation of @ref RoadRunner.
             * Subsequent calls have no effect.
             */
            static void Register();
    };

}

#endif /* RR_INTEGRATOR_REGISTRATION_H_ */
