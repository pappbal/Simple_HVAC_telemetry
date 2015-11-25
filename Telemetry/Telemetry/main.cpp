#include "mainwindow.h"
#include <QApplication>


#include <gui.h>
#include <communication.h>
#include <statehistory.h>
#include <proxy.h>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Serial_Communication serial_Communication(QSerialPort::Baud115200,QSerialPort::Data8,QSerialPort::OddParity,QSerialPort::OneStop,QSerialPort::NoFlowControl);
    //QObject::connect(&serial_Communication.getSerial_port(), SIGNAL(readyRead()), serial_Communication, SLOT(read_received_data()));

    if(serial_Communication.open_serial_port("/dev/ttyS1") == false){
       cout << "Failed opening the port"  <<  endl;
       }

    StateHistory stateHistory;
    Proxy proxy(serial_Communication,stateHistory);
    //GUI gui;
    QObject::connect(&serial_Communication, SIGNAL(signalToProxy()),&proxy, SLOT(dataReady()));
    //QObject::connect(&gui, SIGNAL(signalPID(int,int)), &proxy, SLOT(sendPID(int,int)));


    /*for(int i=0; i<300; i++)
    {
    comm.sendSignal();
    }*/



    //gui.sendSignal(10,5);
    //std::cout << "Program has finished" << std::endl;
    return a.exec();
}
