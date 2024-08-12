# compare diffs between .svg files

import os
import sys
import glob
import subprocess

if (len(sys.argv) < 3):
  usage_str = "Usage: %s <dir1> <dir2>" % (sys.argv[0])
  print(usage_str)
  print("e.g.:  python test_diff_svg.py ~/blah1 ~/blah2")
  exit(1)
else:
   dir1 = sys.argv[1]
   dir2 = sys.argv[2]

svg_files = glob.glob(f'{dir1}/snap*.svg')
svg_files.sort()
if len(svg_files) == 0:
   print("No svg files found in ",dir1)
   exit(1)
for filename in svg_files:
   f = os.path.basename(filename)
   f1 = os.path.join(dir1,f)
   f2 = os.path.join(dir2,f)
   cmd =  ["diff", f1, f2]
   #  print("Running: ", " ".join(cmd))
   res = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
   out, err = res.communicate()
   if res.returncode > 1:
      print("Error running diff")
      exit(1)
   vstr = out.splitlines()
   #   print(vstr)
   if len(vstr) == 0 or (len(vstr) == 4 and vstr[1].startswith(b"<    0 days") and vstr[3].startswith(b">    0 days")):
      print(filename, ": OK")
   else:
      exit(1)