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
/*Package Communication::getData() {
    return Package(received_data_stream);
}*/

void Communication::sendData(Package package){

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


/*Package Communication::getData()
{
    //generates a random ID from 0-6, payload is the double of ID, 0 is handled as invalid
    Package package;
    package.ID=rand() % 7;
    package.payload.append(package.ID*2);
    return package;
}

void Communication::sendData(Package package)
{
    std::cout << "setData called with package ID of " << (uint)package.ID  <<std::endl;
}

void Communication::sendSignal()
{
    emit signalToProxy();
}
*/



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


void Serial_Communication::read_received_data(){

    char data = 0;
    bool start_arrived = false;   
    qint64 length_size = 4;
    qint32 length = 0;

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
            /*quint16 celsius = dataArray[0] + (dataArray[1] << 8);
            double celsius_d = 0.0625 * (celsius >> 3);
            cout << "celsius: " << celsius_d << endl;
            cout << "end packet" << endl;*/

            emit signalToProxy();
            start_arrived = false;

        }

    }
    cout << "end" << endl;


}
