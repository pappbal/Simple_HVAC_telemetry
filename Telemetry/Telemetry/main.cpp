#include "mainwindow.h"
#include <QApplication>


#include <gui.h>
#include <communication.h>
#include <statehistory.h>
#include <proxy.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //State state;
    Communication comm;
    StateHistory stateHistory;
    Proxy proxy(comm,stateHistory);
    GUI gui;
    QObject::connect(&comm, SIGNAL(signalToProxy()),&proxy, SLOT(dataReady()));
    QObject::connect(&gui, SIGNAL(signalPID(int,int)), &proxy, SLOT(sendPID(int,int)));


    for(int i=0; i<300; i++)
    {
    comm.sendSignal();
    }



    gui.sendSignal(10,5);
    std::cout << "Program has finished" << std::endl;
    return a.exec();
}
