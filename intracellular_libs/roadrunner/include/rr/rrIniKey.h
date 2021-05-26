#ifndef rrIniKeyH
#define rrIniKeyH
#include <vector>
#include <fstream>
#include <string>
#include <complex>
#include "rrExporter.h"
#include "rrStringUtils.h"
#include "rrIniSection.h"

namespace rr
{

/**
 * @internal
 * This class stores the definition of a key. A key is a named identifier
 * that is associated with a value. It may or may not have a comment.  All comments
 * must PRECEDE the key on the line in the config file.
 */
class RR_DECLSPEC IniKey
{
protected:
    void SetupKey(const std::string& key);

public:
    std::string                      mKey;
    std::string                      mValue;
    std::string                      mComment;

    IniKey(const std::string& key = "");
    ~IniKey(){}
    void                       ReKey(const std::string& key);
    std::string                       AsString() const;
    int                       AsBool() const;
    int                       AsInt() const;
    double                       AsFloat() const;
    std::complex<double>         AsComplex() const;
    RR_DECLSPEC
    friend std::ostream&         operator<<(std::ostream& stream, const IniKey& aKey);
};
}

#endif
