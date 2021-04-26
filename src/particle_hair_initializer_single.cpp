#include "force_spring.h"
#include "particle_hair_initializer_single.h"

void ParticleHairInitializerSingle::initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &path_length)
{
    const int n_particles = 15;

    path_length = n_particles;

    particles.resize(n_particles);

    const glm::vec3 p_start(-0.4, 0.8, 0.0);
    const glm::vec3 p_end(0.6, 0.8, 0.0);

    glm::vec3 inc = (p_end - p_start) / static_cast<float>(n_particles);

    glm::vec3 pos = p_start;
    for (unsigned int i = 0; i < n_particles; ++i ) {
        particles[i].fixed_ = false;
        particles[i].pos_ = pos;
        particles[i].pos_pre_ = pos;
        particles[i].vel_ = glm::vec3(0.0, 0.0, 0.0);
        particles[i].mass_ = 0.02;

        pos += inc;
    }

    if (n_particles > 0) {
        particles[0].fixed_ = true;
    }

    for (auto f : forces) {
        delete f;
    }
    forces.clear();

    for (unsigned int i = 0; i < n_particles - 1; ++i) {
        forces.push_back(new ForceSpring(particles[i], particles[i + 1], 10.0, 0.2));
    }
}
