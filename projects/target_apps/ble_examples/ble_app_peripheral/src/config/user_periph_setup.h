/**
 ****************************************************************************************
 *
 * @file user_periph_setup.h
 *
 * @brief Peripherals setup header file.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _USER_PERIPH_SETUP_H_
#define _USER_PERIPH_SETUP_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "arch.h"
#include "da1458x_periph_setup.h"
#include "i2c_eeprom.h"

/*
 * DEFINES
 ****************************************************************************************
 */

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <o> DK selection <0=> As in da1458x_periph_setup.h <1=> Basic <2=> Pro <3=> Expert
#define HW_CONFIG (2)

#define HW_CONFIG_BASIC_DK  ((HW_CONFIG==0 && SDK_CONFIG==1) || HW_CONFIG==1)
#define HW_CONFIG_PRO_DK    ((HW_CONFIG==0 && SDK_CONFIG==2) || HW_CONFIG==2)
#define HW_CONFIG_EXPERT_DK ((HW_CONFIG==0 && SDK_CONFIG==3) || HW_CONFIG==3)

//*** <<< end of configuration section >>>    ***

/****************************************************************************************/
/* I2C EEPROM configuration                                                             */
/****************************************************************************************/

// Select EEPROM characteristics
#define I2C_EEPROM_SIZE   0x800         // EEPROM size in bytes
#define I2C_EEPROM_PAGE   256             // EEPROM page size in bytes
#define I2C_SLAVE_ADDRESS 0x50            // Set slave device address
#define I2C_SPEED_MODE    I2C_FAST        // Speed mode: I2C_STANDARD (100 kbits/s), I2C_FAST (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_7BIT_ADDR   // Addressing mode: {I2C_7BIT_ADDR, I2C_10BIT_ADDR}
#define I2C_ADDRESS_SIZE  I2C_2BYTES_ADDR // Address width: {I2C_1BYTE_ADDR, I2C_2BYTES_ADDR, I2C_3BYTES_ADDR}
/****************************************************************************************/
/* SPI FLASH configuration                                                              */
/****************************************************************************************/

#define SPI_FLASH_DEFAULT_SIZE  (256 * 1024)    // SPI Flash memory size in bytes
#define SPI_FLASH_DEFAULT_PAGE  256
#define SPI_SECTOR_SIZE         4096

#ifndef __DA14586__
    #define SPI_EN_GPIO_PORT    GPIO_PORT_0
    #define SPI_EN_GPIO_PIN     GPIO_PIN_3

    #define SPI_CLK_GPIO_PORT   GPIO_PORT_0
    #define SPI_CLK_GPIO_PIN    GPIO_PIN_0

    //#define SPI_DO_GPIO_PORT    GPIO_PORT_0
    //#define SPI_DO_GPIO_PIN     GPIO_PIN_6

    #define SPI_DI_GPIO_PORT    GPIO_PORT_0
    #define SPI_DI_GPIO_PIN     GPIO_PIN_5
#else // DA14586
    #define SPI_EN_GPIO_PORT    GPIO_PORT_2
    #define SPI_EN_GPIO_PIN     GPIO_PIN_3

    #define SPI_CLK_GPIO_PORT   GPIO_PORT_2
    #define SPI_CLK_GPIO_PIN    GPIO_PIN_0

    #define SPI_DO_GPIO_PORT    GPIO_PORT_2
    #define SPI_DO_GPIO_PIN     GPIO_PIN_9

    #define SPI_DI_GPIO_PORT    GPIO_PORT_2
    #define SPI_DI_GPIO_PIN     GPIO_PIN_4
#endif

/****************************************************************************************/
/* UART2 pin configuration (debug print console)                                        */
/****************************************************************************************/

#ifdef CFG_PRINTF_UART2
    #if HW_CONFIG_BASIC_DK
        #define UART2_TX_GPIO_PORT  GPIO_PORT_2
        #define UART2_TX_GPIO_PIN   GPIO_PIN_6

        #define UART2_RX_GPIO_PORT  GPIO_PORT_2
        #define UART2_RX_GPIO_PIN   GPIO_PIN_7

    #elif HW_CONFIG_PRO_DK
        #define UART2_TX_GPIO_PORT  GPIO_PORT_2
        #define UART2_TX_GPIO_PIN   GPIO_PIN_6

        #define UART2_RX_GPIO_PORT  GPIO_PORT_2
        #define UART2_RX_GPIO_PIN   GPIO_PIN_7

    #elif HW_CONFIG_EXPERT_DK
        #define UART2_TX_GPIO_PORT  GPIO_PORT_0
        #define UART2_TX_GPIO_PIN   GPIO_PIN_4

        #define UART2_RX_GPIO_PORT  GPIO_PORT_0
        #define UART2_RX_GPIO_PIN   GPIO_PIN_5

    #else // (e.g. HW_CONFIG_USB_DONGLE)
        #define UART2_TX_GPIO_PORT  GPIO_PORT_0
        #define UART2_TX_GPIO_PIN   GPIO_PIN_4

        #define UART2_RX_GPIO_PORT  GPIO_PORT_0
        #define UART2_RX_GPIO_PIN   GPIO_PIN_5
    #endif
