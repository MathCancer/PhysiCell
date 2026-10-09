/*
 * Stage-two warm-start benchmark: dual() or primal()?
 *
 * Between dFBA timesteps only the bounds of the stage-two problem move (the
 * mirrored Michaelis-Menten bounds, and the objective floor that tracks the
 * stage-one optimum). Theory says a bound change leaves the previous basis
 * dual-feasible, which favours the dual simplex -- but that is an argument, not
 * a measurement, so measure it.
 *
 * The simplex actually used is chosen inside dFBAModel::solveParsimoniousStage()
 * from DFBA_PFBA_DUAL, so this program reads the SAME variable for its label.
 * Do not introduce a second switch here: a benchmark whose label and whose
 * subject disagree is worse than no benchmark.
 *
 *   make bench_pfba
 *   ./bench_pfba data/Ecoli_core.xml                 # the default simplex
 *   DFBA_PFBA_DUAL=1 ./bench_pfba data/Ecoli_core.xml
 */
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "../src/dfba_Model.h"
#include "../src/dfba_Reaction.h"

using namespace std::chrono;

int main(int argc, char** argv)
{
    const char* path = (argc > 1) ? argv[1] : "data/Ecoli_core.xml";
    const char* glc  = (argc > 2) ? argv[2] : "R_EX_glc__D_e";
    const char* o2   = (argc > 3) ? argv[3] : "R_EX_o2_e";
    const int N = 1000;

    const char* env = getenv("DFBA_PFBA_DUAL");
    const bool dual = (env != NULL && std::string(env) == "1");

    // A trajectory of bounds, not random restarts: consecutive steps must be
    // close together or there is no warm start to measure.
    std::mt19937 rng(5);
    std::uniform_real_distribution<double> jitter(0.8, 1.2);
    std::vector<double> gl(N), ox(N);
    for (int i = 0; i < N; i++) { gl[i] = -10.0 * jitter(rng); ox[i] = -20.0 * jitter(rng); }

    dFBAModel model;
    model.readSBMLModel(path);
    model.initProblem();
    model.enableParsimonious(1.0);

    // the same model without pFBA, walking the same trajectory, as the baseline
    dFBAModel plain;
    plain.readSBMLModel(path);
    plain.initProblem();

    double t_stage1 = 0.0, t_stage2 = 0.0, total_flux = 0.0;
    int non_optimal = 0, counted = 0;

    for (int i = 0; i < N; i++)
    {
        model.setReactionLowerBound(glc, gl[i]);
        model.setReactionLowerBound(o2,  ox[i]);

        // optimize() now runs both stages, so stage two cannot be timed in
        // isolation from here. Time the whole call against a pFBA-free model
        // solving the same trajectory, and report the difference as stage two.
        auto t0 = high_resolution_clock::now();
        dFBASolution both = model.optimize();
        const double t_both = duration<double>(high_resolution_clock::now() - t0).count();

        plain.setReactionLowerBound(glc, gl[i]);
        plain.setReactionLowerBound(o2,  ox[i]);
        auto t1 = high_resolution_clock::now();
        dFBASolution one = plain.optimize();
        const double t_one = duration<double>(high_resolution_clock::now() - t1).count();

        if (both.status != "optimal" || one.status != "optimal") { non_optimal++; continue; }
        if (both.secondary_status != "optimal") { non_optimal++; continue; }

        t_stage1 += t_one;
        t_stage2 += (t_both - t_one > 0.0) ? (t_both - t_one) : 0.0;
        total_flux += both.total_absolute_flux;
        counted++;
    }

    printf("%-28s %-7s  stage1 %7.4f ms  stage2 %7.4f ms  sum(total|flux|) %.9g"
           "  solved %d/%d  non-optimal %d\n",
           path, dual ? "dual" : "primal",
           t_stage1 / N * 1000.0, t_stage2 / N * 1000.0,
           total_flux, counted, N, non_optimal);
    return 0;
}
