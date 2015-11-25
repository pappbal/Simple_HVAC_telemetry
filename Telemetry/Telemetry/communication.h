#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <application.h>
#include <stdint.h>
#include <QDataStream>
#include <QIODevice>
#include <QSerialPort>


class Communication : public QObject {

    Q_OBJECT

    protected:
        QByteArray received_data_stream;

    public:
     Communication();
     ~Communication();

     void sendData(Package package);
     Package getData() {return Package(received_data_stream);}

    signals:
        void signalToProxy();


};

class Serial_Communication : public Communication{

    Q_OBJECT

    QSerialPort Serial_port;

    public:

        //Constructor
        Serial_Communication(qint32 baudRate,QSerialPort::DataBits dataBits,QSerialPort::Parity parity,QSerialPort::StopBits stopBits,QSerialPort::FlowControl flow_control);
        //Destructor
        ~Serial_Communication();

        //QSerialPort& getSerial_port() {return QSerialPort;}
        bool open_serial_port(QString portName);


public slots:
        void read_received_data();
};





#endif // COMMUNICATION_H
