#ifndef STATEHISTORY
#define STATEHISTORY

#include <application.h>

class State
{
private:
    qint16 calcTemp(qint8 lower, qint8 upper);
public:
    Temperatures temps;
    Speeds speeds;
    Actuators acts;
    qint32 deficient;
    QVector<quint8> param_set;
    State();
    State(State&& other); //move constructor
    State(const State& other); //copy constructor
    State& operator=(State&& other); //move assignment
    State& operator=(const State& other); //assignment
    void setParam(Package &package);
    int readyToSend();
    void reset();
};


class StateHistory : public QObject
{
    Q_OBJECT


public:
    QVector<State> stateContainer;
   StateHistory();
   // ~StateHistory();
    void append(State &state);
signals:
    void newData();
};

#endif // STATEHISTORY

