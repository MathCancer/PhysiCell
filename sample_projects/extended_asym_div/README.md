# Extended asymmetric division sample

A single `type0` cell divides in a 2-D domain. Which pair of daughter types each division produces is
set per unordered pair under `<extended_asymmetric_division>` in the config, and driven over time by
the rules file. Only `type0` divides, so every division is one of three outcomes: it stays `type0`, or
it produces `type1` and `type2`, or `type3` and `type4`.

## Two configs, two ways of writing the same model

```bash
./project ./config/PhysiCell_settings.xml          # -> output/
./project ./config/PhysiCell_settings_weights.xml  # -> output_weights/
```

`PhysiCell_settings_weights.xml` is `PhysiCell_settings.xml` with three changes: the output folder,
`<asymmetric_division_mode>weights</asymmetric_division_mode>` in the `<options>` block, and
[cell_rules_weights.csv](config/cell_rules_weights.csv) in place of
[cell_rules.csv](config/cell_rules.csv). The rules differ only in their saturation values:

| | `(type0, type0)` | `(type1, type2)`, then `(type3, type4)` | what the code does with them |
|---|---|---|---|
| `probabilities` | 1.0 | saturates at **0.5** | total is 1.5, so the 0.5 excess comes **entirely out of** `(type0, type0)`, leaving 0.5 and 0.5 |
| `weights` | 1.0 | saturates at **1.0** | total is 2.0, and **every** entry is scaled by 1/2.0, leaving 0.5 and 0.5 |

Both land on the same 50/50 split, and the two runs are the same simulation — same cells, same
positions, same types, at every snapshot. What differs is only the recorded values: `(type0, type0)`
reads 0.5 in one and 1.0 in the other, because weights are a re-parameterization, not a different
model.

That table is also the thing to understand before switching an existing model over. The two modes
renormalize differently, and the difference is not subtle: **probabilities take the whole overshoot
out of the symmetric entry, while weights scale every entry proportionally.** Feed this sample's
probabilities config to weights mode unchanged and `(type0, type0)` gets 1.0/1.5 = 2/3 of divisions
instead of 1/2. Same numbers, different model. That is why setting
`<asymmetric_division_probability_tolerance>` together with weights is rejected at startup rather than
one of the two quietly winning.

## Writing a model in weights mode

Weights exist so that rules can move each daughter-type pair independently. Under probabilities every
rule has to cooperate to keep the total at or below 1, which no single rule can check, because a
Hypothesis Grammar rule cannot see what the other pairs currently evaluate to. Under weights the scale
does not matter, so each rule can be written on its own.

The pitfall worth knowing: **under weights there is no implicit symmetric-division remainder.** In
probabilities mode a cell type listing only `(type0, type1): 0.3` divides symmetrically the other 70%
of the time. In weights mode that same cell divides into `type0` and `type1` every single time, because
that is the only pair carrying any weight. Self-renewal needs its own `(type0, type0)` weight, which is
exactly what this sample gives it.

The one exception is an all-zero total, which has no normalized distribution: that is defined as
symmetric division, so a model whose weights start at zero and ramp up through rules divides
symmetrically at first rather than failing.

## Building

```bash
make extended-asym-div-sample && make
```

A note on reproducibility if you compare runs: with `<omp_num_threads>` above 1 this model is not
bit-reproducible from the seed alone, and because the `type0` population is close to a critical
branching process the run-to-run spread in final cell count is large. Set `<omp_num_threads>1</omp_num_threads>`
in both configs to compare them directly.
