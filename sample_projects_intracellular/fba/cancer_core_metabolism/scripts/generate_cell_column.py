"""
generate_cell_column.py
=======================
Generates a PhysiCell-compatible cells.csv file placing one cell per voxel
throughout the entire simulation domain, organised as a 3-D regular grid.

Biological rationale
--------------------
The simulation domain has substrates entering from the xmin boundary (Dirichlet
boundary condition) and diffusing toward xmax.  Cells are arranged on a
voxel-aligned 3-D lattice so that:
  - Every YZ cross-section is fully populated → substrate gradients are
    experienced uniformly across the whole plane, avoiding edge artefacts.
  - Along X, each "layer" of cells sits at a different substrate concentration:
      · Cells near xmin → high substrate availability → expected to survive/grow
      · Cells near xmax → low substrate availability → expected to die/stall

This produces a readable 1-D phenotypic gradient (along X) while maintaining
realistic 3-D cell packing.

Cell placement logic
--------------------
Cells are placed at the centre of each voxel on a regular grid:

    for x in [x_start, x_start+dx, ..., x_max]:
        for y in [y_min, y_min+dy, ..., y_max]:
            for z in [z_min, z_min+dz, ..., z_max]:
                place one cell

Output CSV format (matches PhysiCell cells.csv convention):
    x, y, z, cell_type, volume

Usage
-----
Run directly:
    python generate_cell_column.py

Or import and call generate_cell_column() to integrate into a larger workflow.
"""

import csv
import math
import os
import random

# ---------------------------------------------------------------------------
# Configuration – edit these values to match your simulation setup
# ---------------------------------------------------------------------------

# Grid spacing (dx = dy = dz) as defined in PhysiCell_settings.xml [micron]
VOXEL_SIZE: float = 15.0

# Volume assigned to every placed cell [micron^3]
# Default ~2494 µm³ matches the MCF7_core reference volume in the settings XML
CELL_VOLUME: float = 2494.0

RAND_VOLUME: bool = True  # If True, add ±10% random variability to cell volumes
VOLUME_VARIABILITY: float = 1.8  # Fractional variability (e.g., 0.1 for ±10%)

# If True, cells are placed at voxel centroids (offset by voxel_size/2)
USE_CENTROIDS: bool = True  

# X coordinate of the first cell.  Cells are placed at this value and every
# VOXEL_SIZE increment thereafter until X_MAX is reached.
X_START: float = 0.0          # [micron] – first cell position on the X axis

# Domain boundaries (must match <domain> block in PhysiCell_settings.xml)
X_MAX: float = 300.0         # [micron]
Y_MIN: float = -60.0         # [micron]
Y_MAX: float =  60.0         # [micron]
Z_MIN: float = -15.0         # [micron]
Z_MAX: float =  15.0         # [micron]

# No Y/Z centre needed: cells fill the entire YZ plane at every voxel.

# Cell type name – must match the <cell_definition name="..."> in the XML
CELL_TYPE: str = "MCF7_core"

# Output file path (relative to this script, or provide an absolute path)
OUTPUT_CSV: str = os.path.join(os.path.dirname(__file__), "cells_column.csv")

# ---------------------------------------------------------------------------



def _axis_positions(start: float, end: float, step: float, centroid: bool = True) -> list[float]:
    """
    Return a list of evenly-spaced positions from *start* to *end* (inclusive
    within floating-point tolerance), advancing by *step*.

    If *centroid* is True, positions are offset by step/2 to place cells at
    voxel centroids.  If False, positions start at *start* and advance by *step*.
    """
    positions = []
    if centroid:
        offset_start = start + step / 2
        offset_end = end - step / 2
        n = math.floor((offset_end - offset_start) / step + 1e-9) + 1
        for i in range(n):
            v = offset_start + i * step
            if v <= offset_end + 1e-9:
                positions.append(v)
    else:
        n = math.floor((end - start) / step + 1e-9) + 1
        for i in range(n):
            v = start + i * step
            if v <= end + 1e-9:
                positions.append(v)
    return positions


