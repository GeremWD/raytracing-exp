
#include "scene.h"
#include "primitive.h"
#include "sampler.h"
#include <iostream>
#include <fstream>
#include "write-exr.h"

Scene::Scene() : points(512*512), points2d(512*512), analysis_image(512*512, Vec()), n_touched(512*512, 0) {

    //Scene: radius, position, emission, color, material
    primitives.push_back(new Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF));//Left
    primitives.push_back(new Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF));//Rght
    primitives.push_back(new Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.75,.75,.75),DIFF));//Back
    primitives.push_back(new Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF));//Frnt
    primitives.push_back(new Sphere(1e5, Vec(50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF));//Botm
    primitives.push_back(new Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF));//Top
    primitives.push_back(new Sphere(16.5, Vec(73,16.5,78),       Vec(),Vec(1,1,1)*.999, DIFF));//Glas
    primitives.push_back(new Sphere(16.5, Vec(27,16.5,47),       Vec(),Vec(1, 1, 1), DIFF));//Mirr

    //primitives.push_back(new Sphere(16.5, Vec(50,681.6-.27,81.6),Vec(12,12,12),  Vec(), DIFF)); //Lite
    //primitives.push_back(new Disk(10., Vec(50, 80, 100), Vec(0, -1, 0), Vec(10000, 10000, 10000), Vec(), DIFF));
    //primitives.push_back(new Quad(Vec(50, 80, 100), Vec(20, 0, 0), Vec(0, 0, 20), Vec(10000, 10000, 10000), Vec(), DIFF));
    //primitives.push_back(new Sphere(16.5, Vec(27,16.5,47),       Vec(6, 6, 6),Vec(), DIFF));//Mirr

    Disk *_light = new Disk(10., Vec(50, 80, 100), Vec(0, -1, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
    mapping = new PolarDiskMapping(*_light);
    projection_mapping = new DiskProjection(*_light);


    /*Quad *_light = new Quad(Vec(50, 80, 100), Vec(0, 0, -20), Vec(20, 0, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
    mapping = new QuadMapping(*_light);*/
    
    light = _light;
    primitives.push_back(light);

    sampler = new UniformSampler();
}

bool Scene::intersect(const Ray &r, double &t, Primitive **primitive) {
    double d, inf=t=1e20;
    for(int i=primitives.size();i--;) if((d=primitives[i]->intersect(r))&&d<t){t=d;*primitive = primitives[i];}
    return t<inf;
}

Vec Scene::radiance(const Ray &r_, unsigned short *Xi, int n_samples, bool analysis) {
    double t;                               // distance to intersection
    Ray r=r_;

    Primitive *obj; // the hit object
    if (!intersect(r, t, &obj)) {
        return Vec(); // if miss, return black
    }
    
    Vec x=r.o+r.d*t, n=obj->normal(x), nl=n.dot(r.d)<0?n:n*-1;

    sampler->sample(points2d, n_samples);

    Vec e;
    for(int i = 0; i<n_samples; i++) {
        Vec point = mapping->map(points2d[i]);
        Vec l = (point - x);
        double d = l.dot(l);
        l.norm();
        Vec n = light->normal(point);
        Vec nn=n.dot(l)<0?n:n*-1;
        double t;
        Primitive *intersection;
        if (intersect(Ray(x,l), t, &intersection) && intersection==light){  // shadow ray
            Vec result = light->e * (-l.dot(nl) * l.dot(nn) / d)*M_1_PI;  // 1/pi for brdf
            result = result.positive();

            if(analysis) {
                Vec analysis_point = projection_mapping != nullptr ? projection_mapping->map(point) : points2d[i];

                int x = clampint((int)((analysis_point.x) * 512), 0, 511);
                int y = clampint((int)((analysis_point.y) * 512), 0, 511);
                int pos = y*512+x;
                analysis_image[pos] = analysis_image[pos] + result;
                n_touched[pos]++;
            }
            e = e + obj->c.mult(result);
        }
    }

    if(analysis) {
        for(int i = 0; i<512*512; i++) {
            if(n_touched[i] > 0) 
                analysis_image[i] = analysis_image[i] * (1. / n_touched[i]);
            analysis_image[i].clamp();
        }

        FILE *f = fopen("analysis.ppm", "w");         // Write image to PPM file.
        fprintf(f, "P3\n%d %d\n%d\n", 512, 512, 255);
        for (int i=0; i<512*512; i++)
            fprintf(f,"%d %d %d ", toInt(analysis_image[i].x), toInt(analysis_image[i].y), toInt(analysis_image[i].z));
        write_exr_rgb<double>("analysis.exr", (const double*)analysis_image.data(), 512, 512);        
    }

    e = e * (1. / n_samples);
    e = e + obj->e;
    return e;
}

