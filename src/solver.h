#ifndef SOLVER_H
#define SOLVER_H

#include "particle.h"

class Solver
{
public:
    virtual ~Solver() {};

    virtual void solve(float dt, Particle &p) const = 0;
};

#endif // SOLVER_H
