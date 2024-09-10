#  Copy this file to the root dir and run it from a Unix bash shell: sh test_build_samples.sh
#
#  WARNING: this is primarily intended to be used by core developers when testing a new release.
#            It will create new directories in /user_projects
#
#  This script is intended to serve as a first step in a two-step process. The second step is
#  to run the beta/test_run_samples.py Python script (see its header for instructions).
#
#
#  This test_build_samples.sh script is based on the following:
#
# $ make list-projects
# Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample
#                  celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample
#                  worm-sample interaction-sample mechano-sample rules-sample physimess-sample custom-division-sample
#
#  Note that it does not currently test building the intracellular projects:
# Sample intracellular projects: template_BM ode-energy-sample physiboss-cell-lines-sample
#                  cancer-metabolism-sample physiboss-tutorial physiboss-tutorial-invasion
#
#  After running the script, you can:
#    1) confirm all executables were created
# $ ls -l template_sample biorobots cancer_biorobots cancer_immune_3D celltypes3 heterogeneity pred_prey virus-sample worm interaction_demo mechano_sample rules_sample physimess_sample custom_division_sample
#   and then:
#    2) confirm the user_projects were created; delete them
# $ cd user_projects
# $ ls
# $ rm -rf template biorobots cancer_biorobots cancer_immune celltypes3 hetero pred_prey virus_mac worm interaction mechano rules physimess custom_division

make reset
make template
make 
mv project template_sample
make save PROJ=template
#
echo "\n-----------------------------------------"
make reset
make biorobots-sample 
make
make save PROJ=biorobots
#
echo "\n-----------------------------------------"
make reset
make cancer-biorobots-sample
make
make save PROJ=cancer_biorobots
#
echo "\n-----------------------------------------"
make reset
make cancer-immune-sample
make
make save PROJ=cancer_immune
#
echo "\n-----------------------------------------"
make reset
make celltypes3-sample
make
make save PROJ=celltypes3
#
echo "\n-----------------------------------------"
make reset
make heterogeneity-sample
make
make save PROJ=hetero
#
echo "\n-----------------------------------------"
make reset
make  pred-prey-farmer
make
make save PROJ=pred_prey
#
echo "\n-----------------------------------------"
make reset
make  virus-macrophage-sample
make
make save PROJ=virus_mac
#
echo "\n-----------------------------------------"
make reset
make worm-sample
make
make save PROJ=worm
#
echo "\n-----------------------------------------"
make reset
make interaction-sample
make
make save PROJ=interaction
#
echo "\n-----------------------------------------"
make reset
make mechano-sample
make
mv project mechano_sample
make save PROJ=mechano
#
echo "\n-----------------------------------------"
make reset
make rules-sample
make
mv project rules_sample
make save PROJ=rules
#
echo "\n-----------------------------------------"
make reset
make physimess-sample
make
mv project physimess_sample
make save PROJ=physimess
#
echo "\n-----------------------------------------"
make reset
make custom-division-sample
make
mv project custom_division_sample
make save PROJ=custom_division
