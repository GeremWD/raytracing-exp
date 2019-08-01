
import numpy as np 
import matplotlib.pyplot as plt 

from pylab import rcParams
rcParams['figure.figsize'] = 10, 10

density = "uniform"
#density = "gaussian"

#mode = "soft"
mode = "hard"

rmin = 0
rmax = 0.2

output_folders = ["../output/pcf_output/" + density + "_density/" + mode]

output_folders = ["../output/pcf_disk_output"]


samplers = ["art", "correlated_multi_jittered", "jittered", "multi_jittered", "regular", "uniform", "uniform_jittered", "poisson_disk"]


for output_folder in output_folders:
    for sampler in samplers:
        a = np.loadtxt(output_folder + "/" + sampler + ".txt")
        delta = 0.5 * (rmax-rmin)/len(a)
        x = np.linspace(rmin + delta, rmax - delta, len(a))
        plt.plot(x, a)
        bot, top = plt.ylim()
        plt.ylim(ymin=0, ymax=top)
        plt.title(sampler)
        plt.savefig(output_folder + "/" + sampler + '.png')
        plt.show()
        plt.clf()



