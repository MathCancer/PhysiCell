import subprocess
import xml.etree.ElementTree as ET
import os
import sys
import time

def run_sample(myexec, xml_file, max_time):

    print("\n\n------------   ",myexec, "   ----------------------------------")
    # update max_time and omp_num_threads (=1)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    root.find(".//max_time").text = str(max_time)
    root.find(".//omp_num_threads").text = "1"
    tree.write(xml_file)
    time.sleep(1)
    subprocess.run(["cat", xml_file])
    print(os.getcwd())
    cmd = [os.path.join(os.getcwd(), myexec), xml_file]
    print("Running: ", " ".join(cmd))
    res = subprocess.Popen(cmd, cwd=os.getcwd())
    res.wait()
    exit(res.returncode)

if __name__=="__main__":
    run_sample(*sys.argv[1:])
