#ifndef PARTICLEEMITTERINITIALIZERWATERFALL_H
#define PARTICLEEMITTERINITIALIZERWATERFALL_H

#include "particle_emitter_initializer.h"

class ParticleEmitterInitializerWaterfall : public ParticleEmitterInitializer
{
public:
    ParticleEmitterInitializerWaterfall(glm::vec3 pos, float speed, float spread);

    void initialize(float dt, Particle &p) override;

    glm::vec3 pos_;

    float speed_;
    float spread_;

    float mass_min_ = 0.1;
    float mass_max_ = 0.5;
};

#endif // PARTICLEEMITTERINITIALIZERWATERFALL_H
