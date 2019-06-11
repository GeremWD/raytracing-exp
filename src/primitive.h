
#pragma once

#include "3d.h"
#include "scene.h"
#include "sampler.h"

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance()

class Scene;

struct Primitive {
    Refl_t refl;// reflection type (DIFFuse, SPECular, REFRactive)
    Vec p, e, c; //position, emission, color
    Primitive(Refl_t refl_, Vec p_, Vec e_, Vec c_);
    virtual ~Primitive();

    virtual double intersect(const Ray &r) const = 0;
    virtual Vec normal(const Vec &intersection) const = 0;
    bool isLight();
};


struct Sphere : public Primitive  {

private:
    double rad;       // radius

public:
    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_);  
    double intersect(const Ray &r) const override;
    Vec normal(const Vec &intersection) const override;
};


struct Disk : public Primitive {
    double radius;
    Vec n; // normal
    Disk(double radius_, Vec p_, Vec n_,Vec e_, Vec c_, Refl_t refl_);

    double intersect(const Ray &r) const override;
    Vec normal(const Vec &intersection) const override;
};


struct Quad : public Primitive {
    Vec u, v;
    Quad(Vec center_, Vec u_, Vec v_, Vec e_, Vec c_, Refl_t refl_);

    double intersect(const Ray &r) const override;
    Vec normal(const Vec &intersection) const override;
};

