import numpy as np

f=np.fromfile('test.mat',dtype='double', count=3)
print(f[0],f[1],f[2])
