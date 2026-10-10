"""
Quantify how degenerate the FBA flux distribution is in the cancer_core_metabolism
simulation, by replaying each cell's own LP offline and running FVA on it.

Why
---
Standard FBA has alternative optima: many flux distributions give the same growth
rate, and the solver returns whichever vertex the simplex lands on. In dFBA that
matters more than in a steady-state study, because the exchange fluxes are what
couple the cell to BioFVM. If the secretion profile is not unique, neither is the
extracellular environment it produces.

This module measures that freedom rather than assuming it. For every distinct set
of exchange bounds the simulation actually generated, it reconstructs the LP,
fixes the objective at its optimum, and asks how far each exchange flux could move
without changing growth. The width of that interval is the degeneracy.

How the LP is reconstructed
---------------------------
The addon constrains the problem in three places, and all three are reproduced
here or the replay would be answering a different question:

  1. each exchange reaction's lower bound is the Michaelis-Menten uptake limit
     computed from the local substrate concentration -- this is the per-cell,
     per-step quantity recorded into custom_data as "<flux_id>_lb";
  2. the objective reaction's upper bound is capped at max_growth_rate;
  3. the death model's trigger flux is given a lower bound equal to
     death_flux_threshold (here a maintenance ATP demand).

Exchange upper bounds are never touched by the simulation, so they keep their
static values from the SBML.

Identifiers keep their SBML prefixes (R_, M_), matching the addon, so cobrapy is
loaded with its identifier rewriting disabled.

Solver choice is not neutral: do not use GLPK
---------------------------------------------
Measured on the MCF7 core model over 200 bound sets, comparing three solvers on
identical problems:

                      fraction 1.0     fraction 0.999
  glpk   vs gurobi      1.7e-01          1.9e-05
  glpk   vs cplex       1.7e-01          1.9e-05
  gurobi vs cplex       5.0e-06          1.6e-06

Gurobi and CPLEX, two independent implementations, agree to 5e-06 even with the
objective pinned exactly at the optimum, and both disagree with GLPK by the same
1.7e-01 -- the size of the degeneracy being measured. GLPK is the outlier on
these degenerate LPs, not the method. It also occasionally returns a flux as NaN
"due to numerical instability".

So the default solver is gurobi, fraction_of_optimum stays at 1.0 (the exact
question: how much freedom exists *at* the optimum), and GLPK results should not
be reported. Relaxing to 0.999 shrinks the residual gurobi/cplex gap a little
further and is a reasonable robustness check, but is not needed for correctness.

Usage
-----
    python3 fva_degeneracy.py --output-dir ../output --sbml ../config/MCF7_Zielinski_2017_core_trimmed.sbml

Requires cobra, pandas, numpy, scipy. On a machine with several Python
installations, use the interpreter that can import cobra.
"""

from __future__ import annotations

import argparse
import glob
import os
import xml.etree.ElementTree as ET
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Sequence

import numpy as np
import pandas as pd
import scipy.io as sio

# --------------------------------------------------------------------------- #
# Configuration of the simulation being replayed.
# These must match the <intracellular> block of the configuration that produced
# the output, or the reconstructed LP is not the one the cells actually solved.
# --------------------------------------------------------------------------- #

