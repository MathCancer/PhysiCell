# This script attempts to download the libRoadrunner (binary) libraries and
# headers for your particular operating system. It installs them in 
# the appropriate /addons directory which the Makefile can find.
#
# Author: Randy Heiland

import platform
import urllib.request
import os
import sys
import tarfile
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

os_type = platform.system()

# Old:
# if os.path.exists(os.path.join(os.path.dirname(os.path.dirname(__file__)), "addons", "libRoadrunner", "roadrunner")):

# New: July 2023 - trying to be smarter about deciding whether to (re)download libRR
#  NOTE: needs to be tested cross-platform!
if os.path.exists(os.path.join(os.path.dirname(os.path.dirname(__file__)), "addons", "libRoadrunner", "roadrunner","include","rr","C","rrc_api.h")):
    print('\nlibroadrunner already installed.\n')

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
            # pass
            # print('... for the arm64 processor.')
            # url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/libs/macos12_arm64/libroadrunner_c_api.dylib"
            rr_file = "roadrunner_macos_arm64.tar.gz"
            url = "https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_macos_arm64.tar.gz"
            mac_silicon = True
        else:
            rr_file = "roadrunner-osx-10.9-cp36m.tar.gz"
            url = "https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.4.18/" + rr_file + "/download"
    elif os_type.lower().startswith("win"):
        rr_file = "roadrunner-win64-vs14-cp35m.zip"
        url = "https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.4.18/" + rr_file + "/download"
    elif os_type.lower().startswith("linux"):
        reminder_dynamic_link_path_linux()
        rr_file = "cpplibroadrunner-1.3.0-linux_x86_64.tar.gz"
        url = "https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.3/" + rr_file + "/download"
    else:
        print("Your operating system seems to be unsupported. Please submit a ticket at https://sourceforge.net/p/physicell/tickets/ ")
        sys.exit(1)

    print("url=",url)
    if mac_silicon:
        fname = url.split('/')[-1]
    else:
        fname = url.split('/')[-2]
    print("fname=",fname)

    # home = os.path.expanduser("~")
    print('libRoadRunner will now be installed into this location:')
    # dir_name = os.path.join(home, 'libroadrunner')
    dir_name = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'addons', 'libRoadrunner')
    print(dir_name + '\n')
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

    if os_type.lower().startswith("win"):
        rrlib_dir = my_file[:-4]
    else:  # darwin or linux
        if mac_silicon:
            # idx_end = my_file.rindex('/')
            # rrlib_dir = my_file[:idx_end]
            rrlib_dir = my_file[:-7]
            # rrlib_dir = my_file
        else:
            rrlib_dir = my_file[:-7]
    print('rrlib_dir = ',rrlib_dir)

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
    os.chdir(dir_name)
    print('installing (uncompressing) the file...')
    if os_type.lower().startswith("win"):
        try:
            with zipfile.ZipFile(rr_file) as zf:
                zf.extractall('.')
            os.rename("roadrunner-win64-vs14-cp35m", new_dir_name)
        except:
            print('error unzipping the file')
            exit(1)
    else:  # Darwin or Linux
        try:
            print("untarring ",rr_file)
            tar = tarfile.open(rr_file)
            tar.extractall()
            tar.close()
            if 'darwin' in os_type.lower():
                if mac_silicon:
                    os.rename("roadrunner_macos_arm64", new_dir_name)
                else:
                    os.rename("roadrunner-osx-10.9-cp36m", new_dir_name)
            else:
                os.rename("libroadrunner", new_dir_name)
        except:
            if mac_silicon:
                print()
                # pass
            else:
                print('error untarring the file')
                exit(1)

    print('Done.\n')

    # # LIBRR_DIR := /Users/heiland/libroadrunner/roadrunner-osx-10.9-cp36m
    # print("Replace the following variables in your PhysiCell Makefile with these:\n")
    # #print("LIBRR_DIR := /Users/heiland/libroadrunner/roadrunner-osx-10.9-cp36m")
    # print("LIBRR_DIR := " + rrlib_dir)
    # if os_type == 'Windows':
    #     print("LIBRR_LIBS := " + rrlib_dir + "/bin\n")
    # else:
    #     print("LIBRR_LIBS := " + rrlib_dir + "/lib\n")
