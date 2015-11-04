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
float PID_Controller(double setpoint, uint16_t measured_value);
void PID_sampletime(void);

double Kp, Ki, Kd;
double actual_error=0;
double error_previous=0;
float P ;
float I ;
float D ;
int dummy_thermo ;
float PID ;
double celsius;

#endif /* PID_H_ */
