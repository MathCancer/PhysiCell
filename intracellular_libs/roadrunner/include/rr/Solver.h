// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Solver.h
* @author JKM
* @date 09/01/2015
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner solvers
**/

# ifndef RR_SOLVER_H_
# define RR_SOLVER_H_

// == INCLUDES ================================================

# include "rrLogger.h"
# include "rrOSSpecifics.h"
# include "Dictionary.h"
# include "rrException.h"

# include "tr1proxy/rr_memory.h"
# include "tr1proxy/rr_unordered_map.h"
# include <stdexcept>

// == CODE ====================================================

namespace rr
{
    /**
     * @author JKM
     * @brief Base class for all integrators and steady state solvers
     */
    class RR_DECLSPEC Solver
    {
    public:

        virtual ~Solver() {};

        /**
        * @author JKM
        * @brief Get the name of this solver
        */
        virtual std::string getName() const = 0;

        /**
        * @author JKM
        * @brief Get the description of this solver
        */
        virtual std::string getDescription() const = 0;

        /**
        * @author JKM
        * @brief Get a (user-readable) hint for this solver
        */
        virtual std::string getHint() const = 0;

        /**
        * @author JKM
        * @brief Get a list of all settings for this solver
        */
        std::vector<std::string> getSettings() const;

        /**
        * @author JKM
        * @brief Reset all settings to their respective default values
        */
        virtual void resetSettings();

        /**
        * @author JKM, WBC
        * @brief Get the value of an integrator setting
        * @note Use one of the type-concrete versions like @ref getValueAsInt
        * to avoid type conversion gotchas
        */
        virtual Variant getValue(std::string key) const;

        /**
        * @author JKM
        * @brief Return true if this setting is supported by the integrator
        */
        virtual Variant hasValue(std::string key) const;

        /**
        * @author JKM
        * @brief Get the number of parameters
        */
        virtual unsigned long getNumParams() const;

        /**
        * @author JKM
        * @brief Get the name of the parameter at index n
        */
        virtual std::string getParamName(int n) const;

        /**
        * @author JKM
        * @brief Get the display name of the parameter at index n
        */
        virtual std::string getParamDisplayName(int n) const;

        /**
        * @author JKM
        * @brief Get the hint of the parameter at index n
        */
        virtual std::string getParamHint(int n) const;

        /**
        * @author JKM
        * @brief Get the description of the parameter at index n
        */
        virtual std::string getParamDesc(int n) const;


        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual int getValueAsInt(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned int getValueAsUInt(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual long getValueAsLong(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned long getValueAsULong(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual float getValueAsFloat(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual double getValueAsDouble(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual char getValueAsChar(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned char getValueAsUChar(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual std::string getValueAsString(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual bool getValueAsBool(std::string key);


        virtual void setValue(std::string key, const Variant& value);

        /**
        * @author WBC
        * @brief Gets the hint associated with a given key
        */
        const std::string& getDisplayName(std::string key) const;

        /**
        * @author WBC
        * @brief Gets the hint associated with a given key
        */
        const std::string& getHint(std::string key) const;

        /**
        * @author WBC
        * @brief Gets the description associated with a given key
        */
        const std::string& getDescription(std::string key) const;

        /**
        * @author WBC
        * @brief Gets the type associated with a given key
        */
        const Variant::TypeId getType(std::string key);

        /**
        * @author JKM
        * @brief Get the solver settings as a string
        */
        std::string getSettingsRepr() const;

        /**
        * @author JKM
        * @brief Python dictionary-style string representation of settings
        */
        std::string settingsPyDictRepr() const;

        /**
        * @author JKM
        * @brief Return a string representation of the solver
        */
        virtual std::string toString() const;

        /**
        * @author JKM
        * @brief Return string representation a la Python __repr__ method
        */
        virtual std::string toRepr() const;

    protected:
        typedef std::vector<std::string> SettingsList;
        typedef RR_UNORDERED_MAP <std::string, Variant> SettingsMap;
        typedef RR_UNORDERED_MAP <std::string, std::string> DisplayNameMap;
        typedef RR_UNORDERED_MAP <std::string, std::string> HintMap;
        typedef RR_UNORDERED_MAP <std::string, std::string> DescriptionMap;

        SettingsList sorted_settings;
        SettingsMap settings;
        DisplayNameMap display_names_;
        HintMap hints;
        DescriptionMap descriptions;

        void addSetting(std::string name, Variant val, string display_name, std::string hint, std::string description);
    };

}

# endif /* RR_INTEGRATOR_H_ */
