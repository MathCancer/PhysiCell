# This script provides simple parameter exploration functionality. The script creates
# a new folder (subdirectory) for each set of parameters, makes changes to a default 
# configuration (.xml) file using specified parameter values (in an accompanying .txt file),
# copies the new config file into the new folder, then
# runs the simulation (in the background) which writes results into the new folder.
# 

import xml.etree.ElementTree as ET
from shutil import copyfile
import os
import sys
import subprocess

# print(len(sys.argv))
if (len(sys.argv) < 3):
  usage_str = "Usage: %s <exec_pgm> <params.txt>" % (sys.argv[0])
  print(usage_str)
  print("e.g.:  python params_run.py biorobots params_biorobots.txt")
  exit(1)
else:
   exec_pgm = sys.argv[1]
   params_file = sys.argv[2]

background_str = " &"  # works on Unix
if sys.platform == 'win32':
    background_str = ""


xml_file_in = 'config/PhysiCell_settings.xml'
xml_file_out = 'config/tmp.xml'
copyfile(xml_file_in, xml_file_out)
tree = ET.parse(xml_file_out)
xml_root = tree.getroot()
first_time = True
output_dirs = []
with open(params_file) as f:
    for line in f:
        # print(len(line),line)
        print(line, end="")
        if (line[0] == '#'):
            continue
        (key, val) = line.split()
        if (key == 'run_it'):
            # write the config file to the previous folder (output) dir and start a simulation
            # print('---write config file and start its sim')
            print('---write config file (and start sim): ', xml_file_out)
            tree.write(xml_file_out)   # will create folder_name/config.xml
            log_file = folder_name + ".log"  
            cmd =  exec_pgm + " " + xml_file_out + " > " + log_file + " " + background_str
            print("----- cmd = ",cmd)
            # os.system(cmd)   # <------ Execute the simulation
            # subprocess.Popen([exec_pgm, xml_file_out])
            with open(log_file,"w") as outf:
                subprocess.Popen([exec_pgm, xml_file_out],stdout=outf)
        elif ('.' in key):
            k = key.split('.')
            uep = xml_root
            for idx in range(len(k)):
                uep = uep.find('.//' + k[idx])  # unique entry point (uep) into xml
#                print(k[idx])
            uep.text = val
        else:
            if (key == 'folder'):
                folder_name = val
                output_dirs.append(folder_name)
                if (not os.path.exists(folder_name)):
                    print("--- parsed 'folder', makedir " + folder_name)
                    os.makedirs(folder_name)
                # xml_file_out = folder_name + '/config.xml'  # copy config file into the output dir
                xml_file_out = os.path.join(folder_name, 'config.xml')  # copy config file into the output dir

            try:
                xml_root.find('.//' + key).text = val
            except:
                print("--- Error: could not find ",key," in .xml\n")
                sys.exit(1)

print("\n ------\n Your output results will appear in these directories:\n   ",output_dirs)
print("and check for a .log file of each name for your terminal output from each simulation.\n")
