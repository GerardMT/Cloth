#ifndef PARTICLECLOTHINITIALIZER_H
#define PARTICLECLOTHINITIALIZER_H

#include "force.h"
#include "particle.h"

#include <vector>

using namespace std;

class ParticleClothInitializer
{
public:
    virtual ~ParticleClothInitializer() {};

    virtual void initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &x_particles, unsigned int &y_particles) = 0;
};

#endif // PARTICLECLOTHINITIALIZER_H
