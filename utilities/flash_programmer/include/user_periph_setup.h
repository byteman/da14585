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

#include <stdint.h>
#include "gpio.h"
#include "i2c_eeprom.h"
#include "spi.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#undef DEVELOPMENT_MODE

// FPGA
#define FPGA    (0)

// EEPROM Configuration Wizard
//*** <<< Use Configuration Wizard in Context Menu >>> ***

//  <h> EEPROM Configuration Wizard
//          <o0> Memory Selection   <0=> Custom Memory
//                                  <1=> Microchip 24LC02
//                                  <2=> Microchip 24LC64
//                                  <3=> ST M24M01
#define EEPROM_CONFIG 0
//      <h> Custom Memory Settings (Used only if Custom Memory is selected)
//          <o1.0..18> I2C_EEPROM_SIZE (B) <0x0-0x40000>
//          <o2> I2C_EEPROM_PAGE (B) <0-256>
//          <o3.0..15> I2C_SLAVE_ADDRESS <0x0-0xFFFF>
//          <o4>  I2C_SPEED_MODE        <1=> I2C_STANDARD <2=> I2C_FAST
//          <o5>  I2C_ADDRESS_MODE     <0=> 7-bit addressing <2=> 10-bit addressing
//          <o6>  I2C_ADDRESS_SIZE     <0=> 8-bit memory address <1=> 16-bit memory address <2=> 24-bit memory address
//      </h>
//  </h>

#if EEPROM_CONFIG == 0                          // Custom Memory
#define I2C_EEPROM_SIZE   0x40000               // EEPROM size in bytes
#define I2C_EEPROM_PAGE   256                   // EEPROM page size in bytes
#define I2C_SLAVE_ADDRESS 0x0050                // Set slave device address
#define I2C_SPEED_MODE    (2)                   // 1: standard mode (100 kbits/s), 2: fast mode (400 kbits/s)
#define I2C_ADDRESS_MODE  (0)                   // 0: 7-bit addressing, 1: 10-bit addressing
#define I2C_ADDRESS_SIZE  (1)                   // 0: 8-bit memory address, 1: 16-bit memory address, 3: 24-bit memory address

#elif EEPROM_CONFIG == 1                        // Microchip 24LC02 (2 kBit - 8 byte page)
#define I2C_EEPROM_SIZE   0x100                 // EEPROM size in bytes
#define I2C_EEPROM_PAGE   8                     // EEPROM page size in bytes
#define I2C_SLAVE_ADDRESS 0x50                  // Set slave device address
#define I2C_SPEED_MODE    I2C_FAST              // fast mode (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_7BIT_ADDR         // 7-bit addressing
#define I2C_ADDRESS_SIZE  I2C_1BYTE_ADDR        // 8-bit memory address

#elif EEPROM_CONFIG == 2                        // Microchip 24LC64 (64 kBit - 32 byte page)
#define I2C_EEPROM_SIZE   0x2000                // EEPROM size in bytes
#define I2C_EEPROM_PAGE   32                    // EEPROM page size in bytes
#define I2C_SLAVE_ADDRESS 0x50                  // Set slave device address
#define I2C_SPEED_MODE    I2C_FAST              // fast mode (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_7BIT_ADDR         // 7-bit addressing
#define I2C_ADDRESS_SIZE  I2C_2BYTE_ADDR        // 8-bit memory address

#elif EEPROM_CONFIG == 3                        // ST M24M01 (1 MBit - 256 byte page)
#define I2C_EEPROM_SIZE   0x20000               // EEPROM size in bytes
#define I2C_EEPROM_PAGE   256                   // EEPROM page size in bytes
#define I2C_SLAVE_ADDRESS 0x50                  // Set slave device address
#define I2C_SPEED_MODE    I2C_FAST              // fast mode (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_7BIT_ADDR         // 7-bit addressing
#define I2C_ADDRESS_SIZE  I2C_2BYTE_ADDR        // 8-bit memory address

#endif
//*** <<< end of configuration section >>>    ***

