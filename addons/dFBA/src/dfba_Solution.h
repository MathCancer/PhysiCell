/*
 * dfba_Reaction.h
 *
 *  Created on: jun. 2022
 *      Author: mponce
 */

#ifndef SRC_SOLUTION_H_
#define SRC_SOLUTION_H_

#include <iostream>
#include <map>
#include <vector>


class dFBASolution
{
	private:
	    /** \brief Constraint-Based Model Class that contains a solution*/


    public:

        double objective_value;
        std::string status;
        std::map<std::string, double> fluxes;
        std::map<std::string, double> reduced_costs;

        /**
         * \brief outcome of the optional second optimisation stage.
         *
         * "none" when no second stage is configured -- the overwhelmingly
         * common case, and what every pre-pFBA configuration reports.
         * "optimal" when it ran and selected a flux distribution.
         * "infeasible" / "unknown" when it ran and failed, in which case the
         * fluxes are still the stage-one ones and still optimal.
         *
         * objective_value ALWAYS comes from stage one. Stage two minimises
         * total flux, so reporting its objective here would hand callers a sum
         * of fluxes where they expect growth. The second stage's own objective
         * is in total_absolute_flux instead.
         */
        std::string secondary_status;
        double total_absolute_flux;

        dFBASolution();
        dFBASolution(double objective_value, std::string status, std::map<std::string,double> fluxes);
        ~dFBASolution();

        void clear();

        double getObjectiveValue(){ return this->objective_value; }
        std::string getStatus(){ return this->status; };
        std::map<std::string, double> getFluxes(){ return fluxes; };

};

#endif 