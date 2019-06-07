#
# anim_svg_step.py:  render/animate PhysiCell .svg files, using left/right arrows on keyboard
#
# Usage:
#  python anim_svg_step.py <show_nucleus start_index axes_min axes_max scale_radius>
#    i.e., the arguments <...> are optional and have defaults.
# 
# Keyboard arrows: right/left arrows will single step forward/backward; up/down will increment/decrement step size
#
# Dependencies include matplotlib and numpy. We recommend installing the Anaconda Python3 distribution.
#
# Examples (run from directory containing the .svg files):
#  python anim_svg_step.py 
#  python anim_svg_step.py 0 5 700 1300 12
#
# Author: Randy Heiland
#
#
__author__ = "Randy Heiland"

import sys
import glob
import os
import xml.etree.ElementTree as ET
import math
join_our_list = "(Join/ask questions at https://groups.google.com/forum/#!forum/physicell-users)\n"
try:
  import matplotlib
  import matplotlib.colors as mplc
except:
  print("\n---Error: cannot import matplotlib")
  print("---Try: python -m pip install matplotlib")
  print(join_our_list)
#  print("---Consider installing Anaconda's Python 3 distribution.\n")
  raise
try:
  import numpy as np  # if mpl was installed, numpy should have been too.
except:
  print("\n---Error: cannot import numpy")
  print("---Try: python -m pip install numpy\n")
  print(join_our_list)
  raise
from collections import deque
try:
  # apparently we need mpl's Qt backend to do keypresses 
#  matplotlib.use("Qt5Agg")
  matplotlib.use("TkAgg")
  import matplotlib.pyplot as plt
except:
  print("\n---Error: cannot use matplotlib's TkAgg backend")
  print(join_our_list)
#  print("Consider installing Anaconda's Python 3 distribution.")
  raise


current_idx = 0
print("# args=",len(sys.argv)-1)

#for idx in range(len(sys.argv)):
use_defaults = True
show_nucleus = 0
current_idx = 0
axes_min = 0.0
axes_max = 1000  # but overridden by "width" attribute in .svg
scale_radius = 1.0
if (len(sys.argv) == 6):
  use_defaults = False
  kdx = 1
  show_nucleus = int(sys.argv[kdx])
  kdx += 1
  current_idx = int(sys.argv[kdx])
  kdx += 1
  axes_min = float(sys.argv[kdx])
  kdx += 1
  axes_max = float(sys.argv[kdx])
  kdx += 1
  scale_radius = float(sys.argv[kdx])
elif (len(sys.argv) != 1):
  print("Please provide either no args or 5 args:")
  usage_str = "show_nucleus start_index axes_min axes_max scale_radius"
  print(usage_str)
  print("e.g.,")
  eg_str = "%s 0 0 0 2000 1" % (sys.argv[0])
  print(eg_str)
  sys.exit(1)

#"""
print("show_nucleus=",show_nucleus)
print("current_idx=",current_idx)
print("axes_min=",axes_min)
print("axes_max=",axes_max)
print("scale_radius=",scale_radius)
#"""

"""
if (len(sys.argv) > 1):
   current_idx = int(sys.argv[1])
if (len(sys.argv) > 2):
   axes_min = float(sys.argv[2])
   axes_max = float(sys.argv[3])
if (len(sys.argv) == 5):
   scale_radius = float(sys.argv[4])

if (len(sys.argv) > 5):
  usage_str = "[<start_index> [<axes_min axes_max [scale_radius]]]"
  print(usage_str)
  print("e.g.,")
  eg_str = "%s 10 700 1300 4" % (sys.argv[0])
  print(eg_str)
  sys.exit(1)
"""

print("current_idx=",current_idx)

#d={}   # dictionary to hold all (x,y) positions of cells

""" 
--- for example ---
In [141]: d['cell1599'][0:3]
Out[141]: 
array([[ 4900.  ,  4900.  ],
       [ 4934.17,  4487.91],
       [ 4960.75,  4148.02]])
"""

fig = plt.figure(figsize=(7,7))
ax = fig.gca()
#ax.set_aspect("equal")


#plt.ion()

time_delay = 0.1

count = -1
#while True:
def plot_svg():
  global current_idx, axes_max
  fname = "snapshot%08d.svg" % current_idx
  if (os.path.isfile(fname) == False):
    print("File does not exist: ",fname)
    return

  xlist = deque()
  ylist = deque()
  rlist = deque()
  rgb_list = deque()

#  print('\n---- ' + fname + ':')
  tree = ET.parse(fname)
  root = tree.getroot()
#  print('--- root.tag ---')
#  print(root.tag)
#  print('--- root.attrib ---')
#  print(root.attrib)


#  print('--- child.tag, child.attrib ---')
  numChildren = 0
  for child in root:
#    print(child.tag, child.attrib)
#    print("keys=",child.attrib.keys())
    if use_defaults and ('width' in child.attrib.keys()):
      axes_max = float(child.attrib['width'])
