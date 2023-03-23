#include "facedetector.h"

FaceDetector::FaceDetector(QWidget *parent) :
VideoWidget(parent),
m_tempFBO(nullptr)
{
    // QString modelDirectory = "/Users/nav/Projects_CCPP/qt5/OpenGL/NeuralNetFaceDet/model/";
    QString modelDirectory = MODEL_DIR;
    m_modelPath = modelDirectory+"face_detection_yunet_2022mar.onnx";
    initializeDetector();
}

void FaceDetector::initializeDetector()
{
    m_detector = cv::FaceDetectorYN::create(
        m_modelPath.toStdString(),
        "",
        cv::Size(0,0),
        0.6,
        0.3,
        5000
    );
}

FaceDetector::~FaceDetector()
{
    if(m_tempFBO) delete m_tempFBO;
}

void FaceDetector::initialize()
{
    VideoWidget::initialize();
}

void FaceDetector::process()
{
    if(!m_videoTexture) return;
    m_videoTexture->bind();
    if(!m_tempFBO || m_tempFBO->width() != m_videoTexture->width() || m_tempFBO->height() != m_videoTexture->height())
    {
        if(m_tempFBO) delete m_tempFBO;
        QOpenGLFramebufferObjectFormat formatFBO;
        formatFBO.setInternalTextureFormat(GL_RGBA32F);
        m_tempFBO = new QOpenGLFramebufferObject(m_videoTexture->width(),m_videoTexture->height(),formatFBO);
        m_videoFrame = cv::Mat(m_videoTexture->height(),m_videoTexture->width(),CV_8UC3);
    }
    m_detector->setInputSize(cv::Size(m_videoTexture->width(),m_videoTexture->height()));
    makeCurrent();
    m_videoTexture->bind();
    m_tempFBO->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_videoTexture->textureId(),0);
    glViewport(0,0,m_videoTexture->width(),m_videoTexture->height());
    glReadPixels(0,0,m_videoTexture->width(),m_videoTexture->height(),GL_RGB,GL_UNSIGNED_BYTE,m_videoFrame.data);
    detectFace();
    // m_videoTexture->release();
    // m_tempFBO->release();
}

void FaceDetector::paint()
{
    VideoWidget::paint();
}

void FaceDetector::resize(int W, int H)
{
    qDebug("FaceDetector Resizing");
    m_detector->setInputSize(cv::Size(W,H));
    VideoWidget::resize(W,H);
}

void FaceDetector::detectFace()
{
    m_detector->detect(m_videoFrame,m_faces);
    m_result = m_videoFrame.clone();
    for (int i = 0; i < m_faces.rows; i++)
    {
        cv::rectangle(m_result, cv::Rect2i(
            int(m_faces.at<float>(i,0)),
            int(m_faces.at<float>(i,1)),
            int(m_faces.at<float>(i,2)),
            int(m_faces.at<float>(i,3))),
            cv::Scalar(0,255,0),
            2
        );
    } 
    m_videoTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)m_result.data);

}
