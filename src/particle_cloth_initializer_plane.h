#ifndef PARTICLECLOTHINITIALIZERPLANE_H
#define PARTICLECLOTHINITIALIZERPLANE_H

#include "particle_cloth_initializer.h"

class ParticleClothInitializerPlane : public ParticleClothInitializer
{
public:
    ParticleClothInitializerPlane(unsigned int x_particles, unsigned int y_particles);

    void initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &x_particles, unsigned int &y_particles);

    unsigned int x_particles_;
    unsigned int y_particles_;

    bool fixed_;
};

#endif // PARTICLECLOTHINITIALIZERPLANE
