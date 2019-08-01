


import matplotlib.pyplot as plt
import numpy as np
from math import sqrt, exp, pi, cos, sin
from mayavi import mlab


def sphere_distance(posa, radiusa, posb, radiusb):
    r1 = max(radiusa, radiusb)
    r2 = min(radiusa, radiusb)
    d = np.linalg.norm(posb - posa)
    extent = max(d + r1 + r2, 2*r1)
    overlap = r1 + r2 - d
    if overlap < 0:
        overlap = 0
    if overlap > 2*r2:
        overlap = 2*r2
    return extent - overlap + d + r1 - r2

def gaussianKernel(sigma, x):
    return (1.0 / (sqrt(2 * pi) * sigma)) * exp(-0.5 * (x * x) / (sigma * sigma))


def in_cube(point):
    return point[0] >= 0 and point[0] < 1 and point[1] >= 0 and point[1] < 1 and point[2] >= 0 and point[2] < 1

def sample_spherical(npoints, ndim=3):
    vec = np.random.randn(ndim, npoints)
    vec /= np.linalg.norm(vec, axis=0)
    return vec

n_cube_test = 50

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
            n_in_cube = 0
            for _ in range(n_cube_test):
                test_point = sample_spherical(1)[0] + pos[i]
                if in_cube(test_point):
                    n_in_cube += 1

            weight = n_in_cube / n_cube_test

            for j in range(N):
                if i == j:
                    continue
                d = distance(pos[i], radius[i], pos[j], radius[j])
                val = gaussianKernel(0.05, r - d)
                estimator += val*weight

        estimator /= N * N * (4/3) * pi * (((r+0.5*step) ** 3) - ((r-0.5*step)**3))
        hist[pcfid] = estimator
    return hist


def conflict(pos, radius, n, new_pos, new_radius):
    for j in range(n):
        d = np.linalg.norm(pos[j] - new_pos)
        if d < new_radius + radius[j]:
            return True
    return False


if __name__=='__main__':
    n_spheres = 30

    pos = np.zeros((n_spheres, 3))
    radius = np.zeros(n_spheres)

    for i in range(n_spheres):
        while True:
            pos[i] = np.random.rand(3)
            radius[i] = np.random.rand()
            if not conflict(pos, radius, i, pos[i], radius[i]) and radius[i] > 0.07:
                break
        print(i)

    hist = pcf(pos, radius, 100, sphere_distance)
    #print(hist)

    rmax = 2 * sqrt(1. / (2 * sqrt(3) * n_spheres))
    print(rmax)

    delta = 0.5 * 5 / len(hist)
    x = np.linspace(delta, 5 - delta, len(hist))

    fig, ax = plt.subplots() 
    ax.plot(x, hist)


    mlab.points3d(pos[:,0],pos[:,1],pos[:,2],radius, resolution=64)

    mlab.show()
    plt.show()
        