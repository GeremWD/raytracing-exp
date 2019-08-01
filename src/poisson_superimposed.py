
import numpy as np 
import matplotlib.pyplot as plt 
import pylab
from pylab import rcParams
rcParams['figure.figsize'] = 10, 10

rmin = 0
rmax = 0.1

for r in range(5, 33, 3):
    filename = "../output/pcf_output/poisson_disk/poisson_disk_" + str(r*r) + ".txt"
    a = np.loadtxt(filename)
    delta = 0.5 * (rmax-rmin)/len(a)
    x = np.linspace(rmin + delta, rmax - delta, len(a))
    plt.plot(x, a, label=str(r*r))

pylab.legend(loc='upper left')
plt.savefig("../output/pcf_output/poisson_disk/poisson_pcfs.png")
plt.show()



