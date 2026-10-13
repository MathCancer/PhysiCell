#!/usr/bin/env python3
"""Cells that cannot take a substrate must not reduce what the others get.

Compares the glucose removed from the domain in two runs of the voxel-sharing
test: A has n cells that can take glucose, B has the same n cells plus others
that share their voxel but have Vmax = 0 for glucose. With a cap that divides the
voxel by a head count, B's takers would receive a smaller share and B would
draw less. With a cap weighted by each cell's own request they draw the same.

usage: check_voxel_draw_equal.py <dir_A> <dir_B> [relative_tolerance]
exit status: 0 = equal within tolerance, 1 = not
"""
import glob, os, sys
import xml.etree.ElementTree as ET
import numpy as np
import scipy.io as sio

GLUCOSE_ROW = 5   # rows: x, y, z, voxel volume, oxygen, glucose, ...

def drawn(d):
    xs = sorted(glob.glob(os.path.join(d, "output*.xml")))
    tot = []
    for xf in (xs[0], xs[-1]):
        r = ET.parse(xf).getroot()
        fn = r.find(".//microenvironment/domain/data/filename").text
        tot.append(sio.loadmat(os.path.join(d, fn))["multiscale_microenvironment"][GLUCOSE_ROW].sum())
    return tot[0] - tot[1]

def main():
    a, b = drawn(sys.argv[1]), drawn(sys.argv[2])
    tol = float(sys.argv[3]) if len(sys.argv) > 3 else 0.01
    rel = abs(b - a) / max(abs(a), 1e-30)
    ok = a > 0 and rel <= tol
    print(f"glucose drawn: {sys.argv[1]} = {a:.6g}, {sys.argv[2]} = {b:.6g}, relative difference {rel:.3g}")
    print("PASS" if ok else f"FAIL: relative difference above {tol:g} (or nothing was drawn)")
    return 0 if ok else 1

if __name__ == "__main__":
    sys.exit(main())
