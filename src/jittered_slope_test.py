
import numpy as np
from random import random
from math import pi, cos, sin, sqrt
import matplotlib.pyplot as plt

n_repeat = 10000

xl = []
yl = []
for r in np.linspace(0.001, 3., 100):
    xl.append(r)
    n_good = 0
    for i in range(n_repeat):
        x = random()
        y = random()
        theta = random() * 2*pi
        xx = x + r*cos(theta)
        yy = y + r*sin(theta)

        if not (xx > 0 and xx < 1. and yy > 0 and yy < 1.):
            n_good += 1
    yl.append(n_good / n_repeat)
    #yl2.append(min(sqrt(), 1.))


plt.plot(xl, yl)
#plt.plot(xl, yl2)
plt.show()