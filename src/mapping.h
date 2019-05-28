
#pragma once

#include "3d.h"
#include <vector>

class Mapping {
public:
    virtual void map(std::vector<Vec> &points2d, std::vector<Vec> &points3d)=0;
};

class PolarDiskMapping : public Mapping {
public:
    void map(std::vector<Vec> &points2d, std::vector<Vec> &points3d) override;
};