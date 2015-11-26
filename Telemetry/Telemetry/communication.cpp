#include <communication.h>
#include <QIODevice>
#include <QDebug>
#include <iostream>

using namespace std;



//Serial Port usage:
//0. Get the name of the port first
//1. Call open
//2.write/read

Communication::Communication(){

}

Communication::~Communication(){

}

Serial_Communication::Serial_Communication(qint32 baudRate,QSerialPort::DataBits dataBits,QSerialPort::Parity parity,QSerialPort::StopBits stopBits,QSerialPort::FlowControl flow_control){

    // The name is set in open_serial_port
    Serial_port.setBaudRate(baudRate);
    Serial_port.setDataBits(dataBits);
    Serial_port.setParity(parity);
    Serial_port.setStopBits(stopBits);
    Serial_port.setFlowControl(flow_control);

    cout << "Serial comm constructor" << endl;
}

Serial_Communication::~Serial_Communication(){

    cout << "Serial comm destructor" << endl;

    if(Serial_port.isOpen()) {
        Serial_port.close();
    }

}


bool Serial_Communication::open_serial_port(QString portName){

    Serial_port.setPortName(portName);

    connect(&Serial_port, SIGNAL(readyRead()), this, SLOT(read_received_data()));

    if(Serial_port.open(QIODevice::ReadWrite)) {       
        qDebug() << "Connected to serial port: " << portName;
        return true;
    }
    else {
        qDebug() << "Failed to connect serial port " << portName;
        return false;
    }
}

void Communication::sendData(Package package){

    unsigned char start = 0xff; // START character

    data_stream_out.append(start); // Append start chaarcter to the beginning of the array
    QDataStream stream(&data_stream_out,QIODevice::ReadWrite); // Redirect the stream into the QByteArray
    stream.setByteOrder(QDataStream::LittleEndian); // Set endianness according to the HVAC

    stream << start << package; //Compose the packet

    for(auto elem : data_stream_out){
        cout << "Byte out: " << (uint)elem << endl;
    }
    cout << "End packet " << endl;

    sendData_specific();

}

void Serial_Communication::sendData_specific(){

    if(!Serial_port.isOpen()) {
        qDebug() << "Port "<<Serial_port.portName()<<" is not opened";
        return;
    }

    Serial_port.write(data_stream_out);
}

void Serial_Communication::read_received_data(){

    char data = 0;


    while(Serial_port.bytesAvailable()){

        Serial_port.read(&data,1);

        if((unsigned char)data == 0xff){
            cout << "start: " << std::hex <<static_cast<unsigned>(data) << endl;
            start_arrived = true;
            received_data_stream.clear();
            Temp_array_for_length.clear();
            Temp_array_for_data.clear();
        }
        else if(start_arrived){
            cout << "Header: " << std::hex <<static_cast<unsigned>(data) << endl;
            received_data_stream.append(data);
            start_arrived = false;
            header_arrived = true;
        }
        else if(header_arrived){
            Temp_array_for_length.append(data);
            cout << "Length: " << std::hex <<static_cast<unsigned>(data) << endl;
            data_length += ((uint32_t)data) << 8*length_byte_arrive;
            length_byte_arrive++;
            if(length_byte_arrive == 4){
                header_arrived = false;
                received_data_stream.append(Temp_array_for_length);
            }
        }
        else if(length_byte_arrive == 4){
            Temp_array_for_data.append(data);
             cout << "Data: " << std::hex <<static_cast<unsigned>(data) << endl;
            data_length--;
            if(data_length == 0){
                received_data_stream.append(Temp_array_for_data);
                length_byte_arrive = 0;
                //cout << "------------END PACKET------------" << endl;
                emit signalToProxy();
               // cout << "------------Signal Emitted--------" << endl;
            }

        }

    }
}
