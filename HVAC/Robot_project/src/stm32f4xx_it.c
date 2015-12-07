/**
 *****************************************************************************
 **
 **  File        : stm32f4xx_it.c
 **
 **  Abstract    : Main Interrupt Service Routines.
 **                This file provides template for all exceptions handler and
 **                peripherals interrupt service routine.
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **                STMicroelectronics STM32F4xx Standard Peripherals Library
 **
 **  Distribution: The file is distributed “as is,” without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. Distribution of this file (unmodified or modified) is not
 **  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
 **  rights to distribute the assembled, compiled & linked contents of this
 **  file as part of an application binary file, provided that it is built
 **  using the Atollic TrueSTUDIO(R) toolchain.
 **
 **
 *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "stm32f4_discovery.h"
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h"
#include "PID.h"
#include "tc77.h"
#include "./ventillator.h"
#include "usbd_cdc_vcp.h"

#include "communication.h"
#include <stdlib.h>


uint8_t WORK = 0;




uint8_t set_value[5];
uint32_t measured_value11 = 0, measured_value12 = 0;
uint32_t measured_value31 = 0, measured_value32 = 0;
uint32_t CaptureNumber1 = 0;
uint32_t CaptureNumber3 = 0;
uint32_t Capture1 = 0;
uint32_t Capture3 = 0;
uint32_t Fan_1_frequency = 0;
uint32_t Fan_3_frequency = 0;
uint32_t Fan_1_frequency_previous;
uint32_t Fan_3_frequency_previous;
uint8_t enable = 0;

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pdev);
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

#ifdef USE_USB_OTG_FS
void OTG_FS_WKUP_IRQHandler(void) {
	if (USB_OTG_dev.cfg.low_power) {
		*(uint32_t *) (0xE000ED10) &= 0xFFFFFFF9;
		SystemInit();
		USB_OTG_UngateClock(&USB_OTG_dev);
	}
	EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif

/**
 * @brief  This function handles OTG_HS_WKUP_IRQ Handler.
 * @param  None
 * @retval None
 */
#ifdef USE_USB_OTG_HS
void OTG_HS_WKUP_IRQHandler(void)
{
	if(USB_OTG_dev.cfg.low_power)
	{
		*(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9;
		SystemInit();
		USB_OTG_UngateClock(&USB_OTG_dev);
	}
	EXTI_ClearITPendingBit(EXTI_Line20);
}
#endif

/**
 * @brief  This function handles OTG_xx_IRQ Handler.
 * @param  None
 * @retval None
 */
#ifdef USE_USB_OTG_HS
void OTG_HS_IRQHandler(void)
#else
void OTG_FS_IRQHandler(void)
#endif
{
	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
 * @brief  This function handles EP1_IN Handler.
 * @param  None
 * @retval None
 */
void OTG_HS_EP1_IN_IRQHandler(void)
{
	USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);
}

/**
 * @brief  This function handles EP1_OUT Handler.
 * @param  None
 * @retval None
 */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
	USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
}
#endif

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

//Period = Capture(1) /(TIMx_CLK *(PSC+1)*(ICxPSC)*polarity_index(2))

void TIM5_IRQHandler(void) {

	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET) {
		Fan_1_frequency_previous = Fan_1_frequency;
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
		if (CaptureNumber1 == 0) {
			measured_value11 = TIM_GetCapture2(TIM5);
			CaptureNumber1 = 1;
		} else if (CaptureNumber1 == 1) {
			measured_value12 = TIM_GetCapture2(TIM5);
			if (measured_value12 > measured_value11) {
				Capture1 = (measured_value12 - measured_value11);
			} else if (measured_value12 < measured_value11) {
				Capture1 = ((TIM5->ARR - measured_value11) + measured_value12);
			}
			Fan_1_frequency = ((84000000 / 16800) / 2) / Capture1;
			CaptureNumber1 = 0;
			if (Fan_1_frequency > 50 || Fan_1_frequency <= 13) Fan_1_frequency = Fan_1_frequency_previous;

		}
	}

	if (TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET) {
		Fan_3_frequency_previous = Fan_3_frequency;
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
		if (CaptureNumber3 == 0) {
			measured_value31 = TIM_GetCapture4(TIM5);
			CaptureNumber3 = 1;
		} else if (CaptureNumber3 == 1) {
			measured_value32 = TIM_GetCapture4(TIM5);
			if (measured_value32 > measured_value31) {
				Capture3 = (measured_value32 - measured_value31);
			} else if (measured_value32 < measured_value31) {
				Capture3 = ((TIM5->ARR - measured_value31) + measured_value32);
			}
			Fan_3_frequency = ((84000000 / 16800) / 2) / Capture3;
			CaptureNumber3 = 0;
			if (Fan_3_frequency > 50 || Fan_3_frequency <= 13) Fan_3_frequency = Fan_3_frequency_previous;

		}
	}

}


/*void USART1_IRQHandler(void){

	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET ){
		TIM_ClearITPendingBit(USART1, USART_IT_RXNE);


	}

}*/


uint8_t start_arrived = 0;

uint8_t header_arrived = 0;
uint8_t header;

