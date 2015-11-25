#ifndef GUI_H
#define GUI_H

#include <application.h>
#include <statehistory.h>

class GUI : public QObject
{
    Q_OBJECT
public:
    StateHistory& stateHistory;
    GUI(StateHistory& stateHistory);
    void sendSignal(int pid_ID, int data);
signals:
    void signalPID(int pid_ID, int data);
public slots:
    void plotData();
};




#endif // GUI

