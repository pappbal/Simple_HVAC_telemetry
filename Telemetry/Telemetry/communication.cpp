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

    char data = 0;
    bool start_arrived = false;
    qint64 header_size = 1;
    qint64 length_size = 4;
    qint32 length = 0;

    //Serial_port.read(&data_char,size);
    //data = data_char;

   //received_data_stream = Serial_port.readAll();

   /* for(auto i : received_data_stream){
        cout << (uint)i << endl;

    }
    received_data_stream.clear();
    cout << "End of stream" << endl << endl << endl;*/



    while(Serial_port.read(&data,1) > 0){

        if((unsigned char)data == 0xff){
            cout << "start: " << std::hex <<static_cast<unsigned>(data) << endl;
            start_arrived = true;
            received_data_stream.clear();
            length = 0;
        }
        else if(start_arrived){\
            quint8 header = data;

            cout << "header: " << static_cast<unsigned>(header)<< endl;

            QByteArray lengthArray = Serial_port.read(length_size);

            for(auto& elem : lengthArray){
               // cout << "length: " << static_cast<unsigned>(elem) << endl;
                quint32 length_elem = elem;

                length += length_elem << (8* (&elem-lengthArray.begin()));
            }
            cout << "length: " << length << endl;
            QByteArray dataArray = Serial_port.read(length);

            received_data_stream.append(header);
            received_data_stream.append(lengthArray);
            received_data_stream.append(dataArray);
            for(auto elem : received_data_stream){
                cout << static_cast<unsigned>(elem) << endl;
            }
            quint16 celsius = dataArray[0] + (dataArray[1] << 8);
            double celsius_d = 0.0625 * (celsius >> 3);
            cout << "celsius: " << celsius_d << endl;
            cout << "end packet" << endl;

            emit signalToProxy();
            start_arrived = false;

        }

    }
    cout << "end" << endl;


}

/*void Serial_Communication::send_packet_stream(){

   open_serial_port("/dev/ttyS0");
   Serial_port.write(data_stream);
}*/
