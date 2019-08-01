
import numpy as np 
import matplotlib.pyplot as plt 
import pylab
from pylab import rcParams
from math import sqrt
rcParams['figure.figsize'] = 10, 10

x = []
y = []
yy = []

for r in range(11, 33):
    x.append(r*r)

for r in range(11, 33):
    filename = "../output/pcf_output/jittered_slope_" + str(r*r) + ".txt"
    a = np.loadtxt(filename)
    
    for i in range(len(a)):
        if a[i] > 0.97:
            y.append(1/(i / len(a) * 0.2)) 
            break
    yy.append(sqrt(x[r-11]) / 2)
 
plt.plot(x, y)
plt.plot(x, yy)

plt.savefig("../output/pcf_output/jittered_slope.png")
plt.clf()



