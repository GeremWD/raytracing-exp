
#pragma once

#include "3d.h"
#include "primitive.h"
#include <vector>

class Mapping {
public:
    virtual Vec map(Vec &a)=0;
};

class Disk;

class PolarDiskMapping : public Mapping {
private:
    Disk &disk;
    Vec sw, su, sv;
    
public:
    PolarDiskMapping(Disk &_disk);
    Vec map(Vec &a) override;
};

/*class Polar4DiskMapping : public Mapping {
private:
    Disk &disk;

public:
    Polar4DiskMapping(Disk &_disk);
    Vec map(Vec &a) override;
};*/

class DiskProjection : public Mapping {
private:
    Disk &disk;
    Vec sw, su, sv;
    
public:
    DiskProjection(Disk &_disk);
    Vec map(Vec &a) override;
};

class Quad;

class QuadMapping : public Mapping {
private:
    Quad &quad;
public:
    QuadMapping(Quad &_quad);
    Vec map(Vec &a) override;
};

