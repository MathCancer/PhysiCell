# Counter-Based RNG Changes

This branch adds a deterministic RNG path based on Philox4x32-10 so the same simulation can produce the same random values even when the OpenMP thread count changes.

## What changed

The core RNG layer now has a keyed entry point in [core/PhysiCell_utilities.h](core/PhysiCell_utilities.h) and [core/PhysiCell_utilities.cpp](core/PhysiCell_utilities.cpp). The new `Random(cell_id, time_step, purpose, sub_index)` API generates a value directly from the key and counter inputs, and the existing `UniformRandom()`, `UniformInt()`, `NormalRandom()`, and `LogNormalRandom()` functions can read from a thread-local deterministic context when that context is active.

The main simulation loop sets that context around the major per-cell update phases in [core/PhysiCell_cell_container.cpp](core/PhysiCell_cell_container.cpp). That means the existing simulation code can keep calling the usual random helpers, but those helpers will now draw from a reproducible per-cell, per-step sequence instead of a thread-order-dependent stream.

I also added a reusable end-to-end thread reproducibility checker in [beta/test_thread_repro.py](beta/test_thread_repro.py). The GitHub Actions workflow uses it for the template and intracellular template jobs in [tests.yml](.github/workflows/tests.yml).

The checker runs the same project twice with different thread counts and compares the final XML output after normalizing volatile metadata.

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

- `NormalRandom()` uses two uniforms internally through the Box-Muller transform. See [core/PhysiCell_utilities.cpp](core/PhysiCell_utilities.cpp).
- `LogNormalRandom()` also uses that same two-uniform path because it is built on top of `NormalRandom()`. See [core/PhysiCell_utilities.cpp](core/PhysiCell_utilities.cpp).
- `UniformOnUnitSphere()` uses two uniforms, one for `z` and one for `theta`. See [core/PhysiCell_utilities.cpp](core/PhysiCell_utilities.cpp).
- `LegacyRandomOnUnitSphere()` uses multiple uniforms as well. See [core/PhysiCell_utilities.cpp](core/PhysiCell_utilities.cpp).
- `UniformInUnitDisc()` uses two uniforms. See [core/PhysiCell_rules.cpp](core/PhysiCell_rules.cpp).
- `UniformInUnitSphere()` uses three uniforms. See [core/PhysiCell_rules.cpp](core/PhysiCell_rules.cpp).
- `UniformInAnnulus()` uses two uniforms. See [core/PhysiCell_rules.cpp](core/PhysiCell_rules.cpp).
- `UniformInShell()` uses three uniforms. See [core/PhysiCell_rules.cpp](core/PhysiCell_rules.cpp).

There are also mixed code paths where one logical operation first makes a decision draw and then consumes additional random values inside the same higher-level update. For example, motility update can first draw whether the cell reorients and then, if it does, draw a random direction on the circle or sphere. See [core/PhysiCell_cell.cpp](core/PhysiCell_cell.cpp).

This is why `sub_index` is convenient even when `purpose` is automatic. A single semantic purpose such as `motility update` can safely contain several actual draws without inventing a separate micro-purpose for every internal step.

## Why `cell_id` and `time_step` alone are not enough

They are not enough if more than one random draw can happen for the same cell in the same step, which is normal in PhysiCell.

There are two independent problems.

### Problem 1: multiple draws within one context

Suppose cell 10 is in phenotype step 25 and a function does this:

```cpp
if( UniformRandom() < p )
{
	double theta = UniformRandom();
}
```

If the key were only `(cell_id, time_step)`, both calls would ask for the same value. That is wrong.

`sub_index` fixes this by making the first call use sub-index 0 and the second call use sub-index 1.

### Problem 2: different purposes in the same step

Suppose in the same saved step you do both of these for the same cell:

- phenotype transition draw,
- motility direction draw.

If the key were only `(cell_id, time_step)`, those unrelated events would collide too.

`purpose` fixes this by giving each update phase its own namespace.

## About "the internal index will always start from 0 after restart"

That statement is only partly true.

It is true that `sub_index` should normally restart at 0 when entering a new deterministic context.

It is not true that this makes `purpose` unnecessary.

Why:

