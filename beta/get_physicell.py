# Python script to download the latest release of PhysiCell

import requests

response = requests.get("https://api.github.com/repos/MathCancer/PhysiCell/releases/latest")
release_name_str = response.json()["name"]
print(release_name_str)
print(release_name_str.split())
vnum = release_name_str.split()[1]
print("vnum=",vnum)  # e.g., vnum= 1.10.4

remote_url = 'https://github.com/MathCancer/PhysiCell/releases/download/' + vnum + '/PhysiCell_V.' + vnum + '.zip'
print("remote_url=",remote_url)
local_file = 'PhysiCell.zip'
data = requests.get(remote_url)
with open(local_file, 'wb')as file:
  file.write(data.content)
print("downloaded version ",vnum," to ",local_file)

