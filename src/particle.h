#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle
{
public:
    Particle();

    Particle(glm::vec3 pos, glm::vec3 vel, float mass);

    glm::vec3 pos_;
    glm::vec3 pos_pre_;
    glm::vec3 vel_;

    bool fixed_;

    float mass_;
    glm::vec3 force_;

    float life_time_;
};

#endif // PARTICLE_H
