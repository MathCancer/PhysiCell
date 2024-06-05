#  Copy this file to the root dir and run it from a Unix bash shell: sh test_build_samples.sh
#
#  WARNING: this is primarily intended to be used by core developers when testing a new release.
#            It will create new directories in /user_projects
#
#  Based on the following:
#
# $ make list-projects
# Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample
#                  celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample
#                  worm-sample interaction-sample mechano-sample rules-sample physimess-sample
#
#  Note that it does not currently test building the intracellular projects:
# Sample intracellular projects: template_BM ode-energy-sample physiboss-cell-lines-sample
#                  cancer-metabolism-sample physiboss-tutorial physiboss-tutorial-invasion
#
#
#  After running the script, you can clean up with:
# $ rm template biorobots cancer_biorobots cancer_immune_3D celltypes3 heterogeneity pred_prey virus-sample worm interaction_demo mechano rules-sample physimess-sample 
#   and then:
# $ cd user_projects
# $ rm -rf template biorobots cancer_biorobots cancer_immune celltypes3 hetero pred_prey virus_mac worm interaction mechano rules physimess

make reset
make template
make 
mv project template
make save PROJ=template
#
make reset
make biorobots-sample 
make
make save PROJ=biorobots
#
make reset
make cancer-biorobots-sample
make
make save PROJ=cancer_biorobots
#
make reset
make cancer-immune-sample
make
make save PROJ=cancer_immune
#
make reset
make celltypes3-sample
make
make save PROJ=celltypes3
#
make reset
make heterogeneity-sample
make
make save PROJ=hetero
#
make reset
make  pred-prey-farmer
make
make save PROJ=pred_prey
#
make reset
make  virus-macrophage-sample
make
make save PROJ=virus_mac
#
make reset
make worm-sample
make
make save PROJ=worm
#
make reset
make interaction-sample
make
make save PROJ=interaction
#
make reset
make mechano-sample
make
mv project mechano-sample
make save PROJ=mechano
#
make reset
make rules-sample
make
mv project rules-sample
make save PROJ=rules
#
make reset
make physimess-sample
make
mv project physimess-sample
make save PROJ=physimess
