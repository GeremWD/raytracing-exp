

#include "3d.h"
#include "sampler.h"
#include <functional>

double f(double x, double y) {
    if(x > 0.5) return 0.25; else return 0;
}

int main() {
    int w = 512, n_points = 16, point_width=3;
    Sampler *sampler = new CorrelatedMultiJitteredSampler();

    std::mt19937 gen;
    std::uniform_real_distribution<double> dis(0., 1.);

    std::vector<Vec> left_image(w*w, Vec()), right_image(w*w);
    std::vector<Vec> points(n_points);
    sampler->sample(points, n_points);

    Disk disk(0.5, Vec(0.5, 0.5, 0), Vec(0, 0, 1), Vec(), Vec(), DIFF);
    Mapping *mapping = new PolarDiskMapping(disk);
    //Mapping *mapping = new ConcentricDiskMapping(disk);

    /*for(int x = 0; x<w; x++) {
        for(int y = 0; y<w; y++) {
            double xx = ( .5 + x)/w;
            double yy = ( .5 + y)/w;
            double val = f(xx, yy);
            left_image[y*w+x] = Vec(val, val, val);
            right_image[y*w+x] = Vec(val, val, val);
        }
    }*/

    for(auto point : points) {
        int x = clampint((int)(point.x * w), 0, w-1);
        int y = clampint((int)(point.y * w), 0, w-1);
        int pos = y*w+x;
        Vec color;
        for(int c = 0; c<3; c++) {
            gen.seed(pos*3+c);
            color.v[c] = dis(gen);
        }

        for(int i = -point_width; i<=point_width; i++) {
            for(int j = -point_width; j<=point_width; j++) {
                int newpos = pos + i*w + j;
                if(newpos >= 0 && newpos < w*w)
                    left_image[newpos] = color; 

            }
        }
        Vec mapped_point = mapping->map(point);
        int xmapped = clampint((int)(mapped_point.x * w), 0, w-1);
        int ymapped = clampint((int)(mapped_point.y * w), 0, w-1);
        int posmapped = ymapped*w+xmapped;

        for(int i = -point_width; i<=point_width; i++) {
            for(int j = -point_width; j<=point_width; j++) {
                int newpos = posmapped + i*w + j;
                if(newpos >= 0 && newpos < w*w)
                    right_image[newpos] = color; 

            }
        }
    }

    std::vector<Vec> image(w*w*2);
    for(int x = 0; x<w; x++) {
        for(int y = 0; y<w; y++) {
            image[y*2*w + x] = left_image[y*w+x];
            image[y*2*w + w + x] = right_image[y*w+x];
        }
    }

    write_ppm(image, "flat_experiment_output/image.ppm", 2*w, w);

    return 0;
}


