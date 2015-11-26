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
        virtual void sendData_specific() = 0;

        QByteArray received_data_stream;
        QByteArray data_stream_out;

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
    //Class specific data sending
    void sendData_specific();

    bool start_arrived = false;
    bool header_arrived = false;
    int length_byte_arrive = 0;
    qint32 data_length = 0;
    QByteArray Temp_array_for_length;
    QByteArray Temp_array_for_data;

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
