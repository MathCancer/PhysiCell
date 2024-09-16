import subprocess
import xml.etree.ElementTree as ET
import os
import sys
import time

def make_cellcycle_deterministic(root):
    for phase_transition_rates in root.findall(".//phase_transition_rates"):
        for rate in phase_transition_rates.iter():
            if rate.tag == "rate":
                rate.attrib["fixed_duration"] = "true"

def make_deathmodel_deterministic(root):
    for phase_durations in root.findall(".//phase_durations"):
        for duration in phase_durations.iter():
            if duration.tag == "duration":
                duration.attrib["fixed_duration"] = "true"

def make_chemotaxis_deterministic(root):
    for migration_bias in root.findall(".//migration_bias"):
        migration_bias.text = "1.0"
        
    m_dt = float(root.find(".//dt_mechanics").text) * 0.5
    for persistence_time in root.findall(".//persistence_time"):
        persistence_time.text = str(m_dt)
        
def disable_automated_spring_adhesions(root):
    spring_adhesion = root.find(".//disable_automated_spring_adhesions")
    if spring_adhesion is not None:
        spring_adhesion.text = "true"

def run_sample(myexec, xml_file, max_time):

    print("\n\n------------   ",myexec, "   ----------------------------------")
    # update max_time and omp_num_threads (=1)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    root.find(".//max_time").text = str(max_time)
    
    root.find(".//omp_num_threads").text = "1"
    make_cellcycle_deterministic(root)
    make_deathmodel_deterministic(root)
    make_chemotaxis_deterministic(root)   
    disable_automated_spring_adhesions(root)
    
    tree.write(xml_file)
    subprocess.run(["cat", xml_file])
    cmd = [os.path.join(os.getcwd(), myexec), xml_file]
    print("Running: ", " ".join(cmd))
    res = subprocess.Popen(cmd, cwd=os.getcwd())
    res.wait()
    exit(res.returncode)

if __name__=="__main__":
    run_sample(*sys.argv[1:])
