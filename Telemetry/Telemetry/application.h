#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGlobal>
#include <QObject>
#include <QVector>
#include <iostream>
#include <QDataStream>



//to indicatorsfd
#define ID_temp1 1
#define ID_temp2 2
#define ID_temp3 3
#define ID_temp4 4
#define ID_speed1 5
#define ID_speed2 6
//from controls
#define ID_act_signal1 7
#define ID_act_signal2 8
#define ID_pid_p 9
#define ID_pid_i 10
#define ID_pid_d 11
#define ID_req_temp1 12
#define ID_req_temp2 13
#define ID_req_temp3 14
#define ID_req_temp4 15
#define ID_start 16
#define ID_stop 17
#define ID_self_check 18

#define no_of_params 8 //defines the number of parameters stored in State. temp1..4 + speed1..2


class Package
{
public:
    qint8 ID;
    QVector<qint8> payload;
    Package();
    Package(QByteArray data);
    Package(quint8 ID, qint32 data);
    Package(const Package& other);
};

QDataStream& operator<<(QDataStream& out, const Package& package);

/*---------------------------------------------------------------------*/

class Temperatures
{
public:
    double temp1;
    double temp2;
    double temp3;
    double temp4;

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

class Actuators
{
public:
    qint32 act1;
    qint32 act2;

    void reset();
    Actuators& operator=(Actuators&& other); //move assignment
    Actuators& operator=(const Actuators& other); //assignment
};


#endif // APPLICATION_H

