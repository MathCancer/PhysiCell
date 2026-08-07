# Counter-Based RNG Changes

This branch adds a deterministic RNG path based on Philox4x32-10 so the same simulation can produce the same random values even when the OpenMP thread count changes.

## Why this matters

**Reproducibility.** Same config, same seed, same result — regardless of how many OpenMP threads a run happens to use. Before this work, PhysiCell's thread-local RNG made results depend on thread count, and (for the four races described below) sometimes even on run-to-run scheduling luck at a *fixed* thread count. A figure or a comparison between two runs could differ for reasons that had nothing to do with the model — purely because of a hardware/thread-count choice. With counter-based RNG and the race fixes in place, the number of cores a run happens to use stops being a hidden variable in the result.

**Debugging.** Non-reproducibility isn't just a publishing inconvenience — it made a whole class of core-engine bugs undetectable, because there was never a reliable "this should be identical" baseline to test against. Every race fixed in this branch (unprotected shared-voxel secretion, an attachment-capacity check that's never re-validated, cross-cell mutation races in attack/ingest/fuse) was a genuine, silent correctness bug — present regardless of RNG mode, not something this branch introduced — that had gone unnoticed because nothing before could reliably say "these two runs should match and don't." That comparison is now a real, repeatable regression test ([beta/test_thread_repro.py](../beta/test_thread_repro.py)) instead of something you could only ever suspect informally.

**Potential bitwise restart.** The `(cell_id, step_key, purpose, sub_index)` keyed scheme (see "Minimal key scheme for restart" below) is deliberately restart-friendly: if `step_key` can be reconstructed exactly from restart-stable state rather than an in-memory-only counter, a resumed run could in principle produce bit-identical output to the same run had it never been interrupted. That's *not* implemented or tested here — restart itself is out of scope for this branch — but the keying scheme is designed so it doesn't foreclose that possibility, which a plain thread-local generator has no way to offer no matter how restart is later implemented.

## Credit

