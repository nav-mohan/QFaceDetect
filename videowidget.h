#if !defined(VIDEOWIDGET_H)
#define VIDEOWIDGET_H

#include <QTime>
#include <QObject>
#include <QVideoFrame>
#include <QList>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>


class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    void setFrame(const QVideoFrame &frame);
    virtual void process(){};
    virtual void initialize();
    virtual void paint();
    virtual void resize(int W, int H);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int W, int H) override;

    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_IBO;
    QOpenGLShaderProgram m_program;
    QOpenGLTexture *m_videoTexture;
    int m_frameWidth;
    int m_frameHeight;
};

#endif // VIDEOWIDGET_H