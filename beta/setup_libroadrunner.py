# This script attempts to download the libRoadrunner (binary) libraries and
# headers for your particular operating system. It installs them in 
# the appropriate /addons directory which the Makefile can find.
#
# Author: Randy Heiland

import platform
import urllib.request
import os
import sys
import zipfile

def reminder_dynamic_link_path_macos():
    print("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n")
    print("*    NOTE: if you have not yet done this, you need to specify where the shared libs can be found, e.g., via bash shell:")
    print('export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:./addons/libRoadrunner/roadrunner/lib')

    print("\n*      To make this permanent, add this line to the bottom of the respective shell startup file, e.g., .bashrc, .bash_profile, or .zshenv in your home directory.")
    print("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n")

def reminder_dynamic_link_path_linux():
    print("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n")
    print("*    NOTE: if you have not yet done this, you need to specify where the shared libs can be found, e.g., via bash shell:")
    print('export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./addons/libRoadrunner/roadrunner/lib')

    print("\n*      To make this permanent, add this line to the bottom of the respective shell startup file, e.g., .bashrc, .bash_profile, or .zshenv in your home directory.")
    print("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n")

os_type = platform.system()  # can test that it downloads correct OS .zip by overriding this value

# New: July 2023 - trying to be smarter about deciding whether to (re)download libRR
#  NOTE: needs to be tested cross-platform!
if os.path.exists(os.path.join(os.path.dirname(os.path.dirname(__file__)), "addons", "libRoadrunner", "roadrunner","include","rr","rrc_api.h")):
    print('\nNote: libroadrunner is already installed so we will not re-install it. \nIf you want to force a re-install, delete the "roadrunner" directory in /addons/libRoadrunner then re-compile.\n')

    # regardless, let's remind the user about the env var requirement!
    if os_type.lower() == 'darwin':
        reminder_dynamic_link_path_macos()
    elif os_type.lower().startswith("linux"):
        reminder_dynamic_link_path_linux()

else:
    print('\nThis model requires the libRoadrunner libraries which will now be downloaded.')
    print('(for your ',os_type, ' operating system)')

    # Assume Windows
    rr_file = ""
    url = ""

    mac_silicon = False
    if os_type.lower() == 'darwin':
        reminder_dynamic_link_path_macos()
        if "ARM64" in platform.uname().version:
            rr_file = "roadrunner_macos_arm64.zip"
            url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_macos_arm64.zip"
            mac_silicon = True
        else:
            rr_file = "roadrunner_macos_x86_64.zip"
            url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_macos_x86_64.zip"
    elif os_type.lower().startswith("win"):
        rr_file = "roadrunner_win_x86_64.zip"
        url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_win_x86_64.zip"
    elif os_type.lower().startswith("linux"):
        reminder_dynamic_link_path_linux()
        # the following "manylinux" seems to work on more flavors/releases of Linux than the roadrunner_ubuntu_24.zip which still exists on github.com/PhysiCell-Tools/intracellular_libs
        rr_file = "roadrunner_manylinux.zip"
        url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_manylinux.zip"
    else:
        print("Your operating system seems to be unsupported. Please create an issue on the PhysiCell GitHub repo or reach out on our Slack channel.")
        sys.exit(1)

    print("url=",url)
    fname = url.split('/')[-1]
    print("fname=",fname)

    print('libRoadRunner will now be installed into this location:')
    dir_name = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'addons', 'libRoadrunner')
    print(f'Will create {dir_name} if it does not exist.')
    # print('   - Press ENTER to confirm the location')
    # print('   - Press CTL-C to abort the installation')
    # print('   - Or specify a different location below\n')
    prompt_str = '[' + dir_name + '] >>> '
    try:
        # response = input(prompt_str)
        # if (response == ""):
        if (True):
            # print('got Enter')
            if not os.path.exists(dir_name):
                try:
                    os.makedirs(dir_name)
                except:
                    print('Error trying to create directory: ',dir_name)
                    exit(1)
        # else:
        #     print(type(response))
        #     dir_name = os.path.expanduser(response)
        #     if not os.path.exists(dir_name):
        #         try:
        #             os.makedirs(dir_name)
        #         except:
        #             print('Error trying to create directory: ',dir_name)
        #             exit(1)
    except:
        print('  installation canceled\n')
        exit(1)

    print('Beginning download of libroadrunner into ' + dir_name + ' ...')
    print(url)

    my_file = os.path.join(dir_name, fname)
    print('my_file = ',my_file)

    # rrlib_dir = my_file[:-4]   # assume all are .zip
    # print('rrlib_dir = ',rrlib_dir)

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

    new_dir_name = "roadrunner"
    print(f"chdir to {dir_name}")
    os.chdir(dir_name)
    print('installing (uncompressing) the file...')
    try:
        with zipfile.ZipFile(rr_file) as zf:
            zf.extractall('.')   # should create "roadrunner" directory
    except:
        print('error unzipping the file')
        exit(1)


    # Hack to maintain backwards compatibility. Originally our header path was .../include/rr/C, 
    # but newer releases of the libroadrunner C API seems to have dropped the "/C" directory and put everything in "/rr"
    dst =  "roadrunner/include/rr/C"
    try:
        os.remove(dst)
    except FileNotFoundError:
        print(f"File '{dst}' does not exist.")

    try:
        src =  "."
        os.symlink(src, dst)
        print(f"Symbolic link '{dst}' created, pointing to '{src}'")
    except FileExistsError:
        print(f"Symlink '{dst}' already exists")
    except OSError as e:
        print(f"Error creating symlink: {e}")
        print('Done.\n')
