QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Particles
TEMPLATE = app

CONFIG += c++14
CONFIG(release, release|debug):QMAKE_CXXFLAGS += -Wall -O2

CONFIG(release, release|debug):DESTDIR = release/
CONFIG(release, release|debug):OBJECTS_DIR = release/
CONFIG(release, release|debug):MOC_DIR = release/
CONFIG(release, release|debug):UI_DIR = release/

CONFIG(debug, release|debug):DESTDIR = debug/
CONFIG(debug, release|debug):OBJECTS_DIR = debug/
CONFIG(debug, release|debug):MOC_DIR = debug/
CONFIG(debug, release|debug):UI_DIR = debug/

INCLUDEPATH += /usr/include/eigen3/

LIBS += -lGLEW

SOURCES += \
    camera.cpp \
    collider_plane.cpp \
    collider_polygon.cpp \
    collider_sphere.cpp \
    collider_triangle.cpp \
    force_field_drag.cpp \
    force_field_gravity.cpp \
    force_spring.cpp \
    main.cc \
    main_window.cc \
    glwidget.cc \
    mesh.cpp \
    object.cpp \
    particle.cpp \
    particle_emitter_initializer_fountain.cpp \
    particle_emitter_initializer_semi_sphere.cpp \
    particle_emitter_initializer_waterfall.cpp \
    particle_hair_initializer_curly.cpp \
    particle_hair_initializer_single.cpp \
    particle_system_emitter.cpp \
    particle_system_hair.cpp \
    solver_euler.cpp \
    solver_semi_implicit_euler.cpp \
    solver_verlet.cpp \
    util.cpp

HEADERS  += \
    Transform.h \
    camera.h \
    collider.h \
    collider_plane.h \
    collider_polygon.h \
    collider_sphere.h \
    collider_triangle.h \
    force.h \
    force_field.h \
    force_field_drag.h \
    force_field_gravity.h \
    force_spring.h \
    main_window.h \
    glwidget.h \
    mesh.h \
    object.h \
    paint_gl.h \
    particle.h \
    particle_emitter_initializer.h \
    particle_emitter_initializer_fountain.h \
    particle_emitter_initializer_semi_sphere.h \
    particle_emitter_initializer_waterfall.h \
    particle_hair_initializer.h \
    particle_hair_initializer_curly.h \
    particle_hair_initializer_single.h \
    particle_system_emitter.h \
    particle_system_hair.h \
    solver.h \
    solver_euler.h \
    solver_semi_implicit_euler.h \
    solver_verlet.h \
    util.h

FORMS    += \
    main_window.ui

RESOURCES += \
    shader/particle.frag \
    shader/particle.vert \
    shader/object.frag \
    shader/object.vert \
    shader/path.frag \
    shader/path.vert

