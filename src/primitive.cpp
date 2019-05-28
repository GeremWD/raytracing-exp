
#include "primitive.h"
#include <iostream>

Primitive::Primitive(Refl_t refl_, Vec p_, Vec e_, Vec c_) : refl(refl_), p(p_), e(e_), c(c_) {}

bool Primitive::isLight() {
    return !(e.x<=0 && e.y<=0 && e.z<=0);
}

Sphere::Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
        Primitive(refl_, p_, e_, c_), rad(rad_) {}

double Sphere::intersect(const Ray &r) const { // returns distance, 0 if nohit
    Vec op = p-r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    double t, eps=1e-4, b=op.dot(r.d), det=b*b-op.dot(op)+rad*rad;
    if (det<0) return 0; else det=sqrt(det);
    return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
}

Vec Sphere::normal(const Vec &intersection) const {
    return (intersection - p).norm();
}
/*
Vec Sphere::lightSampling(Scene &scene, const Vec &x, const Vec &nl, unsigned short *Xi) {
    Vec sw=p-x, su=((fabs(sw.x)>.1?Vec(0,1):Vec(1))%sw).norm(), sv=sw%su;
    double cos_a_max = sqrt(1-rad*rad/(x-p).dot(x-p));
    double eps1 = erand48(Xi), eps2 = erand48(Xi);
    double cos_a = 1-eps1+eps1*cos_a_max;
    double sin_a = sqrt(1-cos_a*cos_a);
    double phi = 2*M_PI*eps2;
    Vec l = su*cos(phi)*sin_a + sv*sin(phi)*sin_a + sw*cos_a;
    l.norm();
    double t;
    Primitive *intersection;
    if (scene.intersect(Ray(x,l), t, &intersection) && intersection==this){  // shadow ray
        double omega = 2*M_PI*(1-cos_a_max);
        return e*l.dot(nl)*omega*M_1_PI;  // 1/pi for brdf
    }
    return Vec();
}*/

Disk::Disk(double radius_, Vec p_, Vec n_,Vec e_, Vec c_, Refl_t refl_) :
        Primitive(refl_, p_, e_, c_), radius(radius_), n(n_) {}

double Disk::intersect(const Ray &r) const {
    double t = 0;
    if (intersectPlane(n, p, r.o, r.d, t)) {
        Vec pp = r.o + r.d * t;
        Vec v = pp - p;
        float d2 = v.dot(v);
        return (sqrtf(d2) <= radius) ? t : 0;
    }
    return 0;
}

Vec Disk::normal(const Vec &intersection) const {
    return n;
}

Quad::Quad(Vec p_, Vec u_, Vec v_, Vec e_, Vec c_, Refl_t refl_) :
        Primitive(refl_, p_, e_, c_), u(u_), v(v_) {}


double Quad::intersect(const Ray &r) const {
    double t = 0;
    if (intersectPlane(normal(p), p, r.o, r.d, t)) {
        Vec pp = r.o + r.d * t;
        Vec pos = pp - p;
        Vec norm_u = u, norm_v = v;
        norm_u.norm(); norm_v.norm();
        double vu = pos.dot(norm_u), vv = pos.dot(norm_v);
        return (vu >= -u.length() / 2 && vu <= u.length()/2 && vv >= -v.length()/2 && vv <= v.length()/2) ? t : 0;
    }
    return 0;
}

Vec Quad::normal(const Vec &intersection) const {
    return (u%v).norm();
}