#endif

/****************************************************************************************/
/* LED configuration                                                                    */
/****************************************************************************************/

#if HW_CONFIG_BASIC_DK
    #define GPIO_LED_PORT     GPIO_PORT_1
    #define GPIO_LED_PIN      GPIO_PIN_0

#elif HW_CONFIG_PRO_DK
    #define GPIO_LED_PORT     GPIO_PORT_1
    #define GPIO_LED_PIN      GPIO_PIN_0

#elif HW_CONFIG_EXPERT_DK
    #define GPIO_LED_PORT     GPIO_PORT_1
    #define GPIO_LED_PIN      GPIO_PIN_0

#else // (other configuration)
#endif

/***************************************************************************************/
/* Production debug output configuration                                               */
/***************************************************************************************/
#if PRODUCTION_DEBUG_OUTPUT
    #define PRODUCTION_DEBUG_PORT GPIO_PORT_2
    #define PRODUCTION_DEBUG_PIN GPIO_PIN_5
#endif


//4 channel cs1237
#define ADC1_CLK_PORT  GPIO_PORT_1
#define ADC1_CLK_PIN   GPIO_PIN_2
#define ADC1_DO_PORT  GPIO_PORT_1
#define ADC1_DO_PIN   GPIO_PIN_3

#define ADC2_CLK_PORT  GPIO_PORT_2
#define ADC2_CLK_PIN   GPIO_PIN_3
#define ADC2_DO_PORT  GPIO_PORT_2
#define ADC2_DO_PIN   GPIO_PIN_2


#define ADC3_CLK_PORT  GPIO_PORT_0
#define ADC3_CLK_PIN   GPIO_PIN_7
#define ADC3_DO_PORT  GPIO_PORT_0
#define ADC3_DO_PIN   GPIO_PIN_2


#define ADC4_CLK_PORT  GPIO_PORT_0
#define ADC4_CLK_PIN   GPIO_PIN_4
#define ADC4_DO_PORT  GPIO_PORT_1
#define ADC4_DO_PIN   GPIO_PIN_1


//SC5040b audio
#define AUDIO_SDA_PORT GPIO_PORT_2
#define AUDIO_SDA_PIN  GPIO_PIN_1


//OLED
//CLK
#define LCD_CLK_PORT GPIO_PORT_2
#define LCD_CLK  GPIO_PIN_5
//DI
#define LCD_SDIN_PORT GPIO_PORT_2
#define LCD_SDIN GPIO_PIN_0
//RST
#define LCD_RST_PORT GPIO_PORT_0
#define LCD_RST  GPIO_PIN_5


//KEY

#define KEY_PWR_PORT GPIO_PORT_0
#define KEY_PWR_PIN  GPIO_PIN_6

#define KEY_ZERO_PORT GPIO_PORT_0
#define KEY_ZERO_PIN  GPIO_PIN_0


//PWR control

#define PWR_OFF_PORT GPIO_PORT_2
#define PWR_OFF_PIN  GPIO_PIN_4

#define I2C_GPIO_PORT     GPIO_PORT_2
#define I2C_SCL_PIN       GPIO_PIN_5
#define I2C_SDA_PIN       GPIO_PIN_0

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable pad and peripheral clocks assuming that peripheral power domain is down.
 *        The UART and SPI clocks are set.
 * @return void
 ****************************************************************************************
 */
void periph_init(void);

/**
 ****************************************************************************************
 * @brief Map port pins. The UART and SPI port pins and GPIO ports are mapped.
 * @return void
 ****************************************************************************************
 */
void set_pad_functions(void);

/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 * @return void
 ****************************************************************************************
 */
void GPIO_reservations(void);

#endif // _USER_PERIPH_SETUP_H_
