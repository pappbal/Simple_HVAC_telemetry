/*
 * ventillator.h
 *
 *  Created on: 2013.05.03.
 *      Author: Géza
 */

#ifndef VENTILLATOR_H_
#define VENTILLATOR_H_

#include "stm32f4xx.h"

void ventillator_EN_init(void);
void ventillator_PWM_init(void);
void input_capture_setup(void);
void set_ventillator_PWM (uint8_t venti_num, uint32_t threshold);




#endif /* VENTILLATOR_H_ */
