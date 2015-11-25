#ifndef STATEHISTORY
#define STATEHISTORY

#include <application.h>

class State
{

public:
    Temperatures temps;
    Speeds speeds;
    qint32 deficient;
    QVector<quint8> param_set;

public:
    State();
    State(State&& other); //move constructor
    State(const State& other); //copy constructor
    State& operator=(State&& other); //move assignment
    State& operator=(const State& other); //assignment
    void setParam(Package &package);
    int readyToSend();
    void reset();
};


class StateHistory
{

public:
    QVector<State> stateContainer;
   StateHistory();
   // ~StateHistory();
    void append(State &state);
};

#endif // STATEHISTORY

