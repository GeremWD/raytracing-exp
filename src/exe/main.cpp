#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
#include "scene.h"
#include <iostream>

#include "write-exr.h"

int main(int argc, char *argv[]){
  int w=512, h=384;
  int xanalysis = 287, yanalysis = h-334;
  Ray cam(Vec(50,52,295.6), Vec(0,-0.042612,-1).norm()); // cam pos, dir
  Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135;
  std::vector<Vec> image(w*h);
  Scene scene;
  #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
  for (int y=0; y<h; y++){                       // Loop over image rows
    fprintf(stderr,"\rRendering (%d spp) %5.2f%%",10,100.*y/(h-1));
    unsigned short Xi[3] = {0, 0, (unsigned short)(y*y*y)};
    for (int x=0; x<w; x++) {   // Loop cols
        Vec d = cx*( ( .5 + x)/w - .5) + cy*( ( .5 + y)/h - .5) + cam.d;
        Ray ray(cam.o+d*140,d.norm()); // Camera rays are pushed forward to start in interior
        
        bool analysis = (x == xanalysis && y == yanalysis);
        Vec r = scene.radiance(ray, Xi, analysis ? 512*512 : 10, analysis);
        
        image[(h-y-1)*w+x] = Vec(clamp(r.x),clamp(r.y),clamp(r.z));
    }
  }
  FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
  for (int i=0; i<w*h; i++)
    fprintf(f,"%d %d %d ", toInt(image[i].x), toInt(image[i].y), toInt(image[i].z));
  printf("\n");
}
