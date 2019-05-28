
#include "mapping.h"

PolarDiskMapping::PolarDiskMapping(Disk &_disk)
 : disk(_disk), sw(disk.n), su(((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm()), sv(sw%su) {} 

Vec PolarDiskMapping::map(Vec &a) {
    double y1 = disk.radius * sqrt(a.x) * cos(2*M_PI*a.y);
    double y2 = disk.radius * sqrt(a.x) * sin(2*M_PI*a.y);
    return disk.p + su * y1 + sv * y2;
}

/*
Polar4DiskMapping::Polar4DiskMapping(Disk &_disk) {

}*/


DiskProjection::DiskProjection(Disk &_disk)
 : disk(_disk), sw(disk.n), su(((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm()), sv(sw%su) {} 

Vec DiskProjection::map(Vec &a) {
    return Vec((a - disk.p).dot(su), (a - disk.p).dot(sv), 0) * (1. / (2*disk.radius)) + Vec(0.5, 0.5, 0);
}


QuadMapping::QuadMapping(Quad &_quad) : quad(_quad) {}

Vec QuadMapping::map(Vec &a) {
    Vec b;
    double xi1 = a.x-0.5, xi2 = a.y-0.5;
    return quad.p + quad.u * xi1 + quad.v * xi2;
}


