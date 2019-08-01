
#pragma once

#include "primitive.h"
#include "sampler.h"
#include "mapping.h"
#include <vector>


struct Primitive;
class Sampler;
class Mapping;


class RadianceCallback {
public:
    virtual ~RadianceCallback() {};
    virtual void call(Vec &point2d, Vec &point3d, Vec color)=0;
};


class Scene 
{
public:
    Scene(Primitive *_light, Sampler *_sampler, Mapping *_mapping);
    ~Scene();

    bool intersect(const Ray &r, double &t, Primitive **primitive);
    Vec radiance(const Ray &r_, int n_samples, RadianceCallback *callback=nullptr);

private:
    std::vector<Primitive*> primitives;
    Primitive *light;
    Sampler *sampler;
    Mapping *mapping;
    std::vector<Vec> points2d;
};


