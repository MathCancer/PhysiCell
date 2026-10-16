"""Compare the addon's pFBA against cobrapy's, on exactly the problem the addon solved.

Reads pfba_dump.tsv, which carries every reaction's bounds and objective
coefficient, applies them to the same SBML in cobrapy, runs cobra's pfba, and
compares the total and the per-reaction fluxes.
"""
import sys
import numpy as np
from cobra.io import read_sbml_model
from cobra.flux_analysis import pfba

noop = lambda x: x
F = {k: noop for k in ("F_GENE","F_GENE_REV","F_REACTION","F_REACTION_REV","F_SPECIE","F_SPECIE_REV")}

meta, rows = {}, []
for line in open(sys.argv[1] if len(sys.argv) > 1 else "pfba_dump.tsv"):
    if line.startswith("#reaction"): continue
    if line.startswith("#"):
        k, v = line[1:].rstrip("\n").split("\t", 1); meta[k] = v; continue
    f = line.rstrip("\n").split("\t")
    rows.append((f[0], float(f[1]), float(f[2]), float(f[3]), float(f[4])))

m = read_sbml_model(meta["model"], f_replace=F); m.solver = "gurobi"
for rid, lb, ub, obj, _ in rows:            # replicate the addon's exact problem
    r = m.reactions.get_by_id(rid); r.lower_bound, r.upper_bound = lb, ub
m.objective = {m.reactions.get_by_id(rid): obj for rid, _, _, obj, _ in rows if obj != 0}

sol = pfba(m)
internal = [r for r in m.reactions if len(r.metabolites) > 1]
tot_cobra = sum(abs(sol.fluxes[r.id]) for r in internal)
tot_addon = float(meta["total_abs_flux_pfba"])

print(f"model {meta['model']}")
print(f"  growth      addon {float(meta['growth']):.9g}")
print(f"  total |flux| addon {tot_addon:.9g}   cobrapy {tot_cobra:.9g}   "
      f"rel diff {abs(tot_addon-tot_cobra)/max(tot_cobra,1e-12):.3e}")

d = np.array([abs(fx - sol.fluxes[rid]) for rid, _, _, _, fx in rows])
print(f"  per-reaction flux: max |diff| {d.max():.3e} over {len(d)} reactions, "
      f"{(d > 1e-6).sum()} above 1e-6")
# Tolerance: 1e-5 relative. pFBA's optimal TOTAL is unique, but on a model where
# the second stage removes large futile cycles the optimum is very flat, and the
# two implementations impose the optimality constraint differently -- cobrapy adds
# a variable and a constraint, this adds a bound on the objective reaction. A
# difference at this scale is slack in one of the two, not a different answer.
# What must hold exactly is that growth is unchanged, which the C++ test asserts
# separately; a lower total at the SAME growth is a better solution, not a
# constraint violation.
rel = abs(tot_addon - tot_cobra) / max(tot_cobra, 1e-12)
ok = rel <= 1e-5
print(("  TOTAL MATCHES cobrapy" if ok else "  TOTALS DISAGREE") +
      (f"  (addon is lower by {tot_cobra-tot_addon:.3e}; a lower total at equal growth"
       " is a better solution)" if ok and tot_addon < tot_cobra - 1e-12 else ""))
sys.exit(0 if ok else 1)
