import subprocess
import xml.etree.ElementTree as ET
import os
import time

user_proj = ["biorobots-sample", "physiboss-tnf-model"] 

model_execs = ["./biorobots", "./spheroid_TNF_model"]


# Using dummy max_time values of 99 for many projects; 61 for the more time-consuming cancer_immune_3D. 
# Users can change them as they wish.
max_times = [20, 1440]

for (uproj, myexec, max_time) in zip(user_proj, model_execs, max_times):
    times = [max_time/2, max_time]
    for current_time in times:
        print("\n\n------------   ",uproj,myexec, "   ----------------------------------")
        if current_time == max_time:
            # execute everything but do not reload the project
            tree = ET.parse('config/PhysiCell_settings.xml')
            root = tree.getroot()
            root.find(".//max_time").text = str(current_time)
            root.find(".//omp_num_threads").text = "1"
            new_output_dir = "output_" + uproj
            root.find(".//save//folder").text = new_output_dir
            root.find(".//user_parameters//start_stop").text = "true"

            # set the start and stop to true for the second execution
            tree.write('./config/PhysiCell_settings.xml')
            try:
                os.makedirs(new_output_dir)
            except:
                pass
            time.sleep(1)

            subprocess.run([myexec])
        else:
            subprocess.run(["make","reset"])
            subprocess.run(["make","clean"])
            # make load PROJ=template
            subprocess.run(["make",uproj])
            subprocess.run(["make"])
            # update max_time and omp_num_threads (=1)
            tree = ET.parse('config/PhysiCell_settings.xml')
            root = tree.getroot()
            root.find(".//max_time").text = str(current_time)
            root.find(".//omp_num_threads").text = "1"
            new_output_dir = "output_" + uproj
            root.find(".//save//folder").text = new_output_dir
            root.find(".//user_parameters//start_stop").text = "false"

            # set the start and stop to true for the second execution
            tree.write('./config/PhysiCell_settings.xml')
            try:
                os.makedirs(new_output_dir)
            except:
                pass
            time.sleep(1)

            subprocess.run([myexec])