#  Copy this file to the root dir and run it to:
#     - load each sample project (from user_projects)
#     - compile it
#     - modify each one's .xml:  max_time, # threads, output folder
#     - run it
#
#  WARNING: this is primarily intended to be used by core developers when testing a new release.
#            It will create new output directories  ("output_<proj>")
#
#  Reminder:
# $ ls user_projects/
# biorobots/		hetero/			pred_prey/		worm/
# cancer_biorobots/	interaction/		rules/
# cancer_immune/		mechano/		template/
# celltypes3/		physimess/		virus_mac/

# Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample
#                  celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample
#                  worm-sample interaction-sample mechano-sample rules-sample physimess-sample
#
#  It does not currently test running the intracellular projects:
# Sample intracellular projects: template_BM ode-energy-sample physiboss-cell-lines-sample
#                  cancer-metabolism-sample physiboss-tutorial physiboss-tutorial-invasion
#
#
#   If you want to cleanup the created execs:
# $ rm -rf template biorobots cancer_biorobots cancer_immune celltypes3 hetero pred_prey virus_mac worm interaction mechano rules physimess

import subprocess
import xml.etree.ElementTree as ET
import os
import time

user_proj = ["template", "biorobots", "cancer_biorobots", "celltypes3", "hetero", "pred_prey", "virus_mac", "worm", "interaction", "mechano", "rules", "physimess"]  # 
model_execs = ["project", "biorobots", "cancer_biorobots", "celltypes3", "heterogeneity", "pred_prey", "virus-sample", "worm", "interaction_demo", "project", "project", "project"]

# $ rm -rf template biorobots cancer_biorobotscancer_immune celltypes3 hetero pred_prey virus_mac worm interaction mechano rules physimess 
max_times = [120, 10, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99]

for (uproj, myexec, max_time) in zip(user_proj, model_execs, max_times):
    print("\n\n------------   ",uproj,myexec, "   ----------------------------------")
    subprocess.run(["make","reset"])
    # make load PROJ=template
    s= "PROJ=" + uproj
    print("\n ---- doing: make load ",s)
    subprocess.run(["make","load",s])
    subprocess.run(["make"])

    # update max_time and omp_num_threads (=1)
    tree = ET.parse('config/PhysiCell_settings.xml')
    root = tree.getroot()
    root.find(".//max_time").text = str(max_time)
    root.find(".//omp_num_threads").text = "1"
    new_output_dir = "output_" + uproj
    root.find(".//save//folder").text = new_output_dir
    tree.write('config/PhysiCell_settings.xml')
    try:
        os.makedirs(new_output_dir)
    except:
        pass
    time.sleep(1)

    subprocess.run([myexec])

# 
# echo "\n-----------------------------------------"
# make reset
# make load PROJ=biorobots
# make
#
# echo "\n-----------------------------------------"
# make reset
# make cancer-biorobots-sample
# make
# make save PROJ=cancer_biorobots
# #
# echo "\n-----------------------------------------"
# make reset
# make cancer-immune-sample
# make
# make save PROJ=cancer_immune
# #
# echo "\n-----------------------------------------"
# make reset
# make celltypes3-sample
# make
# make save PROJ=celltypes3
# #
# echo "\n-----------------------------------------"
# make reset
# make heterogeneity-sample
# make
# make save PROJ=hetero
# #
# echo "\n-----------------------------------------"
# make reset
# make  pred-prey-farmer
# make
# make save PROJ=pred_prey
# #
# echo "\n-----------------------------------------"
# make reset
# make  virus-macrophage-sample
# make
# make save PROJ=virus_mac
# #
# echo "\n-----------------------------------------"
# make reset
# make worm-sample
# make
# make save PROJ=worm
# #
# echo "\n-----------------------------------------"
# make reset
# make interaction-sample
# make
# make save PROJ=interaction
# #
# echo "\n-----------------------------------------"
# make reset
# make mechano-sample
# make
# mv project mechano_sample
# make save PROJ=mechano
# #
# echo "\n-----------------------------------------"
# make reset
# make rules-sample
# make
# mv project rules_sample
# make save PROJ=rules
# #
# echo "\n-----------------------------------------"
# make reset
# make physimess-sample
# make
# mv project physimess_sample
# make save PROJ=physimess
