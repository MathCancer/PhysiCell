
// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file rrExecutableModel.h
* @author MTK, ETS, JKM
* @copyright Apache License, Version 2.0
* @brief Base class for all code generators in RoadRunner
**/

# ifndef rrExecutableModelH
# define rrExecutableModelH

// == INCLUDES ================================================

# include "rrOSSpecifics.h"
# include "rrException.h"

# include <stdint.h>
# include <string>
# include <vector>
# include <list>
# include <ostream>


# include "tr1proxy/rr_memory.h"

// == CODE ====================================================

namespace rr
{

class ExecutableModel;

/**
 * @brief Notifies the user of SBML events
 * @details RoadRunner has the capatiblity to notify user objects of any sbml event.
 *
 * In order to listen to sbml events, one simply implements the EventHandler
 * interface and resgisters it with the ExecutableModel::setEventHandler method.
 *
 * To remove it, just pass in a 0.
 *
 * EventHanders are free to change any model parameters.
 *
 * They may return a result value specified by the Result enum. Currently, we
 * we only have the HALT_SIMULATION which will result in RoadRunner::oneStep
 * or RoadRunner::simulate to stop at the current time and return. This may be usefull
 * if someone wants to run a simulation up until some threshold or state is reached.
 */
class EventListener
{
public:
    enum Result
    {
        HALT_SIMULATION               = (0x1 << 0),  // => 0x00000001
    };

    virtual uint onTrigger(ExecutableModel* model, int eventIndex, const std::string& eventId) = 0;
    virtual uint onAssignment(ExecutableModel* model, int eventIndex, const std::string& eventId) = 0;

protected:
    ~EventListener() {};
};

/**
 * listeners are shared objects, so use std smart pointers
 * to manage them.
 */
typedef cxx11_ns::shared_ptr<EventListener> EventListenerPtr;

class EventListenerException: public std::exception
{
public:
    explicit EventListenerException(uint resultCode) :
            resultCode(resultCode)
    {
        msg = "EventHandlerException, resultCode: ";

        switch (resultCode)
        {
        case EventListener::HALT_SIMULATION:
            msg += "HALT_SIMULATION";
            break;
        }
    }

    virtual ~EventListenerException() throw()
    {
    };

    virtual const char* what() const throw()
    {
        return msg.c_str();
    }

    uint getResultCode() const
    {
        return resultCode;
    }

private:
    uint resultCode;
    std::string msg;
};



/**
 * @brief Base class for all code generation systems; allows compiling
 * and evaluating the model
 * @details The ExecutableModel interface provides a way to access an
 * sbml model that was compiled, JIT'd or interpreted
 * as executable (runnable) module.
 *
 * An ExecutableModel holds a ModelData structure, all the simulation
 * values are stored in the ModelData struct, i.e. the dynamic state
 * of the model is fully contained in the ModelData structure.
 *
 * An ExecutableModel shoud also contain all of the initial condisions,
 * rules, functions and whatever other semantic information that was
 * specified in the sbml model.
 */
class RR_DECLSPEC ExecutableModel
{
public:
    /**
     * @author JKM
     * @date 07/31/2015
     * @brief Returns a human-readable description of the code generation backend,
     * e.g. LLVM, legacy C, etc.
     */
    virtual std::string getExecutableModelDesc() const = 0;

    /**
     * get the name of the model
     */
    virtual std::string getModelName() = 0;

    virtual void setTime(double _time) = 0;
    virtual double getTime() = 0;


    /**
     * Loads the initial conditions into the current model state.
     *
     * Initial conditions may have been updated at any time externally.
     */
    virtual void reset() = 0;



    /************************ Floating Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * dependent species are defined by rules and the only way to change them
     * is by changing the values on which they depend.
     */
    virtual int getNumDepFloatingSpecies() = 0;

    /**
     * total number of floating species.
     */
    virtual int getNumFloatingSpecies() = 0;

    virtual int getFloatingSpeciesIndex(const std::string& eid) = 0;
    virtual std::string getFloatingSpeciesId(int index) = 0;

    /**
     * independent species do are not defined by rules, they typically participate
     * in reactions and can have thier values set at any time.
     */
    virtual int getNumIndFloatingSpecies() = 0;

