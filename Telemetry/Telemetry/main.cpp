#include "mainwindow.h"
#include <QApplication>
#include <QQmlApplicationEngine>


#include <gui.h>
#include <communication.h>
#include <statehistory.h>
#include <proxy.h>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    Serial_Communication serial_Communication(QSerialPort::Baud115200,QSerialPort::Data8,QSerialPort::OddParity,QSerialPort::OneStop,QSerialPort::NoFlowControl);

    if(serial_Communication.open_serial_port("/dev/ttyS1") == false){
       cout << "Failed opening the port"  <<  endl;
       }

    StateHistory stateHistory;
    Proxy proxy(serial_Communication,stateHistory);


    GUI gui(engine.rootObjects()[0], *engine.rootContext(), stateHistory);

    QObject::connect(&serial_Communication, SIGNAL(signalToProxy()),&proxy, SLOT(dataReady()));
    QObject::connect(&gui, SIGNAL(signalCommand(qint8,qint32)), &proxy, SLOT(sendCommand(qint8,qint32)));

    QObject::connect(&stateHistory, SIGNAL(newData()),&gui, SLOT(plotData()));


//    gui.signalCommand(16,1);
//    gui.signalCommand(12,40);
//    gui.signalCommand(14,10);

    //gui.sendSignal(10,5);
    //std::cout << "Program has finished" << std::endl;
    return app.exec();
}
