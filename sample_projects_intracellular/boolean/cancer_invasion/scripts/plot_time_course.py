#!/usr/bin/env python
# -*- coding: utf-8 -*-


import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns




from pctk.multicellds import MultiCellDS


def labelsubplot(ax, idx, xpos=-0.1, ypos=1.05, weight="bold", fontsize=12, color='#434343'):
    a_ascci_idx = ord('a')
    subplot_label = chr(a_ascci_idx+idx) + ')'
    box_aspect = ax.get_box_aspect()
    if box_aspect is not None and len(box_aspect) == 3:
        ax.text2D(xpos, ypos, subplot_label, weight="bold", ha='left', va='center',
                fontsize=fontsize, color=color, transform=ax.transAxes)
    else:
        ax.text(xpos, ypos, subplot_label, weight="bold", ha='left', va='center',
                fontsize=fontsize, color=color, transform=ax.transAxes)
   



def get_timeserie_mean(mcds, filter_alive=True):
    time = []
    values = []
    filter_alive = True
    for t, df in mcds.cells_as_frames_iterator():
        time.append(t)
        df = df.iloc[:,3:]
        if filter_alive:
            mask = df['current_phase'] <= 14
            df = df[mask]
        values.append(df.mean(axis=0).values)

    cell_columns = df.columns.tolist()
    df = pd.DataFrame(values, columns=cell_columns)
    df['time'] = time
    return df[['time'] + cell_columns]


def get_timeserie_density(mcds):
    data = []
    for t,m in mcds.microenvironment_as_matrix_iterator():
        data.append((t, m[5,:].sum()))
    df = pd.DataFrame(data=data, columns=['time', 'tnf'])
    return df

def plot_molecular_model(df_cell_variables, list_of_variables, ax1):

    for label in list_of_variables:
        y = df_cell_variables[label]
        time = df_cell_variables["time"]
        ax1.plot(time, y, label="% " + label)

    ax1.set_ylabel("% X")
    ax1.yaxis.grid(True)
    ax1.set_xlim((0, time.values[-1]))
    ax1.set_ylim((0, 1.05))
    
    
def plot_cells(df_time_course, color_dict, ax):

    # Alive/Apoptotic/Necrotic vs Time
    for k in color_dict:
        ax.plot(df_time_course.time, df_time_course[k], "-", c=color_dict[k], label=k)
    
    # setting axes labels
    # ax.set_xlabel("time (min)")
    ax.set_ylabel("N of cells")
    
    # Showing legend
    ax.legend()
    ax.yaxis.grid(True)


def plot_time_course(df_time_course, df_time_tnf, df_cell_variables, list_of_variables):
    fig, axes = plt.subplots(2, 1, figsize=(10,4), dpi=300, sharex=True)
    
    custom_palette = sns.color_palette("deep")
    color_dict = {"live": custom_palette[2], "apoptotic": custom_palette[3], "necrotic":custom_palette[5]}
    
    plot_cells(df_time_course, color_dict, axes[0])

    for i,ax in enumerate(axes):
        labelsubplot(ax, i, xpos=-0.065)

    ax2 = axes[0].twinx()
    ax2.plot(df_time_tnf.time, df_time_tnf['tnf'], 'lightgrey')

    ax2.set_yticks([])
    ax2.fill_between(df_time_tnf.time, df_time_tnf['tnf'], color='lightgrey', alpha=0.3)

    axes[0].set_zorder(ax2.get_zorder()+1)
    axes[0].patch.set_visible(False)

    plot_molecular_model(df_cell_variables, list_of_variables, axes[1])
    threshold = 0.5

    axes[1].hlines(threshold, 0, df_time_course.time.iloc[-1])
    axes[1].legend(loc="upper left")
    axes[1].set_xlabel("Time (min)")


    fig.tight_layout()
    sns.despine(fig)
    
    return fig

def load_datasets(instance_folder, labels_dict):
    
    
    mcds = MultiCellDS(instance_folder)
    
    df_time_course = mcds.get_cells_summary_frame()
    df_time_tnf = get_timeserie_density(mcds)
    df_cell_variables = get_timeserie_mean(mcds)
    df_cell_variables = df_cell_variables.rename(labels_dict, axis=1)
    
    if (df_time_course["live"] == 0).sum() > 0:
        idx = df_time_course.index[df_time_course["live"] == 0][0]
        df_time_course = df_time_course.iloc[:idx]
        df_cell_variables = df_cell_variables.iloc[:idx]
        df_time_tnf = df_time_tnf.iloc[:idx]
        
    return df_time_course, df_time_tnf, df_cell_variables


sns.set_style("white")
sns.set_palette("deep")


def main():
    if len(sys.argv) == 1:
        output_folder = "output"
    else:
        output_folder = sys.argv[1]


    labels_dict = {}
    labels_dict['bound_external_TNFR'] = "TNFR-TNF[e]"
    labels_dict['unbound_external_TNFR'] = "TNFR[e]"
    labels_dict['bound_internal_TNFR'] = "TNFR-TNF[i]"
    df_time_course, df_time_tnf, df_cell_variables = load_datasets(output_folder, labels_dict)

    list_of_variables = list(labels_dict.values())
    fig = plot_time_course(df_time_course, df_time_tnf, df_cell_variables, list_of_variables)

    fig.savefig(f"{output_folder}/Time_course.png")

    
main()