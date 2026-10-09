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

		/**
		 * \brief whether optimize() also fills dFBASolution::fluxes.
		 *
		 * Off by default. Populating that string-keyed map costs one map
		 * insertion per reaction on every solve, which dominated optimize()
		 * for cells solving at every intracellular step. Nothing in the addon
		 * reads it; use getFluxDistribution() instead, which builds the same
		 * map on demand from the reactions.
		 */
		bool track_flux_map = false;

		// ---------------- parsimonious FBA (second stage) ----------------
		//
		// pFBA answers the alternative-optima problem: standard FBA fixes the
		// objective value but not the flux distribution, and in dFBA the flux
		// distribution is what couples the cell to BioFVM. The second stage
		// pins the objective at its optimum and then minimises total flux,
		// which selects the enzyme-parsimonious solution among the equally
		// optimal ones.
		//
		// It is held as a SEPARATE ClpSimplex rather than by reformulating the
		// existing one, so that a run with pFBA disabled solves exactly the
		// problem it solved before: byte-identical results, no silent change
		// to anyone's existing simulations.

		/** \brief true once the second-stage problem has been built */
		bool pfba_enabled = false;

		/** \brief objective must stay at least this fraction of its optimum */
		double pfba_fraction_of_optimum = 1.0;

		/** \brief the second-stage LP: original columns plus split columns */
		ClpSimplex pfba_problem;
		CoinMessageHandler* pfba_handler = nullptr;
		bool pfba_built = false;

		/**
		 * \brief for each reaction, its two split columns in the second-stage
		 * problem, or -1 when the reaction is not split.
		 *
		 * Only reversible INTERNAL reactions are split. Exchange reactions are
		 * excluded deliberately: they are auxiliary constructs linking the model
		 * to BioFVM rather than enzymatic steps, they carry no gene and no
		 * enzyme cost, and leaving them unsplit keeps the flux-to-net_export
		 * mapping reading the same single column it always did.
		 */
		std::vector<int> pfba_pos_col;
		std::vector<int> pfba_neg_col;

		/** \brief build the second-stage problem from the current model */
		void buildParsimoniousProblem();

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

		/** \brief Flux of every reaction from the last solve, built on demand */
		std::map<std::string, double> getFluxDistribution() const;

		/** \brief Whether optimize() also fills dFBASolution::fluxes (default false) */
		bool getTrackFluxMap() const;
		void setTrackFluxMap(bool value);

		// ================ parsimonious FBA ================

		/**
		 * \brief Enable the parsimonious second stage.
		 *
		 * \param fraction_of_optimum the objective is held at or above this
		 *        fraction of its stage-one optimum while total flux is
		 *        minimised. 1.0 asks for the exact optimal face.
		 */
		void enableParsimonious(double fraction_of_optimum = 1.0);
		bool isParsimoniousEnabled() const { return this->pfba_enabled; }
		double getParsimoniousFraction() const { return this->pfba_fraction_of_optimum; }

		/**
		 * \brief Minimise total flux subject to the objective staying optimal.
		 *
		 * Call after optimize() has returned an optimal solution. On success
		 * every reaction's flux value is replaced by the parsimonious one and
		 * the returned solution carries the total-flux value as its objective;
		 * the growth rate must therefore still be taken from the stage-one
		 * solution, not from this one.
		 *
		 * If the second stage does not solve, the stage-one fluxes are left in
		 * place and the status reports the failure. A solver failure here is
		 * not a biological outcome and must not be read as cell death.
		 */
		dFBASolution optimizeParsimonious(double stage_one_objective);

		/** \brief Sum of absolute fluxes over the reactions pFBA minimises */
		double getTotalAbsoluteFlux() const;

		void setObjectiveCoefficient(std::string rId, double coefficient);
		void clearObjective();

		// Deep copy from another model (cleans up existing data first)
		void deepCopy(const dFBAModel& source);

		// Save/restore objective function state
		void saveObjectiveState();
		void restoreObjectiveState();
};

#endif
