#!/usr/bin/env python3
# coding: utf-8

import sys, os
import json
import argparse
import platform
import urllib.request
import zipfile

def param_parser():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument('--pkg', dest="pkg", required=True, help='Available packages', choices=PACKAGES)
    parser.add_argument('--path', dest='path', default='addons/dFBA/ext', help='Default folder destination to install third-party libs if changed, Makefiles you be updated according')
    return parser


PACKAGES = ("coin-or", "libsbml")
    

def main():
    parser = param_parser()
    args = parser.parse_args()
    os_type = platform.system()

    if os.path.exists(os.path.join(os.path.dirname(os.path.dirname(__file__)), "addons", "dFBA", "ext", args.pkg)):
        print('\n%s already installed.\n' % args.pkg)
        return

    json_packages = os.path.join('beta', 'fba_packages.json')

    packages_dict = {}
    with open(json_packages) as fh:
        packages_dict = json.load(fh)

    if not os.path.exists(args.path):
        print("Creating %s folder" % args.path, end=" ")
        os.makedirs(args.path)
        print("Ok!")

    print("Moving to %s folder" % args.path)
    os.chdir(args.path)
      
    arch = None  
    if os_type.lower() == 'darwin':
        arch = "osx"
    elif os_type.lower().startswith("win"):
        arch = "win64"
    elif os_type.lower().startswith("linux"):
        arch = "linux-x64"
    else:
        print("OS not supported !")
        return
            
    pkg_dict = packages_dict[args.pkg][arch]

    print("Fetching package:", end="")
    print("- %s (%s)" % (args.pkg, arch))
    print("Downaling from: %s" % pkg_dict['url'])
    
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

    urllib.request.urlretrieve(pkg_dict['url'], pkg_dict['version'], download_cb)

    fname = pkg_dict["version"]
    
    print("Extracting package in %s... " % args.pkg, end=" ")
    if fname.endswith("zip"):
        archiver = zipfile.ZipFile(fname, 'r')
        archiver.extractall(args.pkg)
    
    print("Ok!")
    print("Dependency retrived correctly :-)\n")

if __name__ == "__main__":
    main()