#!/usr/bin/env python3
"""
plot_df_journal_style_v3.py

Like v2, plus:
- `substrates_color_mode`: "auto" (Matplotlib default cycling) or "distinct" (ensure different colors
  for the two substrates in twin-axis mode). Default: "distinct".

Other v2 features kept:
- Optional twin-y for substrates when exactly 2 (`substrates_twin_if_two`)
- Bottom panel: at most two cell features (left/right)
- No final horizontal line by default
- Toggle scientific notation on right axis with `right_axis_sci` (default True)

"""

from __future__ import annotations

import argparse
from typing import Dict, Iterable, Mapping, Optional, Sequence, Tuple

import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd


# ===== Default style tuned for journals =====
JOURNAL_RC = {
    "font.size": 10,                 # axis tick/label size
    "axes.labelsize": 11,
    "axes.titlesize": 11,
    "legend.fontsize": 9,
    "xtick.direction": "out",
    "ytick.direction": "out",
    "axes.spines.top": False,
    "axes.spines.right": False,      # re-enable right spine only for twin axes
    "figure.dpi": 150,
}


SERIES_COLORS = {
    "glucose": "#f1c232ff",
    "oxygen": "#4285f4ff",
    "acetate": "#cc0000ff",
    "total_biomass": "#000000",
    "n_cells": "#000000",
}

SERIES_LINESTYLES = {
    "total_biomass": "-",
    "n_cells": ":",
}


def _ensure_columns(df: pd.DataFrame, cols: Iterable[str], df_name: str) -> None:
    missing = [c for c in cols if c is not None and c not in df.columns]
    if missing:
        raise ValueError(f"{df_name} is missing required columns: {missing}. "
                         f"Available columns: {list(df.columns)}")


def _plot_series(ax: plt.Axes, x, y, label: str, **kwargs):
    """Use ax.plot directly (not pandas) to ensure correct legend handles/colors."""
    line, = ax.plot(x, y, label=label, **kwargs)
    return line


