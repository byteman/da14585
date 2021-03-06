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

void GPIO_reservations(void)
{
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
	#endif
	
}
#endif // CFG_DEVELOPMENT_DEBUG

void set_pad_functions(void)        // set gpio port function mode
{

#if 1
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
	GPIO_ConfigurePin(KEY_PWR_PORT, 	KEY_PWR_PIN, INPUT_PULLDOWN,	PID_GPIO, false);
	GPIO_ConfigurePin(KEY_ZERO_PORT, KEY_ZERO_PIN, INPUT_PULLDOWN,  PID_GPIO, false);
	//GPIO_ConfigurePin(KEY_PWR_PORT,  KEY_PWR_PIN, INPUT_PULLUP,  PID_GPIO, false);

//输出高控制开机
	GPIO_ConfigurePin(PWR_OFF_PORT,PWR_OFF_PIN,OUTPUT,PID_GPIO,true);


  GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
  GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, false);
	GPIO_ConfigurePin(LCD_RST_PORT,LCD_RST,OUTPUT,PID_GPIO,true);
	
	
	//adc
	GPIO_ConfigurePin(ADC_PORT, ADC_PIN, INPUT, PID_ADC, true);

#endif

}

#include "bt_weight.h"
#define VER_MARJO 1
#define VER_MIN 	4
void periph_init(void)
{
	  
	//SetWord16(CLK_AMBA_REG, 0x00);                 // set clocks (hclk and pclk ) 16MHz
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    //SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);

    //rom patch
    patch_func();
		
		if(factory_test_check(VER_MARJO , VER_MIN) == true){
			return;
		}
    //Init pads
    set_pad_functions();

    // (Re)Initialize peripherals
    // i.e.
    // uart_init(UART_BAUDRATE_115K2, UART_FRAC_BAUDRATE_115K2, UART_CHARFORMAT_8);

#ifdef CFG_PRINTF_UART2
    //SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
    //uart2_init(UART_BAUDRATE_115K2, UART_FRAC_BAUDRATE_115K2, UART_CHARFORMAT_8);
#endif

   // Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}
