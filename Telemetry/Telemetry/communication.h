#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <QSerialPort>

typedef struct packet_s{
    uint32_t ID;
    uint32_t payload;
}packet_t;


class Comm_Interface{

   QSerialPort Serial_port_handle;

public:
   Comm_Interface();
   ~Comm_Interface();

    void signal_packet();
    void send_packet(packet_t packet);
    packet_t get_packet();
};


#endif // COMMUNICATION_H

