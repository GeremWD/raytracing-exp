
#include "3d.h"

void pcf_hard_bining(std::vector<Vec> &pts, std::vector<double> &hist, double rmax);
void pcf_soft_bining(std::vector<Vec> &pts, std::vector<double> &hist, double rmax, double smoothing);

void normalize_histogram(std::vector<double> &hist, std::vector<double> &norm);

void pcf_synthesis(std::vector<Vec> &example, int krmax, int n_bins, std::vector<Vec> &points, int seed);

void pcf_2d_disk(std::vector<Disk2d> &disks, std::vector<double> &hist, double rmax);