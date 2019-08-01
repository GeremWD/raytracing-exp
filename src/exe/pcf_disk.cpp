
#include "pcf.h"
#include "sampler.h"
#include "util.h"
#include <iostream>
#include <cmath>
#include <random>
#include <sstream>

int main()
{
    int n_points = 1024, n_bins = 100, n_repeat = 1000;
    double rmax = 0.5;
    bool compute_uniform = true;

    std::string output_folder = "../output/pcf_disk_output";

    std::vector<double> norm(n_bins, 0);
    std::vector<Vec> points(n_points);
    std::vector<double> hist(n_bins);
    std::vector<Disk2d> disks(n_points);

    std::mt19937 gen(42);

    std::uniform_real_distribution<double> dis(0., 0.02);

    if (compute_uniform)
    {
        Sampler *uniform_sampler = new UniformSampler();

        for (int i = 0; i < n_repeat; i++)
        {
            uniform_sampler->sample(points, n_points);
            for (int j = 0; j < n_points; j++)
            {
                disks[j].x = points[j].x;
                disks[j].y = points[j].y;
                disks[j].r = dis(gen);
            }

            pcf_2d_disk(disks, hist, rmax);

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

    std::vector<Sampler *> samplers;
    samplers.push_back(new JitteredSampler());
    samplers.push_back(new UniformJitteredSampler());
    samplers.push_back(new UniformSampler());
    samplers.push_back(new ArtSampler());
    samplers.push_back(new MultiJitteredSampler());
    samplers.push_back(new CorrelatedMultiJitteredSampler());
    samplers.push_back(new RegularSampler());
    samplers.push_back(new PoissonDiskSampler(0.02));

    std::vector<double> average_hist(n_bins);
    std::fill(average_hist.begin(), average_hist.end(), 0);

    for (auto sampler : samplers)
    {
        for (int i = 0; i < n_repeat; i++)
        {
            sampler->sample(points, n_points);
            for (int j = 0; j < n_points; j++)
            {
                disks[j].x = points[j].x;
                disks[j].y = points[j].y;
                disks[j].r = dis(gen);  
            }

            if(i == 0) {
                std::ofstream file(output_folder + "/" + sampler->name() + "_example.txt");
                for(int j = 0; j<n_points; j++) {
                    file << disks[j].x << " " << disks[j].y << " " << disks[j].r << std::endl;
                }
            }

            pcf_2d_disk(disks, hist, rmax);

            for (int j = 0; j < n_bins; j++)
            {
                average_hist[j] += hist[j] / norm[j];
            }
        }
        for (int j = 0; j < n_bins; j++)
        {
            average_hist[j] /= n_repeat;
        }
        save_vec(output_folder + "/" + sampler->name() + ".txt", average_hist);
    }

    return 0;
}