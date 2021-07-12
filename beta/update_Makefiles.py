# Utility script to add a command to the 'reset' block of all sample Makefiles. Run from root.
#
import glob,os
#os.chdir(".")
for file in glob.glob("sample_projects/*/Makefile"):
    print(file)
    # gsed -e "/touch ALL_CITATIONS.txt/a \\\ttouch ./core/PhysiCell_cell.cpp" <file >tmp
    cmd = 'gsed -e "/touch ALL_CITATIONS.txt/a \\\ttouch ./core/PhysiCell_cell.cpp" < ' + file + ' >tmp'
    print(cmd)
    os.system(cmd)
    cmd = 'cat tmp > ' + file
    print(cmd)
    os.system(cmd)

for file in glob.glob("sample_projects_intracellular/*/*/Makefile"):
    print(file)
    cmd = 'gsed -e "/touch ALL_CITATIONS.txt/a \\\ttouch ./core/PhysiCell_cell.cpp" < ' + file + ' >tmp'
    print(cmd)
    os.system(cmd)
    cmd = 'cat tmp > ' + file
    print(cmd)
    os.system(cmd)
