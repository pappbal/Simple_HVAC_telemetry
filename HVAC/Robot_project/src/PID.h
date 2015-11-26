/*
 * PID.h
 *
 *  Created on: 2013.05.08.
 *      Author: Géza
 */

#ifndef PID_H_
#define PID_H_

#include "stm32f4xx_conf.h"

#define Fan_1_PWM TIM3->CCR1
#define Fan_3_PWM TIM3->CCR4

void init_PID(double Kp_param, double Ki_param, double Kd_param);
uint32_t PID_Controller(double *setpoint,uint16_t measured_value,double *error_previous,double *actual_error,double *P, double *I, double *D, uint32_t Fan_PWM);
void PID_sampletime(void);


void Set_Kp(uint8_t P_param);
void Set_Ki(uint8_t I_param);
void Set_Kd(uint8_t D_param);

void Set_setpoint_1(double setpoint_param);
void Set_setpoint_2(double setpoint_param);
void Set_setpoint_3(double setpoint_param);
void Set_setpoint_4(double setpoint_param);


extern double Kp, Ki, Kd;
extern double actual_error_1,actual_error_2, actual_error_3, actual_error_4;
extern double error_previous_1, error_previous_2, error_previous_3, error_previous_4;
extern double P_1,P_2,P_3,P_4;
extern double I_1,I_2,I_3,I_4;
extern double D_1,D_2,D_3,D_4;
extern double setpoint_1,setpoint_2,setpoint_3,setpoint_4;
int dummy_thermo;

#endif /* PID_H_ */
