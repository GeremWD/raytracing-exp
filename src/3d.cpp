
#include "3d.h"
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

bool intersectPlane(const Vec &n, const Vec &p0, const Vec &l0, const Vec &l, double &t)
{
  Vec nn = n*-1;
    // assuming vectors are all normalized
    double denom = nn.dot(l);
    if (denom > 1e-6) {
      Vec p0l0 = p0 - l0;
      t = p0l0.dot(nn) / denom;
      return (t >= 0);
    }
    return false;
} 
/*
bool intersectPlane(const Vec &n, const Vec &p0, const Vec &l0, const Vec &l, double &t)
{
  double denom = n.dot(l);
  Vec nn;
  if(denom > 0) nn = n;
  else nn = -1;

  
  t = 0;
  if(denom < 1e-6) return false;
  Vec p0l0 = p0 - l0;
  t = abs(p0l0.dot(nn) / denom);
  return true;
}*/

void write_ppm(std::vector<Vec> &image, const std::string &filename, int w, int h) {
  FILE *f = fopen(filename.c_str(), "w");         // Write image to PPM file.
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
  for (int i=0; i<w*h; i++)
    fprintf(f,"%d %d %d ", toInt(image[i].x), toInt(image[i].y), toInt(image[i].z));
  fclose(f);
}