def generate_cell_column(
    voxel_size: float = VOXEL_SIZE,
    cell_volume: float = CELL_VOLUME,
    x_start: float = X_START,
    x_max: float = X_MAX,
    y_min: float = Y_MIN,
    y_max: float = Y_MAX,
    z_min: float = Z_MIN,
    z_max: float = Z_MAX,
    cell_type: str = CELL_TYPE,
    output_csv: str = OUTPUT_CSV,
) -> list[dict]:
    """
    Place one cell per voxel on a 3-D lattice and write a cells.csv file.

    For every X slice (from x_start to x_max) the full YZ plane is populated,
    one cell per voxel.  This ensures every layer of cells experiences the
    substrate gradient uniformly.

    Parameters
    ----------
    voxel_size  : float
        Edge length of a cubic voxel [micron].  Used as the grid spacing in
        all three dimensions.
    cell_volume : float
        Volume assigned to each cell [micron^3].
    x_start     : float
        X coordinate of the first slice [micron].  Subsequent slices are at
        x_start + k * voxel_size for k = 0, 1, 2, ...
    x_max       : float
        Maximum X coordinate of the domain [micron].  No slice beyond this.
    y_min       : float
        Minimum Y coordinate of the domain [micron].
    y_max       : float
        Maximum Y coordinate of the domain [micron].
    z_min       : float
        Minimum Z coordinate of the domain [micron].
    z_max       : float
        Maximum Z coordinate of the domain [micron].
    cell_type   : str
        Cell type name as declared in PhysiCell_settings.xml.
    output_csv  : str
        Destination file path for the generated CSV.

    Returns
    -------
    list[dict]
        List of cell records (each a dict with keys x, y, z, cell_type, volume)
        so the caller can inspect or further process them.
    """

    # Build axis position lists
    x_positions = _axis_positions(x_start, x_max, voxel_size, centroid=USE_CENTROIDS)
    y_positions = _axis_positions(y_min, y_max, voxel_size, centroid=USE_CENTROIDS)
    z_positions = _axis_positions(z_min, z_max, voxel_size, centroid=USE_CENTROIDS)

    if not x_positions:
        raise ValueError(
            f"No valid X positions generated. "
            f"Check that x_start ({x_start}) < x_max ({x_max})."
        )

    # Assemble cell records: iterate over the full 3-D grid
    cells = []
    for x in x_positions:
        for y in y_positions:
            for z in z_positions:
                if RAND_VOLUME:
                    volume = random.uniform(cell_volume * 0.1, cell_volume * VOLUME_VARIABILITY)  # Add ±10% variability
                else:
                    volume = cell_volume
                cells.append({
                    "x": x,
                    "y": y,
                    "z": z,
                    "cell_type": cell_type,
                    "volume": volume,
                })

    # Write CSV
    with open(output_csv, "w", newline="") as fh:
        writer = csv.DictWriter(
            fh,
            fieldnames=["x", "y", "z", "cell_type", "volume"],
        )
        writer.writeheader()
        writer.writerows(cells)

    nx, ny, nz = len(x_positions), len(y_positions), len(z_positions)
    print(f"Wrote {len(cells)} cells to: {output_csv}")
    print(f"  Grid    : {nx} × {ny} × {nz}  (X × Y × Z slices)")
    print(f"  X range : {x_positions[0]:.2f} → {x_positions[-1]:.2f} µm")
    print(f"  Y range : {y_positions[0]:.2f} → {y_positions[-1]:.2f} µm")
    print(f"  Z range : {z_positions[0]:.2f} → {z_positions[-1]:.2f} µm")
    print(f"  Spacing : {voxel_size:.2f} µm (one cell per voxel)")
    print(f"  Volume  : {cell_volume:.2f} µm³ per cell")

    return cells


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    generate_cell_column()
