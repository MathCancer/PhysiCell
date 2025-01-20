# Python script to download the latest release of PhysiCell Studio. It will create a /studio directory 
# from wherever this script is run. Sample commands to run the Studio will be printed.

import requests
import os
import zipfile
import shutil

response = requests.get("https://api.github.com/repos/PhysiCell-Tools/PhysiCell-Studio/releases/latest")
release_name_str = response.json()["name"]
print(release_name_str)
print(release_name_str.split())
vnum = release_name_str.split()[0]
print("vnum=",vnum)  # e.g., vnum= v2.26.7


try:
    os.system('rm -rf studio')
except:
    pass

# PhysiCell-Studio-2.26.7
remote_url = 'https://github.com/PhysiCell-Tools/PhysiCell-Studio/archive/refs/tags/' + vnum + '.zip'
print("remote_url=",remote_url)
local_file = 'Studio.zip'
data = requests.get(remote_url)
with open(local_file, 'wb')as file:
   file.write(data.content)
print("downloaded version ",vnum," to ",local_file)

print("unzip-ing ",local_file)
with zipfile.ZipFile(local_file, 'r') as zip_ref:
    zip_ref.extractall(".")

latest_studio_dir = "PhysiCell-Studio-" + vnum[1:]  # need to drop the 'v' as first character
shutil.move(latest_studio_dir,"studio")
print(f"renamed '{latest_studio_dir}' to 'studio'")

print("\n-- Sample commands to run the Studio:")
print("python studio/bin/studio.py     # attempts to load config/PhysiCell_settings.xml")
print("python studio/bin/studio.py -c <config_file.xml> -e <executable_program>")
print("python studio/bin/studio.py --help")
print()