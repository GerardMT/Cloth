#ifndef COLLIDER_H
#define COLLIDER_H

#include "Transform.h"
#include "particle.h"

class Collider : public Transform
{
public:
    virtual ~Collider() {};

    virtual bool collide(Particle &p) = 0;

    virtual void correct(float dt, Particle &p) = 0; // TODO Remove
};

#endif // COLLIDER_H
