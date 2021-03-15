#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include "image_process.h"



QImage processTest(QImage& coppy_image)
{   
    cv::Mat temp(coppy_image.height(),coppy_image.width(),CV_8UC3,(uchar*)coppy_image.bits(),coppy_image.bytesPerLine());
    cv::GaussianBlur( temp, temp, cv::Size(5,5), 3, 3);
    cv::cvtColor(temp, temp,CV_BGR2RGB);
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits();
    return dest;
}

ImageProcess::ImageProcess(QObject *parent):
    QObject(parent)
{ 
    m_signal = true;
    
    cv::namedWindow( "ShowVideo", cv::WINDOW_AUTOSIZE );
    //cv::namedWindow( "ShowBlur", cv::WINDOW_AUTOSIZE );
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    m_capture.open(deviceID, apiID);
    // check if we succeeded
    if (!m_capture.isOpened()) 
    {
        std::cout << "ERROR! Unable to open camera\n";
    }
    std::cout << "Start grabbing" << std::endl << "Press Q key to terminate" << std::endl;  

}

void ImageProcess::changeSignal()
{
    QMutexLocker locker(&m_mutex);
    m_signal = false;
    std::cout << "Some thing !" << std::endl;
}

void ImageProcess::doWork()
{
    
    cv::Mat frame;
    cv::Mat blur;
    cv::Mat edges;
    int i = 1;
    while(m_signal)
    {
        m_capture.read(frame);
        if (frame.empty()) {
            std::cout << "ERROR! blank frame grabbed\n";
            break;
        }
        // Origin frame
        cv::imshow("ShowVideo", frame);

        // Do the smoothing
        // Could use GaussianBlur(), blur(), medianBlur() or bilateralFilter().
        cv::GaussianBlur( frame, blur, cv::Size(1,1), 3, 3);
        cv::GaussianBlur( blur, blur, cv::Size(1,1), 3, 3);
        cv::Canny(blur, edges, 100, 200, 3);
        
        //cv::imshow("ShowBlur", edges);

        // Exit quen push 'q'
        char k = static_cast<char>(cv::waitKey('x'));
        if (k == 'x')   
        {
            cv::destroyWindow("ShowVideo"); // destroyAllWindows make exit all programer
            break;
        }
        
        const QImage dest((const uchar *) edges.data, edges.cols, edges.rows, edges.step, QImage::Format_Indexed8);
        dest.bits();
        emit imageReady(dest);
        double fps = m_capture.get(cv::CAP_PROP_FPS);
        std::cout << "Frames per second using video.get(CAP_PROP_FPS) : " << fps << std::endl;
        qDebug() <<"Thread : "<< QThread::currentThreadId();   
        std::cout << i++ << '\n';  
    }
    m_capture.release();
    cv::destroyWindow("ShowVideo");
    emit doWorkFinished();  
    
}


