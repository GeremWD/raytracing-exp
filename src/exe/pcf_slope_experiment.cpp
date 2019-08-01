
#include "pcf.h"
#include "sampler.h"
#include "util.h"
#include <iostream>
#include <cmath>
#include <random>
#include <sstream>

int main()
{
    int n_points = 1024, n_bins = 100, n_repeat = 10000;
    double rmax = 0.1, smoothing = 0.001;
    bool compute_uniform = true;

    std::string output_folder = "../output/pcf_output/poisson_disk";

    for (int r = 5; r <= 32; r++)
    {
        n_points = r*r;

        std::vector<double> norm(n_bins, 0);
        std::vector<Vec> points(n_points);
        std::vector<double> hist(n_bins);

        
        if (compute_uniform)
        {
            Sampler *uniform_sampler = new UniformSampler();

            for (int i = 0; i < n_repeat; i++)
            {
                uniform_sampler->sample(points, n_points);

                pcf_hard_bining(points, hist, rmax);

                for (int j = 0; j < n_bins; j++)
                {
                    norm[j] += hist[j];
                }
            }
            for (int j = 0; j < n_bins; j++)
            {
                norm[j] /= n_repeat;
            }
            save_vec(output_folder + "/norm.txt", norm);
        }
        else
        {
            load_vec(output_folder + "/norm.txt", norm);
        }

        //Sampler *sampler = new JitteredSampler();
        Sampler *sampler = new PoissonDiskSampler(0.02);

        std::vector<double> average_hist(n_bins, 0);

        for (int i = 0; i < n_repeat; i++)
        {
            sampler->sample(points, n_points);

            for(int j = 0; j<n_points; j++) {
                for(int k = j+1; k<n_points; k++) {
                    if((points[j] - points[k]).length() < 0.02) {
                        std::cout << "bug" << std::endl;
                    }
                }
            }


            pcf_hard_bining(points, hist, rmax);

            for (int j = 0; j < n_bins; j++)
            {
                average_hist[j] += hist[j] / norm[j];
            }
        }
        for (int j = 0; j < n_bins; j++)
        {
            average_hist[j] /= n_repeat;
        }
        save_vec(output_folder + "/poisson_disk_" + std::to_string(n_points) + ".txt", average_hist);
    }

    return 0;
}