#ifndef PARTICLESYSTEMCLOTH_H
#define PARTICLESYSTEMCLOTH_H

#include "collider.h"
#include "force.h"
#include "force_field.h"
#include "paint_gl.h"
#include "particle_cloth_initializer.h"
#include "solver.h"

#include <QOpenGLShaderProgram>
#include <vector>

using namespace std;

class ParticleSystemCloth : public PaintGL, public Transform
{
public:
    ParticleSystemCloth(Solver &s, ParticleClothInitializer &i);

    ~ParticleSystemCloth();

    void addForceField(ForceField &f);

    void addCollider(Collider &c);

    void solver(Solver &s);

    void particleInitializer(ParticleClothInitializer &i);

    void initialize();

    void initialieGL() override;

    void paintGL(float dt, const Camera &camera) override;

    void transform(glm::mat4 m) override;

    float life_time_;

    bool paint_particles_;
    bool paint_lines_;

    vector<Particle> particles_;

private:
    unsigned int x_particles_;
    unsigned int y_particles_;

    unsigned int n_triangles_;

    vector<Force *> forces_;

    const Solver *solver_;
    vector<ForceField *> force_fields_;
    vector<Collider *> collliders_;

    ParticleClothInitializer *initializer_;

    QOpenGLShaderProgram program_particles_;

    GLuint vao_particles_;
    GLuint vbo_particles_;
    GLuint tbo_particles_;
    GLuint cdbo_particles_;

    QOpenGLShaderProgram program_path_;

    GLuint vao_mesh_;
    GLuint vbo_mesh_;
    GLuint ebo_mesh_;
};

#endif // PARTICLESYSTEMCLOTH_H
