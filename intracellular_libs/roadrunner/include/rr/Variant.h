/*
 * Variant.h
 *
 *  Created on: Apr 26, 2014
 *      Author: andy
 */

#ifndef VARIANT_H_
#define VARIANT_H_

#include "rrExporter.h"
#include <typeinfo>
#include <string>


namespace rr
{

/**
 * A basic type to hold a variety of data types.
 *
 * Unfourtunatly C++ does not have a standard variant type so we have to
 * create one here.
 *
 * This is a fairly low performance class and is intended soley for
 * infrequent operations such as setting configuration parameters.
 *
 * If Python support is enabled, this class can convert to and from a
 * Python object.
 *
 * Usage:
 * This class can convert to and from any primitive data type, and
 * some collections types. More type conversions will be added as needed.
 *
 * To store a value, just assign it, the assignment operator automatically
 * takes care of everything for you:
 * @code
 * // store an int:
 * int i = 23;
 * Variant v = i;
 *
 * // store a double:
 * Variant v = (double)0.123;
 * @endcode
 *
 * Extraction:
 * To retrieve the stored data, uses the convert function, this is templated
 * so it can convert and extract to any type:
 *
 * @code
 * Variant v = getSomeVariant();
 *
 * // to convert to integer:
 * int i = v.convert<int>();
 *
 * // to convert to string:
 * std::string s = v.convert<std::string>();
 * @endcode
 *
 * Rationale:
 * C++ does not have a built in variant type. Other variant types exist
 * such as boost::any and Poco dynamic var. However including the one of these
 * as part of our public API would forever tie us to that particular library, and
 * would impose yet another dependency.
 *
 * Only very basic variant type support is needed and this class exposes the
 * absolute minimum possible API to support such features, and provides
 * just two methods of getting and storing native C++ types in it.
 *
 * This is *currently* implemented by Poco but the implementation is fully
 * opaque and may change in the future.
 */
class RR_DECLSPEC Variant
{
public:

    /**
     * list of currently supported types that a Variant can hold.
     */
    enum TypeId
    {
        STRING, BOOL, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE, CHAR, UCHAR, EMPTY
    };

    /**
     * the current type that this Variant is.
     */
    TypeId type() const;

    /**
     * creates an emtpy variant
     */
    Variant();

    /**
     * create a new variant from an existing supported data type.
     * This templated constructor can assign any primitive type:
     * @code
     * Variant v = (int)1;
     * Variant v = std::string("a string");
     * @endcode
     */
    template <typename T>
    Variant(const T& val) : self(0)
    {
        const std::type_info &info = typeid(T);
        alloc();
        assign(info, &val);
    }

    Variant(const char* str) : self(0)
    {
        const std::type_info &info = typeid(std::string);
        std::string val(str);
        alloc();
        assign(info, &val);
    }

    /**
     * Copy constructor.
     */
    Variant(const Variant& other);

    /**
     * Assignment operator for assigning POD to Var
     * same as the constructor, this assigns a value to an existing
     * Variant.
     */
    template <typename T>
    Variant& operator = (const T& value)
    {
        const std::type_info &info = typeid(T);
        assign(info, &value);
        return *this;
    }

    /**
     * Assignment operator. Assign one variant to another.
     */
    Variant& operator = (const Variant& other);

    /**
     * clean up any data owned by this object.
     */
    virtual ~Variant();

    /**
     * get the type id of the stored data type. This will let you check what kind
     * of data is strored in this variant.
     *
     * @code
     * // to check if this is an integer:
     * Variant v = ...
     * if (v.type() == typeid(int))
     *     cout << "its an int";
     *
     * // to check if its a string:
     * if (v.type() == typeid(std::string))
     *     cout << "ints a string";
     * @endcode
     */
    const std::type_info& typeInfo() const;

    /**
     * Convert this variant to a supported data type.
     *
     * This method will try to perform type coercion, i.e. if this variant contains
     * a string, and it is asked to convert to a int, the string will be parsed as
     * an int. Similary, doubles will be rounded to int, so forth.
     *
     * @code
     * // convert to int:
     * int i = v.convert<int>();
     * @endcode
     */
    template <typename T>
    T convert() const
    {
        T value;
        convert_to(typeid(T), &value);
        return value;
    }

    /**
     * conversion operators, note, MSVC 2012 is buggy and does not support
     * templated conversion operators correctly, so manually implement
     * the supported types here.
     */
    #define VARIANT_IMPLICIT_CONVERT(type)   \
    operator type() const                    \
    {                                        \
        return convert<type>();              \
    }

    VARIANT_IMPLICIT_CONVERT(std::string);

    VARIANT_IMPLICIT_CONVERT(long);

    VARIANT_IMPLICIT_CONVERT(bool);

    VARIANT_IMPLICIT_CONVERT(float);

    VARIANT_IMPLICIT_CONVERT(double);

    VARIANT_IMPLICIT_CONVERT(unsigned long);

    VARIANT_IMPLICIT_CONVERT(int);

    VARIANT_IMPLICIT_CONVERT(unsigned int);

    VARIANT_IMPLICIT_CONVERT(char);

    VARIANT_IMPLICIT_CONVERT(unsigned char);

    /**
     * Parses the string which must be in JSON format. This is a common
     * way to read a Variant from a file or create a new one from a string:
     * @code
     * Variant v = Variant::parse("0.123");
     * @endcode
     */
    static Variant parse(const std::string& val);

    /**
     * Converts the Variant to a string in JSON format.
     */
    std::string toString() const;

    /**
     * @author JKM
     * @brief Convert to Python-compatible representation
     */
    std::string pythonRepr() const;

    /**
     * is this variant a string.
     */
    bool isString() const;

    /**
     * was an interger stored here.
     */
    bool isInteger() const;

    /**
     * is this a numeric type.
     */
    bool isNumeric() const;

    /**
     * is this a boolean type.
     */
    bool isBool() const;

    /**
     * true if empty.
     */
    bool isEmpty() const;

    /**
     * true if this is a signed number.
     */
    bool isSigned() const;


private:
    /**
     * private implementation, this gives us the flexibility to use
     * different implementations in the future and maintain binary
     * compatability.
     */
    struct VariantImpl *self;
    void alloc();
    void assign(const std::type_info& info, const void* value);
    void convert_to(const std::type_info& info, void* value) const;
};

} /* namespace rr */

#endif /* VARIANT_H_ */
