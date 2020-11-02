#include "solver_euler.h"

void SolverEuler::solve(float dt, Particle &p) const
{
    p.pos_pre_ = p.pos_;

    p.pos_ = p.pos_ + dt * p.vel_;
    p.vel_ = p.vel_ + dt * p.force_ / p.mass_;
}
