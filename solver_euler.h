#ifndef SOLVEREULER_H
#define SOLVEREULER_H

#include "solver.h"

class SolverEuler : public Solver
{
public:
    void solve(float dt, Particle &p) const override;
};

#endif // SOLVEREULER_H