The counter-based generator itself, Philox4x32-10, and the counter-based RNG design it comes from, are not new here — they're from D. E. Shaw Research's [Random123](https://www.deshawresearch.com/resources_random123.html) library:

> John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw. "Parallel Random Numbers: As Easy as 1, 2, 3." In *Proceedings of 2011 International Conference for High Performance Computing, Networking, Storage and Analysis* (SC '11). ACM, 2011.

[`modules/philox.h`](../modules/philox.h) is vendored from Random123's `philox.h`, with D. E. Shaw Research's original BSD-style copyright notice and license preserved in the file. This branch's contribution is applying that generator inside PhysiCell — keying it by `(cell_id, time_step, purpose, sub_index)`, wiring a deterministic context around the per-cell update phases, and fixing the separate cross-cell mutation races (below) that RNG determinism alone doesn't address — not the counter-based RNG construction itself.

## What changed

The core RNG layer now has a keyed entry point in [core/PhysiCell_utilities.h](../core/PhysiCell_utilities.h) and [core/PhysiCell_utilities.cpp](../core/PhysiCell_utilities.cpp). The new `Random(cell_id, time_step, purpose, sub_index)` API generates a value directly from the key and counter inputs, and the existing `UniformRandom()`, `UniformInt()`, `NormalRandom()`, and `LogNormalRandom()` functions can read from a thread-local deterministic context when that context is active.

The main simulation loop sets that context around the major per-cell update phases in [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp). That means the existing simulation code can keep calling the usual random helpers, but those helpers will now draw from a reproducible per-cell, per-step sequence instead of a thread-order-dependent stream.

I also added a reusable end-to-end thread reproducibility checker in [beta/test_thread_repro.py](../beta/test_thread_repro.py). The checker runs the same project twice with different thread counts and compares the final XML output after normalizing volatile metadata.

## Why this did not require changing every `Random()` call

The key idea is that the call sites did not need to change one by one because the RNG behavior was redirected at the shared utility layer.

Before this change, every stochastic helper depended on the current thread-local generator state, so the result depended on execution order. After the change, the simulation sets a deterministic context once per cell and per update phase, and the helper functions consult that context automatically.

In practice:

- Code such as `UniformRandom()` still works unchanged.
- When a deterministic context is active, `UniformRandom()` uses the Philox-backed keyed path instead of the legacy stream.
- The same per-cell/per-step/key inputs always produce the same bit pattern, regardless of how OpenMP schedules the work.

So the important change was not rewriting every random call site. The important change was making the shared RNG helpers context-aware and ensuring the simulation engine installs the right context before calling code that already uses those helpers.

## When you still would change a call site

You would only need to change an individual call site if it should use a different key than the one implied by the current simulation phase, or if it runs outside the engine-managed per-cell context and still needs deterministic behavior.

That is why the initialization code in sample projects can still use the legacy `UniformRandom()` calls without issue for this test: the test focuses on the simulation loop, where the context is already installed.

## Minimal key scheme for restart

If the long-term goal is bitwise restart from a saved snapshot, the minimal keyed identity should still represent four different roles, even if not all four are stored explicitly as separate variables.

- `cell_id`: which cell owns the draw.
- `step_key`: which saved simulation step or resumed update step owns the draw.
- `purpose`: which update phase or stochastic subsystem owns the draw.
- `sub_index`: which draw number within that cell/step/purpose context this is.

That means the conceptual minimum is:

`Random(cell_id, step_key, purpose, sub_index)`

For restart, `step_key` does not have to be an unsaved in-memory counter. It can be reconstructed from restart-stable state, as long as the reconstruction is exact. For example, it can be derived from:

- saved simulation time,
- saved diffusion / mechanics / phenotype scheduling state,
- and the current update phase.

The important requirement is not the name of the field. The requirement is that every random draw after restart resolves to the same unique identity it would have had in the uninterrupted run.

## Why each index matters

### `cell_id`

This separates one cell from another.

Example:

- cell 10 draws a motility angle,
- cell 11 draws a motility angle,
- both happen in the same step and same purpose.

Without `cell_id`, both cells would ask for the same keyed random value.

### `step_key`

This separates one update step from the next.

Example:

- cell 10 updates motility at step 100,
- cell 10 updates motility again at step 101.

Without `step_key`, the cell would receive the same random value every time that code path runs.

### `purpose`

This separates different stochastic uses within the same cell and same step.

Example:

- cell 10 uses randomness during phenotype update,
- cell 10 also uses randomness during velocity update,
- both happen in the same global simulation step.

Without `purpose`, those two unrelated code paths would be drawing from the same keyed location and could collide.

### `sub_index`

This separates repeated random draws within the same cell, same step, and same purpose.

Example:

- cell 10 calls `UniformRandom()` twice during motility update,
- first draw selects whether to reorient,
- second draw selects an angle.

Without `sub_index`, both calls would map to the same keyed random value.

### Concrete multi-draw examples already in PhysiCell

This is not just a theoretical issue. Several existing helpers already consume more than one uniform draw for one higher-level stochastic operation.

- `NormalRandom()` uses two uniforms internally through the Box-Muller transform. See [core/PhysiCell_utilities.cpp](../core/PhysiCell_utilities.cpp).
- `LogNormalRandom()` also uses that same two-uniform path because it is built on top of `NormalRandom()`. See [core/PhysiCell_utilities.cpp](../core/PhysiCell_utilities.cpp).
- `UniformOnUnitSphere()` uses two uniforms, one for `z` and one for `theta`. See [core/PhysiCell_utilities.cpp](../core/PhysiCell_utilities.cpp).
- `LegacyRandomOnUnitSphere()` uses multiple uniforms as well. See [core/PhysiCell_utilities.cpp](../core/PhysiCell_utilities.cpp).
- `UniformInUnitDisc()` uses two uniforms. See [core/PhysiCell_rules.cpp](../core/PhysiCell_rules.cpp).
- `UniformInUnitSphere()` uses three uniforms. See [core/PhysiCell_rules.cpp](../core/PhysiCell_rules.cpp).
- `UniformInAnnulus()` uses two uniforms. See [core/PhysiCell_rules.cpp](../core/PhysiCell_rules.cpp).
- `UniformInShell()` uses three uniforms. See [core/PhysiCell_rules.cpp](../core/PhysiCell_rules.cpp).

There are also mixed code paths where one logical operation first makes a decision draw and then consumes additional random values inside the same higher-level update. For example, motility update can first draw whether the cell reorients and then, if it does, draw a random direction on the circle or sphere. See [core/PhysiCell_cell.cpp](../core/PhysiCell_cell.cpp).

This is why `sub_index` is convenient even when `purpose` is automatic. A single semantic purpose such as `motility update` can safely contain several actual draws without inventing a separate micro-purpose for every internal step.

## Restart implication

If restart always begins from a saved snapshot, you may be able to derive `step_key` from the restored snapshot state rather than saving a separate hidden counter. But you still need the full four-role scheme:

- identity of the cell,
- identity of the resumed step,
- identity of the stochastic purpose,
- identity of the draw number within that purpose.

If any one of those roles is missing, different random events can collapse onto the same keyed draw.

## The existing precedent: ordering division and death

Division and death are decided inside the parallel phenotype loop, but a cell can't safely divide or remove itself from `(*all_cells)` while that loop is still running on other threads. So instead of acting immediately, a cell calls `flag_cell_for_division()` / `flag_cell_for_removal()`, which push it onto `cells_ready_to_divide` / `cells_ready_to_die` under `#pragma omp critical`. Only after the parallel loop finishes does [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp) actually call `divide()` / `die()` on those lists — serially, and, when `PhysiCell_settings.use_counter_based_rng` is enabled, only after sorting each list by cell ID first:

```cpp
if ( PhysiCell_settings.use_counter_based_rng )
    std::sort( cells_ready_to_divide.begin(), cells_ready_to_divide.end(),
        []( const Cell* lhs, const Cell* rhs ) { return lhs->ID < rhs->ID; } );
for( int i=0; i < cells_ready_to_divide.size(); i++ )
{
    activate_random_context( cells_ready_to_divide[i]->ID, RANDOM_PURPOSE_DIVISION );
    cells_ready_to_divide[i]->divide();
    clear_random_context();
}
```

The sort matters for two reasons: new daughter cells get their IDs assigned in whatever order `divide()` is called, so without it, which physical cell gets which ID would depend on thread scheduling; and `die()` removes cells from `(*all_cells)` by swapping in the last element, so processing removals in a different order leaves `(*all_cells)` in a different arrangement, which then affects the deterministic per-cell RNG keys and any later code that iterates `(*all_cells)` in order.

This "collect under a critical section while parallel, then apply serially in a fixed order" shape is exactly what Fixes 1 and 2 below extend to cell-cell interactions and spring attachments, which had no such ordering at all.

## Beyond RNG determinism: cross-cell mutation races

Making `Random()` deterministic per `(cell_id, step_key, purpose, sub_index)` only guarantees that a cell's *own* draws are reproducible. It does nothing to protect a cell from having its state changed by a *different* cell's thread while both are being processed in the same `#pragma omp parallel for`. That is a separate class of bug, and thread-reproducibility testing on real projects (worm, virus_macrophage, and a rules-driven virus/macrophage model) turned up several of them in the core engine, independent of RNG.

### The general pattern

Several core functions read another cell's current state, decide what to do based on it, and only then commit a change, e.g.:

```cpp
if( pTarget->something < threshold )       // read, unprotected
{
    pTarget->something = new_value;        // write, protected by #pragma omp critical
}
```

The `critical` block (where present) stops two threads from writing at the exact same instant, so it prevents outright memory corruption. It does *not* stop the read from seeing a stale value: two different cells' threads can both perform the read before either one's write lands, both conclude they should act, and then both act, in whichever order the OpenMP scheduler happens to pick. In a 1-thread run this can never happen, because the loop is always fully sequential, so the "reference" run and a multithreaded run can permanently disagree from that point on.

`#pragma omp critical` alone fixes memory-safety. It does not fix determinism, because it says nothing about *which* thread's read/write pair goes first. `#pragma omp ordered` fixes both: it forces the loop's iterations to commit their marked region in the same relative order a 1-thread run would use, no matter how many threads are active or how they're scheduled. The fixes below use `ordered` wherever the operation is not commutative (an addition, a "first one wins" decision), and plain `critical` where it is (a `max` update, where the result doesn't depend on which thread got there first).

### Fix 1: `standard_cell_cell_interactions` — attack, phagocytosis, fusion

[`core/PhysiCell_standard_models.cpp`](../core/PhysiCell_standard_models.cpp) calls `pCell->ingest_cell(pTarget)`, `pCell->attack_cell(pTarget, dt)`, and `pCell->fuse_cell(pTarget)` directly from a per-cell loop. All three mutate the *target* cell's live state (volume, damage, death flag, position) and are wrapped in `#pragma omp critical` internally — but the guard that decides whether to act (e.g. `pCell_to_eat->phenotype.volume.total < 1e-15`) runs *before* that critical section. Two different cells can both decide to consume, attack, or fuse with the same target in the same step; which one's action actually lands first, and therefore what the target's final state is, depends on thread scheduling.

Fixed by wrapping the call to `standard_cell_cell_interactions()` in [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp) in `#pragma omp ordered` — via `run_cell_cell_interactions_phase_ordered()`, taken only when `use_counter_based_rng` is enabled; see "Why these fixes are gated behind counter-based RNG" below. This also covers the function's early-exit check on `phenotype.death.dead`, which was itself an unsynchronized read of a flag another cell's thread could be writing at the same time.

### Fix 2: `dynamic_spring_attachments` — attachment-capacity race

[`core/PhysiCell_standard_models.cpp`](../core/PhysiCell_standard_models.cpp) checks `pTest->state.spring_attachments.size() < pTest->phenotype.mechanics.maximum_number_of_attachments` to decide whether a neighbor still has room for a new attachment, then calls `attach_cells_as_spring()`. The capacity check is never re-validated once the lock inside `attach_cells_as_spring()` is actually held. Two different cells can both see "room for one more" on the same neighbor at once and both attach, silently pushing that cell past its configured maximum, in a way that depends on scheduling.

Fixed the same way: the call to `dynamic_spring_attachments()` in [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp) is now inside `#pragma omp ordered` — via `run_spring_attachment_phase_ordered()` — when `use_counter_based_rng` is enabled.

### Fix 3: `max_cell_interactive_distance_in_voxel` — stale/racy neighbor-search bound

Each mechanics voxel keeps a running maximum of `radius * relative_maximum_adhesion_distance` over the cells that have been recorded there, used to decide whether a neighbor search needs to look inside that voxel at all. It's updated with an unprotected read-compare-write (`if (current < new) current = new;`) in `Cell::convert_to_cell_definition()` (fires on a rule-triggered type transformation) and, matching upstream PR [#409](https://github.com/MathCancer/PhysiCell/pull/409), in `Cell::update_voxel_in_container()` (fires on ordinary movement between voxels — without this, the bound goes stale as cells migrate, and a voxel's neighbor search can wrongly skip a cell that has since moved in). Both live in [core/PhysiCell_cell.cpp](../core/PhysiCell_cell.cpp).

