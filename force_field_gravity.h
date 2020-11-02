#ifndef FORCEFIELDGRAVITY_H
#define FORCEFIELDGRAVITY_H

#include "force_field.h"

class ForceFieldGravity : public ForceField
{
public:
    void apply(Particle &p) const override;

private:
    glm::vec3 GRAVITY_ = glm::vec3(0.0, -9.8, 0.0);
};

#endif // FORCEFIELDGRAVITY_H
