#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent) : 
QAbstractVideoSurface(parent), 
m_videoWidget(nullptr){;}

VideoSurface::~VideoSurface()
{
    if(m_videoWidget)
        m_videoWidget->deleteLater();
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const 
{
    QList<QVideoFrame::PixelFormat> list;
    if(type == QAbstractVideoBuffer::NoHandle){
        list << QVideoFrame::Format_RGB32;
        list << QVideoFrame::Format_ARGB32;
        list << QVideoFrame::Format_ARGB32_Premultiplied;
        list << QVideoFrame::Format_RGB555;
    }
    return list;
}

QVideoSurfaceFormat VideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const 
{
    return format;
}

bool VideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const{
    return true;
}

bool VideoSurface::present(const QVideoFrame &frame){
    if(m_videoWidget)
        m_videoWidget->setFrame(frame);
    return true;
}

bool VideoSurface::start(const QVideoSurfaceFormat &format){
    return QAbstractVideoSurface::start(format);
}

void VideoSurface::stop(){
    QAbstractVideoSurface::stop();
}