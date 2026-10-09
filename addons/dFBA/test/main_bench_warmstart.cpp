/*
 * Does the addon warm-start its LP between dFBA timesteps?
 *
 * The question is not rhetorical: a per-cell ClpSimplex that is reused across
 * timesteps keeps its basis, so when only the Michaelis-Menten bounds move the
 * next solve should resume from the previous optimum and take a handful of
 * simplex iterations instead of solving from scratch. If instead something
 * rebuilt or re-scaled the problem each step, the basis would be discarded and
 * every solve would pay full price -- which is what this measures.
 *
 * Simplex ITERATIONS are the primary evidence here, not wall time: they are a
 * property of the solve, immune to CPU contention and to this machine's very
 * noisy timings. Wall time is reported alongside as a sanity check.
 *
 *   warm  -- what the addon actually does: one model, move bounds, optimize()
 *   cold  -- a fresh model per step, counting the iterations initProblem()'s
 *            initialSolve() spends getting there from an all-slack basis
 *
 * The cold timing includes building the LP, which is most of its cost; the
 * iteration counts are the clean comparison.
 *
 * Both modes walk the SAME bound trajectory and must reach the same objective
 * at every step; the program checks that and reports the largest disagreement,
 * so a "speedup" that came from solving a different problem cannot pass.
 *
 *   make bench_warmstart && ./bench_warmstart [model.xml] [glc_id] [o2_id]
 */
#include <chrono>
#include <cmath>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

#include "../src/dfba_Model.h"
#include "../src/dfba_Reaction.h"

#include <coin/ClpSimplex.hpp>

using namespace std::chrono;

int main(int argc, char** argv)
{
    const char* path = (argc > 1) ? argv[1] : "data/Ecoli_core.xml";
    const char* glc  = (argc > 2) ? argv[2] : "R_EX_glc__D_e";
    const char* o2   = (argc > 3) ? argv[3] : "R_EX_o2_e";
    const int N = 1000;
    const int N_COLD = 50;   // each cold step re-parses the SBML file

    // Consecutive timesteps, not independent restarts: the bounds have to move
    // a little, the way local concentrations do, or there is nothing to reuse.
    std::mt19937 rng(5);
    std::uniform_real_distribution<double> jitter(0.95, 1.05);
    std::vector<double> gl(N), ox(N);
    double g = -10.0, o = -20.0;
    for (int i = 0; i < N; i++)
    {
        g *= jitter(rng); o *= jitter(rng);
        if (g < -20.0) g = -20.0;  if (g > -1.0) g = -1.0;
        if (o < -40.0) o = -40.0;  if (o > -2.0) o = -2.0;
        gl[i] = g; ox[i] = o;
    }

    std::vector<double> obj_warm(N, 0.0), obj_cold(N, 0.0);
    double iter_warm = 0, iter_cold = 0, t_warm = 0, t_cold = 0;
    int bad = 0;

    {   // warm: one problem, reused -- the addon's own behaviour
        dFBAModel m; m.readSBMLModel(path); m.initProblem();
        for (int i = 0; i < N; i++)
        {
            m.setReactionLowerBound(glc, gl[i]);
            m.setReactionLowerBound(o2,  ox[i]);
            auto t0 = high_resolution_clock::now();
            dFBASolution s = m.optimize();
            t_warm += duration<double>(high_resolution_clock::now() - t0).count();
            if (s.status != "optimal") { bad++; continue; }
            obj_warm[i] = s.objective_value;
            iter_warm += m.getLpModel()->numberIterations();
        }
    }

    {   // cold: a brand-new model per step, so there is no basis to inherit.
        //
        // The iterations that matter here are the ones initProblem() spends:
        // it ends in initialSolve(), which solves the LP from an all-slack
        // basis. Reading numberIterations() after a subsequent optimize()
        // would report ~0 for BOTH modes -- the problem is already optimal by
        // then -- which is exactly the trap that made the first version of
        // this benchmark say warm and cold were identical.
        // Only N_COLD steps: each one re-parses the SBML file, which dwarfs the
        // solve. The per-solve averages stay comparable because both modes walk
        // the same trajectory and this takes its prefix.
        for (int i = 0; i < N_COLD; i++)
        {
            dFBAModel m; m.readSBMLModel(path);
            // Set the bounds on the reaction objects, NOT through
            // setReactionLowerBound(): that writes into this->problem, which
            // does not exist until initProblem() builds it. initProblem() reads
            // these values when it loads the matrix, so the LP is built with
            // this step's bounds and then solved from an all-slack basis.
            m.getReaction(glc)->setLowerBound(gl[i]);
            m.getReaction(o2)->setLowerBound(ox[i]);
            auto t0 = high_resolution_clock::now();
            m.initProblem();                       // all-slack basis + initialSolve()
            t_cold += duration<double>(high_resolution_clock::now() - t0).count();
            iter_cold += m.getLpModel()->numberIterations();
            obj_cold[i] = m.getLpModel()->objectiveValue();
            if (!m.getLpModel()->isProvenOptimal()) bad++;
        }
    }

    double worst = 0.0;
    for (int i = 0; i < N_COLD; i++)
        worst = std::max(worst, std::fabs(obj_warm[i] - obj_cold[i]));

    printf("\n%s   %d consecutive steps\n", path, N);
    printf("  %-6s %12s %14s\n", "", "iters/solve", "ms/solve");
    printf("  %-6s %12.3f %14.4f\n", "warm", iter_warm / N, t_warm / N * 1000.0);
    printf("  %-6s %12.3f %14.4f   (n=%d, includes building the LP)\n",
           "cold", iter_cold / N_COLD, t_cold / N_COLD * 1000.0, N_COLD);
    const double w = iter_warm / N, c = iter_cold / N_COLD;
    if (w > 0.0) printf("  cold needs %.0fx the simplex iterations of a warm re-solve\n", c / w);
    else         printf("  warm re-solves took ZERO simplex iterations across all %d steps\n", N);
    printf("  worst objective disagreement warm vs cold: %.3e   non-optimal: %d\n\n",
           worst, bad);
    return 0;
}