@dataclass
class ModelSetup:
    objective_reaction: str = "R_biomass_reaction"
    max_growth_rate: float = 0.026803          # objective reaction upper bound
    death_trigger_flux: Optional[str] = "R_ATPM"
    death_flux_threshold: float = 1.07         # lower bound on the trigger flux
    exchanges: Sequence[str] = field(default_factory=lambda: [
        "R_EX_arg_L_e", "R_EX_asn_L_e", "R_EX_asp_L_e", "R_EX_chol_e",
        "R_EX_co2_e", "R_EX_glc_e", "R_EX_gln_L_e", "R_EX_glu_L_e",
        "R_EX_gly_e", "R_EX_ile_L_e", "R_EX_lac_L_e", "R_EX_leu_L_e",
        "R_EX_lys_L_e", "R_EX_nh4_e", "R_EX_o2_e", "R_EX_orn_e",
        "R_EX_phe_L_e", "R_EX_pi_e", "R_EX_ser_L_e", "R_EX_thr_L_e",
        "R_EX_trp_L_e", "R_EX_tyr_L_e", "R_EX_val_L_e",
    ])
    sbml_filename: Optional[str] = None        # as written in the configuration

    @classmethod
    def from_config(cls, xml_path: str, cell_definition: Optional[str] = None
                    ) -> "ModelSetup":
        """Build the setup by reading the simulation's own configuration.

        Preferred over writing the values out by hand: the replay has to apply
        exactly the constraints the addon applied, and transcribing them is how
        that silently stops being true.

        ``cell_definition`` selects which definition to read when a project has
        more than one carrying an <intracellular> block; the default is the
        first one found.
        """
        root = ET.parse(xml_path).getroot()

        node = None
        for cd in root.iter("cell_definition"):
            intra = cd.find(".//intracellular")
            if intra is None or intra.get("type") != "dfba":
                continue
            if cell_definition is None or cd.get("name") == cell_definition:
                node = intra
                break
        if node is None:
            raise ValueError(f"{xml_path}: no dfba <intracellular> block"
                             + (f" for cell definition {cell_definition!r}"
                                if cell_definition else ""))

        growth = node.find("growth_model")
        objective = growth.find("objective_reaction").text.strip()
        max_mu = float(growth.find("max_growth_rate").text)

        # The addon floors the trigger flux only when the death model is enabled.
        trigger, threshold = None, 0.0
        death = node.find("death_model")
        if death is not None and death.get("enabled", "false").lower() == "true":
            t = death.find("death_trigger_flux")
            v = death.find("death_flux_threshold")
            if t is not None and (t.text or "").strip() not in ("", "None"):
                trigger = t.text.strip()
                threshold = float(v.text) if v is not None else 0.0

        exchanges = [e.find("fba_flux").text.strip()
                     for e in node.find("transport_model").findall("exchange")]

        sbml = node.find("settings/sbml_filename")
        setup = cls(objective_reaction=objective, max_growth_rate=max_mu,
                    death_trigger_flux=trigger, death_flux_threshold=threshold,
                    exchanges=exchanges,
                    sbml_filename=sbml.text.strip() if sbml is not None else None)
        print(f"setup from {os.path.basename(xml_path)}: {len(exchanges)} exchanges, "
              f"objective {objective} <= {max_mu}, "
              + (f"{trigger} >= {threshold}" if trigger else "no death-model floor"))
        return setup


# --------------------------------------------------------------------------- #
# Reading PhysiCell cell output
# --------------------------------------------------------------------------- #

def expand_labels(labels_node: ET.Element) -> List[str]:
    """Expand MultiCellDS cell labels into one name per matrix row.

    A label with size > 1 covers that many consecutive rows. Those are indexed
    explicitly here as ``name[i]``.

    This deliberately does not use the ``_x``/``_y``/``_z`` suffix convention
    found in some readers: that list has only three entries, while several of
    these arrays are sized by the number of substrates (23 in this project).
    Past the third element such readers either raise IndexError or emit
    duplicate names, in which case only the last substrate's values survive and
    the label list no longer aligns with the matrix rows.
    """
    names: List[str] = []
    for label in labels_node.findall("label"):
        size = int(label.get("size"))
        name = (label.text or "").strip()
        if size == 1:
            names.append(name)
        else:
            names.extend(f"{name}[{i}]" for i in range(size))
    return names


