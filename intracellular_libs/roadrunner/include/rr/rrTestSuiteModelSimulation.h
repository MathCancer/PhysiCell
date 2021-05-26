#ifndef rrSBMLTestSuiteModelSimulationH
#define rrSBMLTestSuiteModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include "rrSBMLModelSimulation.h"
#include "rrStringUtils.h"
#include "rrRoadRunnerOptions.h"
#include "rrRoadRunnerData.h"

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC TestSuiteModelSimulation : public SBMLModelSimulation
{
    protected:
        int                     mCurrentCaseNumber;                     //If simulating test suite cases...
        string                  mModelSettingsFileName;
        RoadRunnerData          mResultData;
        RoadRunnerData          mReferenceData;
        RoadRunnerData          mErrorData;
        string                  GetSettingsFileNameForCase(int sim_case);
        string                  GetReferenceDataFileNameForCase(int caseNr);
        int                     mNrOfFailingPoints;
        double                  mLargestError;

    public:
                                TestSuiteModelSimulation(const string& dataOutputFolder = "", const string& modelFilePath = "", const string& modelFileName = "");
                               ~TestSuiteModelSimulation();
        void                    SetCaseNumber(int cNr);
        bool                    LoadReferenceData();
        bool                    CreateErrorData();
        bool                    SaveAllData();
        bool                    CopyFilesToOutputFolder();
        double                  LargestError();
        bool                    Pass();
        int                     NrOfFailingPoints();
        virtual bool            LoadSettings(const string& fName = "");
        virtual bool            LoadSettingsEx(const string& settingsFName);
};

}



#endif
