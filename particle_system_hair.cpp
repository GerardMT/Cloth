#include "particle_system_hair.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "force_field_gravity.h"
#include "util.h"

using namespace std;

const float quad_vertices[] = {
  -1.0f,  1.0f, 0.0f,
  -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   1.0f,  1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f
};

ParticleSystemHair::ParticleSystemHair(Solver &s, ParticleHairInitializer &i)
{
    solver_ = &s;

    paint_particles_ = true;
    paint_path_ = true;

    i.initialize(particles_, forces_, path_length_);
}

ParticleSystemHair::~ParticleSystemHair()
{
    glDeleteVertexArrays(1, &vao_particles_);
    glDeleteBuffers(1, &vbo_particles_);
    glDeleteBuffers(1, &tbo_particles_);
    glDeleteBuffers(1, &cdbo_particles_);

    glDeleteVertexArrays(1, &vao_path_);
    glDeleteBuffers(1, &vbo_path_);

    for (auto f : forces_) {
        delete f;
    }
}

void ParticleSystemHair::addForceField(ForceField &f)
{
    force_fields_.push_back(&f);
}

void ParticleSystemHair::addCollider(Collider &c)
{
    collliders_.push_back(&c);
}

void ParticleSystemHair::solver(Solver &s)
{
    solver_ = &s;
}

void ParticleSystemHair::particleInitializer(ParticleHairInitializer &i)
{
    i.initialize(particles_, forces_, path_length_);
}

void ParticleSystemHair::initialieGL()
{
    string vert_str;
    readFile("../shader/particle.vert", vert_str);
    program_particles_.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_str.c_str());
    cout << program_particles_.log().toUtf8().constData() << endl;

    string frag_str;
    readFile("../shader/particle.frag", frag_str);
    program_particles_.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_str.c_str());
    cout << program_particles_.log().toUtf8().constData() << endl;

    program_particles_.link();

    glGenVertexArrays(1, &vao_particles_);
    glBindVertexArray(vao_particles_);

    glGenBuffers(1, &vbo_particles_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particles_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo_particles_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_particles_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &cdbo_particles_);
    glBindBuffer(GL_ARRAY_BUFFER, cdbo_particles_);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    readFile("../shader/path.vert", vert_str);
    program_path_.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_str.c_str());
    cout << program_path_.log().toUtf8().constData() << endl;

    readFile("../shader/path.frag", frag_str);
    program_path_.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_str.c_str());
    cout << program_path_.log().toUtf8().constData() << endl;

    program_path_.link();

    glGenVertexArrays(1, &vao_path_);
    glBindVertexArray(vao_path_);

    glGenBuffers(1, &vbo_path_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_path_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ParticleSystemHair::paintGL(float dt, const Camera &camera)
{
    for (auto &p : particles_) {
        p.force_ = glm::vec3(0.0);
    }

    for (auto f : forces_) {
        f->apply();
    }

    for (auto &p : particles_) {
        for (auto &f : force_fields_) {
            f->apply(p);
        }

        if (!p.fixed_) {
            solver_->solve(dt, p);

            for (auto &c : collliders_) {
                if(c->collide(p)) {
                    c->correct(dt, p);
                    break;
                }
            }
        }
    }

    if (paint_path_) {
        program_path_.bind();
        glUniformMatrix4fv(program_path_.uniformLocation("view_projection"), 1, GL_FALSE, glm::value_ptr(camera.view_projection));

        vector<glm::vec3> pos;
        pos.resize(particles_.size());

        for (unsigned int i = 0; i < particles_.size(); ++i) {
            pos[i] = particles_[i].pos_;
        }

        glBindVertexArray(vao_path_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_path_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_.size(), &pos[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        for (unsigned int i = 0; i < particles_.size(); i += path_length_) {
            glDrawArrays(GL_LINE_STRIP, i, path_length_);
        }

        glBindVertexArray(0);
    }

    if (paint_particles_) {
        program_particles_.bind();
        glUniformMatrix4fv(program_particles_.uniformLocation("view_projection"), 1, GL_FALSE, glm::value_ptr(camera.view_projection));

        struct Sort {
            glm::vec3 pos;
            glm::vec3 camera_dir;
            float camera_dist;
        };

        vector<Sort> particles;
        particles.resize(particles_.size());

        for (unsigned int i = 0; i < particles_.size(); ++i) {
            particles[i].pos = particles_[i].pos_;
            glm::vec3 v = particles_[i].pos_ - camera.pos_;
            particles[i].camera_dir = glm::normalize(v);
            particles[i].camera_dist = glm::length(v);
        }

        sort(particles.begin(), particles.end(), [](const Sort &a, const Sort &b) -> bool
        {
            return a.camera_dist > b.camera_dist;
        });

        vector<glm::vec3> pos;
        pos.resize(particles.size());

        vector<glm::vec3> camera_dir;
        camera_dir.resize(particles.size());

        for (unsigned int i = 0; i < particles.size(); ++i) {
            pos[i] = particles[i].pos;
            camera_dir[i] = particles[i].camera_dir;
        }

        glBindVertexArray(vao_particles_);
        glBindBuffer(GL_ARRAY_BUFFER, tbo_particles_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_.size(), &pos[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, cdbo_particles_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_.size(), &camera_dir[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles_.size());

        glBindVertexArray(0);
    }
}

void ParticleSystemHair::transform(glm::mat4 m)
{
    for (auto &p : particles_) {
        if (p.fixed_) {
            p.pos_ = glm::vec3(m * glm::vec4(p.pos_, 1.0));
        }
    }
}
