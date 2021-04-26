#include "solver_semi_implicit_euler.h"

void SolverSemiImplicitEuler::solve(float dt, Particle &p) const
{
    p.pos_pre_ = p.pos_;

    p.vel_ = p.vel_ + dt * p.force_ / p.mass_;
    p.pos_ = p.pos_ + dt * p.vel_;
}
