#include "particle_system_cloth.h"

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

ParticleSystemCloth::ParticleSystemCloth(Solver &s, ParticleClothInitializer &i)
{
    solver_ = &s;

    paint_particles_ = true;
    paint_lines_ = true;

    initializer_ = &i;
}

ParticleSystemCloth::~ParticleSystemCloth()
{
    glDeleteVertexArrays(1, &vao_particles_);
    glDeleteBuffers(1, &vbo_particles_);
    glDeleteBuffers(1, &tbo_particles_);
    glDeleteBuffers(1, &cdbo_particles_);

    glDeleteVertexArrays(1, &vao_mesh_);
    glDeleteBuffers(1, &vbo_mesh_);
    glDeleteBuffers(1, &ebo_mesh_);

    for (auto f : forces_) {
        delete f;
    }
}

void ParticleSystemCloth::addForceField(ForceField &f)
{
    force_fields_.push_back(&f);
}

void ParticleSystemCloth::addCollider(Collider &c)
{
    collliders_.push_back(&c);
}

void ParticleSystemCloth::solver(Solver &s)
{
    solver_ = &s;
}

void ParticleSystemCloth::particleInitializer(ParticleClothInitializer &i)
{
    initializer_ = &i;
    initialize();
}

void ParticleSystemCloth::initialieGL()
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

    glGenVertexArrays(1, &vao_mesh_);
    glBindVertexArray(vao_mesh_);

    glGenBuffers(1, &vbo_mesh_); // TODO Resue particles tbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ebo_mesh_);

    glBindVertexArray(0);

    initialize();
}

void ParticleSystemCloth::paintGL(float dt, const Camera &camera)
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

    if (paint_lines_) {
        program_path_.bind();
        glUniformMatrix4fv(program_path_.uniformLocation("view_projection"), 1, GL_FALSE, glm::value_ptr(camera.view_projection));

        vector<glm::vec3> pos;
        pos.resize(particles_.size());

        for (unsigned int i = 0; i < particles_.size(); ++i) {
            pos[i] = particles_[i].pos_;
        }

        glBindVertexArray(vao_mesh_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_.size(), &pos[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, n_triangles_ * 3, GL_UNSIGNED_INT, (void *) 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

void ParticleSystemCloth::transform(glm::mat4 m)
{
    for (auto &p : particles_) {
        if (p.fixed_) {
            p.pos_ = glm::vec3(m * glm::vec4(p.pos_, 1.0));
            p.pos_pre_ = p.pos_;
        }
    }
}

void ParticleSystemCloth::initialize()
{
    initializer_->initialize(particles_, forces_, x_particles_, y_particles_);

    n_triangles_ = x_particles_ * y_particles_ * 2;

    vector<unsigned int> indices;
    indices.resize(n_triangles_ * 3);

    unsigned int j = 0;
    for (unsigned int x = 0; x < x_particles_ - 1; ++x) {
        for (unsigned int y = 0; y < y_particles_ - 1; ++y) {
            indices[j + 0] = x_particles_ * y + x;
            indices[j + 1] = x_particles_ * y + x + 1;
            indices[j + 2] = x_particles_ * (y + 1) + x;

            indices[j + 3] = x_particles_ * y + x + 1;
            indices[j + 4] = x_particles_ * (y + 1) + x + 1;
            indices[j + 5] = x_particles_ * (y + 1) + x;
            j += 6;
        }
    }

    glBindVertexArray(vao_mesh_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_mesh_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}
