
#pragma once

#include <cmath>
#include <algorithm>

//#define double float

inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
inline int clampint(int x, int min, int max) { return x < min ? min : x > max ? max : x; }
inline int toInt(double x){ return int(pow(clamp(x),1/2.2)*255+.5); }

struct Vec {        // Usage: time ./smallpt 5000 && xv image.ppm
  double x, y, z;                  // position, also color (r,g,b)
  Vec(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; }
  Vec operator+(const Vec &b) const { return Vec(x+b.x,y+b.y,z+b.z); }
  Vec operator-(const Vec &b) const { return Vec(x-b.x,y-b.y,z-b.z); }
  Vec operator*(double b) const { return Vec(x*b,y*b,z*b); }
  Vec mult(const Vec &b) const { return Vec(x*b.x,y*b.y,z*b.z); }
  Vec& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }
  double length() const { return sqrt(x*x+y*y+z*z); }
  double dot(const Vec &b) const { return x*b.x+y*b.y+z*b.z; } // cross:
  Vec operator%(const Vec&b) const {return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
  Vec& abs() {x=fabs(x); y=fabs(y); z=fabs(z); return *this;}
  Vec& positive() {x=std::max(0., x); y=std::max(0., y); z = std::max(0., z); return *this;}
  Vec& clamp() {x = ::clamp(x); y = ::clamp(y); z = ::clamp(z); return *this;}
};
struct Ray { Vec o, d; Ray(Vec o_, Vec d_) : o(o_), d(d_) {} };

bool intersectPlane(const Vec &n, const Vec &p0, const Vec &l0, const Vec &l, double &t);

