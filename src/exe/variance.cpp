
#include <iostream>
#include "scene.h"
#include "write-exr.h"

int main() {
    int w=512, h=384;
    Ray cam(Vec(50,80,295.6), Vec(0,-0.3,-1).norm()); // cam
    Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135;
    std::vector<Vec> image(w*h);
    Scene scene;

    int ymin = 225, ymax = 315, xmin = 120, xmax = 260;
    #pragma omp parallel for schedule(dynamic, 1)
    for(int y = ymin; y<ymax; y++) {
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",10,100.*y/(h-1));
        unsigned short Xi[3] = {0, 0, (unsigned short)(y*y*y)};
        for (int x=xmin; x<xmax; x++) {   // Loop cols
            Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
            Ray ray(cam.o,d.norm()); // Camera rays are pushed forward to start in interior
            Vec r = scene.radiance(ray, Xi, 100, false);
            
            image[(h-y-1)*w+x] = Vec(clamp(r.x),clamp(r.y),clamp(r.z));
        }
    }
    FILE *f = fopen("reference.ppm", "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i=0; i<w*h; i++)
        fprintf(f,"%d %d %d ", toInt(image[i].x), toInt(image[i].y), toInt(image[i].z));
    printf("\n");
    write_exr_rgb<double>("reference.exr", (const double*)image.data(), 512, 384);

    return 0;
}
