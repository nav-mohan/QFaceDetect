#include "streamer.h"

Streamer::Streamer(){}
Streamer::~Streamer(){}

void Streamer::initialize()
{
    avformat_alloc_output_context2(&m_outputFormatContext, NULL, "flv", RTMPADDRESS); //RTMP

    if (!(m_outputFormatContext->oformat->flags & AVFMT_NOFILE))
        avio_open(&m_outputFormatContext->pb, RTMPADDRESS, AVIO_FLAG_WRITE);

    m_outputCodec = const_cast<AVCodec*>(avcodec_find_encoder(AV_CODEC_ID_H264));
    m_outputCodecContext = avcodec_alloc_context3(m_outputCodec);
    m_outputStream = avformat_new_stream(m_outputFormatContext, m_outputCodec);
    
    const AVRational dst_fps = {FPS, 1};
    m_outputCodecContext->codec_tag = 0;
    m_outputCodecContext->codec_id = AV_CODEC_ID_H264;
    m_outputCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    m_outputCodecContext->width = WIDTH;
    m_outputCodecContext->height = HEIGHT;
    m_outputCodecContext->gop_size = 12;
    m_outputCodecContext->pix_fmt = OUTPUT_PIX_FMT;
    m_outputCodecContext->framerate = dst_fps;
    m_outputCodecContext->time_base = av_inv_q(dst_fps);
    m_outputCodecContext->bit_rate = BITRATE;
    if (m_outputFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
        m_outputCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    std::string h264profile = "high444";
    avcodec_parameters_from_context(m_outputStream->codecpar,m_outputCodecContext);
    av_dict_set(&m_outputCodecOptions, "profile", h264profile.c_str(), 0);
    av_dict_set(&m_outputCodecOptions, "preset", "superfast", 0);
    av_dict_set(&m_outputCodecOptions, "tune", "zerolatency", 0);
    int ret = avcodec_open2(m_outputCodecContext, m_outputCodec, &m_outputCodecOptions);
    qDebug() << "avcodec_open2 " << ret << "\n";

    m_outputStream->codecpar->extradata = m_outputCodecContext->extradata;
    m_outputStream->codecpar->extradata_size = m_outputCodecContext->extradata_size;
    av_dump_format(m_outputFormatContext, 0, RTMPADDRESS, 1);
    
    m_outputFrame = av_frame_alloc();
    int i = av_image_get_buffer_size(m_outputCodecContext->pix_fmt,WIDTH,HEIGHT,1);
    m_outputFrameBuffer = new uint8_t[i];

    av_image_fill_arrays(
        m_outputFrame->data,
        m_outputFrame->linesize,
        m_outputFrameBuffer,
        m_outputCodecContext->pix_fmt,
        WIDTH,HEIGHT,1);
    m_outputFrame->width = WIDTH;
    m_outputFrame->height = HEIGHT;
    m_outputFrame->format = static_cast<int>(m_outputCodecContext->pix_fmt);

    ret = avformat_write_header(m_outputFormatContext,nullptr);
    m_resampleContext = sws_getContext(
        WIDTH,HEIGHT,INPUT_PIX_FMT,
        WIDTH,HEIGHT,m_outputCodecContext->pix_fmt,
        SWS_BILINEAR,nullptr,nullptr,nullptr
        );
    m_bits = (uchar *)malloc(WIDTH*HEIGHT*BYTES_PER_PIXEL);
}

void Streamer::streamData(uchar *bits)
{
    memcpy(m_bits,bits,WIDTH*HEIGHT*BYTES_PER_PIXEL);
    const int stride[] = {WIDTH*BYTES_PER_PIXEL};
    int ret = sws_scale(
        m_resampleContext,
        &m_bits,
        stride,
        0,
        HEIGHT,
        m_outputFrame->data,
        m_outputFrame->linesize
    );
    if(ret != HEIGHT){
        char *errbuf = (char*)malloc(1024);
        qDebug() << "SWS ERROR CODE" << ret << ": " << errbuf; 
    }
    m_outputFrame->pts += av_rescale_q(1, m_outputCodecContext->time_base, m_outputStream->time_base);
    av_new_packet(&m_pkt, 0);
    ret = avcodec_send_frame(m_outputCodecContext, m_outputFrame);
    // qDebug() << "avcodec_send_frame " << ret;
    ret = avcodec_receive_packet(m_outputCodecContext,&m_pkt);
    // qDebug() << "avcodec_receive_packet " << ret;
    av_interleaved_write_frame(m_outputFormatContext,&m_pkt);
    av_packet_unref(&m_pkt);
}



