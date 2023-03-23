#if !defined(FACEDETECTOR_H)
#define FACEDETECTOR_H

#include <QObject>

#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include "videowidget.h"

class FaceDetector : public VideoWidget
{
Q_OBJECT
public:
    explicit FaceDetector(QWidget *parent = nullptr);
    ~FaceDetector();

protected:
    void process() override;
    void initialize() override;
    void paint() override;
    void resize(int W, int H) override;

private:
    QString m_modelPath;
    cv::Ptr<cv::FaceDetectorYN> m_detector;
    cv::Mat m_videoFrame;
    cv::Mat m_result;
    cv::Mat m_faces;
    QOpenGLFramebufferObject *m_tempFBO;// used for copying texture from GPU to CPU to OpenCV
    void initializeDetector();
    void detectFace();

};

#endif // FACEDETECTOR_H
