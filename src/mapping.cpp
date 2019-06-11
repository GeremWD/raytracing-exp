
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

Vec ConcentricDiskMapping::map(Vec &a) {
    double phi = 0, radius = disk.radius;

    double aa = 2*a.x - 1, bb = 2*a.y - 1;
    
    if (aa == 0 || bb == 0) {
        return Vec(0, 0, 0);
    }
    
    if(aa > bb) {
        radius *= aa;
        phi = (M_PI / 4) * (bb/aa); 
    } else {
        radius *= bb;
        phi = (M_PI / 2) - ((M_PI/4) * (aa/bb));
    }
    return disk.p + su * cos(phi) * radius + sv * sin(phi) * radius;
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


