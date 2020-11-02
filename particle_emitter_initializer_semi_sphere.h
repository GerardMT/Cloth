#ifndef PARTICLEEMITTERINITIALIZERSEMISPHERE_H
#define PARTICLEEMITTERINITIALIZERSEMISPHERE_H

#include "particle_emitter_initializer.h"

class ParticleEmitterInitializerSemiSphere : public ParticleEmitterInitializer
{
public:
    ParticleEmitterInitializerSemiSphere(glm::vec3 pos_, float speed, float radius);

    void initialize(float dt, Particle &p) override;

    glm::vec3 pos_;

    float speed_;
    float radius_;

    float mass_min_ = 0.1;
    float mass_max_ = 0.5;
};

#endif // PARTICLEEMITTERINITIALIZERSEMISPHERE_H
