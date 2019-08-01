
#include "pcf.h"
#include <algorithm>
#include <iostream>
#include <random>


void pcf_hard_bining(std::vector<Vec> &pts, std::vector<double> &hist, double rmax)
{
    double rmin = 0;
    std::fill(hist.begin(), hist.end(), 0);
    auto n_bins = hist.size();
    for (auto i = 0; i < pts.size(); i++)
    {
        for (auto j = i + 1; j < pts.size(); j++)
        {
            double d = (pts[i] - pts[j]).length();
            if (d < rmin || d >= rmax)
                continue;
            int bin = (int)((d - rmin) / ((rmax - rmin) / n_bins));
            hist[bin] += 1.;
        }
    }
}

void normalize_histogram(std::vector<double> &hist)
{
    double sum = 0;
    for (unsigned i = 0; i < hist.size(); i++)
    {
        sum += hist[i];
    }
    for (auto i = 0; i < hist.size(); i++)
    {
        hist[i] /= sum;
    }
}

double gaussianKernel(const double sigma, const double x)
{
    return (1.0 / (sqrt(2 * M_PI) * sigma)) * exp(-0.5 * (x * x) / (sigma * sigma));
}

/*
void pcf_soft_bining(std::vector<Vec> &pts, std::vector<double> &hist, double rmin, double rmax, double smoothing)
{
    std::fill(hist.begin(), hist.end(), 0);
    auto n_bins = hist.size();
    for (int bin = 0; bin < n_bins; bin++)
    {
        for (auto i = 0; i < pts.size(); i++)
        {
            for (auto j = i + 1; j < pts.size(); j++)
            {
                double d = (pts[i] - pts[j]).length();
                double r = rmin + (bin + 0.5) * (rmax - rmin) / (double)n_bins;
                double val = gaussianKernel(smoothing, r - d);
                hist[bin] += val;
            }
        }
    }
}*/

void pcf_soft_bining(std::vector<Vec> &pts, std::vector<double> &hist, double rmax, double smoothing)
{
    double rmin = 0;

    std::fill(hist.begin(), hist.end(), 0);
    
    int N = pts.size();

    double ra = rmin; //*smoothing;
    double rb = rmax;

    int nbbins = hist.size();
    
    //int pcfid=0;
    double step = (rb-ra)/(double)nbbins;

    for (int pcfid = 0; pcfid < nbbins; ++pcfid)
    {
        double r = ra + (pcfid + 0.5) * (rb - ra) / (double)nbbins;

        double estimator = 0;
        //double n = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                //if(i==j) continue;

                //for the pair (i,j)
                double d = (pts[i] - pts[j]).length();
                double val = gaussianKernel(smoothing, r - d);
                estimator += val;

                //for the pair (j,i)
                estimator += val;
            }
        }
        double cov = 1.0 - ((2.0 * r) / M_PI) * 2.0 + ((r * r) / M_PI); //2.0*rmax*rmax*acos(r/(2.0*rmax)) - (rmax/2.0)*(sqrt(4.0*rmax*rmax - r*r));
        double factor = 2.0 * M_PI * r * cov;
        factor = 1.0 / factor;

        estimator *= factor;

        //estimator /= (N * (N - 1));

        /*double ar = M_PI * ((r+0.5*step) * (r+0.5*step) - (r-0.5*step)*(r-0.5*step));
        estimator /= ar;
        estimator /= N*N;*/

        hist[pcfid] = estimator;
    }
}

bool is_new_pcf_ok(std::vector<double> &new_pcf, std::vector<double> &target_pcf, double threshold) {
    for(int i = 0; i<new_pcf.size(); i++) {
        if(new_pcf[i] - target_pcf[i] > threshold) {
            //std::cout << "miss " << i << std::endl;
            return false;
        }
    }
    return true;
}

void pcf_synthesis(std::vector<Vec> &example, int krmax, int n_bins, std::vector<Vec> &points, int seed) {
    int n = example.size();
    double rmax = krmax * 2 * sqrt(1. / (2 * sqrt(3) * n));
    double smoothing = 0.001;
    double threshold = 0.5;
    
    std::vector<double> target_pcf(n_bins);
    pcf_soft_bining(points, target_pcf, rmax, smoothing);

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(0., 1.);
    
    points.resize(0);
    std::vector<double> new_pcf(n_bins);

    int n_try = 0;
    while(points.size() < n) {

        /*if(n_try == 100) {
            threshold += 0.1;
            std::cout << threshold << std::endl;
            n_try = 0;
        } */
        
        Vec point(dis(gen), dis(gen), 0.);
        points.push_back(point);
        pcf_soft_bining(points, new_pcf, rmax, smoothing);

        /*for(int i = 0; i<n_bins; i++) {
            new_pcf[i] *= points.size() * (points.size()-1) / (n*(n-1));
        }*/

        if(is_new_pcf_ok(new_pcf, target_pcf, threshold)) {
            n_try = 0;
            std::cout << points.size() << std::endl;
            continue;
        } 
        points.pop_back();
        n_try++;

    }
}


double disk_distance(Disk2d &a, Disk2d &b) {
    double r1 = std::max(a.r, b.r), r2 = std::min(a.r, b.r);
    double d = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    double extent = std::max(d + r1 + r2, 2*r1);
    double overlap = r1 + r2 - d;
    if(overlap < 0) overlap = 0;
    if(overlap > 2*r2) overlap = 2*r2;
    return extent - overlap + d + r1 - r2;
}


void pcf_2d_disk(std::vector<Disk2d> &disks, std::vector<double> &hist, double rmax)
{
    double rmin = 0;
    std::fill(hist.begin(), hist.end(), 0);
    auto n_bins = hist.size();
    for (auto i = 0; i < disks.size(); i++)
    {
        for (auto j = i + 1; j < disks.size(); j++)
        {
            double d = disk_distance(disks[i], disks[j]);
            if (d < rmin || d >= rmax)
                continue;
            int bin = (int)((d - rmin) / ((rmax - rmin) / n_bins));
            hist[bin] += 1.;
        }
    }
}





