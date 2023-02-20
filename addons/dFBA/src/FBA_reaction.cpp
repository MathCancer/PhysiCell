/*
 * Reaction.cpp
 *
 *  Created on: 13 jun. 2019
 *      Author: mponce
 */

#include "FBA_reaction.h"

#include <iostream>
#include <vector>
#include <map>

#include "FBA_metabolite.h"



FBA_reaction::FBA_reaction(std::string id)
{
    this->id = id;
    this->name = "";
    this->lowerBound = 0;
    this->upperBound = 1000;
    this->objectiveCoefficient = 0;
    this->fluxValue = 0;
}

FBA_reaction::~FBA_reaction()
{

}

const std::string & FBA_reaction::getId() const
{
    return this->id;
}

void FBA_reaction::setName(std::string name)
{
    this->name = name;
}

const std::string & FBA_reaction::getName() const
{
    return this->name;
}

void FBA_reaction::setLowerBound(double lowerBound)
{
    this->lowerBound = lowerBound;
}

double FBA_reaction::getLowerBound()
{
    return this->lowerBound;
}

void FBA_reaction::setUpperBound(double upperBound)
{
    this->upperBound = upperBound;
}

double FBA_reaction::getUpperBound()
{
    return this->upperBound;
}

void FBA_reaction::setObjectiveCoefficient(double ojectiveCoefficient)
{
    this->objectiveCoefficient = ojectiveCoefficient;
}

double FBA_reaction::getObjectiveCoefficient()
{
    return this->objectiveCoefficient;
}

void FBA_reaction::setFluxValue(double fluxValue)
{
    this->fluxValue = fluxValue;
}

double FBA_reaction::getFluxValue()
{
    return this->fluxValue;
}

int FBA_reaction::getNumberOfMetabolites()
{
    return this->metabolites.size();
}

const std::map < const FBA_metabolite *, double >&FBA_reaction::getMetabolites() const
{
    return this->metabolites;
}

bool FBA_reaction::reversible()
{
    return lowerBound < 0;
}

bool FBA_reaction::hasMetabolite(std::string mId)
{
    std::map < std::string, const FBA_metabolite *>::iterator it;
    it = this->idMetaboliteMap.find(mId);
    bool hasMetabolite = (it != this->idMetaboliteMap.end());
    return hasMetabolite;
}


void FBA_reaction::addMetabolite(const FBA_metabolite * met, double stoich)
{
    if (this->hasMetabolite(met->getId()))
    {
        this->metabolites[met] += stoich;
    }
    else
    {
        this->idMetaboliteMap[met->getId()] = met;
        this->metabolites[met] = stoich;
    }
}

std::vector < std::string > FBA_reaction::getReactants()
{
    std::vector < std::string > reactants;
    for (auto itr = this->metabolites.begin();
            itr != this->metabolites.end(); ++itr)
    {

        const FBA_metabolite *met = itr->first;
        double sotich = itr->second;
        if (sotich < 0)
        {
            std::string mId = met->getId();
            reactants.push_back(mId);
        }
    }

    return reactants;
}

std::vector < std::string > FBA_reaction::getProducts()
{
    std::vector < std::string > products;
    for (auto itr = this->metabolites.begin();
            itr != this->metabolites.end(); ++itr)
    {

        const FBA_metabolite *met = itr->first;
        double sotich = itr->second;
        if (sotich > 0)
        {
            std::string mId = met->getId();
            products.push_back(mId);
        }
    }
    return products;
}

double FBA_reaction::getStoichCoefficient(std::string mId)
{
    double coefficient = 0;
    if (hasMetabolite(mId))
    {
        const FBA_metabolite *met = this->idMetaboliteMap[mId];
        coefficient = this->metabolites[met];
    }
    return coefficient;
}

std::string FBA_reaction::getReactionString()
{
    std::vector < std::string > compounds;
    std::string reactionString = "";

    compounds = this->getReactants();
    for (unsigned int i = 0; i < compounds.size(); i++)
    {
        std::string & mId = compounds[i];
        const FBA_metabolite *met = this->idMetaboliteMap[mId];
        // Change the sign (-) of the coeeficient for printing
        float coeff = -1. * this->getStoichCoefficient(mId);
        if (int (coeff) == coeff)
            reactionString += std::to_string(int (coeff));
        else
            reactionString += std::to_string(coeff);

        //reactionString += " " + met->getName();
        reactionString += " " + met->getId();

        if (i < compounds.size() - 1)
            reactionString += " + ";
    }
    if (reversible())
        reactionString += " <==> ";
    else
        reactionString += " --> ";

    compounds = getProducts();
    for (unsigned int i = 0; i < compounds.size(); i++)
    {
        const std::string & mId = compounds[i];
        const FBA_metabolite *met = this->idMetaboliteMap[mId];
        float coeff = getStoichCoefficient(mId);
        if (int (coeff) == coeff)
            reactionString += std::to_string(int (coeff));
        else
            reactionString += std::to_string(coeff);

        reactionString += " " + met->getId();

        if (i < compounds.size() - 1)
            reactionString += " + ";
    }
    return reactionString;
}
