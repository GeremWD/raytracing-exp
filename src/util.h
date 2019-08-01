
#include <vector>
#include <string>
#include <iostream>
#include "3d.h"

template <typename T> void save_vec(std::string const & filename, std::vector<T> &vec) {
    std::ofstream file(filename);
    for(unsigned i = 0; i<vec.size(); i++) {
        file << vec[i] << std::endl;
    }
}

template <typename T> void load_vec(std::string const & filename, std::vector<T> &vec) {
    std::ifstream file(filename);
    for(unsigned i = 0; i<vec.size(); i++) {
        file >> vec[i];
    }
}

void writeEPS(std::string const & _filename, const std::vector<Vec>& pts, double radius, double scale);