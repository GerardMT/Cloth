#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "particle.h"

class ForceField
{
public:
    virtual ~ForceField() {};

    virtual void apply(Particle &p) const = 0;
};

#endif // FORCEFIELD_H
