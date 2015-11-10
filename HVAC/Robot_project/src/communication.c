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

extern uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);

void Data_transform(uint8_t mod, uint8_t egesz, uint8_t vesszo, uint8_t tort, uint8_t kieg) {
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
}
void Send_data(uint8_t param) {
	uint8_t* buf;
	buf = &param;
	VCP_DataTx(buf, 2);
}
