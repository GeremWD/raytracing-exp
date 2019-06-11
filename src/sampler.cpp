
#include "sampler.h"
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>

UniformSampler::UniformSampler() : gen(0), dis(0., 1.) {}

void UniformSampler::sample(std::vector<Vec> &points, int n) {
    for(int i = 0; i<n; i++) {
        points[i].x = dis(gen);
        points[i].y = dis(gen);
    }
}

std::string UniformSampler::name() { return "uniform";}


RegularSampler::RegularSampler() {}

void RegularSampler::sample(std::vector<Vec> &points, int n) {
    int width = round(sqrt((double)n));
    for(int x = 0; x < width; x++) {
        for(int y = 0; y<width; y++) {
            points[y*width+x] = Vec(x*(1. / width) + 0.5 / width, y*(1./width) + 0.5 / width, 0);
        }
    }
}

std::string RegularSampler::name() { return "regular";}


ArtSampler::ArtSampler() {
    std::fstream file("table.dat");
    loadTileSet(file);
}


void ArtSampler::sample(std::vector<Vec>& points, int n)
{
    int tileID = rand() % tileCount;
    for (int i = 0; i < n; i++) {
        points[i] = getSample(tileID, i);
    }
}

void ArtSampler::loadTileSet(std::fstream &file) {
    file >> scale >> idCount;
    file >> ONE;
    idList.resize(ONE);
    for (unsigned i = 0; i < ONE; i++) file >> idList[i];
    nChildren = scale * scale;
    mask = nChildren - 1;
    shift = round(log2(nChildren));
    tileCount = idCount * idCount;
    tiles.resize(tileCount);
    for (unsigned id = 0; id < tiles.size(); id++) {
        tiles[id].read(file, nChildren);
        if (file.eof()) {
            fprintf(stderr, "Failed to load tiles\n");
            exit(1);
        }
    }
    // Populate the matrix:
    M = new Matrix[nChildren];
    double step = 1.0 / scale;
    for (int Y = 0; Y < scale; Y++) {
        for (int X = 0; X < scale; X++) {
            M[Y * scale + X] = {step, step * X, step * Y};
        }
    }
    fprintf(stderr, "Loaded %4d tiles\n", (int)tiles.size());
}

Vec transform(const Vec &p, const Matrix &m) {
    double X = m.scale * p.x + m.tx;
    double Y = m.scale * p.y + m.ty;
    return {X, Y};
};

Vec ArtSampler::getSample(unsigned tileID, unsigned sampleNo) const {
    Matrix m = IDENTITY;
    while (sampleNo > 0) {
        unsigned childNo = tiles[tileID].order[ sampleNo & mask ];
        tileID = tiles[tileID].children[childNo];
        m = m.concat( M[childNo] );
        sampleNo = (sampleNo >> shift);
    }
    return transform(tiles[tileID].p, m);
}

std::string ArtSampler::name() { return "art";}


MultiJitteredSampler::MultiJitteredSampler() : gen(0), dis(0., 1.) {}

void MultiJitteredSampler::sample(std::vector<Vec> &points, int n) {
    int m = round(sqrt((double)n));
    for(int j = 0; j < m; j++) {
        for(int i = 0; i < m; i++) {
            points[j*m+i].x = (i+(j+dis(gen)) / m) / m;
            points[j*m+i].y = (i+(j+dis(gen)) / m) / m;
            points[j*m+i].z = 0;
        }
    }
    for(int j = 0; j < m; j++) {
        for(int i = 0; i < m; i++) {
            int k = j + dis(gen) * (m-j);
            std::swap(points[j*m+i].x, points[k*m+i].x);
        }
    }
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            int k = i + dis(gen) * (m-i);
            std::swap(points[j*m+i].y, points[j*m+k].y);
        }
    }
}

std::string MultiJitteredSampler::name() { return "multi_jittered";}


CorrelatedMultiJitteredSampler::CorrelatedMultiJitteredSampler() : gen(0), dis(0., 1.) {}

void CorrelatedMultiJitteredSampler::sample(std::vector<Vec> &points, int n) {
    int m = round(sqrt((double)n));
    for(int j = 0; j < m; j++) {
        for(int i = 0; i < m; i++) {
            points[j*m+i].x = (i+(j+dis(gen)) / m) / m;
            points[j*m+i].y = (i+(j+dis(gen)) / m) / m;
            points[j*m+i].z = 0;
        }
    }
    for(int j = 0; j < m; j++) {
        int k = j + dis(gen) * (m-j);
        for(int i = 0; i < m; i++) {
            std::swap(points[j*m+i].x, points[k*m+i].x);
        }
    }
    for(int i = 0; i < m; i++) {
        int k = i + dis(gen) * (m-i);
        for(int j = 0; j < m; j++) {
            std::swap(points[j*m+i].y, points[j*m+k].y);
        }
    }
}

std::string CorrelatedMultiJitteredSampler::name() { return "correlated_multi_jittered";}