def read_cells_snapshot(xml_path: str) -> pd.DataFrame:
    """Read one MultiCellDS snapshot into a DataFrame with one row per cell."""
    root = ET.parse(xml_path).getroot()
    node = root.find(".//cell_population/custom/simplified_data")
    if node is None:
        raise ValueError(f"{xml_path}: no simplified_data node")

    labels = expand_labels(node.find("labels"))
    mat_name = node.find("filename").text
    mat_path = os.path.join(os.path.dirname(xml_path), mat_name)
    data = sio.loadmat(mat_path)["cells"]

    if data.shape[0] != len(labels):
        raise ValueError(
            f"{xml_path}: {len(labels)} labels but {data.shape[0]} matrix rows. "
            "The label expansion and the matrix have diverged; do not trust any "
            "column past the first multi-element variable."
        )

    time_node = root.find(".//current_time")
    time = float(time_node.text) if time_node is not None else np.nan

    df = pd.DataFrame(data.T, columns=labels)
    df.insert(0, "time", time)

    # Readable aliases for the position triple, which expand_labels names
    # position[0..2]. x is depth into the tissue from the vessel in this project.
    for i, axis in enumerate("xyz"):
        col = f"position[{i}]"
        if col in df.columns:
            df[axis] = df[col]
    return df


def read_run(output_dir: str, every: int = 1) -> pd.DataFrame:
    """Read every snapshot in an output directory, concatenated.

    ``every`` subsamples snapshots (``every=6`` keeps one in six).
    """
    xml_files = sorted(glob.glob(os.path.join(output_dir, "output*.xml")))
    if not xml_files:
        raise FileNotFoundError(f"no output*.xml in {output_dir}")
    frames = [read_cells_snapshot(f) for f in xml_files[::every]]
    df = pd.concat(frames, ignore_index=True)
    print(f"read {len(frames)} snapshots from {output_dir}: "
          f"{len(df)} cell-timepoints, t = {df.time.min():.0f} to {df.time.max():.0f} min")
    return df


# --------------------------------------------------------------------------- #
# Deduplicating the recorded bound sets
# --------------------------------------------------------------------------- #

def bound_matrix(df: pd.DataFrame, exchanges: Sequence[str]) -> np.ndarray:
    missing = [f"{e}_lb" for e in exchanges if f"{e}_lb" not in df.columns]
    if missing:
        raise KeyError(
            f"missing recorded lower bounds: {missing}. The simulation must be run "
            "with the custom_data variables that record them."
        )
    return df[[f"{e}_lb" for e in exchanges]].to_numpy(dtype=float)


def unique_bound_sets(df: pd.DataFrame, exchanges: Sequence[str],
                      decimals: int = 5) -> tuple:
    """Collapse identical bound vectors.

    Returns ``(unique_bounds, index_per_row)`` where ``index_per_row[i]`` is the
    row of ``unique_bounds`` that cell-timepoint ``i`` maps to.

    The compression is usually large, because the Michaelis-Menten bound is a
    function of the local substrate concentration alone -- not of the cell -- so
    every cell sharing a voxel at a given time has an identical bound vector.
    The exception is an exchange whose uptake was clamped to the amount actually
    present in the voxel, which does depend on the cell's dry weight.

    ``decimals`` sets the rounding applied before comparison. The default is
    fine enough to be effectively exact; coarsen it to trade a little accuracy
    for fewer FVA problems, and report whatever value was used.
    """
    bounds = np.round(bound_matrix(df, exchanges), decimals=decimals)
    uniq, inverse = np.unique(bounds, axis=0, return_inverse=True)
    print(f"bound sets: {len(bounds)} recorded -> {len(uniq)} unique "
          f"(x{len(bounds) / max(len(uniq), 1):.1f} compression, rounded at 1e-{decimals})")
    return uniq, inverse


# --------------------------------------------------------------------------- #
# The replay
# --------------------------------------------------------------------------- #

def load_model_quiet(sbml_path: str, solver: Optional[str] = None):
    """load_model without the banner, for worker processes."""
    from cobra.io import read_sbml_model
    noop = lambda x: x
    model = read_sbml_model(sbml_path, f_replace={
        "F_GENE": noop, "F_GENE_REV": noop,
        "F_REACTION": noop, "F_REACTION_REV": noop,
        "F_SPECIE": noop, "F_SPECIE_REV": noop,
    })
    if solver:
        model.solver = solver
    return model


