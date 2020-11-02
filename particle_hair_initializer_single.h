#ifndef PARTICLEHAIRINITIALIZERSINGLE_H
#define PARTICLEHAIRINITIALIZERSINGLE_H

#include "particle_hair_initializer.h"

class ParticleHairInitializerSingle : public ParticleHairInitializer
{
public:
    void initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &path_length) override;
};

#endif // PARTICLEHAIRINITIALIZERSINGLE_H