    /**
     * get the floating species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesAmounts(int len, int const *indx,
            double *values) = 0;

    virtual int setFloatingSpeciesAmounts(int len, int const *indx,
            const double *values) = 0;

    virtual int getFloatingSpeciesAmountRates(int len, int const *indx,
            double *values) = 0;


    virtual int getFloatingSpeciesConcentrationRates(int len, int const *indx,
                double *values) = 0;

    /**
     * get the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesConcentrations(int len, int const *indx,
            double *values) = 0;

    /**
     * set the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setFloatingSpeciesConcentrations(int len, int const *indx,
            double const *values) = 0;

    /**
     * Set the initial concentrations of the floating species.
     *
     * Takes the same indices as the other floating species methods.
     *
     * Note, if a floating species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setFloatingSpeciesInitConcentrations(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial concentrations of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitConcentrations(int len, int const *indx,
                    double *values) = 0;

    /**
     * Set the initial amounts of the floating species.
     *
     * Takes the same indices as the other floating species methods.
     *
     * Note, if a floating species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setFloatingSpeciesInitAmounts(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial amounts of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitAmounts(int len, int const *indx,
                    double *values) = 0;

    /************************ End Floating Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/



    /************************ Boundary Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/


    /**
     * get the number of boundary species.
     */
    virtual int getNumBoundarySpecies() = 0;
    virtual int getBoundarySpeciesIndex(const std::string &eid) = 0;
    virtual std::string getBoundarySpeciesId(int index) = 0;

    /**
     * get the boundary species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesAmounts(int len, int const *indx,
            double *values) = 0;


    /**
     * get the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesConcentrations(int len, int const *indx,
            double *values) = 0;

    /**
     * set the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setBoundarySpeciesConcentrations(int len, int const *indx,
            double const *values) = 0;


    /************************ End Boundary Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Global Parameters Section ***************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * get the number of global parameters
     */
    virtual int getNumGlobalParameters() = 0;

    /**
     * index of the global parameter id, -1 if it does not exist.
     */
    virtual int getGlobalParameterIndex(const std::string& eid) = 0;

    /**
     * id of the indexed global parameter.
     */
    virtual std::string getGlobalParameterId(int index) = 0;

    /**
     * get the global parameter values
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getGlobalParameterValues(int len, int const *indx,
            double *values) = 0;

    virtual int setGlobalParameterValues(int len, int const *indx,
            const double *values) = 0;


    /************************ Global Parameters Species Section *******************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Compartments Section ********************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumCompartments() = 0;
    virtual int getCompartmentIndex(const std::string& eid) = 0;
    virtual std::string getCompartmentId(int index) = 0;

    /**
     * get the compartment volumes
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getCompartmentVolumes(int len, int const *indx,
            double *values) = 0;

    virtual int setCompartmentVolumes(int len, int const *indx,
                const double *values) = 0;

    /**
     * Set the initial volumes of the compartments.
     *
     * Takes the same indices as the other compartment methods.
     *
     * Note, if a compartment has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setCompartmentInitVolumes(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial volume of the compartments,
     * uses the same indexing as the other compartment methods.
     */
    virtual int getCompartmentInitVolumes(int len, int const *indx,
                    double *values) = 0;


    /************************ End Compartments Species Section ********************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Selection Ids Species Section ***********************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * populates a given list with all the ids that this class can accept.
     *
     * @param ids: a list of strings that will be filled by this class.
     * @param types: the types of ids that are requested. Can be set to
     * 0xffffffff to request all the ids that this class supports.
     * This should by a bitwise OR of the filelds in SelectionRecord::SelectionType
     */
    virtual void getIds(int types, std::list<std::string> &ids) = 0;

    /**
     * returns a bit field of the ids that this class supports.
     */
    virtual int getSupportedIdTypes() = 0;

    /**
     * gets the value for the given id string. The string must be a SelectionRecord
     * string that is accepted by this class.
     */
    virtual double getValue(const std::string& id) = 0;

    /**
     * sets the value coresponding to the given selection stringl
     */
    virtual void setValue(const std::string& id, double value) = 0;


    /************************ End Selection Ids Species Section *******************/
    #endif /***********************************************************************/
    /******************************************************************************/

    /**
     * allocate a block of memory and copy the stochiometric values into it,
     * and return it.
     *
     * The caller is responsible for freeing the memory that is referenced by data.
     *
     * @param[out] rows will hold the number of rows in the matrix.
     * @param[out] cols will hold the number of columns in the matrix.
     * @param[out] data a pointer which will hold a newly allocated memory block.
     */
    virtual int getStoichiometryMatrix(int* rows, int* cols, double** data) = 0;

