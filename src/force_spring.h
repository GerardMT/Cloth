#ifndef FORCESPRING_H
#define FORCESPRING_H

#include "force.h"
#include "particle.h"

class ForceSpring : public Force
{
public:
    ForceSpring(Particle &p1, Particle &p2, float k_e, float k_d, float length_coef = 1.0f);

    void apply() const override;

    Particle *p1_;
    Particle *p2_;

    float k_e_;
    float k_d_;
    float length_;
};

#endif // FORCESPRING_H
