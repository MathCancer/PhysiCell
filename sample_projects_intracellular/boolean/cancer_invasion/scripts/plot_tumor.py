import sys
import os
import glob
import numpy as np
from pyMCDS_cells import pyMCDS_cells
import matplotlib.pyplot as plt

argc = len(sys.argv)-1
print("# args=",argc)

#data_dir = 'output'
if (argc < 1):
#  data_dir = int(sys.argv[kdx])
  print("Usage: provide output subdir")
  sys.exit(-1)

kdx = 1
data_dir = sys.argv[kdx]
print('data_dir = ',data_dir)
os.chdir(data_dir)
xml_files = glob.glob('output*.xml')
os.chdir('..')
xml_files.sort()
#print('xml_files = ',xml_files)

ds_count = len(xml_files)
ds_count = 192
print("----- ds_count = ",ds_count)
mcds = [pyMCDS_cells(xml_files[i], data_dir) for i in range(ds_count)]

tval = np.linspace(0, mcds[-1].get_time(), ds_count)
print('tval= ',tval)

# count epi cells still live 
necrotic = np.array( [(np.count_nonzero((mcds[idx].data['discrete_cells']['current_phase'] > 100))) for idx in range(ds_count)]  )
apoptotic = np.array( [(np.count_nonzero((mcds[idx].data['discrete_cells']['current_phase'] == 100))) for idx in range(ds_count)]  )
live = np.array( [(np.count_nonzero((mcds[idx].data['discrete_cells']['current_phase'] < 100))) for idx in range(ds_count)]  )
# # count epi cells infected
# y_infected = np.array( [(np.count_nonzero((mcds[idx].data['discrete_cells']['cell_type'] == 1) & (mcds[idx].data['discrete_cells']['virion'] > 1.) == True)) for idx in range(ds_count)] )

# # count epi cells dead
# y_dead = np.array( [(np.count_nonzero((mcds[idx].data['discrete_cells']['cell_type'] == 1) & (mcds[idx].data['discrete_cells']['cycle_model'] >= 100) == True)) for idx in range(ds_count)] )
fig = plt.figure(figsize=(10,6), dpi=200, tight_layout=True)
fig.subplots(1)
fig.axes[0].semilogy(tval, live, label='live')#, linewidth=2)  #, color='lime')
# fig.axes[0].plot(tval, live, label='live')#, linewidth=2)  #, color='lime')
# fig.axes[0].plot(tval, necrotic, label='necrotic')#, linewidth=2)  #, color='lime')
# plt.plot(tval, apoptotic, label='apoptotic', linewidth=2)  #, color='lime')

plt.legend(loc='upper right', prop={'size': 10})
plt.xticks(
  [0, 1440,2*1440, 3*1440, 4*1440, 5*1440, 6*1440, 7*1440, 8*1440],#, 9*1440, 10*1440,11*1440, 12*1440, 13*1440, 14*1440, 15*1440], #, 16*1440, 17*1440, 18*1440, 19*1440], 
  ('0', '1', '2','3','4','5','6', '7','8')#,'9','10', '11', '12', '13', '14', '15')#, '16', '17', '18', '19')
)

plt.xlabel('Time (days)')
plt.ylabel('Number of cells')
fig.axes[0].axvspan(1000., 11520, facecolor='0.9')
# fig.axes[0].axvspan(1000., 5320, facecolor='0.9')
# fig.axes[0].axvspan(8200., 12520, facecolor='0.9')
# fig.axes[0].axvspan(15400., 19720, facecolor='0.9')
plt.savefig(data_dir + '.png')
plt.show()