    /**
     * Get the current stiochiometry value for the given species / reaction.
     *
     * If either are not valid, NaN is returned.
     */
    virtual double getStoichiometry(int speciesIndex, int reactionIndex) = 0;


    virtual int getNumConservedMoieties() = 0;
    virtual int getConservedMoietyIndex(const std::string& eid) = 0;
    virtual std::string getConservedMoietyId(int index) = 0;
    virtual int getConservedMoietyValues(int len, int const *indx, double *values) = 0;
    virtual int setConservedMoietyValues(int len, int const *indx,
            const double *values) = 0;

    virtual int getNumRateRules() = 0;

    /**
     * @author JKM
     * @date 07/31/2015
     * @brief Gets the symbols defined by rate rules, i.e.
     * returns all x such that x' = f(x) is a rule which defines parameter x.
     */
    virtual std::vector<std::string> getRateRuleSymbols() const {
        throw NotImplementedException("getRateRuleSymbols not implemented in " + getExecutableModelDesc());
    }

    /**
     * get the number of reactions the model has
     */
    virtual int getNumReactions() = 0;

    /**
     * get the index of a named reaction
     * @returns >= 0 on success, < 0 on failure.
     */
    virtual int getReactionIndex(const std::string& eid) = 0;

    /**
     * get the name of the specified reaction
     */
    virtual std::string getReactionId(int index) = 0;

    /**
     * get the vector of reaction rates.
     *
     * @param len: the length of the suplied buffer, must be >= reaction rates size.
     * @param indx: pointer to index array. If NULL, then it is ignored and the
     * reaction rates are copied directly into the suplied buffer.
     * @param values: pointer to user suplied buffer where rates will be stored.
     */
    virtual int getReactionRates(int len, int const *indx,
                double *values) = 0;

    /**
     * get the 'values' i.e. the what the rate rule integrates to, and
     * store it in the given array.
     *
     * The length of rateRuleValues obviously must be the number of
     * rate rules we have.
     */
    virtual void getRateRuleValues(double *rateRuleValues) = 0;

    /**
     * get the id of an element of the state vector.
     */
    virtual std::string getStateVectorId(int index) = 0;

    /**
     * The state vector is a vector of elements that are defined by
     * differential equations (rate rules) or independent floating species
     * are defined by reactions.
     *
     * To get the ids of the state vector elements, use getStateVectorId.
     *
     * copies the internal model state vector into the provided
     * buffer.
     *
     * @param[out] stateVector a buffer to copy the state vector into, if NULL,
     *         return the size required.
     *
     * @return the number of items coppied into the provided buffer, if
     *         stateVector is NULL, returns the length of the state vector.
     */
    virtual int getStateVector(double *stateVector) = 0;

    /**
     * sets the internal model state to the provided packed state vector.
     *
     * @param[in] an array which holds the packed state vector, must be
     *         at least the size returned by getStateVector.
     *
     * @return the number of items copied from the state vector, negative
     *         on failure.
     */
    virtual int setStateVector(const double *stateVector) = 0;

    /**
     * the state vector y is the rate rule values and floating species
     * concentrations concatenated. y is of length numFloatingSpecies + numRateRules.
     *
     * The state vector is packed such that the first n raterule elements are the
     * values of the rate rules, and the last n floatingspecies are the floating
     * species values.
     *
     * @param[in] time current simulator time
     * @param[in] y state vector, must be either null, or have a size of that
     *         speciefied by getStateVector. If y is null, then the model is
     *         evaluated using its current state. If y is not null, then the
     *         y is considered the state vector.
     * @param[out] dydt calculated rate of change of the state vector, if null,
     *         it is ignored.
     */
    virtual void getStateVectorRate(double time, const double *y, double* dydt=0) = 0;

    virtual void testConstraints() = 0;

    virtual std::string getInfo() = 0;

    virtual void print(std::ostream &stream) = 0;

    /******************************* Events Section *******************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumEvents() = 0;

    /**
     * get the event status, false if the even is not triggered, true if it is.
     *
     * The reason this returns an unsigned char instead of a bool array is this
     * array is typically stuffed into an std::vector, and std::vector<bool> is
     * well, weird as it's actually implemented as a bitfield, and can not be
     * used as a C array.
     *
     * So, on every modern system I'm aware of, bool is an unsigned char, so
     * use that data type here.
     */
    virtual int getEventTriggers(int len, const int *indx, unsigned char *values) = 0;


