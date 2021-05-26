#ifndef rrNLEQInterfaceH
#define rrNLEQInterfaceH
#include <vector>
#include "rrExporter.h"
#include "rrExecutableModel.h"
#include "rrSteadyStateSolver.h"
using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQInterface : public rrSteadyStateSolver
{

public:
    /**
     * Creates a new Instance of NLEQ for the given Model
     */
    NLEQInterface(ExecutableModel *_model = NULL);

    ~NLEQInterface();

    /**
     * Thea actual solver routine making the call to NLEQ1
     *
     * @return sums of squares
     */
    double solve();


    /**
     * Implement Dictionary Interface
     */
public:

    /**
     * set an arbitrary key
     */
    virtual void setItem(const std::string& key, const rr::Variant& value);

    /**
     * get a value. Variants are POD.
     */
    virtual Variant getItem(const std::string& key) const;

    /**
     * is there a key matching this name.
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
     * list of keys that this integrator supports.
     */
    static const Dictionary* getSteadyStateOptions();


private:
    int nOpts;
    long *IWK;
    long LIWK;
    long LWRK;
    double *RWK;
    double *XScal;
    long ierr;
    long *iopt;
    ExecutableModel *model; // Model generated from the SBML. Static so we can access it from standalone function
    long n;
    void setup();

    bool isAvailable();

    int maxIterations;
    double relativeTolerance;
    double minDamping;


    /// <summary>
    /// Sets the Scaling Factors
    /// </summary>
    /// <param name="sx">Array of Scaling factors</param>
    void                            setScalingFactors(const vector<double>& sx);

    /// <summary>
    /// Returns the Number of Newton Iterations
    /// </summary>
    /// <returns>the Number of Newton Iterations</returns>
    int                             getNumberOfNewtonIterations();

    /// <summary>
    /// Returns the Number of Corrector steps
    /// </summary>
    /// <returns>Returns the Number of Corrector steps</returns>
    int                             getNumberOfCorrectorSteps();

    /// <summary>
    /// Returns the Number of Model Evaluations
    /// </summary>
    /// <returns>the Number of Model Evaluations</returns>
    int                             getNumberOfModelEvaluations();

    /// <summary>
    /// Returns the Number Of Jacobian Evaluations
    /// </summary>
    /// <returns>the Number Of Jacobian Evaluations</returns>
    int                             getNumberOfJacobianEvaluations();

    /// <summary>
    /// Returns the Number of Model Evaluations For Jacobian
    /// </summary>
    /// <returns>the Number of Model Evaluations For Jacobian</returns>
    int                             getNumberOfModelEvaluationsForJacobian();


    double                          computeSumsOfSquares();

    friend class NLEQSolver;

};
}

#endif
