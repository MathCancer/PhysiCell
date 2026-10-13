"""Shared figure size and typography for composite panels.

Panel width PLOT_W_MM (52.5 mm) is sized for ~3 panels across a full-width
Cell Press figure (174 mm). Assemble multi-panel figures at 174 mm width in
LaTeX; add bold uppercase panel labels (A, B, C; 9–10 pt) there, not in these exports.

Figure checklist (verify against current author guide):
  - Full-width figure: 174 mm; single column: 85 mm; max height: 225 mm
  - Font: Arial or Helvetica, embedded in PDF; text 6–8 pt at final print size
  - Line weights: 0.5–1.5 pt; combination figures ≥300 dpi (vector preferred)
  - Panel labels on the assembled figure only; legend text stays minimal
"""

MM_PER_IN = 25.4
# Sub-panel width for ~3-across layout in a 174 mm full-width Cell Press figure
# (52.5 mm leaves room for inter-panel gaps and panel labels).
PLOT_W_MM = 174.0 / 3.0 - 5.5
LEG_W_MM = 22.0                        # right-side legend gutter (flux plots)
TOTAL_H_MM = (297.0 / 2.0) / 2.0      # 74.25 mm total figure height (flux stack)

FIG_W_IN = (PLOT_W_MM + LEG_W_MM) / MM_PER_IN
FIG_H_IN = TOTAL_H_MM / MM_PER_IN

# Single-panel time series aligned to one row of the flux combined stack
FIG_FLUX_ROW_W_IN = FIG_W_IN
FIG_FLUX_ROW_H_IN = FIG_H_IN / 2.0
FLUX_ROW_LEFT = 0.24
FLUX_ROW_RIGHT = PLOT_W_MM / (PLOT_W_MM + LEG_W_MM)
FLUX_ROW_TOP = 0.96
FLUX_ROW_BOTTOM = 0.14

# Bottom-legend variant: full plot width (no right gutter), slightly taller canvas
FIG_FLUX_ROW_BELOW_H_IN = FIG_H_IN / 1.55
FLUX_ROW_BELOW_LEFT = 0.22
FLUX_ROW_BELOW_RIGHT = 0.96
FLUX_ROW_BELOW_TOP = 0.92
FLUX_ROW_BELOW_BOTTOM = 0.28

# Combined single-panel plots with legend below axes (substrate, biomass)
COMBINED_PANEL_WIDTH_SCALE = 2.0       # 2× wide sub-panel (~105 mm)
COMBINED_PANEL_LEFT = 0.18
COMBINED_PANEL_RIGHT = 0.98
COMBINED_PANEL_TOP = 0.93
COMBINED_PANEL_BOTTOM = 0.27
COMBINED_PANEL_LEGEND_BBOX = (0.5, -0.24)  # axes coords: just below x-axis label
_COMBINED_PANEL_W_FRAC = COMBINED_PANEL_RIGHT - COMBINED_PANEL_LEFT
_COMBINED_PANEL_H_FRAC = COMBINED_PANEL_TOP - COMBINED_PANEL_BOTTOM
_COMBINED_PANEL_BASE_H_IN = (PLOT_W_MM / MM_PER_IN) * _COMBINED_PANEL_W_FRAC / _COMBINED_PANEL_H_FRAC
FIG_COMBINED_PANEL_W_IN = COMBINED_PANEL_WIDTH_SCALE * PLOT_W_MM / MM_PER_IN
FIG_COMBINED_PANEL_H_IN = _COMBINED_PANEL_BASE_H_IN
COMBINED_PANEL_PAD_INCHES = 0.02

# Back-compat aliases (substrate time series naming)
COMBINED_TS_LEFT = COMBINED_PANEL_LEFT
COMBINED_TS_RIGHT = COMBINED_PANEL_RIGHT
COMBINED_TS_TOP = COMBINED_PANEL_TOP
COMBINED_TS_BOTTOM = COMBINED_PANEL_BOTTOM
FIG_COMBINED_TS_W_IN = FIG_COMBINED_PANEL_W_IN
FIG_COMBINED_TS_H_IN = FIG_COMBINED_PANEL_H_IN

FS_LABEL = 6.5
COMBINED_PANEL_FS_LABEL = 8.0
FS_TICK = 6.0
FS_TITLE = 7.0
FS_ROW_LABEL = 6.5
FS_CB_LABEL = 6.0
FS_CB_TICK = 5.5
FS_LEGEND = 6.0
LW_DATA = 0.8
LW_SPINE = 0.5

_CELL_PRESS_RC = {
    'font.family': 'sans-serif',
    'font.sans-serif': ['Arial', 'Helvetica', 'DejaVu Sans', 'Liberation Sans'],
    'pdf.fonttype': 42,
    'ps.fonttype': 42,
    'svg.fonttype': 'none',
}


def apply_cell_press_style():
    """Apply Arial-first typography and embeddable fonts for Cell Press export."""
    import matplotlib.pyplot as plt
    plt.rcParams.update(_CELL_PRESS_RC)


def layout_combined_panel(fig):
    """Reserve bottom margin for x-axis label + legend below the plot area."""
    fig.subplots_adjust(
        left=COMBINED_PANEL_LEFT, right=COMBINED_PANEL_RIGHT,
        top=COMBINED_PANEL_TOP, bottom=COMBINED_PANEL_BOTTOM,
    )


def layout_flux_row_panel(fig):
    """Layout a single-panel plot to match one row of the flux combined stack."""
    fig.subplots_adjust(
        left=FLUX_ROW_LEFT, right=FLUX_ROW_RIGHT,
        top=FLUX_ROW_TOP, bottom=FLUX_ROW_BOTTOM,
    )


def layout_flux_row_panel_legend_below(fig):
    """Flux-row width; full plot area with legend below the x-axis."""
    fig.subplots_adjust(
        left=FLUX_ROW_BELOW_LEFT, right=FLUX_ROW_BELOW_RIGHT,
        top=FLUX_ROW_BELOW_TOP, bottom=FLUX_ROW_BELOW_BOTTOM,
    )


def add_flux_row_legend(ax, *, ncol=2, italic=False):
    """Place legend to the right of the plot (flux combined stack style)."""
    leg = ax.legend(
        loc='center left',
        bbox_to_anchor=(1.01, 0.5),
        ncol=ncol,
        fontsize=FS_LEGEND,
        frameon=False,
        handlelength=1.0,
        labelspacing=0.25,
        borderpad=0,
        handletextpad=0.4,
    )
    for txt in leg.get_texts():
        txt.set_color('#222222')
        if italic:
            txt.set_fontstyle('italic')
    return leg


def add_combined_panel_legend(ax, *, ncol=2, italic=False):
    """Place legend below the x-axis using axes-relative coordinates."""
    leg = ax.legend(
        loc='upper center',
        bbox_to_anchor=COMBINED_PANEL_LEGEND_BBOX,
        ncol=ncol,
        fontsize=FS_LEGEND,
        frameon=False,
        handlelength=1.0,
        labelspacing=0.2,
        columnspacing=0.6,
        borderaxespad=0.0,
        handletextpad=0.35,
    )
    for txt in leg.get_texts():
        txt.set_color('#222222')
        if italic:
            txt.set_fontstyle('italic')
    return leg
