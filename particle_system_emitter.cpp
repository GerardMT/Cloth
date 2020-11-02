#include "particle_system_emitter.h"

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

ParticleSystemEmitter::ParticleSystemEmitter(Solver &s, ParticleEmitterInitializer &i, unsigned int n, float life_time)
{
    solver_ = &s;
    particles_.resize(n);

    for (auto &p : particles_) {
        p.life_time_ = life_time;
    }

    initializer_ = &i;
    life_time_ = life_time;
}

ParticleSystemEmitter::~ParticleSystemEmitter()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &tbo_);
    glDeleteBuffers(1, &cdbo_);
}

void ParticleSystemEmitter::nParticles(unsigned int n)
{
    unsigned int n_pre = particles_.size();
    particles_.resize(n);

    if (n > n_pre) {
        for (unsigned int i = n_pre - 1; i < particles_.size(); ++i) {
            particles_[i].life_time_ = life_time_;
        }
    }
}

void ParticleSystemEmitter::addForceField(ForceField &f)
{
    force_fields_.push_back(&f);
}

void ParticleSystemEmitter::addCollider(Collider &c)
{
    collliders_.push_back(&c);
}

void ParticleSystemEmitter::solver(Solver &s)
{
    solver_ = &s;
}

void ParticleSystemEmitter::particleInitializer(ParticleEmitterInitializer &i)
{
    initializer_ = &i;
}

void ParticleSystemEmitter::initialieGL()
{
    string vert_str;
    readFile("../shader/particle.vert", vert_str);
    program_.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_str.c_str());
    cout << program_.log().toUtf8().constData() << endl;

    string frag_str;
    readFile("../shader/particle.frag", frag_str);
    program_.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_str.c_str());
    cout << program_.log().toUtf8().constData() << endl;

    program_.link();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &cdbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cdbo_);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

void ParticleSystemEmitter::paintGL(float dt, const Camera &camera)
{
    unsigned int n_alive = 0;

    unsigned int revived = 0;
    unsigned int revived_max = ceil(static_cast<float>(particles_.size()) / life_time_ * dt);

    for (auto &p : particles_) {
        p.force_ = glm::vec3(0.0);

        p.life_time_ += dt;
        if (p.life_time_ >= life_time_ && revived <= revived_max) {
            p.life_time_ = 0;
            initializer_->initialize(dt, p);
            ++revived;
        }

        if (p.life_time_ < life_time_) {
            ++n_alive;

            glm::vec3 pos = p.pos_;

            for (auto &f : force_fields_) {
                f->apply(p);
            }

            solver_->solve(dt, p);

            for (auto &c : collliders_) {
                if(c->collide(p)) {
                    c->correct(dt, p);
                    break;
                }
            }

            if (glm::length(p.pos_ - pos) < 0.001) {
                p.life_time_ = life_time_;
            }
        }
    }

    program_.bind();
    glUniformMatrix4fv(program_.uniformLocation("view_projection"), 1, GL_FALSE, glm::value_ptr(camera.view_projection));

    struct Sort {
      glm::vec3 pos;
      glm::vec3 camera_dir;
      float camera_dist;
    };

    vector<Sort> particles_alive;
    particles_alive.resize(n_alive);

    unsigned int n = 0;
    for (unsigned int i = 0; i < particles_.size(); ++i) {
        if (particles_[i].life_time_ < life_time_) {
            particles_alive[n].pos = particles_[i].pos_;
            glm::vec3 v = particles_[i].pos_ - camera.pos_;
            particles_alive[n].camera_dir = glm::normalize(v);
            particles_alive[n].camera_dist = glm::length(v);
            ++n;
        }
    }

    sort(particles_alive.begin(), particles_alive.end(), [](const Sort &a, const Sort &b) -> bool
    {
        return a.camera_dist > b.camera_dist;
    });

    vector<glm::vec3> pos;
    pos.resize(n_alive);

    vector<glm::vec3> camera_dir;
    camera_dir.resize(particles_.size());

    for (unsigned int i = 0; i < n_alive; ++i) {
        pos[i] = particles_alive[i].pos;
        camera_dir[i] = particles_alive[i].camera_dir;
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * n_alive, &pos[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, cdbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * n_alive, &camera_dir[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, n_alive);

    glBindVertexArray(0);
}
