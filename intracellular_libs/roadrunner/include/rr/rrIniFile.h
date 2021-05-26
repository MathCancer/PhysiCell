#ifndef IniFileH
#define IniFileH
#include <vector>
#include <fstream>
#include <string>
#include "rrExporter.h"
#include "rrStringUtils.h"
#include "rrIniSection.h"
#include "rrIniKey.h"
#include "rrFileName.h"

namespace rr
{

using std::string;
using std::fstream;
using std::ios;
using std::ios_base;

const int MAX_LINE_BUFFER_SIZE = 2048;
class IniKey;
class IniSection;

// IniFile typedefs
typedef std::vector<IniKey*>             KeyList;
typedef std::vector<string>             NonKeyList;
typedef std::vector<IniSection*>          SectionList;
typedef SectionList::iterator           SectionItor;

/**
 * @internal
 */
class RR_DECLSPEC IniFile
{
    private:
        // When set, this define will cause SetValue() to create a new section, if
        // the requested section does not allready exist.
        const int            mAutoCreateSections;

        // When set, this define causes SetValue() to create a new key, if the
        // requested key does not allready exist.
        const int             mAutoCreateKeys;

        string                 mCommentIndicators;
        const string         mEqualIndicator;
        const string         mWhiteSpace;

        // General Purpose Utility Functions
        string                GetNextWord(string& CommandLine);
        string                Trim(string& Str);
        int                    WriteLine(fstream& stream, const char* fmt, ...);

    protected:
        SectionList            mSections;        // List of sections
        FileName            mIniFileName;    // The filename to write to
        bool                mIsDirty;        // Tracks whether or not data has changed.
        bool                mWasFound;
        bool                 mAutoSave;        //Save ini file automatically on destruction


    public:
        int                    mFlags;        // Our settings flags.
                            IniFile(const string& fName = "", bool autoLoad = false, bool autoSave = false);
        virtual                ~IniFile();

        int                    GetNumberOfSections(){return mSections.size();}
        IniSection*        GetSection(int i){return mSections[i];}

        // File handling methods
        string              GetFilePath(){return mIniFileName.GetPath();}
        string              GetFileName(){return mIniFileName;}
        string              GetFullFileName(){return mIniFileName.GetPathAndFileName();}
        bool                SetFilePath(const string& path);
        bool                Load(const string& fName = "");
        IniSection*          LoadSection(const string& theSection);
        bool                Save(ios_base::openmode openMode = ios::out|ios::trunc);
        bool                UnLoad(){return Save();}
        bool                WasItFound(){return mWasFound;} //!Boolean indicating if the last key was found in the ini file
        bool                SectionExists(const string& section);

        //Reading
        // ReadValue: Default access method. Returns the raw string value
        string                ReadValue(const string& Key, const string& Section = "");
         string                ReadString(const string& Key, const string& Section = "", const string& def_val="");
        double                ReadDouble(const string& Key, const string& Section = "", double def_value = 0);
        float                ReadFloat(const string& Key, const string& Section = "", double def_value = 0){return (float) ReadDouble(Key, Section, def_value);}
        int                    ReadInteger(const string& Key, const string& Section = "", int def_value = 0);
        bool                ReadBool(const string& Key, const string& Section = "", bool def_value = false);

           //Writing
         bool                WriteValue(const string& Key, const string& Value, const string& Comment = "", const string& Section = "");
        bool                WriteFloat(const string& Key, double value, const string& Comment = "", const string& Section = "");
        bool                WriteInteger(const string& Key, int nValue, const string& Comment = "", const string& Section = "");
        bool                WriteBool(const string& Key, bool bValue, const string& Comment = "", const string& Section = "");
        bool                WriteString(const string& Key, const string& Value, const string& Comment = "", const string& Section = ""){return WriteValue(Key, Value, Comment, Section);}
         bool                WriteNonKey(const string& nonKey, const string& Section = "");

        //KeyHandling
        bool                SetKeyComment(const string& Key, const string& Comment, const string& Section = "");
        bool                SetSectionComment(const string& Section, const string& Comment);
        bool                DeleteKey(const string& Key, const string& FromSection = "");
        bool                DeleteSection(const string& Section);
        bool                DeleteSectionsWithKeyValue(const string& key, const string& value);
        bool                CreateKey(const string& Key, const string& Value, const string& Comment = "", const string& Section = "");

        //Section stuff
        bool                CreateSection(const string& Section, const string& Comment = "");
        bool                CreateSection(const string& Section, const string& Comment, KeyList Keys);

        // Utility Methods
        int                    SectionCount();
        int                    KeyCount();
        int                 KeyCount(const string& section);
        void                Clear();
        void                SetFileName(const string& fName);
        string                CommentStr(string& Comment);
        IniKey*                GetKey(const string& Key, const string& Section);
        IniSection*            GetSection(const string& Section, bool create = false);
        IniSection*            GetSection(const unsigned int secNr);
        bool                 ClearSection(const string& section);
        bool                 IsModified(){return mIsDirty;}
};

}
#endif
