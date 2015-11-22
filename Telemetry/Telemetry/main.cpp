#include "mainwindow.h"
#include <QApplication>

#ifndef PROXY_H
#include <proxy.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //State state;
    Communication comm;
    StateHistory stateHistory;
    Proxy proxy(comm,stateHistory);
    QObject::connect(&comm, SIGNAL(signalProxy()),&proxy, SLOT(dataReady()));


    for(int i=0; i<300; i++)
    {
    comm.sendSignal();
    }

    std::cout << "Program has finished" << std::endl;

    return a.exec();
}
