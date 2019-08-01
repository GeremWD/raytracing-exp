
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
    //int x = (xmin + xmax) / 2;
    //int y = (ymin + ymax) * 2; 
    //int x = xmin + 86, y = ymin + 58;
    int x = xmin + 15, y = ymin + 40;

    std::string output_folder = "mse_pixels_output/quad_shadow";

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
    samplers.push_back(new UniformSampler());
    samplers.push_back(new ArtSampler());
    samplers.push_back(new MultiJitteredSampler());
    samplers.push_back(new CorrelatedMultiJitteredSampler());
    samplers.push_back(new RegularSampler());

    for(auto &test : tests) {
        Scene scene(test.light, samplers[0], test.mapping);

        Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
        Ray ray(cam.o,d.norm()); 
        Vec ref = scene.radiance(ray, 1024*1024, nullptr);

        int n_repeat = 100;

        for(auto i = 0; i<samplers.size(); i++) {
            Sampler *sampler = samplers[i];
            std::string filename = test.name + "_" + sampler->name();
            std::ofstream file(output_folder + "/" + filename + ".txt");

            Scene scene(test.light, sampler, test.mapping);

            for(int n_samples = 4; ; n_samples*=1.5) {
                int n_samples_corrected = round(sqrt((double)n_samples));

                n_samples_corrected *= n_samples_corrected;
                if(n_samples_corrected > 3000) break;   

                double average_error = 0;
                for(int repeat=0; repeat < n_repeat; repeat++) {
                    Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
                    Ray ray(cam.o,d.norm());
                    Vec r = scene.radiance(ray, n_samples_corrected, nullptr);
                    r.clamp();
                    double error = (r - ref).length() * (r - ref).length();
                    average_error += error;
                }
                average_error /= n_repeat;
                file << n_samples_corrected << " " << average_error / n_repeat  << std::endl;
            }
        }
    }
    return 0;
}