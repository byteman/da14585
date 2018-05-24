/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include "user_periph_setup.h"       // peripheral configuration
#include "gpio.h"
#include "uart.h"                    // UART initialization

/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void custom_GPIO_reservations(void)
{
#ifdef CFG_PRINTF_UART2
    //RESERVE_GPIO(UART2_TX, UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, PID_UART2_TX);x
    //RESERVE_GPIO(UART2_RX, UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, PID_UART2_RX);
#endif

  // RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
#if 1	
	RESERVE_GPIO(AUDIO,AUDIO_SDA_PORT,AUDIO_SDA_PIN,PID_GPIO);
	
	RESERVE_GPIO(ADC,ADC1_CLK_PORT,ADC1_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC1_DO_PORT,ADC1_DO_PIN,PID_GPIO);
	
	RESERVE_GPIO(ADC,ADC2_CLK_PORT,ADC2_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC2_DO_PORT,ADC2_DO_PIN,PID_GPIO);

	RESERVE_GPIO(ADC,ADC3_CLK_PORT,ADC3_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC3_DO_PORT,ADC3_DO_PIN,PID_GPIO);

	
	RESERVE_GPIO(ADC,ADC4_CLK_PORT,ADC4_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC4_DO_PORT,ADC4_DO_PIN,PID_GPIO);

	

#endif	
	RESERVE_GPIO(KEY1,KEY_PWR_PORT, KEY_PWR_PIN, PID_GPIO);
	RESERVE_GPIO(KEY2,KEY_ZERO_PORT,KEY_ZERO_PIN,PID_GPIO);
	
	RESERVE_GPIO(PWR,PWR_OFF_PORT,PWR_OFF_PIN,PID_GPIO);
	
	
		//这里可以不用保留为GPIO。
	//RESERVE_GPIO(LCD,LCD_CLK_PORT,LCD_CLK,PID_GPIO);
	//RESERVE_GPIO(LCD,LCD_SDIN_PORT,LCD_SDIN,PID_GPIO);
	RESERVE_GPIO(LCD,LCD_RST_PORT,LCD_RST,PID_GPIO);

//EEPROM
	RESERVE_GPIO(EEPROM,I2C_GPIO_PORT,I2C_SCL_PIN,PID_I2C_SCL);
	RESERVE_GPIO(EEPROM,I2C_GPIO_PORT,I2C_SDA_PIN,PID_I2C_SDA);
	
	RESERVE_GPIO(ADC_PWR,ADC_PORT,ADC_PIN,PID_ADC);
	
	
}
#endif // CFG_DEVELOPMENT_DEBUG

void custom_set_pad_functions(void)        // set gpio port function mode
{


	GPIO_ConfigurePin(AUDIO_SDA_PORT,AUDIO_SDA_PIN,OUTPUT,PID_GPIO,false);

	GPIO_ConfigurePin(ADC1_CLK_PORT,ADC1_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC1_DO_PORT,ADC1_DO_PIN,INPUT,PID_GPIO,true);
	
	GPIO_ConfigurePin(ADC2_CLK_PORT,ADC2_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC2_DO_PORT,ADC2_DO_PIN,INPUT,PID_GPIO,true);

	GPIO_ConfigurePin(ADC3_CLK_PORT,ADC3_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC3_DO_PORT,ADC3_DO_PIN,INPUT,PID_GPIO,true);

	GPIO_ConfigurePin(ADC4_CLK_PORT,ADC4_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC4_DO_PORT,ADC4_DO_PIN,INPUT,PID_GPIO,true);

//按键是平时高，按下低 ,启用上拉电阻.	
	
	//GPIO_ConfigurePin(KEY_ZERO_PORT,KEY_ZERO_PIN,INPUT,PID_GPIO,false);
	GPIO_ConfigurePin(KEY_PWR_PORT, KEY_PWR_PIN, INPUT,PID_GPIO,true);
	GPIO_ConfigurePin(KEY_ZERO_PORT, KEY_ZERO_PIN, INPUT_PULLUP, PID_GPIO, false);
	//GPIO_ConfigurePin(KEY_PWR_PORT,  KEY_PWR_PIN, INPUT_PULLUP,  PID_GPIO, false);

//输出高控制开机
	GPIO_ConfigurePin(PWR_OFF_PORT,PWR_OFF_PIN,OUTPUT,PID_GPIO,false);


  GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
  GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, false);
	GPIO_ConfigurePin(LCD_RST_PORT,LCD_RST,OUTPUT,PID_GPIO,true);
	
	
	//adc
	GPIO_ConfigurePin(ADC_PORT, ADC_PIN, INPUT, PID_ADC, true);

	//init_eeprom_pin();
	//init_oled_pin();
}

//配置EEPROM
void init_eeprom_pin(void)
{
	GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
  GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, false);

}
void init_oled_pin(void)
{
#if 1
	GPIO_ConfigurePin(LCD_CLK_PORT,LCD_CLK,OUTPUT,PID_GPIO,true);
	GPIO_ConfigurePin(LCD_SDIN_PORT,LCD_SDIN,OUTPUT,PID_GPIO,true);
	GPIO_ConfigurePin(LCD_RST_PORT,LCD_RST,OUTPUT,PID_GPIO,true);
#else
	GPIO_ConfigurePin(LCD_CLK_PORT,LCD_CLK,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(LCD_SDIN_PORT,LCD_SDIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(LCD_RST_PORT,LCD_RST,OUTPUT,PID_GPIO,false);
#endif
}