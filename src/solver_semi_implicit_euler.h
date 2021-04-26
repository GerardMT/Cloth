#ifndef SOLVERSEMIIMPLICITEULER_H
#define SOLVERSEMIIMPLICITEULER_H

#include "solver.h"

class SolverSemiImplicitEuler : public Solver
{
public:
    void solve(float dt, Particle &p) const override;
};

#endif // SOLVERSEMIIMPLICITEULER_H
