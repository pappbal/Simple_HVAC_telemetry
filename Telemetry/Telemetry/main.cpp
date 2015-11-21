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

    Communication comm;
    StateHistory stateHistory;
    Proxy proxy(comm,stateHistory);
    QObject::connect(&comm, SIGNAL(signalProxy()),&proxy, SLOT(dataReady()));

    comm.sendSignal();



    return a.exec();
}
