/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief Secondary Bootloader application
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include <stdio.h>
#include "datasheet.h"
#include "core_cm0.h"
#include "user_periph_setup.h"
#include "uart_booter.h"
#include "i2c_eeprom.h"
#include "bootloader.h"
#include "spi_commands.h"
#include "gpio.h"
#include "uart.h"
  
 void Start_run_user_application(void);

 /**
****************************************************************************************
* @brief Run the user application after receiving a binany from uart or reading the binary from a non volatile memory booting 
****************************************************************************************
*/
void Start_run_user_application(void)
{
    volatile unsigned short tmp;

#ifdef SPI_FLASH_SUPPORTED
    spi_release();

    // reset SPI GPIOs
    GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_CS_PIN, INPUT_PULLUP, PID_GPIO, true );
    GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_CLK_PIN, INPUT_PULLDOWN, PID_GPIO, false );
    GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_DO_PIN, INPUT_PULLDOWN, PID_GPIO, false );
    GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_DI_PIN, INPUT_PULLDOWN, PID_GPIO, false );
#endif

#ifdef EEPROM_FLASH_SUPPORTED
    i2c_eeprom_release();

    // reset I2C GPIOs
    GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT_PULLDOWN, PID_GPIO, false);
    GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT_PULLDOWN, PID_GPIO, false);
#endif

    if (*(volatile unsigned*)0x07FC0000 & 0x07FC0000)
    {
        tmp = GetWord16(SYS_CTRL_REG);
        //Remap to SysRAM and trigger  SW Reset
        tmp = (tmp & ~REMAP_ADR0) | 0x2;
        tmp |= SW_RESET;
        SetWord16(SYS_CTRL_REG, tmp);
    }
    tmp = GetWord16(SYS_CTRL_REG);
    //  tmp&=~0x0003;
    tmp|=0x8000;
    SetWord16(SYS_CTRL_REG,tmp);
}

extern int FwDownload(void);

int main (void)
{ 
#if defined(SPI_FLASH_SUPPORTED ) || defined(EEPROM_FLASH_SUPPORTED)
    int ret = -1;
#endif

#ifdef UART_SUPPORTED
    int fw_size;
    unsigned int i;
    char* from ; 
    char*  to;
#endif

    SetWord16(SET_FREEZE_REG,FRZ_WDOG);
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP,0);   // exit peripheral power down
    // Power up peripherals' power domain 
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));  

    while (1){

#ifdef UART_SUPPORTED
        if (GPIO_GetPinStatus(UART_GPIO_PORT, UART_RX_PIN)) {
            uart_initialization();                      // initialize UART and UART pins
            fw_size = FwDownload();                     // download FW
            uart_release();                             // release UART and reset UART pins
            if (fw_size>0){
                from=(char*) (SYSRAM_COPY_BASE_ADDRESS);  
                to=(char*) (SYSRAM_BASE_ADDRESS);         
                for(i=0;i<fw_size;i++)  to[i] = from[i]; 

                SetWord16(WATCHDOG_REG, 0xC8);          // 200 * 10.24ms active time for initialization!
                SetWord16(RESET_FREEZE_REG, FRZ_WDOG);  // Start WDOG
                Start_run_user_application();
            }
        }
#endif

#ifdef SPI_FLASH_SUPPORTED
        ret =spi_loadActiveImage();
        if (!ret) {
            SetWord16(WATCHDOG_REG, 0xC8);          // 200 * 10.24ms active time for initialization!
            SetWord16(RESET_FREEZE_REG, FRZ_WDOG);  // Start WDOG
            Start_run_user_application();
        }
#endif

#ifdef EEPROM_FLASH_SUPPORTED
        ret = eeprom_loadActiveImage();
        if (!ret) {
            SetWord16(WATCHDOG_REG, 0xC8);          // 200 * 10.24ms active time for initialization!
            SetWord16(RESET_FREEZE_REG, FRZ_WDOG);  // Start WDOG
            Start_run_user_application();
        }
#endif

    } // while (1)
} 

