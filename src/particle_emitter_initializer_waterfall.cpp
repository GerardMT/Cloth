#include "particle_emitter_initializer_waterfall.h"

ParticleEmitterInitializerWaterfall::ParticleEmitterInitializerWaterfall(glm::vec3 pos, float speed, float spread)
{
    pos_ = pos;
    speed_ = speed;
    spread_ = spread;
}

void ParticleEmitterInitializerWaterfall::initialize(float dt, Particle &p)
{
    p.fixed_ = false;

    p.pos_pre_ = pos_;

    float i = M_PI / 2.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (M_PI / 2.0 * spread_)));
    float a = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0 * M_PI)));

    p.vel_.x = speed_ * sin(i) * cos(a);
    p.vel_.z = speed_ * sin(i) * sin(a);
    p.vel_.y = speed_ * cos(i);

    p.mass_ = mass_min_ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mass_max_ - mass_min_)));

    p.pos_ = p.pos_pre_ + dt * p.vel_;
}
