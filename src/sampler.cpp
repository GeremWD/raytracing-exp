
#include "sampler.h"
#include <random>

Point2d::Point2d() {}
Point2d::Point2d(const Point2d& p) : x(p.x), y(p.y) {}

UniformDiskSampler::UniformDiskSampler(Disk &_disk) : disk(_disk) {}

void UniformDiskSampler::sample(std::vector<Vec> &points, std::vector<Point2d> &points2d, int n, unsigned short *Xi) {
    Vec sw=disk.n, su=((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm(), sv=sw%su;
    for(int i = 0; i<n; i++) {
        double xi1 = erand48(Xi), xi2 = erand48(Xi);
        double y1 = disk.radius * sqrt(xi1) * cos(2*M_PI*xi2);
        double y2 = disk.radius * sqrt(xi1) * sin(2*M_PI*xi2);
        points[i] = disk.p + su * y1 + sv * y2;
        points2d[i].x = y1;
        points2d[i].y = y2;
    }
}


UniformQuadSampler::UniformQuadSampler(Quad &_quad) : quad(_quad) {}

void UniformQuadSampler::sample(std::vector<Vec> &points, std::vector<Point2d> &points2d, int n, unsigned short *Xi) {
    for(int i = 0; i<n; i++) {
        double xi1 = erand48(Xi)-0.5, xi2 = erand48(Xi)-0.5;
        points[i] = quad.p + quad.u * xi1 + quad.v * xi2;
        points2d[i].x = xi1 * quad.u.length();
        points2d[i].y = xi2 * quad.v.length();
    }
}

