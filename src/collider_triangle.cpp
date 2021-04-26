#include "collider_triangle.h"

ColliderTriangle::ColliderTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float bouncing, float friction)
{
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;

    glm::vec3 c = glm::cross(p2 - p1, p3 - p1);

    area_ = 0.5 * glm::length(c);

    glm::vec3 n = glm::normalize(c);
    collider_plane_ = ColliderPlane(n, p1, bouncing, friction);
}

float area(glm::vec3 i, glm::vec3 j, glm::vec3 k)
{
     return 0.5 * glm::length(glm::cross(j - i, k - i));
}

bool ColliderTriangle::collide(Particle &p)
{
    if (collider_plane_.collide(p)) {
        glm::vec3 l_v = p.pos_ - p.pos_pre_;

        if (glm::dot(l_v, collider_plane_.n_) == 0) { // Line and plane are parallel
            return false;
        }

        float l_d = glm::dot(p1_ - p.pos_pre_, collider_plane_.n_) / glm::dot(l_v, collider_plane_.n_);

        glm::vec3 i = p.pos_pre_ + l_v * l_d;

        return area(i, p2_, p3_) + area(p1_, i, p3_) + area(p1_, p2_, i) <= area_;
    } else {
        return false;
    }
}

void ColliderTriangle::correct(float dt, Particle &p)
{
    collider_plane_.correct(dt, p);
}

void ColliderTriangle::transform(glm::mat4 m)
{
    // TODO
}