def plot_panels(
    d_substrates: pd.DataFrame,
    d_cells: pd.DataFrame,
    substrates_cols: Sequence[str],
    cell_left_col: str,
    cell_right_col: Optional[str] = None,
    figsize: Tuple[float, float] = (6.5, 5.5),
    title_substrates: str = "Substrates",
    ylabel_top_left: str = "Concentration (mM)",
    ylabel_top_right: Optional[str] = None,   # used only if twin top is active
    ylabel_bottom_left: str = "Cell feature (left)",
    ylabel_bottom_right: str = "Cell feature (right)",
    labels: Optional[Mapping[str, str]] = None,
    style_rc: Optional[Mapping[str, object]] = None,
    show: bool = True,
    save_prefix: Optional[str] = None,
    save_formats: Iterable[str] = ("pdf", "png", "svg"),
    substrates_twin_if_two: bool = False,
    substrates_color_mode: str = "distinct",  # "auto" or "distinct"
    add_final_line: bool = False,             # optional, off by default
    right_axis_sci: bool = True,
):
    """
    Two-panel plot with optional twin-y on the top panel (if exactly 2 substrates),
    and at most TWO cell features (left/right) on the bottom panel.
    """
    labels = dict(labels or {})

    # --- Apply style ---
    rc_to_apply = dict(JOURNAL_RC)
    if style_rc:
        rc_to_apply.update(style_rc)
    mpl.rcParams.update(rc_to_apply)

    # --- Validate columns ---
    if len(substrates_cols) == 0:
        raise ValueError("Provide at least one substrate column.")
    _ensure_columns(d_substrates, substrates_cols, "d_substrates")
    _ensure_columns(d_cells, [cell_left_col] + ([cell_right_col] if cell_right_col else []), "d_cells")

    # --- Figure ---
    fig, (ax_top_left, ax_bot_left) = plt.subplots(
        2, 1, figsize=figsize, sharex=True, constrained_layout=True
    )

    # ===== (a) Substrates =====
    top_handles, top_labels = [], []
    ax_top_right = None
    colors = plt.rcParams["axes.prop_cycle"].by_key().get("color", [])
    if substrates_twin_if_two and len(substrates_cols) == 2:
        # left: first substrate, right: second substrate
        s_left, s_right = substrates_cols
        y_left = d_substrates[s_left].values
        y_right = d_substrates[s_right].values

        # color selection
        if substrates_color_mode not in {"auto", "distinct"}:
            raise ValueError("substrates_color_mode must be 'auto' or 'distinct'")
        left_kwargs = {"lw": 2.0}
        right_kwargs = {"lw": 2.0, "ls": "--"}
        if s_left in SERIES_COLORS:
            left_kwargs["color"] = SERIES_COLORS[s_left]
        if s_right in SERIES_COLORS:
            right_kwargs["color"] = SERIES_COLORS[s_right]
        if substrates_color_mode == "distinct":
            # Ensure different colors
            if "color" in left_kwargs and "color" in right_kwargs:
                pass
            elif len(colors) < 2:
                # fallback: force different basic colors
                left_kwargs["color"] = "tab:blue"
                right_kwargs["color"] = "tab:orange"
            else:
                left_kwargs["color"] = colors[0]
                right_kwargs["color"] = colors[1]
        # else: "auto" -> let Matplotlib choose

        # left
        h1 = _plot_series(ax_top_left, d_substrates.index.values, y_left,
                          labels.get(s_left, s_left), **left_kwargs)
        # right
        ax_top_right = ax_top_left.twinx()
        ax_top_right.spines["top"].set_visible(False)
        ax_top_right.spines["right"].set_visible(True)
        h2 = _plot_series(ax_top_right, d_substrates.index.values, y_right,
                          labels.get(s_right, s_right), **right_kwargs)
        ax_top_left.set_ylabel(ylabel_top_left)
        if ylabel_top_right is None:
            ylabel_top_right = ylabel_top_left
        ax_top_right.set_ylabel(ylabel_top_right)
        top_handles = [h1, h2]
        top_labels = [h1.get_label(), h2.get_label()]
    else:
        # single axis for all substrates
        for i, s in enumerate(substrates_cols):
            kw = {"lw": 2.0}
            if s in SERIES_COLORS:
                kw["color"] = SERIES_COLORS[s]
            elif i < len(colors):
                kw["color"] = colors[i]
            h = _plot_series(ax_top_left, d_substrates.index.values, d_substrates[s].values,
                             labels.get(s, s), **kw)
            top_handles.append(h)
            top_labels.append(h.get_label())
        ax_top_left.set_ylabel(ylabel_top_left)

    ax_top_left.set_title(title_substrates)
    ax_top_left.text(-0.08, 1.05, "a", transform=ax_top_left.transAxes,
                     fontsize=12, fontweight="bold", va="bottom")
    # Legend under panel (a)
    if top_handles:
        ax_top_left.legend(
            top_handles, top_labels, frameon=False, loc="upper center",
            bbox_to_anchor=(0.5, -0.22), ncol=min(len(top_handles), 3) or 1, handlelength=2
        )

    # ===== (b) Cell features (max two: left/right) =====
    ax_bot_right = None
    h_bottom, n_bottom = [], []

    # left
    left_kwargs = {"lw": 2.0}
    if cell_left_col in SERIES_COLORS:
        left_kwargs["color"] = SERIES_COLORS[cell_left_col]
    if cell_left_col in SERIES_LINESTYLES:
        left_kwargs["ls"] = SERIES_LINESTYLES[cell_left_col]
    hL = _plot_series(ax_bot_left, d_cells.index.values, d_cells[cell_left_col].values,
                      labels.get(cell_left_col, cell_left_col), **left_kwargs)
    h_bottom.append(hL); n_bottom.append(hL.get_label())
    ax_bot_left.set_ylabel(ylabel_bottom_left)

    # optional right
    if cell_right_col:
        ax_bot_right = ax_bot_left.twinx()
        ax_bot_right.spines["top"].set_visible(False)
        ax_bot_right.spines["right"].set_visible(True)
        # different color from left if possible
        kw = {"lw": 2.0, "ls": "--"}
        if cell_right_col in SERIES_COLORS:
            kw["color"] = SERIES_COLORS[cell_right_col]
        elif len(colors) >= 2:
            kw["color"] = colors[1]
        if cell_right_col in SERIES_LINESTYLES:
            kw["ls"] = SERIES_LINESTYLES[cell_right_col]
        hR = _plot_series(ax_bot_right, d_cells.index.values, d_cells[cell_right_col].values,
                          labels.get(cell_right_col, cell_right_col), **kw)
        h_bottom.append(hR); n_bottom.append(hR.get_label())
        ax_bot_right.set_ylabel(ylabel_bottom_right)
        if right_axis_sci:
            ax_bot_right.ticklabel_format(style="sci", axis="y", scilimits=(0, 0))

    if add_final_line:
        final_val = d_cells[cell_left_col].iloc[-1]
        ax_bot_left.axhline(y=final_val, linestyle=":", linewidth=1.5, color=hL.get_color())

    ax_bot_left.set_xlabel("Time (hours)")

    # Legend under panel (b)
    if h_bottom:
        ax_bot_left.legend(
            h_bottom, n_bottom, frameon=False, loc="upper center",
            bbox_to_anchor=(0.5, -0.22), ncol=min(len(h_bottom), 3) or 1, handlelength=2
        )

    # spacing for legends
    fig.subplots_adjust(hspace=0.35, bottom=0.20)

    # Panel letter
    ax_bot_left.text(-0.08, 1.05, "b", transform=ax_bot_left.transAxes,
                     fontsize=12, fontweight="bold", va="bottom")

    # Save if asked
    if save_prefix:
        for ext in save_formats:
            out = f"{save_prefix}.{ext}"
            if ext.lower() == "png":
                fig.savefig(out, dpi=600)
            else:
                fig.savefig(out)

    if show:
        plt.show()

    return fig, (ax_top_left, ax_top_right, ax_bot_left, ax_bot_right)