#      print("--- found width --> axes_max =", axes_max)
    if child.text and "Current time" in child.text:
      svals = child.text.split()
      title_str = "(" + str(current_idx) + ") Current time: " + svals[2] + "d, " + svals[4] + "h, " + svals[7] + "m"

#    print("width ",child.attrib['width'])
#    print('attrib=',child.attrib)
#    if (child.attrib['id'] == 'tissue'):
    if ('id' in child.attrib.keys()):
#      print('-------- found tissue!!')
      tissue_parent = child
      break

#  print('------ search tissue')
  cells_parent = None

  for child in tissue_parent:
#    print('attrib=',child.attrib)
    if (child.attrib['id'] == 'cells'):
#      print('-------- found cells, setting cells_parent')
      cells_parent = child
      break
    numChildren += 1


  num_cells = 0
#  print('------ search cells')
  for child in cells_parent:
#    print(child.tag, child.attrib)
#    print('attrib=',child.attrib)
    for circle in child:  # two circles in each child: outer + nucleus
    #  circle.attrib={'cx': '1085.59','cy': '1225.24','fill': 'rgb(159,159,96)','r': '6.67717','stroke': 'rgb(159,159,96)','stroke-width': '0.5'}
#      print('  --- cx,cy=',circle.attrib['cx'],circle.attrib['cy'])
      xval = float(circle.attrib['cx'])

      s = circle.attrib['fill']
#      print("s=",s)
#      print("type(s)=",type(s))
      if (s[0:3] == "rgb"):  # if an rgb string, e.g. "rgb(175,175,80)" 
        rgb = list(map(int, s[4:-1].split(",")))  
        rgb[:]=[x/255. for x in rgb]
      else:     # otherwise, must be a color name
        rgb_tuple = mplc.to_rgb(mplc.cnames[s])  # a tuple
        rgb = [x for x in rgb_tuple]

      # test for bogus x,y locations (rwh TODO: use max of domain?)
      too_large_val = 10000.
      if (math.fabs(xval) > too_large_val):
        print("bogus xval=",xval)
        break
      yval = float(circle.attrib['cy'])
      if (math.fabs(yval) > too_large_val):
        print("bogus xval=",xval)
        break

      rval = float(circle.attrib['r'])
#      if (rgb[0] > rgb[1]):
#        print(num_cells,rgb, rval)
      xlist.append(xval)
      ylist.append(yval)
      rlist.append(rval)
      rgb_list.append(rgb)

#     For .svg files with cells that *have* a nucleus, there will be a 2nd
      if (show_nucleus == 0):
        break

    num_cells += 1

#    if num_cells > 3:   # for debugging
#      print(fname,':  num_cells= ',num_cells," --- debug exit.")
#      sys.exit(1)
#      break

  print(fname,':  num_cells= ',num_cells)

  xvals = np.array(xlist)
  yvals = np.array(ylist)
  rvals = np.array(rlist)
  rgbs =  np.array(rgb_list)
#print("xvals[0:5]=",xvals[0:5])
#print("rvals[0:5]=",rvals[0:5])
#  print("rvals.min, max=",rvals.min(),rvals.max())

  plt.cla()
  title_str += " (" + str(num_cells) + " agents)"
  plt.title(title_str)
  plt.xlim(axes_min,axes_max)
  plt.ylim(axes_min,axes_max)
  plt.scatter(xvals,yvals, s=rvals*scale_radius, c=rgbs)
#plt.xlim(0,2000)  # TODO - get these values from width,height in .svg at top
#plt.ylim(0,2000)
  plt.pause(time_delay)

step_value = 1
def press(event):
  global current_idx, step_value
#    print('press', event.key)
  sys.stdout.flush()
  if event.key == 'escape':
    sys.exit(1)
  elif event.key == 'h':  # help
    print('esc: quit')
    print('right arrow: increment by step_value')
    print('left arrow:  decrement by step_value')
    print('up arrow:   increment step_value by 1')
    print('down arrow: decrement step_value by 1')
    print('0: reset to 0th frame')
    print('h: help')
  elif event.key == 'left':  # left arrow key
#    print('go backwards')
#    fig.canvas.draw()
    current_idx -= step_value
    if (current_idx < 0):
      current_idx = 0
    plot_svg()
  elif event.key == 'right':  # right arrow key
#        print('go forwards')
#        fig.canvas.draw()
    current_idx += step_value
    plot_svg()
  elif event.key == 'up':  # up arrow key
    step_value += 1
    print('step_value=',step_value)
  elif event.key == 'down':  # down arrow key
    step_value -= 1
    if (step_value <= 0):
      step_value = 1
    print('step_value=',step_value)
  elif event.key == '0':  # reset to 0th frame/file
    current_idx = 0
    plot_svg()
  else:
    print('press', event.key)


#for current_idx in range(40):
#  fname = "snapshot%08d.svg" % current_idx
#  plot_svg(fname)
plot_svg()
print("\nNOTE: click in plot window to give it focus before using keys.")

fig.canvas.mpl_connect('key_press_event', press)

# keep last plot displayed
#plt.ioff()
plt.show()
