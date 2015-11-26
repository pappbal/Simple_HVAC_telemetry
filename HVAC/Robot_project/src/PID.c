/*
 * PID.c
 *
 *  Created on: 2013.05.08.
 *      Author: Géza
 */

#include "stm32f4xx_conf.h"
#include "PID.h"

double Kp, Ki, Kd;
double actual_error_1 = 0 ,actual_error_2 = 0, actual_error_3 = 0, actual_error_4 = 0;
double error_previous_1 = 0, error_previous_2 = 0, error_previous_3 = 0, error_previous_4 = 0;
double P_1,P_2,P_3,P_4;
double I_1,I_2,I_3,I_4;
double D_1,D_2,D_3,D_4;
double setpoint_1=0,setpoint_2,setpoint_3,setpoint_4;
int dummy_thermo;



void PID_sampletime(void) {
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InitStructure.TIM_Period = 41999;
	TIM_InitStructure.TIM_Prescaler = 1999;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

void init_PID(double Kp_param, double Ki_param, double Kd_param) {
	Kp = Kp_param;
	Ki = Ki_param;
	Kd = Kd_param;
}

void Set_Kp(uint8_t P_param){
	Kp = P_param;
}
void Set_Ki(uint8_t I_param){
	Ki = I_param;
}
void Set_Kd(uint8_t D_param){
	Kd = D_param;
}

void Set_setpoint_1(double setpoint_param){
	setpoint_1 = setpoint_param;
}
void Set_setpoint_2(double setpoint_param){
	setpoint_2 = setpoint_param;
}
void Set_setpoint_3(double setpoint_param){
	setpoint_3 = setpoint_param;
}
void Set_setpoint_4(double setpoint_param){
	setpoint_4 = setpoint_param;
}

//Variables

uint32_t PID_Controller(double *setpoint, uint16_t measured_value, double* error_previous, double* actual_error, double* P, double* I, double* D, uint32_t Fan_PWM) {

	uint32_t PID = 0;
	double celsius = 0.0625 * (measured_value >> 3);

	if (dummy_thermo < 6) {
		dummy_thermo++;
		return 0;
	} else {
		dummy_thermo = 6;

		*error_previous = *actual_error;
		*actual_error = celsius - *setpoint;

		*P = *actual_error;

		*I += *error_previous;

		if (*I >= 50)
			*I = 50;

		if ((*actual_error - 0.2 < celsius) || (*actual_error + 0.2 > celsius))
			*D = 0;
		else
			*D = *actual_error - *error_previous;

		if (Fan_PWM)
			*I = 0;

		PID = Kp * (*P) + Ki * (*I) + Kd * (*D);

		return PID;
	}

}
