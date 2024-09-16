#!/usr/bin/env python
# coding: utf-8

import re
import os
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


from pctk.multicellds import MultiCellDS

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

    threshold = 0.5

    for label in list_of_variables:
        y = df_cell_variables[label]
        time = df_cell_variables["time"]
        ax1.plot(time, y, label="% X " + label)

    ax1.set_ylabel("% X")
    ax1.yaxis.grid(True)
    ax1.set_xlim((0,time.values[-1]))
    ax1.set_ylim((0,1))
    # ax1.set_xlabel("time (min)")
    
def plot_cells(df_time_course, color_dict, ax):

    # Alive/Apoptotic/Necrotic vs Time
    for k in color_dict:
        ax.plot(df_time_course.time, df_time_course[k], "-", c=color_dict[k], label=k)
    
    # setting axes labels
    # ax.set_xlabel("time (min)")
    ax.set_ylabel("Nº of cells")
    
    # Showing legend
    ax.legend()
    ax.yaxis.grid(True)

def main():

    color_dict = {"live": "g", "apoptotic": "r", "necrotic":"k"}


    output_folder = sys.argv[1]

    mcds = MultiCellDS(output_folder=output_folder)

    df_time_course = mcds.get_cells_summary_frame()
    df_cell_variables = get_timeserie_mean(mcds)
    df_time_tnf = get_timeserie_density(mcds)

    # df_time_course.to_csv(instance_folder + "time_course.tsv", sep="\t")
    # df_cell_variables.to_csv(instance_folder + "cell_variables.tsv", sep="\t")
    # df_time_tnf.to_csv(instance_folder + "tnf_time.tsv", sep="\t")

    fig, axes = plt.subplots(3, 1, figsize=(12,12), dpi=150, sharex=True)
    plot_cells(df_time_course, color_dict, axes[0])
    
    list_of_variables = ['bound_external_TNFR', 'unbound_external_TNFR', 'bound_internal_TNFR']
    plot_molecular_model(df_cell_variables, list_of_variables, axes[1])
    threshold = 0.5
    
    axes[1].hlines(threshold, 0, df_time_course.time.iloc[-1], label="Activation threshold")
    ax2 = axes[1].twinx()
    ax2.plot(df_time_tnf.time, df_time_tnf['tnf'], 'r', label="[TNF]")
    ax2.set_ylabel("[TNF]")
    ax2.set_ylim([0, 1000])
    axes[1].legend(loc="upper left")
    ax2.legend(loc="upper right")

    list_of_variables = ['tnf_node', 'nfkb_node', 'fadd_node']
    plot_molecular_model(df_cell_variables, list_of_variables, axes[2])
    axes[2].set_xlabel("time (min)")
    ax2 = axes[2].twinx()
    ax2.plot(df_time_tnf.time, df_time_tnf['tnf'], 'r', label="[TNF]")
    ax2.set_ylabel("[TNF]")
    ax2.set_ylim([0, 1000])
    axes[2].legend(loc="upper left")
    ax2.legend(loc="upper right")

    fig.tight_layout()
    fig.savefig('variables_vs_time.png')

main()
