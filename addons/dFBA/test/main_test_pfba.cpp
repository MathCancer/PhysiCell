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
    dFBASolution a = pf.optimize();
    CHECK(a.status == "optimal", "stage one solves");
    CHECK_NEAR(a.objective_value, mu_plain, 1e-9, "stage one matches plain FBA exactly");

    dFBASolution b = pf.optimizeParsimonious(a.objective_value);
    CHECK(b.status == "optimal", "stage two solves");

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
    CHECK_NEAR(b.objective_value, tot_pfba, 1e-6,
               "reported stage-two objective equals the realised total flux");

    DFBA_TEST_SECTION("pFBA is idempotent");
    dFBASolution c = pf.optimizeParsimonious(a.objective_value);
    CHECK_NEAR(c.objective_value, b.objective_value, 1e-9,
               "re-running stage two changes nothing");

    DFBA_TEST_SECTION("a deep copy carries the pFBA setting and solves independently");
    dFBAModel clone; clone.deepCopy(pf);
    CHECK(clone.isParsimoniousEnabled(), "clone has pFBA enabled");
    clone.setReactionLowerBound("R_EX_glc__D_e", GLC);
    clone.setReactionLowerBound("R_EX_o2_e", O2);
    dFBASolution d = clone.optimize();
    dFBASolution e = clone.optimizeParsimonious(d.objective_value);
    CHECK(e.status == "optimal", "clone's stage two solves");
    CHECK_NEAR(e.objective_value, b.objective_value, 1e-6, "clone agrees with the original");

    DFBA_TEST_SECTION("disabled by default");
    dFBAModel off; off.readSBMLModel(f.c_str()); off.initProblem();
    CHECK(!off.isParsimoniousEnabled(), "pFBA is off unless asked for");
    CHECK(off.optimizeParsimonious(1.0).status == "disabled",
          "calling stage two while disabled is a no-op, not a crash");

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
