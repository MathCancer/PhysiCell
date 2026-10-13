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

#include "dfba_Metabolite.h"
#include "dfba_Solution.h"

LIBSBML_CPP_NAMESPACE_USE

// Forward declaration of dFBAReaction
class dFBAReaction;

class dFBAModel
{
	private:
		/** \brief Model ID */
		std::string id;

		/** \brief vector of metabolite objects*/
		std::vector<dFBAMetabolite*> metabolites;

		/** \brief Saved objective reaction ID for restore */
		std::string original_objective_reaction;

		/** \brief Saved objective coefficients for restore */
		std::map<std::string, double> original_objective_coefficients;

		/** \brief map between metabolites' ids and metabolites' references **/
		std::map<std::string, int> metaboliteIndexer;

		/** \brief vector of reaction objects*/
		std::vector<dFBAReaction*> reactions;

		/** \brief map between reaction IDs and reaction references */
		std::map< std::string, int> reactionsIndexer;

		/** \brief solution  */
		dFBASolution solution;

		/** \brief Coin CLP simplex model to encode the FBA problem**/
		ClpSimplex problem;
		
		CoinMessageHandler* handler;

		bool is_initialized = false;

	public:

		/** \brief Constructor */
		dFBAModel();

		/** \brief Destructor */
		~dFBAModel();

		/** \brief Copy */
		dFBAModel(const dFBAModel& copy);

		/** \brief new assignment operator */
		dFBAModel& operator=(const dFBAModel& other);

		/** \brief Clear function */
		void clear();

		/** \brief Check if the model is initialized*/
		bool isInitialized();

		/** \brief Check if there is a metaboltie with a given ID*/
		bool hasMetabolite(const std::string& id) const;

		/** \brief a metabolite pointer using a string Id*/
		dFBAMetabolite* getMetabolite(const std::string& mId) const;

		/** \brief Add new metabolite to the model*/
		dFBAMetabolite* addMetabolite(const std::string& id);

		
		/** \brief Check if there is a reaction with a given ID*/
		bool hasReaction(std::string rId);
		
		/** \brief Get a reaction pointer using string ID*/
		dFBAReaction* getReaction(std::string rId);

		/** \brief Add new reaction to the model*/
		void addReaction(dFBAReaction* rxn);

		/** \brief Get the integer index of a reaction*/
		const int getReactionIndex(std::string rId);
		
		/** \brief Get the upper bound of a reactions*/
		double getReactionUpperBound(std::string rId);
		
		/** \brief Set the upper bound of a reactions*/
		void setReactionUpperBound(std::string rId, double upperBound);
		
		/** \brief Get the upper bound of a reactions*/
		double getReactionLowerBound(std::string rId);
		
		/** \brief Set the lower bound of a reactions*/
		void setReactionLowerBound(std::string rId, double lowerBound);

		/** \brief Get the number of model reactions*/
		const int getNumReactions();

		/** \brief Get the number of model metabolites*/
		const int getNumMetabolites();

		/** \brief Get a metabolite pointer using s string Id*/
		const std::vector<dFBAMetabolite*> getListOfMetabolites() const;

		/** \brief Get the list of reaction pointers*/
		const std::vector<dFBAReaction*> getListOfReactions() const;

			/** \brief Get the list of reaction pointers*/
		std::vector<dFBAReaction*> getListOfBoundaryReactions();

		/** \brief Get the list of IDs of the boundary reactions*/
		std::vector<std::string> getListOfBoundaryReactionIds();

		/** \brief Get the ClpSimplex model */
		const ClpSimplex* getLpModel() const;

		/** \brief Parse and read a metabolic model from a SBML file*/
		void readSBMLModel(const char* sbmlFileName);
		
		/** \brief Initialize the ClpSimplex model */
		void initProblem();

		/** \brief Initialize the CBM model and the LP problem */
		void initModel(const char* sbmlFileName);

		/** \brief Write problem in LP format */
		void writeProblem(const char *filename);

		/** \brief Run FBA using primal method */
		dFBASolution optimize();

		/** \brief Get solution status */
		std::string getId() { return this->id; };

		/** \brief Get solution status */
		bool getSolutionStatus();

		/** \brief Get objective value */
		double getObjectiveValue();

		dFBASolution getSolution(){ return this->solution; }

		void setObjectiveCoefficient(std::string rId, double coefficient);
		void clearObjective();

		// Deep copy from another model (cleans up existing data first)
		void deepCopy(const dFBAModel& source);

		// Save/restore objective function state
		void saveObjectiveState();
		void restoreObjectiveState();
};

#endif