def load_model(sbml_path: str, solver: Optional[str] = None):
    """Load the SBML with cobrapy, keeping the R_/M_ identifier prefixes."""
    from cobra.io import read_sbml_model
    noop = lambda x: x
    model = read_sbml_model(sbml_path, f_replace={
        "F_GENE": noop, "F_GENE_REV": noop,
        "F_REACTION": noop, "F_REACTION_REV": noop,
        "F_SPECIE": noop, "F_SPECIE_REV": noop,
    })
    print(f"loaded {sbml_path}: {len(model.reactions)} reactions, "
          f"{len(model.metabolites)} metabolites, {len(model.genes)} genes")
    return model


def apply_simulation_constraints(model, setup: ModelSetup, verbose: bool = True) -> None:
    """Apply the constraints the addon imposes once, at initialisation.

    Call this on the model (not inside a per-bound-set context) so it applies to
    every replay.
    """
    obj = model.reactions.get_by_id(setup.objective_reaction)
    obj.upper_bound = setup.max_growth_rate
    model.objective = obj
    if setup.death_trigger_flux:
        trig = model.reactions.get_by_id(setup.death_trigger_flux)
        trig.lower_bound = setup.death_flux_threshold
        if verbose:
            print(f"constraints: {obj.id}.ub = {obj.upper_bound}, "
                  f"{trig.id}.lb = {trig.lower_bound}")
    elif verbose:
        print(f"constraints: {obj.id}.ub = {obj.upper_bound}, no death-model floor")


def fva_for_bound_sets(model, setup: ModelSetup, uniq_bounds: np.ndarray,
                       fraction_of_optimum: float = 1.0,
                       progress_every: int = 50) -> pd.DataFrame:
    """Run FVA over the exchange reactions for each unique bound set.

    For each set: apply the recorded lower bounds, maximise the objective to get
    Z*, then find the min and max of every exchange flux subject to the objective
    staying at (a fraction of) Z*. The width max - min is the degeneracy of that
    flux at that operating point.

    Infeasible bound sets are recorded with status "infeasible" and NaN ranges;
    they correspond to cells the simulation also found infeasible, i.e. starving.
    """
    from cobra.flux_analysis import flux_variability_analysis

    exchanges = list(setup.exchanges)
    records = []
    for k, lbs in enumerate(uniq_bounds):
        if progress_every and k % progress_every == 0:
            print(f"  FVA {k}/{len(uniq_bounds)}", flush=True)
        with model:
            for rid, lb in zip(exchanges, lbs):
                model.reactions.get_by_id(rid).lower_bound = float(lb)

            sol = model.optimize()
            if sol.status != "optimal" or sol.objective_value is None:
                records.append({"set": k, "status": sol.status, "growth": np.nan})
                continue
            growth = float(sol.objective_value)

            try:
                # processes=1 is required, not merely faster: this is called
                # inside daemonic Pool workers, which cannot spawn children,
                # and cobrapy's FVA would otherwise try to.
                fva = flux_variability_analysis(
                    model, reaction_list=exchanges,
                    fraction_of_optimum=fraction_of_optimum, loopless=False,
                    processes=1)
            except Exception as exc:                      # solver trouble, not biology
                records.append({"set": k, "status": f"fva_failed: {exc}",
                                "growth": growth})
                continue

            row = {"set": k, "status": "optimal", "growth": growth}
            for rid in exchanges:
                lo = float(fva.at[rid, "minimum"])
                hi = float(fva.at[rid, "maximum"])
                row[f"{rid}_min"] = lo
                row[f"{rid}_max"] = hi
                row[f"{rid}_range"] = hi - lo
            records.append(row)

    out = pd.DataFrame.from_records(records).set_index("set")
    if progress_every:
        n_ok = int((out.status == "optimal").sum())
        print(f"FVA done: {n_ok}/{len(out)} bound sets optimal")
    return out


# --------------------------------------------------------------------------- #
# A leaner FVA, and the parallel driver
# --------------------------------------------------------------------------- #

_WORKER: Dict[str, object] = {}


