#include "mainwindow.h"
#include <QApplication>

#include "communication.h"
#include <QDebug>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Serial_Communication serial_Communication(QSerialPort::Baud115200,QSerialPort::Data8,QSerialPort::OddParity,QSerialPort::OneStop,QSerialPort::NoFlowControl);
    //QObject::connect(&serial_Communication.getSerial_port(), SIGNAL(readyRead()), serial_Communication, SLOT(read_received_data()));

    if(serial_Communication.open_serial_port("/dev/ttyS1") == false){
        qDebug() << "Failed opening the port";      
    }


    return a.exec();
}
