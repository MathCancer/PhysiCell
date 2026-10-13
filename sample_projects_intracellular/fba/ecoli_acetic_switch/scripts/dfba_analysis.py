import os
import glob
import xml.etree.ElementTree as ET
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
from pctk import multicellds
from matplotlib.ticker import ScalarFormatter
import numpy as np

# Try to import the custom module, but don't fail if missing
try:
    from plot_df_journal_style_ext import plot_panels
except ImportError:
    pass

# ==========================================
# CONFIGURATION
# ==========================================
RUNS_DIR = "../runs"      # Folder containing the simulation subfolders
FIGS_DIR = "figs"         # Folder to save the figures

SERIES_COLORS = {
    "glucose": "#f1c232ff",
    "oxygen": "#4285f4ff",
    "acetate": "#cc0000ff",
    "biomass": "#000000",
    "n_cells": "#000000",
}

# --- Settings for Plot 3 (Spatial Gradients) ---
PLOT3_TIME_INDICES   = [15, 60, 100]              # Time indices to plot
PLOT3_SUBSTRATES     = ['oxygen', 'glucose', 'acetate'] # Substrates to plot
PLOT3_MICROENV_COLS  = ['x','y','z','volume','oxygen','glucose','acetate','co2'] # Column mapping
PLOT3_LEVELS         = 15
PLOT3_ARROW_TARGET   = 8            # ~how many vectors along each axis
PLOT3_ARROW_FRAC     = 0.05         # arrow length as a fraction of domain size
PLOT3_GRAD_THRESH    = 10           # keep arrows above this percentile of |grad|
PLOT3_CMAP           = 'YlOrRd'     # Default colormap

# --- Numeric cleanup ---
# Values smaller than this are rounded down to 0 (in mM).
SUBSTRATE_ZERO_THRESH = 1e-3

# ==========================================
# HELPER FUNCTIONS
# ==========================================

def get_domain_volume(xml_path):
    """Parses the PhysiCell settings XML to calculate the domain volume in um^3."""
    tree = ET.parse(xml_path)
    root = tree.getroot()
    domain = root.find("domain")
    
    x_min = float(domain.find("x_min").text)
    x_max = float(domain.find("x_max").text)
    y_min = float(domain.find("y_min").text)
    y_max = float(domain.find("y_max").text)
    z_min = float(domain.find("z_min").text)
    z_max = float(domain.find("z_max").text)
    
    x_range = x_max - x_min
    y_range = y_max - y_min
    z_range = z_max - z_min
    
    return x_range * y_range * z_range

def _reshape_to_grid(x, y, v):
    """Reshapes flat coordinate vectors into a 2D grid."""
    xu = np.unique(x); yu = np.unique(y)
    nx, ny = xu.size, yu.size
    if nx * ny != v.size:
        raise ValueError(f"Cannot reshape: expected {nx}*{ny}={nx*ny} points, got {v.size}")
    ix = np.searchsorted(xu, x)
    iy = np.searchsorted(yu, y)
    Z = np.full((ny, nx), np.nan)
    Z[iy, ix] = v
    dx = np.diff(xu).mean() if nx>1 else 1.0
    dy = np.diff(yu).mean() if ny>1 else 1.0
    Xg, Yg = np.meshgrid(xu, yu, indexing='xy')
    return Xg, Yg, Z, dx, dy

def _soft_zero(Z, abs_eps=1e-12, rel_eps=1e-6, clamp_nonneg=False):
    """Set tiny values to exactly 0 to remove floating noise."""
    Z = Z.copy()
    finite = np.isfinite(Z)
    if not finite.any():
        return Z
    max_abs = np.nanmax(np.abs(Z[finite]))
    eps = max(abs_eps, rel_eps * max_abs) if max_abs > 0 else abs_eps
    Z[np.abs(Z) < eps] = 0.0
    if clamp_nonneg:
        Z[Z < 0] = 0.0
    return Z

