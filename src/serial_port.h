#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H


#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>
#include <QGridLayout>
#include <QtWidgets>
#include <QThread>
#include <QtSerialBus/QModbusTcpClient>

class PortCom : public QObject
{
    Q_OBJECT
public:
    PortCom();
    void setPortName(const QString &port_name, const QString &output);
private:
    QString m_portname;
    QString m_request;
    QByteArray m_output;
    QSerialPort *m_serialPort;

public slots:
    void setPort();

signals:
    void setFinished();
    void runningProcess();

};


class MyDiaLog : public QDialog
{
    Q_OBJECT

public slots:
    void pushButton();
    void doSomething();

private:
    QGridLayout *m_layout;
    QLabel *m_label_port;
    QLabel *m_label_time;   
    QLabel *m_label_request;
    QLabel *m_label_traffic;
    QLabel *m_label_status;
    
    QLineEdit *m_ports ;
    QSpinBox *m_spin_box;
    QLineEdit *m_line_edit;

    QPushButton *m_button;
    PortCom *onPort;

    QThread myThread;

public:
    MyDiaLog(QWidget *parent = nullptr);
    ~MyDiaLog();
    void printPort();


 
};



#endif