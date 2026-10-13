/*
 * dfba_Reaction.h
 *
 *  Created on: jun. 2022
 *      Author: mponce
 */

#ifndef SRC_REACTION_H_
#define SRC_REACTION_H_

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "dfba_Metabolite.h"

// Forward declaration of dFBAModel
class dFBAModel;

class dFBAReaction
{
	private:
		std::string id;
		std::string name;

		double lowerBound;
		double upperBound;
		double objectiveCoefficient;
		double fluxValue;

		// Map of metabolite IDs to stoichiometric coefficients
    	std::map<std::string, double> metabolites;

	public:
		/** \brief Constructor */
		dFBAReaction(std::string id);

		/** \brief Destructor */
		~dFBAReaction();

		/** \brief Copy */
		dFBAReaction(const dFBAReaction& copy);

		const std::string& getId() const;

		void setName(std::string name);
		const std::string& getName() const;

		int getNumberOfMetabolites() const;

		void setLowerBound(double lowerBound);
		double getLowerBound();

		void setUpperBound(double upperBound);
		double getUpperBound();

		void setObjectiveCoefficient(double ojectiveCoefficient);
		double getObjectiveCoefficient() const;

		void setFluxValue(double flux_value);
		double getFluxValue();

		bool reversible() const;
		bool hasMetabolite(const std::string& mId) const;
		void addMetabolite(const std::string& mId, double stoich);

		std::vector<std::string> getReactants() const;
    	std::vector<std::string> getProducts() const;
		double getStoichCoefficient(const std::string& mId) const;

		std::string getReactionString(const dFBAModel& model) const;

		const std::map<std::string, double>& getMetabolites() const;
};



#endif /* SRC_REACTION_H_ */
