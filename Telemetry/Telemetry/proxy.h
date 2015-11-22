#ifndef PROXY_H
#define PROXY_H



//to indicators
#define ID_temp1 1
#define ID_temp2 2
#define ID_temp3 3
#define ID_temp4 4
#define ID_speed1 5
#define ID_speed2 6
//from controls
#define ID_act_signal 7
#define ID_pid_p 8
#define ID_pid_i 9
#define ID_pid_d 10
#define ID_req_temp 11
#define ID_start 12
#define ID_stop 13
#define ID_self_check 14

#define no_of_params 6 //defines the number of parameters stored in State. temp1..4 + speed1..2

#include <QtGlobal>
#include <QVector>
#include <iostream>
#include <QDataStream>
#include <QObject>
#include <qbytearray.h>



class Package
{
public:
    qint8 ID;
    QVector<qint8> payload;
    Package();
    Package(QByteArray& data);
    Package(const Package& other);
};


/*---------------------------------------------------------------------*/

class Temperatures
{
public:
    qint32 temp1;
    qint32 temp2;
    qint32 temp3;
    qint32 temp4;

    void reset();
    Temperatures& operator=(Temperatures&& other); //move assignment
    Temperatures& operator=(const Temperatures& other); //assignment
};




/*---------------------------------------------------------------------*/

class Speeds
{
public:
    qint32 speed1;
    qint32 speed2;

    void reset();
    Speeds& operator=(Speeds&& other); //move assignment
    Speeds& operator=(const Speeds& other); //assignment
};

/*---------------------------------------------------------------------*/

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


/*---------------------------------------------------------------------*/

class Communication : public QObject
{
    Q_OBJECT
public:
    Communication();
    ~Communication();
    Package getData();
    void sendSignal();
signals:
    void signalProxy();
};

/*---------------------------------------------------------------------*/

class StateHistory
{

public:
    QVector<State> stateContainer;
   StateHistory();
   // ~StateHistory();
    void append(State &state);
};



class Proxy : public QObject
{
    Q_OBJECT
private:
    Communication& comm;
    State currentState; //ebbe gyujtom a beerkezo adatokat. Amikor televan, beleteszem a history-ba. Szamon kell tudnom tartni, hogy melyik elemek vannak keszen. Ha olyan jon, amit mar beallitottam, eldobom.
    StateHistory& stateHistory;
public:
    Proxy(Communication &comm, StateHistory &stateHistory);

    ~Proxy();
public slots:
    void dataReady();
};



/*-------------------------------------------------------------------------------------------*/


#endif // PROXY_H