// SPI Flash Manufacturer and ID
#define W25X10CL_MANF_DEV_ID (0xEF10)     // W25X10CL Manufacturer and ID
#define W25X20CL_MANF_DEV_ID (0xEF11)     // W25X10CL Manufacturer and ID

// SPI Flash options
#define W25X10CL_SIZE 131072              // SPI Flash memory size in bytes
#define W25X20CL_SIZE 262144              // SPI Flash memory size in bytes
#define W25X10CL_PAGE 256                 // SPI Flash memory page size in bytes
#define W25X20CL_PAGE 256                 // SPI Flash memory page size in bytes

#define SPI_FLASH_DEFAULT_SIZE 262144     // SPI Flash memory size in bytes
#define SPI_FLASH_DEFAULT_PAGE 256        // SPI Flash memory page size in bytes

//SPI initialization parameters
#define SPI_WORD_MODE  SPI_8BIT_MODE      // 0: 8-bit, 1: 16-bit, 2: 32-bit, 3: 9-bit
#define SPI_SMN_MODE   SPI_MASTER_MODE    // 0: Master mode, 1: Slave mode
#define SPI_POL_MODE   SPI_CLK_INIT_HIGH  // 0: SPI clk initially low, 1: SPI clk initially high
#define SPI_PHA_MODE   SPI_PHASE_1        // If same with SPI_POL_MODE, data are valid on clk high edge, else on low
#define SPI_MINT_EN    SPI_NO_MINT        // (SPI interrupt to the ICU) 0: Disabled, 1: Enabled
#define SPI_CLK_DIV    SPI_XTAL_DIV_2     // (SPI clock divider) 0: 8, 1: 4, 2: 2, 3: 14

// UART
// init values
#if (FPGA)
#define UART_PIN_PAIR   0 //(0:(P0_0 , P0_1),2:(P0_2 , P0_3),4:(P0_4 , P0_5),6:(P0_6 , P0_7))
#define UART_GPIO_PORT  GPIO_PORT_0
#define UART_TX_PIN     GPIO_PIN_0
#define UART_RX_PIN     GPIO_PIN_1
#else
#define UART_PIN_PAIR   4 //(0:(P0_0 , P0_1),2:(P0_2 , P0_3),4:(P0_4 , P0_5),6:(P0_6 , P0_7))
#define UART_GPIO_PORT  GPIO_PORT_0
#define UART_TX_PIN     GPIO_PIN_4
#define UART_RX_PIN     GPIO_PIN_5
#endif

typedef struct __uart_sel_pins
{
    uint8_t uart_port_tx;
    uint8_t uart_tx_pin;
    uint8_t uart_port_rx;
    uint8_t uart_rx_pin;
}_uart_sel_pins;

// SPI
// init values
#define SPI_CS_PORT     GPIO_PORT_0
#define SPI_CS_PIN      GPIO_PIN_3
#define SPI_CLK_PORT    GPIO_PORT_0
#define SPI_CLK_PIN     GPIO_PIN_0
#define SPI_DO_PORT     GPIO_PORT_0
#define SPI_DO_PIN      GPIO_PIN_6
#define SPI_DI_PORT     GPIO_PORT_0
#define SPI_DI_PIN      GPIO_PIN_5


typedef struct __spi_sel_pins
{
    uint8_t spi_cs_port;
    uint8_t spi_cs_pin;
    uint8_t spi_clk_port;
    uint8_t spi_clk_pin;
    uint8_t spi_do_port;
    uint8_t spi_do_pin;
    uint8_t spi_di_port;
    uint8_t spi_di_pin;
}_spi_sel_pins;


// I2C/EEPROM
// init values
#define I2C_SCL_PORT    GPIO_PORT_0
#define I2C_SCL_PIN     GPIO_PIN_2
#define I2C_SDA_PORT    GPIO_PORT_0
#define I2C_SDA_PIN     GPIO_PIN_3

typedef struct __i2c_sel_pins
{
    uint8_t i2c_scl_port;
    uint8_t i2c_scl_pin;
    uint8_t i2c_sda_port;
    uint8_t i2c_sda_pin;
}_i2c_sel_pins;

#endif // _USER_PERIPH_SETUP_H_


