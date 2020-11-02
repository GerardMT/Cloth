#ifndef PARTICLESYSTEMHAIR_H
#define PARTICLESYSTEMHAIR_H

#include "collider.h"
#include "force.h"
#include "force_field.h"
#include "paint_gl.h"
#include "particle_hair_initializer.h"
#include "solver.h"

#include <QOpenGLShaderProgram>
#include <vector>

using namespace std;

class ParticleSystemHair : public PaintGL, public Transform
{
public:
    ParticleSystemHair(Solver &s, ParticleHairInitializer &i);

    ~ParticleSystemHair();

    void addForceField(ForceField &f);

    void addCollider(Collider &c);

    void solver(Solver &s);

    void particleInitializer(ParticleHairInitializer &i);

    void initialieGL() override;

    void paintGL(float dt, const Camera &camera) override;

    void transform(glm::mat4 m) override;

    float life_time_;

    bool paint_particles_;
    bool paint_path_;

    unsigned int path_length_;

private:
    vector<Particle> particles_;
    vector<Force *> forces_;

    const Solver *solver_;
    vector<ForceField *> force_fields_;
    vector<Collider *> collliders_;

    QOpenGLShaderProgram program_particles_;

    GLuint vao_particles_;
    GLuint vbo_particles_;
    GLuint tbo_particles_;
    GLuint cdbo_particles_;

    QOpenGLShaderProgram program_path_;

    GLuint vao_path_;
    GLuint vbo_path_;
};

#endif // PARTICLESYSTEMHAIR_H