Unlike Fixes 1 and 2, this doesn't need `ordered`: a `max` over a fixed set of candidate values is the same regardless of the order you compare them in, so a plain `#pragma omp critical` around the read-compare-write is enough to make it both race-free and deterministic.

### Fix 4: cell secretion/uptake into shared diffusion voxels

PhysiCell's real per-cell secretion path is `Cell_Container::update_all_cells()`'s first loop → `Secretion::advance()` → `Basic_Agent::simulate_secretion_and_uptake()` (in [BioFVM/BioFVM_basic_agent.cpp](../BioFVM/BioFVM_basic_agent.cpp)), which does:

```cpp
(*pS)(current_voxel_index) += cell_source_sink_solver_temp1;
(*pS)(current_voxel_index) /= cell_source_sink_solver_temp2;
(*pS)(current_voxel_index) += cell_source_sink_solver_temp_export2;
```

directly on the microenvironment's shared voxel density vector, with no lock at all. Diffusion voxels are typically larger than a single cell, so it's normal for several cells to share one; when two cells in the same voxel are processed by different threads at the same time, this isn't just reordering, it's a genuine lost update — one thread's contribution can be silently overwritten instead of added.

Fixed by wrapping the call to `phenotype.secretion.advance(...)` in [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp) in `#pragma omp ordered` — via `run_secretion_phase_ordered()` — when `use_counter_based_rng` is enabled.

