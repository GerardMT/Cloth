#ifndef FORCEFIELDDRAG_H
#define FORCEFIELDDRAG_H

#include "force_field.h"

class ForceFieldDrag : public ForceField
{
public:
    ForceFieldDrag(float drag);

    void apply(Particle &p) const override;

private:
    float drag_;
};

#endif // FORCEFIELDDRAG_H
