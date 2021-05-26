/*
 * RK4Integrator.h
 *
 *  Created on: Jul 22, 2014
 *      Author: andy
 */

#ifndef RK4Integrator_H_
#define RK4Integrator_H_

#include <Integrator.h>
#include <rrRoadRunnerOptions.h>

namespace rr
{


    /**
    * A super basic 4'th order fixed step integrator.
    *
    * The RungeKuttaIntegrator will be the more sophisticated,
    * general purpose Runge-Kutta integrator which will support
    * different orders and adaptive time stepping.
    *
    * This object is mainly here as an example of creating a new Integrator.
    *
    */
    class RK4Integrator: public Integrator
    {
    public:


        /**
        * Creates a new RK4Integrator.
        *
        * The IntegratorFactory is the ONLY object that creates integrators.
        *
        * Integrators are created when the IntegratorFactory::New method is called,
        * typically by the top level RoadRunner object.
        *
        * The integrator will hold onto the ExecutableModel pointer, m, and when the
        * integrate method is called, will advance the model object forward in time.
        *
        * @param m: a borrowed reference to an existing ExecutableModel object.
        * @param o: a reference to a SimulatOptions object where the configuration
        * parameters will be read from.
        */
        RK4Integrator(ExecutableModel *m);

        /**
        * clean up any mess.
        */
        virtual ~RK4Integrator();

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m);


        /**
        * implement Integrator interface
        */
    public:

        /**
        * integrates the model from t0 to tf.
        */
        virtual double integrate(double t0, double tf);

        /**
        * copies the state vector out of the model and into cvode vector,
        * re-initializes cvode.
        */
        virtual void restart(double t0);

        /**
        * get a description of this object, compatable with python __str__
        */
        virtual std::string toString() const;

        /**
        * get a short descriptions of this object, compatable with python __repr__.
        */
        virtual std::string toRepr() const;

        // ** Meta Info ********************************************************

        /**
         * @author JKM
         * @brief Get the name for this integrator
         * @note Delegates to @ref getName
         */
        std::string getName() const;

        /**
         * @author JKM
         * @brief Get the name for this integrator
         */
        static std::string getRK4Name();

        /**
         * @author JKM
         * @brief Get the description for this integrator
         * @note Delegates to @ref getDescription
         */
        std::string getDescription() const;

        /**
         * @author JKM
         * @brief Get the description for this integrator
         */
        static std::string getRK4Description();

        /**
         * @author WBC
         * @brief Get the hint for this integrator
         * @note Delegates to @ref getHint
         */
        std::string getHint() const;

        /**
         * @author JKM
         * @brief Get the hint for this integrator
         */
        static std::string getRK4Hint();

        // ** Getters / Setters ************************************************

        virtual Variant getValue(std::string key);

        /**
         * @author JKM
         * @brief Always deterministic for RK4
         */
        IntegrationMethod getIntegrationMethod() const;

        /**
        * @author JKM
        * @brief Reset all integrator settings to their respective default values
        */
        void resetSettings();

        // ** Listeners ********************************************************

        /**
        * the integrator can hold a single listener. If clients require multicast,
        * they can create a multi-cast listener.
        */
        virtual void setListener(IntegratorListenerPtr);

        /**
        * get the integrator listener
        */
        virtual IntegratorListenerPtr getListener();

    public:

        /**
        * set an arbitrary key
        */
//         virtual void setItem(const std::string& key, const rr::Variant& value);

        /**
        * get a value. Variants are POD.
        */
//         virtual Variant getItem(const std::string& key) const;

        /**
        * is there a key matching this name.
        */
//         virtual bool hasKey(const std::string& key) const;

        /**
        * remove a value
        */
//         virtual int deleteItem(const std::string& key);

        /**
        * list of keys in this object.
        */
//         virtual std::vector<std::string> getKeys() const;


    private:
        ExecutableModel *model;

        unsigned stateVectorSize;

        /**
        * arrays to store function eval values.
        */
        double *k1, *k2, *k3, *k4, *y, *ytmp;

        void testRootsAtInitialTime();
        void applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus);

    };


    // ** Registration *********************************************************


    class RK4IntegratorRegistrar : public IntegratorRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return RK4Integrator::getRK4Name();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return RK4Integrator::getRK4Description();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return RK4Integrator::getRK4Hint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual Integrator* construct(ExecutableModel *model) const {
                return new RK4Integrator(model);
            }
    };

} /* namespace rr */

#endif /* RK4Integrator_H_ */
