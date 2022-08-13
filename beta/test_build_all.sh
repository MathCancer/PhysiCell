# Copy this file to the root dir and run it from a bash shell: sh test_build_all.sh
# (created from the following info)
#
# $ make list-projects
# Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample
#                  celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample
#                  worm-sample interaction-sample
#
# Sample intracellular projects: ode-energy-sample physiboss-cell-lines-sample cancer-metabolism-sample
#

make reset
make template
make -j2
make reset
make biorobots-sample 
make
make reset
make cancer-biorobots-sample
make
make reset
make cancer-immune-sample
make
make reset
make celltypes3-sample
make
make reset
make heterogeneity-sample
make
make reset
make  pred-prey-farmer
make
make reset
make  virus-macrophage-sample
make
make reset
make worm-sample
make
make reset
make  interaction-sample
make

# now the intracellular models, requiring additional libs
make reset
make ode-energy-sample 
make
make reset
make physiboss-cell-lines-sample cancer-metabolism-sample
make
make reset
make cancer-metabolism-sample
make
