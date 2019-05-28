
#pragma once

#include <vector>
#include "3d.h"
#include "primitive.h"

struct Point2d {
    double x, y;

    Point2d();
    Point2d(const Point2d& p);
};

class Sampler {

public:
    virtual void sample(std::vector<Vec> &points, std::vector<Point2d> &points2d, int n, unsigned short *Xi)=0;
};

class Disk;

class UniformDiskSampler : public Sampler {
private:
    Disk &disk;

public:
    UniformDiskSampler(Disk &_disk);
    void sample(std::vector<Vec> &points, std::vector<Point2d> &points2d, int n, unsigned short *Xi) override;
};

class Quad;

class UniformQuadSampler : public Sampler {
private:
    Quad &quad;

public:
    UniformQuadSampler(Quad &_quad);
    void sample(std::vector<Vec> &points, std::vector<Point2d> &points2d, int n, unsigned short *Xi) override;
};