    /**
     * Itterate through all of the current and pending events and apply them. If any
     * events trigger a state change which triggers any additional events, these
     * are applied as well. After this method finishes, all events are processed.
     *
     * @param timeEnd: model time when the event occured.
     * @param previousEventStatus: array of previous event triggered states.
     * @param initialState (optional): initial state vector, may be NULL, in which
     * the current state is used.
     * @param finalState (optional): final state vector, where the final state is
     * coppied to. May be NULL, in which case, ignored.
     */
    virtual int applyEvents(double timeEnd, const unsigned char* previousEventStatus,
                const double *initialState, double* finalState) = 0;


    /**
     * evaluate the event 'roots' -- when events transition form triggered - non-triggered
     * or triggered to non-triggered state.
     *
     * Simplest method is to return 1 for triggered, -1 for not-triggered, so long
     * as there is a zero crossing.
     *
     * @param time[in] current time
     * @param y[in] the state vector
     * @param gdot[out] result event roots, this is of length numEvents.
     */
    virtual void getEventRoots(double time, const double* y, double* gdot) = 0;

    virtual double getNextPendingEventTime(bool pop) = 0;

    virtual int getPendingEventSize() = 0;

    virtual void resetEvents() = 0;

    /**
     * need a virtual destructor as object implementing this interface
     * can be deleted directly, i.e.
     * ExecutableModel *p = createModel(...);
     * delete p;
     */
    virtual ~ExecutableModel() {};

    /******************************* Events Section *******************************/
     #endif /**********************************************************************/
    /******************************************************************************/


    /**
     * Gets the index for an event id.
     * If there is no event with this id, returns -1.
     */
    virtual int getEventIndex(const std::string& eid) = 0;
    virtual std::string getEventId(int index) = 0;
    virtual void setEventListener(int index, EventListenerPtr eventHandler) = 0;
    virtual EventListenerPtr getEventListener(int index) = 0;

    /**
     * Get the amount rate of change for the i'th floating species
     * given a reaction rates vector.
     *
     * TODO: This should be merged with getFloatingSpeciesAmountRates, but that will
     * break inteface, will do in next point release.
     *
     * TODO: If the conversion factor changes in between getting the
     * reaction rates vector via getReactionRates
     *
     * @param index: index of the desired floating speceis rate.
     * @param reactionRates: pointer to buffer of reaction rates.
     */
    virtual double getFloatingSpeciesAmountRate(int index,
            const double *reactionRates) = 0;

    /**
     * reset the model accordign to a bitfield specified by the
     * SelectionRecord::SelectionType values.
     */
    virtual void reset(int options) = 0;

    /**
     * set the seed used by the random number generator. This will by definition
     * reset the RNG.
     */
    virtual void setRandomSeed(int64_t) = 0;

    /**
     * get the seed used by the RNG.
     */
    virtual int64_t getRandomSeed() = 0;

    /**
     * Get a uniform random number between 0 and 1 created by the RNG.
     *
     * The type of RNG can be specified in the config file with the
     * RANDOM_GENERATOR_TYPE key.
     */
    virtual double getRandom() = 0;

    /**
     * Get the current set of flags
     */
    virtual uint32_t getFlags() const = 0;

    /**
     * Set certain options that determine the state of the ExecutableModel,
     * these are listed in
     */
    virtual void setFlags(uint32_t) = 0;

    enum ExecutableModelFlags {
        /**
         * A simulation is currently running. This means that the model
         * should not have to re-calculate the reaction rate vector
         * as it was calculated in the previous integration step.
         */
        INTEGRATION                       = (0x1 << 0),  // => 0x00000001

        /**
         * optimize not-recalculating the reaction rates during selection.
         */
        OPTIMIZE_REACTION_RATE_SELECTION  = (0x1 << 1),  // => 0x00000010
    };

    /**
     * for source compatability
     */
    void computeAllRatesOfChange() {};

    friend class RoadRunner;

protected:

    /**
     * is integration is currently proceeding.
     */
    void setIntegration(bool value) {
        uint32_t flags = getFlags();
        if(value) {
            flags |= INTEGRATION;
        } else {
            flags &= ~INTEGRATION;
        }
        setFlags(flags);
    }


};





/**
 * dump the model to a stream convenience func
 */
RR_DECLSPEC std::ostream& operator << (std::ostream &stream, ExecutableModel* model);


}
#endif
