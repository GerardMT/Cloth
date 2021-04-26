#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "collider.h"
#include "mesh.h"
#include "paint_gl.h"

#include <QOpenGLShaderProgram>

class Object : public PaintGL, public Transform
{
public:
    Object(Mesh &m, glm::vec3 pos, glm::vec4 color);

    ~Object();

    void addCollider(Collider &c);

    void transform(glm::mat4 m) override;

    void initialieGL() override;

    void paintGL(float dt, const Camera &camera) override;

private:
    Mesh *mesh_;

    vector<Collider *> colliders_;

    glm::mat4 model_;

    QOpenGLShaderProgram program_;

    glm::vec4 color_;

    GLuint vao_;
    GLuint vbo_;
    GLuint nbo_;
    GLuint fbo_;
};

#endif // OBJECT_H
