#!/bin/bash
# Voxel-sharing non-negativity test. Needs a staged dfba-unit-test binary.
# usage (from the staged root): scripts/run_voxel_sharing_test.sh [./dfba-unit-test]
# Output goes to output_voxel_n1/ and output_voxel_n8/; existing results are
# moved aside, never deleted.
BIN=${1:-./dfba-unit-test}
PY=${PYTHON:-python3}
rc=0
for n in 1 8; do
    out=output_voxel_n$n
    if [ -d "$out" ] && [ -n "$(ls -A $out 2>/dev/null)" ]; then
        mv "$out" "${out}_$(date +%Y%m%d_%H%M%S)"
    fi
    mkdir -p "$out"
    "$BIN" config/Unit_test_voxel_sharing_n$n.xml > "$out.log" 2>&1 \
        || { echo "simulation failed for n=$n, see $out.log"; rc=2; continue; }
    $PY scripts/check_voxel_nonnegative.py "$out" || [ $rc -ne 0 ] || rc=1
done
exit $rc
