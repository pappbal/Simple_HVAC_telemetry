#ifndef STATEHISTORY
#define STATEHISTORY

#include <application.h>

class State
{
private:
    double calcTemp(quint8 lower, quint8 upper);
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
    void setParam(Package package);
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

    void AddNewState(State &newState) { stateContainer.push_back(newState); }
    State GetCurrentState() const { return stateContainer.back(); }
    unsigned GetSize() const { return stateContainer.size(); }
    QVector<State>::const_iterator End() const {return stateContainer.end(); }

signals:
    void newData();
};

#endif // STATEHISTORY

