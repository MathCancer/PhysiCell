#
# cell_tracks.py - plot 2-D cell tracks associated with PhysiCell .svg files
#
# Usage:
#  python cell_tracks.py <max # of .svg frames>
# 
# Dependencies include matplotlib and numpy. We recommend installing the Anaconda Python3 distribution.
#
# Examples (run from directory containing the .svg files):
#  python cell_tracks.py 100
#
# Author: Randy Heiland
#
import sys
import xml.etree.ElementTree as ET
import numpy as np
import glob
import matplotlib.pyplot as plt
import math


#print(len(sys.argv))
if (len(sys.argv) < 2):
  usage_str = "Usage: %s <max # svg frames>" % (sys.argv[0])
  print(usage_str)
  print("e.g.,")
  eg_str = "%s 42" % (sys.argv[0])
  print(eg_str)
  exit(1)
else:
   maxCount = int(sys.argv[1])

d={}   # dictionary to hold all (x,y) positions of cells

""" 
--- for example ---
In [141]: d['cell1599'][0:3]
Out[141]: 
array([[ 4900.  ,  4900.  ],
       [ 4934.17,  4487.91],
       [ 4960.75,  4148.02]])
"""

count = 0
for fname in glob.glob('snapshot*.svg'):
#for fname in['snapshot00000000.svg', 'snapshot00000001.svg']:
#for fname in['snapshot00000000.svg']:
#  print(fname)
  count += 1
  if count > maxCount:
    break

#  print('\n---- ' + fname + ':')
  tree=ET.parse(fname)

#  print('--- root.tag, root.attrib ---')
  root=tree.getroot()
#  print('--- root.tag ---')
#  print(root.tag)
#  print('--- root.attrib ---')
#  print(root.attrib)


#  print('--- child.tag, child.attrib ---')
  numChildren = 0
  for child in root:
#  print(child.tag, child.attrib)
#    print('attrib=',child.attrib)
#  if (child.attrib['id'] == 'tissue'):
    if ('id' in child.attrib.keys()):
#      print('-------- found tissue!!')
      tissue_parent = child
      break

#  print('------ search tissue')
  for child in tissue_parent:
#    print('attrib=',child.attrib)
    if (child.attrib['id'] == 'cells'):
#      print('-------- found cells!!')
      cells_parent = child
      break
    numChildren += 1


  num_cells = 0
#  print('------ search cells')
  for child in cells_parent:
#    print(child.tag, child.attrib)
#    print('attrib=',child.attrib)
    for circle in child:
#      print('  --- cx,cy=',circle.attrib['cx'],circle.attrib['cy'])
      xval = float(circle.attrib['cx'])

      # should we test for bogus x,y locations??
      if (math.fabs(xval) > 10000.):
        print("xval=",xval)
        break
      yval = float(circle.attrib['cy'])
      if (math.fabs(yval) > 10000.):
        print("xval=",xval)
        break
      if (child.attrib['id'] in d.keys()):
        d[child.attrib['id']] = np.vstack((d[child.attrib['id']], [ float(circle.attrib['cx']), float(circle.attrib['cy']) ]))
      else:
        d[child.attrib['id']] = np.array( [ float(circle.attrib['cx']), float(circle.attrib['cy']) ])
      break
#    if (child.attrib['id'] == 'cells'):
#      print('-------- found cells!!')
#      tissue_child = child
    num_cells += 1
  print(fname,':  num_cells= ',num_cells)


fig = plt.figure(figsize=(8,8))
ax = fig.gca()
ax.set_aspect("equal")
#ax.set_xticks([])
#ax.set_yticks([]);
#ax.set_xlim(0, 8); ax.set_ylim(0, 8)

#print 'dir(fig)=',dir(fig)
#fig.set_figwidth(8)
#fig.set_figheight(8)

for key in d.keys():
  if (len(d[key].shape) == 2):
    x = d[key][:,0]
    y = d[key][:,1]
    plt.plot(x,y)
  else:
    print(key, " has no x,y points")
#    print(" d[",key,"].shape=", d[key].shape)
#    print(" d[",key,"].size=", d[key].size)
#    print( d[key])

title_str = " max # SVG frames: " + str(maxCount)
plt.title(title_str)
plt.show()
