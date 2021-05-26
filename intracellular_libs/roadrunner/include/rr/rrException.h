// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Integrator.h
* @author ETS, JKM
* @date 2014
* @copyright Apache License, Version 2.0
* @brief RoadRunner exception classes
**/

#ifndef rrExceptionH
#define rrExceptionH
#include <exception>
#include <string>
#include "rrExporter.h"

using std::string;
using std::exception;

namespace rr
{

class RR_DECLSPEC Exception : public std::exception
{
    protected:
        string mMessage;   //Exception message

    public:
        //string& Message;
                                Exception(const string& desc);
        virtual                ~Exception() throw();
        virtual const char*     what() const throw();
        string                  Message() const;
        string                  getMessage() const;
};

class RR_DECLSPEC CoreException : public Exception
{
    public:
        CoreException(const string& msg);
        CoreException(const string& msg1, const string& msg2);
};

class RR_DECLSPEC ScannerException : public Exception
{
    public:
        ScannerException(const string& msg);
};

class RR_DECLSPEC NLEQException : public Exception
{
    public:
        NLEQException(const string& msg);
};

class RR_DECLSPEC NOMException : public Exception
{
    public:
        NOMException(const string& msg);
};

class RR_DECLSPEC CVODEException : public Exception
{
    public:
        CVODEException(const string& msg);
};

class RR_DECLSPEC NotImplementedException : public Exception
{
    public:
        NotImplementedException(const string& msg);
};

class RR_DECLSPEC InvalidKeyException : public Exception
{
    public:
        InvalidKeyException(const string& msg);
};

/**
 * @author JKM
 * @brief Thrown whenever an uninitialized value is encountered,
 * see https://github.com/sys-bio/roadrunner/issues/180
 */
class RR_DECLSPEC UninitializedValueException : public Exception
{
    public:
        UninitializedValueException(const string& msg);
};

/**
 * @author JKM
 * @brief Called whenever an uninitialized value is encountered,
 * throws @ref UninitializedValueException
 */
void UninitializedValue(const string& msg);

}//namepsace rr
#endif
