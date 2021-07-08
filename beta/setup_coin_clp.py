#!/usr/bin/env python3
# coding: utf-8

import sys, os
import json
import argparse

import requests
import hashlib
import tarfile
import zipfile

def param_parser():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument('--pkg', dest="pkg", required=True, help='Available packages', choices=PACKAGES)
    parser.add_argument('--arch', dest="arch", required=True, choices=ARCHS, help='Current arch')
    parser.add_argument('--checksum', dest="checksum", default=False, help='Check file integrity after downloading')
    parser.add_argument('--path', dest='path', default='ext', help='Default folder destination to install third-party libs if changed, Makefiles you be updated according')
    return parser


PACKAGES = ("coin-or", "libsbml")
ARCHS = ("linux-x64", "linux-x86", "win64", "win32", "osx")

SBML_DIR = "libsbml"
    

def main():
    parser = param_parser()
    args = parser.parse_args()

    # json_packages = os.path.join(os.curdir, 'config')
    # json_packages = os.path.join(json_packages, 'packages.json')
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
        
    pkg_dict = packages_dict[args.pkg][args.arch]

    print("Fetching package:", end="")
    print("- %s (%s)" % (args.pkg, args.arch))
    print("Downaling from: %s" % pkg_dict['url'])
    r = requests.get(pkg_dict['url'], allow_redirects=True)

    # if args.checksum:
    #     print("Package cheksum(sha256)", end=" ")
    #     hash_strn = hashlib.sha256(r.content).hexdigest()
    #     assert pkg_dict['sha256'] == hash_strn
    #     print("Ok!")

    
    fname = pkg_dict["version"]
    with open(fname, 'wb') as fh:
        fh.write(r.content)

    if args.pkg == 'libsbml':
        print("Extracting package in %s... " % args.pkg, end=" ")
        if fname.endswith("zip"):
            archiver = zipfile.ZipFile(fname, 'r')
            archiver.extractall()
        elif fname.endswith("tar.gz") or fname.endswith("gz"):
            archiver = tarfile.open(fname, "r:gz")
            archiver.extractall()
            old_lib_dir = os.path.commonprefix(archiver.getnames())
            archiver.close()
            os.rename(old_lib_dir, SBML_DIR)
            if os.path.exists(old_lib_dir):
                os.rmdir(old_lib_dir)
            os.remove(fname)
    
    elif args.pkg == "coin-or":
        print("Extracting package in %s... " % args.pkg, end=" ")
        if fname.endswith("zip"):
            archiver = zipfile.ZipFile(fname, 'r')
            archiver.extractall()
        elif fname.endswith("tar.gz") or fname.endswith("gz"):
            archiver = tarfile.open(fname, "r:gz")
            archiver.extractall(path=args.pkg)
            archiver.close()
            os.remove(fname)
    
    print("Ok!")
    print("Dependency retrived correctly :-)\n")

if __name__ == "__main__":
    main()