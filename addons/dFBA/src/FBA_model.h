/**
 * \brief Wrapper class
 *
 *
 * Created on 06/11/2019
 * M. Ponce-de-Leon, Barcelona Supercomputing Center
 */

#ifndef __FBA_model_h__
#define __FBA_model_h__

#include <iostream>
#include <map>
#include <vector>

#include <sbml/SBMLTypes.h>
#include <sbml/packages/fbc/common/FbcExtensionTypes.h>

#include <coin/CoinPackedMatrix.hpp>
#include <coin/CoinPackedVector.hpp>
#include <coin/ClpSimplex.hpp>


#include "FBA_metabolite.h"
#include "FBA_reaction.h"

LIBSBML_CPP_NAMESPACE_USE

class FBA_model
{
private:
	/** \brief Constraint-Based Model Class to perform FBA*/

	std::string id;

	/** \brief vector of reaction objects*/
	std::vector<FBA_metabolite*> metabolites;

	/** \brief map between metabolites' ids and metabolites' references **/
	std::map<std::string, int> metaboliteIndexer;

	/** \brief vector of reaction objects*/
	std::vector<FBA_reaction*> reactions;

	/** \brief map between reaction IDs and reaction references */
	std::map< std::string, int> reactionsIndexer;

	/** \brief Coin CLP simplex model to encode the FBA problem**/
	ClpSimplex lp_model;
	
	CoinMessageHandler* handler;

	bool is_initialized = false;

public:

	/** \brief Constructor */
	FBA_model();

	/** \brief Destructor */
	~FBA_model();

	/** \brief Check if there is a metaboltie with a given ID*/
	bool hasMetabolite(std::string mId);

	/** \brief a metabolite pointer using a string Id*/
	const FBA_metabolite* getMetabolite(std::string mId);

	/** \brief Add new metabolite to the model*/
	void addMetabolite(FBA_metabolite* met);

	
	/** \brief Check if there is a reaction with a given ID*/
	bool hasReaction(std::string rId);
	
	/** \brief Get a reaction pointer using string ID*/
	FBA_reaction* getReaction(std::string rId);

	/** \brief Add new reaction to the model*/
	void addReaction(FBA_reaction* rxn);

	/** \brief Get the integer index of a reaction*/
	const int getReactionIndex(std::string rId);
	
	/** \brief Get the upper bound of a reactions*/
	float getReactionUpperBound(std::string rId);
	
	/** \brief Set the upper bound of a reactions*/
	void setReactionUpperBound(std::string rId, float upperBound);
	
	/** \brief Get the upper bound of a reactions*/
	float getReactionLowerBound(std::string rId);
	
	/** \brief Set the lower bound of a reactions*/
	void setReactionLowerBound(std::string rId, float lowerBound);

	/** \brief Get the number of model reactions*/
	const int getNumReactions();

	/** \brief Get the number of model metabolites*/
	const int getNumMetabolites();

	/** \brief Get a metabolite pointer using s string Id*/
	const std::vector<FBA_metabolite*> getListOfMetabolites() const;

	/** \brief Get the list of reaction pointers*/
	const std::vector<FBA_reaction*> getListOfReactions() const;

		/** \brief Get the list of reaction pointers*/
	std::vector<FBA_reaction*> getListOfBoundaryReactions();

	/** \brief Get the list of IDs of the boundary reactions*/
	std::vector<std::string> getListOfBoundaryReactionIds();

	/** \brief Get the ClpSimplex model */
	const ClpSimplex* getLpModel() const;

	/** \brief Parse and read a metabolic model from a SBML file*/
	void readSBMLModel(const char* sbmlFileName);
	
	/** \brief Get the ClpSimplex model */
	void initLpModel();

	void initFBAmodel(const char* sbmlFileName);

	/** \brief Write LP problem in MPS format */
	void writeLp(const char *filename);

	/** \brief Run FBA using primal method */
	void runFBA();

	/** \brief Get solution status */
	bool getSolutionStatus();

	/** \brief Get objective value */
	float getObjectiveValue();
};



#endif
