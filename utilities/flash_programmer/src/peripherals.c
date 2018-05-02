/**
 ****************************************************************************************
 *
 * @file peripherals.c
 *
 * @brief Peripherals initialization functions
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include <string.h>
#include "uart.h"
#include "peripherals.h"
#include "user_periph_setup.h"
#include "i2c_eeprom.h"

_uart_sel_pins uart_sel_pins;
_spi_sel_pins spi_sel_pins;
_i2c_sel_pins i2c_sel_pins;
void init_uart_pads(void);
void init_spi_pads(void);
void init_i2c_pads(void);

static void set_all_gpios_to_input_pullup(void)
{
    const uint8_t port_num[]    = { 0, 1,  2, 4 };
    const uint8_t num_of_pins[] = { 8, 6, 10, 8 };

    int data_reg;
    int mode_reg;

    for(int i = 0; i < 4; i++) {
        data_reg = GPIO_BASE + (port_num[i] << 5) + 6;
        for(int pin = 0; pin < num_of_pins[i]; pin++) {
            mode_reg = data_reg + (pin << 1);
            SetWord16(mode_reg, INPUT_PULLUP | PID_GPIO);
        }
    }
}

 /**
 ****************************************************************************************
 * @brief Enable pad and peripheral clocks assuming that peripheral power domain
 *        is down. The UART and SPI clocks are set.
 ****************************************************************************************
 */
void periph_init(void)  // set I2C, SPI, UART, UART2 serial clks
{
#if (FPGA)
    SetBits16(BANDGAP_REG, 0x8000, 1);
#endif

    // system init
    SetWord16(CLK_AMBA_REG, 0x00);                // set clocks (hclk and pclk) 16MHz
    SetWord16(SET_FREEZE_REG, FRZ_WDOG);          // stop watchdog
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);     // open pads
    SetBits16(SYS_CTRL_REG, DEBUGGER_ENABLE, 1);  // open debugger
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);     // exit peripheral power down
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    // Init pads
    set_all_gpios_to_input_pullup();
    init_uart_pads();
    init_spi_pads();
    init_i2c_pads();
}

/**
 ****************************************************************************************
 * @brief Set GPIO port function mode for the UART interface.
 ****************************************************************************************
 */
void init_uart_pads(void)
{
    memset(&uart_sel_pins, 0, sizeof(uart_sel_pins));
    uart_sel_pins.uart_port_tx = UART_GPIO_PORT;
    uart_sel_pins.uart_tx_pin = UART_TX_PIN;
    uart_sel_pins.uart_port_rx = UART_GPIO_PORT;
    uart_sel_pins.uart_rx_pin = UART_RX_PIN;
}

void set_pad_uart(void)
{
    GPIO_ConfigurePin((GPIO_PORT) uart_sel_pins.uart_port_tx, (GPIO_PIN) uart_sel_pins.uart_tx_pin, OUTPUT, PID_UART1_TX, false);
    GPIO_ConfigurePin((GPIO_PORT) uart_sel_pins.uart_port_rx, (GPIO_PIN) uart_sel_pins.uart_rx_pin, INPUT, PID_UART1_RX, false);
}

void update_uart_pads(GPIO_PORT port, GPIO_PIN tx_pin, GPIO_PIN rx_pin)
{
    uart_sel_pins.uart_port_tx = port;
    uart_sel_pins.uart_tx_pin = tx_pin;
    uart_sel_pins.uart_port_rx = port;
    uart_sel_pins.uart_rx_pin = rx_pin;
}

/**
 ****************************************************************************************
 * @brief Set GPIO port function mode for SPI interface.
 ****************************************************************************************
 */
void init_spi_pads(void)
{
    memset(&spi_sel_pins, 0, sizeof(_spi_sel_pins));
    spi_sel_pins.spi_cs_port = SPI_CS_PORT;
    spi_sel_pins.spi_cs_pin = SPI_CS_PIN;
    spi_sel_pins.spi_clk_port = SPI_CLK_PORT;
    spi_sel_pins.spi_clk_pin = SPI_CLK_PIN;
    spi_sel_pins.spi_do_port = SPI_DO_PORT;
    spi_sel_pins.spi_do_pin = SPI_DO_PIN;
    spi_sel_pins.spi_di_port = SPI_DI_PORT;
    spi_sel_pins.spi_di_pin = SPI_DI_PIN;
}

void set_pad_spi(void)
{
    GPIO_ConfigurePin( (GPIO_PORT) spi_sel_pins.spi_cs_port, (GPIO_PIN) spi_sel_pins.spi_cs_pin, OUTPUT, PID_SPI_EN, true );
    GPIO_ConfigurePin( (GPIO_PORT) spi_sel_pins.spi_clk_port, (GPIO_PIN) spi_sel_pins.spi_clk_pin, OUTPUT, PID_SPI_CLK, false );
    GPIO_ConfigurePin( (GPIO_PORT) spi_sel_pins.spi_do_port, (GPIO_PIN) spi_sel_pins.spi_do_pin, OUTPUT, PID_SPI_DO, false );
    GPIO_ConfigurePin( (GPIO_PORT) spi_sel_pins.spi_di_port, (GPIO_PIN) spi_sel_pins.spi_di_pin, INPUT, PID_SPI_DI, false );
}

/**
 ****************************************************************************************
 * @brief Set GPIO port function mode for EEPROM interface.
 ****************************************************************************************
 */
void init_i2c_pads(void)
{
    memset(&i2c_sel_pins, 0, sizeof(_i2c_sel_pins));
    i2c_sel_pins.i2c_scl_port = I2C_SCL_PORT;
    i2c_sel_pins.i2c_scl_pin = I2C_SCL_PIN;
    i2c_sel_pins.i2c_sda_port = I2C_SDA_PORT;
    i2c_sel_pins.i2c_sda_pin = I2C_SDA_PIN;
}

void set_pad_eeprom(void)
{
    GPIO_ConfigurePin( (GPIO_PORT) i2c_sel_pins.i2c_scl_port, (GPIO_PIN) i2c_sel_pins.i2c_scl_pin, INPUT, PID_I2C_SCL, false);
    GPIO_ConfigurePin( (GPIO_PORT) i2c_sel_pins.i2c_sda_port, (GPIO_PIN) i2c_sel_pins.i2c_sda_pin, INPUT, PID_I2C_SDA, false);
}
