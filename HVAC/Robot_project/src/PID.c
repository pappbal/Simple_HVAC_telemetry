/*
 * PID.c
 *
 *  Created on: 2013.05.08.
 *      Author: Géza
 */

#include "stm32f4xx_conf.h"
#include "PID.h"




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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

void init_PID(double Kp_param, double Ki_param, double Kd_param) {
	Kp = Kp_param;
	Ki = Ki_param;
	Kd = Kd_param;
}

//Variables

float PID_Controller(double setpoint, uint16_t measured_value) {

	celsius = 0.0625 * (measured_value >> 3);

	if (dummy_thermo < 6) {
		dummy_thermo++;
		return 0;
	} else {
		dummy_thermo = 6;
		error_previous = actual_error;
		actual_error = celsius - setpoint;

		P = actual_error;

		I += error_previous;

		if (I >= 50)
			I = 50;

		if ((actual_error - 0.2 < celsius) || (actual_error + 0.2 > celsius))
			D = 0;
		else
			D = actual_error - error_previous;
		if ((TIM3 ->CCR1 == 0) || (TIM3 ->CCR4 == 0))
			I = 0;

		PID = Kp * P + Ki * I + Kd * D;

		return PID;
	}

}
