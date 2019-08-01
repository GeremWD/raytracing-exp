

#include <iostream>
#include <fstream>
#include "scene.h"
#include "write-exr.h" 


struct Test {
    Primitive *light;
    Mapping *mapping;
    std::string name;
};


int main() {
    int w=512, h=384;
    Ray cam(Vec(50,80,295.6), Vec(0,-0.3,-1).norm()); // cam
    Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135;
   
    int ymin = 225, ymax = 315, xmin = 120, xmax = 260;
    std::swap(ymin, ymax);
    ymin = h-ymin-1;
    ymax = h-ymax-1;

    std::string output_folder = "visuals_output/quad_shadow";

    std::vector<Vec> reference((xmax-xmin)*(ymax-ymin));

    std::vector<Test> tests;

    Disk *disk_light = new Disk(10., Vec(50, 80, 100), Vec(0, -1, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
    Mapping *polar_mapping = new PolarDiskMapping(*disk_light);
    tests.emplace_back(Test{disk_light, polar_mapping, "disk_polar"});

    Mapping *concentric_mapping = new ConcentricDiskMapping(*disk_light);
    tests.emplace_back(Test{disk_light, concentric_mapping, "disk_concentric"});

    Quad *quad_light = new Quad(Vec(50, 80, 100), Vec(0, 0, -20), Vec(20, 0, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
    Mapping *quad_mapping = new QuadMapping(*quad_light);
    tests.emplace_back(Test{quad_light, quad_mapping, "quad"});

    std::vector<Sampler*> samplers;
    samplers.push_back(new JitteredSampler());
    samplers.push_back(new UniformJitteredSampler());
    //samplers.push_back(new UniformSampler());
    //samplers.push_back(new ArtSampler());
    //samplers.push_back(new MultiJitteredSampler());
    samplers.push_back(new CorrelatedMultiJitteredSampler());
    //samplers.push_back(new ArtSampler());
    //samplers.push_back(new RegularSampler());


    Scene scene_disk(disk_light, samplers[0], polar_mapping);
    #pragma omp parallel for schedule(dynamic, 1)
    for(int y = ymin; y<ymax; y++) {
        for (int x=xmin; x<xmax; x++) {  
            Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
            Ray ray(cam.o,d.norm()); 
            Vec r = scene_disk.radiance(ray, 16384, nullptr);
            reference[((ymax-ymin)-(y-ymin)-1)*(xmax-xmin)+(x-xmin)] = Vec(clamp(r.x),clamp(r.y),clamp(r.z));
        }
    }
    //write_ppm(reference, output_folder + "/reference_disk.ppm", xmax-xmin, ymax-ymin);
    write_exr_rgb<double>(output_folder + "/reference_disk.exr", (const double*)reference.data(), xmax-xmin, ymax-ymin);

    Scene scene_quad(quad_light, samplers[0], quad_mapping);
    #pragma omp parallel for schedule(dynamic, 1)
    for(int y = ymin; y<ymax; y++) {
        for (int x=xmin; x<xmax; x++) {  
            Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
            Ray ray(cam.o,d.norm()); 
            Vec r = scene_quad.radiance(ray, 16384, nullptr);
            reference[((ymax-ymin)-(y-ymin)-1)*(xmax-xmin)+(x-xmin)] = Vec(clamp(r.x),clamp(r.y),clamp(r.z));
        }
    }
    //write_ppm(reference, output_folder + "/reference_quad.ppm", xmax-xmin, ymax-ymin);
    write_exr_rgb<double>(output_folder + "/reference_quad.exr", (const double*)reference.data(), xmax-xmin, ymax-ymin);

    for(auto &test : tests) {

        for(auto i = 0; i<samplers.size(); i++) {
            Sampler *sampler = samplers[i];

            std::string filename = test.name + "_" + sampler->name();

            Scene scene(test.light, sampler, test.mapping);

            std::vector<Vec> output((xmax-xmin)*(ymax-ymin));
            #pragma omp parallel for schedule(dynamic, 1)
            for(int y = ymin; y<ymax; y++) {
                for (int x=xmin; x<xmax; x++) {   // Loop cols
                    Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
                    Ray ray(cam.o,d.norm()); // Camera rays are pushed forward to start in interior
                    Vec r = scene.radiance(ray, 16, nullptr);
                    output[((ymax-ymin)-(y-ymin)-1)*(xmax-xmin)+(x-xmin)] = Vec(clamp(r.x),clamp(r.y),clamp(r.z));
                }
            }

            //write_ppm(output, output_folder + "/" + filename + ".ppm", xmax-xmin, ymax-ymin);
            write_exr_rgb<double>(output_folder + "/" + filename + ".exr", (const double*)output.data(), xmax-xmin, ymax-ymin);
        }
    }
    return 0;
}
