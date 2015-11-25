#ifndef GUI_H
#define GUI_H

#include <application.h>

class GUI : public QObject
{
    Q_OBJECT
public:
    void sendSignal(int pid_ID, int data);
signals:
    void signalPID(int pid_ID, int data);
};


#endif // GUI

