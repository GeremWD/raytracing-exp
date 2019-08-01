#include <cmath>  
#include <stdlib.h> 
#include <stdio.h> 
#include "scene.h"
#include <iostream>

#include "write-exr.h"
#include "util.h"

const int n_samples_root = 1024;
const int delta = 256;
const int image_res = 512;

class Callback : public RadianceCallback
{
private:
  int i = 0;

  std::vector<Vec> points;
  std::vector<Vec> colors;

  std::mt19937 gen;
  std::uniform_real_distribution<double> dis;

  std::vector<Vec> analysis_image;
  std::vector<int> n_touched;

  Mapping *analysis_mapping;

  std::string output_folder = "../output/main_experiment/left_pixel";

public:
  Callback(Mapping *_analysis_mapping) 
  : gen(42), dis(0., 1.), analysis_image(image_res * image_res, Vec()), n_touched(image_res * image_res, 0), 
    analysis_mapping(_analysis_mapping) {}

  void call(Vec &point2d, Vec &point3d, Vec color)
  {
    Vec analysis_point = analysis_mapping != nullptr ? analysis_mapping->map(point2d) : point2d;

    if ((i / n_samples_root) % delta == delta / 2 && (i % n_samples_root) % delta == delta / 2)
    {
      points.push_back(analysis_point);
      colors.push_back(Vec(dis(gen), dis(gen), dis(gen)));
    }

    int x = clampint((int)((analysis_point.x) * image_res), 0, image_res-1);
    int y = clampint((int)((analysis_point.y) * image_res), 0, image_res-1);
    int pos = y * image_res + x;
    analysis_image[pos] = analysis_image[pos] + color;
    n_touched[pos]++;
    i++;
  }

  void save(std::string const & filename)
  {
    //writeEPS("points.eps", points, colors, 5, 512);

    for (int i = 0; i < 512 * 512; i++)
    {
      if (n_touched[i] > 0)
        analysis_image[i] = analysis_image[i] * (1. / n_touched[i]);
      analysis_image[i].clamp();
    }

    for (unsigned p = 0; p < points.size(); p++)
    {
      auto point = points[p];
      int x = clampint((int)((point.x) * image_res), 0, image_res-1);
      int y = clampint((int)((point.y) * image_res), 0, image_res-1);
      int pos = y * image_res + x;
      int w = image_res, point_width = 5;

      for (int i = -point_width; i <= point_width; i++)
      {
        for (int j = -point_width; j <= point_width; j++)
        {
          if (i * i + j * j > point_width * point_width)
            continue;
          int newpos = pos + i * w + j;
          if (newpos >= 0 && newpos < w * w)
            analysis_image[newpos] = colors[p];
        }
      }
    }

    write_ppm(analysis_image, output_folder + "/" + filename + ".ppm", image_res, image_res);
    write_exr_rgb<double>(output_folder + "/" + filename + ".exr", (const double *)analysis_image.data(), image_res, image_res);
  }
};

struct Experiment
{
  Scene *scene;
  Callback *callback;
  std::string filename;
};

int main(int argc, char *argv[])
{
  int w = 512, h = 384;
  int xanalysis = 206, yanalysis = h - 270;
  //int xanalysis = 255, yanalysis = h - 240;

  // cam pos, dir
  //Ray cam(Vec(50,52,295.6), Vec(0,-0.05,-1).norm()); 
  //Ray cam(Vec(50,75,100), Vec(0,-1,0.25).norm());
  Ray cam(Vec(50, 80, 295.6), Vec(0, -0.3, -1).norm());

  Vec cx = Vec(w * .5135 / h), cy = (cx % cam.d).norm() * .5135;
  std::vector<Vec> image(w * h);

  std::vector<Experiment> experiments;

  Quad *quad_light = new Quad(Vec(50, 80, 100), Vec(0, 0, -20), Vec(20, 0, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
  Mapping *quad_mapping = new QuadMapping(*quad_light);
  Mapping *quad_projection_mapping = nullptr;
  Callback quad_callback(quad_projection_mapping);
  Scene quad_scene(quad_light, new RegularSampler(), quad_mapping);
  experiments.push_back(Experiment{&quad_scene, &quad_callback, "quad"});

  Disk *disk_light = new Disk(10., Vec(50, 80, 100), Vec(0, -1, 0), Vec(10000, 10000, 10000), Vec(), DIFF);
  Mapping *polar_mapping = new PolarDiskMapping(*disk_light);
  Mapping *concentric_mapping = new ConcentricDiskMapping(*disk_light);
  Mapping *polar_projection_mapping = new PolarProjection(*disk_light);
  Callback polar_callback(polar_projection_mapping);
  Scene polar_scene(disk_light, new RegularSampler(), polar_mapping);
  experiments.push_back(Experiment{&polar_scene, &polar_callback, "polar"});

  for (auto experiment : experiments)
  {
    #pragma omp parallel for schedule(dynamic, 1) // OpenMP
    for (int y = 0; y < h; y++)
    { // Loop over image rows
      fprintf(stderr, "\rRendering (%d spp) %5.2f%%", 10, 100. * y / (h - 1));
      for (int x = 0; x < w; x++)
      { // Loop cols
        Vec d = cx * ((.5 + x) / w - .5) + cy * ((.5 + y) / h - .5) + cam.d;
        Ray ray(cam.o, d.norm()); 
        bool analysis = (x == xanalysis && y == yanalysis);
        Vec r = experiment.scene->radiance(ray, analysis ? n_samples_root * n_samples_root : 100, analysis ? experiment.callback : nullptr);

        image[(h - y - 1) * w + x] = Vec(clamp(r.x), clamp(r.y), clamp(r.z));
      }
    }
    write_ppm(image, "image.ppm", w, h);
    write_exr_rgb<double>("image.exr", (const double *)image.data(), 512, 384);

    experiment.callback->save(experiment.filename);
  }
}
