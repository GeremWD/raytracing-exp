
#pragma once

#include "primitive.h"
#include "sampler.h"
#include <vector>

struct Primitive;
class Sampler;
struct Point2d;

class Scene 
{
private:
    std::vector<Primitive*> primitives;
    Primitive *light;
    Sampler *sampler;
    std::vector<Vec> points;
    std::vector<Point2d> points2d;
    std::vector<Vec> analysis_image;
    std::vector<int> n_touched;

public:
    Scene();

    bool intersect(const Ray &r, double &t, Primitive **primitive);
    Vec radiance(const Ray &r_, unsigned short *Xi, int n_samples, bool analysis=false);
};