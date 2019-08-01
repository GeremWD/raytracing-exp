

import numpy as np 
import matplotlib.pyplot as plt 
from math import sqrt

from pylab import rcParams
rcParams['figure.figsize'] = 10, 10

rmin = 0
rmax = 3.0

filename = "../output/pcf_disk_output/poisson_disk"

a = np.loadtxt(filename + ".txt")
delta = 0.5 * (rmax-rmin)/len(a)
x = np.linspace(rmin + delta, rmax - delta, len(a))
plt.plot(x, a)
bot, top = plt.ylim()
plt.ylim(ymin=0, ymax=top)
plt.savefig(filename + ".png")
plt.show()



