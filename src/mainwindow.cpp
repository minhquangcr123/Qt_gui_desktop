#include "mainwindow.h"
#include <QMetaType>
Q_DECLARE_METATYPE(QCameraInfo)


MainWindow::MainWindow(QMainWindow *parent):
    QMainWindow(parent)
{   
    // Make window
    this->setWindowTitle(tr("Application"));
    //this->setStyleSheet("background-color: black;");

    window_center = new QWidget(this);
    this->setCentralWidget(window_center);

    window_app = new QWidget(this);
    window_app->setFixedWidth(1024);
    window_app->setFixedHeight(640);

//! [0]

//! [1]
    m_imageRegion = new QLabel(tr("<i>Choose a menu option, or right-click to invoke a context menu</i>"), window_app);
    m_imageRegion->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    m_imageRegion->setFixedWidth(500);
    m_imageRegion->setFixedHeight(500);

    m_imageRegion2 = new QLabel(tr("<i>Choose a menu option, or right-click to invoke a context menu</i>"), window_app);
    m_imageRegion2->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    m_imageRegion2->setFixedWidth(500);
    m_imageRegion->setFixedHeight(500);

    m_test = new QLabel(tr("Some thing for test hahahahaha!"), window_app);
    m_test->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)); 

    m_test2 = new QLabel(tr("Some thing for test !"), window_app);
    m_test2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

       

// Configure layout
//! [1]
    

    m_layout = new QGridLayout(window_app); // Neu window_app destroy thi m_layout destroy    
    m_layout->setHorizontalSpacing(0);
    m_layout->addWidget(m_test, 0, 0);
    m_layout->addWidget(m_imageRegion, 1 ,0);
    m_layout->addWidget(m_imageRegion2, 1, 1);
    m_layout->addWidget(m_test2, 2,0);
    window_app->setLayout(m_layout);
    
    center_layout = new QGridLayout(window_center); //Neu center window destroy thi center layout destroy
    center_layout->addWidget(window_app, 0 ,0);
    window_center->setLayout(center_layout);
    
    
//! [2]
    QString message {tr("A context menu is available by right-clicking")};
    this->statusBar()->showMessage(message);
    this->createAction();
    this->createMenus();

    qDebug()<<"From main thread: "<<QThread::currentThreadId();
}


MainWindow::~MainWindow()
{
    std::cout << "Deconstructor ! Main Window \n" ;
    
    test_thread.quit();
    test_thread.wait();
    
}   
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
   std::cout << event << std::endl;
}


bool MainWindow::loadFile(const QString& pathfile)
{
    QImageReader reader{pathfile};
    reader.setAutoTransform(true);
    const QImage newImage { reader.read()};
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(pathfile), reader.errorString()));
        return false;
    }

    this->setImage(newImage);
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(pathfile)).arg(m_image.width()).arg(m_image.height()).arg(m_image.depth());
    this->statusBar()->showMessage(message);
    return 0;
}

// Obj
void MainWindow::open() //Open then chose file from dialog
{
    m_imageRegion->setText(tr("Invoked <b>File|Open</b>"));  
    QFileDialog dialog(this, tr("Open File"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles().first();
        std::cout << fileNames.toStdString() << std::endl;
    }
    this->loadFile(fileNames);
}

void MainWindow::setImage(const QImage &newImage)
{
    m_image = newImage;
    m_imageRegion->setPixmap(QPixmap::fromImage(m_image));

}

void MainWindow::filter()
{
    if (m_image_process.isNull())
    {   
        m_image_process = processTest(m_image);
        m_imageRegion2->setPixmap(QPixmap::fromImage(m_image));
    }
    
}
void MainWindow::actFromThread(const QImage &image)
{
    if (!image.isNull())
    {
    m_imageRegion2->setPixmap(QPixmap::fromImage(image));
    qDebug()<<"From main thread: "<<QThread::currentThreadId();
    }
}

void MainWindow::videoCapture()
{  
    // serialDialog = new MyDiaLog(this);
    // serialDialog->show();

    wokerImage = new ImageProcess; // Khong nen tao object trong constructor main thread. vi nhu the no se hoat dong o main thread
    wokerImage->moveToThread(&test_thread);

    this->connect(&test_thread, SIGNAL(started()), wokerImage, SLOT(doWork()));
    this->connect(wokerImage, SIGNAL(imageReady(const QImage&)), this, SLOT(actFromThread(const QImage&)));
    this->connect(wokerImage, SIGNAL(doWorkFinished()), &test_thread, SLOT(quit())); 
    this->connect(&test_thread, SIGNAL(finished()), wokerImage, SLOT(deleteLater()));

    test_thread.start();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Exit !" ;
    if (test_thread.isRunning())
    {
        QTimer *timer = new QTimer(this);
        this->connect(timer, SIGNAL(timeout()), wokerImage, SLOT(changeSignal()), Qt::DirectConnection);
        timer->start(100);
    }
}

// Start
void MainWindow::createAction()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    this->connect(openAct, &QAction::triggered, this, &MainWindow::open);


    filterAct = new QAction(tr("&Filter..."), this);
    filterAct->setStatusTip(tr("Filter the Image"));
    this->connect(filterAct, &QAction::triggered, this, &MainWindow::filter);
        

    videoDeviceGroup = new QActionGroup(this);
    videoDeviceGroup->setExclusive(true);
    
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras(); 
    for (const QCameraInfo &cameraInfo : availableCameras)  //QCameraInfo
    {
        qDebug() <<"Camera available info : " << cameraInfo.description() << "\n";
        videoDeviceAction = new QAction(cameraInfo.description(), videoDeviceGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
    }
    this->connect(videoDeviceGroup, &QActionGroup::triggered, this, &MainWindow::videoCapture); 
    //Pointer to member function method

}
void MainWindow::createMenus()
{
    m_fileMenu = this->menuBar()->addMenu(tr("&File"));     //File Menu
    m_fileMenu->addAction(openAct);
    
    m_processImg = this->menuBar()->addMenu(tr("&Filter"));
    m_processImg->addAction(filterAct);

    m_videocapture = this->menuBar()->addMenu(tr("&Video Capture"));
    m_videocapture->addAction(videoDeviceAction);


}

