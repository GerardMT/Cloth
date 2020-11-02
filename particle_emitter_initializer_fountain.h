#ifndef PARTICLEEMITTERINITIALIZERFOUNTAIN_H
#define PARTICLEEMITTERINITIALIZERFOUNTAIN_H

#include "particle_emitter_initializer.h"

class ParticleEmitterInitializerFountain : public ParticleEmitterInitializer
{
public:
    ParticleEmitterInitializerFountain(glm::vec3 pos, float speed, float spread);

    void initialize(float dt, Particle &p) override;

    glm::vec3 pos_;
    float speed_;
    float spread_;

    float mass_min_ = 0.1;
    float mass_max_ = 0.5;
};

#endif // PARTICLEEMITTERINITIALIZERFOUNTAIN_H
