#ifndef COLLIDERTRIANGLE_H
#define COLLIDERTRIANGLE_H

#include "collider_plane.h"

class ColliderTriangle : public Collider
{
public:
    ColliderTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float bouncing, float friction);

    bool collide(Particle &p) override;

    void correct(float dt, Particle &p) override;

    void transform(glm::mat4 m) override;

private:
    ColliderPlane collider_plane_;

    glm::vec3 p1_;
    glm::vec3 p2_;
    glm::vec3 p3_;

    float area_;
};

#endif // COLLIDERTRIANGLE_H
