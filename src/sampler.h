
#pragma once

#include <vector>
#include <random>
#include "3d.h"
#include "primitive.h"

class Sampler {
public:
    virtual void sample(std::vector<Vec> &points, int n)=0;
};

class UniformSampler : public Sampler {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

public:
    UniformSampler();
    void sample(std::vector<Vec> &points, int n) override;
};
