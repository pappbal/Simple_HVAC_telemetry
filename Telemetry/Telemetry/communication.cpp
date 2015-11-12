#include "communication.h"
#include <QSerialPort>

Comm_Interface::Comm_Interface() {

    Serial_port_handle.setBaudRate(Serial_port_handle.Baud115200);
    Serial_port_handle.setDataBits(Serial_port_handle.Data8);
    Serial_port_handle.setFlowControl(Serial_port_handle.NoFlowControl);


   /* USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;*/



}
