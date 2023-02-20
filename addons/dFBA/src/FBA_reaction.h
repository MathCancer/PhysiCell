/*
 * Reaction.h
 *
 *  Created on: 13 jun. 2019
 *      Author: mponce
 */

#ifndef SRC_REACTION_H_
#define SRC_REACTION_H_

#include <iostream>
#include <vector>
#include <map>

#include "FBA_metabolite.h"

class FBA_reaction
{
	private:
		std::string id;
		std::string name;

		double lowerBound;
		double upperBound;
		double objectiveCoefficient;
		double fluxValue;

		std::map<const FBA_metabolite*, double> metabolites;
		std::map<std::string, const FBA_metabolite*> idMetaboliteMap;

	public:
		FBA_reaction(std::string id);
		~FBA_reaction();

		const std::string& getId() const;

		void setName(std::string name);
		const std::string& getName() const;

		int getNumberOfMetabolites();

		void setLowerBound(double lowerBound);
		double getLowerBound();

		void setUpperBound(double upperBound);
		double getUpperBound();

		void setObjectiveCoefficient(double ojectiveCoefficient);
		double getObjectiveCoefficient();

		void setFluxValue(double flux_value);
		double getFluxValue();

		bool reversible();
		bool hasMetabolite(std::string mId);
		void addMetabolite(const FBA_metabolite* met, double stoich);

		std::vector<std::string> getReactants();
		std::vector<std::string> getProducts();
		double getStoichCoefficient(std::string mId);

		std::string getReactionString();

		const std::map<const FBA_metabolite*, double>& getMetabolites() const;
};



#endif /* SRC_REACTION_H_ */
