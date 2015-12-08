#include <communication.h>
#include <QIODevice>
#include <QDebug>
#include <iostream>

using namespace std;


/**
 * @brief Communication::sendData(Package package)
 * Sending a package using the communication protocol specific function
 * @param package The package to be sent
 * @return No return
 */
void Communication::sendData(Package package){

    unsigned char start = 0xff; // START character

    data_stream_out.clear(); // Clear output buffer first
    data_stream_out.append(start); // Append start character to the beginning of the array
    QDataStream stream(&data_stream_out,QIODevice::ReadWrite); // Redirect the stream into the QByteArray
    stream.setByteOrder(QDataStream::LittleEndian); // Set endianness according to the HVAC

    stream << start << package; //Compose the packet
    sendData_specific();
}

/**
 * @brief Serial_Communication::Serial_Communication(qint32 baudRate,QSerialPort::DataBits dataBits,QSerialPort::Parity parity,QSerialPort::StopBits stopBits,QSerialPort::FlowControl flow_control)
 * Serial communication class constructor
 * @param baudRate Baud rate of the serial port
 * @param dataBits Number of data bits the USART frame uses
 * @param parity   Type of parity bit for USART frame
 * @param stopBits Number of stop bits for the USART frame
 * @param flow_control Type of flow control for USART frames
 * @return No return value
 */
Serial_Communication::Serial_Communication(qint32 baudRate,QSerialPort::DataBits dataBits,QSerialPort::Parity parity,QSerialPort::StopBits stopBits,QSerialPort::FlowControl flow_control){

    // The name is set in open_serial_port
    Serial_port.setBaudRate(baudRate);
    Serial_port.setDataBits(dataBits);
    Serial_port.setParity(parity);
    Serial_port.setStopBits(stopBits);
    Serial_port.setFlowControl(flow_control);    
}

/**
 * @brief Serial_Communication::~Serial_Communication()
 * Serial communication class destructor, closing the serial port
 * @return No return value
 */
Serial_Communication::~Serial_Communication(){

    if(Serial_port.isOpen()) {
        Serial_port.close();
    }
}

/**
 * @brief Serial_Communication::open_serial_port(QString portName)
 * Opening the serial port defined in portName parameter
 * @param The name of the serial port
 * @return True if success, otherwise false
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

/**
 * @brief Serial_Communication::sendData_specific
 * This is the protocol specific implementetion of the sendData virtual function.
 *
 * Sending out packet whis has been put in data_stream_out private variable.
 * @return No return value
 */
void Serial_Communication::sendData_specific(){

    if(!Serial_port.isOpen()) {
        qDebug() << "Port "<<Serial_port.portName()<<" is not opened";
        return;
    }

    Serial_port.write(data_stream_out);
}

/**
 * @brief Serial_Communication::read_received_data
 * Reading the received data byte from serial port
 *
 * This function reads 1 byte of data from the serial port once and composes the message with the following steps:
 * 1. Waiting for START frame: header is 0xff
 * 2. After START frame, the next frame is the HEADER
 * 3. After HEADER has received, LENGTH of the data field comes in 4 bytes (needed by Proxy), need to compose the length based on the 4 bytes
 * 4. After LENGTH has been received, wait for equivalent DATA frames
 * 5. Send signal to proxy (signalToProxy()) indicating a full message has arrived, ready for parsing
 * The message is stored in received_data_stream QByteArray
 * @return No return value
 */
void Serial_Communication::read_received_data(){

    char data = 0;

    while(Serial_port.bytesAvailable()){

        Serial_port.read(&data,1);

        if((unsigned char)data == 0xff){         
            start_arrived = true;
            received_data_stream.clear();
            Temp_array_for_length.clear();
            Temp_array_for_data.clear();
        }
        else if(start_arrived){
            received_data_stream.append(data);
            start_arrived = false;
            header_arrived = true;
        }
        else if(header_arrived){
            Temp_array_for_length.append(data);
            data_length += ((uint32_t)data) << 8*length_byte_arrive;
            length_byte_arrive++;
            if(length_byte_arrive == 4){
                header_arrived = false;
                received_data_stream.append(Temp_array_for_length);
            }
        }
        else if(length_byte_arrive == 4){
            Temp_array_for_data.append(data);
            data_length--;
            if(data_length == 0){
                received_data_stream.append(Temp_array_for_data);
                length_byte_arrive = 0;
                emit signalToProxy();
            }
        }
    }
}
