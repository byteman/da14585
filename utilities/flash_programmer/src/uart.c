/**
 ****************************************************************************************
 *
 * @file uart.c
 *
 * @brief Programmer UART code.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include "uart.h"
#include "gpio.h"
#include "peripherals.h"

/**
 ****************************************************************************************
  @brief Enable uart interrupts

 ****************************************************************************************
 */
void uart_irq_enable(void)
{
    // irq_set_en_ext (EXT_IRQ_12_VECTOR);
}


/**
 ****************************************************************************************
  @brief Disable uart interrupts

 ****************************************************************************************
 */
void uart_irq_disable(void)
{
    // irq_clr_en_ext (EXT_IRQ_12_VECTOR);
}

/**
 ****************************************************************************************
  @brief Send a character to uart

 ****************************************************************************************
 */
void uart_send (unsigned char txdata)
{
    SetWord16(UART_RBR_THR_DLL_REG, txdata);
}

/**
 ****************************************************************************************
  @brief Receive a character from uart

 ****************************************************************************************
 */
unsigned char uart_recv (void)
{
    return (GetWord16(UART_RBR_THR_DLL_REG));
}

/**
 ****************************************************************************************
  @brief Wait end of transmission

 ****************************************************************************************
 */
void uart_wait_end_of_tx(void)
{
    while (!(GetWord16(UART_LSR_REG) & 0x40));
}

/**
 ****************************************************************************************
  @brief Wait end of reception

 ****************************************************************************************
 */
void uart_wait_end_of_rx(void)
{
    while (!(GetWord16(UART_LSR_REG) & 0x01));
}

/**
 ****************************************************************************************
  @brief Poll for reception

 ****************************************************************************************
 */
int uart_rx_poll(void)
{
    return (GetWord16(UART_LSR_REG) & 0x01);
}

/**
 ****************************************************************************************
  @brief Enable UART pads

 ****************************************************************************************
 */
void uart_pads (unsigned char sel)
{
    switch (sel)
    {
        case 0:
            SetWord16(P00_MODE_REG, FUNC_UART_TX);      // set P0_0 to uart1 TX function
            SetWord16(P01_MODE_REG, FUNC_UART_RX);      // set P0_1 to uart1 RX function
            update_uart_pads(GPIO_PORT_0, GPIO_PIN_0, GPIO_PIN_1);
            break;
        case 2:
            SetWord16(P02_MODE_REG, FUNC_UART_TX);      // set P0_2 to uart1 TX function
            SetWord16(P03_MODE_REG, FUNC_UART_RX);      // set P0_3 to uart1 RX function
            update_uart_pads(GPIO_PORT_0, GPIO_PIN_2, GPIO_PIN_3);
            break;
        case 4:
            SetWord16(P04_MODE_REG, FUNC_UART_TX);      // set P0_4 to uart1 TX function
            SetWord16(P05_MODE_REG, FUNC_UART_RX);      // set P0_5 to uart1 RX function
            update_uart_pads(GPIO_PORT_0, GPIO_PIN_4, GPIO_PIN_5);
            break;
        case 6:
            SetWord16(P06_MODE_REG, FUNC_UART_TX);      // set P0_6 to uart1 TX function
            SetWord16(P07_MODE_REG, FUNC_UART_RX);      // set P0_7 to uart1 RX function
            update_uart_pads(GPIO_PORT_0, GPIO_PIN_6, GPIO_PIN_7);
            break;
        default:
            set_pad_uart();
            break;
    }
}

/**
 ****************************************************************************************
  @brief Initiate  UART peripheral

 ****************************************************************************************
 */
