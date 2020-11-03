#include "force_spring.h"
#include "particle_cloth_initializer_plane.h"

#include <iostream>

using namespace std;

ParticleClothInitializerPlane::ParticleClothInitializerPlane(unsigned int x_particles, unsigned int y_particles)
{
    x_particles_ = x_particles;
    y_particles_ = y_particles;

    fixed_ = true;
}

void ParticleClothInitializerPlane::initialize(vector<Particle> &particles, vector<Force *> &forces, unsigned int &x_particles, unsigned int &y_particles)
{
    for (auto f : forces) {
        delete f;
    }
    forces.clear();

    const float size_half = 0.7;

    glm::vec3 p1(-size_half, 0.8, -size_half);
    glm::vec3 p2(size_half, 0.8, -size_half);
    glm::vec3 p3(-size_half, 0.8, size_half);

    x_particles = x_particles_;
    y_particles = y_particles_;

    glm::vec3 dx = (p2 - p1) / static_cast<float>(x_particles_);
    glm::vec3 dy = (p3 - p1) / static_cast<float>(y_particles_);

    particles.resize(x_particles_ * y_particles_);

    for (unsigned int x = 0; x < x_particles_; ++x) {
        for (unsigned int y = 0; y < y_particles_; ++y) {
            unsigned int i = x_particles * y + x;

            glm::vec3 pos = p1 + dx * static_cast<float>(x) + dy * static_cast<float>(y);

            particles[i].fixed_ = false;
            particles[i].pos_ = pos;
            particles[i].pos_pre_ = pos;
            particles[i].vel_ = glm::vec3(0.0, 0.0, 0.0);
            particles[i].mass_ = 0.01;
        }
    }

    if (fixed_) {
        for (unsigned int y = 0; y < y_particles_; ++y) {
            particles[y * x_particles_ + x_particles_ - 1].fixed_ = true;
        }
    }

    for (unsigned int x = 0; x < x_particles_; ++x) {
        for (unsigned int y = 0; y < y_particles_; ++y) {
        }
    }

    // Streach
    const float streach_k_e = 1.0;
    const float streach_k_d = 0.05;

    // Horizontal
    for (unsigned int x = 0; x < x_particles_ - 1; ++x) {
        for (unsigned int y = 0; y < y_particles_; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * y + x + 1], streach_k_e, streach_k_d));
        }
    }

    // Vertical
    for (unsigned int x = 0; x < x_particles_; ++x) {
        for (unsigned int y = 0; y < y_particles_ - 1; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * (y + 1) + x], streach_k_e, streach_k_d));
        }
    }

    // Shear
    const float shear_k_e = 1.0;
    const float shear_k_d = 0.05;

    // Diagonal 1
    for (unsigned int x = 0; x < x_particles_ - 2; ++x) {
        for (unsigned int y = 0; y < y_particles_ - 2; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * (y + 2) + x + 2], shear_k_e, shear_k_d));
        }
    }

    // Diagonal 2
    for (unsigned int x = 0; x < x_particles_ - 2; ++x) {
        for (unsigned int y = 2; y < y_particles_; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * (y - 2) + x + 2], shear_k_e, shear_k_d));
        }
    }

    // Bend
    const float bend_k_e = 1.0;
    const float bend_k_d = 0.05;

    // Horizontal
    for (unsigned int x = 0; x < x_particles_ - 4; ++x) {
        for (unsigned int y = 0; y < y_particles_; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * y + x + 4], bend_k_e, bend_k_d));
        }
    }

    // Vertical
    for (unsigned int x = 0; x < x_particles_; ++x) {
        for (unsigned int y = 0; y < y_particles_ - 4; ++y) {
            forces.push_back(new ForceSpring(particles[x_particles * y + x], particles[x_particles * (y + 4) + x], bend_k_e, bend_k_d));
        }
    }
}
