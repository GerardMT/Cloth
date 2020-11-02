#include "collider_plane.h"

ColliderPlane::ColliderPlane()
{
}

ColliderPlane::ColliderPlane(glm::vec3 normal, glm::vec3 point, float bouncing, float friction)
{
    n_ = normal;
    d_ = -glm::dot(normal, point);
    p_ = point;

    bouncing_ = bouncing;
    b_ = 1.0 + bouncing;

    friction_ = friction;
}

bool ColliderPlane::collide(Particle &p)
{
    return (glm::dot(n_, p.pos_) + d_) * (glm::dot(n_, p.pos_pre_) + d_) <= 0;
}

void ColliderPlane::correct(float dt, Particle &p)
{
    p.pos_ = p.pos_ - (b_) * (glm::dot(n_, p.pos_) + d_) * n_;

    glm::vec3 v_n = glm::dot(n_, p.vel_) * n_;
    p.vel_ = p.vel_ - (b_) * v_n - friction_ * (p.vel_ - v_n);

    p.pos_pre_ = p.pos_ - dt * p.vel_;
}

float ColliderPlane::bounding()
{
    return bouncing_;
}

void ColliderPlane::bouncing(float b)
{
    bouncing_ = b;
}

void ColliderPlane::transform(glm::mat4 m)
{
    // TODO
}

