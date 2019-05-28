
#pragma once

#include "3d.h"
#include <vector>

class Mapping {

public:
    virtual void map(std::vector<Vec> &points2d, std::vector<Vec> &points3d);    

};