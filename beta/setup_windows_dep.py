# This script attempts to download the windows mingw64 libraries which cannot be statically 
# included in the binary, and puts them in the root folder of PhysiCell
#
# Authors: Vincent Noel

import urllib.request
import os
import sys
import tarfile
from pathlib import Path
import tempfile
import zstandard  
import shutil

if zstandard is None:
            raise ImportError("pip install zstandard")

libs = {
    "https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-gcc-libs-13.2.0-3-any.pkg.tar.zst": [
        "mingw64/bin/libgcc_s_seh-1.dll", "mingw64/bin/libgomp-1.dll"
    ],
    "https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-libwinpthread-git-11.0.0.r551.g86a5e0f41-1-any.pkg.tar.zst": [
        "mingw64/bin/libwinpthread-1.dll"
    ]
}

for url, files in libs.items():

    fname = url.split("/")[-1]

    with tempfile.TemporaryDirectory() as temp_dir:

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
        
        urllib.request.urlretrieve(url, os.path.join(temp_dir, fname), download_cb)
            
        
        archive = Path(os.path.join(temp_dir, fname))
        out_path = Path(temp_dir)

        dctx = zstandard.ZstdDecompressor()

        with tempfile.TemporaryFile(suffix=".tar") as ofh:
            with archive.open("rb") as ifh:
                dctx.copy_stream(ifh, ofh)
            ofh.seek(0)
            with tarfile.open(fileobj=ofh) as z:
                z.extractall(out_path)
    
        for file_path in files:
            shutil.copyfile(os.path.join(out_path, file_path), os.path.join(os.getcwd(), file_path.split("/")[-1]))

print('Done.\n')
