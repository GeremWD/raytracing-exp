#include "util.h"
#include "3d.h"
#include <fstream>
#include <string>

void writeEPS(std::string const &_filename, const std::vector<Vec> &pts, double radius, double scale)
{
    std::string filename = _filename;
    if (filename.compare(filename.size() - 4, 4, ".eps") != 0)
    {
        filename.erase(filename.end() - 4, filename.end());
        filename += ".eps";
    }

    std::ofstream os;
    os.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    radius /= scale;

    os << "%!PS-Adobe-3.1 EPSF-3.0\n";
    os << "%%HiResBoundingBox: " << -radius << " " << -radius << " " << scale + radius << " " << scale + radius << "\n";
    os << "%%BoundingBox: " << -radius << " " << -radius << " " << scale + radius << " " << scale + radius << "\n";
    os << "%%CropBox: " << -radius << " " << -radius << " " << scale + radius << " " << scale + radius << "\n";
    os << "/radius { " << radius << " } def\n";
    os << "/p { radius 0 360 arc closepath fill stroke } def\n";
    os << "gsave " << scale << " " << scale << " scale\n";

    os << "0 0 0 setrgbcolor\n";

    for (unsigned int i = 0; i < pts.size(); ++i)
    {
        os << pts[i].x << " " << pts[i].y << " p\n";
    }
    os << "grestore\n";
    os.close();
}
