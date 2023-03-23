#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) : 
QOpenGLWidget(parent),
m_videoTexture(nullptr)
{;}

VideoWidget::~VideoWidget()
{
    if(m_VAO.isCreated())
    {
        makeCurrent();
        if(m_videoTexture)
            delete m_videoTexture;
        m_VAO.release();
    }
}

void VideoWidget::setFrame(const QVideoFrame &frame)
{
    // qDebug("Setting Frame");
    QVideoFrame localFrame = frame;
    localFrame.map(QAbstractVideoBuffer::ReadOnly);
    makeCurrent();
    if(
        !m_videoTexture || 
        m_videoTexture->width() != localFrame.width() || 
        m_videoTexture->height() != localFrame.height()
    )
    {
        if(m_videoTexture)
            delete m_videoTexture;
        qDebug("Creating m_videoTexture %dX%d",localFrame.width(), localFrame.height());
        m_videoTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        m_videoTexture->setSize(localFrame.width(), localFrame.height());
        m_videoTexture->setFormat(QOpenGLTexture::RGBA32F);
        m_videoTexture->setWrapMode(QOpenGLTexture::ClampToBorder);
        m_videoTexture->setMinificationFilter(QOpenGLTexture::Nearest);
        m_videoTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
        m_videoTexture->allocateStorage();
    }
    QVideoFrame::PixelFormat format = localFrame.pixelFormat();
    if(format == QVideoFrame::Format_ARGB32)
    {
        unsigned int bytesPerSample = localFrame.bytesPerLine() / localFrame.width() / 4;
        if(bytesPerSample == sizeof(unsigned char))
        {
            // qDebug("Writing to m_videoTexture");
            m_videoTexture->setData(QOpenGLTexture::BGRA, QOpenGLTexture::UInt8, (const void *)localFrame.bits());
        }
    }
    localFrame.unmap();
    process();
    update();
}

void VideoWidget::initializeGL(){initialize();}
void VideoWidget::initialize()
{
    initializeOpenGLFunctions();
    glClearColor(0.4f,0.1f,0.2f,1.0f);
    m_VAO.create();
    m_VAO.bind();

    // Vertex Buffer
    m_VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_VBO.create();
    m_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VBO.bind();
    m_VBO.allocate(16*sizeof(float));
    const float _vBuffer[16] = {-1.0,-1.0,0.0,1.0,+1.0,-1.0,0.0,1.0,+1.0,+1.0,0.0,1.0,-1.0,+1.0,0.0,1.0};
    m_VBO.write(0,(const void*)_vBuffer,16*sizeof(float));
    m_VBO.release();

    // Index Buffer
    m_IBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_IBO.create();
    m_IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_IBO.bind();
    m_IBO.allocate(6*sizeof(unsigned int));
    const unsigned int _iBuffer[6] = {0,1,2,0,2,3};
    m_IBO.write(0,(const void*)_iBuffer,6*sizeof(unsigned int));
    m_IBO.release();

    QString shaderDirectory = SHADER_DIR;
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, shaderDirectory+"displayRGBVideo.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, shaderDirectory+"displayRGBVideo.frag");
    m_program.link();
    m_program.bind();
    m_program.setUniformValue("qt_flip",true);
    m_program.release();
    qDebug("VideoWidget::initialize");
}

void VideoWidget::resizeGL(int W, int H){resize(W,H);}
void VideoWidget::resize(int W,int H)
{   
    QDesktopWidget *dkWidget = QApplication::desktop();
    QList<QScreen *> screenList = QGuiApplication::screens();
    float devicePixelRatio = screenList[dkWidget->screenNumber(this)]->devicePixelRatio();
    qDebug("RESIZING %dX%d|%f",W,H,devicePixelRatio);
    m_frameHeight = H * devicePixelRatio;
    m_frameWidth = W * devicePixelRatio;
}

void VideoWidget::paintGL(){paint();}
void VideoWidget::paint()
{
    // qDebug("Paint");
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    if(!m_videoTexture) return;
    if(!m_program.bind()) return;
    if(!m_VBO.bind()) return;
    if(!m_IBO.bind()) return;
    m_videoTexture->bind();

    m_program.setUniformValue("qt_texture",0);
    m_program.setAttributeBuffer("qt_vertex",GL_FLOAT,0,4,4*sizeof(float));
    m_program.enableAttributeArray("qt_vertex");
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_videoTexture->release();
    m_IBO.release();
    m_VBO.release();
    m_program.release();
}