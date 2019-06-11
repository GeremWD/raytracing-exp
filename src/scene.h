
#pragma once

#include "primitive.h"
#include "sampler.h"
#include "mapping.h"
#include <vector>

struct Primitive;
class Sampler;
class Mapping;

class Scene 
{
private:
    std::vector<Primitive*> primitives;
    Primitive *light;
    Sampler *sampler;
    Mapping *mapping;
    Mapping *projection_mapping=nullptr;
    std::vector<Vec> points;
    std::vector<Vec> points2d;
    std::vector<Vec> analysis_image;
    std::vector<int> n_touched;

public:
    Scene(Primitive *_light, Sampler *_sampler, Mapping *_mapping);
    ~Scene();

    bool intersect(const Ray &r, double &t, Primitive **primitive);
    Vec radiance(const Ray &r_, int n_samples, bool analysis=false);
};