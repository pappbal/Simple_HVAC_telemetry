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
    void sendSignal(qint8 pid_ID, qint32 data);
signals:
    void signalCommand(qint8 pid_ID, qint32 data);
public slots:
    void plotData();
};




#endif // GUI

