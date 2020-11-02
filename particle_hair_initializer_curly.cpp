#include "force_spring.h"
#include "particle_hair_initializer_curly.h"

#include <iostream>

using namespace std;

ParticleHairInitializerCurly::ParticleHairInitializerCurly(glm::vec3 pos)
{
    pos_ = pos;
}

void ParticleHairInitializerCurly::initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &path_length)
{
    const float radius = 0.55;
    const int a_steps = 15;
    const int i_steps = 5;
    const float length = 0.45;
    const float i_start = glm::radians(45.0f);

    const float hair_particles = 10;

    path_length = hair_particles;

    for (auto f : forces) {
        delete f;
    }
    forces.clear();

    particles.resize(a_steps * i_steps * hair_particles);

    double a_inc = (2.0f * M_PI) / static_cast<float>(a_steps);
    double i_inc = ((M_PI / 2.0f) - i_start) / static_cast<float>(i_steps);

    unsigned int first_index = 0;

    float a = 0.0f;
    for (unsigned a_i = 0; a_i < a_steps; ++a_i) {
        float i = i_start;
        for (unsigned i_i = 0; i_i < i_steps; ++i_i) {
            double a_rand = a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (M_PI / 2.0)));
            glm::vec3 dir = glm::vec3(sin(i) * cos(a_rand), cos(i), sin(i) * sin(a_rand));

            glm::vec3 p_start = pos_ + dir * radius;
            glm::vec3 p_end = pos_ + dir * (length + radius);

            glm::vec3 inc = (p_end - p_start) / static_cast<float>(hair_particles);

            glm::vec3 pos = p_start;
            for (unsigned int k = 0; k < hair_particles; ++k ) {
                particles[first_index + k].fixed_ = false;
                particles[first_index + k].pos_ = pos;
                particles[first_index + k].pos_pre_ = pos;
                particles[first_index + k].vel_ = glm::vec3(0.0, 0.0, 0.0);
                particles[first_index + k].mass_ = 0.01;

                pos += inc;
            }

            if (hair_particles > 0) {
                particles[first_index].fixed_ = true;
            }

            for (unsigned int k = first_index; k < first_index + hair_particles - 1; ++k) {
                forces.push_back(new ForceSpring(particles[k], particles[k + 1], 4.0, 0.05, 1.5));
            }

            for (unsigned int k = first_index; k < first_index + hair_particles - 2; k += 2) {
                forces.push_back(new ForceSpring(particles[k], particles[k + 2], 7.0, 0.05, 0.4));
            }

            first_index += hair_particles;
            i += i_inc;
        }
        a += a_inc;
    }
}