### Why these fixes are gated behind counter-based RNG

`#pragma omp ordered` isn't free: it forces the loop's marked region to execute in strict sequence across *every* iteration, not just the ones that actually conflict, which caps how much of that loop can ever be parallelized (this is "DOACROSS" parallelism in the literature, as opposed to the fully-independent "DOALL" case a plain `parallel for` handles). Benchmarks on comparable loop-carried-dependency patterns show real, sometimes large, slowdowns from coarse-grained `ordered` use, so applying it unconditionally to every PhysiCell run — including the large fraction of existing projects that have never asked for cross-thread-count reproducibility — was not an acceptable default.

So Fixes 1, 2, and 4 are gated at their call site in `Cell_Container::update_all_cells()`, not inside a shared function: each is an `if( use_counter_based_rng ) { ...call a small ordered-only function... } else { ...original inline loop... }` written directly where the phase runs. The `if` branch calls one of three small, ordered-only functions added to the anonymous namespace in [core/PhysiCell_cell_container.cpp](../core/PhysiCell_cell_container.cpp): `run_secretion_phase_ordered()`, `run_spring_attachment_phase_ordered()`, and `run_cell_cell_interactions_phase_ordered()`. `use_counter_based_rng` is checked once per phase per step, not once per cell.

The `else` branch of each does *not* call `activate_random_context()`/`clear_random_context()`, even though the equivalent (non-split) loops elsewhere in `update_all_cells()` do. That's not an inconsistency: those other loops keep the guarded call so one loop body works for both RNG modes, avoiding a needless second copy. But inside an `else` block that only runs when `use_counter_based_rng` is already `false`, that same guarded call is provably dead code — the internal `if( use_counter_based_rng )` check inside it can never be true there, so calling it can never do anything. Once the racy phases already need two separate loop bodies (for the ordered-vs-not structural difference), there's no cost to leaving those calls out of the `else` branch specifically, and doing so makes that branch true original behavior — no context bookkeeping at all — rather than "calls that happen to no-op."

