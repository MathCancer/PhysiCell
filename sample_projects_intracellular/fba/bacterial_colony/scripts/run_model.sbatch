#!/bin/bash

PROGRAM="bacterial-colony"
JOB_NAME="pcdfba"

#SBATCH --chdir=.

#SBATCH --qos=gp_debug # gp_bscls
#SBATCH --job-name=${JOB_NAME}
#SBATCH --output=logs/${JOB_NAME}.%j.log
#SBATCH --error=logs/${JOB_NAME}.%j.err

#SBATCH --cpus-per-task=112
#SBATCH --ntasks=1
#SBATCH --time=1:59:00

date
./${PROGRAM}
date
