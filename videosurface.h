#if !defined(VIDEOSURFACE_H)
#define VIDEOSURFACE_H

#include <QDebug>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QVideoFrame>
#include <QList>
#include <QObject>

#include "videowidget.h"

class VideoSurface : public QAbstractVideoSurface
{
Q_OBJECT
public:
    explicit VideoSurface(QObject *parent = nullptr);
    ~VideoSurface();
    VideoWidget *m_videoWidget;
    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    bool present(const QVideoFrame &frame);
    bool start(const QVideoSurfaceFormat &format);
    void stop();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle ) const;

};

#endif // VIDEOSURFACE_H
