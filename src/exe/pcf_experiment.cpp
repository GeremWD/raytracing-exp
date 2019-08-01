
#include "pcf.h"
#include "sampler.h"
#include "util.h"
#include <iostream>
#include <cmath>
#include <random>
#include <sstream>

double RationalApproximation(double t)
{
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) / 
               (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

double NormalCDFInverse(double p)
{
    if (p <= 0.0 || p >= 1.0)
    {
        std::stringstream os;
        os << "Invalid input argument (" << p 
           << "); must be larger than 0 but less than 1.";
        throw std::invalid_argument( os.str() );
    }

    // See article above for explanation of this section.
    if (p < 0.5)
    {
        // F^-1(p) = - G^-1(p)
        return -RationalApproximation( sqrt(-2.0*log(p)) );
    }
    else
    {
        // F^-1(p) = G^-1(1-p)
        return RationalApproximation( sqrt(-2.0*log(1-p)) );
    }
}

int main() {
    int n_points = 1024, n_bins = 100, n_repeat = 1000;
    double rmax = 0.2, smoothing = 0.001;
    bool compute_uniform = true;
    bool soft_bining = true;
    bool custom_distribution = false;

    if(soft_bining) n_repeat = 1;

    std::string top_output_folder = "../output/pcf_output/uniform_density";
    std::string output_folder = top_output_folder + (soft_bining ?  "/soft" : "/hard");

    std::vector<double> norm(n_bins, 0);
    std::vector<Vec> points(n_points);
    std::vector<double> hist(n_bins);
    
    if(compute_uniform) {
        Sampler *uniform_sampler = new UniformSampler();

        for(int i = 0; i<n_repeat; i++) {
            uniform_sampler->sample(points, n_points);
            if(custom_distribution) {
                for(unsigned p = 0; p<points.size(); p++) {
                    points[p].x = (NormalCDFInverse(points[p].x) + 3.) / 6.;
                    points[p].y = (NormalCDFInverse(points[p].y) + 3.) / 6.;
                }
            }

            if(!soft_bining)
                pcf_hard_bining(points, hist, rmax);
            else
                pcf_soft_bining(points, hist, rmax, smoothing);
    
            for(int j = 0; j<n_bins; j++) {
                norm[j] += hist[j];
            }
        }
        for(int j = 0; j<n_bins; j++) {
            norm[j] /= n_repeat;
        }
        save_vec(output_folder + "/norm.txt", norm);
    } else {
        load_vec(output_folder + "/norm.txt", norm);
    }

    std::vector<Sampler*> samplers;
    samplers.push_back(new JitteredSampler());
    samplers.push_back(new UniformJitteredSampler());
    samplers.push_back(new UniformSampler());
    samplers.push_back(new ArtSampler());
    samplers.push_back(new MultiJitteredSampler());
    samplers.push_back(new CorrelatedMultiJitteredSampler());
    samplers.push_back(new RegularSampler());
    samplers.push_back(new PoissonDiskSampler(0.02));

    std::vector<double> average_hist(n_bins);

    for(auto sampler : samplers) {
        std::fill(average_hist.begin(), average_hist.end(), 0);

        for(int i = 0; i<n_repeat; i++) {
            sampler->sample(points, n_points);
            
            if(custom_distribution) {
                for(unsigned p = 0; p<points.size(); p++) {
                    points[p].x = (NormalCDFInverse(points[p].x) + 3.) / 6.;
                    points[p].y = (NormalCDFInverse(points[p].y) + 3.) / 6.;
                }
            }

            if(i==0) {
                writeEPS(output_folder+"/"+sampler->name()+"_points.eps", points, 1., 512);
            }

            if(!soft_bining)
                pcf_hard_bining(points, hist, rmax);
            else
                pcf_soft_bining(points, hist, rmax, smoothing);

            for(int j = 0; j<n_bins; j++) {
                if(!soft_bining)
                    average_hist[j] += hist[j] / norm[j];
                else
                    average_hist[j] += hist[j];
            }
        }
        for(int j = 0; j<n_bins; j++) {
            average_hist[j] /= n_repeat;
        }
        save_vec(output_folder + "/" + sampler->name() + ".txt", average_hist);
    }

    return 0;
}