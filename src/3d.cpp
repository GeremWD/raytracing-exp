
#include "3d.h"
#include <algorithm>

bool intersectPlane(const Vec &n, const Vec &p0, const Vec &l0, const Vec &l, double &t)
{
  //Vec nn = n.dot(l) > 0 ? n : n*-1;
  Vec nn = n*-1;
  float denom = nn.dot(l);
  if (denom > 1e-6) {
    Vec p0l0 = p0 - l0;
    t = p0l0.dot(nn) / denom;
    return (t >= 0);
  }
  return false;
} 