uint8_t length_byte_count = 0;
uint32_t length;

uint8_t data_byte_count = 0;
uint8_t data[2];

// Sending out data regulary ~ 1 sec period
void TIM7_IRQHandler(void) {
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET) {
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

			uint16_t temperature = 0;
			uint8_t received_data;


			// Sending out data if client started the system
			if(WORK == 1) {

				/*Temperature message:
				 * 1 byte - START
				 * 1 byte - HEADER
				 * 4 byte - Length  - Client needs this length
				 * 2 byte - Data
				 * TOTAL: 8 byte
				 */

				uint8_t message_temperature[temperature_message_length];

				//Sending temperature 1 data
				temperature = 0;
				temperature = get_temperature(1);
				construct_temperature_message(message_temperature,temperature,ID_temp1);
				Send_data(message_temperature,temperature_message_length);

				//Sending temperature 2 data
				temperature = 0;
				temperature = get_temperature(2);
				//temperature = 0;
				construct_temperature_message(message_temperature,temperature,ID_temp2);
				Send_data(message_temperature,temperature_message_length);

				//Sending temperature 3 data
				temperature = 0;
				temperature = get_temperature(3);
				construct_temperature_message(message_temperature,temperature,ID_temp3);
				Send_data(message_temperature,temperature_message_length);

				//Sending temperature 4 data
				temperature = 0;
				temperature = get_temperature(4);
				//temperature = 0;
				construct_temperature_message(message_temperature,temperature,ID_temp4);
				Send_data(message_temperature,temperature_message_length);

				//free(message);

				/*Fan frequency message:
				 * 1 byte - START
				 * 1 byte - HEADER
				 * 4 byte - Length  - Client needs this length
				 * 1 byte - Data
				 * TOTAL: 7 byte
				 */

				uint8_t message_frequency[fan_frequency_message_length];
				// Sending frequency of Fan 1
				if(Fan_1_frequency == Fan_1_frequency_previous && Fan_1_frequency < 15) {
					Fan_1_frequency = 0;
					Fan_1_frequency_previous = 0;
				}
				construct_fan_frequency_message(message_frequency,Fan_1_frequency,ID_freq1);
				Send_data(message_frequency,fan_frequency_message_length);

				// Sending frequency of Fan 3
				if(Fan_3_frequency == Fan_3_frequency_previous && Fan_3_frequency < 15) {
					Fan_3_frequency = 0;
					Fan_3_frequency_previous = 0;
				}
				construct_fan_frequency_message(message_frequency,Fan_3_frequency,ID_freq3);
				Send_data(message_frequency,fan_frequency_message_length);


				/*Fan PWM message:
				 * 1 byte - START
				 * 1 byte - HEADER
				 * 4 byte - Length  - Client needs this length
				 * 1 byte - Data
				 * TOTAL: 7 byte
				 */

				uint8_t message_PWM[fan_PWM_message_length];

				// Sending PWM of fan 1
				construct_fan_PWM_message(message_PWM,Fan_1_PWM,ID_fan_1_PWM);
				Send_data(message_PWM,fan_PWM_message_length);

				// Sending PWM of fan 3
				construct_fan_PWM_message(message_PWM,Fan_3_PWM,ID_fan_3_PWM);
				Send_data(message_PWM,fan_PWM_message_length);
			}
			else {
				//If sending out data is not allowed, send halted message continuously
				uint8_t message_Halted[halted_message_length];
				construct_halt_message(message_Halted);
				Send_data(message_Halted,halted_message_length);
			}


			// Processing  received data
			while(Get_data(&received_data) == 1){

				if(received_data == 0xff){
					start_arrived = 1;
					header = 0;
				}
				else if(start_arrived){
					header = received_data;
					start_arrived = 0;
					header_arrived = 1;
				}
				else if(header_arrived){
					length += ((uint32_t)received_data) << (length_byte_count * 8);
					length_byte_count++;
					if(length_byte_count == 4){
						header_arrived = 0;
					}
				}
				else if(length_byte_count == 4){
					data[data_byte_count] = received_data;
					data_byte_count++;
					if(data_byte_count == length){
						length_byte_count = 0;
						length = 0;
						data_byte_count = 0;
						Parse_message(header,data);
					}
				}
			}
	}
}

uint8_t set_vent = 1;
uint16_t temp = 0;

// Setting PWM of the fans
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(WORK == 1) {

			//if(set_vent == 1){
				temp = get_temperature(1);
				set_ventillator_PWM(1, 100 * PID_Controller(&setpoint_1, temp, &error_previous_1, &actual_error_1, &P_1, &I_1, &D_1, Fan_1_PWM));

				//get_temperature(2);
				//get_temperature(4);
				set_vent = 3;
			//}
			//else if(set_vent == 3){
				temp = get_temperature(3);
				//temp = get_temperature(2);
				set_ventillator_PWM(3, 100 * PID_Controller(&setpoint_3, temp, &error_previous_3, &actual_error_3, &P_3, &I_3, &D_3, Fan_3_PWM));
				set_vent = 1;
			//}
		}
		else {
			set_ventillator_PWM(1,0);
			set_ventillator_PWM(3,0);
		}
	}
}

