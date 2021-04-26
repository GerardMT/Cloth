#include "object.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "util.h"

using namespace std;

Object::Object(Mesh &m, glm::vec3 pos, glm::vec4 color)
{
    mesh_ = &m;

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, pos);

    color_ = color;
}

Object::~Object()
{
    delete mesh_;

    for (auto c : colliders_) {
        delete c;
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &nbo_);
    glDeleteBuffers(1, &fbo_);
}

void Object::addCollider(Collider &c)
{
    colliders_.push_back(&c);
}

void Object::transform(glm::mat4 m)
{
    for (auto c : colliders_) {
        c->transform(m);
    }

    model_ = model_ * m;
}

void Object::initialieGL()
{
    string vert_str;
    readFile("../../res/shader/object.vert", vert_str);
    program_.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_str.c_str());
    cout << program_.log().toUtf8().constData() << endl;

    string frag_str;
    readFile("../../res/shader/object.frag", frag_str);
    program_.addShaderFromSourceCode(QOpenGLShader::Fragment, frag_str.c_str());
    cout << program_.log().toUtf8().constData() << endl;

    program_.link();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(float), &mesh_->vertices_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &nbo_);
    glBindBuffer(GL_ARRAY_BUFFER, nbo_);
    glBufferData(GL_ARRAY_BUFFER, mesh_->normals_.size() * sizeof(float), &mesh_->normals_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &fbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->faces_.size() * sizeof(unsigned int), &mesh_->faces_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::paintGL(__attribute__((unused)) float dt, const Camera &camera)
{
    program_.bind();
    glUniformMatrix4fv(program_.uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model_));
    glUniformMatrix4fv(program_.uniformLocation("view_projection"), 1, GL_FALSE, glm::value_ptr(camera.view_projection));
    glUniform4fv(program_.uniformLocation("color"), 1, glm::value_ptr(color_));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, mesh_->faces_.size(), GL_UNSIGNED_INT, (void *) 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
