#include "camerawidget.h"

void CameraWidget::showEvent(QShowEvent *showEvent)
{
    if(m_camera)
        m_camera->start();
}

CameraWidget::CameraWidget(QWidget *parent) : 
QWidget(parent), 
m_camera(nullptr), 
m_cameraThread(nullptr),
m_videoSurface(nullptr),
m_videoWidget(nullptr)
{
    this->setLayout(new QVBoxLayout());
    // m_videoWidget = new VideoWidget();
    m_videoWidget = new FaceDetector();
    this->layout()->addWidget(m_videoWidget);

    m_videoSurface = new VideoSurface();
    m_videoSurface->m_videoWidget = m_videoWidget;
    
    initCamera();
    m_videoWidget->setMinimumSize(CAMERA_WIDGET_WIDTH,CAMERA_WIDGET_HEIGHT);
}

CameraWidget::~CameraWidget()
{
    if(m_cameraThread)
    {
        m_cameraThread->quit();
        m_cameraThread->deleteLater();
    }
    if(m_videoSurface)
    {
        m_videoSurface->stop();
        m_videoSurface->deleteLater();
    }
    if(m_camera)
    {
        m_camera->stop();
        m_camera->deleteLater();
    }
}

void CameraWidget::initCamera()
{
    qDebug("INIT CAMERA");
    m_camera = new QCamera();
    
    QCameraViewfinderSettings camSet = m_camera->viewfinderSettings();
    camSet.setResolution(CAMERA_WIDGET_WIDTH,CAMERA_WIDGET_HEIGHT);
    camSet.setMaximumFrameRate(30.0f);
    camSet.setMinimumFrameRate(30.0f);
    camSet.setPixelFormat(QVideoFrame::Format_ARGB32);
    m_camera->setViewfinderSettings(camSet);
    m_camera->setViewfinder(m_videoSurface);
    m_camera->setCaptureMode(QCamera::CaptureVideo);
    
    m_cameraThread = new QThread();
    m_camera->moveToThread(m_cameraThread);
    m_cameraThread->start();
    qDebug("dont init camera");
}