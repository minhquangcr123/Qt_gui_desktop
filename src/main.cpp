#include <iostream>
#include <QtWidgets>
#include "mainwindow.h"

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    qDebug() << "runtime version: " << qVersion() << " compiled with: " << QT_VERSION_STR << Qt::endl;
    qDebug()<<"Thread: "<<QThread::currentThreadId();
    return app.exec();
}

