#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "tc77.h"

void tc77_pin_config(void) {

	GPIO_InitTypeDef GPIO_InitStrucure;
	SPI_InitTypeDef SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Hõmérõ 3 chip select óajel
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //Hõmérõ 2 és 4 chip select órajel
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Hõmérõ 1 chip select órajel

	// Hõmérõk MOSI,MISO,SCK
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStrucure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStrucure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStrucure);

	// Lábak alternate function-nak meghatározása
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2 );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2 );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2 );

	//Hõmérõ 1 Chip Select
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStrucure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStrucure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStrucure);
	GPIO_SetBits(GPIOC, GPIO_Pin_1 );

	//Hõmérõ 2 - 4 Chip Select
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStrucure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStrucure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStrucure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0 );
	GPIO_SetBits(GPIOC, GPIO_Pin_12 );

	//Hõmérõ 3 Chip Select
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStrucure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStrucure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStrucure);
	GPIO_SetBits(GPIOA, GPIO_Pin_15 );

	// SPI2 beállítása
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);

}

double get_temperature(uint8_t thermometer_num) {
	GPIO_TypeDef *chip_select_port;
	uint16_t chip_select_pin;
	uint16_t nothing = 0;
	uint16_t measured_value;
	//double celsius;

	switch (thermometer_num) {
	case 1:
		chip_select_port = GPIOC;
		chip_select_pin = GPIO_Pin_1;
		break;
	case 2:
		chip_select_port = GPIOB;
		chip_select_pin = GPIO_Pin_12;
		break;
	case 3:
		chip_select_port = GPIOA;
		chip_select_pin = GPIO_Pin_15;
		break;
	case 4:
		chip_select_port = GPIOB;
		chip_select_pin = GPIO_Pin_0;
		break;
	}

	GPIO_ResetBits(chip_select_port, chip_select_pin);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE ) == RESET);

	SPI_I2S_SendData(SPI2, nothing);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY ) == SET);
	measured_value = SPI_I2S_ReceiveData(SPI2 );

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE ) == RESET);
	GPIO_SetBits(chip_select_port, chip_select_pin);


	return measured_value;

}
