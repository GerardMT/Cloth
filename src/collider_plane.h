#ifndef COLLIDERPLANE_H
#define COLLIDERPLANE_H

#include "collider.h"

class ColliderPlane : public Collider
{
public:
    ColliderPlane();

    ColliderPlane(glm::vec3 normal, glm::vec3 point, float bouncing, float friction);

    bool collide(Particle &p) override;

    void correct(float dt, Particle &p) override;

    float bounding();

    void bouncing(float b);

    void transform(glm::mat4 m) override;

    glm::vec3 n_;
    glm::vec3 p_;

    float d_;

    float friction_;

private:
    float bouncing_;
    float b_;
};

#endif // COLLIDERPLANE_H