def _init_worker(sbml_path: str, setup: ModelSetup,
                 solver_timeout: Optional[float] = None,
                 solver: Optional[str] = None,
                 fraction_of_optimum: float = 1.0) -> None:
    import logging
    logging.getLogger("cobra").setLevel(logging.ERROR)
    model = load_model_quiet(sbml_path, solver=solver)
    apply_simulation_constraints(model, setup, verbose=False)
    if solver_timeout:
        # A small number of bound sets are pathological for the simplex and
        # spin indefinitely. Without a limit, one such set stalls its worker
        # while every other worker sits idle and the run never finishes --
        # observed here as a job frozen at 66000/66669 for 25 minutes with a
        # single worker burning 4x the CPU of its peers. With a limit the set
        # is reported as timed out and counted.
        try:
            model.solver.configuration.timeout = solver_timeout
        except Exception as exc:
            print(f"  (solver timeout unsupported: {exc})", flush=True)
    _WORKER["model"] = model
    _WORKER["setup"] = setup
    _WORKER["fraction"] = fraction_of_optimum


def fva_one(model, setup: ModelSetup, lbs: Sequence[float],
            fraction_of_optimum: float = 1.0) -> dict:
    """FVA for a single bound set, driven straight through optlang.

    About 2.5x the throughput of cobra.flux_analysis.flux_variability_analysis
    on this model, because it does not rebuild the problem and assemble a
    DataFrame per call.

    NOT used by default. verify_against_cobrapy() shows it agrees to ~2e-13 on
    well-conditioned bound sets but diverges by up to ~6e-6 on near-degenerate
    ones, presumably because the optimum-fixing constraint is imposed slightly
    differently and the interval endpoints are sensitive to that near a
    degenerate vertex. Since the size of those intervals is exactly what is
    being reported, the reference implementation runs instead and the speed
    comes from parallelism. Kept, and kept verifiable, in case the gap is worth
    closing later.
    """
    exchanges = list(setup.exchanges)
    with model:
        for rid, lb in zip(exchanges, lbs):
            model.reactions.get_by_id(rid).lower_bound = float(lb)

        growth = model.slim_optimize()
        if growth != growth:                       # NaN: infeasible
            return {"status": "infeasible", "growth": float("nan")}

        obj = model.reactions.get_by_id(setup.objective_reaction)
        obj.lower_bound = fraction_of_optimum * growth

        row = {"status": "optimal", "growth": float(growth)}
        for rid in exchanges:
            rxn = model.reactions.get_by_id(rid)
            model.objective = rxn
            model.objective.direction = "min"
            lo = model.slim_optimize()
            model.objective.direction = "max"
            hi = model.slim_optimize()
            row[f"{rid}_min"] = float(lo)
            row[f"{rid}_max"] = float(hi)
            row[f"{rid}_range"] = float(hi - lo)
        return row


def _work(item):
    """One bound set, through cobrapy's own FVA.

    fva_one below is about 2.5x faster, but on near-degenerate bound sets it
    diverges from cobrapy by up to ~6e-6 in the interval endpoints -- small, but
    the whole point of this analysis is the size of those intervals, so the
    reference implementation is what runs. The speed comes from parallelism
    instead, which changes no numbers at all.
    """
    k, lbs = item
    model, setup = _WORKER["model"], _WORKER["setup"]
    df = fva_for_bound_sets(model, setup, [lbs], progress_every=0,
                            fraction_of_optimum=_WORKER["fraction"])
    row = df.iloc[0].to_dict()
    row["set"] = k
    return row


