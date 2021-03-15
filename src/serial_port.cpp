#include "serial_port.h"


MyDiaLog::MyDiaLog(QWidget *parent):
    QDialog(parent),
    m_label_port ( new QLabel (QObject::tr("Serial Port:"), this)),
    m_label_time ( new QLabel (QObject::tr("Time respone :"), this)),
    m_label_request ( new QLabel (QObject::tr("Request:"), this)),
    m_label_traffic ( new QLabel (QObject::tr("No traffic."), this)),
    m_label_status ( new QLabel (QObject::tr("Status: Not running."), this))
{
    m_spin_box  = new QSpinBox (this);
    m_ports = new QLineEdit(this);
    m_line_edit =  new QLineEdit(this);
    m_button = new QPushButton(tr("Start"), this);

    this->setWindowTitle(QObject::tr("Serial Port !"));
    this->printPort();
//! [ Setting Object]
    m_spin_box->setRange(0, 10000);
    m_spin_box->setValue(1000);

//! [Connect Signal slots]
    this->connect(m_button, &QPushButton::clicked, this, &MyDiaLog::pushButton );

//! [ Set Layout ]
    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_label_port, 0, 0);
    m_layout->addWidget(m_ports, 0, 1);
    m_layout->addWidget(m_label_time, 1, 0);
    m_layout->addWidget(m_spin_box, 1, 1);
    m_layout->addWidget(m_label_request, 2, 0);
    m_layout->addWidget(m_line_edit, 2, 1);
    m_layout->addWidget(m_label_traffic, 3, 0);
    m_layout->addWidget(m_label_status, 4, 0);
    m_layout->addWidget(m_button, 1, 2 );
    this->setLayout(m_layout);

}

MyDiaLog::~MyDiaLog()
{
    myThread.quit();
    myThread.wait();

}
void MyDiaLog::pushButton()
{   
    //Configure Object on another Thread
    qDebug()<<"Thread: "<<QThread::currentThreadId();
    onPort = new PortCom;
    onPort->setPortName(m_ports->text(), m_line_edit->text());
    onPort->moveToThread(&myThread);
    
    //Frist Event signal slot
    this->connect(&myThread, SIGNAL(started()), onPort, SLOT(setPort()));
    this->connect(onPort, SIGNAL(setFinished()), &myThread, SLOT(quit()));
    this->connect(&myThread, SIGNAL(finished()), onPort, SLOT(deleteLater()));
    this->connect(onPort, SIGNAL(runningProcess()), this, SLOT(doSomething()));
    myThread.start();
    
}


void MyDiaLog::doSomething()
{
    m_label_traffic->setText("Running");
}

void MyDiaLog::printPort()
{
    QTextStream out(stdout);
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    out << "Total number of pysical ports available: " << serialPortInfos.count() << "\n";

    const QString blankString = "N/A";
    QString description;
    QString manufacturer;
    QString serialNumber;

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        out << "\nPort: " << serialPortInfo.portName()
            << "\nLocation: " << serialPortInfo.systemLocation()
            << "\nDescription: " << (!description.isEmpty() ? description : blankString)
            << "\nManufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString)
            << "\nSerial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString)
            << "\nVendor Identifier: " << (serialPortInfo.hasVendorIdentifier()
                                         ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16)
                                         : blankString)
            << "\nProduct Identifier: " << (serialPortInfo.hasProductIdentifier()
                                          ? QByteArray::number(serialPortInfo.productIdentifier(), 16)
                                          : blankString) << "\n\n\n";
    }
}



// Class PortCom
PortCom::PortCom()
{
    m_serialPort = new QSerialPort(this);
}

void PortCom::setPort()
{
    m_serialPort->close();
    m_serialPort->setPortName(m_portname);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::WriteOnly))
    {   
        
        qDebug() <<"Connected to " << m_portname;
        emit runningProcess();
        char array[] {0};
        qDebug()<<"Thread " << " : "<< QThread::currentThreadId();
        m_serialPort->write(m_output);
        m_serialPort->waitForBytesWritten(1000);    
        //sudo chmod o+rw /dev/virtualcom0      : chmod cho cac port doc viet 
    }

    m_serialPort->close();
    emit setFinished();
}

void PortCom::setPortName(const QString &port_name, const QString &output)
{
    m_portname = port_name;
    m_output = output.toUtf8(); // Convert to Qbyte
}