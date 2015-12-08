#ifndef STATEHISTORY
#define STATEHISTORY

#include <application.h>

/**
 * @brief The State class
 * Represents the state of the controller at a time.
 *
 * Contains:
 * - temperatures
 * - speeds
 * - actuator signals
 * Deficient bit signals a not complete package, if some data is not sent by the controller.
 * The param_set QVector is for signalling the valid fields of the state.
 *
 */
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


/**
 * @brief The StateHistory class
 * Contains the history of the controller. All states are pushed to the container vector by Proxy.
 * Data is read by GUI when signalled for new data.
 *
 */
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
    /**
     * @brief newData
     * Signals GUI about new state in stateHistory
     */
    void newData();
};

#endif // STATEHISTORY

