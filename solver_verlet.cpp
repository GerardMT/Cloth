#include "solver_verlet.h"

void SolverVerlet::solve(float dt, Particle &p) const
{
    glm::vec3 pos = p.pos_;
    p.pos_ = p.pos_ + k_ * (p.pos_ - p.pos_pre_) + dt * dt * p.force_ / p.mass_;
    p.vel_ = (p.pos_ - pos) / dt;
    p.pos_pre_ = pos;
}
