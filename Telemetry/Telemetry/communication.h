#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <QSerialPort>
#include <QDataStream>
#include <QIODevice>


// Headers
#define START 0xff
#define ID_temp1 1
#define ID_temp2 2
#define ID_temp3 3
#define ID_temp4 4
#define ID_freq1 5
#define ID_freq3 6
#define ID_fan_1_PWM 7
#define ID_fan_3_PWM 8
#define ID_pid_p 9
#define ID_pid_i 10
#define ID_pid_d 11
#define ID_req_temp 12
#define ID_start 13
#define ID_stop 14
#define ID_self_check 15


/*class Package{

    uint32_t ID;
    uint32_t payload;

public:
    SendData():ID(0),payload(0){}
    uint32_t getID(){return ID;}
};*/


class Communication : public QObject {

    Q_OBJECT

    protected:
        QByteArray received_data_stream;

    public:
        Communication() : received_data_stream{0} {};

        //void send_packet(SendData &packet);

        //virtual void read_received_data() = 0;
        /*Package*/ int getData() { /*return Package(received_data_stream);*/ return received_data_stream[0];}


};


class Serial_Communication : Communication{


    Q_OBJECT

    QSerialPort Serial_port;
    int received_byte_counter;
    bool start_arrived;
    //void send_packet_stream();

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