def _parse_labels(kv) -> Dict[str, str]:
    out: Dict[str, str] = {}
    for item in kv:
        if "=" in item:
            k, v = item.split("=", 1)
            out[k.strip()] = v.strip()
    return out


def _cli():
    p = argparse.ArgumentParser(description="Journal-style plotter v3 (substrates + up to two cell features).")
    p.add_argument("--substrates", required=True, help="Path to substrates CSV")
    p.add_argument("--cells", required=True, help="Path to cells CSV")
    p.add_argument("--index-col", default=None, help="Column to use as index (time in hours). If omitted, uses file index.")

    p.add_argument("--substrates-cols", required=True, help="Comma-separated list of substrates to plot")
    p.add_argument("--substrates-twin-if-two", action="store_true", help="If exactly two substrates, plot 2nd on right y-axis")
    p.add_argument("--substrates-color-mode", choices=["auto", "distinct"], default="distinct",
                   help="Color strategy for substrates; 'distinct' forces two different colors in twin mode.")

    p.add_argument("--cell-left", required=True, help="Cell feature for left y-axis (bottom panel)")
    p.add_argument("--cell-right", default=None, help="Optional cell feature for right y-axis (bottom panel)")

    p.add_argument("--title", default="Substrates")
    p.add_argument("--ylabel-top-left", default="Concentration (mM)")
    p.add_argument("--ylabel-top-right", default=None)
    p.add_argument("--ylabel-bottom-left", default="Cell feature (left)")
    p.add_argument("--ylabel-bottom-right", default="Cell feature (right)")

    p.add_argument("--labels", nargs="*", default=[], help='Labels as key=value pairs (quote values with spaces)')
    p.add_argument("--save-prefix", default=None, help="If set, save outputs with this prefix")
    p.add_argument("--no-show", action="store_true", help="Do not show the figure")
    p.add_argument("--add-final-line", action="store_true", help="Draw a horizontal line at the final left cell value")
    p.add_argument("--no-right-sci", action="store_true", help="Disable scientific notation on right y-axis")

    args = p.parse_args()

    # Read CSVs
    read_kwargs = {}
    if args.index_col is not None:
        read_kwargs["index_col"] = args.index_col

    d_sub = pd.read_csv(args.substrates, **read_kwargs)
    d_cells = pd.read_csv(args.cells, **read_kwargs)

    # If index was provided as column, try to convert to numeric (hours) if possible
    if args.index_col is not None:
        # try numeric, else datetime to numeric hours from start
        try:
            d_sub.index = pd.to_numeric(d_sub.index)
            d_cells.index = pd.to_numeric(d_cells.index)
        except Exception:
            try:
                sub_time = pd.to_datetime(d_sub.index)
                cells_time = pd.to_datetime(d_cells.index)
                t0 = min(sub_time.min(), cells_time.min())
                d_sub.index = (sub_time - t0).total_seconds() / 3600.0
                d_cells.index = (cells_time - t0).total_seconds() / 3600.0
            except Exception:
                pass

    labels = _parse_labels(args.labels)
    substrates_cols = [c.strip() for c in args.substrates_cols.split(",") if c.strip()]

    plot_panels(
        d_substrates=d_sub,
        d_cells=d_cells,
        substrates_cols=substrates_cols,
        substrates_twin_if_two=args.substrates_twin_if_two,
        substrates_color_mode=args.substrates_color_mode,
        cell_left_col=args.cell_left,
        cell_right_col=args.cell_right,
        title_substrates=args.title,
        ylabel_top_left=args.ylabel_top_left,
        ylabel_top_right=args.ylabel_top_right,
        ylabel_bottom_left=args.ylabel_bottom_left,
        ylabel_bottom_right=args.ylabel_bottom_right,
        labels=labels,
        show=not args.no_show,
        save_prefix=args.save_prefix,
        add_final_line=args.add_final_line,
        right_axis_sci=not args.no_right_sci,
    )


if __name__ == "__main__":
    _cli()
