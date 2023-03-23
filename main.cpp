#include <QApplication>
#include <QSurfaceFormat>

#include "camerawidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(10);
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    QSurfaceFormat::setDefaultFormat(format);

    qDebug() << format.defaultFormat();
    
    qDebug() << MODEL_DIR;

    CameraWidget cw(nullptr);
    cw.show();
    return app.exec();

}