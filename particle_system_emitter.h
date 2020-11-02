#ifndef PARTICLESSYSTEMEMITTER_H
#define PARTICLESSYSTEMEMITTER_H

#include <GL/glew.h>
#include <QOpenGLShaderProgram>
#include <vector>

#include "collider.h"
#include "force_field.h"
#include "paint_gl.h"
#include "particle.h"
#include "particle_emitter_initializer.h"
#include "solver.h"

using namespace std;

class ParticleSystemEmitter : public PaintGL
{
public:
    ParticleSystemEmitter(Solver &s, ParticleEmitterInitializer &i, unsigned int n, float life_time_);

    ~ParticleSystemEmitter();

    void nParticles(unsigned int n);

    void addForceField(ForceField &f);

    void addCollider(Collider &c);

    void solver(Solver &s);

    void particleInitializer(ParticleEmitterInitializer &i);

    void initialieGL() override;

    void paintGL(float dt, const Camera &camera) override;

    float life_time_;

private:
    vector<Particle> particles_;

    const Solver *solver_;
    vector<ForceField *> force_fields_;
    vector<Collider *> collliders_;

    ParticleEmitterInitializer *initializer_;

    QOpenGLShaderProgram program_;

    GLuint vao_;
    GLuint vbo_;
    GLuint tbo_;
    GLuint cdbo_;
};

#endif // PARTICLESSYSTEMEMITTER_H
