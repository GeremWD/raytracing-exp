
import numpy as np 
import matplotlib.pyplot as plt 
from math import sqrt

from pylab import rcParams
rcParams['figure.figsize'] = 10, 10

n_points = 1024
krmax = 5
rmin = 0
rmax = krmax * 2 * sqrt(1. / (2 * sqrt(3) * n_points));

output_folder = "../output/pcf_synthesis"

for name in ("example", "output"):
    a = np.loadtxt(output_folder + "/" + name + ".txt")
    delta = 0.5 * (rmax-rmin)/len(a)
    x = np.linspace(rmin + delta, rmax - delta, len(a))
    plt.plot(x, a)
    bot, top = plt.ylim()
    plt.ylim(ymin=0, ymax=top)
    plt.savefig(output_folder + "/" + name + '.png')
    #plt.show()
    plt.clf()

