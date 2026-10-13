#!/usr/bin/env python3
"""Regression test: the uptake cap must not let a voxel's substrate go negative.

dFBAIntracellular::update_exchange_bounds caps each cell's uptake at the content
of its voxel, but applies the cap to each cell INDEPENDENTLY. With n cells in one
voxel the summed sink can be n times the content, and BioFVM applies the
net_export_rate as an explicit term with no positivity protection, so the
extracellular concentration goes negative.

Run on the output of config/Unit_test_voxel_sharing_n{1,8}.xml. Glucose has no
transport in those configs, so every voxel is an isolated pool and the check
exposes the cap arithmetic alone. n=1 is the control: one cell's request fits in
the voxel and the cap is irrelevant to it. n=8 must also stay non-negative.

usage: check_voxel_nonnegative.py <output_dir> [tolerance_mM]

Tolerance (default 1e-4 mM, ~6e-4 of the 0.165 mM initial content). The cap is
computed from the cell's volume before the step and the sink is applied from its
volume after the step's growth, an O(growth_rate*dt) ~ 1e-4 relative mismatch.
That is a separate, second-order effect and is deliberately not what this test
gates; the overdraw it is about is of order 1e-2 to 1 mM.
exit status: 0 = every substrate >= -tolerance at every snapshot, 1 = violated
"""
import glob, os, sys
import xml.etree.ElementTree as ET
import numpy as np
import scipy.io as sio

def main():
    d = sys.argv[1]
    tol = float(sys.argv[2]) if len(sys.argv) > 2 else 1e-4
    xmls = sorted(glob.glob(os.path.join(d, "output*.xml")))
    if len(xmls) < 3:
        print(f"ERROR: {len(xmls)} snapshots in {d}; the run did not produce output")
        return 2
    names = None
    worst = {}
    for xf in xmls:
        r = ET.parse(xf).getroot()
        if names is None:
            names = [v.get("name") for v in r.findall(".//microenvironment/domain/variables/variable")]
        fn = r.find(".//microenvironment/domain/data/filename").text
        mm = sio.loadmat(os.path.join(d, fn))["multiscale_microenvironment"]
        # rows: x, y, z, voxel volume, then one row per substrate in declaration order
        sub = mm[4:4 + len(names), :]
        t = float(r.find(".//current_time").text)
        for k, nm in enumerate(names):
            m = float(sub[k].min())
            if nm not in worst or m < worst[nm][0]:
                worst[nm] = (m, t)
    ncells = None
    try:
        r = ET.parse(xmls[-1]).getroot()
        cn = r.find(".//cell_population/custom/simplified_data")
        ncells = sio.loadmat(os.path.join(d, cn.find("filename").text))["cells"].shape[1]
    except Exception:
        pass
    print(f"{d}: {len(xmls)} snapshots, {ncells} cells at the end")
    bad = False
    for nm in names:
        m, t = worst[nm]
        ok = m >= -tol
        bad |= not ok
        print(f"  {'ok  ' if ok else 'FAIL'} {nm:10s} min concentration {m: .6e} mM (t={t:g} min)")
    print("PASS" if not bad else f"FAIL: a substrate went below -{tol:g} mM")
    return 1 if bad else 0

if __name__ == "__main__":
    sys.exit(main())
