#ifndef rrUtilsH
#define rrUtilsH

#if defined(_WIN32) || defined(__WIN32__)
#define NOMINMAX
#include <windows.h>
#endif

#include <float.h>    //ms compatible IEEE functions, e.g. _isnan
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "rrExporter.h"
#include "rrConstants.h"
#include "rrStringUtils.h"
#include <stdint.h>


namespace rr
{

using std::vector;
using std::string;
using std::set;

RR_DECLSPEC bool            cleanFolder(const string& folder, const string& baseName,  const std::vector<std::string>& extensions);
RR_DECLSPEC string          getTime();
RR_DECLSPEC string          getDateTime();
RR_DECLSPEC string          getMD5(const string& text);
RR_DECLSPEC void            sleep(int ms);

//Misc.
/*!
\brief indexOf returns the index of an element in the vector. If not found, or if vector size is zero, it returns -1.
*/
RR_DECLSPEC int             indexOf(const std::vector<std::string>& vec, const std::string& elem );
RR_DECLSPEC bool            isNaN(const double& aNum);
RR_DECLSPEC bool            isNullOrEmpty(const string& str);    //Can't be null, but empty
RR_DECLSPEC void            pause(bool doIt = true, const string& msg = "");

//String utilities
RR_DECLSPEC string          removeTrailingSeparator(const string& fldr, const char sep = gPathSeparator);//"\\");

//File  Utilities
RR_DECLSPEC int             populateFileSet(const string& modelsFolder, set<string>& models);


/**
 * check file access, just calls access, but on windows, access is _access,
 *
 * file modes:
 * 00 Existence only
 * 02 Write permission
 * 04 Read permission
 * 06 Read and write permission
*/
RR_DECLSPEC bool fileExists(const string& fileN, int fileMode=0);

RR_DECLSPEC bool            folderExists(const string& folderN);
RR_DECLSPEC bool            createFolder(const string& path);

RR_DECLSPEC string          getParentFolder(const string& path);
RR_DECLSPEC string          getCurrentExeFolder();

/**
 * If roadrunner is build and running as shared library,
 * either using the C++ shared library, or using the
 * _roadrunner.so (or .pyd on Win).
 */
RR_DECLSPEC std::string getCurrentSharedLibDir();

/**
 * Get a temporary directory path.
 *
 * If the config specifies a TEMP_DIR entry, this is used, if not
 * this will return the system specified temporary directory.
 */
RR_DECLSPEC std::string getTempDir();

/**
 * Join two file paths together.
 */
RR_DECLSPEC std::string joinPath(const string& p1, const string& p2,
        const char pathSeparator = gPathSeparator);

/**
 * join three file paths together.
 */
RR_DECLSPEC std::string joinPath(const string& p1, const string& p2,
        const string& p3, const char pathSeparator = gPathSeparator);

RR_DECLSPEC std::string joinPath(const string& p1, const string& p2,
        const string& p3, const string& p4, const char pathSeparator =
                gPathSeparator);

RR_DECLSPEC std::string joinPath(const string& p1, const string& p2,
        const string& p3, const string& p4, const string& p5,
        const char pathSeparator = gPathSeparator);


RR_DECLSPEC string          getCWD();
RR_DECLSPEC const char      getPathSeparator();

RR_DECLSPEC vector<string>  getLinesInFile(const string& fName);
RR_DECLSPEC string          getFileContent(const string& fName);
RR_DECLSPEC void            createTestSuiteFileNameParts(int caseNr, const string& postFixPart, string& FilePath, string& modelFileName, string& settingsFileName);
RR_DECLSPEC string          getTestSuiteSubFolderName(int caseNr);

//CArray utilities
RR_DECLSPEC bool            copyCArrayToStdVector(const int* src,     vector<int>& dest, int size);
RR_DECLSPEC bool            copyCArrayToStdVector(const double* src,  vector<double>& dest, int size);
RR_DECLSPEC bool            copyValues(vector<double>& dest, double* source, const int& nrVals, const int& startIndex);
RR_DECLSPEC bool            copyCArrayToStdVector(const bool* src,    vector<bool>& dest, int size);
RR_DECLSPEC bool            copyStdVectorToCArray(const vector<double>& src, double* dest,  int size);
RR_DECLSPEC bool            copyStdVectorToCArray(const vector<bool>&   src,  bool*  dest,  int size);
RR_DECLSPEC double*         createVector(const vector<double>& vec);
RR_DECLSPEC vector<double>  createVector(const double* src, const int& size);

#if defined(_WIN32) || defined(__WIN32__)
RR_DECLSPEC HINSTANCE       loadDLL(const string& dll);
RR_DECLSPEC bool            unLoadDLL(HINSTANCE dllHandle);
RR_DECLSPEC FARPROC         getFunctionPtr(const string& funcName, HINSTANCE DLLHandle);
RR_DECLSPEC string          getWINAPIError(DWORD errorCode, LPTSTR lpszFunction);
#endif

#undef CreateFile
RR_DECLSPEC bool            createFile(const string& fName, std::ios_base::openmode mode = std::ios::trunc );



/**
 * Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970.
 *
 * Note, this is a *PRIVATE* function, only reason why it is exported is so that
 * the test suite can verify that it is increasing.
 */
RR_DECLSPEC int64_t getMicroSeconds();

} // rr Namespace
#endif