def fva_parallel(sbml_path: str, setup: ModelSetup, uniq_bounds: np.ndarray,
                 processes: Optional[int] = None,
                 chunksize: int = 8,
                 solver_timeout: Optional[float] = 20.0,
                 solver: Optional[str] = None,
                 fraction_of_optimum: float = 1.0) -> pd.DataFrame:
    """Run fva_one over every unique bound set, across processes."""
    import multiprocessing as mp
    if processes is None:
        processes = max(1, (os.cpu_count() or 2) - 1)
    print(f"FVA over {len(uniq_bounds)} bound sets on {processes} processes "
          f"({len(uniq_bounds) * (2 * len(setup.exchanges) + 1):,} LPs)", flush=True)

    items = list(enumerate(uniq_bounds))
    _WORKER["fraction"] = fraction_of_optimum
    with mp.Pool(processes, initializer=_init_worker,
                 initargs=(sbml_path, setup, solver_timeout, solver,
                           fraction_of_optimum)) as pool:
        records = []
        for i, row in enumerate(pool.imap_unordered(_work, items, chunksize=chunksize), 1):
            records.append(row)
            if i % 2000 == 0:
                print(f"  {i}/{len(items)}", flush=True)

    out = pd.DataFrame.from_records(records).set_index("set").sort_index()
    n_ok = int((out.status == "optimal").sum())
    print(f"FVA done: {n_ok}/{len(out)} bound sets optimal")

    failures = out.status[~out.status.isin(["optimal", "infeasible"])]
    if len(failures):
        print("  solver failures (distinct reasons):")
        for reason, n in failures.value_counts().items():
            print(f"    {n:>7} x {reason}")
    if n_ok == 0:
        raise RuntimeError(
            "no bound set solved. Infeasible cells are expected, but zero "
            "optimal means the replay is broken rather than the biology; see "
            "the reasons above.")
    return out