def _threshold_zero(Z, thresh=SUBSTRATE_ZERO_THRESH, clamp_nonneg=False, use_abs=False):
    """Hard-threshold values to 0 when they are below `thresh`.

    By default this uses a one-sided threshold (values < thresh -> 0).
    Set `use_abs=True` to threshold by magnitude (|Z| < thresh -> 0).
    """
    Z = np.asarray(Z).copy()
    finite = np.isfinite(Z)
    if not finite.any():
        return Z
    if use_abs:
        Z[finite & (np.abs(Z) < thresh)] = 0.0
    else:
        Z[finite & (Z < thresh)] = 0.0
    if clamp_nonneg:
        Z[Z < 0] = 0.0
    return Z

# ==========================================
# MAIN PROCESSING LOGIC
# ==========================================

def process_simulation(sim_folder, output_fig_dir):
    sim_name = os.path.basename(os.path.normpath(sim_folder))
    print(f"Processing: {sim_name}")
    
    # Find config file
    config_files = glob.glob(os.path.join(sim_folder, "*.xml"))
    config_path = None
    for f in config_files:
        if "settings" in os.path.basename(f):
            config_path = f
            break
    if not config_path and config_files:
        config_path = config_files[0]
        
    if not config_path:
        print(f"  Warning: No XML config found in {sim_folder}. Skipping.")
        return

    # Find output folder
    output_path = os.path.join(sim_folder, "output")
    if not os.path.exists(output_path):
        print(f"  Warning: No 'output' folder found in {sim_folder}. Skipping.")
        return

    # Get volume
    try:
        total_domain_volume_um3 = get_domain_volume(config_path)
        reactor_volume_L = total_domain_volume_um3 / 1e15
    except Exception as e:
        print(f"  Error reading config {config_path}: {e}")
        return

    # Initialize MCDS reader
    try:
        reader = multicellds.MultiCellDS(output_folder=output_path)
    except Exception as e:
        print(f"  Error initializing MultiCellDS for {output_path}: {e}")
        return

    # ---------------------------------------------------------
    # PART 1: Data Collection for Time Series (Plots 1 & 2)
    # ---------------------------------------------------------
    data_substrates = []
    data_cells = []
    
    # We iterate once for the time-series averages
    # Assuming indices: 4=oxygen, 5=glucose, 6=acetate
    for t, a in reader.microenvironment_as_matrix_iterator():
        # Substrate averages

        # Round down tiny concentrations (mM) to exactly 0
        glc = np.where(a[5] < SUBSTRATE_ZERO_THRESH, 0.0, a[5])
        ace = np.where(a[6] < SUBSTRATE_ZERO_THRESH, 0.0, a[6])
        o2  = np.where(a[4] < SUBSTRATE_ZERO_THRESH, 0.0, a[4])

        data_substrates.append((
            t/60, 
            (glc.sum() / len(glc)), 
            (ace.sum() / len(ace)), 
            (o2.sum() / len(o2)) 
        ))

    # Cells iteration
    for t, a in reader.cells_as_matrix_iterator():
        total_volume = a[:,4].sum() # um3
        scaled_volume = total_volume / 10**12 # mL
        density = 1.04 # g/ml
        solid_fraction = 0.25
        biomass = scaled_volume * density * solid_fraction / reactor_volume_L
        data_cells.append((t/60, a[:,-2].mean(), a.shape[0], biomass))

    d_substrates = pd.DataFrame(data=data_substrates, columns=["time", "glucose", "acetate", "oxygen"]).set_index("time")
    d_cells = pd.DataFrame(data=data_cells, columns=["time", "growth_rate", "n_cells", "total_biomass"]).set_index("time")

    # ---------------------------------------------------------
    # PART 2: Plotting Time Series
    # ---------------------------------------------------------
    
    # --- Plot 1: Growth Rate and N_Cells ---
    fig1, ax_g = plt.subplots(figsize=(6, 4))
    d_cells["growth_rate"].plot(ax=ax_g, label="Growth Rate")
    ax_g.set_ylabel("Growth Rate")
    ax_g.set_xlabel("Time (hours)")
    ax_g.grid(axis="x")
    
    ax_n = ax_g.twinx()
    d_cells["n_cells"].plot(ax=ax_n, c=SERIES_COLORS["n_cells"], linestyle=":", label="N Cells")
    ax_n.set_ylabel("N Cells")
    plt.title(f"Growth Rate and Cell Count\n{sim_name}")
    
    fig1_path = os.path.join(output_fig_dir, f"{sim_name}_growth_ncells.png")
    plt.savefig(fig1_path, dpi=300, bbox_inches='tight')
    plt.close(fig1)

    # --- Plot 2: Substrates and Biomass (Nature Style) ---
    mpl.rcParams['font.size'] = 7
    mpl.rcParams['lines.linewidth'] = 1.0
    
    fig_width = 5.5
    fig_height = 2.5
    fig, ax1 = plt.subplots(figsize=(fig_width, fig_height), dpi=300)
    colors = {
        'glucose': SERIES_COLORS['glucose'],
        'acetate': SERIES_COLORS['acetate'],
        'oxygen': SERIES_COLORS['oxygen'],
        'biomass': SERIES_COLORS['biomass'],
    }

    l1, = ax1.plot(d_substrates.index, d_substrates["glucose"], label="Glucose", color=colors['glucose'])
    l2, = ax1.plot(d_substrates.index, d_substrates["acetate"], label="Acetate", color=colors['acetate'])
    l3, = ax1.plot(d_substrates.index, d_substrates["oxygen"], label="Oxygen", color=colors['oxygen'])

    ax1.set_xlabel("Time (hours)")
    ax1.set_ylabel("Concentration (mM)")
    ax2 = ax1.twinx()
    l4, = ax2.plot(d_cells.index, d_cells["total_biomass"], label="Total biomass", color=colors['biomass'], linestyle="-")
    ax2.set_ylabel("Total biomass (g/L)")

    # 4-Axis Box Style
    ax1.spines['top'].set_visible(True)
    ax2.spines['top'].set_visible(True)

    # Legends
    lines_1 = [l1, l2, l3]
    leg1 = ax1.legend(lines_1, [l.get_label() for l in lines_1], loc="center right", bbox_to_anchor=(0.98, 0.65), frameon=True, fancybox=True)
    lines_2 = [l4]
    leg2 = ax2.legend(lines_2, [l.get_label() for l in lines_2], loc="center left", bbox_to_anchor=(0.02, 0.65), frameon=True, fancybox=True)

    plt.title(f"Substrates and Biomass Dynamics\n{sim_name}")
    plt.tight_layout()
    fig2_path = os.path.join(output_fig_dir, f"{sim_name}_substrates_biomass.png")
    plt.savefig(fig2_path, dpi=300)
    plt.close(fig)

    # ---------------------------------------------------------
    # PART 3: Spatial Gradient Fields (Final Polish)
    # ---------------------------------------------------------
    try:
        print(f"  Generating spatial gradient plot for indices {PLOT3_TIME_INDICES}...")
        
        # 1. Collect snapshots
        wanted = set(PLOT3_TIME_INDICES)
        snapshots = {}
        
        # Note: Using a fresh iterator to re-read
        for idx, (t, a) in enumerate(reader.microenvironment_as_matrix_iterator()):
            if idx in wanted:
                snapshots[idx] = (t, a)
            if len(snapshots) == len(wanted):
                break
        
        missing = [i for i in PLOT3_TIME_INDICES if i not in snapshots]
        if missing:
            print(f"  Warning: Simulation '{sim_name}' is too short. Missing indices: {missing}. Skipping Plot 3.")
        else:
            # 2. Setup Figure
            nrows = len(PLOT3_SUBSTRATES)
            ncols = len(PLOT3_TIME_INDICES)
            fig_width_p3 = 7.2
            fig_height_p3 = 6.0 
            fig3, axes = plt.subplots(nrows, ncols, figsize=(fig_width_p3, fig_height_p3), dpi=300)
            axes = np.atleast_2d(axes)

            # 3. Plotting Loop
            for r, sub in enumerate(PLOT3_SUBSTRATES):
                try:
                    sub_idx = PLOT3_MICROENV_COLS.index(sub)
                except ValueError:
                    print(f"  Error: Substrate '{sub}' not found. Skipping row.")
                    continue

                # Colormap selection
                if sub in SERIES_COLORS:
                    cmap_obj = mpl.colors.LinearSegmentedColormap.from_list(
                        f"{sub}_series_cmap", ["#ffffff", SERIES_COLORS[sub]]
                    )
                else:
                    cmap_obj = plt.get_cmap(PLOT3_CMAP)

                for c, idx in enumerate(PLOT3_TIME_INDICES):
                    ax = axes[r, c]
                    t, A = snapshots[idx]

                    # --- 3D Slicing: Pick center slice ---
                    x_all, y_all, z_all = A[0, :], A[1, :], A[2, :]
                    vals_all = A[sub_idx, :]

                    unique_z = np.unique(z_all)
                    # z_focus = unique_z[np.argmin(np.abs(unique_z))]
                    z_focus = unique_z.max()
                    mask_plane = (z_all == z_focus)

                    x = x_all[mask_plane]
                    y = y_all[mask_plane]
                    vals = vals_all[mask_plane]
                    
                    Xg, Yg, Z, dx, dy = _reshape_to_grid(x, y, vals)

                    # Soft zero 
                    clamp_nonneg = (sub in {'oxygen','glucose','acetate','co2'})
                    Z = _soft_zero(Z, abs_eps=1e-12, rel_eps=1e-6, clamp_nonneg=clamp_nonneg)

                    # Hard threshold: values below SUBSTRATE_ZERO_THRESH are forced to 0
                    # (keeps near-zero background from showing up as tiny gradients).
                    if clamp_nonneg:
                        Z = _threshold_zero(Z, thresh=SUBSTRATE_ZERO_THRESH, clamp_nonneg=True, use_abs=False)
                    else:
                        Z = _threshold_zero(Z, thresh=SUBSTRATE_ZERO_THRESH, clamp_nonneg=False, use_abs=True)

                    # Check for "Flat" field to avoid numerical noise arrows
                    z_range = np.nanmax(Z) - np.nanmin(Z)
                    is_flat = z_range < 1e-6 

                    if is_flat:
                        # If flat, we just don't compute/plot arrows
                        mask = np.zeros_like(Z, dtype=bool) 
                    else:
                        Z_med = np.nanmedian(Z) if np.isfinite(Z).any() else 0.0
                        Zg = np.where(np.isfinite(Z), Z, Z_med)
                        dZdy, dZdx = np.gradient(Zg, dy, dx)
                        U, V = -dZdx, -dZdy
                        mag = np.hypot(U, V)
                        
                        # Subsample 
                        sx = max(1, Z.shape[1] // PLOT3_ARROW_TARGET)
                        sy = max(1, Z.shape[0] // PLOT3_ARROW_TARGET)
                        Xs = Xg[::sy, ::sx]; Ys = Yg[::sy, ::sx]
                        Us = U[::sy, ::sx]; Vs = V[::sy, ::sx]; Ms = mag[::sy, ::sx]
                        
                        finite = np.isfinite(Ms)
                        if finite.any():
                            th = np.percentile(Ms[finite], max(0, PLOT3_GRAD_THRESH - 5))
                            mask = finite & (Ms > th)
                        else:
                            mask = np.zeros_like(Ms, dtype=bool)

                        # Normalize
                        Msafe = np.where(Ms == 0, 1.0, Ms)
                        Un = Us / Msafe; Vn = Vs / Msafe
                        L = min(Xg.max() - Xg.min(), Yg.max() - Yg.min()) * PLOT3_ARROW_FRAC

                    # Display Scaling
                    finite_Z = Z[np.isfinite(Z)]
                    peak = np.nanmax(np.abs(finite_Z)) if finite_Z.size else 0
                    exp = int(np.floor(np.log10(peak))) if peak > 0 else 0
                    use_exp = (abs(exp) >= 2)
                    scale = 10.0 ** exp if use_exp else 1.0
                    Z_disp = Z / scale

                    # Plot Contour
                    cntr = ax.contourf(Xg, Yg, Z_disp, levels=PLOT3_LEVELS, cmap=cmap_obj)
                    
                    # --- SMART ARROW COLOR ---
                    # 1. Get average value of the field
                    avg_val = np.nanmean(Z) if np.isfinite(Z).any() else 0.5
                    # 2. Normalize to 0-1 range for the colormap
                    norm_func = mpl.colors.Normalize(vmin=np.nanmin(Z), vmax=np.nanmax(Z))
                    # 3. Get RGBA color of the average background
                    rgba = cmap_obj(norm_func(avg_val))
                    # 4. Calculate Luminance (Perceived brightness)
                    luminance = 0.299*rgba[0] + 0.587*rgba[1] + 0.114*rgba[2]
                    
                    # If background is bright (>0.5), use BLACK arrows. Otherwise WHITE.
                    arrow_color = 'black' if luminance > 0.5 else 'white'
                    # -------------------------

                    # Plot Arrows (if not flat)
                    if not is_flat:
                        ax.quiver(
                            Xs[mask], Ys[mask], Un[mask]*L, Vn[mask]*L,
                            color=arrow_color, alpha=0.9, angles='xy', scale_units='xy', scale=1.0,
                            width=0.005, headwidth=4.5, headlength=5.0, pivot='middle'
                        )

                    # Colorbar
                    cb = fig3.colorbar(cntr, ax=ax, fraction=0.046, pad=0.018)
                    z_max = np.nanmax(np.abs(Z))
                    if z_max > 1e-9:
                        fmt = ScalarFormatter(useMathText=True)
                        fmt.set_scientific(False)
                        cb.ax.yaxis.set_major_formatter(fmt)
                        cb.update_ticks()
                        label = f"{sub.capitalize()} (mM)"
                        if use_exp and exp != 0:
                            label = f"{sub.capitalize()} (mM × $10^{{{exp}}}$)"
                        cb.set_label(label, fontsize=7)
                        cb.ax.tick_params(labelsize=6)
                    else:
                        cb.ax.set_visible(False)

                    # Axis Styling
                    if r == 0:
                        ax.set_title(f"t = {t/60.0:.1f} hr", fontsize=8, fontweight='bold')
                    ax.set_aspect('equal')
                    if r == nrows - 1: ax.set_xlabel('X (μm)')
                    else: ax.set_xlabel('')
                    if c == 0: ax.set_ylabel('Y (μm)')
                    else: ax.set_ylabel('')
                    ax.set_xticks([]); ax.set_yticks([])

            # Row labels
            for r, sub in enumerate(PLOT3_SUBSTRATES):
                axes[r, 0].text(-0.12, 0.5, sub.capitalize(), transform=axes[r, 0].transAxes,
                                ha='right', va='center', fontsize=8, fontweight='bold', rotation=90)

            fig3.suptitle(f"Substrate Gradients (Z={z_focus:.1f} μm): {sim_name}", fontsize=10, fontweight='bold', y=0.995)
            fig3.tight_layout(rect=[0, 0.02, 1, 0.97], w_pad=2.5, h_pad=2.0)
            
            fig3_path = os.path.join(output_fig_dir, f"{sim_name}_spatial_gradients.png")
            plt.savefig(fig3_path, dpi=300)
            plt.close(fig3)

    except Exception as e:
        print(f"  Error generating spatial plot for {sim_name}: {e}")
        # We ensure we close the figure even if error occurs, to free memory
        try:
            plt.close(fig3)
        except:
            pass

    print(f"  Saved plots to {output_fig_dir}")

def main():
    print(f"Looking for runs in: {os.path.abspath(RUNS_DIR)}")
    
    if not os.path.exists(RUNS_DIR):
        print(f"Error: Runs directory '{RUNS_DIR}' not found.")
        return

    if not os.path.exists(FIGS_DIR):
        os.makedirs(FIGS_DIR)

    sim_folders = [f.path for f in os.scandir(RUNS_DIR) if f.is_dir()]
    sim_folders.sort()
    
    if not sim_folders:
        print("No simulation folders found.")
        return

    print(f"Found {len(sim_folders)} simulation folders.")

    for sim_folder in sim_folders:
        process_simulation(sim_folder, FIGS_DIR)

if __name__ == "__main__":
    main()