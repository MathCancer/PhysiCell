#
#  This Python script assumes you have run the beta/test_build_samples.sh script first.
#  See its header for instructions. That script will create folders in /user_projects.
#
#  Copy this Python script from /beta to the root dir if you want to edit it, then run it.
#  It will do the following:
#     - load each sample project (from user_projects; created by test_build_sample.sh)
#     - compile it
#     - modify each project's .xml:  max_time, # threads, output folder
#     - run it
#
#  WARNING: this is primarily intended to be used by core developers when testing a new release.
#            It will create new output directories  ("output_<proj>")
#
#    Run via (if you didn't copy it to the root dir and edit it):
#  $ python beta/test_run_samples.py 
#    or, pipe the terminal output to a file:
#  $ python beta/test_run_samples.py > test_run_samples.out
#
#  Any serious error should show up in your terminal output. Some "errors" may be benign, e.g.,
#   "Error: Could not find <cell_rules> section of XML config file"
#
#  Reminder:
# $ ls user_projects/
# biorobots/		hetero/			pred_prey/		worm/
# cancer_biorobots/	interaction/		rules/
# cancer_immune/		custom_division/  mechano/		template/
# celltypes3/		physimess/		virus_mac/

# Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample
#                  celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample
#                  worm-sample interaction-sample mechano-sample rules-sample physimess-sample custom-division-sample
#
#  It does not currently test running the intracellular projects:
# Sample intracellular projects: template_BM ode-energy-sample physiboss-cell-lines-sample
#                  cancer-metabolism-sample physiboss-tutorial physiboss-tutorial-invasion
#
#
#   If you want to cleanup the created execs:
# $ rm -rf template biorobots cancer_biorobots cancer_immune_3D celltypes3 hetero pred_prey virus_mac worm interaction mechano rules physimess custom_division

import subprocess
import xml.etree.ElementTree as ET
import os
import time

user_proj = ["template", "biorobots", "cancer_biorobots", "celltypes3", "hetero", "pred_prey", "virus_mac", "worm", "interaction", "mechano", "rules", "physimess", "custom_division"] 

model_execs = ["project", "biorobots", "cancer_biorobots", "celltypes3", "heterogeneity", "pred_prey", "virus-sample", "worm", "interaction_demo", "project", "project", "project", "project"]


# Using dummy max_time values of 99 for many projects; 61 for the more time-consuming cancer_immune_3D. 
# Users can change them as they wish.
max_times = [1440, 10, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 3600]

# if you want to include the 3D cancer_immune sample, uncomment these 3 lines
user_proj.append("cancer_immune") 
model_execs.append("cancer_immune_3D") 
max_times.append(61)

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