- in one resumed step, cell 10 can enter several different stochastic contexts,
- each of those contexts may start its own `sub_index` at 0,
- but they must still remain distinct from each other.

So after restart you can absolutely do this:

- phenotype context for cell 10, step 25: sub-index starts at 0,
- velocity context for cell 10, step 25: sub-index also starts at 0.

That is correct only if `purpose` is also part of the key. Otherwise both contexts collide.

So the right rule is:

- `sub_index` resets to 0 per context,
- not per whole simulation step.

## Restart implication

If restart always begins from a saved snapshot, you may be able to derive `step_key` from the restored snapshot state rather than saving a separate hidden counter. But you still need the full four-role scheme:

- identity of the cell,
- identity of the resumed step,
- identity of the stochastic purpose,
- identity of the draw number within that purpose.

If any one of those roles is missing, different random events can collapse onto the same keyed draw.

## The existing precedent: ordering division and death

The counter-based RNG work already had to solve a version of this problem for cell division and death, and the fixes below follow the same pattern it established.

Division and death are decided inside the parallel phenotype loop, but a cell can't safely divide or remove itself from `(*all_cells)` while that loop is still running on other threads. So instead of acting immediately, a cell calls `flag_cell_for_division()` / `flag_cell_for_removal()`, which push it onto `cells_ready_to_divide` / `cells_ready_to_die` under `#pragma omp critical`. Only after the parallel loop finishes does [core/PhysiCell_cell_container.cpp](core/PhysiCell_cell_container.cpp) actually call `divide()` / `die()` on those lists — serially, and, when `PhysiCell_settings.use_counter_based_rng` is enabled, only after sorting each list by cell ID first:

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

[`core/PhysiCell_standard_models.cpp`](core/PhysiCell_standard_models.cpp) calls `pCell->ingest_cell(pTarget)`, `pCell->attack_cell(pTarget, dt)`, and `pCell->fuse_cell(pTarget)` directly from a per-cell loop. All three mutate the *target* cell's live state (volume, damage, death flag, position) and are wrapped in `#pragma omp critical` internally — but the guard that decides whether to act (e.g. `pCell_to_eat->phenotype.volume.total < 1e-15`) runs *before* that critical section. Two different cells can both decide to consume, attack, or fuse with the same target in the same step; which one's action actually lands first, and therefore what the target's final state is, depends on thread scheduling.

Fixed by wrapping the call to `standard_cell_cell_interactions()` in [core/PhysiCell_cell_container.cpp](core/PhysiCell_cell_container.cpp) in `#pragma omp ordered` (loop changed to `schedule(static) ordered`). This also covers the function's early-exit check on `phenotype.death.dead`, which was itself an unsynchronized read of a flag another cell's thread could be writing at the same time.

### Fix 2: `dynamic_spring_attachments` — attachment-capacity race

[`core/PhysiCell_standard_models.cpp`](core/PhysiCell_standard_models.cpp) checks `pTest->state.spring_attachments.size() < pTest->phenotype.mechanics.maximum_number_of_attachments` to decide whether a neighbor still has room for a new attachment, then calls `attach_cells_as_spring()`. The capacity check is never re-validated once the lock inside `attach_cells_as_spring()` is actually held. Two different cells can both see "room for one more" on the same neighbor at once and both attach, silently pushing that cell past its configured maximum, in a way that depends on scheduling.

Fixed the same way: the call to `dynamic_spring_attachments()` in [core/PhysiCell_cell_container.cpp](core/PhysiCell_cell_container.cpp) is now inside `#pragma omp ordered`.

### Fix 3: `max_cell_interactive_distance_in_voxel` — stale/racy neighbor-search bound

Each mechanics voxel keeps a running maximum of `radius * relative_maximum_adhesion_distance` over the cells that have been recorded there, used to decide whether a neighbor search needs to look inside that voxel at all. It's updated with an unprotected read-compare-write (`if (current < new) current = new;`) in `Cell::convert_to_cell_definition()` (fires on a rule-triggered type transformation) and, matching upstream PR [#409](https://github.com/MathCancer/PhysiCell/pull/409), in `Cell::update_voxel_in_container()` (fires on ordinary movement between voxels — without this, the bound goes stale as cells migrate, and a voxel's neighbor search can wrongly skip a cell that has since moved in). Both live in [core/PhysiCell_cell.cpp](core/PhysiCell_cell.cpp).

