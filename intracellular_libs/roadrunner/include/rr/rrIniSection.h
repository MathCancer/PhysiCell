#ifndef rrIniSectionH
#define rrIniSectionH
#include <vector>
#include <string>
#include "rrExporter.h"
#include "rrIniKey.h"

namespace rr
{

class IniKey;
typedef std::vector<IniKey*>             KeyList;
typedef KeyList::iterator                 KeyItor;
typedef std::vector<string>             NonKeyList;
typedef NonKeyList::iterator             NonKeyItor;


/**
 * @internal
 * This class stores the definition of a section. A section contains any number
 * of keys (see IniKeys), and may or may not have a comment.
 */
class RR_DECLSPEC IniSection
{
    private:
        bool            mIsDirty;

    public:
        bool              mIsUsed;
        string            mName;
        string            mComment;
        KeyList           mKeys;            //vector of pointers to keys
        NonKeyList        mNonKeys;         //vector of pointers to non_keys

                        IniSection();
                        IniSection(const std::string& nameValueString, const char& sep);
                       ~IniSection();
        IniKey*          CreateKey(const string& _keyName, const string& Value = "", const string& Comment = "");
        IniKey*            GetKey(const int& i);
        IniKey*            GetKey(const string& keyName, bool create = false);
        size_t             KeyCount(){return mKeys.size();}
        size_t             NonKeyCount(){return mNonKeys.size();}
        void            Clear(){mKeys.clear(); mNonKeys.clear();}
           string            GetNonKeysAsString();
        string            AsString();
};
}
#endif
