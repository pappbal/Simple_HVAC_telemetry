/*
 * communication.h
 *
 *  Created on: 2015. nov. 9.
 *      Author: egezhus
 */

#ifndef SRC_COMMUNICATION_H_
#define SRC_COMMUNICATION_H_

//void Data_transform(uint8_t mod, uint8_t egesz, uint8_t vesszo, uint8_t tort, uint8_t kieg);
void Send_data(uint8_t* data, uint32_t len);
uint8_t Get_data(uint8_t* data);
void construct_temperature_message(uint8_t * message,uint16_t temperature, uint8_t header);
void construct_fan_frequency_message(uint8_t * message,uint32_t frequency, uint8_t header);
void construct_fan_PWM_message(uint8_t * message,uint32_t PWM, uint8_t header);
void Parse_message(uint8_t header,uint8_t* data);
void Set_Serial_send_receive_time();

void Set_USART();


#endif /* SRC_COMMUNICATION_H_ */
