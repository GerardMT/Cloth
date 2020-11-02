#ifndef PARTICLEHAIRINITIALIZERCURLY_H
#define PARTICLEHAIRINITIALIZERCURLY_H

#include "particle_hair_initializer.h"

class ParticleHairInitializerCurly : public ParticleHairInitializer
{
public:
    ParticleHairInitializerCurly(glm::vec3 pos);

    void initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &path_length) override;

    glm::vec3 pos_;
};

#endif // PARTICLEHAIRINITIALIZERCURLY_H
