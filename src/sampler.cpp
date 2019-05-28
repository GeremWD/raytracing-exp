
#include "sampler.h"
#include <random>

UniformSampler::UniformSampler() : gen(0), dis(0., 1.) {}

void UniformSampler::sample(std::vector<Vec> &points, int n) {
    for(int i = 0; i<n; i++) {
        points[i].x = dis(gen);
        points[i].y = dis(gen);
    }
}
