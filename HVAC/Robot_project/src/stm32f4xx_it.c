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
 **  Distribution: The file is distributed �as is,� without any warranty
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
#include "ventillator.h"

uint8_t set_value[5];
double setpoint = 0;
uint32_t measured_value1 = 0, measured_value2 = 0;
uint32_t measured_value11 = 0, measured_value22 = 0;
uint32_t CaptureNumber1 = 0;
uint32_t CaptureNumber3 = 0;
uint32_t Capture = 0;
uint32_t Capture3 = 0;
uint32_t TIM5Freq1 = 0;
uint32_t TIM5Freq3 = 0;
uint32_t TIM5Freq1_previous;
uint32_t TIM5Freq3_previous;
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
		TIM5Freq1_previous = TIM5Freq1;
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
		if (CaptureNumber1 == 0) {
			measured_value1 = TIM_GetCapture2(TIM5);
			CaptureNumber1 = 1;
		} else if (CaptureNumber1 == 1) {
			measured_value2 = TIM_GetCapture2(TIM5);
			if (measured_value2 > measured_value1) {
				Capture = (measured_value2 - measured_value1);
			} else if (measured_value2 < measured_value1) {
				Capture = ((TIM5->ARR - measured_value1) + measured_value2);
			}
			TIM5Freq1 = ((84000000 / 16800) / 2) / Capture;
			CaptureNumber1 = 0;
			if (TIM5Freq1 > 50)
				TIM5Freq1 = TIM5Freq1_previous;
		}
	}

	if (TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET) {
		TIM5Freq3_previous = TIM5Freq3;
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
		if (CaptureNumber3 == 0) {
			measured_value11 = TIM_GetCapture4(TIM5);
			CaptureNumber3 = 1;
		} else if (CaptureNumber3 == 1) {
			measured_value22 = TIM_GetCapture4(TIM5);
			if (measured_value22 > measured_value11) {
				Capture3 = (measured_value22 - measured_value11);
			} else if (measured_value22 < measured_value11) {
				Capture3 = ((TIM5->ARR - measured_value11) + measured_value22);
			}
			TIM5Freq3 = ((84000000 / 16800) / 2) / Capture3;
			CaptureNumber3 = 0;
			if (TIM5Freq3 > 50)
				TIM5Freq3 = TIM5Freq3_previous;
		}
	}

}



void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		VCP_get_char(&set_value[0]);
		VCP_get_char(&set_value[1]);
		VCP_get_char(&set_value[2]);
		VCP_get_char(&set_value[3]);
		VCP_get_char(&set_value[4]);
		Data_transform(set_value[0], set_value[1], set_value[2], set_value[3],set_value[4]);

		if ((TIM3->CCR1) == 0)
			TIM5Freq1 = 0;
		if ((TIM3->CCR4) == 0)
			TIM5Freq3 = 0;

		set_ventillator_PWM(1,100 * PID_Controller(setpoint, get_temperature(1)));
		set_ventillator_PWM(3, 100 * PID_Controller(setpoint, get_temperature(1)));
		Send_data(get_temperature(1));
		Send_data(TIM5Freq1);
		Send_data(TIM5Freq3);
		Send_data(TIM3->CCR1);

	}
}

