#ifndef PARTICLEHAIRINITIALIZER_H
#define PARTICLEHAIRINITIALIZER_H

#include "force.h"
#include "particle.h"

#include <vector>

using namespace std;

class ParticleHairInitializer
{
public:
    virtual ~ParticleHairInitializer() {};

    virtual void initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &path_length) = 0;
};

#endif // PARTICLEHAIRINITIALIZER_H
