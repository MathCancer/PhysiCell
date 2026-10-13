/*
 * dfba_Reaction.cpp
 *
 *  Created on: 13 jun. 2019
 *      Author: mponce
 */

#include "dfba_Reaction.h"
#include "dfba_Model.h"  // Include the full definition

dFBAReaction::dFBAReaction(std::string id)
{
    this->id = id;
    this->name = "";
    this->lowerBound = 0;
    this->upperBound = 1000;
    this->objectiveCoefficient = 0;
    this->fluxValue = 0;
}

dFBAReaction::~dFBAReaction() {
}



dFBAReaction::dFBAReaction(const dFBAReaction& copy) {
    // Copy primitive members
    this->id = copy.id;
    this->name = copy.name;
    this->lowerBound = copy.lowerBound;
    this->upperBound = copy.upperBound;
    this->objectiveCoefficient = copy.objectiveCoefficient;
    this->fluxValue = copy.fluxValue;
    this->metabolites = copy.metabolites;
}




const std::string & dFBAReaction::getId() const
{
    return this->id;
}

void dFBAReaction::setName(std::string name)
{
    this->name = name;
}

const std::string & dFBAReaction::getName() const
{
    return this->name;
}

void dFBAReaction::setLowerBound(double lowerBound)
{
    this->lowerBound = lowerBound;
}

double dFBAReaction::getLowerBound()
{
    return this->lowerBound;
}

void dFBAReaction::setUpperBound(double upperBound)
{
    this->upperBound = upperBound;
}

double dFBAReaction::getUpperBound()
{
    return this->upperBound;
}

void dFBAReaction::setObjectiveCoefficient(double ojectiveCoefficient)
{
    this->objectiveCoefficient = ojectiveCoefficient;
}

double dFBAReaction::getObjectiveCoefficient() const
{
    return this->objectiveCoefficient;
}

void dFBAReaction::setFluxValue(double fluxValue)
{
    this->fluxValue = fluxValue;
}

double dFBAReaction::getFluxValue()
{
    return this->fluxValue;
}

int dFBAReaction::getNumberOfMetabolites() const {
    return this->metabolites.size();
}

const std::map<std::string, double>& dFBAReaction::getMetabolites() const {
    return this->metabolites;
}

bool dFBAReaction::reversible() const
{
    return lowerBound < 0;
}

bool dFBAReaction::hasMetabolite(const std::string& mId) const
{
    return this->metabolites.find(mId) != this->metabolites.end();
}

void dFBAReaction::addMetabolite(const std::string& mId, double stoich) {
    auto it = metabolites.find(mId);
    if (it != metabolites.end()) {
        // If the metabolite is already involved, just update the stoichiometric coefficient
        it->second += stoich;
    } else {
        // Otherwise, add a new metabolite with its stoichiometric coefficient
        metabolites[mId] = stoich;
    }
}


std::vector<std::string> dFBAReaction::getReactants() const
{
    std::vector<std::string> reactants;

    for (const auto& metabolite : this->metabolites)
    {
        const std::string& mId = metabolite.first;
        double stoich = metabolite.second;

        if (stoich < 0) // Reactants have negative stoichiometric coefficients
        {
            reactants.push_back(mId);
        }
    }

    return reactants;
}


std::vector<std::string> dFBAReaction::getProducts() const
{
    std::vector<std::string> products;

    for (const auto& metabolite : this->metabolites)
    {
        const std::string& mId = metabolite.first;
        double stoich = metabolite.second;

        if (stoich > 0) // Products have positive stoichiometric coefficients
        {
            products.push_back(mId);
        }
    }

    return products;
}


double dFBAReaction::getStoichCoefficient(const std::string& mId) const
{
    // Check if the metabolite exists in the map
    auto it = this->metabolites.find(mId);
    if (it != this->metabolites.end())
    {
        return it->second; // Return the stoichiometric coefficient if found
    }

    return 0.0; // Return 0 if metabolite is not found
}

std::string dFBAReaction::getReactionString(const dFBAModel& model) const {
    std::vector<std::string> compounds;
    std::string reactionString = "";

    // Get reactants
    compounds = this->getReactants();
    for (unsigned int i = 0; i < compounds.size(); i++) {
        const std::string& mId = compounds[i];
        const dFBAMetabolite* met = model.getMetabolite(mId);  // Accessing the actual metabolite

        // Update reaction string using the metabolite details
        if (met) {
            double coeff = -1.0 * this->getStoichCoefficient(mId);
            reactionString += std::to_string(coeff) + " " + met->getId();
            if (i < compounds.size() - 1) {
                reactionString += " + ";
            }
        }
    }

    reactionString += reversible() ? " <==> " : " --> ";

    // Get products (similar approach)
    compounds = getProducts();
    for (unsigned int i = 0; i < compounds.size(); i++) {
        const std::string& mId = compounds[i];
        const dFBAMetabolite* met = model.getMetabolite(mId);

        if (met) {
            double coeff = this->getStoichCoefficient(mId);
            reactionString += std::to_string(coeff) + " " + met->getId();
            if (i < compounds.size() - 1) {
                reactionString += " + ";
            }
        }
    }

    return reactionString;
}

