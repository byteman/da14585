/**
 ****************************************************************************************
 *
 * @file uart_booter.h
 *
 * @brief user booter protocol settings
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */
 
#ifndef _UART_BOOTER_H
#define _UART_BOOTER_H

#define MAX_CODE_LENGTH          0x16000
#define SYSRAM_COPY_BASE_ADDRESS 2000
#define SYSRAM_BASE_ADDRESS      0
#define SOH   0x01              
#define STX   0x02              
#define ACK   0x06              
#define NAK   0x15           

#endif
