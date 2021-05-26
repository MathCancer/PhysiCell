#ifndef rrSBMLModelSimulationH
#define rrSBMLModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include "rrExporter.h"
#include "rrStringUtils.h"
#include "rrRoadRunnerOptions.h"
#include "rrRoadRunnerData.h"

namespace rr
{

class RoadRunner;

/**
 * @internal
 *
 * Read an sbml test suite, simulate and compare the results.
 */
class RR_DECLSPEC SBMLModelSimulation
{
    protected:
        string                  mModelFileName;
        string                  mModelFilePath;
        string                  mModelSettingsFileName;
        string                  mSimulationLogFile;
        string                  mDataOutputFolder;
        string                  mTempDataFolder;
        RoadRunner             *mEngine;
        SimulateOptions         mSettings;
        bool                    mCompileIfDllExists;

    public:
                                SBMLModelSimulation(const string& dataOutputFolder = "", const string& tempDataFilePath = "");
        virtual                ~SBMLModelSimulation();
        bool                    SetModelFilePath(const string& path);
        bool                    SetModelFileName(const string& name);
        bool                    SetDataOutputFolder(const string& name);
        string                  GetModelsFullFilePath();
        string                  GetDataOutputFolder();
        string                  GetTempDataFolder();
        bool                    UseEngine(RoadRunner* engine);




        bool                    SaveModelAsXML(const string& folder);

        //API
        bool                    SetTimeStart(const double& tStart);
        bool                    SetTimeEnd(const double& tEnd);
        bool                    SetNumberOfPoints(const int& pts);
        bool                    SetSelectionList(const string& list);
        virtual bool            LoadSBMLFromFile();                    //Use current file information to load sbml from file
        virtual bool            Simulate();
        virtual bool            SaveResult();
        void                    loadSBMLTolerances(std::string const& filename);
        virtual bool            LoadSettings(const string& fName = "");
        virtual RoadRunnerData  GetResult();

        void                    ReCompileIfDllExists(const bool& doIt);
        bool                    DoCompileIfDllExists();

//        // models are created by loadSBML, if a shared lib already exits, it is loaded.
        bool                    CreateModel();
        bool                    GenerateModelCode();
        bool                    CompileModel();
        bool                    GenerateAndCompileModel();

        /// absolute tolerance for residual comparison, does not affect integrator
        double mAbsolute;
        /// relative tolerance for residual comparison, does not affect integrator
        double mRelative;
};

}



#endif
