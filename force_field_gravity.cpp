#include "force_field_gravity.h"

void ForceFieldGravity::apply(Particle &p) const
{
    p.force_ += GRAVITY_ * p.mass_;
}
