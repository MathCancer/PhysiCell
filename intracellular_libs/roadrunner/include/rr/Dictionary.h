/*
 * Dictionary.h
 *
 *  Created on: Jul 14, 2014
 *      Author: andy
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include "Variant.h"
#include <string>
#include <vector>

#include "tr1proxy/rr_memory.h"
#include "tr1proxy/rr_unordered_map.h"

#if defined(SWIG) || defined(SWIGPYTHON)
#include "PyUtils.h"
#endif


namespace rr {

/**
 * @brief This class is frozen, no new features
 * A dictionary interface that objects can implement. These are automatically
 * mapped to python dictionaries.
 */
class RR_DECLSPEC Dictionary  // Yes, I know this is a pure virtual *INTERFACE*
{                             // so it should NOT need to be exported, but MSVC
public:                       // *mistakenly* complains in ONE instance.
                              // add the declspec just to shut it up.

#ifndef SWIG

    /**
     * Set the value for an arbitrary key.
     *
     * The Variant object has a large number of creation operators so
     * you only have to give the value as native C++ object i.e.
     *
     * @code
     * Dictionary*d = (...);
     * d->setItem("someIntKey", 1);
     * d->setItem("someStrKey", string("some string"));
     * d->setItem("someFloatVal, 0.123);
     * @endcode
     */
    virtual void setItem(const std::string& key, const rr::Variant& value) = 0;

    /**
     * Get a value. Variants are POD. If the key does not exist, an std::exception
     * will be thrown.
     *
     * The Variant object has the assigment operator overloaded so
     * it will automatically convert to any native C++ object. If the
     * Variant can not be converted, it will raise an exception, i.e.
     *
     * @code
     * Dictionary* d = ...;
     * int a = d->getItem("someIntKey");
     * string s = d->getItem("someStrVal");
     * double val = d->getItem("someDoubleVal");
     * @endcode
     */
    virtual Variant getItem(const std::string& key) const = 0;

    /**
     * is there a key matching this name.
     */
    virtual bool hasKey(const std::string& key) const = 0;

    /**
     * remove a value
     */
    virtual int deleteItem(const std::string& key) = 0;

    /**
     * list of keys in this object.
     */
    virtual std::vector<std::string> getKeys() const = 0;

    /**
     * Pure virtual interface, you should never have to delete
     * an instance of this type directly.
     */
    virtual ~Dictionary() {};

#endif

    /**
     * implement the python method if we are being used in the python binding.
     *
     * Note, if these methods are added to an %extend section, derived classes
     * will not see them, so as far as I can tell, this is the only way to
     * add these methods.
     */
#if defined(SWIGPYTHON)
    PyObject *keys() {
        return dictionary_keys(this);
    }

    PyObject *values() {
        return dictionary_values(this);
    }

    PyObject *items() {
        return dictionary_items(this);
    }

    PyObject *__getitem__(const char* key) {
        return dictionary_getitem(this, key);
    }

    PyObject *__setitem__(const char* key, PyObject* value) {
        return dictionary_setitem(this, key, value);
    }

    void __delitem__(const char* key) {
        return dictionary_delitem(this, key);
    }

    PyObject *__contains__(const char* key) {
        return dictionary_contains(this, key);
    }

    std::string helloPython() {
        return "hello python";
    }
#endif
};


/**
 * @brief This class is frozen, no new features
 * Basic implementation of the Dictionary interface which uses a
 * std unordered map to store the values.
 */
class RR_DECLSPEC BasicDictionary : public Dictionary
{
public:
    BasicDictionary() {};

    /**
     * sets an item in the internal unordered map.
     */
    virtual void setItem(const std::string& key, const rr::Variant& value);

    /**
     * gets an item from the internal unordered map.
     */
    virtual Variant getItem(const std::string& key) const;

    /**
     * is there a key matching this name.
     *
     * @retruns true if this key exists, false otherwise.
     */
    virtual bool hasKey(const std::string& key) const;

    /**
     * remove a value
     */
    virtual int deleteItem(const std::string& key);

    /**
     * list of keys in this object.
     */
    virtual std::vector<std::string> getKeys() const;

    /**
     * Pure virtual interface, you should never have to delete
     * an instance of this type directly.
     */
    virtual ~BasicDictionary() {};

protected:
    typedef cxx11_ns::unordered_map<std::string, Variant> VariantMap;
    VariantMap items;
};

}





#endif /* DICTIONARY_H_ */