This keeps the diff against the pre-fix code additive with one small subtraction: the three new ordered functions, each racy loop wrapped in an `if`/`else`, and the now-pointless `activate_random_context()`/`clear_random_context()` calls dropped from the three `else` branches. Nothing else in `update_all_cells()` — intracellular update, phenotype update, division/death, `evaluate_interactions`, `custom_cell_rule`, `update_velocity`, spring-force application, position update — was touched.

This is a deliberate, explicit trade-off, not an oversight: **with `counter_based_rng` disabled (the default), the three races described in Fixes 1, 2, and 4 are still present.** They are not reproducibility-only concerns — the secretion race in particular is a genuine lost-update on shared substrate mass, independent of RNG mode entirely, and predates the counter-based RNG feature. Turning on `counter_based_rng` is currently the only way to get both deterministic RNG *and* these race-free interaction/attachment/secretion phases. Fix 3 (`max_cell_interactive_distance_in_voxel`) is not gated — a `critical`-protected `max` update is cheap enough that there was no reason to make it conditional.

## How to enable counter-based RNG

Counter-based RNG is opt-in and **off by default**. If `<rng_mode>` is not set at all, the simulation uses the legacy thread-local generator (`PhysiCell_settings.use_counter_based_rng = false` in [modules/PhysiCell_settings.h](../modules/PhysiCell_settings.h)), which is not guaranteed to be reproducible across different thread counts.

