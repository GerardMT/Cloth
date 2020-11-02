#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include "camera.h"
#include "force_field_drag.h"
#include "force_field_gravity.h"
#include "object.h"
#include "particle_hair_initializer_curly.h"
#include "particle_hair_initializer_single.h"
#include "particle_system_emitter.h"
#include "particle_system_hair.h"
#include "solver.h"

#include <GL/glew.h>
#include <QGLWidget>
#include <QOpenGLShaderProgram>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);

    ~GLWidget();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

private:
    Camera camera_;

    vector<Object *> objects_;

    Solver *solver_euler_;
    Solver *solver_semi_implicit_euler_;
    Solver *solver_verlet_;

    ParticleSystemHair *particles_system_hair;

    ParticleHairInitializerSingle *particle_hair_initializer_single_;
    ParticleHairInitializerCurly *particle_hair_initializer_curly_;

    ForceFieldGravity *force_field_gravity_;
    ForceFieldDrag *force_field_drag_;

    vector<PaintGL*> paint_gl_;

    float dt_;

    chrono::steady_clock::time_point  time_last_;

    bool rotate_ = false;

    int rotate_x_ = 0;
    int rotate_y_ = 0;

    int rotate_last_x_;
    int rotate_last_y_;

    bool move_ = false;

    int move_x_ = 0;
    int move_y_ = 0;

    int move_last_x_;
    int move_last_y_;

    bool forward_ = false;
    bool backwards_ = false;
    bool left_ = false;
    bool right_ = false;

    bool first_paint_ = true;
    float target_frame_time_;

private slots:
    void uiSolverEuler(bool v);

    void uiSolverSemiImplicitEuler(bool v);

    void uiSolverVerlet(bool v);

    void uiPaintParticles(bool v);

    void uiPaintPath(bool v);

    void uiInitializerRope(bool v);

    void uiInitializerCurly(bool v);
};

#endif  //  GLWIDGET_H_