def verify_against_cobrapy(sbml_path: str, setup: ModelSetup,
                           uniq_bounds: np.ndarray, n: int = 60,
                           tol: float = 1e-7) -> bool:
    """Check the lean path reproduces cobrapy's own FVA.

    A faster inner loop is only worth having if it computes the same thing, so
    this is run before trusting it rather than assumed.
    """
    model = load_model_quiet(sbml_path)
    apply_simulation_constraints(model, setup, verbose=False)
    sample = uniq_bounds[:: max(1, len(uniq_bounds) // n)][:n]

    ref = fva_for_bound_sets(model, setup, sample, progress_every=0)
    worst, checked = 0.0, 0
    for k, lbs in enumerate(sample):
        mine = fva_one(model, setup, lbs)
        if mine["status"] != ref.at[k, "status"]:
            print(f"!! status mismatch at set {k}: {mine['status']} vs {ref.at[k,'status']}")
            return False
        if mine["status"] != "optimal":
            continue
        for rid in setup.exchanges:
            for suffix in ("_min", "_max"):
                a, b = mine[rid + suffix], float(ref.at[k, rid + suffix])
                worst = max(worst, abs(a - b)); checked += 1
    ok = worst <= tol
    print(f"lean FVA vs cobrapy on {len(sample)} bound sets: "
          f"{checked} bounds compared, worst difference {worst:.3e} "
          f"-> {'MATCH' if ok else 'MISMATCH'}")
    return ok


# --------------------------------------------------------------------------- #
# Self-check and summary
# --------------------------------------------------------------------------- #

def check_recorded_flux_within_range(df: pd.DataFrame, inverse: np.ndarray,
                                     fva: pd.DataFrame, setup: ModelSetup,
                                     tol: float = 1e-6,
                                     decimals: Optional[int] = None,
                                     solver_tol: float = 1e-6) -> pd.DataFrame:
    """Verify that each flux the simulation recorded lies inside its FVA interval.

    This is the strongest available check that the replay reconstructed the same
    LP the simulation solved. A recorded flux outside its own feasible interval
    means the replay and the simulation disagree about the problem -- a missing
    constraint, a wrong bound, or an identifier mismatch -- not that FBA is wrong.

    ``decimals`` must be the rounding used to deduplicate the bound sets. The
    replay solves with the rounded bound while the simulation used the exact
    one, so a flux sitting on its uptake bound -- which most uptake-limited
    fluxes do -- can legitimately fall outside the replayed interval by up to
    half of that rounding step. Ignoring this reports every uptake-limited flux
    as a violation: at decimals=5 that was 163,662 spurious failures, all of
    them at or below 5e-6 and none above 1e-4.
    """
    if decimals is not None:
        # Two independent allowances, which add: half the dedup rounding step,
        # because the replay solves with the rounded bound while the simulation
        # used the exact one; and the solver's own feasibility tolerance, since
        # a flux reported as sitting on a bound only does so to within that.
        rounding = 0.5 * 10.0 ** (-decimals)
        tol = max(tol, rounding + solver_tol)
        print(f"self-check tolerance {tol:.1e} "
              f"= {rounding:.1e} dedup rounding + {solver_tol:.1e} solver feasibility")
    rows = []
    for rid in setup.exchanges:
        if f"{rid}_min" not in fva.columns:
            continue
        flux = df[rid].to_numpy(dtype=float)
        lo = fva[f"{rid}_min"].to_numpy()[inverse]
        hi = fva[f"{rid}_max"].to_numpy()[inverse]
        ok = np.isnan(lo) | ((flux >= lo - tol) & (flux <= hi + tol))
        rows.append({"reaction": rid,
                     "checked": int(np.sum(~np.isnan(lo))),
                     "outside": int(np.sum(~ok)),
                     "worst_excess": float(np.nanmax(
                         np.maximum(lo - flux, flux - hi))) if len(flux) else np.nan})
    out = pd.DataFrame(rows)
    if out.empty:
        print("!! self-check could not run: no FVA intervals were produced")
        return out
    bad = int(out.outside.sum())
    if bad:
        print(f"!! {bad} recorded fluxes fall outside their FVA interval -- the "
              "replay does not match the simulation's LP. Investigate before "
              "trusting any range below.")
    else:
        print("self-check passed: every recorded flux lies within its FVA interval")
    return out


def verify_replay_exact(df: pd.DataFrame, setup: ModelSetup, sbml_path: str,
                        n: int = 500, solver: Optional[str] = "gurobi",
                        tol: float = 1e-6, seed: int = 0) -> bool:
    """Validate the replay using each cell's EXACT bounds, with no rounding.

    The routine self-check compares a cell's flux against the interval computed
    for the *rounded* representative of its bound group, so it is limited by the
    deduplication step: a bound shifted by the rounding moves the interval
    endpoint by that much times the LP's sensitivity, which is not a fixed
    constant. That makes it a poor test of whether the replay is faithful.

    This asks the question directly instead. For a random sample of
    cell-timepoints it solves the cell's own unrounded bound vector and checks
    the recorded flux lies inside the resulting interval. Nothing is rounded, so
    a failure here means the reconstructed LP genuinely differs from the one the
    simulation solved.
    """
    rng = np.random.default_rng(seed)
    idx = rng.choice(len(df), size=min(n, len(df)), replace=False)
    exact = df[[f"{e}_lb" for e in setup.exchanges]].to_numpy(float)[idx]

    model = load_model_quiet(sbml_path, solver=solver)
    apply_simulation_constraints(model, setup, verbose=False)

    worst, checked, bad, skipped = 0.0, 0, 0, 0
    for row, i in zip(exact, idx):
        res = fva_for_bound_sets(model, setup, [row], progress_every=0)
        if res.iloc[0]["status"] != "optimal":
            skipped += 1
            continue
        for rid in setup.exchanges:
            f = float(df[rid].to_numpy(float)[i])
            lo, hi = float(res.iloc[0][f"{rid}_min"]), float(res.iloc[0][f"{rid}_max"])
            if not np.isfinite(f) or not np.isfinite(lo):
                continue
            excess = max(lo - f, f - hi)
            worst = max(worst, excess)
            checked += 1
            bad += excess > tol
    ok = bad == 0
    print(f"exact-bounds replay check: {len(idx) - skipped} cell-timepoints "
          f"({skipped} infeasible), {checked} fluxes, {bad} outside, "
          f"worst excess {worst:.3e} -> {'PASS' if ok else 'FAIL'}")
    return ok


def summarise(df: pd.DataFrame, inverse: np.ndarray, fva: pd.DataFrame,
              setup: ModelSetup) -> pd.DataFrame:
    """Per-exchange degeneracy summary over all cell-timepoints."""
    rows = []
    for rid in setup.exchanges:
        col = f"{rid}_range"
        if col not in fva.columns:
            continue
        rng = fva[col].to_numpy()[inverse]
        flux = np.abs(df[rid].to_numpy(dtype=float))
        with np.errstate(divide="ignore", invalid="ignore"):
            rel = np.where(flux > 1e-12, rng / flux, np.nan)
        rows.append({
            "reaction": rid,
            "median_range": np.nanmedian(rng),
            "p95_range": np.nanpercentile(rng, 95) if np.any(~np.isnan(rng)) else np.nan,
            "max_range": np.nanmax(rng) if np.any(~np.isnan(rng)) else np.nan,
            "median_rel": np.nanmedian(rel),
            "frac_degenerate": float(np.nanmean(rng > 1e-9)),
        })
    out = pd.DataFrame(rows).sort_values("median_range", ascending=False)
    return out.reset_index(drop=True)


def attach_ranges(df: pd.DataFrame, inverse: np.ndarray, fva: pd.DataFrame,
                  setup: ModelSetup) -> pd.DataFrame:
    """Return df with the FVA range of each exchange joined onto every cell-timepoint."""
    out = df.copy()
    out["bound_set"] = inverse
    out["replay_status"] = fva["status"].to_numpy()[inverse]
    for rid in setup.exchanges:
        if f"{rid}_range" in fva.columns:
            out[f"{rid}_range"] = fva[f"{rid}_range"].to_numpy()[inverse]
            out[f"{rid}_fva_min"] = fva[f"{rid}_min"].to_numpy()[inverse]
            out[f"{rid}_fva_max"] = fva[f"{rid}_max"].to_numpy()[inverse]
    return out


# --------------------------------------------------------------------------- #

def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--output-dir", default="../output")
    ap.add_argument("--sbml", default="../config/MCF7_Zielinski_2017_core_trimmed.sbml")
    ap.add_argument("--config", default=None,
                    help="PhysiCell settings XML to read the LP setup from "
                         "(strongly preferred over the built-in defaults)")
    ap.add_argument("--every", type=int, default=1,
                    help="keep one snapshot in EVERY (default: all)")
    ap.add_argument("--decimals", type=int, default=5,
                    help="rounding applied before deduplicating bound sets")
    ap.add_argument("--fraction-of-optimum", type=float, default=1.0,
                    help="1.0 asks for the exact optimal face; see the module "
                         "docstring on solver choice")
    ap.add_argument("--solver", default="gurobi",
                    help="optlang solver interface (gurobi, glpk, ...)")
    ap.add_argument("--processes", type=int, default=None,
                    help="worker processes (default: cpu_count - 1)")
    ap.add_argument("--solver-timeout", type=float, default=20.0,
                    help="seconds before one LP is abandoned (0 = no limit)")
    ap.add_argument("--verify", action="store_true",
                    help="check the lean FVA against cobrapy before running")
    ap.add_argument("--save-prefix", default="fva_degeneracy",
                    help="write <prefix>_cells.csv and <prefix>_summary.csv")
    args = ap.parse_args()

    setup = (ModelSetup.from_config(args.config) if args.config else ModelSetup())
    df = read_run(args.output_dir, every=args.every)
    uniq, inverse = unique_bound_sets(df, setup.exchanges, decimals=args.decimals)

    if args.verify:
        if not verify_against_cobrapy(args.sbml, setup, uniq):
            raise SystemExit("lean FVA does not reproduce cobrapy; refusing to continue")
    fva = fva_parallel(args.sbml, setup, uniq, processes=args.processes,
                       solver_timeout=args.solver_timeout or None,
                       solver=args.solver,
                       fraction_of_optimum=args.fraction_of_optimum)

    check_recorded_flux_within_range(df, inverse, fva, setup,
                                     decimals=args.decimals)
    summary = summarise(df, inverse, fva, setup)
    print("\nper-exchange degeneracy (mmol/gDW/h), widest first:")
    print(summary.to_string(index=False))

    cells = attach_ranges(df, inverse, fva, setup)
    cells.to_csv(f"{args.save_prefix}_cells.csv", index=False)
    summary.to_csv(f"{args.save_prefix}_summary.csv", index=False)
    print(f"\nwrote {args.save_prefix}_cells.csv and {args.save_prefix}_summary.csv")


if __name__ == "__main__":
    main()
