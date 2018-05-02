/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief Programmer UART interface header file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _UART_H_
#define _UART_H_

#define UART_BAUD_9600   0
#define UART_BAUD_19200  1
#define UART_BAUD_57600  2
#define UART_BAUD_115200 3
#define UART_BAUD_1M     4

/// Divider for 1000000 bits/s
#define UART_BAUDRATE_1M            1
#define UART_FRAC_BAUDRATE_1M       0

/// Divider for 115200 bits/s
#define UART_BAUDRATE_115K2         8
#define UART_FRAC_BAUDRATE_115K2   11

/// Divider for 57600 bits/s
#define UART_BAUDRATE_57K6          17
#define UART_FRAC_BAUDRATE_57K6     6

/// Divider for 19200 bits/s
#define UART_BAUDRATE_19K2          52
#define UART_FRAC_BAUDRATE_19K2     1

/// Divider for 9600 bits/s
#define UART_BAUDRATE_9K6           104
#define UART_FRAC_BAUDRATE_9K6      3


void uart_irq_enable(void);
void uart_irq_disable(void);
void uart_send (unsigned char txdata);
unsigned char uart_recv (void);
void uart_wait_end_of_tx(void);
void uart_wait_end_of_rx(void);
void uart_pads (unsigned char sel);
void uart_init (unsigned char pad_sel, unsigned char baud_rate);

void uart_putc(unsigned char letter);
unsigned char uart_getc(void);
void uart_print(char *buf);

int uart_rx_poll(void);

void uart_print_char_hex(unsigned char c);
void uart_print_hex(unsigned char *c, int len);
void uart_print_int(unsigned int word) ;
#endif
