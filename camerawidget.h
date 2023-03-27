#if !defined(CAMERAWIDGET_H)
#define CAMERAWIDGET_H

#include <QTimer>
#include <QVBoxLayout>
#include <QDebug>
#include <QThread>
#include <QList>
#include <QObject>
#include <QWidget>
#include <QCamera>
#include <QEvent>

#include "videowidget.h"
#include "videosurface.h"
#include "facedetector.h"

#define CAMERA_WIDGET_WIDTH 1280
#define CAMERA_WIDGET_HEIGHT 720

class CameraWidget : public QWidget
{
Q_OBJECT
public:
    CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget();
    VideoWidget *m_videoWidget;
    VideoSurface *m_videoSurface;
    QCamera *m_camera;
    QThread *m_cameraThread;

protected:
    void showEvent(QShowEvent *showEvent);

private:
    void initCamera();

};

#endif // CAMERAWIDGET_H
