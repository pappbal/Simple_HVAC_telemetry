/*
 * PID.h
 *
 *  Created on: 2013.05.08.
 *      Author: Géza
 */

#ifndef PID_H_
#define PID_H_

#include "stm32f4xx_conf.h"



void init_PID(double Kp_param, double Ki_param, double Kd_param);
uint32_t PID_Controller(double setpoint, uint16_t measured_value);
void PID_sampletime(void);

extern double Kp, Ki, Kd;
extern double actual_error;
extern double error_previous;
extern double P;
extern double I;
extern double D;
extern int dummy_thermo;
extern uint32_t PID;
extern double celsius;

#endif /* PID_H_ */
