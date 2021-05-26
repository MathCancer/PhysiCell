#ifndef rrFileNameH
#define rrFileNameH
#include <string>
#include <ostream>
#include "rrExporter.h"
using std::string;
using std::ostream;

namespace rr
{

/**
 * @internal
 * @deprecated
 */
class RR_DECLSPEC FileName
{
    private:
        mutable string              mPathAndName;
        string                      mPath;
        string                      mName;
        void                        MakeFileString();

    public:
                                    FileName(const string& name = "", const string& path = "");
                                    FileName(const FileName& fN);
                                   ~FileName();

        FileName&                   operator = (const FileName& fN);
        FileName&                   operator = (const string& fN);
        FileName&                   operator = (const char* fN);
        bool                        operator !=(const char* fN);
                                    operator string();// {return mPathAndName;}
        bool                        SetPath(const string& path);
        void                        SetFileName(const string& name);
        void                        SetNameAndPath(const string& path, const string& name);
        void                        SetFileNameAndPath(const string& name);

        string                      GetFileName() const;
        string                      GetPath() const;
        string                      GetPathAndFileName() const;
        string                      Get();
        string                      GetFileNameNoExtension();
        unsigned int                size();
};

RR_DECLSPEC std::ostream& operator         <<(std::ostream &os, FileName &obj);

}
#endif
