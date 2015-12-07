/*
 * communication.c
 *
 *  Created on: 2015. nov. 9.
 *      Author: egezhus
 */

#include "usbd_cdc_vcp.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "PID.h"
#include "communication.h"


/*void Data_transform(uint8_t mod, uint8_t egesz, uint8_t vesszo, uint8_t tort, uint8_t kieg) {
	double temp1 = (tort - 48);
	double temp2 = (kieg - 48);

	switch (mod - 48) {
	case 0:
		if (egesz == 45)
			setpoint = 0;
		else {
			if (tort == 44)
				setpoint = 10 * (egesz - 48) + (vesszo - 48) + temp2 / 10;
			else if (tort != 44)
				setpoint = 10 * (egesz - 48);
			if (vesszo == 44)
				setpoint = (egesz - 48) + temp1 / 10;
			if ((vesszo != 44) && (tort != 44))
				setpoint = 10 * (egesz - 48) + (vesszo - 48);
		}
		break;
	case 1:
		if (egesz == 45)
			Kp = 0;
		else {
			if (vesszo == 44)
				Kp = (egesz - 48) + temp1 / 10 + temp2 / 100;
			else if (vesszo != 44)
				Kp = (egesz - 48);
		}
		break;
	case 2:
		if (egesz == 45)
			Kd = 0;
		else {
			if (vesszo == 44)
				Kd = (egesz - 48) + temp1 / 10 + temp2 / 100;
			else if (vesszo != 44)
				Kd = (egesz - 48);
		}
		break;
	case 3:
		if (egesz == 45)
			Ki = 0;
		else {
			if (vesszo == 44)
				Ki = (egesz - 48) + temp1 / 10 + temp2 / 100;
			else if (vesszo != 44)
				Ki = (egesz - 48);
		}
		break;

	}
}*/

void Send_data(uint8_t* data, uint32_t len) {

	VCP_DataTx(data, len);
	return;
}

uint8_t Get_data(uint8_t* data){


	return VCP_get_char(data);

}


void Set_Serial_send_receive_time(){

	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_InitStructure.TIM_Period = 41999;
	TIM_InitStructure.TIM_Prescaler = 199;//1999;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_InitStructure);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM7, ENABLE);

}


void construct_temperature_message(uint8_t * message, uint16_t temperature, uint8_t header){

	message[0] = START; //START
	message[1] = header; //HEADER
	message[2] = 2; //Length
	message[3] = 0; //Length
	message[4] = 0; //Length
	message[5] = 0; //Length
	message[6] = (uint8_t)temperature; // Temperature lower 1 byte
	message[7] = (uint8_t)(temperature >> 8); // Temperature upper 1 byte
}

void construct_fan_frequency_message(uint8_t * message,uint32_t frequency, uint8_t header){

	message[0] = START; //START
	message[1] = header; //HEADER
	message[2] = 1; //Length
	message[3] = 0; //Length
	message[4] = 0; //Length
	message[5] = 0; //Length
	message[6] = (uint8_t)frequency; // Frequency, casting it to uint8_t, it is enough now

}

void construct_fan_PWM_message(uint8_t * message,uint32_t PWM, uint8_t header){

	message[0] = START; //START
	message[1] = header; //HEADER
	message[2] = 1; //Length
	message[3] = 0; //Length
	message[4] = 0; //Length
	message[5] = 0; //Length
	message[6] = (uint8_t)PWM; // PWM, casting it to uint8_t, it is enough now

}

void construct_halt_message(uint8_t * message){
	message[0] = START; //START
	message[1] = ID_halted; //HEADER
	message[2] = 1; //Length
	message[3] = 0; //Length
	message[4] = 0; //Length
	message[5] = 0; //Length
	message[6] = (uint8_t)0; // A dummy data byte
}

void Parse_message(uint8_t header,uint8_t* data){

	switch(header){
	case ID_pid_p:
		Set_Kp(*data);
	break;

	case ID_pid_i:
		Set_Ki(*data);
	break;

	case ID_pid_d:
		Set_Kd(*data);
	break;

	case ID_req_temp1:
		Set_setpoint_1((double)*data);
	break;

	case ID_req_temp2:
		Set_setpoint_2((double)*data);
	break;

	case ID_req_temp3:
		Set_setpoint_3((double)*data);
	break;

	case ID_req_temp4:
		Set_setpoint_4((double)*data);
	break;

	case ID_start:
		actual_error_1 = 0; actual_error_2 = 0; actual_error_3 = 0; actual_error_4 = 0;
		P_1 = 0; P_2 = 0; P_3 = 0; P_4 = 0;
		I_1 = 0; I_2 = 0; I_3 = 0; I_4 = 0;
		D_1 = 0; D_2 = 0; D_3 = 0; D_4 = 0;
		error_previous_1 = 0; error_previous_2 = 0; error_previous_3 = 0; error_previous_4 = 0;
		WORK = 1;
	break;

	case ID_stop:
		WORK = 0;
		actual_error_1 = 0; actual_error_2 = 0; actual_error_3 = 0; actual_error_4 = 0;
		P_1 = 0; P_2 = 0; P_3 = 0; P_4 = 0;
		I_1 = 0; I_2 = 0; I_3 = 0; I_4 = 0;
		D_1 = 0; D_2 = 0; D_3 = 0; D_4 = 0;
		error_previous_1 = 0; error_previous_2 = 0; error_previous_3 = 0; error_previous_4 = 0;
	break;

	case ID_self_check:

	break;

	default:
	break;
	}
}
