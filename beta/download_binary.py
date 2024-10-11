# This script attempts to download the MaBoSS (binary) library(s) and
# headers for your particular operating system. It installs them in a standard
# location (relative to a PhysiCell installation) which will be used by a PhysiCell Makefile.
#
# Authors: Randy Heiland, Vincent Noel

import platform
import urllib.request
import os
import sys
import tarfile
import stat
import shutil

physicell_version = "1.14.1"
repo_physicell = "MathCancer/PhysiCell"
physiboss_version = "v2.2.3"
repo_physiboss = "PhysiBoSS/PhysiBoSS"
list_models = {
    "physiboss-tutorial": "https://github.com/" + repo_physiboss + "/releases/download/" + physiboss_version + "/", 
    "physiboss-tutorial-invasion": "https://github.com/" + repo_physiboss + "/releases/download/" + physiboss_version + "/", 
    "physiboss-cell-lines": "https://github.com/" + repo_physiboss + "/releases/download/" + physiboss_version + "/",   
    "template_BM": "https://github.com/" + repo_physiboss + "/releases/download/" + physiboss_version + "/", 
    "template": "https://github.com/" + repo_physicell + "/releases/download/" + physicell_version + "/", 
    "rules": "https://github.com/" + repo_physicell + "/releases/download/" + physicell_version + "/", 
    "physimess": "https://github.com/" + repo_physicell + "/releases/download/" + physicell_version + "/", 
    "interaction": "https://github.com/" + repo_physicell + "/releases/download/" + physicell_version + "/", 
}

def print_usage():
     print("Usage : python download_binary.py <model>")
     print("")
     print("Models available : %s" % (",".join(list_models.keys())))

if len(sys.argv) < 2:
     print_usage()
     exit(1)
     
model = sys.argv[1]
     
if model in ["-h", "--help"] or model not in list_models.keys():
     print_usage()
     exit(1)

print('> Chosen model: ', model)

os_type = platform.system()
print('> Operating system: ', os_type)
     
mb_file = ""
if os_type.lower() == 'darwin':
    mb_file = model + "-macos.tar.gz"
elif os_type.lower().startswith("win") or os_type.lower().startswith("msys_nt") or os_type.lower().startswith("mingw64_nt"):
    mb_file = model + "-win.tar.gz"
elif os_type.lower().startswith("linux"):
    mb_file = model + "-linux.tar.gz"
else:
    print("Your operating system seems to be unsupported. Please create an new issue at https://github.com/PhysiBoSS/PhysiBoSS/issues/ ")
    sys.exit(1)

url = list_models[model] + mb_file
print('> Downloading from: ', url)

dir_name = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
print('> Loading into directory: ', dir_name)

my_file = os.path.join(dir_name, mb_file)
print('> File: ',my_file)

def download_cb(blocknum, blocksize, totalsize):
    readsofar = blocknum * blocksize
    if totalsize > 0:
        percent = readsofar * 1e2 / totalsize
        s = "\r%5.1f%% %*d / %d" % (
            percent, len(str(totalsize)), readsofar, totalsize)
        sys.stderr.write(s)
        if readsofar >= totalsize: # near the end
            sys.stderr.write("\n")
    else: # total size is unknown
        sys.stderr.write("read %d\n" % (readsofar,))

urllib.request.urlretrieve(url, my_file, download_cb)

print('> Creating backup of XML settings, Makefile, main.cpp')
if os.path.exists("Makefile"):
    shutil.copyfile("Makefile", "Makefile-backup")
if os.path.exists("main.cpp"):
    shutil.copyfile("main.cpp", "main-backup.cpp")
if os.path.exists(os.path.join("config", "PhysiCell_settings.xml")):
    shutil.copyfile(os.path.join("config", "PhysiCell_settings.xml"), os.path.join("PhysiCell_settings-backup.xml"))
    
os.chdir(dir_name)
print('> Uncompressing the model')

try:
    tar = tarfile.open(mb_file)
    tar.extractall()
    binary_name = [names for names in tar.getnames() if not names.endswith(".dll")][0]
    tar.close()
    os.remove(mb_file)

except:
    print('! Error untarring the file')
    exit(1)

new_name = "project" if not mb_file.endswith("win.tar.gz") else "project.exe"
os.rename(binary_name, new_name)
st = os.stat(new_name)
os.chmod(new_name, st.st_mode | stat.S_IEXEC)

print('> Done. You can now run %s\n' % new_name)