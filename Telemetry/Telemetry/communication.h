#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <application.h>

class Communication : public QObject
{
    Q_OBJECT
public:
    Communication();
    ~Communication();
    Package getData();
    void sendSignal();
    void sendData(Package package);
signals:
    void signalToProxy();

};

#endif // COMMUNICATION_H

