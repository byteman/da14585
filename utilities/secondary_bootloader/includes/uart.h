/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief uart initialization and print functions header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include <stdint.h>

#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/// Divider for 115200 bits/s
#define UART_BAUDRATE_115K2         8
#define UART_FRAC_BAUDRATE_115K2   11

/// Divider for 57600 bits/s
#define UART_BAUDRATE_57K6          17
#define UART_FRAC_BAUDRATE_57K6     6

/// Divider for 38400 bits/s
#define UART_BAUDRATE_38K4          26
#define UART_FRAC_BAUDRATE_38K4     1

/// Divider for 28800 bits/s
#define UART_BAUDRATE_28K8          34
#define UART_FRAC_BAUDRATE_28K8     12

/// Divider for 19200 bits/s
#define UART_BAUDRATE_19K2          52
#define UART_FRAC_BAUDRATE_19K2     1

/// Divider for 14400 bits/s
#define UART_BAUDRATE_14K4          69
#define UART_FRAC_BAUDRATE_14K4     7

/// Divider for 9600 bits/s
#define UART_BAUDRATE_9K6           104
#define UART_FRAC_BAUDRATE_9K6      3

#define UART_FIFO_EN 1
#define UART_RX_FIFO_RESET 2
#define UART_TX_FIFO_RESET 4

void uart_initialization(void);
void uart_release(void);

char uart_receive_byte(char *ch);
void uart_send_byte(char ch);
 
  
#endif
