
import matplotlib.pyplot as plt
import numpy as np
from math import sqrt, exp, pi, cos, sin



def disk_distance(posa, radiusa, posb, radiusb):
    r1 = max(radiusa, radiusb)
    r2 = min(radiusa, radiusb)
    d = sqrt((posa[0] - posb[0])*(posa[0] - posb[0]) + (posa[1] - posb[1]) * (posa[1] - posb[1]))
    extent = max(d + r1 + r2, 2*r1)
    overlap = r1 + r2 - d
    if overlap < 0:
        overlap = 0
    if overlap > 2*r2:
        overlap = 2*r2
    return extent - overlap + d + r1 - r2


def gaussianKernel(sigma, x):
    return (1.0 / (sqrt(2 * pi) * sigma)) * exp(-0.5 * (x * x) / (sigma * sigma))


def in_square(point):
    return point[0] >= 0 and point[0] < 1 and point[1] >= 0 and point[1] < 1


n_square_test = 50

def pcf(pos, radius, n_bins, distance):
    hist = [0]*n_bins
    
    N = len(pos)
    ra = 0
    rb = 5 * 2 * sqrt(1. / (2 * sqrt(3) * N))
    
    step = (rb-ra)/ n_bins

    for pcfid in range(n_bins):
        r = ra + (pcfid + 0.5) * step
        estimator = 0
        for i in range(N):
            n_in_square = 0 
            for k in range(n_square_test):
                theta = k * 2 * pi / n_square_test
                test_point = list(pos[i])
                test_point[0] += r*cos(theta)
                test_point[1] += r*sin(theta)
                if in_square(test_point):
                    n_in_square += 1
            weight = n_in_square / n_square_test
            weight = 1

            for j in range(N):
                if i == j:
                    continue
                d = distance(pos[i], radius[i], pos[j], radius[j])
                val = gaussianKernel(0.05, r - d)
                estimator += val*weight

        estimator /= N * N * pi * ((r+0.5*step) * (r+0.5*step) - (r-0.5*step)*(r-0.5*step))
        hist[pcfid] = estimator
    return hist



if __name__=='__main__':
    n_disks = 50

    pos = np.random.rand(n_disks, 2)
    radius = np.random.rand(n_disks)/10.    

    n_disks = 10
    pos = [(0.5, 0.9), (0.2, 0.8), (0.8, 0.875), (0.1, 0.55), (0.35, 0.45), (0.7,1-0.375), (0.9, 0.425), (0.125, 0.2), (0.6, 0.225), (0.9, 0.175)]
    radius = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1]

    hist = pcf(pos, radius, 100)
    #print(hist)

    rmax = 2 * sqrt(1. / (2 * sqrt(3) * n_disks))
    print(rmax)

    delta = 0.5 * 5 / len(hist)
    x = np.linspace(delta, 5 - delta, len(hist))

    fig, ax = plt.subplots() 
    ax.plot(x, hist)

    fig, ax = plt.subplots() 

    ax.set_xlim((0, 1))
    ax.set_ylim((0, 1))
    ax.set_aspect('equal')

    for i in range(n_disks):
        ax.add_artist(plt.Circle(pos[i], radius[i], fill=False))

    plt.show()

    #fig.savefig('plotcircles.png')
