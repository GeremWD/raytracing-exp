
#include "mapping.h"

Mapping::~Mapping() {}

std::string Mapping::name() { return "";}


PolarDiskMapping::PolarDiskMapping(Disk &_disk)
 : disk(_disk), sw(disk.n), su(((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm()), sv(sw%su) {} 

Vec PolarDiskMapping::map(Vec &a) {
    double y1 = disk.radius * sqrt(a.x) * cos(2*M_PI*a.y);
    double y2 = disk.radius * sqrt(a.x) * sin(2*M_PI*a.y);
    return disk.p + su * y1 + sv * y2;
}


ConcentricDiskMapping::ConcentricDiskMapping(Disk &_disk)
 : disk(_disk), sw(disk.n), su(((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm()), sv(sw%su) {}


Vec ConcentricDiskMapping::map(Vec &onSquare) {
    double phi, r, u, v;
    double a = 2*onSquare.x-1;
    double b = 2*onSquare.y-1;
    if(a > -b) {
        if(a > b) {
            r = a;
            phi = (M_PI/4) * (b/a);
        } else {
            r = b;
            phi = (M_PI/4) * (2 - (a/b));
        }
    } else {
        if(a < b) {
            r = -a;
            phi = (M_PI/4) * (4+(b/a));
        } else {
            r = -b;
            if(b != 0) {
                phi = (M_PI/4) * (6 - (a/b));
            } else {
                phi = 0;
            }
        }
    }
    return disk.p + su * r * cos(phi) * disk.radius + sv * r * sin(phi) * disk.radius;
}


/*
Polar4DiskMapping::Polar4DiskMapping(Disk &_disk) {

}*/


DiskProjection::DiskProjection(Disk &_disk)
 : disk(_disk), sw(disk.n), su(((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm()), sv(sw%su) {} 

Vec DiskProjection::map(Vec &a) {
    return Vec((a - disk.p).dot(su), (a - disk.p).dot(sv), 0) * (1. / (2*disk.radius)) + Vec(0.5, 0.5, 0);
}


PolarProjection::PolarProjection(Disk &_disk)
    : disk_projection(_disk) {}

Vec PolarProjection::map(Vec &a) {
    //Vec v = disk_projection.map(a);
    //v.x = sqrt(v.x);
    //v.y = 2*M_PI*v.y;
    Vec v = a;
    v.x = sqrt(v.x);
    return v;
}


QuadMapping::QuadMapping(Quad &_quad) : quad(_quad) {}

Vec QuadMapping::map(Vec &a) {
    Vec b;
    double xi1 = a.x-0.5, xi2 = a.y-0.5;
    return quad.p + quad.u * xi1 + quad.v * xi2;
}


