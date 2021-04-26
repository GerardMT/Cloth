#include <QApplication>
#include <QGLFormat>

#include "./main_window.h"

int main(int argc, char *argv[])
{
    QGLFormat fmt;
    fmt.setVersion(3, 3);
    fmt.setSamples(4);
    fmt.setProfile(QGLFormat::CoreProfile);
    QGLFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
