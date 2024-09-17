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

if os.path.exists(os.path.join(os.path.dirname(__file__), "MaBoSS")):
    print('libMaBoSS already installed')

else:
    os_type = platform.system()
    print('operating system = ',os_type)

    # Assume Windows
    mb_file = ""
    url = ""
    maboss_version = "v2.5.6"
    if os_type.lower() == 'darwin':
        if "ARM64" in platform.uname().version:
            mb_file = "libMaBoSS-macos-arm64.tar.gz"
        else:
            mb_file = "libMaBoSS-osx64.tar.gz"
    elif os_type.lower().startswith("win") or os_type.lower().startswith("msys_nt") or os_type.lower().startswith("mingw64_nt"):
        mb_file = "libMaBoSS-win64.tar.gz"
    elif os_type.lower().startswith("linux"):
        mb_file = "libMaBoSS-linux64.tar.gz"
    else:
        print("Your operating system seems to be unsupported. Please submit a ticket at https://sourceforge.net/p/physicell/tickets/ ")
        sys.exit(1)

    url = "https://github.com/sysbio-curie/MaBoSS/releases/download/" + maboss_version + "/" + mb_file

    print('libMaBoSS will now be installed into the addon PhysiBoSS addon folder:')
    dir_name = os.path.dirname(__file__)
    print(dir_name + '\n')
    
    if not os.path.exists(dir_name):
                try:
                    os.makedirs(dir_name)
                except:
                    print('Error trying to create directory: ',dir_name)
                    exit(1)
    print('Beginning download of libMaBoSS into ' + dir_name + ' ...')
    print(url)

    my_file = os.path.join(dir_name, mb_file)
    print('my_file = ',my_file)

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

    os.chdir(dir_name)
    print('installing (uncompressing) the file...')
    
    try:
        tar = tarfile.open(mb_file)
        tar.extractall()
        tar.close()
        os.remove(mb_file)
    except:
        print('error untarring the file')
        exit(1)

    print('Done.\n')
