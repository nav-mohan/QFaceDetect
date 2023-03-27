#if !defined(STREAMER_H)
#define STREAMER_H

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
};
#include <QDebug>
#include <QObject>

#define RTMPADDRESS "rtmp://167.99.183.228/streaming"
#define FPS 24
#define WIDTH 1280
#define HEIGHT 720
#define INPUT_PIX_FMT AV_PIX_FMT_RGB24
#define BYTES_PER_PIXEL 3
#define OUTPUT_PIX_FMT AV_PIX_FMT_YUV444P
#define BITRATE 3000000


class Streamer : public QObject
{

Q_OBJECT
public:
    Streamer();
    ~Streamer();

    AVFormatContext *m_outputFormatContext = nullptr;
    AVCodecContext *m_outputCodecContext = nullptr;
    AVCodec *m_outputCodec = nullptr;
    AVStream *m_outputStream = nullptr;
    AVDictionary *m_outputCodecOptions = nullptr;
    AVFrame *m_outputFrame = nullptr;
    uint8_t *m_outputFrameBuffer = nullptr;
    SwsContext *m_resampleContext = nullptr;
    AVPacket m_pkt;
    uchar *m_bits;

public slots:
    void initialize();
    void streamData(uchar *bits);
};

#endif // STREAMER_H
