#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMenu>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraInfo>
#include <QVariant>
#include <iostream>
#include <QThread>

#include "serial_port.h"
#include "image_process.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);
    virtual ~MainWindow();
    bool loadFile(const QString& pathfile);
    void setTest();
private:

// Object
    QWidget *window_center;
    QWidget *window_app;
    QGridLayout *m_layout;
    QGridLayout *center_layout;

    QLabel *m_imageRegion;
    QLabel *m_imageRegion2;

    QLabel *m_test;
    QLabel *m_test2;

    QMenu *m_fileMenu;
    QWidget *video_capture;

    QMenu *m_processImg;
    QImage m_image;
    QImage m_image_process;

    QMenu *m_videocapture;
    QThread test_thread;
    ImageProcess *wokerImage;

    MyDiaLog *serialDialog;
 
// ACtion
    // Image Action 
    QAction *openAct;
    QAction *filterAct;
    // Video Action
    QActionGroup *videoDeviceGroup;
    QAction *videoDeviceAction;

private slots:
    void open();
    void filter();
    void setImage(const QImage &); 
    void videoCapture();
    void actFromThread(const QImage &image);
 
// Start
    void createAction();
    void createMenus();


protected:

// Event
    #ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
    #endif // QT_NO_CONTEXTMENU
    void closeEvent(QCloseEvent *event) override ;

};

#endif