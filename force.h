#ifndef FORCE_H
#define FORCE_H

class Force
{
public:
    virtual ~Force() {};

    virtual void apply() const = 0;
};

#endif // FORCE_H
