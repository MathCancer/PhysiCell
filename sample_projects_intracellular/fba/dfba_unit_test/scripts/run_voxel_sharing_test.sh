#!/bin/bash
# Voxel-sharing test: a voxel's substrate must not go negative when several cells
# draw on it, and cells that cannot take a substrate must not dilute the others.
# Needs a staged dfba-unit-test binary.
# usage (from the staged root): scripts/run_voxel_sharing_test.sh [./dfba-unit-test]
# Output goes to output_voxel_<case>/; existing results are moved aside, never deleted.
BIN=${1:-./dfba-unit-test}
PY=${PYTHON:-python3}
rc=0
for c in n1 n4 n8 mixed; do
    out=output_voxel_$c
    if [ -d "$out" ] && [ -n "$(ls -A $out 2>/dev/null)" ]; then
        mv "$out" "${out}_$(date +%Y%m%d_%H%M%S)"
    fi
    mkdir -p "$out"
    "$BIN" config/Unit_test_voxel_sharing_$c.xml > "$out.log" 2>&1 \
        || { echo "simulation failed for $c, see $out.log"; rc=2; continue; }
    $PY scripts/check_voxel_nonnegative.py "$out" || { [ $rc -ne 0 ] || rc=1; }
done
$PY scripts/check_voxel_draw_equal.py output_voxel_n4 output_voxel_mixed || { [ $rc -ne 0 ] || rc=1; }
exit $rc
