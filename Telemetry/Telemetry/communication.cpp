#include "communication.h"
#include <QSerialPort>
#include <QIODevice>
#include <QDebug>
#include <iostream>

using namespace std;

/*void Communication::send_packet(SendData &packet){

    //Create stream
    //data_stream << packet;
    //qDebug() << "packet ID: " << packet.getID();

    // Send packet according to the protocol
    //send_packet_stream();
}*/


//Serial Port usage:
//0. Get the name of the port first
//1. Call open
//2.write/read



Serial_Communication::Serial_Communication(qint32 baudRate,QSerialPort::DataBits dataBits,QSerialPort::Parity parity,QSerialPort::StopBits stopBits,QSerialPort::FlowControl flow_control){

    // The name is set in open_serial_port
    Serial_port.setBaudRate(baudRate);
    Serial_port.setDataBits(dataBits);
    Serial_port.setParity(parity);
    Serial_port.setStopBits(stopBits);
    Serial_port.setFlowControl(flow_control);

    received_byte_counter = 0;
    start_arrived = false;

    qDebug() << "constructor ";
}

Serial_Communication::~Serial_Communication(){

    qDebug() << "Destructor ";

    if(Serial_port.isOpen()) {
        Serial_port.close();
    }

}

bool Serial_Communication::open_serial_port(QString portName){

    Serial_port.setPortName(portName);

    connect(&Serial_port, SIGNAL(readyRead()), this, SLOT(read_received_data()));

    if(Serial_port.open(QIODevice::ReadWrite)) {
        Serial_port.clear();
        qDebug() << "Connected to serial port: " << portName;
        return true;
    }
    else return false;

}


void Serial_Communication::read_received_data(){

    /*char data_char = 0;
    quint8 data;
    qint64 size = 4;*/

    //Serial_port.read(&data_char,size);
    //data = data_char;

    received_data_stream = Serial_port.readAll();

    for(auto i : received_data_stream){
        cout << (uint)i << endl;

    }
    received_data_stream.clear();
    cout << "End of stream" << endl << endl << endl;

   /* for(int i =0;i<size;i++){
    cout << "data: " << (uint)(data+i) << endl;
    }*/


    /*while(Serial_port.read(&data_char,4) > 0){
        cout << "data: " << (uint)data << endl;
    }*/


    /*if(start_arrived){

        received_data_stream.append(data);
        received_byte_counter++;

        if(received_byte_counter == 3){
            quint16 celsius_1 = received_data_stream[1];
            quint16 celsius_2 = received_data_stream[2] << 8;
            celsius_1 += celsius_2;
            double celsius_d = 0.0625 * (celsius_1 >> 3);
            cout << "Header " << (uint)received_data_stream[0] << "  Temperature: " << celsius_d << endl;
            received_data_stream.clear();
            received_byte_counter = 0;
            start_arrived = false;
        }
    }

    if(data == 0xff){
         start_arrived = true;
    }*/

}

/*void Serial_Communication::send_packet_stream(){

   open_serial_port("/dev/ttyS0");
   Serial_port.write(data_stream);
}*/
