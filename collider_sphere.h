#ifndef COLLIDERSPHERE_H
#define COLLIDERSPHERE_H

#include "collider.h"

class ColliderSphere : public Collider
{
public:
    ColliderSphere(glm::vec3 center, float radius, float bouncing, float friction);

    bool collide(Particle &p) override;

    void correct(float dt, Particle &p) override;

    void transform(glm::mat4 m) override;

    float bouncing_;
    float friction_;

private:
    glm::vec3 center_;
    float radius_;

    glm::vec3 i_;
};

#endif // COLLIDERSPHERE_H