Unlike Fixes 1 and 2, this doesn't need `ordered`: a `max` over a fixed set of candidate values is the same regardless of the order you compare them in, so a plain `#pragma omp critical` around the read-compare-write is enough to make it both race-free and deterministic.

### Fix 4: cell secretion/uptake into shared diffusion voxels

This was the one that actually explained the residual, hard-to-pin-down mismatches in a rules-driven multi-cell-type model, after Fixes 1–3 were already in place and confirmed to not be the cause (isolated by testing with those mechanisms enabled, disabled, and recombined).

PhysiCell's real per-cell secretion path is `Cell_Container::update_all_cells()`'s first loop → `Secretion::advance()` → `Basic_Agent::simulate_secretion_and_uptake()` (in [BioFVM/BioFVM_basic_agent.cpp](BioFVM/BioFVM_basic_agent.cpp)), which does:

```cpp
(*pS)(current_voxel_index) += cell_source_sink_solver_temp1;
(*pS)(current_voxel_index) /= cell_source_sink_solver_temp2;
(*pS)(current_voxel_index) += cell_source_sink_solver_temp_export2;
```

directly on the microenvironment's shared voxel density vector, with no lock at all. Diffusion voxels are typically larger than a single cell, so it's normal for several cells to share one; when two cells in the same voxel are processed by different threads at the same time, this isn't just reordering, it's a genuine lost update — one thread's contribution can be silently overwritten instead of added.

Fixed by wrapping the call to `phenotype.secretion.advance(...)` in [core/PhysiCell_cell_container.cpp](core/PhysiCell_cell_container.cpp) in `#pragma omp ordered`.

## How to enable counter-based RNG

Counter-based RNG is opt-in and **off by default**. If `<rng_mode>` is not set at all, the simulation uses the legacy thread-local generator (`PhysiCell_settings.use_counter_based_rng = false` in [modules/PhysiCell_settings.h](modules/PhysiCell_settings.h)), which is not guaranteed to be reproducible across different thread counts.

To enable it, add (or set) an `<rng_mode>` element inside `<options>` in the config XML:

```xml
<options>
    ...
    <rng_mode>counter_based</rng_mode>
</options>
```

Accepted values, parsed in [modules/PhysiCell_settings.cpp](modules/PhysiCell_settings.cpp):

- `counter_based`, `counter`, or `philox` → enables the deterministic Philox-based path (`use_counter_based_rng = true`).
- `legacy`, `thread_local`, or `mt19937` → explicitly selects the old, non-deterministic thread-local generator (`use_counter_based_rng = false`).
- Omitted or empty → stays at the default, `false`.

An unrecognized value prints an error and leaves the setting unchanged.

## Testing a new model for thread reproducibility

[beta/test_thread_repro.py](beta/test_thread_repro.py) runs an already-built executable twice, once per thread count, and diffs the output. It does not build the project for you — build first (`make`), then run:

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

The script runs both thread counts, then diffs the final XML/SVG snapshots with [beta/test_diff_svg.py](beta/test_diff_svg.py) and reports the first mismatch, if any.

Because these races can be intermittent — several of the fixes above only failed some fraction of the time — a single passing run is not strong evidence on its own. Repeat the same command several times (5–10x) before trusting a "pass," especially for a new or modified model.

## Validation

The new system tests pass with 1 thread and 4 threads for the template project.

The four cross-cell-mutation fixes above were found and validated by testing the template project with multiple cell behaviors activated (phagocytosis, attack, transform, and others) using `beta/test_thread_repro.py` across single and multiple threads. Repeated trials failed consistently before Fix 4 (the secretion race) was in place; after all four fixes were applied, repeated trials passed bit-exact.

Known open item: a project-level (not core-engine) bug remains in the worm sample's custom `contact_function` ([custom_modules/custom.cpp](custom_modules/custom.cpp) when the worm project is loaded), which writes into an attached cell's `custom_data` from within a parallel loop without going through the engine's RNG/ordering machinery. That is a project-code issue rather than a PhysiCell core issue and is tracked separately.
