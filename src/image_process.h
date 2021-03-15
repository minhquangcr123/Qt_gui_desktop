
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>

// Normal function
QImage processTest(QImage& coppy_image);

    
class ImageProcess: public QObject
{   
    Q_OBJECT

private:
    cv::Mat m_image; 
    cv::VideoCapture m_capture; 
    std::string m_filename;
    bool m_signal;
    QMutex m_mutex;
public:
    ImageProcess(QObject *parent = nullptr);
    

public slots:
    void changeSignal();
    void doWork();

signals:
    void imageReady(const QImage &image);
    void doWorkFinished();

};

#endif