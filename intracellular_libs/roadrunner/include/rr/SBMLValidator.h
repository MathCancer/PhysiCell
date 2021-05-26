/*
 * SBMLValidator.h
 *
 *  Created on: Aug 24, 2014
 *      Author: andy
 */

#ifndef SBMLVALIDATOR_H_
#define SBMLVALIDATOR_H_

#include <string>

namespace rr
{
enum ValidateSBML
{

    /**
     * Check consistency of measurement units associated
     * with quantities (SBML L2V4 rules 105nn)
     */
    VALIDATE_UNITS               = (0x1 << 0),  // => 0x00000001

    /**
     * Check correctness and consistency of identifiers used
     * for model entities (SBML L2V4 rules 103nn)
     */
    VALIDATE_IDENTIFIER          = (0x1 << 1),  // => 0x00000001

    /**
     * Check syntax of MathML mathematical expressions
     * (SBML L2V4 rules 102nn)
     */
    VALIDATE_MATHML              = (0x1 << 2),  // => 0x00000001

    /**
     * Check validity of SBO identifiers (if any) used in
     * the model (SBML L2V4 rules 107nn)
     */
    VALIDATE_SBO                 = (0x1 << 3),  // => 0x00000001

    /**
     * Perform static analysis of whether the model is overdetermined
     */
    VALIDATE_OVERDETERMINED      = (0x1 << 4),  // => 0x00000001

    /**
     * Perform additional checks for recommended good modeling practices
     */
    VALIDATE_MODELING_PRACTICE   = (0x1 << 5),  // => 0x00000001

    /**
     * Perform all other general SBML consistency checks
     * (SBML L2V4 rules 2nnnn; highly recommended)
     */
    VALIDATE_GENERAL             = (0x1 << 6),  // => 0x00000001
};

std::string validateSBML(const std::string src, unsigned opt
        = VALIDATE_IDENTIFIER | VALIDATE_GENERAL | VALIDATE_MATHML);

/**
* @author JKM
* @brief Returns true if stoichiometry is defined for every species
* in every reaction.
* @details Missing stoichiometry is not handled by the integrator.
* Must bail or results are undefined.
*/
bool isStoichDefined(const std::string sbml);

/**
* @author JKM
* @brief Adds missing stoichiometry information.
* Assumes unit stoichiometry where not specified
* @return SBML string with stoich fixed
*/
std::string fixMissingStoich(const std::string sbml);

} /* namespace rr */

#endif /* SBMLVALIDATOR_H_ */
