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
#ifdef CFG_PRINTF_UART2
    RESERVE_GPIO(UART2_TX, UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, PID_UART2_TX);x
    RESERVE_GPIO(UART2_RX, UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, PID_UART2_RX);
#endif

  // RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
	
	RESERVE_GPIO(AUDIO,AUDIO_SDA_PORT,AUDIO_SDA_PIN,PID_GPIO);
	
	RESERVE_GPIO(ADC,ADC1_CLK_PORT,ADC1_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC1_DO_PORT,ADC1_DO_PIN,PID_GPIO);
	
	RESERVE_GPIO(ADC,ADC2_CLK_PORT,ADC2_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC2_DO_PORT,ADC2_DO_PIN,PID_GPIO);

	RESERVE_GPIO(ADC,ADC3_CLK_PORT,ADC3_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC3_DO_PORT,ADC3_DO_PIN,PID_GPIO);

	
	RESERVE_GPIO(ADC,ADC4_CLK_PORT,ADC4_CLK_PIN,PID_GPIO);
	RESERVE_GPIO(ADC,ADC4_DO_PORT,ADC4_DO_PIN,PID_GPIO);

	
	
	RESERVE_GPIO(LCD,LCD_CLK_PORT,LCD_CLK,PID_GPIO);
	RESERVE_GPIO(LCD,LCD_SDIN_PORT,LCD_SDIN,PID_GPIO);
	RESERVE_GPIO(LCD,LCD_RST_PORT,LCD_RST,PID_GPIO);
	
}
#endif // CFG_DEVELOPMENT_DEBUG

void set_pad_functions(void)        // set gpio port function mode
{
#ifdef __DA14586__
    // disallow spontaneous flash wake-up
    GPIO_ConfigurePin(SPI_EN_GPIO_PORT, SPI_EN_GPIO_PIN, OUTPUT, PID_GPIO, true);x
#endif

#ifdef CFG_PRINTF_UART2
    GPIO_ConfigurePin(UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, INPUT, PID_UART2_RX, false);
#endif

	GPIO_ConfigurePin(AUDIO_SDA_PORT,AUDIO_SDA_PIN,OUTPUT,PID_GPIO,false);
	
	GPIO_ConfigurePin(ADC1_CLK_PORT,ADC1_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC1_DO_PORT,ADC1_DO_PIN,INPUT,PID_GPIO,true);
	
	GPIO_ConfigurePin(ADC2_CLK_PORT,ADC2_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC2_DO_PORT,ADC2_DO_PIN,INPUT,PID_GPIO,true);

	GPIO_ConfigurePin(ADC3_CLK_PORT,ADC3_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC3_DO_PORT,ADC3_DO_PIN,INPUT,PID_GPIO,true);

	
	GPIO_ConfigurePin(ADC4_CLK_PORT,ADC4_CLK_PIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(ADC4_DO_PORT,ADC4_DO_PIN,INPUT,PID_GPIO,true);

	
	
	GPIO_ConfigurePin(LCD_CLK_PORT,LCD_CLK,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(LCD_SDIN_PORT,LCD_SDIN,OUTPUT,PID_GPIO,false);
	GPIO_ConfigurePin(LCD_RST_PORT,LCD_RST,OUTPUT,PID_GPIO,false);
	
  //GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
}

void periph_init(void)
{
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);

    //rom patch
    patch_func();

    //Init pads
    set_pad_functions();

    // (Re)Initialize peripherals
    // i.e.
    // uart_init(UART_BAUDRATE_115K2, UART_FRAC_BAUDRATE_115K2, UART_CHARFORMAT_8);

#ifdef CFG_PRINTF_UART2
    SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
    uart2_init(UART_BAUDRATE_115K2, UART_FRAC_BAUDRATE_115K2, UART_CHARFORMAT_8);
#endif

   // Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}
