/**
 ****************************************************************************************
 *
 * @file spi_flash_mx25.h
 *
 * @brief flash memory driver extension for Macronix devices file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _SPI_FLASH_MX_H_
#define _SPI_FLASH_MX_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "spi.h"
#include <stdint.h>

/* MX25R Commands */
#define MX25R_READ_CONFIG_REG   0x15
#define MX25R_READ_SECURITY_REG 0x2b

/* MX25R Security Register bits*/
#define MX25R_SECURITY_REG_P_FAIL 5
#define MX25R_SECURITY_REG_E_FAIL 6

/* MX25R Power Mode */
#define MX25R_LPM 0x00
#define MX25R_HPM 0x02

/**
 ****************************************************************************************
 * @brief MX25R Read Configuration Register
 * @return content of the configuration register
 ****************************************************************************************
 */
uint32_t spi_flash_MX25R_read_config_reg(void);

/**
 ****************************************************************************************
 * @brief MX25R Write Status & Config register
 * @param[in] dataToWrite:   Value to be written to Status and Configuration Register
 * @return error code or success (ERR_OK)
 ****************************************************************************************
 */
int32_t spi_flash_MX25R_write_status_config_reg(uint32_t dataToWrite);

/**
 ****************************************************************************************
 * @brief MX25R Get Power Mode
 * @return current power mode
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_get_power_mode(void);

/**
 ****************************************************************************************
 * @brief MX25R Set Power Mode
 * @param[in] powerMode: Low Power Mode (MX25R_LPM) or High Performance Mode (MX25R_HPM)
 * @return error code or success (ERR_OK)
 ****************************************************************************************
 */
 int32_t spi_flash_MX25R_set_power_mode(int8_t powerMode);

/**
 ****************************************************************************************
 * @brief MX25R Read Security Register
 * @return content of the Security Register
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_read_security_reg(void);

/**
 ****************************************************************************************
 * @brief MX25R Check P_fail bit
 * @return 1 if last page program failed, 0 otherwise
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_program_fail(void);

/**
 ****************************************************************************************
 * @brief MX25R Check E_fail bit
 * @return 1 if last erase failed, 0 otherwise
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_erase_fail(void);

#endif //_SPI_FLASH_MX25_H_