void uart_init (unsigned char pad_sel, unsigned char baud_rate)
 {
    volatile int i;

    SetBits16(CLK_PER_REG, UART1_ENABLE, 1);  	// enable  clock

    // Init uart pads
    uart_pads(pad_sel);

    for (i = 1; i < 20; i++);

    // Reset uart
    SetWord16(UART_SRR_REG, 7);

    // disable fifo
    SetWord16(UART_IIR_FCR_REG, 0);

    // Disable interrupts
    SetWord16(UART_IER_DLH_REG, 0);

    SetWord16(UART_LCR_REG, 0x83);  /// Set bit 8 om LCR in order to access DivisorLatch register

    // set division = serial_clk/(16*baudrate)
    switch (baud_rate) {
        case UART_BAUD_9600:
            SetWord16(UART_IER_DLH_REG, (UART_BAUDRATE_9K6 >> 8) & 0xFF);
            SetWord16(UART_RBR_THR_DLL_REG, UART_BAUDRATE_9K6);
            SetWord16(UART_DLF_REG, UART_FRAC_BAUDRATE_9K6);
            break;
        case UART_BAUD_19200:
            SetWord16(UART_IER_DLH_REG, (UART_BAUDRATE_19K2 >> 8) & 0xFF);
            SetWord16(UART_RBR_THR_DLL_REG, UART_BAUDRATE_19K2);
            SetWord16(UART_DLF_REG, UART_FRAC_BAUDRATE_19K2);
            break;
        case UART_BAUD_57600:
            SetWord16(UART_IER_DLH_REG, (UART_BAUDRATE_57K6 >> 8) & 0xFF);
            SetWord16(UART_RBR_THR_DLL_REG, UART_BAUDRATE_57K6);
            SetWord16(UART_DLF_REG, UART_FRAC_BAUDRATE_57K6);
            break;
        case UART_BAUD_115200:
            SetWord16(UART_IER_DLH_REG, (UART_BAUDRATE_115K2 >> 8) & 0xFF);
            SetWord16(UART_RBR_THR_DLL_REG, UART_BAUDRATE_115K2);
            SetWord16(UART_DLF_REG, UART_FRAC_BAUDRATE_115K2);
            break;
        case UART_BAUD_1M:
            SetWord16(UART_IER_DLH_REG, (UART_BAUDRATE_1M >> 8) & 0xFF);
            SetWord16(UART_RBR_THR_DLL_REG, UART_BAUDRATE_1M);
            SetWord16(UART_DLF_REG, UART_FRAC_BAUDRATE_1M);
            break;
    }

    // set no parity stop bit, data length  (Line control register)
    SetWord16(UART_LCR_REG, 3);  // no parity, 1 stop bit 8 data length and clear bit 8
}

/**
 ****************************************************************************************
  @brief Put character to UART

 ****************************************************************************************
 */
void uart_putc(unsigned char letter)
{
    uart_send (letter);
    uart_wait_end_of_tx();
}

/**
 ****************************************************************************************
  @brief Get character from UART

 ****************************************************************************************
 */
unsigned char uart_getc(void)
{
    uart_wait_end_of_rx();
    return (uart_recv());
}

/**
 ****************************************************************************************
  @brief Prints a strint to UART

 ****************************************************************************************
 */
void uart_print(char *buf)
{
    while (*buf != '\0')
        uart_putc((unsigned char) *buf++);

}

/**
 ****************************************************************************************
  @brief Prints a character to HEX

 ****************************************************************************************
 */
void uart_print_char_hex(unsigned char c)
{
    unsigned char temp;
    unsigned char buf[2];
    unsigned char i;

    buf[1] = '\0';

    for (i = 0; i < 2; i++) {
        temp = c / 16;
        if (temp < 10)
            buf[0] = temp + 48;
        else
            buf[0] = temp + 87;

        uart_print((char *)buf);

        c = c << 4;
    }
}

/**
 ****************************************************************************************
  @brief Prints HEX

 ****************************************************************************************
 */
void uart_print_hex(unsigned char *c, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        uart_print_char_hex(c[i]);
        if ((i & 0x3) == 0x3)
            uart_print(" ");
        if ((i & 0xF) == 0xF)
            uart_print("\r\n");
    }
}

/**
 ****************************************************************************************
  @brief Prints integer

 ****************************************************************************************
 */
void uart_print_int(unsigned int word)
{
    uart_print_char_hex (word >> 24);
    uart_print_char_hex (word >> 16);
    uart_print_char_hex (word >> 8);
    uart_print_char_hex (word);
}
