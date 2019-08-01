
#pragma once

#include <vector>
#include <random>
#include <fstream>
#include "3d.h"
#include "primitive.h"

class Sampler {
public:
    virtual void sample(std::vector<Vec> &points, int n)=0;
    virtual std::string name()=0;
};

class UniformSampler : public Sampler {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

public:
    UniformSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};

class RegularSampler : public Sampler {
public:
    RegularSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};

struct Matrix { // Postscript-style matrix, but only with scale and translate
    double scale, tx, ty;
    Matrix concat(const Matrix &transformation) const {
        Matrix result;
        result.scale = transformation.scale * scale;
        result.tx = transformation.tx * scale + tx;
        result.ty = transformation.ty * scale + ty;
        return result;
    };
};

struct Tile {
    Vec p;
    std::vector<unsigned> children; // ids of sub-tiles
    std::vector<unsigned> order;

    std::fstream& read(std::fstream &file, int nChildren) {
        file >> p.x >> p.y;
        children.resize(nChildren);
        for (int i = 0; i < nChildren; i++) {
            file >> children[i];
        }
        order.resize(nChildren);
        for (int i = 0; i < nChildren; i++) {
            file >> order[i];
        }
        return file;
    };
};

class ArtSampler: public Sampler
{
private:
    void loadTileSet(std::fstream &file);

    Vec getSample(unsigned tileID, unsigned sampleNo) const;

    int idCount, tileCount;
    unsigned ONE;
    std::vector<unsigned> idList;
    int scale;
    int nChildren;
    unsigned mask;
    unsigned shift;
    const Matrix IDENTITY = {1, 0, 0};
    Matrix *M;

    std::vector<Tile> tiles;

public:
    ArtSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};


class JitteredSampler : public Sampler
{
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

public:
    JitteredSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};


class UniformJitteredSampler : public Sampler {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
public:
    UniformJitteredSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;    
};


class MultiJitteredSampler : public Sampler
{
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

public:
    MultiJitteredSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};    


class CorrelatedMultiJitteredSampler : public Sampler
{
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

public:
    CorrelatedMultiJitteredSampler();
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};    


class PoissonDiskSampler : public Sampler
{
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
    double radius;

public:
    PoissonDiskSampler(double _radius);
    void sample(std::vector<Vec> &points, int n) override;
    std::string name() override;
};
