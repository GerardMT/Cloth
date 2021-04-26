#include "collider_sphere.h"

#include "collider_plane.h"

ColliderSphere::ColliderSphere(glm::vec3 center, float radius, float bouncing, float friction)
{
    center_ = center;
    radius_ = radius;

    bouncing_ = bouncing;
    friction_ = friction;
}

bool ColliderSphere::collide(Particle &p)
{
    glm::vec3 l_v = p.pos_ - p.pos_pre_;

    float a = glm::dot(l_v, l_v);
    float b = glm::dot(2.0f * l_v, p.pos_pre_ - center_);
    float c = glm::dot(center_, center_) + glm::dot(p.pos_pre_, p.pos_pre_) - glm::dot(2.0f * p.pos_pre_, center_) - radius_ * radius_;

    float s = sqrt(b * b - 4.0 * a * c);
    float c_2 = 2.0f * c;

    float l_1 = c_2 / (-b + s);
    if (l_1 >= 0.0f && l_1 <= 1.0f) {
        i_ = p.pos_pre_ + l_1 * l_v;
        return true;
    } else {
        float l_2 = c_2 / (-b - s);
        if (l_2 >= 0.0f && l_2 <= 1.0f) {
            i_ = p.pos_pre_ + l_2 * l_v;
            return true;
        } else {
            return false;
        }
    }
}

void ColliderSphere::correct(float dt, Particle &p)
{
    glm::vec3 n = glm::normalize(i_ - center_);

    ColliderPlane collidrPlane(n, i_, bouncing_, friction_);
    collidrPlane.correct(dt, p);
}

void ColliderSphere::transform(glm::mat4 m)
{
    center_ = glm::vec3(m * glm::vec4(center_, 1.0));
}