To enable it, add (or set) an `<rng_mode>` element inside `<options>` in the config XML:

```xml
<options>
    ...
    <rng_mode>counter_based</rng_mode>
</options>
```

Accepted values, parsed in [modules/PhysiCell_settings.cpp](../modules/PhysiCell_settings.cpp):

- `counter_based`, `counter`, or `philox` → enables the deterministic Philox-based path (`use_counter_based_rng = true`).
- `legacy`, `thread_local`, or `mt19937` → explicitly selects the old, non-deterministic thread-local generator (`use_counter_based_rng = false`).
- Omitted or empty → stays at the default, `false`.

An unrecognized value prints an error and leaves the setting unchanged.

## Testing a new model for thread reproducibility

[beta/test_thread_repro.py](../beta/test_thread_repro.py) runs an already-built executable twice, once per thread count, and diffs the output. It does not build the project for you — build first (`make`), then run:

```bash
python beta/test_thread_repro.py <executable> <config_file> <max_time> <threads_a> <threads_b> <rng_mode> <work_dir>
```

Example, testing the currently-loaded project with 1 vs. 4 threads for 1440 simulated minutes:

```bash
python beta/test_thread_repro.py project config/PhysiCell_settings.xml 1440 1 4 counter_based local_runs
```

- `executable`: path to the built binary (relative to the repo root), e.g. `project`.
- `config_file`: the XML config to copy and use for both runs.
- `max_time`: simulated end time, in the config's time units.
- `threads_a`, `threads_b`: the two thread counts to compare (must differ).
- `rng_mode`: written into each run's copy of `<options><rng_mode>` before running (omit, or pass `off`/`none`/`default`, to leave the config's existing setting untouched).
- `work_dir`: where the two runs' output is written (each invocation creates a timestamped subfolder inside it).

The script runs both thread counts, then diffs the final XML/SVG snapshots with [beta/test_diff_svg.py](../beta/test_diff_svg.py) and reports the first mismatch, if any.

Because these races can be intermittent — several of the fixes above only failed some fraction of the time — a single passing run is not strong evidence on its own. Repeat the same command several times (5–10x) before trusting a "pass," especially for a new or modified model.

## Validation

The new system tests pass with 1 thread and 4 threads for the template project.

The four cross-cell-mutation fixes above were found and validated by testing the template project with multiple cell behaviors activated (phagocytosis, attack, transform, and others) using `beta/test_thread_repro.py` across single and multiple threads. Repeated trials failed consistently before Fix 4 (the secretion race) was in place; after all four fixes were applied, repeated trials passed bit-exact.

After gating Fixes 1, 2, and 4 behind `use_counter_based_rng`, both branches of each rewritten phase were re-checked: with `rng_mode=counter_based`, repeated trials still pass bit-exact; with `rng_mode=legacy`, both thread counts still run to completion without errors (confirming the unordered branch itself is correct), and are not expected to, and do not, match each other — legacy mode was never meant to guarantee cross-thread-count reproducibility, gated fixes or not.

Known open item: a project-level (not core-engine) bug remains in the worm sample's custom `contact_function` ([custom_modules/custom.cpp](../custom_modules/custom.cpp) when the worm project is loaded), which writes into an attached cell's `custom_data` from within a parallel loop without going through the engine's RNG/ordering machinery. That is a project-code issue rather than a PhysiCell core issue and is tracked separately.
