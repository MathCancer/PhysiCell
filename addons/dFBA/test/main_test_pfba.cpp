/*
 * Parsimonious FBA: does the second stage preserve growth, reduce total flux,
 * and agree with cobrapy?
 *
 * Usage: ./test_pfba [model.xml]   (writes pfba_dump.tsv for the cobrapy check)
 */
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

#include "../src/dfba_Model.h"
#include "../src/dfba_Reaction.h"
#include "dfba_test.h"

int main(int argc, char** argv)
{
    const std::string f = (argc > 1) ? argv[1] : "data/Ecoli_core.xml";
    const double GLC = -10.0, O2 = -20.0;

    DFBA_TEST_SECTION("stage one is unchanged when pFBA is off");
    dFBAModel plain;
    plain.readSBMLModel(f.c_str()); plain.initProblem();
    plain.setReactionLowerBound("R_EX_glc__D_e", GLC);
    plain.setReactionLowerBound("R_EX_o2_e", O2);
    dFBASolution s1 = plain.optimize();
    CHECK(s1.status == "optimal", "plain FBA solves");
    const double mu_plain = s1.objective_value;
    const double tot_plain = plain.getTotalAbsoluteFlux();
    printf("   growth %.9g, total |flux| %.9g\n", mu_plain, tot_plain);

    DFBA_TEST_SECTION("pFBA preserves growth and reduces total flux");
    dFBAModel pf;
    pf.readSBMLModel(f.c_str()); pf.initProblem();
    pf.enableParsimonious(1.0);
    pf.setReactionLowerBound("R_EX_glc__D_e", GLC);
    pf.setReactionLowerBound("R_EX_o2_e", O2);
    // ONE call. The second stage is run by optimize() itself, so this is also
    // the test that a caller cannot forget to apply pFBA.
    dFBASolution a = pf.optimize();
    CHECK(a.status == "optimal", "stage one solves");
    CHECK(a.secondary_status == "optimal", "optimize() ran the second stage by itself");
    CHECK_NEAR(a.objective_value, mu_plain, 1e-9,
               "objective_value is stage one's growth, not stage two's total flux");

    double mu_after = 0.0;
    for (auto r : pf.getListOfReactions())
        if (r->getObjectiveCoefficient() != 0.0) mu_after = r->getFluxValue();
    CHECK_NEAR(mu_after, mu_plain, 1e-6, "growth is preserved by stage two");

    const double tot_pfba = pf.getTotalAbsoluteFlux();
    printf("   total |flux|: plain %.9g -> pFBA %.9g\n", tot_plain, tot_pfba);
    CHECK(tot_pfba <= tot_plain + 1e-6, "total flux does not increase");
    if (tot_plain - tot_pfba > 1e-6)
        printf("   stage two reduced total flux by %.3g%% -- the FBA optimum was degenerate\n",
               100.0 * (1.0 - tot_pfba / tot_plain));
    else
        printf("   no reduction: the FBA optimum was already unique at these bounds\n");
    CHECK_NEAR(a.total_absolute_flux, tot_pfba, 1e-6,
               "total_absolute_flux describes the distribution being returned");

    // The unified reversible flux. Each split reaction's original column is
    // constrained by v - p + m = 0, so Sum(p+m) from the LP can only equal
    // Sum|v| read back off the reactions if every column really does hold
    // p - m with at most one of the pair nonzero. This is what makes reading v
    // directly correct, rather than subtracting p and m by hand.
    CHECK_NEAR(pf.getTotalAbsoluteFlux(), tot_pfba, 1e-9,
               "reversible fluxes are the unified p - m, consistent with Sum(p+m)");

    DFBA_TEST_SECTION("pFBA is idempotent");
    dFBASolution c = pf.optimize();
    CHECK_NEAR(c.total_absolute_flux, a.total_absolute_flux, 1e-9,
               "re-solving changes nothing");
    CHECK_NEAR(c.objective_value, a.objective_value, 1e-12, "growth is stable too");

    DFBA_TEST_SECTION("a deep copy carries the pFBA setting and solves independently");
    dFBAModel clone; clone.deepCopy(pf);
    CHECK(clone.isParsimoniousEnabled(), "clone has pFBA enabled");
    clone.setReactionLowerBound("R_EX_glc__D_e", GLC);
    clone.setReactionLowerBound("R_EX_o2_e", O2);
    dFBASolution d = clone.optimize();
    CHECK(d.secondary_status == "optimal", "clone's second stage ran");
    CHECK_NEAR(d.total_absolute_flux, a.total_absolute_flux, 1e-6,
               "clone agrees with the original");
    CHECK_NEAR(d.objective_value, mu_plain, 1e-6, "clone's growth matches too");

    DFBA_TEST_SECTION("disabled by default, and then nothing changes");
    dFBAModel off; off.readSBMLModel(f.c_str()); off.initProblem();
    CHECK(!off.isParsimoniousEnabled(), "pFBA is off unless asked for");
    off.setReactionLowerBound("R_EX_glc__D_e", GLC);
    off.setReactionLowerBound("R_EX_o2_e", O2);
    dFBASolution g = off.optimize();
    CHECK(g.secondary_status == "none", "no second stage is reported when none is configured");
    CHECK_NEAR(g.objective_value, mu_plain, 1e-12,
               "a pFBA-free model still solves exactly the problem it always did");
    CHECK_NEAR(off.getTotalAbsoluteFlux(), tot_plain, 1e-9,
               "and returns the same, unreduced flux distribution");

    DFBA_TEST_SECTION("a reused model stays healthy across many bound changes");
    // Regression test for a real failure. The second-stage problem is
    // warm-started across timesteps, and after many re-solves with moving
    // bounds Clp returned status 4 -- "stopped due to errors" -- on problems
    // that solve fine from a fresh model. Before the retry-from-clean-basis
    // in solveParsimoniousStage(), 3 of these 81 combinations failed, and the
    // affected cells silently kept stage-one fluxes 256x less parsimonious
    // than the ones pFBA would have given them.
    {
        dFBAModel sweep;
        sweep.readSBMLModel(f.c_str()); sweep.initProblem();
        sweep.enableParsimonious(1.0);
        const double gs[] = {0.0, -1e-9, -1e-6, -1e-3, -0.01, -0.1, -1.0, -5.0, -10.0};
        const double os[] = {0.0, -1e-9, -1e-6, -1e-3, -0.01, -0.1, -1.0, -5.0, -20.0};
        int solved = 0, stage_two_failed = 0;
        for (size_t i = 0; i < sizeof(gs)/sizeof(*gs); i++)
          for (size_t j = 0; j < sizeof(os)/sizeof(*os); j++)
          {
            sweep.setReactionLowerBound("R_EX_glc__D_e", gs[i]);
            sweep.setReactionLowerBound("R_EX_o2_e", os[j]);
            dFBASolution s = sweep.optimize();
            if (s.status != "optimal") continue;
            solved++;
            if (s.secondary_status != "optimal") stage_two_failed++;
          }
        printf("   %d of %d feasible bound sets reached stage two\n",
               solved - stage_two_failed, solved);
        CHECK_EQ(stage_two_failed, 0,
                 "the second stage solves for every feasible bound set on a reused model");
    }

    // Dump the COMPLETE problem, not just the two bounds the test tried to set:
    // the reaction ids differ between models (R_EX_glc__D_e vs R_EX_glc_e), so
    // the only way the cobrapy check can replicate exactly what was solved is to
    // be handed every bound.
    FILE* out = fopen("pfba_dump.tsv", "w");
    fprintf(out, "#model\t%s\n", f.c_str());
    fprintf(out, "#growth\t%.17g\n#total_abs_flux_fba\t%.17g\n#total_abs_flux_pfba\t%.17g\n",
            mu_plain, tot_plain, tot_pfba);
    fprintf(out, "#reaction\tlb\tub\tobj\tflux_pfba\n");
    for (auto r : pf.getListOfReactions())
        fprintf(out, "%s\t%.17g\t%.17g\t%.17g\t%.17g\n", r->getId().c_str(),
                r->getLowerBound(), r->getUpperBound(),
                r->getObjectiveCoefficient(), r->getFluxValue());
    fclose(out);
    printf("\nwrote pfba_dump.tsv for the cobrapy comparison\n");

    return DFBA_TEST_SUMMARY();
}
