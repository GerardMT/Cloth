#ifndef SOLVERVERLET_H
#define SOLVERVERLET_H

#include "solver.h"

class SolverVerlet : public Solver
{
public:
    void solve(float dt, Particle &p) const override;

private:
    const float k_ = 1.0f;
};

#endif // SOLVERVERLET_H
