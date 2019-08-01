
#include "3d.h"
#include "pcf.h"
#include "sampler.h"
#include "util.h"
#include <vector>


int main() {
    int n_points = 256, n_bins = 100, krmax = 5;
    double smoothing = 0.001;
    std::string output_folder = "../output/pcf_synthesis";

    double rmax = krmax * 2 * sqrt(1. / (2 * sqrt(3) * n_points));

    Sampler *sampler = new PoissonDiskSampler(0.02);
    std::vector<Vec> examples(n_points);
    sampler->sample(examples, n_points);
    

    std::vector<double> example_pcf(n_bins);
    pcf_soft_bining(examples, example_pcf, rmax, smoothing);
    save_vec(output_folder + "/example.txt", example_pcf);
    writeEPS(output_folder+"/example.eps", examples, 1., 512);

    /*std::vector<Vec> output(n_points);
    pcf_synthesis(examples, krmax, n_bins, output, 42);

    std::vector<double> output_pcf(n_bins);
    pcf_soft_bining(output, output_pcf, rmax, smoothing);
    save_vec(output_folder + "/output.txt", output_pcf);
   
    writeEPS(output_folder+"/output.eps", output, 1., 512);*/

    return 0;
}