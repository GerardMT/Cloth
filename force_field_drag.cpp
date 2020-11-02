#include "force_field_drag.h"

ForceFieldDrag::ForceFieldDrag(float drag)
{
    drag_ = drag;
}

void ForceFieldDrag::apply(Particle &p) const
{
    p.force_ += -drag_ * p.vel_;
}
